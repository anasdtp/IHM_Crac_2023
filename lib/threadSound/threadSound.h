#ifndef __THREAD_SOUND_H
#define __THREAD_SOUND_H

#include "mbed.h"
#include "mp3dec.h"

extern "C" {
    void BSP_AUDIO_OUT_TransferComplete_CallBack(void);
    void BSP_AUDIO_OUT_HalfTransfer_CallBack(void);
    void BSP_AUDIO_OUT_Error_CallBack(void);
}

class ThreadSound {
   public:
    enum ErrorSound {
        ERROR_RESSOURCE_USED = -1,
        ERROR_FILE_NOT_FOUND = -2,

        NO_ERROR = 0
    };

    ~ThreadSound();
    static ErrorSound playMp3(const char *file, uint8_t volume = 75);

   private:
    // Unique instance de la classe
    static ThreadSound *const threadSound;
    // Permet d'interdire la création d'autres instances
    ThreadSound();
    ThreadSound(ThreadSound &other) {}
    ThreadSound &operator=(ThreadSound &other) {  return *this;  }

    enum FlagSound {
        FLAG_MP3_DECODER_BUFFER0_READY = 1,
        FLAG_MP3_DECODER_BUFFER1_READY = 2,
        FLAG_MP3_DECODER_READ_FINISH = 4,
        FLAG_PLAY_BUFFER0_RELEASE = 8,
        FLAG_PLAY_BUFFER1_RELEASE = 16,
        FLAG_HALF_BUFFER = 32,
        FLAG_FULL_BUFFER = 64,
        FLAG_DMA_ERROR = 128,
        FLAG_IS_PLAYING = 256,

        FLAG_FORCE_TERMINATE = 512
    };

    static EventFlags m_flags;
    static Thread *m_mp3Decoder;
    static Thread *m_playSound;
    static FILE *m_infile;
    static int m_sampleRate;
    static uint8_t m_volume;
    static bool m_mono;
    
    static HMP3Decoder m_hMP3Decoder;
    static short m_outBuf[2][MAX_NCHAN * MAX_NGRAN * MAX_NSAMP];
    static uint16_t m_nbDatas[2];

    static void runMp3Decoder();
    static void runPlaySound();

    static int fillReadBuffer(unsigned char *readBuf, unsigned char *readPtr, int bufSize, int bytesLeft);
    uint8_t playSoundProcess();

    friend void BSP_AUDIO_OUT_TransferComplete_CallBack(void);
    friend void BSP_AUDIO_OUT_HalfTransfer_CallBack(void);
    friend void BSP_AUDIO_OUT_Error_CallBack(void);
};

#endif
