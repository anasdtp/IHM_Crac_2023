#include "threadSound.h"

#include <Drivers/STM32469I-Discovery/stm32469i_discovery_audio.h>

// Création des membres statiques
EventFlags ThreadSound::m_flags;
Thread *ThreadSound::m_mp3Decoder = nullptr;
Thread *ThreadSound::m_playSound = nullptr;
FILE *ThreadSound::m_infile = nullptr;
HMP3Decoder ThreadSound::m_hMP3Decoder;
short ThreadSound::m_outBuf[2][MAX_NCHAN * MAX_NGRAN * MAX_NSAMP];
uint16_t ThreadSound::m_nbDatas[2];
int ThreadSound::m_sampleRate = 44100;
uint8_t ThreadSound::m_volume = 80;
bool ThreadSound::m_mono = false;

// Création de l'instance unique de la classe
ThreadSound *const ThreadSound::threadSound = new ThreadSound();

ThreadSound::ThreadSound() {
}

ThreadSound::~ThreadSound() {
    if (m_mp3Decoder) {
        m_playSound->terminate();
        m_mp3Decoder->terminate();
        m_playSound->join();
        m_mp3Decoder->join();
        delete m_playSound;
        m_playSound = nullptr;
        delete m_mp3Decoder;
        m_mp3Decoder = nullptr;
    }
    if (m_infile) {
        fclose(m_infile);
        m_infile = nullptr;
    }
}

ThreadSound::ErrorSound ThreadSound::playMp3(const char *file, uint8_t volume) {

    ErrorSound error;
    // Teste si la ressource est disponible
    CriticalSectionLock::enable();
    if (m_flags.get() & FLAG_IS_PLAYING) {
        // Ressource occupée
        error = ERROR_RESSOURCE_USED;
    } else {
        // Ressource disponible
        error = NO_ERROR;
        // Prend la ressource
        m_flags.set(FLAG_IS_PLAYING);
    }
    CriticalSectionLock::disable();

    if (error != NO_ERROR) return error;

    m_flags.clear();
    m_volume = volume;
    m_infile = fopen(file, "rb");
    if (!m_infile) {
        return ERROR_FILE_NOT_FOUND;
    }
    m_mp3Decoder = new Thread;
    m_mp3Decoder->start(callback(ThreadSound::runMp3Decoder));
    m_playSound = new Thread;
    m_playSound->start(callback(ThreadSound::runPlaySound));
    return NO_ERROR;
}

#define READBUF_SIZE (1024 * 16) /* feel free to change this, but keep big enough for >= one frame at high bitrates */

int ThreadSound::fillReadBuffer(unsigned char *readBuf, unsigned char *readPtr, int bufSize, int bytesLeft) {
    int nRead;

    /* move last, small chunk from end of buffer to start, then fill with new data */
    memmove(readBuf, readPtr, bytesLeft);
    nRead = fread(readBuf + bytesLeft, 1, bufSize - bytesLeft, m_infile);
    /* zero-pad to avoid finding false sync word after last frame (from old data in readBuf) */
    if (nRead < bufSize - bytesLeft)
        memset(readBuf + bytesLeft + nRead, 0, bufSize - bytesLeft - nRead);

    return nRead;
}

void ThreadSound::runMp3Decoder() {
    static unsigned char readBuf[READBUF_SIZE];
    int bytesLeft = 0;
    int outOfData = 0;
    int eofReached = 0;
    unsigned char *readPtr = readBuf;
    int nRead = 0;
    int nFrames = 0;
    int offset;
    int err;
    MP3FrameInfo mp3FrameInfo;
    int indexOutBuf = 0;
    short *outBuf = m_outBuf[0];

    if ((m_hMP3Decoder = MP3InitDecoder()) == 0) {
        return;
    }

    m_flags.set(FLAG_PLAY_BUFFER1_RELEASE);

    do {
        /* somewhat arbitrary trigger to refill buffer - should always be enough for a full frame */
        if (bytesLeft < 2 * MAINBUF_SIZE && !eofReached) {
            nRead = fillReadBuffer(readBuf, readPtr, READBUF_SIZE, bytesLeft);
            bytesLeft += nRead;
            readPtr = readBuf;
            if (nRead == 0)
                eofReached = 1;
        }

        /* find start of next MP3 frame - assume EOF if no sync found */
        offset = MP3FindSyncWord(readPtr, bytesLeft);
        if (offset < 0) {
            outOfData = 1;
            break;
        }
        readPtr += offset;
        bytesLeft -= offset;

        /* decode one MP3 frame - if offset < 0 then bytesLeft was less than a full frame */
        err = MP3Decode(m_hMP3Decoder, &readPtr, &bytesLeft, outBuf, 0);
        nFrames++;

        if (err) {
            /* error occurred */
            switch (err) {
                case ERR_MP3_INDATA_UNDERFLOW:
                    outOfData = 1;
                    break;
                case ERR_MP3_MAINDATA_UNDERFLOW:
                    /* do nothing - next call to decode will provide more mainData */
                    break;
                case ERR_MP3_FREE_BITRATE_SYNC:
                default:
                    outOfData = 1;
                    break;
            }
        } else {
            /* no error */
            MP3GetLastFrameInfo(m_hMP3Decoder, &mp3FrameInfo);
            m_sampleRate = mp3FrameInfo.samprate;
            m_mono = mp3FrameInfo.nChans == 1;
            m_nbDatas[indexOutBuf] = mp3FrameInfo.bitsPerSample * mp3FrameInfo.outputSamps / 8;
            if (indexOutBuf == 0) {
                indexOutBuf = 1;
                m_flags.set(FLAG_MP3_DECODER_BUFFER0_READY);
                m_flags.wait_all(FLAG_PLAY_BUFFER1_RELEASE);
            } else {
                indexOutBuf = 0;
                m_flags.set(FLAG_MP3_DECODER_BUFFER1_READY);
                m_flags.wait_all(FLAG_PLAY_BUFFER0_RELEASE);
            }
            outBuf = m_outBuf[indexOutBuf];
        }

    } while (!outOfData);

    MP3FreeDecoder(m_hMP3Decoder);
    m_hMP3Decoder = nullptr;

    fclose(m_infile);
    m_infile = nullptr;

    printf("Décodage réussi\n");
}

void ThreadSound::runPlaySound() {
    int nbTotalDatas = 0;

    if (!(m_flags.wait_all(FLAG_MP3_DECODER_BUFFER0_READY, 100) & FLAG_MP3_DECODER_BUFFER0_READY)) {
        // erreur ou fin
        return;
    }

    if (BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_BOTH, m_volume, m_sampleRate) == 0) {
        printf("AUDIO CODEC OK\n");
    } else {
        printf("AUDIO CODEC FAIL\nTry to reset board\n");
        return;
    }

    if (m_mono) BSP_AUDIO_OUT_ChangeAudioConfig(BSP_AUDIO_OUT_CIRCULARMODE | BSP_AUDIO_OUT_MONOMODE);

    /*
    Start playing the file from a circular buffer, once the DMA is enabled, it is
    always in running state. Application has to fill the buffer with the audio data
    using Transfer complete and/or half transfer complete interrupts callbacks
    (BSP_AUDIO_OUT_TransferComplete_CallBack() or BSP_AUDIO_OUT_HalfTransfer_CallBack()...
    */

    BSP_AUDIO_OUT_Play((uint16_t *)m_outBuf[0], m_nbDatas[0]*2);
    int datas = m_nbDatas[0];

    while (1) {
        nbTotalDatas++;
        m_flags.wait_all(FLAG_HALF_BUFFER);
        m_flags.set(FLAG_PLAY_BUFFER0_RELEASE);
        if (!(m_flags.wait_all(FLAG_MP3_DECODER_BUFFER1_READY, 0) & FLAG_MP3_DECODER_BUFFER1_READY)) {
            break;
        }
        nbTotalDatas++;
        m_flags.wait_all(FLAG_FULL_BUFFER);
        m_flags.set(FLAG_PLAY_BUFFER1_RELEASE);
        if (!(m_flags.wait_all(FLAG_MP3_DECODER_BUFFER0_READY, 0) & FLAG_MP3_DECODER_BUFFER0_READY)) {
            break;
        }
    }
    BSP_AUDIO_OUT_Stop(CODEC_PDWN_HW);

    printf("Nb Datas %d x %d = %d\n", nbTotalDatas, datas, nbTotalDatas * datas);

    // Libère la ressource
    m_flags.clear(FLAG_IS_PLAYING);
}

/*------------------------------------------------------------------------------
       Callbacks implementation:
           the callbacks API are defined __weak in the stm32769i_discovery_audio.c file
           and their implementation should be done the user code if they are needed.
           Below some examples of callback implementations.
  ----------------------------------------------------------------------------*/
/**
 * @brief  Manages the full Transfer complete event.
 * @param  None
 * @retval None
 */
void BSP_AUDIO_OUT_TransferComplete_CallBack(void) {
    ThreadSound::m_flags.set(ThreadSound::FLAG_FULL_BUFFER);
    //   if (audio_state == AUDIO_STATE_PLAYING)
    //   {
    //     /* allows AUDIO_Process() to refill 2nd part of the buffer  */
    //     buffer_ctl.state = BUFFER_OFFSET_FULL;
    //   }
}

/**
 * @brief  Manages the DMA Half Transfer complete event.
 * @param  None
 * @retval None
 */
void BSP_AUDIO_OUT_HalfTransfer_CallBack(void) {
    ThreadSound::m_flags.set(ThreadSound::FLAG_HALF_BUFFER);
    //   if (audio_state == AUDIO_STATE_PLAYING)
    //   {
    //     /* allows AUDIO_Process() to refill 1st part of the buffer  */
    //     buffer_ctl.state = BUFFER_OFFSET_HALF;
    //   }
}

// /**
//  * @brief  Manages the DMA FIFO error event.
//  * @param  None
//  * @retval None
//  */
void BSP_AUDIO_OUT_Error_CallBack(void)
{
    ThreadSound::m_flags.set(ThreadSound::FLAG_DMA_ERROR);
//   /* Display message on the LCD screen */
//   printf("DMA  ERROR\n");

//   /* Stop the program with an infinite loop */
//   while (1)
//     ;

//   /* could also generate a system reset to recover from the error */
//   /* .... */
}
