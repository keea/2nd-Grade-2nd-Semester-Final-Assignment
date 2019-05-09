/*
*
* @file		GAME_INFO
* @date     Thu May 10 13:25:34 2018
* @author
*/
#pragma once

//디버그 모드 설정
//#define _DEBUG_MODE

//카드 이미지 관련 설정.
const int CARD_IMAGE_NUM = 6; //카드 이미지 갯수
const int CARD_ROW = 3;
const int CARD_COL = 4;
const int CARD_WIDTH = 150;
const int CARD_HEIGHT = 150;
const int CARD_ANI_TIME = 1000;

//숫자 이미지 관련 설정.
const int NUM_IMAGE_NUM = 10; //숫자이미지 갯수 
const int NUM_WIDTH = 50;
const int NUM_HEIGTH = 50;

//이미지 매니져 관련 설정
enum IMAGE_MANAGER {
	CARD = 0,
	NUMBER = 1,
	IMAGE_MANAGER_NUM,
};

//제한시간 설정 (기본값 30초)
const int TIMER = 30;

//스테이지 관련 설정.
enum SCENE
{
	MAIN, //메인화면
	GAME, //게임화면
	RESULT, //결과화면
};

//버튼 이미지 관련 설정
const int BTN_WIDTH = 256;
const int BTN_HEIGHT = 92;

//결과 타입
enum RESULT_TYPE {
	WIN,
	LOSE,
};