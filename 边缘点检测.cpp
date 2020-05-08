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
	float sumx = 128, sumwhitex = 0.5, powersumx = 128;//部分变量暂时不用，这几个作为阈值，灰度平均值，白色覆盖率，灰度平方平均数
	//***本函数阈值设定：灰度大于返回1****


	for (int i = 58; i < 102; i++)  //本图片中心区域灰度值
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

		for (int i = xia; i < shang; i++)  //行循环
		{
			uchar* data = dstImage.ptr<uchar>(i);  //获取第i行的首地址
			for (int j = zuo; j < you; j++)  //列循环
			{
				int intensity = data[j];
				sum = sum + intensity;
			}
		}
		//计算平均值
		sum = sum / mianji;


		//cout << "\n[" << x << "," << y << "],领域平均灰度值：" << sum << "     中心灰度值：" << zhongxinsum;
		//cout << "\n此领域灰度值与中心区域灰度值的比值" << sum / zhongxinsum << "\n";

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

