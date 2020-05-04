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
	// 初始化
	SetWindowTitle("CSXZCCC");
	SetWindowSize(16, 10);
	InitGraphics();

	PlaySound(TEXT(".\\1.wav"), NULL, SND_FILENAME | SND_ASYNC);
	getBOARD();//生成窗口，以下顺序不要换
	getMENU();//生成菜单
	getBOTTON();//生成按钮
	getcolorBOTTON();//生成颜色选择
	getsizeBOTTON();//生成画笔选择

	// 注册响应函数
	registerCharEvent(CharEventProcess);        // 字符
	registerKeyboardEvent(KeyboardEventProcess);// 键盘
	registerMouseEvent(MouseEventProcess);      // 鼠标
	registerTimerEvent(TimerEventProcess);      // 定时器

	managechoice();//接口

}

void KeyboardEventProcess(int key, int event) {
	uiGetKeyboard(key, event); // GUI获取键盘
	display();// 刷新显示
}
void MouseEventProcess(int x, int y, int button, int event) {
	uiGetMouse(x, y, button, event); //GUI获取鼠标
	display();// 刷新显示
}
void TimerEventProcess(int timerID) {

}
void CharEventProcess(char ch) {
	uiGetChar(ch); // GUI字符输入
	display();//刷新显示
}
void display() {
	//刷新显示，不要换顺序
	DisplayClear();
	getBOARD();
	getMENU();
	getBOTTON();
	getcolorBOTTON();
	getsizeBOTTON();
	managechoice();

}


