/*********************************************************************************
OOPY MUSIC PLAYER
Copyright (C) 2011, SHARPUFO.HuangRongBin 

OOPY MUSIC PLAYER ��һ��������ֲ�������������STM32F103ZE΢�������ϡ�
֧��44.1K 16bit ��˫���� WAV�ļ��Լ� LV0,LV1,LV2��˫���� FLAC�ļ���
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
The player supports 44.1K 16bit stereo WAV files and LV0,LV1,LV2 FLAC files playback.

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
/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
///////////////////////////////////////////////////////////////////////////////////////////



/********************************************************************************/

/*
PS:
The SDIO write operation in this project has not working well yet.
And I have no  and won't have time to see into it. :)
*/
/********************************************************************************/

/*ALL RIGHT OF ORIGINAL PIECE RESERVED,USE THE CODE UNDER YOUR RESPONSIBILITY!*/
