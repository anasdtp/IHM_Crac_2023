/**
 ******************************************************************************
 * @file    BSP/Src/audio_play.c
 * @author  MCD Application Team
 * @brief   This example code shows how to use the audio feature in the
 *          stm32f769i_discovery driver
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2016 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <mbed.h>
#include <Drivers/STM32469I-Discovery/stm32469i_discovery_audio.h>
#include <audioDemo.h>

#include <sstream>
#include <fstream>

#include <sound.h>

static int indexSound = 0;
const int sizeSound = 204800;

/** @addtogroup STM32F7xx_HAL_Examples
 * @{
 */

/** @addtogroup BSP
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  AUDIO_ERROR_NONE = 0,
  AUDIO_ERROR_NOTREADY,
  AUDIO_ERROR_IO,
  AUDIO_ERROR_EOF,
} AUDIO_ErrorTypeDef;

/* The Audio file is flashed with ST-Link Utility @ flash address =  AUDIO_SRC_FILE_ADDRESS */
// #define AUDIO_SRC_FILE_ADDRESS       0x08080000   /* Audio file address in flash */
// #define AUDIO_FILE_SIZE              0x80000

/* Private define ------------------------------------------------------------*/

/*Since SysTick is set to 1ms (unless to set it quicker) */
/* to run up to 48khz, a buffer around 1000 (or more) is requested*/
/* to run up to 96khz, a buffer around 2000 (or more) is requested*/
#define AUDIO_DEFAULT_VOLUME 70

/* Audio file size and start address are defined here since the audio file is
   stored in Flash memory as a constant table of 16-bit data */
#define AUDIO_START_OFFSET_ADDRESS 0 /* Offset relative to audio file header size */
#define AUDIO_BUFFER_SIZE 2048
/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  AUDIO_STATE_IDLE = 0,
  AUDIO_STATE_INIT,
  AUDIO_STATE_PLAYING,
  AUDIO_STATE_FIN1,
  AUDIO_STATE_FIN2
} AUDIO_PLAYBACK_StateTypeDef;

typedef enum
{
  BUFFER_OFFSET_NONE = 0,
  BUFFER_OFFSET_HALF,
  BUFFER_OFFSET_FULL,
} BUFFER_StateTypeDef;

typedef struct
{
  uint8_t buff[AUDIO_BUFFER_SIZE];
  uint32_t fptr;
  volatile BUFFER_StateTypeDef state;
} AUDIO_BufferTypeDef;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static AUDIO_BufferTypeDef buffer_ctl;
static AUDIO_PLAYBACK_StateTypeDef audio_state;
__IO uint32_t uwVolume = 20;
__IO uint32_t uwPauseEnabledStatus = 0;

static uint32_t AudioFreq[1] =
    {
        96000};

/* Private function prototypes -----------------------------------------------*/
static uint32_t GetData(uint8_t *pbuf, uint32_t NbrOfData);
AUDIO_ErrorTypeDef AUDIO_Start();

/* Private functions ---------------------------------------------------------*/
uint8_t AUDIO_Process(void);

DigitalOut hp(PE_3, 0);

/**
 * @brief  Audio Play demo
 * @param  None
 * @retval None
 */
void AudioPlay_demo(void)
{
  uint32_t *AudioFreq_ptr;
  AudioFreq_ptr = AudioFreq; /*AF_48K*/

  uwVolume = 60;

  /*  if(BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE1, uwVolume, *AudioFreq_ptr) == 0)
  if(BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE2, uwVolume, *AudioFreq_ptr) == 0)*/
  if (BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_BOTH, uwVolume, *AudioFreq_ptr) == 0)
  {
    printf("AUDIO CODEC OK\n");
  }
  else
  {
    printf("AUDIO CODEC FAIL\nTry to reset board\n");
  }

  /* Play audio on slots 1 and 3 */
  BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_0123);

  /*
  Start playing the file from a circular buffer, once the DMA is enabled, it is
  always in running state. Application has to fill the buffer with the audio data
  using Transfer complete and/or half transfer complete interrupts callbacks
  (BSP_AUDIO_OUT_TransferComplete_CallBack() or BSP_AUDIO_OUT_HalfTransfer_CallBack()...
  */
  AUDIO_Start();

  /* Display the state on the screen */
  printf("PLAYING...\n");

  printf("VOL:    %3ld\n", uwVolume);

  printf("FREQ: %6ld\n", *AudioFreq_ptr);
  printf("Hear nothing ? Have you copied the audio file with STM-LINK UTILITY ?\n");

  /* Infinite loop */
  while (AUDIO_Process() != AUDIO_ERROR_EOF);

    // /* Get the TouchScreen State */
    // ts_action = (TS_ActionTypeDef) TouchScreen_GetTouchPosition();

    // switch (ts_action)
    // {
    // case TS_ACT_VOLUME_UP:
    //   HAL_Delay(50);
    //   /* Increase volume by 5% */
    //   if (uwVolume < 95)
    //     uwVolume += 5;
    //   else
    //     uwVolume = 100;
    //   sprintf((char*)volume_str, "       VOL:    %3ld     ", uwVolume);
    //   BSP_AUDIO_OUT_SetVolume(uwVolume);
    //   BSP_LCD_DisplayStringAt(0, LINE(9), (uint8_t *)volume_str, CENTER_MODE);
    //   break;
    // case TS_ACT_VOLUME_DOWN:
    //   HAL_Delay(50);
    //   /* Decrease volume by 5% */
    //   if (uwVolume > 5)
    //     uwVolume -= 5;
    //   else
    //     uwVolume = 0;
    //   sprintf((char*)volume_str, "       VOL:    %3ld     ", uwVolume);
    //   BSP_AUDIO_OUT_SetVolume(uwVolume);
    //   BSP_LCD_DisplayStringAt(0, LINE(9), (uint8_t *)volume_str, CENTER_MODE);
    //   break;
    // case TS_ACT_PAUSE:
    //   /* Set Pause / Resume */
    //   if (uwPauseEnabledStatus == 1)
    //   { /* Pause is enabled, call Resume */
    //     BSP_AUDIO_OUT_Resume();
    //     uwPauseEnabledStatus = 0;
    //     BSP_LCD_DisplayStringAt(0, LINE(8), (uint8_t *)"       PLAYING...     ", CENTER_MODE);
    //     BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    //     BSP_LCD_FillPolygon(Points2, 3);
    //     BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    //     BSP_LCD_FillRect(100, 140, 25 , 80);
    //     BSP_LCD_FillRect(140, 140, 25 , 80);
    //   }
    //   else
    //   { /* Pause the playback */
    //     BSP_AUDIO_OUT_Pause();
    //     uwPauseEnabledStatus = 1;
    //     BSP_LCD_DisplayStringAt(0, LINE(8), (uint8_t *)"       PAUSE  ...     ", CENTER_MODE);
    //     BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    //     BSP_LCD_FillRect(100, 140, 80 , 80);
    //     BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    //     BSP_LCD_FillPolygon(Points2, 3);
    //   }
    //   HAL_Delay(200);
    //   break;

    // default:
    //   break;
    // }

    // if (CheckForUserInput() > 0)
    // {
    //   BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
    //   return;
    // }
}

extern ifstream audioFile;

bool ouvertureFichierAudio();

ifstream audioFile;

bool ouvertureFichierAudio()
{
  printf("Essai ouverture fichier\n");
  audioFile.open("/sd/test.bin", ios::binary);
  if (!audioFile.is_open())
  {
    printf("Fichier impossible à ouvrir\n");
    return true;
  }
  printf("Fichier ouvert\n");
  return true;
}

/**
 * @brief  Starts Audio streaming.
 * @param  None
 * @retval Audio error
 */
AUDIO_ErrorTypeDef AUDIO_Start()
{
  // if (!ouvertureFichierAudio())
  //   return AUDIO_ERROR_EOF;
  // uint32_t bytesread;

  // buffer_ctl.state = BUFFER_OFFSET_NONE;

  // bytesread = GetData(&buffer_ctl.buff[0],
  //                     AUDIO_BUFFER_SIZE);
  // printf("Start - Lecture de %u octets\n", bytesread);                      
  // if (bytesread > 0)
  // {
  //   BSP_AUDIO_OUT_Play((uint16_t *)&buffer_ctl.buff[0], AUDIO_BUFFER_SIZE);
  //   audio_state = AUDIO_STATE_PLAYING;
  //   buffer_ctl.fptr = bytesread;
  //   return AUDIO_ERROR_NONE;
  // }
  // return AUDIO_ERROR_IO;

  uint32_t bytesread;

  buffer_ctl.state = BUFFER_OFFSET_NONE;

  bytesread = GetData(&buffer_ctl.buff[0],
                      AUDIO_BUFFER_SIZE);
  printf("Start - Lecture de %u octets\n", bytesread);                      
  if (bytesread > 0)
  {
    BSP_AUDIO_OUT_Play((uint16_t *)&buffer_ctl.buff[0], AUDIO_BUFFER_SIZE);
    audio_state = AUDIO_STATE_PLAYING;
    buffer_ctl.fptr = bytesread;
    return AUDIO_ERROR_NONE;
  }
  return AUDIO_ERROR_IO;
}

/**
 * @brief  Manages Audio process.
 * @param  None
 * @retval Audio error
 */
uint8_t AUDIO_Process(void)
{
  uint32_t bytesread;
  AUDIO_ErrorTypeDef error_state = AUDIO_ERROR_NONE;

  switch (audio_state)
  {
  case AUDIO_STATE_PLAYING:

    /* 1st half buffer played; so fill it and continue playing from bottom*/
    if (buffer_ctl.state == BUFFER_OFFSET_HALF)
    {
      bytesread = GetData(&buffer_ctl.buff[0],
                          AUDIO_BUFFER_SIZE / 2);

      if (bytesread > 0)
      {
        buffer_ctl.state = BUFFER_OFFSET_NONE;
        buffer_ctl.fptr += bytesread;
      }
      if (bytesread != AUDIO_BUFFER_SIZE / 2)
        audio_state = AUDIO_STATE_FIN1;
      if (bytesread == 0)
        audio_state = AUDIO_STATE_FIN2;
    }

    /* 2nd half buffer played; so fill it and continue playing from top */
    if (buffer_ctl.state == BUFFER_OFFSET_FULL)
    {
      bytesread = GetData(&buffer_ctl.buff[AUDIO_BUFFER_SIZE / 2],
                          AUDIO_BUFFER_SIZE / 2);
      if (bytesread > 0)
      {
        buffer_ctl.state = BUFFER_OFFSET_NONE;
        buffer_ctl.fptr += bytesread;
      }
      if (bytesread != AUDIO_BUFFER_SIZE / 2)
        audio_state = AUDIO_STATE_FIN1;
      if (bytesread == 0)
        audio_state = AUDIO_STATE_FIN2;
    }
    break;
  case AUDIO_STATE_FIN1:
    if ((buffer_ctl.state == BUFFER_OFFSET_HALF) || (buffer_ctl.state == BUFFER_OFFSET_FULL))
    {
      audio_state = AUDIO_STATE_FIN2;
    }
    break;
  case AUDIO_STATE_FIN2:
    BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
    audioFile.close();
    error_state = AUDIO_ERROR_EOF;
    break;
  default:
    error_state = AUDIO_ERROR_NOTREADY;
    break;
  }
  return (uint8_t)error_state;
}

/**
 * @brief  Gets Data from storage unit.
 * @param  None
 * @retval None
 */
static uint32_t GetData(uint8_t *pbuf, uint32_t NbrOfData)
{
  static int nb = 0;
  uint32_t i;
  for (i=0; i<NbrOfData; i++) {
    if (indexSound >= sizeSound) break;
    pbuf[i] = (uint8_t)sound[indexSound++];
  }
  printf("%5d GetDatas\n", nb++);
  return i;

//  return audioFile.readsome((char *)pbuf, NbrOfData);
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
void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{
  if (audio_state == AUDIO_STATE_PLAYING)
  {
    /* allows AUDIO_Process() to refill 2nd part of the buffer  */
    buffer_ctl.state = BUFFER_OFFSET_FULL;
  }
}

/**
 * @brief  Manages the DMA Half Transfer complete event.
 * @param  None
 * @retval None
 */
void BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
{
  if (audio_state == AUDIO_STATE_PLAYING)
  {
    /* allows AUDIO_Process() to refill 1st part of the buffer  */
    buffer_ctl.state = BUFFER_OFFSET_HALF;
  }
}

/**
 * @brief  Manages the DMA FIFO error event.
 * @param  None
 * @retval None
 */
void BSP_AUDIO_OUT_Error_CallBack(void)
{
  /* Display message on the LCD screen */
  printf("DMA  ERROR\n");

  /* Stop the program with an infinite loop */
  while (1)
    ;

  /* could also generate a system reset to recover from the error */
  /* .... */
}

