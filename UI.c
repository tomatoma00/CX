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


static double widthofwindow, heightofwindow;   // 窗口尺寸
static int finallychoice = 0;//当前的操作（当前选择的按钮）
static char colorofpen[12]="Black";
static char sizeofpen[3] = "01";

/*finallychoice 表：
11——新建文件 12——打开文件 13——保存文件 14——关闭文件 15——退出
21——选择     22——复制     23——粘贴     24——删除
31——矩形     32——圆角矩形 33——菱形     34——箭头
41——关于     42——使用帮助
*/
static double bottonH = 0.4;//按钮高度
static double bottonW = 0.85;//快捷按钮宽度
static double menuW = 1;//主菜单按钮宽度
static double frameW = 0.05;//页边距


void getBOARD(void) {
	char nowpencolor[20];
	strcpy(nowpencolor, GetPenColor());
	//边框绘制
	MovePen(0, 0);
	drawBox(frameW, frameW, widthofwindow - 2 * frameW, heightofwindow - 2 * frameW, 0, "", 'C', "Black");
	SetPenColor("Gray");
	drawBox(frameW, heightofwindow - bottonH - frameW, widthofwindow - 2 * frameW, bottonH, 1, "主菜单栏", 'R', "Black");
	SetPenColor("Light Gray");
	drawBox(frameW, frameW, bottonW, heightofwindow - 2 * frameW - bottonH, 1, " ", 'R', "Light Gray");
	SetPenColor(nowpencolor);
}

void getMENU(void){
	//主菜单栏生成函数
	int listchoice;//用于判断选择了下拉菜单的哪一个

	static char * filelist[] = { "文件","新建  | Ctrl-N","打开  | Ctrl-O", "保存  | Ctrl-S","关闭","退出" };
	static char * editlist[] = { "编辑","选择  | Ctrl-M","复制  | Ctrl-C", "粘贴  | Ctrl-V","删除  | Ctrl-D" };
	static char * drawlist[] = { "绘图","矩形","圆角矩形", "菱形","箭头" };
	static char * helplist[] = { "帮助","关于","使用帮助  | Ctrl-H" };

	widthofwindow = GetWindowWidth();
	heightofwindow = GetWindowHeight();

    //主菜单绘制
	setMenuColors("Gray", "Black", "Light Gray", "Brown", 1);
	listchoice = menuList(GenUIID(10), frameW + bottonW, heightofwindow - bottonH - frameW, menuW, 1.5*menuW, bottonH, filelist, sizeof(filelist) / sizeof(filelist[0]));
	if (listchoice > 0) finallychoice = 10 + listchoice;
	if (listchoice == 5) exit(-1); // 对应退出
	listchoice = menuList(GenUIID(11), frameW + bottonW + 1 * menuW, heightofwindow - bottonH - frameW, menuW, 1.5*menuW, bottonH, editlist, sizeof(editlist) / sizeof(editlist[0]));
	if (listchoice > 0) finallychoice = 20 + listchoice;
	listchoice = menuList(GenUIID(12), frameW + bottonW + 2 * menuW, heightofwindow - bottonH - frameW, menuW, 1.5*menuW, bottonH, drawlist, sizeof(drawlist) / sizeof(drawlist[0]));
	if (listchoice > 0) finallychoice = 30 + listchoice;
	listchoice = menuList(GenUIID(12), frameW + bottonW + 3 * menuW, heightofwindow - bottonH - frameW, menuW, 1.5*menuW, bottonH, helplist, sizeof(helplist) / sizeof(helplist[0]));
	if (listchoice > 0) finallychoice = 40 + listchoice;

}

void getBOTTON(void) {
	//按钮绘制（快捷按钮）
	setButtonColors("Light Gray", "Black", "Gray", "Black", 1);
	if (button(GenUIID(0), frameW, heightofwindow - 2 * bottonH - frameW, bottonW, bottonH, "新建")) finallychoice = 11;
	if (button(GenUIID(1), frameW, heightofwindow - 3 * bottonH - frameW, bottonW, bottonH, "保存")) finallychoice = 13;
	if (button(GenUIID(2), frameW, heightofwindow - 4 * bottonH - frameW, bottonW, bottonH, "删除")) finallychoice = 24;
	if (button(GenUIID(3), frameW, heightofwindow - 5 * bottonH - frameW, bottonW, bottonH, "矩形")) finallychoice = 31;
	if (button(GenUIID(4), frameW, heightofwindow - 6 * bottonH - frameW, bottonW, bottonH, "圆矩")) finallychoice = 32;
	if (button(GenUIID(5), frameW, heightofwindow - 7 * bottonH - frameW, bottonW, bottonH, "菱形")) finallychoice = 33; 
	if (button(GenUIID(6), frameW, heightofwindow - 8 * bottonH - frameW, bottonW, bottonH, "箭头")) finallychoice = 34; 

}

void getcolorBOTTON(void){
	//选择颜色的按钮
	double halfwindowH = heightofwindow / 2;
	static char pencolorbotton[14][20] = { "Black" ,"Dark Gray","Gray","Light Gray","White",
	"Brown" ,"Red" ,"Orange" ,"Yellow" ,"Green" ,"Blue" ,"Violet" ,"Magenta" ,"Cyan" };
	drawLabel(frameW, heightofwindow / 2, "画笔颜色");

	//********注：由于各种原因，循环它失效了，只能直接一个个输入*******
	setButtonColors(pencolorbotton[0], "Black", pencolorbotton[0], "Black", 1);
	if (button(GenUIID(0), frameW, halfwindowH - 1 * bottonH, bottonW*0.45, bottonH*0.9, " ")) {
		strcpy(colorofpen, pencolorbotton[0]); /*SetPenColor(pencolorbotton[i]);*/ }
	setButtonColors(pencolorbotton[1], "Black", pencolorbotton[1], "Black", 1);
	if (button(GenUIID(0), frameW + bottonW / 2, halfwindowH - 1 * bottonH, bottonW*0.45, bottonH*0.9, " ")) {
		strcpy(colorofpen, pencolorbotton[1]); }
	setButtonColors(pencolorbotton[2], "Black", pencolorbotton[2], "Black", 1);
	if (button(GenUIID(0), frameW, halfwindowH - 2 * bottonH, bottonW*0.45, bottonH*0.9, " ")) {
		strcpy(colorofpen, pencolorbotton[2]); }
	setButtonColors(pencolorbotton[3], "Black", pencolorbotton[3], "Black", 1);
	if (button(GenUIID(0), frameW + bottonW / 2, halfwindowH - 2 * bottonH, bottonW*0.45, bottonH*0.9, " ")) {
		strcpy(colorofpen, pencolorbotton[3]);}
	setButtonColors(pencolorbotton[4], "Black", pencolorbotton[4], "Black", 1);
	if (button(GenUIID(0), frameW, halfwindowH - 3 * bottonH, bottonW*0.45, bottonH*0.9, " ")) {
		strcpy(colorofpen, pencolorbotton[4]);}
	setButtonColors(pencolorbotton[5], "Black", pencolorbotton[5], "Black", 1);
	if (button(GenUIID(0), frameW + bottonW / 2, halfwindowH - 3 * bottonH, bottonW*0.45, bottonH*0.9, " ")) {
		strcpy(colorofpen, pencolorbotton[5]);}
	setButtonColors(pencolorbotton[6], "Black", pencolorbotton[6], "Black", 1);
	if (button(GenUIID(0), frameW, halfwindowH - 4 * bottonH, bottonW*0.45, bottonH*0.9, " ")) {
		strcpy(colorofpen, pencolorbotton[6]);}
	setButtonColors(pencolorbotton[7], "Black", pencolorbotton[7], "Black", 1);
	if (button(GenUIID(0), frameW + bottonW / 2, halfwindowH - 4 * bottonH, bottonW*0.45, bottonH*0.9, " ")) {
		strcpy(colorofpen, pencolorbotton[7]);}
	setButtonColors(pencolorbotton[8], "Black", pencolorbotton[8], "Black", 1);
	if (button(GenUIID(0), frameW, halfwindowH - 5 * bottonH, bottonW*0.45, bottonH*0.9, " ")) {
		strcpy(colorofpen, pencolorbotton[8]);}
	setButtonColors(pencolorbotton[9], "Black", pencolorbotton[9], "Black", 1);
	if (button(GenUIID(0), frameW + bottonW / 2, halfwindowH - 5 * bottonH, bottonW*0.45, bottonH*0.9, " ")) {
		strcpy(colorofpen, pencolorbotton[9]);}
	setButtonColors(pencolorbotton[10], "Black", pencolorbotton[10], "Black", 1);
	if (button(GenUIID(0), frameW, halfwindowH - 6 * bottonH, bottonW*0.45, bottonH*0.9, " ")) {
		strcpy(colorofpen, pencolorbotton[10]);}
	setButtonColors(pencolorbotton[11], "Black", pencolorbotton[11], "Black", 1);
	if (button(GenUIID(0), frameW + bottonW / 2, halfwindowH - 6 * bottonH, bottonW*0.45, bottonH*0.9, " ")) {
		strcpy(colorofpen, pencolorbotton[11]);}
	setButtonColors(pencolorbotton[12], "Black", pencolorbotton[12], "Black", 1);
	if (button(GenUIID(0), frameW, halfwindowH - 7 * bottonH, bottonW*0.45, bottonH*0.9, " ")) {
		strcpy(colorofpen, pencolorbotton[12]);}
	setButtonColors(pencolorbotton[13], "Black", pencolorbotton[13], "Black", 1);
	if (button(GenUIID(0), frameW + bottonW / 2, halfwindowH - 7 * bottonH, bottonW*0.45, bottonH*0.9, " ")) {
		strcpy(colorofpen, pencolorbotton[13]);
	}
}

void getsizeBOTTON(void) {
	double halfwindowH = heightofwindow / 2;
	static char input[3]="1";
	drawLabel(frameW, heightofwindow / 2+2*bottonH, "画笔粗细");
	setTextBoxColors("White", "Black", "White", "Black", 1);
	SetPenColor("Black");
	textbox(GenUIID(0), frameW, heightofwindow / 2 + 0.5 * bottonH, bottonW*0.95, bottonH, input, sizeof(input));
	strcpy(sizeofpen, input);
}

void managechoice(void) {
	//对选择进行操作，并实现显示功能
	char output[90];
	static int thepensize;
	thepensize = (sizeofpen[0] - 48) * 10 + sizeofpen[1] - 48;
	strcpy(output, "正在进行的操作是");
	switch (finallychoice){
	case 11:
		strcpy(output, "刚刚进行的操作是新建。");
		break;
	case 12:
		strcpy(output, "刚刚进行的操作是打开。");
		break;
	case 13:
		strcpy(output, "刚刚进行的操作是保存。");
		break;
	case 14:
		strcpy(output, "刚刚进行的操作是关闭。");
		break;
	case 15:
		strcat(output, "退出。");
		break;
	case 21:
		strcat(output, "选择。");
		break;
	case 22:
		PlaySound(TEXT(".\\1.wav"), NULL, SND_FILENAME | SND_ASYNC);
		strcat(output, "复制。");
		break;
	case 23:
		PlaySound(TEXT(".\\1.wav"), NULL, SND_FILENAME | SND_ASYNC);
		strcat(output, "粘贴。");
		break;
	case 24:
		PlaySound(TEXT(".\\1.wav"), NULL, SND_FILENAME | SND_ASYNC);
		strcpy(output, "刚刚进行的操作是删除。");
		break;
	case 31:
		PlaySound(TEXT(".\\1.wav"), NULL, SND_FILENAME | SND_ASYNC);
		strcat(output, "绘制矩形。");
		break;
	case 32:
		PlaySound(TEXT(".\\1.wav"), NULL, SND_FILENAME | SND_ASYNC);
		strcat(output, "绘制圆角矩形。");
		break;
	case 33:
		PlaySound(TEXT(".\\1.wav"), NULL, SND_FILENAME | SND_ASYNC);
		strcat(output, "绘制菱形。");
		break;
	case 34:
		PlaySound(TEXT(".\\1.wav"), NULL, SND_FILENAME | SND_ASYNC);
		strcat(output, "绘制箭头。");
		break;
	case 41:
		strcat(output, "查看关于。");
		break;
	case 42:
		strcat(output, "查看使用帮助。");
		break;
	default:
		strcat(output, "暂无。");
		break;
	}
	MovePen(0, 0);
	strcat(output, "当前画笔颜色是");
	//PlaySound(TEXT("C:\\Users\\shenchentao\\Desktop\\与学习有关的临时文档\\程序设计专题\\homework\\ourproject\\aoligei.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	strcat(output, colorofpen);
	strcat(output, "。粗细是");
	strcat(output, sizeofpen);
	drawLabel(frameW + bottonW * 1.5, frameW * 2, output);
}



