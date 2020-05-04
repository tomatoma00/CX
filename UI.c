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


static double widthofwindow, heightofwindow;   // ���ڳߴ�
static int finallychoice = 0;//��ǰ�Ĳ�������ǰѡ��İ�ť��
static char colorofpen[12]="Black";
static char sizeofpen[3] = "01";

/*finallychoice ��
11�����½��ļ� 12�������ļ� 13���������ļ� 14�����ر��ļ� 15�����˳�
21����ѡ��     22��������     23����ճ��     24����ɾ��
31��������     32����Բ�Ǿ��� 33��������     34������ͷ
41��������     42����ʹ�ð���
*/
static double bottonH = 0.4;//��ť�߶�
static double bottonW = 0.85;//��ݰ�ť���
static double menuW = 1;//���˵���ť���
static double frameW = 0.05;//ҳ�߾�


void getBOARD(void) {
	char nowpencolor[20];
	strcpy(nowpencolor, GetPenColor());
	//�߿����
	MovePen(0, 0);
	drawBox(frameW, frameW, widthofwindow - 2 * frameW, heightofwindow - 2 * frameW, 0, "", 'C', "Black");
	SetPenColor("Gray");
	drawBox(frameW, heightofwindow - bottonH - frameW, widthofwindow - 2 * frameW, bottonH, 1, "���˵���", 'R', "Black");
	SetPenColor("Light Gray");
	drawBox(frameW, frameW, bottonW, heightofwindow - 2 * frameW - bottonH, 1, " ", 'R', "Light Gray");
	SetPenColor(nowpencolor);
}

void getMENU(void){
	//���˵������ɺ���
	int listchoice;//�����ж�ѡ���������˵�����һ��

	static char * filelist[] = { "�ļ�","�½�  | Ctrl-N","��  | Ctrl-O", "����  | Ctrl-S","�ر�","�˳�" };
	static char * editlist[] = { "�༭","ѡ��  | Ctrl-M","����  | Ctrl-C", "ճ��  | Ctrl-V","ɾ��  | Ctrl-D" };
	static char * drawlist[] = { "��ͼ","����","Բ�Ǿ���", "����","��ͷ" };
	static char * helplist[] = { "����","����","ʹ�ð���  | Ctrl-H" };

	widthofwindow = GetWindowWidth();
	heightofwindow = GetWindowHeight();

    //���˵�����
	setMenuColors("Gray", "Black", "Light Gray", "Brown", 1);
	listchoice = menuList(GenUIID(10), frameW + bottonW, heightofwindow - bottonH - frameW, menuW, 1.5*menuW, bottonH, filelist, sizeof(filelist) / sizeof(filelist[0]));
	if (listchoice > 0) finallychoice = 10 + listchoice;
	if (listchoice == 5) exit(-1); // ��Ӧ�˳�
	listchoice = menuList(GenUIID(11), frameW + bottonW + 1 * menuW, heightofwindow - bottonH - frameW, menuW, 1.5*menuW, bottonH, editlist, sizeof(editlist) / sizeof(editlist[0]));
	if (listchoice > 0) finallychoice = 20 + listchoice;
	listchoice = menuList(GenUIID(12), frameW + bottonW + 2 * menuW, heightofwindow - bottonH - frameW, menuW, 1.5*menuW, bottonH, drawlist, sizeof(drawlist) / sizeof(drawlist[0]));
	if (listchoice > 0) finallychoice = 30 + listchoice;
	listchoice = menuList(GenUIID(12), frameW + bottonW + 3 * menuW, heightofwindow - bottonH - frameW, menuW, 1.5*menuW, bottonH, helplist, sizeof(helplist) / sizeof(helplist[0]));
	if (listchoice > 0) finallychoice = 40 + listchoice;

}

void getBOTTON(void) {
	//��ť���ƣ���ݰ�ť��
	setButtonColors("Light Gray", "Black", "Gray", "Black", 1);
	if (button(GenUIID(0), frameW, heightofwindow - 2 * bottonH - frameW, bottonW, bottonH, "�½�")) finallychoice = 11;
	if (button(GenUIID(1), frameW, heightofwindow - 3 * bottonH - frameW, bottonW, bottonH, "����")) finallychoice = 13;
	if (button(GenUIID(2), frameW, heightofwindow - 4 * bottonH - frameW, bottonW, bottonH, "ɾ��")) finallychoice = 24;
	if (button(GenUIID(3), frameW, heightofwindow - 5 * bottonH - frameW, bottonW, bottonH, "����")) finallychoice = 31;
	if (button(GenUIID(4), frameW, heightofwindow - 6 * bottonH - frameW, bottonW, bottonH, "Բ��")) finallychoice = 32;
	if (button(GenUIID(5), frameW, heightofwindow - 7 * bottonH - frameW, bottonW, bottonH, "����")) finallychoice = 33; 
	if (button(GenUIID(6), frameW, heightofwindow - 8 * bottonH - frameW, bottonW, bottonH, "��ͷ")) finallychoice = 34; 

}

void getcolorBOTTON(void){
	//ѡ����ɫ�İ�ť
	double halfwindowH = heightofwindow / 2;
	static char pencolorbotton[14][20] = { "Black" ,"Dark Gray","Gray","Light Gray","White",
	"Brown" ,"Red" ,"Orange" ,"Yellow" ,"Green" ,"Blue" ,"Violet" ,"Magenta" ,"Cyan" };
	drawLabel(frameW, heightofwindow / 2, "������ɫ");

	//********ע�����ڸ���ԭ��ѭ����ʧЧ�ˣ�ֻ��ֱ��һ��������*******
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
	drawLabel(frameW, heightofwindow / 2+2*bottonH, "���ʴ�ϸ");
	setTextBoxColors("White", "Black", "White", "Black", 1);
	SetPenColor("Black");
	textbox(GenUIID(0), frameW, heightofwindow / 2 + 0.5 * bottonH, bottonW*0.95, bottonH, input, sizeof(input));
	strcpy(sizeofpen, input);
}

void managechoice(void) {
	//��ѡ����в�������ʵ����ʾ����
	char output[90];
	static int thepensize;
	thepensize = (sizeofpen[0] - 48) * 10 + sizeofpen[1] - 48;
	strcpy(output, "���ڽ��еĲ�����");
	switch (finallychoice){
	case 11:
		strcpy(output, "�ոս��еĲ������½���");
		break;
	case 12:
		strcpy(output, "�ոս��еĲ����Ǵ򿪡�");
		break;
	case 13:
		strcpy(output, "�ոս��еĲ����Ǳ��档");
		break;
	case 14:
		strcpy(output, "�ոս��еĲ����ǹرա�");
		break;
	case 15:
		strcat(output, "�˳���");
		break;
	case 21:
		strcat(output, "ѡ��");
		break;
	case 22:
		PlaySound(TEXT(".\\1.wav"), NULL, SND_FILENAME | SND_ASYNC);
		strcat(output, "���ơ�");
		break;
	case 23:
		PlaySound(TEXT(".\\1.wav"), NULL, SND_FILENAME | SND_ASYNC);
		strcat(output, "ճ����");
		break;
	case 24:
		PlaySound(TEXT(".\\1.wav"), NULL, SND_FILENAME | SND_ASYNC);
		strcpy(output, "�ոս��еĲ�����ɾ����");
		break;
	case 31:
		PlaySound(TEXT(".\\1.wav"), NULL, SND_FILENAME | SND_ASYNC);
		strcat(output, "���ƾ��Ρ�");
		break;
	case 32:
		PlaySound(TEXT(".\\1.wav"), NULL, SND_FILENAME | SND_ASYNC);
		strcat(output, "����Բ�Ǿ��Ρ�");
		break;
	case 33:
		PlaySound(TEXT(".\\1.wav"), NULL, SND_FILENAME | SND_ASYNC);
		strcat(output, "�������Ρ�");
		break;
	case 34:
		PlaySound(TEXT(".\\1.wav"), NULL, SND_FILENAME | SND_ASYNC);
		strcat(output, "���Ƽ�ͷ��");
		break;
	case 41:
		strcat(output, "�鿴���ڡ�");
		break;
	case 42:
		strcat(output, "�鿴ʹ�ð�����");
		break;
	default:
		strcat(output, "���ޡ�");
		break;
	}
	MovePen(0, 0);
	strcat(output, "��ǰ������ɫ��");
	//PlaySound(TEXT("C:\\Users\\shenchentao\\Desktop\\��ѧϰ�йص���ʱ�ĵ�\\�������ר��\\homework\\ourproject\\aoligei.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	strcat(output, colorofpen);
	strcat(output, "����ϸ��");
	strcat(output, sizeofpen);
	drawLabel(frameW + bottonW * 1.5, frameW * 2, output);
}



