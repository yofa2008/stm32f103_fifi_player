#ifndef __OOPY_COMMON_H__
#define __OOPY_COMMON_H__

#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "mytype.h"
#include "stm32f10x_lib.h"
#include "sdcard.h"
#include "fatfs\ff.h"
#include "fatfs\diskio.h"
#include "tft\tft_otm3225.h"
#include "tft\lcd_printf.h"

//20171103 add by Jin
#define DEBUG

#include "LED.h"
#include "debug.h"

#define RCC_PLL_HSE

#define RCC_PLL_CLOCKMUL 6

#define SDCARD_MODE_DMA 1
#define SDCARD_MODE_INT 0
#define SDCARD_MODE_POL 2

#define SD_DMA_CLKDIV ((u8)0x08)
#define SD_INT_CLKDIV ((u8)0x10)
#define SD_POL_CLKDIV ((u8)0x10)

#define FLAC_MAX_BLOCKSIZE	(1152)
#define FLAC_MAX_FRAMESIZE	(5*1024)
#define PCM_BUFFER_LEN (1024)			//缓冲区长度（U16)
#define PCM_BUFFER_NUM (6)				//缓冲区个数

struct PCMBUF
{
unsigned short buf[PCM_BUFFER_LEN];
unsigned short size;
unsigned short ptr;
};

typedef struct PCMBUF PCMBUF;

extern PCMBUF pcm_buf[PCM_BUFFER_NUM];

extern unsigned char now_buf;
extern unsigned short pcm_send_data;
extern unsigned char overflow;
extern volatile int sync_lrck,mono;

#define REPEAT_MODE_ALL 0
#define REPEAT_MODE_DIR 1
#define REPEAT_MODE_ONE 2
#define REPEAT_MODE_RND 3
#define REPEAT_MODE_LST 4

#define PAUSE_DISP_TIME (10000*RCC_PLL_CLOCKMUL)

void SPI1_GPIOA6_Reset(void);

#define ENABLE_AUDIO  {SPI1_Config();SPI_Cmd(SPI1,ENABLE);}
#define DISABLE_AUDIO {SPI_Cmd(SPI1,DISABLE);SPI1_GPIOA6_Reset();}

extern unsigned short fnum,dirnum;//file num,dir num
extern unsigned int mode;
extern unsigned short Dir_filesnum[],random_list[];

extern unsigned short random_index,random_max,random_fnum,listnum,maxlist,random_dir;
extern volatile unsigned int key_left,key_left_long,key_left_push;
extern volatile unsigned int key_right,key_right_long,key_right_push;
extern volatile unsigned int key_play,key_play_long,key_play_push;
extern volatile unsigned int key_up,key_up_long,key_up_push;
extern volatile unsigned int key_down,key_down_long,key_down_push;
extern const unsigned short icon_music_red[],icon_music_blue[],icon_folder[],heart_blue[],heart_red[];
extern volatile unsigned int counter_100hz,msec2,msec;
extern WCHAR Lfn_Unicode[];

#define TIM_DIS_LINE  8
#define FMT_DIS_LINE  9


/* Function prototypes -----------------------------------------------*/
void delay_ms(unsigned long ms);
void Delay_10Ms(unsigned long delay);
void ClearKey(void);
void RCC_Config(void);
void Timer_Config(u16 arr,u16 psc);
void NVIC_Config(void);
void ADC_Config(void);
void Key_Config(void);	
void I2S_Config(void);
void SPI1_Config(void);
void SPI2_Config(void);
void I2s_SetFreq(unsigned short freq);

int playFLAC(char * filename);
int playWAV(char * filename);
//int playMP3(char * filename);


#endif
