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
	//缓冲置0
	memset(pImage, 0, 25600 * sizeof(char));//我把memset(pImage,0,X*Y*sizeof(char));中X,Y换成了160,160

	//读图象数据：bmp图象数据的保存顺序是从下至上，从左至右，
	//而我们需要的图象数据从上至下，从左至右，所以读数据时从
	//最后一行数据开始，而保存时从缓冲的最前位置开始
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
	/*  输出格式如下：
		TempFeature[0]特征点个数（2字节）（最大120个特征点）
		特征点信息（10 * 120字节）
		特征点信息格式：
		x坐标：2字节
		y坐标：2字节
		角度：2字节，0~359
		类型：2字节，0表示端点，1表示叉点
		质量：2字节，范围0~15，越大质量越高*/

	unsigned char TempImage[160 * 160];
	//例程中unsigned char TempFeature[1202];产生重定义，已删去。
	int ret;
	int x;
	int y;

	HINSTANCE hIn = NULL;
	hIn = LoadLibrary(_T("C:\\Users\\shenchentao\\Desktop\\opencv\\附加DLL\\SYDllTest.dll"));
	if (hIn == INVALID_HANDLE_VALUE)
	{
		cout << "Load DLL error" << endl;
		return -1;
	}
	//载入dll

	ret = SYFp_File_LoadBMP(src_img, TempImage, &x, &y);
	if (ret == -1)
	{
		cout << "Load BMP Error" << endl;
		return 0;
	}
	//else { cout << "Load" << src_img<< "succeed" << endl; }
	//载入图片

	FCreateTemplate pFun = NULL;
	pFun = (FCreateTemplate)GetProcAddress(hIn, "CreateTemplate");

	ret = pFun(TempImage, 160, 160, TempFeature);
	if (ret != 1)
	{
		cout << "Keypoints Extraction Error" << endl;
		return 0;
	}
	//特征点


	return 1;
}


int cvtKeyPoint(unsigned char* TempFeature, vector<KeyPoint>* keypoints)
{
	//特征点转化成向量，顺便帮自己记一下vector用法，vector<int>vec:构造一个int的vec向量
	/*KeyPoint() : pt(0,0), size(0), angle(-1), response(0), octave(0), class_id(-1) {}
	各项属性
	pt(x, y) :关键点的点坐标；point2f类型;
	size() :该关键点邻域直径大小；
	angle : 角度，表示关键点的方向，值为[零, 三百六十)，负值表示不使用。
	response : 响应强度
	octacv:从哪一层金字塔得到的此关键点。
	class_id:当要对图片进行分类时，用class_id对每个关键点进行区分，默认为-1。
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
		cout << "坐标：" << KP->pt << " ";
		cout << "领域大小：32" << " ";
		cout << "角度：" << KP->angle << " ";
		cout << "响应强度：" << KP->response << " ";
		cout << "类别：" << KP->class_id << " ";
		cout << endl;
	}
	cout << "end " << endl;
	cout << endl;*/
}