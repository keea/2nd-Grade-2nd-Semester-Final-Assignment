#pragma once

enum JUDGMENT
{
	NONE,
	MISS,
	GREAT,
	PERFECT, 
	
};

enum SCENE
{
	GAME_MAIN,
	GAME,
	GAME_RESULT,
	SCENE_NONE, //바꾸는 값이 없다.
};

enum SOUND{
	S_BGM,
	S_GAME_BGM,
	S_CLICK,
	S_NUM,
};

#define BACKGROUND_COLOR Color(231, 180, 51) 
#define H_LIGTH_YELLOW Color(238, 203, 94) 
#define H_DARK_YELLOW Color(159, 125, 36) 
#define H_RESULT_YELLOW Color(255, 255, 60)
#define H_PINK Color(227, 79, 195) 
#define H_TIMER_YELLOW Color(255, 231, 106)
#define H_GRAY Color(216, 213, 109)

#define FONT_CUTE L"HY얕은샘물M"
#define FONT_DEFAULT L"HY목각파임B"