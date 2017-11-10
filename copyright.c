/*********************************************************************************
OOPY MUSIC PLAYER
Copyright (C) 2011, SHARPUFO.HuangRongBin 

OOPY MUSIC PLAYER 是一个免费音乐播放器，运行于STM32F103ZE微控制器上。
支持44.1K 16bit 的双声道 WAV文件以及 LV0,LV1,LV2的双声道 FLAC文件。
具体硬件配置详见电路原理图.

保留所有原创部分之权利,只允许非商业性、非赢利性使用!
你可以个人性质、非营利性、非商业性的使用、更改和传播本播放器代码。
但本播放器代码作者没有任何义务的维护和技术支持，也不承担任何直接或者间接的
由于使用到本播放器代码而导致的后果和风险!
任何的后果和风险将由使用者自行承担，请谨慎使用!

本播放器代码使用到的其他代码组件版权归原作者所有，请遵循其版权声明!

本播放器代码使用到的FLAC解码代码模块来自于K9spud LLC的TRAXMOD，
并且最顶层的播放代码做了适于本播放器的大幅修改,并加入了快进和快退的功能。
在此向原作者K9spud LLC致谢，并请遵循其版权声明!

本播放器代码使用到ChaN的FATFS R0.07E 文件系统代码模块，并做了小幅修改以适用于本播放器。
在此向原作者ChaN致谢，并请遵循其版权声明!

本播放器代码使用到STM32 V2.0.3的库代码，部分做了修改，
在此向原作者MCD Application Team致谢，并请遵循其版权声明!

如果本播放器代码有使用到您的代码部分并被本人遗落未列出,请通知本人更正！

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
