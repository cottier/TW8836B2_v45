/**
 * @file
 * SOsdMenuMap.c 
 * @author Brian Kang
 * @version 1.0
 * @section LICENSE
 *	Copyright (C) 2011~2012 Intersil Corporation
 * @section DESCRIPTION
 *	SpiOsd Menu Map
*/
/*
Disclaimer: THIS INFORMATION IS PROVIDED 'AS-IS' FOR EVALUATION PURPOSES ONLY.  
INTERSIL CORPORATION AND ITS SUBSIDIARIES ('INTERSIL') DISCLAIM ALL WARRANTIES, 
INCLUDING WITHOUT LIMITATION FITNESS FOR A PARTICULAR PURPOSE AND MERCHANTABILITY.  
Intersil provides evaluation platforms to help our customers to develop products. 
However, factors beyond Intersil's control could significantly affect Intersil 
product performance. 
It remains the customers' responsibility to verify the actual system performance.
*/
//*****************************************************************************
//
//									TW8835 MENU-B.c
//
//*****************************************************************************
// TW8835 have 2KB XDATA, we have to move the static table datas to CODE segment.
// code segment size: 0x10000 64KB * 4Banks.
// xdata segment size: 0x1000 4KB.
//
//SOSD_WIN1 use LutOffset 0x000. SOsdHwBuffWrite2Hw() need it.	   

//parent
//parent_to_me_item_number.

//curr_menu
//curr_focused_item_number

#include "config.h"
#include "reg.h"
#include "typedefs.h"
#include "TW8836.h"

#include "Global.h"
#include "CPU.h"
#include "printf.h"
#include "util.h"

#include "I2C.h"
#ifdef USE_SFLASH_EEPROM
#include "spi.h"
#endif

#include "SOsd.h"
#include "FOsd.h"
#include "InputCtrl.h"
#include "SpiFlashMap.h"
#include "SOsdMenu.h"
#include "eeprom.h"
#include "ImageCtrl.h"
#include "Settings.h"
#include "Demo.h"


#ifdef SPIOSD_USE_800X480_IMG
#ifdef SUPPORT_SPIOSD																			  
//==========================
// LEVEL 0 PAGE
//==========================


//-------------------------------------
// main MENU
//-------------------------------------
//
//		Video	Audio	System
//
//
//		GPS		Phone	Information
//
//
//						demo exit
//-------------------------------------
//description
//background: use 8bit RLE. 128 Color. Use SOSD_WIN1
//            color 1: transparent color. normally color 0 is a black.
//focused(pressed): use SOSD_WIN8
//				

#if 0
code struct menu_sitem_info_s menu_main_page_items[]= {												            //  ^ < V > 
{ TYPE_BG, proc_main_bg,	X_START+0,Y_START+0, 	  ALIGN_TOPLEFT,	&img_main_bg,NULL,     					0,SOSD_WIN1,0x000,  0,0,0,0 },	//0

{ 0, proc_demo_icon,  		X_START+697, Y_START+424, ALIGN_CENTER,  	&img_navi_demo,&img_navi_demo1,  		0,SOSD_WIN8,0x100,  6,2,3,2 },	//1	
{ 0, proc_exit_icon,  		X_START+750, Y_START+424, ALIGN_CENTER,  	&img_navi_close,&img_navi_close1,  		0,SOSD_WIN8,0x100,  6,1,3,1 },	//2	

{ 0, proc_main_input, 		X_START+149, Y_START+89,  ALIGN_CENTER, 	&img_main_input,&img_main_input1,  		0,SOSD_WIN8,0x100,  6,5,6,4 },	//3	
{ 0, proc_main_audio, 		X_START+350, Y_START+84,  ALIGN_CENTER, 	&img_main_audio,&img_main_audio1,  		0,SOSD_WIN8,0x100,  7,3,7,5 },	//4
{ 0, proc_main_system,		X_START+544, Y_START+84,  ALIGN_CENTER, 	&img_main_system,&img_main_system1, 	0,SOSD_WIN8,0x100,  8,4,8,3 },	//5
{ 0, proc_main_gps, 		X_START+147, Y_START+242, ALIGN_CENTER, 	&img_main_gps,&img_main_gps1,    		0,SOSD_WIN8,0x100,  3,8,2,7 },	//6   
{ 0, proc_main_phone, 		X_START+351, Y_START+243, ALIGN_CENTER, 	&img_main_phone,&img_main_phone1,  		0,SOSD_WIN8,0x100,  4,6,2,8 },	//7
{ 0, proc_main_info,  		X_START+551, Y_START+246, ALIGN_CENTER, 	&img_main_carinfo,&img_main_carinfo1,  	0,SOSD_WIN8,0x100,  5,7,2,6 }	//8

};
struct menu_page_info_s menu_main_page = {	MENU_TYPE_NORMAL, 3,3, 3,6,	menu_main_page_items, NULL };			
#else
code struct menu_sitem_info_s menu_main_page_items[]= {												                            //  ^ < V > 
{ TYPE_BG, proc_main_bg,	X_START+0,Y_START+0, 	  ALIGN_TOPLEFT,	&img_main_bg,NULL,     					0,SOSD_WIN1,0x000,  0,0,0,0 },	//0

{ 0, proc_main_input, 		X_START+149, Y_START+89,  ALIGN_CENTER, 	&img_main_input,&img_main_input1,  		0,SOSD_WIN8,0x100,  1,3,1,2 },	//1	
{ 0, proc_main_audio, 		X_START+350, Y_START+84,  ALIGN_CENTER, 	&img_main_audio,&img_main_audio1,  		0,SOSD_WIN8,0x100,  2,1,2,3 },	//2
{ 0, proc_main_system,		X_START+544, Y_START+84,  ALIGN_CENTER, 	&img_main_system,&img_main_system1, 	0,SOSD_WIN8,0x100,  3,2,3,1 },	//3
};
struct menu_page_info_s menu_main_page = {	MENU_TYPE_NORMAL, 3,3, 3,6,	menu_main_page_items, NULL };			
#endif


//==========================
// LEVEL 1 PAGE
//==========================

//----------------------
// Source Select MENU
//main->input(video)
//----------------------
//	cvbs svideo Ypbpr rgb dvi hdmi
//
//
//
//
//					return setting exit
//------------------------------------------
//description
//background: use 8bit RLE. 128 Color. Use SOSD_WIN0
//            color 1: transparent color. normally color 0 is a black.

code struct menu_sitem_info_s menu_input_page_items[]= {													//  ^ < V >
{ TYPE_BG, proc_input_bg,	X_START+0,Y_START+414,	ALIGN_TOPLEFT,	&img_input_bg_bottom,NULL,				0,SOSD_WIN1,0x000,  0,0,0,0  },	//bg_bottom:fixed
{ TYPE_MBG, NULL,			X_START+0,Y_START+0,	ALIGN_TOPLEFT,	&img_input_bg_top,NULL,					0,SOSD_WIN2,0x000,  0,0,0,0  }, //bg_top:move down

{ 0, proc_return_icon,		X_START+651,Y_START+430,ALIGN_CENTER,	&img_input_return,&img_input_return1,	0,SOSD_WIN8,0x100,  MENU_SRC_CVBS,MENU_SRC_EXIT,MENU_SRC_HDMI,MENU_SRC_SETTING },	//14 - 128
{ 0, proc_input_setting,	X_START+695,Y_START+422,ALIGN_CENTER,	&img_navi_setup,&img_navi_setup1,		0,SOSD_WIN8,0x100,  MENU_SRC_CVBS,MENU_SRC_RETURN, MENU_SRC_HDMI,MENU_SRC_EXIT },//15 - 128
{ 0, proc_exit_icon,		X_START+750,Y_START+423,ALIGN_CENTER,	&img_navi_close,&img_navi_close1,		0,SOSD_WIN8,0x100,  MENU_SRC_CVBS,MENU_SRC_SETTING, MENU_SRC_HDMI,MENU_SRC_RETURN  },//16 - 128

{ 0, proc_input_cvbs,		X_START+104,Y_START+0,	ALIGN_CENTER,	&img_input_cvbs,&img_input_cvbs1,		0,SOSD_WIN8,0x100,  MENU_SRC_EXIT,MENU_SRC_BT656,MENU_SRC_RETURN,MENU_SRC_SVIDEO },	//2
{ 0, proc_input_svideo,		X_START+184,Y_START+2,	ALIGN_CENTER,	&img_input_svideo,&img_input_svideo1,	0,SOSD_WIN8,0x100,  MENU_SRC_EXIT,MENU_SRC_CVBS,MENU_SRC_RETURN,MENU_SRC_YPBPR },	//4
{ 0, proc_input_ypbpr,		X_START+276,Y_START+0,	ALIGN_CENTER,	&img_input_ypbpr,&img_input_ypbpr1,		0,SOSD_WIN8,0x100,  MENU_SRC_EXIT,MENU_SRC_SVIDEO,MENU_SRC_RETURN,MENU_SRC_PC },	//6
{ 0, proc_input_pc,			X_START+365,Y_START+1,	ALIGN_CENTER,	&img_input_pc,&img_input_pc1,			0,SOSD_WIN8,0x100,  MENU_SRC_EXIT,MENU_SRC_YPBPR,MENU_SRC_RETURN,MENU_SRC_LVDS },	//8
{ 0, proc_input_lvds,		X_START+455,Y_START+1,	ALIGN_CENTER,	&img_input_dvi,&img_input_dvi1,			0,SOSD_WIN8,0x100,  MENU_SRC_EXIT,MENU_SRC_PC,MENU_SRC_RETURN,MENU_SRC_HDMI },	//10
{ 0, proc_input_hdmi,		X_START+539,Y_START+1,	ALIGN_CENTER,	&img_input_hdmi,&img_input_hdmi1,		0,SOSD_WIN8,0x100,  MENU_SRC_EXIT,MENU_SRC_LVDS,MENU_SRC_RETURN,MENU_SRC_BT656 }, //12 need a disable option
{ 0, proc_input_bt656,		X_START+633,Y_START+3,	ALIGN_CENTER,	&img_input_ext,&img_input_ext1,			0,SOSD_WIN8,0x100,  MENU_SRC_EXIT,MENU_SRC_HDMI,MENU_SRC_RETURN,MENU_SRC_CVBS }, //12 need a disable option

//select
{ 0, NULL,					X_START+104,Y_START+54,	ALIGN_CENTER,	&img_input_select,NULL,					0,SOSD_WIN3,0x000,  0,0,0,0  }, //cvbs - max 128
{ 0, NULL,					X_START+184,Y_START+54,	ALIGN_CENTER,	&img_input_select,NULL,					0,SOSD_WIN3,0x000,  0,0,0,0  }, //svideo - max 128
{ 0, NULL,					X_START+276,Y_START+54,	ALIGN_CENTER,	&img_input_select,NULL,					0,SOSD_WIN3,0x000,  0,0,0,0  }, //ypbpr - max 128
{ 0, NULL,					X_START+365,Y_START+54,	ALIGN_CENTER,	&img_input_select,NULL,					0,SOSD_WIN3,0x000,  0,0,0,0  }, //pc - max 128
{ 0, NULL,					X_START+455,Y_START+54,	ALIGN_CENTER,	&img_input_select,NULL,					0,SOSD_WIN3,0x000,  0,0,0,0  }, //dvi - max 128
{ 0, NULL,					X_START+539,Y_START+54,	ALIGN_CENTER,	&img_input_select,NULL,					0,SOSD_WIN3,0x000,  0,0,0,0  }, //hdmi - max 128
{ 0, NULL,					X_START+633,Y_START+54,	ALIGN_CENTER,	&img_input_select,NULL,					0,SOSD_WIN3,0x000,  0,0,0,0  } //ext - max 128
};
struct menu_page_info_s menu_input_page = {	MENU_TYPE_SCRLDN | MENU_TYPE_NOTE, 5,5, 5,7,	menu_input_page_items, NULL };	//auto focus	

//----------------------
//menu_audio_page
//main->audio
//------------------------------------------
//description
#if 0
code struct menu_sitem_info_s menu_audio_page_items[] = {
{ TYPE_BG,proc_audio_bg,	X_START+0,Y_START+0,	ALIGN_TOPLEFT,	&img_audio_bg,NULL,						0,SOSD_WIN1,0x000,  0,0,0,0},
{ 0,proc_return_icon,		X_START+697,Y_START+423,ALIGN_TOPLEFT,	&img_navi_return,&img_navi_return1,		0,SOSD_WIN8,0x100,  2,2,2,2},
{ 0,proc_exit_icon,			X_START+750,Y_START+423,ALIGN_TOPLEFT,	&img_navi_close,&img_navi_close1,		0,SOSD_WIN8,0x100,  1,1,1,1}
};	  
struct menu_page_info_s menu_audio_page = { MENU_TYPE_NORMAL,1,1, 3,0,	menu_audio_page_items, NULL };		
#endif


//----------------------
// system setup MENU
//main->system
//----------------------
//------------------------------------------
//description

//------------------------------------------
//menu_gps_page
//main->gps
//------------------------------------------
//description
#if 0
code struct menu_sitem_info_s menu_gps_page_items[] = {
{ TYPE_BG,proc_gps_bg,		X_START+0,Y_START+0,	ALIGN_TOPLEFT,	&img_gps_bg,NULL,						0,SOSD_WIN1,0x000,  0,0,0,0},
{ 0,proc_return_icon,		X_START+696,Y_START+423,ALIGN_TOPLEFT,	&img_navi_return,&img_navi_return1,		0,SOSD_WIN8,0x0C0,  2,2,2,2},
{ 0,proc_exit_icon,			X_START+749,Y_START+424,ALIGN_TOPLEFT,	&img_navi_close,&img_navi_close1,		0,SOSD_WIN8,0x0C0,  1,1,1,1}
};	  
struct menu_page_info_s menu_gps_page = { MENU_TYPE_NORMAL, 1,1, 3,0,	menu_gps_page_items, NULL };		
#endif

//------------------------------------------
//menu_phone_page
//main->phone
//------------------------------------------
//description
#if 0
code struct menu_sitem_info_s menu_phone_page_items[] = {
{ TYPE_BG,proc_phone_bg,	X_START+0,Y_START+0,	ALIGN_TOPLEFT,	&img_phone_bg,NULL,						0,SOSD_WIN1,0x000,  0,0,0,0},

{ 0,proc_phone_return,		X_START+698,Y_START+423, ALIGN_TOPLEFT,	&img_navi_return,&img_navi_return1,		0,SOSD_WIN8,0x0C0,  3,25,2,2},	//1
{ 0,proc_exit_icon,			X_START+749,Y_START+423,ALIGN_TOPLEFT,	&img_navi_close,&img_navi_close1,		0,SOSD_WIN8,0x0C0,  3,1,1,21},	//2

{ 0,proc_phone_1,			X_START+ 70,Y_START+ 90,ALIGN_TOPLEFT,	&img_phone_10,&img_phone_11,			0,SOSD_WIN8,0x0C0,  1,5,6,4},	//3
{ 0,proc_phone_2,			X_START+150,Y_START+ 90,ALIGN_TOPLEFT,	&img_phone_20,&img_phone_21,			0,SOSD_WIN8,0x0C0,  1,3,7,5},	//4
{ 0,proc_phone_3,			X_START+230,Y_START+ 90,ALIGN_TOPLEFT,	&img_phone_30,&img_phone_31,			0,SOSD_WIN8,0x0C0,  1,4,8,3},	//5

{ 0,proc_phone_4,			X_START+ 70,Y_START+145,ALIGN_TOPLEFT,	&img_phone_40,&img_phone_41,			0,SOSD_WIN8,0x0C0,  3,8,9,7},	//6
{ 0,proc_phone_5,			X_START+150,Y_START+145,ALIGN_TOPLEFT,	&img_phone_50,&img_phone_51,			0,SOSD_WIN8,0x0C0,  4,6,10,8},	//7
{ 0,proc_phone_6,			X_START+230,Y_START+145,ALIGN_TOPLEFT,	&img_phone_60,&img_phone_61,			0,SOSD_WIN8,0x0C0,  5,7,11,6},	//8

{ 0,proc_phone_7,			X_START+ 70,Y_START+200,ALIGN_TOPLEFT,	&img_phone_70,&img_phone_71,			0,SOSD_WIN8,0x0C0,  6,11,12,10},//9
{ 0,proc_phone_8,			X_START+150,Y_START+200,ALIGN_TOPLEFT,	&img_phone_80,&img_phone_81,			0,SOSD_WIN8,0x0C0,  7,9,13,11},	//10
{ 0,proc_phone_9,			X_START+230,Y_START+200,ALIGN_TOPLEFT,	&img_phone_90,&img_phone_91,			0,SOSD_WIN8,0x0C0,  8,10,14,19},//11

{ 0,proc_phone_star,		X_START+ 70,Y_START+255,ALIGN_TOPLEFT,	&img_phone_star0,&img_phone_star1,		0,SOSD_WIN8,0x0C0,  9,14,15,13},//12
{ 0,proc_phone_0,			X_START+149,Y_START+255,ALIGN_TOPLEFT,	&img_phone_00,&img_phone_01,			0,SOSD_WIN8,0x0C0,  10,12,15,14},//13
{ 0,proc_phone_sharp,		X_START+230,Y_START+255,ALIGN_TOPLEFT,	&img_phone_sharp0,&img_phone_sharp1,	0,SOSD_WIN8,0x0C0,  11,13,15,12},//14

{ 0,proc_phone_dial,		X_START+ 70,Y_START+315,ALIGN_TOPLEFT,	&img_phone_dial0,&img_phone_dial1,		0,SOSD_WIN8,0x0C0,  13,20,21,16},//15

{ 0,proc_phone_up,			X_START+351,Y_START+315,ALIGN_TOPLEFT,	&img_phone_up0,&img_phone_up1,			0,SOSD_WIN8,0x0C0,  1,15,1,17},	//16
{ 0,proc_phone_down,		X_START+431,Y_START+315,ALIGN_TOPLEFT,	&img_phone_down0,&img_phone_down1,		0,SOSD_WIN8,0x0C0,  1,16,1,18},	//17
{ 0,proc_phone_check,		X_START+511,Y_START+315,ALIGN_TOPLEFT,	&img_phone_check0,&img_phone_check1,	0,SOSD_WIN8,0x0C0,  1,17,1,19},	//18
{ 0,proc_phone_left,		X_START+591,Y_START+315,ALIGN_TOPLEFT,	&img_phone_left0,&img_phone_left1,		0,SOSD_WIN8,0x0C0,  1,18,1,20},	//19
{ 0,proc_phone_right,		X_START+671,Y_START+315,ALIGN_TOPLEFT,	&img_phone_right0,&img_phone_right1,	0,SOSD_WIN8,0x0C0,  1,19,1,15},	//20

{ 0,proc_phone_help,		X_START+ 15,Y_START+422,ALIGN_TOPLEFT,	&img_phone_help0,&img_phone_help1,		0,SOSD_WIN8,0x0C0,  15,2,3,22},	//21
{ 0,proc_phone_dir,			X_START+123,Y_START+422,ALIGN_TOPLEFT,	&img_phone_dir0,&img_phone_dir1,		0,SOSD_WIN8,0x0C0,  15,21,3,23},//22
{ 0,proc_phone_set,			X_START+263,Y_START+422,ALIGN_TOPLEFT,	&img_phone_set0,&img_phone_set1,		0,SOSD_WIN8,0x0C0,  15,22,3,24},//23
{ 0,proc_phone_msg,			X_START+403,Y_START+422,ALIGN_TOPLEFT,	&img_phone_msg0,&img_phone_msg1,		0,SOSD_WIN8,0x0C0,  15,23,3,25},//24
{ 0,proc_phone_menu,		X_START+543,Y_START+422,ALIGN_TOPLEFT,	&img_phone_menu0,&img_phone_menu1,		0,SOSD_WIN8,0x0C0,  15,24,3,1}	//25

};	  
code struct menu_fitem_info_s menu_phone_text_items[] = {
//	{ 0,0,	520-220,324-260,	10,2, 0,0,	0xF0,	 16,0x07E0	},
//	{ 0,0,	396,163,	10,2, 0,0,	0xF0,	 16,0x07E0	},
	{ 0,0,	X_START+396,Y_START+163,	15,2, 0,0,	0x1A,	 16,0x07E0	},	//win0,osdram0~29
	{ 1,40,	X_START+396,Y_START+241,	15,1, 0,0,	0x1A,	 16,0x07E0	},	//win1,osdram40~69
};
struct menu_page_info_s menu_phone_page = { MENU_TYPE_NORMAL, 1,1, 3,23,	menu_phone_page_items, menu_phone_text_items };		
#endif



//------------------------------------------
//menu_car_info_page
//main->car_info
//------------------------------------------
//description
#if 0
code struct menu_sitem_info_s menu_carinfo_page_items[] = {
{ TYPE_BG,proc_carinfo_bg,	X_START+0,Y_START+0,	ALIGN_TOPLEFT,	&img_carinfo_bg,NULL,					0,SOSD_WIN1,0x000,  0,0,0,0},
{ 0,proc_return_icon,		X_START+697,Y_START+423,ALIGN_TOPLEFT,	&img_navi_return,&img_navi_return1,		0,SOSD_WIN8,0x0C0,  2,2,2,2},
{ 0,proc_exit_icon,			X_START+748,Y_START+423,ALIGN_TOPLEFT,	&img_navi_close,&img_navi_close1,		0,SOSD_WIN8,0x0C0,  1,1,1,1}
};	  
struct menu_page_info_s menu_carinfo_page = { MENU_TYPE_NORMAL, 1,1, 3,0,	menu_carinfo_page_items, NULL };		
#endif

//------------------------------------------
//menu_demo_page
//main->demo
//test 4BPP RLE
//------------------------------------------
//proc_pigeon_demo_start
#if 0
code struct menu_sitem_info_s menu_demo_page_items[] = {												   //  ^ < V >
{ TYPE_BG,proc_demo_bg,		X_START+0,Y_START+0,	ALIGN_TOPLEFT,	&img_demo_bg,NULL,							0,SOSD_WIN1,0x000,  0,0,0,0},

{ 0,proc_return_icon,		X_START+701,Y_START+425,ALIGN_TOPLEFT,	&img_navi_return,&img_navi_return,			0,SOSD_WIN8,0x100,  6,2,3,2}, //1
{ 0,proc_exit_icon,			X_START+752,Y_START+426,ALIGN_TOPLEFT,	&img_navi_close,&img_navi_close1,			0,SOSD_WIN8,0x100,  6,1,3,1}, //2

{ 0,proc_grid_demo_start,	X_START+149,Y_START+90,ALIGN_TOPLEFT,	&img_demo_grid,&img_demo_grid1,				0,SOSD_WIN8,0x100,  6,5,6,4}, //3
{ 0,proc_rose_demo_start,	X_START+344,Y_START+89,ALIGN_TOPLEFT,	&img_demo_rose,&img_demo_rose1,				0,SOSD_WIN8,0x100,  7,3,7,5}, //4
{ 0,proc_wait_demo_start,	X_START+559,Y_START+90,ALIGN_TOPLEFT,	&img_demo_ani,&img_demo_ani1,				0,SOSD_WIN8,0x100,  8,4,8,3}, //5
{ 0,proc_FPalette_demo_start,	X_START+147,Y_START+242,ALIGN_TOPLEFT,	&img_demo_palette,&img_demo_palette1,		0,SOSD_WIN8,0x100,  3,8,2,7}, //6
{ 0,proc_SOsd_demo1_start,	X_START+351,Y_START+243,ALIGN_TOPLEFT,	&img_demo_demoA,&img_demo_demoA1,			0,SOSD_WIN8,0x100,  4,6,2,8}, //7
{ 0,proc_SOsd_demo2_start,	X_START+551,Y_START+246,ALIGN_TOPLEFT,	&img_demo_demoB,&img_demo_demoB1,			0,SOSD_WIN8,0x100,  5,7,2,6}  //8
};	  
struct menu_page_info_s menu_demo_page = { MENU_TYPE_NORMAL, 3,3, 3,6,	menu_demo_page_items, NULL };		
#endif


//------------------------
// level 2 page
//------------------------



//----------------------
//touch calibration page
//main->setting->touch
//----------------------


//==========================
// LIST MENU
//==========================

//----------------------
// internal decoder image Select MENU
//manu->input->setting with CVBS or SVideo
//----------------------
//					return setting exit
// Brightness
// Contrast
// Hue
// Saturation
// Sharpness
//
//------------------------------------------
//description
//use global input
//background: use 8bit RLE. 128 Color. Use SOSD_WIN0
//            color 1: transparent color. normally color 0 is a black.
#if 0
code struct menu_sitem_info_s menu_yuv_list_page_items[]= {											  //  ^ < V >
{ TYPE_BG, proc_yuv_list_bg,	X_START+620,Y_START+420,ALIGN_TOPLEFT,	&img_navi_menu,NULL,					0,SOSD_WIN3,0x180,  0,0,0,0},//0
{ TYPE_MBG, NULL,				X_START+0,Y_START+0,	ALIGN_TOPLEFT,	&img_yuv_menu_bg,NULL,					0,SOSD_WIN1,0x000,  0,0,0,0},//1

{ 0, proc_menu_list_return,		X_START+623,Y_START+424,ALIGN_TOPLEFT,	&img_navi_return,&img_navi_return1,		0,SOSD_WIN8,0x100,  5,4,9,3},//2
{ 0, proc_home_icon,			X_START+682,Y_START+424,ALIGN_TOPLEFT,	&img_navi_home,&img_navi_home1,			0,SOSD_WIN8,0x100,  5,2,9,4},//3
{ 0, proc_exit_icon,			X_START+743,Y_START+424,ALIGN_TOPLEFT,	&img_navi_close,&img_navi_close1,		0,SOSD_WIN8,0x100,  5,3,9,2},//4

{ 0, proc_yuv_list_bright,		X_START+0,Y_START+61,	ALIGN_TOPLEFT,	&img_yuv_bright,&img_yuv_bright1,		0,SOSD_WIN8,0x100,  9,2,6,2},//5
{ 0, proc_yuv_list_contrast,	X_START+0,Y_START+133,	ALIGN_TOPLEFT,	&img_yuv_contrast,&img_yuv_contrast1,	0,SOSD_WIN8,0x100,  5,2,7,2},//6
{ 0, proc_yuv_list_hue,			X_START+0,Y_START+205,	ALIGN_TOPLEFT,	&img_yuv_hue,&img_yuv_hue1,				0,SOSD_WIN8,0x100,  6,2,8,2},//7
{ 0, proc_yuv_list_saturate,	X_START+0,Y_START+277,	ALIGN_TOPLEFT,	&img_yuv_saturate,&img_yuv_saturate1,	0,SOSD_WIN8,0x100,  7,2,9,2},//8
{ 0, proc_yuv_list_sharp,		X_START+0,Y_START+349,	ALIGN_TOPLEFT,	&img_yuv_sharp,&img_yuv_sharp1,			0,SOSD_WIN8,0x100,  8,2,5,2} //9
};
struct menu_page_info_s menu_yuv_list_page = { MENU_TYPE_LIST, 5,5, 5,5,	menu_yuv_list_page_items, NULL };		
#else
code struct menu_sitem_info_s menu_yuv_list_page_items[]= {											  							//  ^ < V >
{ TYPE_MBG, NULL,				X_START+0,Y_START+0,	ALIGN_TOPLEFT,	&img_yuv_menu_bg,NULL,					0,SOSD_WIN1,0x000,  0,0,0,0},//0

{ 0, proc_yuv_list_bright,		X_START+0,Y_START+61,	ALIGN_TOPLEFT,	&img_yuv_bright,&img_yuv_bright1,		0,SOSD_WIN8,0x100,  2,1,2,1},//1
{ 0, proc_yuv_list_contrast,	X_START+0,Y_START+133,	ALIGN_TOPLEFT,	&img_yuv_contrast,&img_yuv_contrast1,	0,SOSD_WIN8,0x100,  1,2,1,2},//2
};
struct menu_page_info_s menu_yuv_list_page = { MENU_TYPE_LIST, 2,2, 2,2,	menu_yuv_list_page_items, NULL };		
#endif


#if 0
code struct menu_sitem_info_s menu_rgb_list_page_items[]= {											//  ^ < V >
{ TYPE_BG, proc_rgb_list_bg,	X_START+620,Y_START+420,ALIGN_TOPLEFT,	&img_navi_menu,NULL,					0,SOSD_WIN3,0x180,  0,0,0,0},
{ TYPE_MBG, NULL,				X_START+0,Y_START+0,	ALIGN_TOPLEFT,	&img_rgb_menu_bg,NULL,					0,SOSD_WIN1,0x000,  0,0,0,0},

{ 0, proc_menu_list_return,		X_START+623,Y_START+424,ALIGN_TOPLEFT,	&img_navi_return,&img_navi_return1,		0,SOSD_WIN8,0x100,  5,4,7,3},//2
{ 0, proc_home_icon,			X_START+682,Y_START+424,ALIGN_TOPLEFT,	&img_navi_home,&img_navi_home1,			0,SOSD_WIN8,0x100,  5,2,7,4},//3
{ 0, proc_exit_icon,			X_START+743,Y_START+424,ALIGN_TOPLEFT,	&img_navi_close,&img_navi_close1,		0,SOSD_WIN8,0x100,  5,3,7,2},//4

{ 0, proc_rgb_list_bright,		X_START+0,Y_START+61,	ALIGN_TOPLEFT,	&img_rgb_bright,&img_rgb_bright1,		0,SOSD_WIN8,0x100,  7,2,6,2},//5
{ 0, proc_rgb_list_contrast,	X_START+0,Y_START+181,	ALIGN_TOPLEFT,	&img_rgb_contrast,&img_rgb_contrast1,	0,SOSD_WIN8,0x100,  5,2,7,2},//6
{ 0, proc_rgb_list_color,		X_START+0,Y_START+301,	ALIGN_TOPLEFT,	&img_rgb_color,&img_rgb_color1,			0,SOSD_WIN8,0x100,  6,2,5,2} //7
};
struct menu_page_info_s menu_rgb_list_page = { MENU_TYPE_LIST, 5,5, 5,3,	menu_rgb_list_page_items, NULL };		
#endif

//----------------------
// ADC image Select MENU
//manu->input->setting with RGB or HDMI
//----------------------
//					return setting exit
// R Gain
// G Gain
// B Gain
// Phase
//
//------------------------------------------
//description
//use global input
//background: use 8bit RLE. 128 Color. Use SOSD_WIN0
//            color 1: transparent color. normally color 0 is a black.

#if 0
code struct menu_sitem_info_s menu_apc_list_page_items[]= {											//  ^ < V >
{ TYPE_BG, proc_apc_list_bg,	X_START+620,Y_START+420,ALIGN_TOPLEFT,	&img_navi_menu,NULL,					0,SOSD_WIN3,0x180,  0,0,0,0},//0
{ TYPE_MBG, NULL,				X_START+0,Y_START+0,	ALIGN_TOPLEFT,	&img_apc_menu_bg,NULL,					0,SOSD_WIN1,0x000,  0,0,0,0},//1

{ 0, proc_menu_list_return,		X_START+623,Y_START+424,ALIGN_TOPLEFT,	&img_navi_return,&img_navi_return1,		0,SOSD_WIN8,0x100,  5,4,12,3},//2
{ 0, proc_home_icon,			X_START+682,Y_START+424,ALIGN_TOPLEFT,	&img_navi_home,&img_navi_home1,			0,SOSD_WIN8,0x100,  5,2,12,4},//3
{ 0, proc_exit_icon,			X_START+743,Y_START+424,ALIGN_TOPLEFT,	&img_navi_close,&img_navi_close1,		0,SOSD_WIN8,0x100,  5,3,12,2},//4

{ 0, proc_apc_list_bright,		X_START+0,Y_START+61,	ALIGN_TOPLEFT,	&img_apc_bright,&img_apc_bright1,		0,SOSD_WIN8,0x100,  12,2,6,2},//5
{ 0, proc_apc_list_contrast,	X_START+0,Y_START+106,	ALIGN_TOPLEFT,	&img_apc_contrast,&img_apc_contrast1,	0,SOSD_WIN8,0x100,  5,2,7,2}, //6
{ 0, proc_apc_list_color,		X_START+0,Y_START+151,	ALIGN_TOPLEFT,	&img_apc_color,&img_apc_color1,			0,SOSD_WIN8,0x100,  6,2,8,2}, //7
{ 0, proc_apc_list_position,	X_START+0,Y_START+196,	ALIGN_TOPLEFT,	&img_apc_position,&img_apc_position1,	0,SOSD_WIN8,0x100,  7,2,9,2}, //8
{ 0, proc_apc_list_phase,		X_START+0,Y_START+241,	ALIGN_TOPLEFT,	&img_apc_phase,&img_apc_phase1,			0,SOSD_WIN8,0x100,  8,2,10,2}, //9
{ 0, proc_apc_list_clock,		X_START+0,Y_START+286,	ALIGN_TOPLEFT,	&img_apc_pclock,&img_apc_pclock1,		0,SOSD_WIN8,0x100,  9,2,11,2},//10
{ 0, proc_apc_list_autoadj,		X_START+0,Y_START+331,	ALIGN_TOPLEFT,	&img_apc_autoadj,&img_apc_autoadj1,		0,SOSD_WIN8,0x100,  10,2,12,2},//11
{ 0, proc_apc_list_autocolor,	X_START+0,Y_START+376,	ALIGN_TOPLEFT,	&img_apc_autocolor,&img_apc_autocolor1,	0,SOSD_WIN8,0x100,  11,2,5,2} //12
};
struct menu_page_info_s menu_apc_list_page = { MENU_TYPE_LIST,	5,5, 5,8,	menu_apc_list_page_items, NULL };		
#endif


#if 0
code struct menu_sitem_info_s menu_hdmi_list_page_items[]= {											//  ^ < V >
{ TYPE_BG, proc_hdmi_list_bg,	X_START+620,Y_START+420,ALIGN_TOPLEFT,	&img_navi_menu,NULL,					0,SOSD_WIN3,0x180,  0,0,0,0},
{ TYPE_MBG, NULL,				X_START+0,Y_START+0,	ALIGN_TOPLEFT,	&img_hdmi_menu_bg,NULL,					0,SOSD_WIN1,0x000,  0,0,0,0},

{ 0, proc_menu_list_return,		X_START+623,Y_START+424,ALIGN_TOPLEFT,	&img_navi_return,&img_navi_return1,		0,SOSD_WIN8,0x100,  5,4,6,3},//2
{ 0, proc_home_icon,			X_START+682,Y_START+424,ALIGN_TOPLEFT,	&img_navi_home,&img_navi_home1,			0,SOSD_WIN8,0x100,  5,2,6,4},//3
{ 0, proc_exit_icon,			X_START+743,Y_START+424,ALIGN_TOPLEFT,	&img_navi_close,&img_navi_close1,		0,SOSD_WIN8,0x100,  5,3,6,2}, //4

{ 0, proc_hdmi_list_mode,		X_START+0,Y_START+61,	ALIGN_TOPLEFT,	&img_hdmi_mode,&img_hdmi_mode1,			0,SOSD_WIN8,0x100,  6,2,6,2},//5
{ 0, proc_hdmi_list_setting,	X_START+0,Y_START+241,	ALIGN_TOPLEFT,	&img_hdmi_setting,&img_hdmi_setting1,	0,SOSD_WIN8,0x100,  5,2,5,2} //6

};
struct menu_page_info_s menu_hdmi_list_page = {	MENU_TYPE_LIST, 5,5, 5,2,	menu_hdmi_list_page_items, NULL };		
#endif


//----------------------
//ImageEnhanced block image on the Back-End
//main->setting->display
//----------------------
//					
//
//
//
// 
//
//------------------------------------------
//description


//----------------------
// main->system->display->OSD MENU
//----------------------
//					
//
//
//
// 
//
//------------------------------------------
//description
#if 0
code struct menu_sitem_info_s menu_osd_page_items[] = {														//  ^ < V >
{ TYPE_BG,proc_osd_bg,		X_START+620,Y_START+420,ALIGN_TOPLEFT,	&img_navi_menu,NULL,				0,SOSD_WIN3,0x180,  0,0,0,0},
{ TYPE_MBG,NULL,			X_START+0,Y_START+0,	ALIGN_TOPLEFT,	&img_osd_bg,NULL,					0,SOSD_WIN1,0x000,  0,0,0,0},

{ 0,proc_menu_list_return,	X_START+623,Y_START+424,ALIGN_TOPLEFT,	&img_navi_return,&img_navi_return1,	0,SOSD_WIN8,0x100,  5,4,6,3},//2
{ 0,proc_home_icon,			X_START+682,Y_START+424,ALIGN_TOPLEFT,	&img_navi_home,&img_navi_home1,		0,SOSD_WIN8,0x100,  5,2,6,4},//3
{ 0,proc_exit_icon,			X_START+743,Y_START+424,ALIGN_TOPLEFT,	&img_navi_close,&img_navi_close1,	0,SOSD_WIN8,0x100,  5,3,6,2},//4
																									
{ 0,proc_osd_time,			X_START+0,Y_START+61,	ALIGN_TOPLEFT,	&img_osd_timer,NULL,				0,SOSD_WIN8,0x100,  6,2,6,2},//5
{ 0,proc_osd_trans,			X_START+0,Y_START+241,	ALIGN_TOPLEFT,	&img_osd_trans,NULL,				0,SOSD_WIN8,0x100,  5,2,5,2} //6
};
struct menu_page_info_s menu_osd_page = { MENU_TYPE_LIST, 5,5, 5,2,	menu_osd_page_items, NULL };		
#endif


//======================================
// DIALOG
//======================================
//manu->input->setting with DVI
#if 0
code struct menu_sitem_info_s menu_dvi_mode_page_items[]= {											 			// ^ < V >
{ TYPE_BG, proc_dvi_mode_bg,X_START+220,Y_START+220,ALIGN_TOPLEFT,	&img_dvi_mode_bg,NULL,						0,SOSD_WIN1,0x000,  0,0,0,0},

{ 0, proc_dvi_mode_ok,		X_START+306,Y_START+385,ALIGN_TOPLEFT,	&img_dialog_ok,&img_dialog_ok1,				0,SOSD_WIN8,0x100,  3,2,4,2},//1
{ 0, proc_dvi_mode_cancel,	X_START+381,Y_START+385,ALIGN_TOPLEFT,	&img_dialog_cancel,&img_dialog_cancel1,		0,SOSD_WIN8,0x100,  3,1,4,1},//2

{ 0, proc_dvi_mode_24bit,	X_START+285,Y_START+328,ALIGN_TOPLEFT,	&img_dvi_mode_24bit,&img_dvi_mode_24bit1,	0,SOSD_WIN8,0x100,  1,4,1,4},//3
{ 0, proc_dvi_mode_16bit,	X_START+436,Y_START+327,ALIGN_TOPLEFT,	&img_dvi_mode_16bit,&img_dvi_mode_16bit1,	0,SOSD_WIN8,0x100,  1,3,1,3},//4
//select
{ 0, NULL,					X_START+296,Y_START+273,ALIGN_TOPLEFT,	&img_dvi_mode_select24,NULL,				0,SOSD_WIN3,0x180,  0,0,0,0},//3+2 
{ 0, NULL,					X_START+445,Y_START+274,ALIGN_TOPLEFT,	&img_dvi_mode_select16,NULL,				0,SOSD_WIN3,0x180,  0,0,0,0} //4+2 
};
struct menu_page_info_s menu_dvi_mode_page = { MENU_TYPE_DIALOG | MENU_TYPE_NOTE, 3,3, 3,2,	menu_dvi_mode_page_items, NULL };		
#endif


//manu->input->setting with HDMI
#if 0
code struct menu_sitem_info_s menu_hdmi_mode_page_items[]= {												 				//  ^ < V >
{ TYPE_BG, proc_hdmi_mode_bg,	X_START+220,Y_START+220,ALIGN_TOPLEFT,	&img_hdmi_mode_bg,NULL,						0,SOSD_WIN1,0x000,  0,0,0,0},

{ 0, proc_hdmi_mode_ok,			X_START+306,Y_START+385,ALIGN_TOPLEFT,	&img_dialog_ok,&img_dialog_ok1,				0,SOSD_WIN8,0x100,  3,2,4,2},//1
{ 0, proc_hdmi_mode_cancel,		X_START+381,Y_START+385,ALIGN_TOPLEFT,	&img_dialog_cancel,&img_dialog_cancel1,		0,SOSD_WIN8,0x100,  3,1,4,1},//2
																									
{ 0, proc_hdmi_mode_pc,			X_START+274,Y_START+327,ALIGN_TOPLEFT,	&img_hdmi_mode_pc,&img_hdmi_mode_pc1,		0,SOSD_WIN8,0x100,  1,4,1,4},//3
{ 0, proc_hdmi_mode_tv,			X_START+424,Y_START+328,ALIGN_TOPLEFT,	&img_hdmi_mode_tv,&img_hdmi_mode_tv1,		0,SOSD_WIN8,0x100,  1,3,1,3},//4
//select area																							
{ 0, proc_hdmi_mode_pc,			X_START+282,Y_START+271,ALIGN_TOPLEFT,	&img_hdmi_mode_selectPC,NULL, 				0,SOSD_WIN3,0x180,  0,0,0,0},//3+2 
{ 0, proc_hdmi_mode_tv,			X_START+436,Y_START+267,ALIGN_TOPLEFT,	&img_hdmi_mode_selectTV,NULL,				0,SOSD_WIN3,0x180,  0,0,0,0} //4+2 
};
//struct menu_page_info_s menu_hdmi_mode_page = {	MENU_TYPE_DIALOG | MENU_TYPE_NOTE, 3,3, 3,2,	menu_hdmi_mode_page_items, NULL };		
struct menu_page_info_s menu_hdmi_mode_page = {	MENU_TYPE_DIALOG | MENU_TYPE_NOTE, 3,3, 3,2,	menu_hdmi_mode_page_items, NULL };		
#endif

//----------------------
// autoadj popup MENU
// main->input->adc->display->autoadj MENU
//----------------------
//					
//------------------------------------------
//description
#if 0
code struct menu_sitem_info_s menu_autoadj_page_items[] = {
{ TYPE_BG,proc_autoadj_bg,		X_START+220,Y_START+260,ALIGN_TOPLEFT,	&img_autoadj_bg,NULL,					0,SOSD_WIN1,0x000,  0,0,0,0},
{ 0,proc_autoadj_ok,			X_START+306,Y_START+385,ALIGN_TOPLEFT,	&img_dialog_ok,&img_dialog_ok1,			0,SOSD_WIN8,0x080,  2,2,2,2},
{ 0,proc_autoadj_cancel,		X_START+381,Y_START+385,ALIGN_TOPLEFT,	&img_dialog_cancel,&img_dialog_cancel1,	0,SOSD_WIN8,0x080,  1,1,1,1},
//wait image. I need a image & WIN & LUT loc.
{ 0,proc_wait_menu_start,		X_START+376,Y_START+215,ALIGN_TOPLEFT,	&img_wait,NULL,							0,SOSD_WIN0,0x100,  0,0,0,0}
};	  
struct menu_page_info_s menu_autoadj_page = { MENU_TYPE_DIALOG, 1,1, 3,0,	menu_autoadj_page_items, NULL };		
#endif



#if 0
code struct menu_sitem_info_s menu_autocolor_page_items[] = {
{ TYPE_BG,proc_autocolor_bg,	X_START+220,Y_START+260,ALIGN_TOPLEFT,	&img_autocolor_bg,NULL,						0,SOSD_WIN1,0x000,  0,0,0,0},
{ 0,proc_autocolor_ok,			X_START+306,Y_START+385,ALIGN_TOPLEFT,	&img_dialog_ok,&img_dialog_ok1,				0,SOSD_WIN8,0x080,  2,2,2,2},
{ 0,proc_autocolor_cancel,		X_START+381,Y_START+385,ALIGN_TOPLEFT,	&img_dialog_cancel,&img_dialog_cancel1,		0,SOSD_WIN8,0x080,  1,1,1,1},
//wait image. I need a image & WIN & LUT loc.
{ 0,proc_wait_menu_start,		X_START+376,Y_START+215,ALIGN_TOPLEFT,	&img_wait,NULL,								0,SOSD_WIN0,0x100,  0,0,0,0}
};	  
struct menu_page_info_s menu_autocolor_page = { MENU_TYPE_DIALOG, 1,1, 3,0,	menu_autocolor_page_items, NULL };		
#endif

// aspect
//----------------------
// aspect popup MENU
//main->system->display->aspect
//----------------------
//------------------------------------------
//description
#if 0
code struct menu_sitem_info_s menu_aspect_page_items[] = {												   //^ < V >

{ TYPE_BG,proc_aspect_bg,	X_START+220,Y_START+220,ALIGN_TOPLEFT,	&img_popup_aspect_bg,NULL,		0,SOSD_WIN1,0x000,  0,0,0,0},

{ 0,proc_aspect_ok,			X_START+306,Y_START+385,ALIGN_TOPLEFT,	&img_dialog_ok,&img_dialog_ok1,					0,SOSD_WIN8,0x080,  3,2,6,2},//1
{ 0,proc_aspect_cancel,		X_START+381,Y_START+385,ALIGN_TOPLEFT,	&img_dialog_cancel,&img_dialog_cancel1,			0,SOSD_WIN8,0x080,  3,1,6,1},//2

{ 0,proc_aspect_normal,		X_START+229,Y_START+327,ALIGN_TOPLEFT,	&img_popup_aspect_normal,&img_popup_aspect_normal1,	0,SOSD_WIN8,0x080,  1,6,1,4},//3
{ 0,proc_aspect_zoom,		X_START+318,Y_START+329,ALIGN_TOPLEFT,	&img_popup_aspect_zoom,&img_popup_aspect_zoom1,		0,SOSD_WIN8,0x080,  1,3,1,5},//4
{ 0,proc_aspect_full,		X_START+410,Y_START+328,ALIGN_TOPLEFT,	&img_popup_aspect_full,&img_popup_aspect_full1,		0,SOSD_WIN8,0x080,  1,4,1,6},//5
{ 0,proc_aspect_pano,		X_START+468,Y_START+328,ALIGN_TOPLEFT,	&img_popup_aspect_pano,&img_popup_aspect_pano1,		0,SOSD_WIN8,0x080,  1,5,1,3},//6

{ 0, proc_aspect_normal,	X_START+240,Y_START+284,ALIGN_TOPLEFT,	&img_popup_aspect_normal_select,NULL,				0,SOSD_WIN3,0x180,  0,0,0,0},//3+4 
{ 0, proc_aspect_zoom,		X_START+324,Y_START+284,ALIGN_TOPLEFT,	&img_popup_aspect_zoom_select,NULL,					0,SOSD_WIN3,0x180,  0,0,0,0},//4+4 
{ 0, proc_aspect_full,		X_START+408,Y_START+284,ALIGN_TOPLEFT,	&img_popup_aspect_full_select,NULL,					0,SOSD_WIN3,0x180,  0,0,0,0},//5+4 
{ 0, proc_aspect_pano,		X_START+491,Y_START+284,ALIGN_TOPLEFT,	&img_popup_aspect_pano_sel,NULL,					0,SOSD_WIN3,0x180,  0,0,0,0} //6+4 
};
struct menu_page_info_s menu_aspect_page = { MENU_TYPE_DIALOG | MENU_TYPE_NOTE, 3,3, 3,4,	menu_aspect_page_items, NULL };		
#endif

//----------------------
// flip popup MENU
// main->system->display->flip MENU
//----------------------
//
//------------------------------------------
//description

//----------------------
// flip popup MENU
// main->system->display->resolution MENU
//----------------------
//
//------------------------------------------
//description

//----------------------
// restore popup MENU
// main->system->restore MENU
//----------------------//
//------------------------------------------
//description



//----------------------
// info popup MENU
// main->system->sys_info MENU
//----------------------
//					
//------------------------------------------
//description


//=====================
// SLIDER
//=====================

//------------------------------------------
//description
//	item[2].x item[2].y  : start of slide bar
//	item[3].x item[3].y  :   end of slide bar
#if 0
code struct menu_sitem_info_s menu_slider_page_items[] = {											//  ^ < V >
{ TYPE_BG, proc_slider_bg,		X_START+220,Y_START+260,ALIGN_TOPLEFT,	&img_slide_bg,NULL,						0,SOSD_WIN1,0x000,  0,0,0,0},
{ TYPE_BG, NULL,				X_START+243,Y_START+285,ALIGN_TOPLEFT,	&img_slide_bg,NULL,						0,SOSD_WIN2,0x040,  0,0,0,0}, //title

{ 0, proc_slider_ok,			X_START+306,Y_START+385,ALIGN_TOPLEFT,	&img_dialog_ok,&img_dialog_ok1,			0,SOSD_WIN8,0x100,  5,3,5,3},	//2	 
{ 0, proc_slider_undo,			X_START+381,Y_START+385,ALIGN_TOPLEFT,	&img_dialog_cancel,&img_dialog_cancel1,	0,SOSD_WIN8,0x100,  5,2,5,2},	//3

{ TYPE_SLIDE, proc_slider_ctrl,	X_START+242-220, Y_START+320-260,ALIGN_TOPMIDDLE,&img_slide_gray,NULL,			0,SOSD_WIN3,0x080,  2,0,2,0},	//start x,y. Gray 
{ TYPE_SLIDE, proc_slider_ctrl,	X_START+488-220, Y_START+370-260,ALIGN_TOPMIDDLE,&img_slide_red,NULL,			0,SOSD_WIN8,0x100,  2,0,2,0},	//end x,y.   Red

{ 0, proc_slider_left_tsc,		X_START+239, Y_START+385,ALIGN_TOPLEFT,	NULL,&img_slide_left1,					0,SOSD_WIN4,0x180,  0,0,0,0},	//
{ 0, proc_slider_right_tsc,		X_START+496, Y_START+386,ALIGN_TOPLEFT,	NULL,&img_slide_right1,					0,SOSD_WIN4,0x180,  0,0,0,0}	//
};
code struct menu_fitem_info_s menu_slider_text_items[] = {
	{ 0,0,	X_START+520-220,Y_START+324-260,	3,1, 0,0,	0xF0,	 16,0x07E0	},
};
struct menu_page_info_s menu_slider_page = { MENU_TYPE_SLIDE, 4,4, 4,1*2+2,	 menu_slider_page_items, menu_slider_text_items }; //note: 1 item
#else
code struct menu_sitem_info_s menu_slider_page_items[] = {											//  ^ < V >
{ TYPE_BG, proc_slider_bg,		X_START+220,Y_START+260,ALIGN_TOPLEFT,	&img_slide_bg,NULL,						0,SOSD_WIN1,0x000,  0,0,0,0},
{ TYPE_BG, NULL,				X_START+243,Y_START+285,ALIGN_TOPLEFT,	&img_slide_bg,NULL,						0,SOSD_WIN2,0x040,  0,0,0,0}, //title

{ 0, proc_slider_ok,			X_START+306,Y_START+385,ALIGN_TOPLEFT,	&img_dialog_ok,&img_dialog_ok1,			0,SOSD_WIN8,0x100,  5,3,5,3},	//2	 
{ 0, proc_slider_undo,			X_START+381,Y_START+385,ALIGN_TOPLEFT,	&img_dialog_cancel,&img_dialog_cancel1,	0,SOSD_WIN8,0x100,  5,2,5,2},	//3

{ TYPE_SLIDE, proc_slider_ctrl,	X_START+242-220, Y_START+320-260,ALIGN_TOPMIDDLE,&img_slide_gray,NULL,			0,SOSD_WIN3,0x080,  2,0,2,0},	//start x,y. Gray 
{ TYPE_SLIDE, proc_slider_ctrl,	X_START+488-220, Y_START+370-260,ALIGN_TOPMIDDLE,&img_slide_red,NULL,			0,SOSD_WIN8,0x100,  2,0,2,0},	//end x,y.   Red
};
code struct menu_fitem_info_s menu_slider_text_items[] = {
	{ 0,0,	X_START+520-220,Y_START+324-260,	3,1, 0,0,	0xF0,	 16,0x07E0	},
};
struct menu_page_info_s menu_slider_page = { MENU_TYPE_SLIDE, 4,4, 4,1*2+2,	 menu_slider_page_items, menu_slider_text_items }; //note: 1 item
#endif



//#define SLIDE3_1	3
//#define SLIDE3_2	5
//#define SLIDE3_3	7
//#define SLIDE3_OK	8
//#define SLIDE3_CANCEL	9
//BKTODO:Add 3 slide bar for RGB contrast & RGB brightness
//code struct menu_sitem_info_s menu_slider3_page_items[] = {
//code struct menu_fitem_info_s menu_slider3_text_items[] = {
//struct menu_page_info_s menu_slider3_page = { MENU_TYPE_SLIDE3, 4,4, 4,3*2+2,	 menu_slider3_page_items, menu_slider3_text_items }; //note 3 items

//=====================
// POSITION
//=====================


//----------------------
// position popup MENU
//manu->input->setting with RGB/HDMI->display->position
//----------------------
//					
//
//
//
// 
//
//------------------------------------------
//description
//code struct menu_sitem_info_s menu_position_page_items[] = {

												 

//----------------------
// language MENU
//----------------------
//					
//------------------------------------------
//description
//code struct menu_sitem_info_s menu_lang_info[] = {
//	{ proc_lang_bg,			300,200,ALIGN_TOPLEFT,	image_lang_bg,	0xC0, SOSD_WIN2, 0,0,0,0},
//	{ proc_lang_eng,		300,200,ALIGN_TOPLEFT,	image_lang_eng,	0xC0, SOSD_WIN3, 0,0,0,0},
//	{ proc_lang_kor,		450,350,ALIGN_TOPLEFT,	image_lang_kor,	0xC0, SOSD_WIN4, 0,0,0,0},
//	{ proc_lang_jpn,		450,350,ALIGN_TOPLEFT,	image_lang_jpn,	0xC0, SOSD_WIN4, 0,0,0,0},
//	{ proc_lang_chn,		450,350,ALIGN_TOPLEFT,	img_ok,	0xC0, SOSD_WIN4, 0,0,0,0},
//	{ proc_lang_ok,			450,350,ALIGN_TOPLEFT,	img_ok,	0xC0, SOSD_WIN4, 0,0,0,0},
//};

#endif //..SUPPORT_SPIOSD
#endif //..SPIOSD_USE_800X480_IMG

