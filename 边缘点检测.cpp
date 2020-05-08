#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <vector>
#include <SDKDDKVer.h>
#define _CRT_SECURE_NO_WARNINGS

using namespace cv;
using namespace std;

int bianyuandian(Mat src, vector<KeyPoint>* keypoints)
{
	Mat dstImage=src.clone();
	vector<KeyPoint>::iterator KP;
	float sum = 0,sumwhite=0,powersum=0,zhongxinsum=0;
	float sumx = 128, sumwhitex = 0.5, powersumx = 128;//���ֱ�����ʱ���ã��⼸����Ϊ��ֵ���Ҷ�ƽ��ֵ����ɫ�����ʣ��Ҷ�ƽ��ƽ����
	//***��������ֵ�趨���Ҷȴ��ڷ���1****


	for (int i = 58; i < 102; i++)  //��ͼƬ��������Ҷ�ֵ
	{
		uchar* data = dstImage.ptr<uchar>(i);
		for (int j = 58; j < 102; j++)
		{
			int intensity = data[j];
			zhongxinsum = zhongxinsum + intensity;
		}
		//cout <<"\n"<< endl;
	}
	zhongxinsum = zhongxinsum / 1936;



	for (KP = (*keypoints).begin(); KP != (*keypoints).end(); )
	{
		int x = KP->pt.x;
		int y = KP->pt.y;
		int zuo = max(0, x - 16);
		int you = min(160, x + 16);
		int shang = min(160, y + 16);
		int xia = max(0, y - 16);
		int mianji = (you - zuo)*(shang - xia);

		sum = 0;

		for (int i = xia; i < shang; i++)  //��ѭ��
		{
			uchar* data = dstImage.ptr<uchar>(i);  //��ȡ��i�е��׵�ַ
			for (int j = zuo; j < you; j++)  //��ѭ��
			{
				int intensity = data[j];
				sum = sum + intensity;
			}
		}
		//����ƽ��ֵ
		sum = sum / mianji;


		//cout << "\n[" << x << "," << y << "],����ƽ���Ҷ�ֵ��" << sum << "     ���ĻҶ�ֵ��" << zhongxinsum;
		//cout << "\n������Ҷ�ֵ����������Ҷ�ֵ�ı�ֵ" << sum / zhongxinsum << "\n";

		if (sum / zhongxinsum > 1.2||sum>140) {
			KP = (*keypoints).erase(KP);
		}
		else
		{
             KP++;
		}
	}

	//cout << "\n";

	return 0;
}

