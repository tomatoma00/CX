#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#pragma comment(lib,"winmm.lib")

#include "imgui.h"
#include "UI.h"

void KeyboardEventProcess(int key, int event);
void MouseEventProcess(int x, int y, int button, int event);
void TimerEventProcess(int timerID);
void CharEventProcess(char ch);

void Main()
{
	// ��ʼ��
	SetWindowTitle("CSXZCCC");
	SetWindowSize(16, 10);
	InitGraphics();

	PlaySound(TEXT(".\\1.wav"), NULL, SND_FILENAME | SND_ASYNC);
	getBOARD();//���ɴ��ڣ�����˳��Ҫ��
	getMENU();//���ɲ˵�
	getBOTTON();//���ɰ�ť
	getcolorBOTTON();//������ɫѡ��
	getsizeBOTTON();//���ɻ���ѡ��

	// ע����Ӧ����
	registerCharEvent(CharEventProcess);        // �ַ�
	registerKeyboardEvent(KeyboardEventProcess);// ����
	registerMouseEvent(MouseEventProcess);      // ���
	registerTimerEvent(TimerEventProcess);      // ��ʱ��

	managechoice();//�ӿ�

}

void KeyboardEventProcess(int key, int event) {
	uiGetKeyboard(key, event); // GUI��ȡ����
	display();// ˢ����ʾ
}
void MouseEventProcess(int x, int y, int button, int event) {
	uiGetMouse(x, y, button, event); //GUI��ȡ���
	display();// ˢ����ʾ
}
void TimerEventProcess(int timerID) {

}
void CharEventProcess(char ch) {
	uiGetChar(ch); // GUI�ַ�����
	display();//ˢ����ʾ
}
void display() {
	//ˢ����ʾ����Ҫ��˳��
	DisplayClear();
	getBOARD();
	getMENU();
	getBOTTON();
	getcolorBOTTON();
	getsizeBOTTON();
	managechoice();

}


