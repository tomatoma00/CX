#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <direct.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <vector>
#include <SDKDDKVer.h>
#define _CRT_SECURE_NO_WARNINGS

using namespace std;
using namespace cv;
typedef int(*FCreateTemplate)(unsigned char*, int, int, unsigned char*);

int SYFp_File_LoadBMP(char* file, unsigned char* pImage, int* pX, int* pY);
int Ffeature(char * src_img, unsigned char* TempFeature);
int cvtKeyPoint(unsigned char* TempFeature, vector<KeyPoint>* keypoints);

int SYFp_File_LoadBMP(char* file, unsigned char* pImage, int* pX, int* pY)
{
	FILE *fp;
	int i, X, Y;
	unsigned char head[1078];

	X = *pX;
	Y = *pY;
	//������0
	memset(pImage, 0, 25600 * sizeof(char));//�Ұ�memset(pImage,0,X*Y*sizeof(char));��X,Y������160,160

	//��ͼ�����ݣ�bmpͼ�����ݵı���˳���Ǵ������ϣ��������ң�
	//��������Ҫ��ͼ�����ݴ������£��������ң����Զ�����ʱ��
	//���һ�����ݿ�ʼ��������ʱ�ӻ������ǰλ�ÿ�ʼ
	fp = fopen((const char*)file, "rb");
	if (fp == NULL)
		return -1;
	fread(head, 26, 1, fp);

	X = head[18] + (head[19] << 8) + (head[20] << 8) + (head[21] << 8);
	Y = head[22] + (head[23] << 8) + (head[24] << 8) + (head[25] << 8);
	*pX = X;
	*pY = Y;
	for (i = 0; i < Y; i++)
	{
		fseek(fp, (1078 + (Y - 1)*X) * sizeof(char) - i * X, SEEK_SET);
		fread(pImage + i * X, X * sizeof(char), 1, fp);
	}
	fclose(fp);

	return 0;
}


int Ffeature(char * src_img, unsigned char* TempFeature)
{
	/*  �����ʽ���£�
		TempFeature[0]�����������2�ֽڣ������120�������㣩
		��������Ϣ��10 * 120�ֽڣ�
		��������Ϣ��ʽ��
		x���꣺2�ֽ�
		y���꣺2�ֽ�
		�Ƕȣ�2�ֽڣ�0~359
		���ͣ�2�ֽڣ�0��ʾ�˵㣬1��ʾ���
		������2�ֽڣ���Χ0~15��Խ������Խ��*/

	unsigned char TempImage[160 * 160];
	//������unsigned char TempFeature[1202];�����ض��壬��ɾȥ��
	int ret;
	int x;
	int y;

	HINSTANCE hIn = NULL;
	hIn = LoadLibrary(_T("C:\\Users\\shenchentao\\Desktop\\opencv\\����DLL\\SYDllTest.dll"));
	if (hIn == INVALID_HANDLE_VALUE)
	{
		cout << "Load DLL error" << endl;
		return -1;
	}
	//����dll

	ret = SYFp_File_LoadBMP(src_img, TempImage, &x, &y);
	if (ret == -1)
	{
		cout << "Load BMP Error" << endl;
		return 0;
	}
	//else { cout << "Load" << src_img<< "succeed" << endl; }
	//����ͼƬ

	FCreateTemplate pFun = NULL;
	pFun = (FCreateTemplate)GetProcAddress(hIn, "CreateTemplate");

	ret = pFun(TempImage, 160, 160, TempFeature);
	if (ret != 1)
	{
		cout << "Keypoints Extraction Error" << endl;
		return 0;
	}
	//������


	return 1;
}


int cvtKeyPoint(unsigned char* TempFeature, vector<KeyPoint>* keypoints)
{
	//������ת����������˳����Լ���һ��vector�÷���vector<int>vec:����һ��int��vec����
	/*KeyPoint() : pt(0,0), size(0), angle(-1), response(0), octave(0), class_id(-1) {}
	��������
	pt(x, y) :�ؼ���ĵ����ꣻpoint2f����;
	size() :�ùؼ�������ֱ����С��
	angle : �Ƕȣ���ʾ�ؼ���ķ���ֵΪ[��, ������ʮ)����ֵ��ʾ��ʹ�á�
	response : ��Ӧǿ��
	octacv:����һ��������õ��Ĵ˹ؼ��㡣
	class_id:��Ҫ��ͼƬ���з���ʱ����class_id��ÿ���ؼ���������֣�Ĭ��Ϊ-1��
	*/
	int len;
	int i, j;
	Point2f point;
	float angle, m;
	int type;

	len = (int)TempFeature[0];
	if (len == 0)
		return 0;
	for (i = 2, j = 0; i < 2 + 10 * len; i += 10)
	{
		point = Point2f((float)TempFeature[i], (float)TempFeature[i + 2]);
		angle = (float)TempFeature[i + 4];
		type = (float)TempFeature[i + 6];
		m = (float)TempFeature[i + 8];
		(*keypoints)[j] = KeyPoint::KeyPoint(point, 32, angle, m, 0, type);
		j++;
	}
	/*
	vector<KeyPoint>::iterator KP;
	for (KP = (*keypoints).begin(); KP != (*keypoints).end(); KP++)
	{
		cout << "���꣺" << KP->pt << " ";
		cout << "�����С��32" << " ";
		cout << "�Ƕȣ�" << KP->angle << " ";
		cout << "��Ӧǿ�ȣ�" << KP->response << " ";
		cout << "���" << KP->class_id << " ";
		cout << endl;
	}
	cout << "end " << endl;
	cout << endl;*/
}