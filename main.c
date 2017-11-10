/*********************************************************************************
OOPY MUSIC PLAYER
Copyright (C) 2011, SHARPUFO.HuangRongBin 

OOPY MUSIC PLAYER ��һ��������ֲ�������������STM32F103ZE΢�������ϡ�
֧��44.1K 16bit �ĵ��������������� WAV�ļ��Լ� LV0,LV1,LV2�� FLAC�ļ���
����Ӳ�����������·ԭ��ͼ.

��������ԭ������֮Ȩ��,ֻ�������ҵ�ԡ���Ӯ����ʹ��!
����Ը������ʡ���Ӫ���ԡ�����ҵ�Ե�ʹ�á����ĺʹ��������������롣
������������������û���κ������ά���ͼ���֧�֣�Ҳ���е��κ�ֱ�ӻ��߼�ӵ�
����ʹ�õ�����������������µĺ���ͷ���!
�κεĺ���ͷ��ս���ʹ�������ге��������ʹ��!

������������ʹ�õ����������������Ȩ��ԭ�������У�����ѭ���Ȩ����!

������������ʹ�õ���FLAC�������ģ��������K9spud LLC��TRAXMOD��
�������Ĳ��Ŵ����������ڱ��������Ĵ���޸�,�������˿���Ϳ��˵Ĺ��ܡ�
�ڴ���ԭ����K9spud LLC��л��������ѭ���Ȩ����!

������������ʹ�õ�ChaN��FATFS R0.07E �ļ�ϵͳ����ģ�飬������С���޸��������ڱ���������
�ڴ���ԭ����ChaN��л��������ѭ���Ȩ����!

������������ʹ�õ�STM32 V2.0.3�Ŀ���룬���������޸ģ�
�ڴ���ԭ����MCD Application Team��л��������ѭ���Ȩ����!

�����������������ʹ�õ����Ĵ��벿�ֲ�����������δ�г�,��֪ͨ���˸�����

//OOPY MUSIC PLAYER
//Copyright (C) 2011, SHARPUFO.HuangRongBin
OOPY MUSIC PLAYER is a free music player code,which runs on a STM32F103ZE controller.
The player supports 44.1K 16bit mono or stereo WAV files and LV0,LV1,LV2 FLAC files playback.

OOPY MUSIC PLAYER is a free music player code,and there is NO WARRANTY.
No restriction on use. You can use, modify and redistribute it for
personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
Redistributions of source code must retain this copyright notice.

///////////////////////////////////////////////////////////////////////////////////////////
//Oopy music player use the flac decoding module from	TRAXMOD which which is 
//Copyright (c) 2009, K9spud LLC.
// http://www.k9spud.com/traxmod/
//
// Largely based on main.c from the Rockbox FLAC test decoder program, which is
// Copyright (c) 2005, Dave Chapman
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////
// And the playback code was largely modified to fit into OOPY player. 
///////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////
Oopy music player uses the FATFS R0.07E File System module from TRAXMOD which is
/  Copyright (C) 2009, ChaN, all right reserved.
/
/ * The FatFs module is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
///////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////
Oopy music player uses the STM32 LIB CODE V2.0.3 which is
******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
///////////////////////////////////////////////////////////////////////////////////////////



********************************************************************************/

/*
PS:
The SDIO write operation in this project has not working well yet.
And I have no  and won't have time to see into it. :)
*/
/********************************************************************************/

/*ALL RIGHT OF ORIGINAL PIECE RESERVED,USE THE CODE UNDER YOUR RESPONSIBILITY!*/

/********************COPYRIGHT 2010 Sharpufo.HuangRongBin ***************************
* File Name          : main.c
* Author             : Sharpufo.HuangRongBin
* Version            : V 0.0.3
* Date               : 09/04/2011
* Description        : OOPY MUSIC PLAYER MAIN FILE
* Support Format	 : 44K 16BIT Stereo and mono WAV(uncompressed) and LEVEL(0-2) FLAC
********************************************************************************/

/*Log*/
/*
*R0.0.1 First released 29/03/2011

*R0.0.2 Add mono support.Correct some type false and fix some bugs .

*R0.0.3 Use the flash room of the mcu (about 87k bytes) to store the unicode to GBK convert table to fast track and album selection.
*/


//* Includes ------------------------------------------------------------------*/
#include "common.h"

/* Private macro -------------------------------------------------------------*/
#define SORT_BY_LFN	//�Ƿ񰴳��ļ������򣬷����ö��ļ�������

#define MAX_ITEM (320/16-2)
#define MAX_TRACK_ITEM (MAX_ITEM-2)

#ifdef SORT_BY_LFN
	#define SORT_LFN_LEN 7			//�ļ�������ʱ�õ��ĳ��ļ�������
	#define MAXFILE 200			 	//ÿ���ļ��������ܶ�ȡ�����Ŀɲ��������ļ���Ŀ
	#define MAXDIR	100			 	//���ļ������ܶ�ȡ���������ļ�����Ŀ(�ļ��������ļ�����Ϊ��Ŷ�ȡ���ļ��У�
 
	char Filename[MAXFILE+1][13];	 	//��ǰ�ļ����ж�ȡ�������пɲ��������ļ��Ķ��ļ���
	WORD LfnName[MAXFILE+1][SORT_LFN_LEN];//��ǰ�ļ����ж�ȡ��������(<MAXFILE)�ɲ��������ļ��ĳ��ļ���(ǰ�����ַ�),��������

#define MAX_RANDOM_LIST 800

#else
	#define MAXFILE 300			 	//ÿ���ļ��������ܶ�ȡ������wav�ļ���Ŀ
	#define MAXDIR	80			 	//���ļ������ܶ�ȡ���������ļ�����Ŀ(�ļ���wav�ļ�����Ϊ��Ŷ�ȡ���ļ���
	char Filename[MAXFILE+1][13];	//��ǰ�ļ����ж�ȡ��������(<MAXFILE)�ɲ��������ļ��Ķ��ļ���,��������

	#define MAX_RANDOM_LIST 1000
#endif

/*Global variables ---------------------------------------------------------*/

int mda_or_interrupt=0;

/*File System Object*/
  	FATFS fs;
	DIR dir;
	 
  	FIL file1;

	FIL f_font;//�ֿ��ļ�
	FIL f_u2gb;//unicode ת gbk ���ձ� �ļ�
	u8 font_on;//�ֿ��ļ��Ƿ��Ѿ��ɹ���
	u8 u2gb_on;//���ձ��ļ��Ƿ��Ѿ��ɹ���

	FRESULT fres;
	FILINFO f_info;

/*---------------------------------------------------------*/

WCHAR CurrentDirLfnUnicode[_MAX_LFN];//��ǰ�ļ��г��ļ���
WCHAR SelectLfnUnicode[MAX_ITEM][_MAX_LFN];//�ļ�ѡ��ʱ����ĵ�ǰѡ����ļ��ĳ��ļ�����Unicode

unsigned char tempstring[60];

unsigned short tracknum=0,dirnum=0,listnum,maxlist;
unsigned short maxtracks,maxdir,total_music_files;

WORD FileIndex[MAXFILE+1];
char Dirname[MAXDIR+1][13];		 	//���ļ����������һ�����ļ��еĶ��ļ�����������
WORD DirIndex[MAXDIR+1];				//�ļ�������ļ��Ķ�ȡ���
unsigned short DirFilesNum[MAXDIR+1]; //���ļ����������һ�����ļ���������������������ļ���)�����������ļ�����Ŀ

unsigned short
			random_list[MAX_RANDOM_LIST+1],//��������б�
			random_index,//��ǰ����б��ָ��
			random_max,	//����б���
			random_dir,  //����б��Ԫ��ת��Ϊ�ڼ����ļ���
			random_tracknum;	//����б��Ԫ��ת��Ϊ���ļ��еڼ����ļ����

unsigned int mode;//repeat mode

PCMBUF pcm_buf[PCM_BUFFER_NUM];	//audio transfer buffer
unsigned char 	now_buf=0,	//which one buffer now transfer,
			 	overflow=0;//transfer buffer overflow flag.


/*prototype*/
void Get_Dir_LFN (void);

/*Executed Body-----------------------------------------------*/


void delay_ms(unsigned long ms)
{
volatile long i;
while(ms--)
#ifdef RCC_PLL_HSE
	for(i=0;i<10000*RCC_PLL_CLOCKMUL/4;i++);
#else
	for(i=0;i<10000;i++);
#endif
}


void Delay_10Ms(unsigned long delay)
{
counter_100hz=delay;
while(counter_100hz);
}


/////////////////////////////////////////////////////////////
int IsFlacName(char * fname)
{
char f[13];U8 p=0;
memset(f,0,13);
strncpy(f,fname,13);
while(f[p]!=0){p++;}
if(p>4)p-=4;
if(f[p]=='.'&&(f[p+1]=='f'||f[p+1]=='F')&& (f[p+2]=='l'|| f[p+2]=='L')&& (f[p+3]=='a'|| f[p+3]=='A'))return 0;
else return 1; 
}

/////////////////////////////////////////////////////////////
int IsWavName(char * fname)
{
char f[13];U8 p=0;
memset(f,0,13);
strncpy(f,fname,13);
while(f[p]!=0){p++;}
if(p>4)p-=4;
if(f[p]=='.'&&(f[p+1]=='w'||f[p+1]=='W')&& (f[p+2]=='a'|| f[p+2]=='A')&& (f[p+3]=='v'|| f[p+3]=='V'))return 0;
else return 1;
}

/////////////////////////////////////////////////////////////
int IsMp3Name(char * fname)
{
char f[13];U8 p=0;
memset(f,0,13);
strncpy(f,fname,13);
while(f[p]!=0){p++;}
if(p>4)p-=4;
if(f[p]=='.'&&(f[p+1]=='m'||f[p+1]=='M')&& (f[p+2]=='p'|| f[p+2]=='P')&& (f[p+3]=='3'))return 0;
else return 1;
}


/////////////////////////////////////////////////////////////
//�����ļ����еĿɲ��ŵ������ļ�������
//���ظ�����
unsigned short SearchMusicFilesNumInDir(char * subdir)
{
unsigned short num=0;
DIR mydir;
FILINFO info;
f_opendir(&mydir,subdir);
while(1)
{
if((fres=f_readdir(&mydir,&info,TRUE))==FR_OK)
	{
	if(info.fname[0]==0)break;
	}

if(IsWavName(info.fname)!=0 && IsFlacName(info.fname)!=0 ) continue;
strncpy(tempstring,subdir,13);
strcat(tempstring,"\\");
strncat(tempstring,info.fname,13);

//if(IsMp3Name(info.fname)!=0)//��mp3�ļ���?
	{
	if(IsWavName(info.fname)==0) //��wav�ļ���?
		{
		if(playableWAV(tempstring)==0)goto ADDFILE;
		}
	if(IsFlacName(info.fname)==0) //��flac�ļ���?
		{
		if(playableFLAC(tempstring)==0)goto ADDFILE;
		}
	continue;
	}
ADDFILE:
num++;
if(num==MAXFILE)break;
}
return num;
}

//unicode �ַ��Ƚ� ����ȷ���0
int strncmp_unicode(WCHAR * s1,WCHAR *s2,unsigned char n)
{
unsigned char i=0;
while(i<=n)
{
if(*s2==0)
	if(*s1)return -1;
	else return 0;
if(*s1==0)
	if(*s2)return 1;
	else return 0;
if(*s1==*s2){i++;s1++;s2++;continue;}
if(*s1>*s2){return 1;}
else return -1; 
}
return 0;
}

//unsigned char �ַ����Ƚ� ����ȷ���0
/////////////////////////////////////////////////////////////
int strncmp_unchar(unsigned char  * s1,unsigned char  *s2,unsigned char n)
{
unsigned char i=0;
while(i<=n)
{
if(*s2==0)
	if(*s1)return -1;
	else return 0;
if(*s1==0)
	if(*s2)return 1;
	else return 0;
if(*s1==*s2){i++;s1++;s2++;continue;}
if(*s1>*s2){return 1;}
else return -1; 
}
return 0;
}

/////////////////////////////////////////////////////////////
//����unicode �ַ����ģ���ʾ����ȡ�
unsigned int  unis_width(unsigned short *us)
{
unsigned int wd=0;
while(*us)
{
if(*us<0x80)wd++;
else wd+=2;
us++;
}
return wd;
}

/////////////////////////////////////////////////////////////
//����unicode �ַ��� �ַ������ȡ�
unsigned int  unis_lenth(unsigned short *us)
{
unsigned int len=0;
while(*us)
{
us++;
len++;
}
return len;
}


/////////////////////////////////////////////////////////////
//ѡ���ļ���
#define ROOM_COL 25 //�ļ�����ʾ�Ŀ�ȣ��԰��Ӣ�ĳ��ȼ�)

//�ļ���ѡ������ʾһ���б���
void ShowDirItem(unsigned char i,unsigned short f,unsigned short b,unsigned int begin)
{
if(SelectLfnUnicode[i][0])
   LCD_Put_Width_Unis(16,(i+2)*16,SelectLfnUnicode[i],f,b,ROOM_COL);
else 
   LCD_Put_Width_GBK16(16,(i+2)*16,(unsigned char *)(Dirname[i+begin]),f,b,ROOM_COL);	
}


//ѡ���ļ���
int SelectAlbum(unsigned short * olddir)
{
DIR mydir;
FILINFO info;
unsigned short t[ROOM_COL*2+_MAX_LFN];
unsigned int i,k,j,select,disp_offset,key;
unsigned short begin=*olddir;
unsigned short *p;
key_left=key_right=key_up=0;
key_left_long=key_right_long=key_up_long=0;

DISABLE_AUDIO;

if(begin>(MAX_ITEM/2)){begin-=(MAX_ITEM/2);select=(MAX_ITEM/2);}
else {select=begin;begin=0;}
for(;;)
{
memset(SelectLfnUnicode[0],0,sizeof(U16)*_MAX_LFN*MAX_ITEM);

if(begin==0)
	{
	p=SelectLfnUnicode[0];
	*p++='*';*p++='-';*p++=' ';
	*p++='C';
	*p++='A';
	*p++='R';
	*p++='D';
	*p++=' ';*p++='*';*p++=' ';
	*p++='R';
	*p++='O';
	*p++='O';
	*p++='T';
	*p++=' ';*p++='-';*p++='*';
	}

f_opendir(&mydir,"");

for(i=0;;)
{
f_readdir(&mydir,&info,TRUE);
if(info.fname[0]==0)break;
if(i>=MAX_ITEM)break;
if(begin+i>=maxdir)break;
for(j=begin;j<begin+MAX_ITEM && j<maxdir;j++)
	if(strncmp(Dirname[j],info.fname,12)==0)
	{
	memcpy(SelectLfnUnicode[j-begin],Lfn_Unicode,_MAX_LFN*sizeof(U16));
	//for(k=0;k<_MAX_LFN;k++)SelectLfnUnicode[j-begin][k]=Lfn_Unicode[k];
	i++;
	}
}

#define TITLE_ALBUM_COLOR CYAN

LCD_Clear_Screen(Black);
LCD_PutString(4*8,0,"ALBUM SELECTION",TITLE_ALBUM_COLOR,BLACK);
LCD_PutString(0,16,"-----------------------------",TITLE_ALBUM_COLOR,BLACK);

for(i=0;i+begin<maxdir && i<MAX_ITEM; i++)
	 LCD_BMP(0,(i+2)*16,15,(i+2)*16+15,icon_folder);

LCD_Rectangle(225,0,225+10,319,Blue);
if(begin+MAX_ITEM>=maxdir)
	LCD_Rectangle(225+4,begin*319/maxdir,225+10-3,319,Cyan);
else
	LCD_Rectangle(225+4,begin*319/maxdir,225+10-3,(begin+MAX_ITEM)*319/maxdir,Cyan);


if(begin==0 && DirFilesNum[0]==0)
{
ShowDirItem(0,Gray,Black,begin);
for(i=1;i<select;i++)
	ShowDirItem(i,YELLOW,Black,begin);
}
else 
	for(i=0;i<select;i++)
		ShowDirItem(i,YELLOW,Black,begin);

ShowDirItem(select,Magenta,Black,begin);
	
for(i=select+1;i<MAX_ITEM && i+begin <maxdir;i++)
	ShowDirItem(i,YELLOW,Black,begin);

disp_offset=0;msec2=0;

ClearKey();

while(1)
{
if(msec2>15)
{
msec2=0;
for(i=0;i<ROOM_COL*2+_MAX_LFN;i++)t[i]=' ';

for(i=0;i<unis_lenth(SelectLfnUnicode[select]);i++)
			t[i+ROOM_COL]=SelectLfnUnicode[select][i];

if(unis_width(SelectLfnUnicode[select])>ROOM_COL)
	{

		if(disp_offset<unis_lenth(SelectLfnUnicode[select])+ROOM_COL)
			{
			disp_offset++;
			}
		else disp_offset=0;

	 	LCD_Put_Width_Unis(16,16*(select+2),t+disp_offset,Magenta,Black,ROOM_COL);
		LCD_PutChar8x16((2+ROOM_COL)*8,16*(select+2),' ',Magenta,Black);
	}
}

if(key_right)	  //������ѡ�񡿴���
		{
		ClearKey();
		delay_ms(200);
		if(DirFilesNum[0]==0 && (begin+select)==0)continue;
		if(*olddir!=begin+select)*olddir=begin+select;
		ShowDirItem(select,Orange,Black,begin);
		return 1;
		}

if(key_left)		//���������ء�����
		{
		ClearKey();
		delay_ms(200);
		return 0;
		}

if(key_down_push||key_down)	  //���������¡�����
		{
		key_down_push=60;key_down=0;
		if(select<MAX_ITEM-1)
			if(select+begin+1<maxdir)
						{
						ShowDirItem(select,Yellow,Black,begin);						
						select++;
						ShowDirItem(select,Magenta,Black,begin);
						disp_offset=0; 
						delay_ms(150);
						continue;
						}
		if(select==MAX_ITEM-1)
			if(begin+MAX_ITEM<maxdir)
				{begin+=MAX_ITEM;select=0;break;}
		}


if(key_up_push||key_up)	 //���������ϡ�����
	{
	key_up_push=70;key_up=0;
	if(!(begin==0 && DirFilesNum[0]==0 && select==1))
				{
				if(select>0)
					{
						ShowDirItem(select,Yellow,Black,begin);
						//LCD_Put_Width_Unis(16,16*select-1,SelectLfnUnicode[select],Yellow,Black,ROOM_COL);						
						select--;
						ShowDirItem(select,Magenta,Black,begin);
						//LCD_Put_Width_Unis(16,16*select-1,SelectLfnUnicode[select],Magenta,Black,ROOM_COL);
					disp_offset=0; 
					delay_ms(150);
					continue;
					}

				if(select==0)
					if(begin>=MAX_ITEM){begin-=MAX_ITEM;select=MAX_ITEM-1;break;}
					else 
						if(begin)
							{
							select=begin-1;
							begin=0;
							if(DirFilesNum[0]==0 && select==0)select=1;
							
							break;
							}
						else continue;	
			}
		}


if(key_play)	//��������һҳ������
			{
			ClearKey();
			if(begin+MAX_ITEM<maxdir)
					{
					begin+=MAX_ITEM;
					if(begin+select>=maxdir)
						select=maxdir-1-begin;
					break;
					}
			}

if(key_play_long)  //��������һҳ������
			{
			ClearKey();
			if(begin)
			{
			if(begin>=MAX_ITEM)	begin-=MAX_ITEM;
			else begin=0;
			break;
			}
			}

	}
 }
}


//�ļ�ѡ������ʾһ���б���
void ShowFilesInfoItem(unsigned char i,unsigned short f,unsigned short b,unsigned int begin)
{
if(SelectLfnUnicode[i][0])
   LCD_Put_Width_Unis(16,(i+4)*16,SelectLfnUnicode[i],f,b,ROOM_COL);
else 
   LCD_Put_Width_GBK16(16,(i+4)*16,(unsigned char *)Filename[i+begin],f,b,ROOM_COL);	
}

//ѡ��һ���ļ����е��ļ�
int SelectTrack(unsigned short dirid,unsigned short *filenum)
{
DIR mydir;
FILINFO info;
unsigned int key;
unsigned short begin=*filenum;
unsigned short t[ROOM_COL*2+_MAX_LFN];
unsigned int i,k,j,select,disp_offset;
key_left=key_right=key_up=key_down=0;
key_left_long=key_right_long=key_up_long=key_down_long=0;
DISABLE_AUDIO;
//Get_Dir_LFN();
if(begin>(MAX_TRACK_ITEM/2)){begin-=(MAX_TRACK_ITEM/2);select=(MAX_TRACK_ITEM/2);}
else {select=begin;begin=0;}
while(1)
{
memset(SelectLfnUnicode[0],0,sizeof(U16)*_MAX_LFN*MAX_TRACK_ITEM);
f_opendir(&mydir,Dirname[dirid]);//���ļ���

for(i=0;;)
{
f_readdir(&mydir,&info,TRUE);//��ȡһ��Ŀ¼��
if(info.fname[0]==0)break;//�Ѿ����ļ��������һ��Ŀ¼��
if(i>=MAX_TRACK_ITEM || i+begin>=DirFilesNum[dirid])break;//�Ѿ���ȡ��8���б����ļ��Ķ��ļ�����Ӧ�ĳ��ļ���
for(j=begin;j<begin+MAX_TRACK_ITEM && j<DirFilesNum[dirid];j++)
	if(strncmp(Filename[j],info.fname,12)==0)//�Ƚϵ�ǰĿ¼����ļ����볤�ļ����Ƿ�һ��
	{
	memcpy(SelectLfnUnicode[j-begin],Lfn_Unicode,_MAX_LFN*sizeof(U16));//�������ļ���
	i++;//
	}
}

#define TITLE_TRACK_COLOR BLUE2

LCD_Clear_Screen(Black);
LCD_PutString(4*8,0,"TRACK SELECTION",TITLE_TRACK_COLOR,BLACK);	//title
LCD_PutString(0,16,"-----------------------------",TITLE_TRACK_COLOR,BLACK);//split line
LCD_BMP(0,2*16,15,2*16+15,icon_folder);						//folder icon
if(CurrentDirLfnUnicode[0])									//folder name
   LCD_Put_Width_Unis(16,2*16,CurrentDirLfnUnicode,YELLOW,BLACK,ROOM_COL);
else 
   LCD_Put_Width_GBK16(16,2*16,(unsigned char *)Dirname[dirid],YELLOW,BLACK,ROOM_COL);


LCD_PutString(0,3*16,"-----------------------------",TITLE_TRACK_COLOR,BLACK);//split line

for(i=0;i+begin<DirFilesNum[dirid] && i<MAX_TRACK_ITEM; i++)	 //file icon
	 {
	 if(IsWavName(Filename[i+begin]))LCD_BMP(0,(i+4)*16,15,(i+4)*16+15,icon_music_blue);	//flac
	 else  LCD_BMP(0,(i+4)*16,15,(i+4)*16+15,icon_music_red);								//wav
	 }

LCD_Rectangle(225,0,225+10,319,Blue);													   //progress bar
if(begin+MAX_TRACK_ITEM>=DirFilesNum[dirid])
	{
	LCD_Rectangle(225+4,begin*319/DirFilesNum[dirid]+3,225+10-3,319-3,Cyan);
	}
else
	{
	LCD_Rectangle(225+4,begin*319/DirFilesNum[dirid]+3,225+10-3,(begin+MAX_TRACK_ITEM)*319/DirFilesNum[dirid]-3,Cyan);
	}

for(i=0;i<select;i++)//track list name
	ShowFilesInfoItem(i,Green,Black,begin);

ShowFilesInfoItem(select,Magenta,Black,begin);	 //selected one 

for(i=select+1;i<MAX_TRACK_ITEM && i+begin<DirFilesNum[dirid];i++)	 
	ShowFilesInfoItem(i,Green,Black,begin);

disp_offset=0;msec2=0;

delay_ms(200);
ClearKey();		   //discard key

while(1)
{
if(msec2>15)//if name too long , let it run by step 150ms to display all
{
msec2=0;
for(i=0;i<ROOM_COL*2+_MAX_LFN;i++)t[i]=' ';

for(i=0;i<unis_lenth(SelectLfnUnicode[select]);i++)
			t[i+ROOM_COL]=SelectLfnUnicode[select][i];

//when the name of selected track is too long,it need to run to display all
if(unis_width(SelectLfnUnicode[select])>ROOM_COL)
	{
	if(disp_offset<unis_lenth(SelectLfnUnicode[select])+ROOM_COL)//ÿ��ʱ������ʾ�ĵ�һ������������ƣ�ʵ�������Ч��
			disp_offset++;
	else disp_offset=0;

	 	LCD_Put_Width_Unis(16,16*select+64,t+disp_offset,Magenta,Black,ROOM_COL);
		LCD_PutChar8x16((2+ROOM_COL)*8,16*select+64,' ',Magenta,Black);
	}
}

if(key_right)//������ѡ�񡿴���	, enter key	= right key
		{
		ClearKey();
		delay_ms(200);
		*filenum= begin+select;//���ص�ǰѡ����ļ���Ӧ�ı��
		return 0;
		}

if(key_left)//���������ء�����	 ,cancle and return key =left key
		{
		ClearKey();
		delay_ms(200);
		return 1;
		}

if(key_down_push || key_down)	//���������¡�����,down key
		{
		key_down=0;key_down_push=60;
		if(select<MAX_TRACK_ITEM-1)
			if(select+begin+1<DirFilesNum[dirid])
						{
						ShowFilesInfoItem(select,Green,Black,begin);					
						select++;
						ShowFilesInfoItem(select,Magenta,Black,begin);
						disp_offset=0; 
						delay_ms(150);
						continue;
						}
				if(MAX_TRACK_ITEM-1==select)
					if(begin+MAX_TRACK_ITEM<DirFilesNum[dirid])
						{begin+=MAX_TRACK_ITEM;select=0;break;}
				}

if(key_up_push || key_up) //���������ϡ����� ,up key
				{
				key_up=0;key_up_push=70;
				if(select>0)
					{
					ShowFilesInfoItem(select,Green,Black,begin);					
					select--;
					ShowFilesInfoItem(select,Magenta,Black,begin);
					disp_offset=0; 
					delay_ms(150);
					continue;
					}
				if(0==select)
					if(begin>=MAX_TRACK_ITEM){begin-=MAX_TRACK_ITEM;select=MAX_TRACK_ITEM-1;break;}
					else 
						if(begin)
							{
							select=begin-1;
							begin=0;
							break;
							}
						else continue;
				}



if(key_play){  //��������һҳ������	, next page=play key
			key_play=0;
			if(begin+MAX_TRACK_ITEM<DirFilesNum[dirid])
					{
					begin+=MAX_TRACK_ITEM;
					if(begin+select>=DirFilesNum[dirid])
						select=DirFilesNum[dirid]-1-begin;
					break;
					}
			else if(begin+MAX_TRACK_ITEM!=DirFilesNum[dirid] && DirFilesNum[dirid]>MAX_TRACK_ITEM)
					{
					begin=DirFilesNum[dirid]-MAX_TRACK_ITEM;
					select=MAX_TRACK_ITEM-1;
					break;
					}
			}

if(key_play_long)  //��������һҳ������	previous page=play key long
			{
			key_play_long=0;
			if(begin>=MAX_TRACK_ITEM){begin-=MAX_TRACK_ITEM;select=MAX_TRACK_ITEM-1;break;}
			else if(begin!=0){
							 begin=0;break;
							 }
			}

	}
}
//
}


void Load_Dir_Files_List(void);

void Select(void)
{
do
	{
	if(SelectAlbum(&dirnum)==0)break;
	Load_Dir_Files_List();
	tracknum=0;
	}
while(SelectTrack(dirnum,&tracknum));
}

const char fre_str[17][20]=
	{
	"FR_OK",
	"FR_DISK_ERR",		/* 1 */
	"FR_INT_ERR",			/* 2 */
	"FR_NOT_READY",		/* 3 */
	"FR_NO_FILE",			/* 4 */
	"FR_NO_PATH",			/* 5 */
	"FR_INVALID_NAME",	/* 6 */
	"FR_DENIED",		/* 7 */
	"FR_EXIST",			/* 8 */
	"FR_INVALID_OBJECT",	/* 9 */
	"FR_WRITE_PROTECTED",	/* 10 */
	"FR_INVALID_DRIVE",	/* 11 */
	"FR_NOT_ENABLED",		/* 12 */
	"FR_NO_FILESYSTEM",	/* 13 */
	"FR_MKFS_ABORTED",	/* 14 */
	"FR_TIMEOUT",		/* 15 */
	"UNKNOWN_ERR"
	};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Open_Root_And_Find_Music_Files(void)
{
unsigned int i,k;
unsigned long tn;
DIR dir;
dirnum=0;tracknum=0;
DISABLE_AUDIO;

#ifdef DEBUG
							//turn off led2	
printf("11-1..OPENROOT\n");
#endif

OPENROOT:
while(1)
{
tracknum=0;
for(i=0;i<MAXDIR;i++)DirFilesNum[i]=0;
if((fres=f_opendir(&dir,""))!=FR_OK)
									{
									LCD_Txt_Puts("OPEN ROOT FAILED!\nCODE=");
									if(fres>=17)fres=17;
									for(i=0;i<20;i++)tempstring[i]=fre_str[fres][i];
									LCD_Txt_Puts(tempstring);
									LCD_Txt_Display();
									delay_ms(1000);
									return 1;
									}
else break;
}

memset(Dirname[0],0,MAXDIR*13);
maxdir=1;
i=SearchMusicFilesNumInDir("");//�����ļ����еĿɲ��������ļ�������
strcpy(Dirname[0],"\\");
DirFilesNum[0]=i;

while(1)
{
if((fres=f_readdir(&dir,&f_info,FALSE))==FR_OK)
	{
	if(f_info.fname[0]==0)break;
	}
else goto OPENROOT;

if(f_info.fattrib & AM_DIR)
	{
	if(maxdir<MAXDIR)
		{
			strcpy(Dirname[maxdir],f_info.fname);
			i=SearchMusicFilesNumInDir(Dirname[maxdir]);
			if(i)
				{
				strcpy(Dirname[maxdir],f_info.fname);
				DirFilesNum[maxdir]=i;
				DirIndex[maxdir]=dir.index;
				maxdir++;
				}
		}
	}
}

for(total_music_files=0,i=0;i<maxdir;i++)
	total_music_files+=DirFilesNum[i];


delay_ms(2000);

if(0==total_music_files)
	{
	LCD_Txt_Clear();
	LCD_Txt_Set_YX(5,0);
	LCD_Txt_Set_Color(Red);
	LCD_Txt_Puts("û�п��Բ��ŵ������ļ�!");
	LCD_Txt_Puts("\n�������һ�Ŵ洢��������!");
	LCD_Txt_Puts("\n\nNO MUSIC FILE!");
	LCD_Txt_Puts("\nINSERT ANOTHER CARD THEN REBOOT PLAYER!");
	LCD_Txt_Display();
	while(1);//HALT
	}

//light_timer=LIGHT_ON_TIME;
//light_auto_off=TRUE;

//Sort Dirs Filename
for(i=1;i<maxdir;i++)
	for(k=i;k<maxdir;k++)
		if(strncmp_unchar((unsigned char*)Dirname[i],(unsigned char*)Dirname[k],8)>0)
			{
			strncpy(Dirname[MAXDIR],Dirname[i],12);
			tn=DirFilesNum[i];
			strncpy(Dirname[i],Dirname[k],12);
			DirFilesNum[i]=DirFilesNum[k];;
			strncpy(Dirname[k],Dirname[MAXDIR],12);
			DirFilesNum[k]=tn;
			}

if(DirFilesNum[0])dirnum=0;
else dirnum=1;

return 0;

}


void Show_Logo()
{
LCD_Clear_Screen(BLACK);
f_open(&file1,"system/LOGO320.bmp",FA_READ);
LCD_File_Bmp(&file1);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Open_File_System(void)
{
unsigned long disk_size;

if((fres=f_mount(0, &fs))!=FR_OK){
								LCD_Txt_Puts("Open Disk Failed!\nCODE=");
								LCD_Txt_Puts((char *)fre_str[fres]);
								LCD_Txt_Display();
								delay_ms(2000);
								return 1;
								}
LCD_Txt_Clear();
if((fres=f_opendir(&dir,"\\"))==FR_OK)
	{
	LCD_Txt_Puts("\nOPEN FATFS OK!\nFORMAT:");
	if(fs.fs_type==FS_FAT32)LCD_Txt_Puts("FAT32");
		else if(fs.fs_type==FS_FAT16)LCD_Txt_Puts("FAT16");

	#if !_FS_READONLY
	disk_size=(fs.free_clust*fs.csize)>>11;
	LCD_Txt_Puts("\nFREE  : ");
	if(disk_size<1024)
			{LCD_Txt_Put_Num(disk_size);LCD_Txt_Puts(" MB");}
	else{
		LCD_Txt_Put_Num(disk_size/1024);LCD_Txt_Putc('.');LCD_Txt_Put_Num((disk_size%1024)*100/1024);
		LCD_Txt_Puts(" GB");
		}
	#endif
	LCD_Txt_Puts("\nTOTAL : ");disk_size=(fs.max_clust*fs.csize)>>11;//MB
	if(disk_size<1024)
			{LCD_Txt_Put_Num(disk_size);LCD_Txt_Puts(" MB\n");}
	else{
		LCD_Txt_Put_Num(disk_size/1024);
		LCD_Txt_Putc('.');
		LCD_Txt_Put_Num((disk_size%1024)*100/1024);
		LCD_Txt_Puts(" GB\n");
		}
	}
LCD_Txt_Display();
return 0;
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Sort_Filename(void)
{
unsigned int i,k,j;
#ifdef SORT_BY_LFN
for(i=0;i<maxtracks;i++)
	for(k=i+1;k<maxtracks;k++)
		{
		if(strncmp_unicode(LfnName[i],LfnName[k],SORT_LFN_LEN)>0)
			{
			strncpy(Filename[MAXFILE],Filename[i],12);
			strncpy(Filename[i],Filename[k],12);
			strncpy(Filename[k],Filename[MAXFILE],12);
			FileIndex[MAXFILE]=FileIndex[i];
			FileIndex[i]=FileIndex[k];
			FileIndex[k]=FileIndex[MAXFILE];
			for(j=0;j<SORT_LFN_LEN;j++)
				{
				LfnName[MAXFILE][j]=LfnName[i][j];
				LfnName[i][j]=LfnName[k][j];
				LfnName[k][j]=LfnName[MAXFILE][j];
				}
			}
		}

#else
for(i=0;i<maxtracks;i++)
	for(k=i+1;k<maxtracks;k++)
		if(strncmp_unchar(Filename[i],Filename[k],12)>0)
			{
			strncpy(Filename[MAXFILE],Filename[i],12);
			strncpy(Filename[i],Filename[k],12);
			strncpy(Filename[k],Filename[MAXFILE],12);
			FileIndex[MAXFILE]=FileIndex[i];
			FileIndex[i]=FileIndex[k];
			FileIndex[k]=FileIndex[MAXFILE];
			}
#endif
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Open_Fonts_Files(void)
{
f_close(&f_font);
if(( fres = f_open (&f_font,"SYSTEM/GBK16.FNT", FA_READ))!=FR_OK)
	{
	LCD_Txt_Puts("OPEN GBK16.FNT FAILED!\n");
	font_on=0;
	}
else 
	{
	LCD_Txt_Puts("OPEN GBK16.FNT OK!\n");
	font_on=1;
	}
f_close(&f_u2gb);
if(( fres = f_open (&f_u2gb,"SYSTEM/U2GB.TBL", FA_READ))!=FR_OK)
	{
	LCD_Txt_Puts("OPEN U2GB.TBL FAILED!\n");
	u2gb_on=0;
	}
else 
	{
	LCD_Txt_Puts("OPEN U2GB.TBL OK!\n");
	u2gb_on=1;
	}
}


FRESULT search_one_big_file(char * bigfile)
{
#define BIG_FILE_SIZE ((10*1024*1024))
DIR rootdir,subdir;
FILINFO root_info,sub_info;
if((fres=f_opendir(&rootdir,""))!=FR_OK)return FR_NO_FILE;
*bigfile=0;
do
{
f_readdir(&rootdir,&root_info,FALSE);
}
while(root_info.fsize<BIG_FILE_SIZE && root_info.fname[0]);

if(root_info.fsize>=BIG_FILE_SIZE)
		{
		strcpy(bigfile,root_info.fname);
		return FR_OK;
		}

//no big file in root,search sub dir.
f_opendir(&rootdir,"");
do
{
f_readdir(&rootdir,&root_info,FALSE);
if(root_info.fattrib & AM_DIR)
	{
	if(f_opendir(&subdir,root_info.fname)==FR_OK)
			{
			 do
			 {
			 f_readdir(&subdir,&sub_info,FALSE);
			 }
			 while(sub_info.fsize<BIG_FILE_SIZE && sub_info.fname[0]);

			 if(sub_info.fsize>=BIG_FILE_SIZE)
			 	{
				strcpy(bigfile,root_info.fname);
				strcat(bigfile,"/");
				strcat(bigfile,sub_info.fname);
				return FR_OK;
				}
			}

	}
}
while(root_info.fname[0]);
return FR_NO_FILE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Test_Reading_Speed(void)
{
FIL file;
unsigned int readed,time,speed;
UINT br;
UINT pcm_readed_size=0;
u8 * test_buffer;
test_buffer=malloc(4096);
if(test_buffer==NULL)return;
fres=search_one_big_file(tempstring);
if(fres==FR_OK)
{
f_open(&file,tempstring,FA_READ);
msec2=0;
pcm_readed_size=0;
while(msec2<200)
{
f_read(&file,test_buffer,4096,&br);
pcm_readed_size+=br;
}
free(test_buffer);
f_close(&file);
LCD_Txt_Puts("RD SPEED:");
LCD_Txt_Put_Num(pcm_readed_size/1024/2);
LCD_Txt_Puts("KB/S");
}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Get_Dir_LFN(void)
{
unsigned int i;
unsigned short *p;
DIR dir;
if(dirnum>0)
	{
	f_opendir(&dir,"");
 	while(1)
		{
		f_readdir(&dir,&f_info,TRUE);
		if(f_info.fname[0]==0)break;
		if(strncmp(f_info.fname,Dirname[dirnum],12)==0)
				{
				for(i=0;i<_MAX_LFN;i++)CurrentDirLfnUnicode[i]=Lfn_Unicode[i];
				break;
				}
		}
	}
else
	{
	p=CurrentDirLfnUnicode;
	*p++='*';*p++='-';*p++=' ';
	*p++='C';
	*p++='A';
	*p++='R';
	*p++='D';
	*p++=' ';*p++='*';*p++=' ';
	*p++='R';
	*p++='O';
	*p++='O';
	*p++='T';
	*p++=' ';*p++='-';*p++='*';
	*p++=0;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Halt(void)
{
LCD_Txt_Clear();
LCD_Txt_Set_Color(Red);
LCD_Txt_Set_YX(5,0);
LCD_Txt_Puts("DISK I/O failed!\n");
LCD_Txt_Puts("Please INSERT Card!\nThen RESET Oopy!");
LCD_Txt_Display();
while(1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int Search_Music_Files_In_Dir(void)
{
unsigned short file_dirindex;
unsigned int i;

#define SEARCH_FILES_OK 0
#define OPEN_DIR_FAILED 1
#define NO_MUSIC_FILE 2

f_opendir(&dir,Dirname[dirnum]);
maxtracks=0;
file_dirindex=0;

memset(Filename[0], 0, 13*MAXFILE); 


while(1)
{
if((fres=f_readdir(&dir,&f_info,TRUE))!=FR_OK){tracknum=0;return OPEN_DIR_FAILED;}
else if(f_info.fname[0]==0)break;

file_dirindex++;

if(IsWavName(f_info.fname)!=0 && IsFlacName(f_info.fname)!=0 )
	continue;

if(dirnum)
	{
	strncpy(tempstring,Dirname[dirnum],13);
	strcat(tempstring,"\\");
	}
else tempstring[0]=0;

strncat(tempstring,f_info.fname,13);
if(IsMp3Name(f_info.fname)!=0)
	if(playableWAV(tempstring) && playableFLAC(tempstring))continue;

FileIndex[maxtracks]=file_dirindex-1;
strcpy(Filename[maxtracks],f_info.fname);

#ifdef SORT_BY_LFN
for(i=0;i<SORT_LFN_LEN;i++)
	LfnName[maxtracks][i]=Lfn_Unicode[i];
#endif

maxtracks++;
if(maxtracks==MAXFILE)break;
}

if(maxtracks==0)return NO_MUSIC_FILE;

return SEARCH_FILES_OK;
}

/*******************************************************************************/
void Load_Dir_Files_List(void)
{
unsigned int res;
DISABLE_AUDIO;
do
{
tracknum=0;
Get_Dir_LFN();
res=Search_Music_Files_In_Dir();
if(res==OPEN_DIR_FAILED)Halt();
else
	if(res==NO_MUSIC_FILE)
	{
	if(maxdir)dirnum++;
		if(dirnum>=maxdir)dirnum=0;
	continue;
	}
}
while(res!=SEARCH_FILES_OK);
Sort_Filename();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Show_Info(void)
{
 LCD_Txt_Clear();
 LCD_Txt_Set_YX(0,3);
 LCD_Txt_Set_Color(Orange);
 if(Dirname[dirnum][0]!='\\')
	if(CurrentDirLfnUnicode[0])LCD_Txt_n_Uns(CurrentDirLfnUnicode,_MAX_LFN);//��ʾ�ļ��еĳ��ļ���
	else  LCD_Txt_Puts(Dirname[dirnum]);						 //û�г��ļ�������ʾ���ļ���
 else LCD_Txt_Puts("��Ŀ¼ ROOT");

 LCD_Txt_Set_YX(1,0);
 LCD_Txt_Puts("------------------------------");
  LCD_Txt_Set_YX(3,3);
 LCD_Txt_Set_Color(Cyan);
 if(Lfn_Unicode[0])LCD_Txt_n_Uns(Lfn_Unicode,_MAX_LFN);	//��ʾ�����ļ��ĳ��ļ���
 else 	LCD_Txt_Puts(Filename[tracknum]);				//û�г��ļ�������ʾ���ļ���
  LCD_Txt_Set_YX(6,0);
  LCD_Txt_Puts("------------------------------");
  LCD_Txt_Set_YX(15,0);
  LCD_Txt_Set_Color(Green);
  LCD_Txt_Puts("------------------------------TRACKS: ");
  LCD_Txt_Set_Color(BLUE2);
  LCD_Txt_Put_Num(tracknum+1);
  LCD_Txt_Set_Color(Green);
  LCD_Printf(" / %d / %d\nALBUMS: ",maxtracks,total_music_files);
  if(DirFilesNum[0])
  		{
		LCD_Txt_Set_Color(BLUE2);LCD_Txt_Put_Num(dirnum+1);
		LCD_Txt_Set_Color(Green);LCD_Printf(" / %d",maxdir+1);
		}
  else 
  		{
		LCD_Txt_Set_Color(BLUE2);LCD_Txt_Put_Num(dirnum);
		LCD_Txt_Set_Color(Green);LCD_Printf(" / %d",maxdir);
		}
  LCD_Txt_Puts("\nREPEAT: ");
  switch(mode)
  {
  case REPEAT_MODE_ALL :LCD_Txt_Puts("ALL");break;
  case REPEAT_MODE_DIR :LCD_Txt_Puts("ALBUM");break;
  case REPEAT_MODE_ONE :LCD_Txt_Puts("SINGLE");break;
  case REPEAT_MODE_RND :
  						LCD_Txt_Puts("RANDOM ");
  						LCD_Txt_Set_Color(BLUE2);LCD_Txt_Put_Num(random_index+1);
  						LCD_Txt_Set_Color(Green);LCD_Printf(" / %d",random_max);
						break;
  case REPEAT_MODE_LST:LCD_Txt_Puts("LST");break;
  }
  LCD_Txt_Puts("\n------------------------------");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Find_File_LFN()
{
unsigned int i;
//���Ҷ��ļ�����Ӧ�ĳ��ļ���
//ֱ�Ӷ�ȡ���ļ�����UniCode����dir_read()��������Unicode,��Lfn_Unicode[]��
//��ʾʱ�ٱ任��GBK
//Unicode �� GBK �Ķ��ձ�����SD CARD��systemĿ¼��,�ļ���Ϊ U2GB.TBL
//GBK�����ļ�Ҳ���ڸ�Ŀ¼�У��ļ���ΪGBK16.FNT ������ɨ�裬��λ����
f_opendir(&dir,Dirname[dirnum]);
///////////////////////////////////////
//��ȡĿ¼����������ļ���
for(i=0;i<FileIndex[tracknum];i++)
	f_readdir(&dir,&f_info,FALSE);
//��ȡĿ¼��������ļ���
f_readdir(&dir,&f_info,TRUE);
}



void GetSrandSeed(void)
{
unsigned int i;
unsigned int res;
volatile unsigned int seed;
for(i=0,seed=0;i<32;i++)
		{
		ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
        while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET);  //ADC complete 
        res=ADC_GetConversionValue(ADC1);
		if(res&1)
			seed++;
		seed<<=1;
		delay_ms(2);
		}

srand(seed);
}

//������б�random_list[]���������һ����λ
//����:max��ʾ��Χ0-max
//����:��λ�±�
unsigned short Check_And_Find_Place_For_Random_List(unsigned short max)
{
unsigned short i,rn,rp;

rn=((unsigned int )rand())% max;
rp=0;
i=rn+1;
while(1)
{
if(random_list[rp]==0)
	{
	if(i==1)return rp;
	else i--;
	}
if(++rp>=max)rp=0;
}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����һ������б�random_list[max]
//��ÿ���ļ������ظ�.)
void Generate_Random_List(void)
{
unsigned short i,n;
unsigned short total_files;
											 
memset(random_list,0,MAX_RANDOM_LIST*sizeof(unsigned short));

//�����ܵ��ļ���
for(i=0,total_files=0;i<maxdir;i++)
	total_files+=DirFilesNum[i];

if(total_files<MAX_RANDOM_LIST)random_max=total_files;
else random_max=MAX_RANDOM_LIST;

GetSrandSeed();


//�ӵ�һ���ļ��е�һ���ļ��������һ���ļ��е����һ���ļ��ֱ���Ϊ1��max���ļ�.
for(random_index=1;random_index<=random_max;random_index++)
	{
	n=Check_And_Find_Place_For_Random_List(random_max);//��ȡһ����λ
	if(n>random_max)n=0;
	random_list[n]=random_index;//�ÿ�λΪ��n���ļ�
	}
if(random_max)random_index=random_max-1;
else random_index=0;
}

/*******************************************************************************/
//����б�ǰָ��ָ����һ���ļ�
void Random_File_List_Next(void)
{
unsigned short time_out=0,tn,td;
do
{
random_index++;
if(random_index>=random_max)random_index=0;
}
while(random_list[random_index]==0 && ++time_out<1024);
if(random_list[random_index]==0)tn=0;
else	tn=random_list[random_index]-1;

td=0;
while(tn>=DirFilesNum[td])
	{tn-=DirFilesNum[td++];}
random_dir=td%maxdir;
random_tracknum=tn%DirFilesNum[random_dir];
}

/*******************************************************************************/
//����б�ǰָ��ָ����һ���ļ�
void Random_File_List_Previous(void)
{
unsigned short tn,td;
if(random_max==0){random_index=0;return;}
if(random_index==0)random_index=random_max-1;
else random_index--;

if(random_list[random_index]==0)tn=rand()%random_max;
else	tn=random_list[random_index]-1;

td=0;
while(tn>=DirFilesNum[td])
	{tn-=DirFilesNum[td++];}
random_dir=td;
random_tracknum=tn;
}

void Random_List_Seek_To_Current_File(void)
{
int n,m;
//����������б�ָ��ָ��ǰ���ļ�λ��.
for(n=0,m=0;n<dirnum;n++)
	m+=DirFilesNum[n];
m+=tracknum+1;
for(random_index=0;random_index<random_max;random_index++)
	if(random_list[random_index]==m)break;
}

/*******************************************************************************/
void LCD_Color_Show()
{
LCD_Set_Area(0,0,239,319);
LCD_Clear_Screen(BLUE);
delay_ms(200);
LCD_Clear_Screen(BLUE2);
delay_ms(200);
LCD_Clear_Screen(RED);
delay_ms(200);
LCD_Clear_Screen(RED);
delay_ms(200);
LCD_Clear_Screen(YELLOW);
delay_ms(200);
LCD_Clear_Screen(CYAN);
delay_ms(200);
LCD_Clear_Screen(ORANGE);
delay_ms(200);
LCD_Clear_Screen(GREEN);
}

int select_file;


/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{
u8 disk_res;
SD_CardInfo SDCardInfo;
SD_Error Status;
UINT i,k;

font_on=0;
Timer_Config(100,(6773)*RCC_PLL_CLOCKMUL/4);
RCC_Config();
NVIC_Config();

#ifdef DEBUG

USART1_UART_Init(9600UL);			//usart1 INIT
hw_led_init();                      //leds INIT

printf("usart1 init ok\n");
#endif

Key_Config();
SPI1_Config();
DISABLE_AUDIO;
delay_ms(1000);
ADC_Config();

#ifdef DEBUG
hw_led_control(led1,OFF);hw_led_control(led2,OFF);		//turn off leds
printf("1..Key_Config,SPI1_Config,DISABLE_AUDIO,ADC_Config\n");	
#endif

LCD_Init();							//lcd INIT

#ifdef DEBUG
hw_led_control(led1,ON);								//led1 turn on.
printf("2..LCD_Init\n");
#endif

LCD_Color_Show();

#ifdef DEBUG
hw_led_control(led2,ON);								//led2 turn on
printf("3..LCD_Color_Show\n");
#endif

GetSrandSeed();

#ifdef DEBUG
//hw_led_control(led1,OFF);hw_led_control(led2,OFF);		//turn off leds	
printf("4..GetSrandSeed\n");
#endif


mda_or_interrupt=SDCARD_MODE_DMA;

DISK_INIT:
disk_res=disk_initialize(0);

if(disk_res==RES_OK)
	Status=SD_GetCardInfo(&SDCardInfo);

	#ifdef DEBUG
	hw_led_control(led1,OFF);							//turn on led1	
	printf("5..SD_GetCardInfo\n");
	#endif

LCD_Txt_Set_Color(CYAN);
LCD_Txt_Clear();
GetSrandSeed();

	#ifdef DEBUG
	hw_led_control(led2,OFF);							//turn on led2	
	printf("6..GetSrandSeed\n");
	#endif

if(Open_File_System())goto DISK_INIT;

	#ifdef DEBUG
	hw_led_control(led2,ON);							//turn off led2	
	printf("7..Open_File_System\n");
	#endif

Open_Fonts_Files();

#ifdef DEBUG
//hw_led_control(led1,ON);hw_led_control(led2,OFF);		//turn off leds
printf("8..Open_Fonts_Files\n");	
#endif

Test_Reading_Speed();									//before read_speed():266KHz;after read_speed():4.808MHz

#ifdef DEBUG
hw_led_control(led2,OFF);							//turn off led2	
printf("9..Test_Reading_Speed\n");
#endif

LCD_Txt_Display();

#ifdef DEBUG
hw_led_control(led1,ON);							//turn off led2	
printf("10..LCD_Txt_Display\n");
#endif

delay_ms(1000);
Show_Logo();

#ifdef DEBUG
hw_led_control(led2,ON);							//turn off led2	
printf("11..Show_Logo\n");
#endif

if(		Open_Root_And_Find_Music_Files()	)goto DISK_INIT;

#ifdef DEBUG
hw_led_control(led1,OFF);hw_led_control(led2,OFF);		//turn on leds	
printf("12..Open_Root_And_Find_Music_Files\n");
#endif

Generate_Random_List();

#ifdef DEBUG
printf("13..Generate_Random_List\n");
#endif

fres=f_opendir(&dir,"");

#ifdef DEBUG
printf("14..f_opendir\n");
#endif

mode=REPEAT_MODE_ALL;

Load_Dir_Files_List();//��ȡ�ļ����������ļ��б�

#ifdef DEBUG
printf("15..Load_Dir_Files_List\n");
#endif

tracknum=0;

NEXTTRACK:
while(1)
{
DISABLE_AUDIO;
switch(mode)
{
case REPEAT_MODE_RND:

#ifdef DEBUG
printf("16..REPEAT_MODE_RND\n");
#endif

//����ظ�ģʽ�����ģʽ
	if(!select_file)
				{
				//û���ֶ�ѡ�񲥷��ļ���
				if(dirnum!=random_dir)
							{
							dirnum=random_dir;
							Load_Dir_Files_List();//������������б��ȡ�ļ��е������ļ���Ϣ
							}
				tracknum=random_tracknum;
				}
	else
	 	{
		//����ֶ�ѡ�����ļ��������������б�ָ��ָ��ǰ���ļ�λ��.
		Random_List_Seek_To_Current_File();
		}
break;

case REPEAT_MODE_ALL://������Ŀѭ��

#ifdef DEBUG
printf("17..REPEAT_MODE_ALL\n");
#endif

			 if(tracknum>=maxtracks)//���һ���ļ�������ϣ����뵽��һ���ļ���
			  {
			  tracknum=0;
			  if(++dirnum>=maxdir)dirnum=0;
			  Load_Dir_Files_List();
			  continue;//goto NEXTTRACK
			  }
break;

case REPEAT_MODE_DIR://�ļ���ѭ��

#ifdef DEBUG
printf("18..REPEAT_MODE_DIR\n");
#endif

if(tracknum>=maxtracks)//���һ���ļ�������ϣ����´ӵ�һ���ļ����š�
	tracknum=0;	

break;

default : break; //����ģʽ����������
}

select_file=0;

Find_File_LFN();//�������ڲ��ŵ��ļ��ĳ��ļ�����Ϣ

#ifdef DEBUG
printf("19..Find_File_LFN\n");
#endif

Show_Info();//��ʾ�ļ�����Ϣ

#ifdef DEBUG
printf("20..Show_Info\n");
#endif

if(dirnum)
	{
	strncpy(tempstring,Dirname[dirnum],13);
	strcat(tempstring,"\\");
	}
else tempstring[0]=0;

strncat(tempstring,Filename[tracknum],13);
if(IsWavName(Filename[tracknum])==0){

	#ifdef DEBUG
	printf("100..playWAV\n");
	#endif

	playWAV(tempstring);
}
else
 if(IsFlacName(Filename[tracknum])==0){

	#ifdef DEBUG
	printf("200..playFLAC\n");
	#endif

	playFLAC(tempstring);
}
 else 
 	{
	//playMAD(tempstring);
	//mp3(tempstring);
	}


if(key_right)//��һ��
	{
	key_right=0;
	switch(mode)
	{
	case REPEAT_MODE_RND:
		Random_File_List_Next();
	break;

 	case REPEAT_MODE_DIR:
		tracknum++;
		if(tracknum>=DirFilesNum[dirnum])tracknum=0;
	break;

	case REPEAT_MODE_ONE:
	case REPEAT_MODE_ALL:
	default:
		tracknum++;
		if(tracknum>=maxtracks)
			{
			if(++dirnum>=maxdir)
				if(DirFilesNum[0]==0)
					dirnum=1;
				else
					dirnum=0;
			Load_Dir_Files_List();
			tracknum=0;
			}
	break;
	}
	goto NEXTTRACK;
	}


if(key_left)//��һ��
	{
	key_left=0;

	switch(mode)
	{
	case REPEAT_MODE_DIR:
	if(tracknum)tracknum--;
	else tracknum=DirFilesNum[dirnum];
	break;

	case REPEAT_MODE_RND:
		Random_File_List_Previous();
	break;

	case REPEAT_MODE_ALL:
	case REPEAT_MODE_ONE:
	default:
 	if(tracknum)tracknum--;
	else  		if(dirnum)
						  {
						  dirnum--;
						  if(dirnum==0 && DirFilesNum[0]==0)dirnum=maxdir-1;
						  Load_Dir_Files_List();
						  tracknum=DirFilesNum[dirnum]-1;
						  }
				else
						 {
						 dirnum=maxdir-1;
						 Load_Dir_Files_List();
						 tracknum=DirFilesNum[dirnum]-1;
					 	 }
	break;
	}
	goto NEXTTRACK;

  }

if(key_up)//ѡ���ļ�
	{
	key_right=0;
	SelectTrack(dirnum,&tracknum);
	select_file=1;
	goto NEXTTRACK;
	}

if(key_up_long)	//ѡ��Ŀ¼
	{
	Select();
	select_file=1;
	goto NEXTTRACK;
	}


key_left=key_right=key_up=key_down=0;
key_left_long=key_right_long=key_up_long=key_down_long=0;

switch(mode) //����Ǹ����������Ž���
	{
	case REPEAT_MODE_RND:	Random_File_List_Next();break;
	case REPEAT_MODE_ONE:	break;//repeat this song
	case REPEAT_MODE_DIR:
	case REPEAT_MODE_ALL:
	default				:	tracknum++;break;//next song
						
	}
}

}


/*******************************************************************************
* Function Name  : RCC_Config
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Config(void)
{
u32 mul;
ErrorStatus HSEStartUpStatus;   
/* RCC system reset(for debug purpose)*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  RCC_MCOConfig(RCC_MCO_HSE);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
 
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    /* PCLK1 = HCLK */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    
#if RCC_PLL_CLOCKMUL==3
	mul=RCC_PLLMul_3;
#endif

#if RCC_PLL_CLOCKMUL==4
    mul=RCC_PLLMul_4;
#endif

#if RCC_PLL_CLOCKMUL==5
    mul=RCC_PLLMul_5;
#endif

#if RCC_PLL_CLOCKMUL==6
    mul=RCC_PLLMul_6;
#endif

#if RCC_PLL_CLOCKMUL==7
    mul=RCC_PLLMul_7;
#endif

#if RCC_PLL_CLOCKMUL==8
    mul=RCC_PLLMul_8;
#endif

#if RCC_PLL_CLOCKMUL==9
    mul=RCC_PLLMul_9;
#endif

#if RCC_PLL_CLOCKMUL==10
    mul=RCC_PLLMul_10;
#endif

#if RCC_PLL_CLOCKMUL==11
    mul=RCC_PLLMul_11;
#endif

#if RCC_PLL_CLOCKMUL==12
    mul=RCC_PLLMul_12;
#endif

#if RCC_PLL_CLOCKMUL==13
    mul=RCC_PLLMul_13;
#endif

#if RCC_PLL_CLOCKMUL==14
    mul=RCC_PLLMul_14;
#endif

#if RCC_PLL_CLOCKMUL==15
    mul=RCC_PLLMul_15;
#endif

#if RCC_PLL_CLOCKMUL==16
    mul=RCC_PLLMul_16;
#endif

#ifdef RCC_PLL_HSE
	 RCC_PLLConfig(RCC_PLLSource_HSE_Div1, mul);
#else
	 RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);
#endif

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
}

/*******************************************************************************
* Function Name  : NVIC_Config
* Description    : Configures SDIO IRQ channel.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* SPI1 IRQ Channel Config */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* SDIO IRQ Channel Config */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* TIM3 IRQ Channel Config */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : Timer_Config
* Description    : Configures Timer.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
void Timer_Config(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;//TIM3ʱ��ʹ��    
 	TIM3->ARR=arr;  //�趨�������Զ���װֵ//�պ�1ms    
	TIM3->PSC=psc;  //Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��
	TIM3->DIER|=1<<0;   //��������ж�
	TIM3->DIER|=1<<6;   //�������ж�
	TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3			 
}


void Key_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIOC->ODR |=(1<<0)| (1<<1)|(1<<2)|(1<<3)|(1<<4);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

}



void I2S_GPIO_config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Disable the JTAG interface and enable the SWJ interface */
 // GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
  /* Configure SPI2 pins: CK, WS and SD */
  //2017109 add by Jin: I2S2 interface
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  //
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIOA->ODR&~(1<<9);

}


 #define GPIO_CS                  GPIOB
 #define RCC_APB2Periph_GPIO_CS   RCC_APB2Periph_GPIOB
 #define GPIO_Pin_CS              GPIO_Pin_0
  
/* Select SPI FLASH: Chip Select pin low  */
#define SPI_FLASH_CS_LOW()       GPIO_ResetBits(GPIO_CS, GPIO_Pin_CS)
/* Deselect SPI FLASH: Chip Select pin high */
#define SPI_FLASH_CS_HIGH()      GPIO_SetBits(GPIO_CS, GPIO_Pin_CS)

void SPI1_GPIO_config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Disable the JTAG interface and enable the SWJ interface */
  /* Configure SPI2 pins: CK, WS and SD */
  /* Configure SPI1 pins: SCK, NSS and LRCK*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure I/O for Flash Chip select */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_CS;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIO_CS,&GPIO_InitStructure);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}


void SPI1_GPIOA6_Reset()
{ 
GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIOA->BRR =1<<(6);
}

void SPI1_Config(void)
{
  SPI_InitTypeDef   SPI_InitStructure;
  //////////////////////SPI2////////////////////////////////////////
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA |
                         RCC_APB2Periph_GPIO_CS, ENABLE);
  SPI1_GPIO_config();
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  /* Enable SPI2 RXNE interrupt */
  SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);
}

void SPI2_GPIO_config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Disable the JTAG interface and enable the SWJ interface */
  /* Configure SPI2 pins: CK, WS and SD */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

}

void SPI2_Config(void)
{
  SPI_InitTypeDef   SPI_InitStructure;
  //////////////////////SPI2////////////////////////////////////////
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  SPI2_GPIO_config();
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2, &SPI_InitStructure);

  /* Enable SPI2 RXNE interrupt */
  SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);
}


void I2S_Config(void)
{ 
  I2S_InitTypeDef I2S_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  I2S_GPIO_config();
  /* I2S peripheral Config */
  I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
  I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_44k;
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_High;

  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;
  I2S_Init(SPI2, &I2S_InitStructure);

  SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_TXE, ENABLE);
}


void I2S_SetFreq(unsigned short freq)
{
I2S_InitTypeDef I2S_InitStructure;
DISABLE_AUDIO;
if(freq!=I2S_AudioFreq_48k && freq !=I2S_AudioFreq_44k && freq!=I2S_AudioFreq_22k && freq!=I2S_AudioFreq_16k && freq!=I2S_AudioFreq_8k)return;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  I2S_GPIO_config();
  /* I2S peripheral Config */
  I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
  I2S_InitStructure.I2S_AudioFreq = freq;
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;

  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;
  I2S_Init(SPI2, &I2S_InitStructure);
}

void ADC_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  /* Configure PC5 (ADC Channel15) as analog input -------------------------*/ 
  GPIO_StructInit(&GPIO_InitStructure); 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
} 


void ADC_Config(void)
{
ADC_InitTypeDef ADC_InitStructure; 

RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);

ADC_GPIO_Config(); 

  /* ADC1 configuration ------------------------------------------------------*/ 
  ADC_StructInit(&ADC_InitStructure); 
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 
  ADC_InitStructure.ADC_ScanConvMode = ENABLE; 
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 
  ADC_InitStructure.ADC_NbrOfChannel = 1; 
  ADC_Init(ADC1, &ADC_InitStructure); 
  /* ADC1 regular channel10 configuration */  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5); 
   
  /* Enable ADC1 */ 
  ADC_Cmd(ADC1, ENABLE); 
  /* Enable ADC1 reset calibaration register */    
  ADC_ResetCalibration(ADC1); 
  /* Check the end of ADC1 reset calibration register */ 
  while(ADC_GetResetCalibrationStatus(ADC1)); 
  /* Start ADC1 calibaration */ 
  ADC_StartCalibration(ADC1); 
  /* Check the end of ADC1 calibration */ 
  while(ADC_GetCalibrationStatus(ADC1)); 
}

