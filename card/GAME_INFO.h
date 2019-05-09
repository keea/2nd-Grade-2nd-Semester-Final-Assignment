/*
*
* @file		GAME_INFO
* @date     Thu May 10 13:25:34 2018
* @author
*/
#pragma once

//����� ��� ����
//#define _DEBUG_MODE

//ī�� �̹��� ���� ����.
const int CARD_IMAGE_NUM = 6; //ī�� �̹��� ����
const int CARD_ROW = 3;
const int CARD_COL = 4;
const int CARD_WIDTH = 150;
const int CARD_HEIGHT = 150;
const int CARD_ANI_TIME = 1000;

//���� �̹��� ���� ����.
const int NUM_IMAGE_NUM = 10; //�����̹��� ���� 
const int NUM_WIDTH = 50;
const int NUM_HEIGTH = 50;

//�̹��� �Ŵ��� ���� ����
enum IMAGE_MANAGER {
	CARD = 0,
	NUMBER = 1,
	IMAGE_MANAGER_NUM,
};

//���ѽð� ���� (�⺻�� 30��)
const int TIMER = 30;

//�������� ���� ����.
enum SCENE
{
	MAIN, //����ȭ��
	GAME, //����ȭ��
	RESULT, //���ȭ��
};

//��ư �̹��� ���� ����
const int BTN_WIDTH = 256;
const int BTN_HEIGHT = 92;

//��� Ÿ��
enum RESULT_TYPE {
	WIN,
	LOSE,
};