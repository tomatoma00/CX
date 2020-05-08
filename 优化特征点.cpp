#pragma once
#include "highgui/highgui.hpp"
#include "highgui/highgui_c.h"
#include <opencv.hpp>
#include "opencv_modules.hpp"
#include <opencv2/features2d/features2d.hpp>
#include "opencv2/nonfree/nonfree.hpp"  
#include "opencv2/nonfree/features2d.hpp"  
#include "opencv2/legacy/legacy.hpp" 
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp" 
#include <tchar.h>
#include <iostream>
#include <string.h>
#include <direct.h>
#include <math.h>
#include "opencv2/core/core.hpp"
#include <vector>
#include "opencv2/contrib/contrib.hpp"
#include "cv.hpp"
#include "opencv/highgui.h"
#include <SDKDDKVer.h>
#define _CRT_SECURE_NO_WARNINGS

using namespace std;
using namespace cv;

float get_Angle(const Mat& image, const int half_k, Point2f pt);

//youhua和lingyu函数已经不用了
void youhua(vector<KeyPoint>* keypointsorb, vector<KeyPoint>* keypoints,double size) {
	vector<KeyPoint>::iterator tp1,tp2;
	int flag = 0;
	int absx, absy;
	/*
	cout << "before" << endl;
	for (tp1 = (*keypointsorb).begin(); tp1 != (*keypointsorb).end(); tp1++)
	{
		cout << "PT=" << tp1->pt << " ";

	}
	cout << "\n";*/

	for (tp1 = (*keypointsorb).begin(); tp1 != (*keypointsorb).end(); ) {
		flag = 0;
		for (tp2 = (*keypoints).begin(); tp2 != (*keypoints).end(); tp2++) {
			absx = abs(tp1->pt.x - tp2->pt.x);
			absy = abs(tp1->pt.y - tp2->pt.y);
			if (absx*absx+absy*absy < size) {
				flag = 1;
				break;
			}
		}

		tp1->pt.x = tp1->pt.x ;
		tp1->pt.y = tp1->pt.y ;

		if (flag == 0) {
				tp1 = (*keypointsorb).erase(tp1);
			}
		else {
			tp1++;
		}
	}
	/*
	cout << "after" << endl;
	for (tp1 = (*keypointsorb).begin(); tp1 != (*keypointsorb).end(); tp1++)
	{
		cout << "PT=" << tp1->pt << " ";
	
	}
	cout << "\n";*/
}
void lingyu(vector<KeyPoint>* keypoints,double beishu) {
	vector<KeyPoint>::iterator tp1;
	int flag = 0;
	int absx, absy;
	/*
	cout << "before" << endl;
	for (tp1 = (*keypointsorb).begin(); tp1 != (*keypointsorb).end(); tp1++)
	{cout << "PT=" << tp1->pt << " ";}
	cout << "\n";*/

	for (tp1 = (*keypoints).begin(); tp1 != (*keypoints).end(); ) {
		tp1->pt.x = tp1->pt.x / beishu;
		tp1->pt.y = tp1->pt.y / beishu;		
			tp1++;
	}
	/*
	cout << "after" << endl;
	for (tp1 = (*keypointsorb).begin(); tp1 != (*keypointsorb).end(); tp1++)
	{cout << "PT=" << tp1->pt << " ";}
	cout << "\n";*/
}

void youhua2(vector<KeyPoint>* keypointsnew, vector<KeyPoint>* keypoints, double size, Mat src) {
	
	int intsize = (int)(size);
	int num=0;

	float i, j;
	Point2f point;
	float angle, m;
	int type;
	
	vector<KeyPoint>::iterator tp1;
	for (tp1 = (*keypoints).begin(); tp1 != (*keypoints).end(); ) {
		float left = tp1->pt.x - (int)(intsize / 2);
		float top = tp1->pt.y - (int)(intsize / 2);
		for (i = left; i < left + intsize; i=i+1) {
			for (j = top; j < top + intsize; j=j+1) {

				point = Point2f(i, j);
				angle = get_Angle(src, 15, (tp1)->pt);
				type = tp1->class_id;
				m = tp1->response;
				(*keypointsnew)[num] = KeyPoint::KeyPoint(point, 32, angle, m, 0, type);
				//printf("%d is OK,pt is [%f,%f]\n", num,i,j);
				num++;
				/*
				for (double newangle = tp1->angle - 2; newangle <= tp1->angle + 2; newangle++) {
					point = Point2f(i, j);
					angle = newangle;
					type = tp1->class_id;
					m = tp1->response;
					(*keypointsnew)[num] = KeyPoint::KeyPoint(point, 32, angle, m, 0, type);
					//printf("%d is OK,pt is [%f,%f]\n", num,i,j);
					num++;
				}*/


			}
		}
		tp1++;
	}
	/*
	cout << "after" << endl;
	for (tp1 = (*keypointsnew).begin(); tp1 != (*keypointsnew).end(); tp1++)
	{cout << "PT=" << tp1->pt << " ";}
	cout << "\n";*/
 }

double getorbangle(Mat src, KeyPoint thekeypoint) {
	float zhixinx, zhixiny,i,j;
	float cx, cy;
	double sumx = 0, sumy = 0, sum = 0;
	cx = thekeypoint.pt.x;
	cy = thekeypoint.pt.y;
	for (i = cx - 15; i <= cx + 15; i=i+1) {
		uchar* data = src.ptr<uchar>(i);  //获取第i行的首地址
		for (int j = cy-(int)(sqrt(225-(cx-i)*(cx-i) )); j <= cy + (int)(sqrt(225 - (cx - i)*(cx - i))); j=j+1)  //列循环
		{
			int intensity = data[j];
			sum = sum + intensity;
			sumx = sumx + intensity * i;
			sumy = sumy + intensity * j;
		}
	}

	zhixinx = sumx / sum;
	zhixiny = sumy / sum;

	if (zhixinx == cx && zhixiny == cy) return 0;

	double r;
	float theangle;
	r = sqrt((zhixinx - cx)*(zhixinx - cx) + (zhixiny - cy)*(zhixiny - cy));
	if (zhixiny - cy < 0) {
		theangle = 360 - 180 / 3.1415926536*acos((zhixinx - cx) / r);
	}
	else {
		theangle = 180 / 3.1415926536*acos((zhixinx - cx) / r);
	}
	cout << "zhixin:["<<zhixinx<<","<<zhixiny<<"]";
	return theangle;
}

void printkp(Mat src, vector<KeyPoint>* keypoints) {
	vector<KeyPoint>::iterator tp1;
	float tempangle;
	std::cout << "***原始特征点信息****" << endl;
	for (tp1 = (*keypoints).begin(); tp1 != (*keypoints).end(); tp1++)
	{
		//tempangle = getorbangle(src, *tp1);
		tempangle = get_Angle(src, 15, (tp1)->pt);
		std::cout << "PT=" << tp1->pt << " " << "ANGLE=" << tp1->angle<< "  ORB-ANGLE=" << tempangle << "\n";
	}
	std::cout << "\n";
}

float get_Angle(const Mat& image, const int half_k, Point2f pt) {

	int halfPatchSize = 31 / 2;
	vector<int> umax(halfPatchSize + 2);

	int v, v0, vmax = cvFloor(halfPatchSize * sqrt(2.f) / 2 + 1);
	int vmin = cvCeil(halfPatchSize * sqrt(2.f) / 2);
	for (v = 0; v <= vmax; ++v)
		umax[v] = cvRound(sqrt((double)halfPatchSize * halfPatchSize - v * v));

	// Make sure we are symmetric
	for (v = halfPatchSize, v0 = 0; v >= vmin; --v)
	{
		while (umax[v0] == umax[v0 + 1])
			++v0;
		umax[v] = v0;
		++v0;
	}


	int m_01 = 0, m_10 = 0;

	const uchar* center = &image.at<uchar>(cvRound(pt.y), cvRound(pt.x));

	// Treat the center line differently, v=0
	for (int u = -half_k; u <= half_k; ++u)
		m_10 += u * center[u];

	// Go line by line in the circular patch
	int step = (int)image.step1();
	for (int v = 1; v <= half_k; ++v)
	{
		// Proceed over the two lines
		int v_sum = 0;
		int d = umax[v];
		for (int u = -d; u <= d; ++u)
		{
			int val_plus = center[u + v * step], val_minus = center[u - v * step];
			v_sum += (val_plus - val_minus);
			m_10 += u * (val_plus + val_minus);
		}
		m_01 += v * v_sum;
	}
	return fastAtan2((float)m_01, (float)m_10);
}