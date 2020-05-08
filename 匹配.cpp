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

using namespace cv;
using namespace std;

int SYFp_File_LoadBMP(char* file, unsigned char* pImage, int* pX, int* pY);
int Ffeature(char * src_img, unsigned char* TempFeature);
int cvtKeyPoint(unsigned char* TempFeature, vector<KeyPoint>* keypoints);
int bianyuandian(Mat src, vector<KeyPoint>* keypoints);
void youhua(vector<KeyPoint>* keypointsorb, vector<KeyPoint>* keypoints,double size);
void youhua2(vector<KeyPoint>* keypointsnew, vector<KeyPoint>* keypoints, double size, Mat src);
void lingyu(vector<KeyPoint>* keypoints, double beishu);
double getorbangle(Mat src, KeyPoint thekeypoint);
float get_Angle(const Mat& image, const int half_k, Point2f pt);
void printkp(Mat src, vector<KeyPoint>* keypoints);

float Fmatch(char * file_1, char * file_2, float rate)
{
	/***************************获取特征点，包括细节点提取函数**********************/
	float score;
	unsigned char TempFeature1[1202], TempFeature2[1202];

	Ffeature(file_1, TempFeature1);
	Ffeature(file_2, TempFeature2);//特征点信息提取

	int len1 = (int)TempFeature1[0], len2 = (int)TempFeature2[0];
	vector<KeyPoint> keyPoints_1(len1), keyPoints_2(len2);
	cvtKeyPoint(TempFeature1, &keyPoints_1);
	cvtKeyPoint(TempFeature2, &keyPoints_2);//特征点信息转化成向量

	Mat img_1 = imread(file_1);
	Mat img_2 = imread(file_2);
	if (!img_1.data || !img_2.data)
	{
		cout << "error reading images " << endl;
		return 0;
	}//打开图像
	
	Mat imggray_1, imggray_2, imggray_01,imggray_02;
	cvtColor(img_1, imggray_1, CV_BGR2GRAY);
	cvtColor(img_2, imggray_2, CV_BGR2GRAY);//将图片转化成灰度图

	/********************************消除边缘点的影响与优化*********************************/
	bianyuandian(imggray_1, &keyPoints_1);
	bianyuandian(imggray_2, &keyPoints_2);
	if (keyPoints_1.size() == 0 || keyPoints_2.size() == 0)  return 0;
	/************************orb特征点提取、优化、比例缩放图片**************************************************/
	vector<KeyPoint> keyPoints_3(8000), keyPoints_4(8000);
	OrbFeatureDetector orbdetect;
	/*
	orbdetect.detect(imggray_1, keyPoints_3);
	orbdetect.detect(imggray_2, keyPoints_4);
	bianyuandian(imggray_1, &keyPoints_3);
	bianyuandian(imggray_2, &keyPoints_4);*/

	youhua2(&keyPoints_3, &keyPoints_1,5, imggray_1);
	youhua2(&keyPoints_4, &keyPoints_2,5, imggray_2);//用细节点12去get优化的34
	printkp(imggray_1, &keyPoints_1);
	printkp(imggray_2, &keyPoints_2);
	if (keyPoints_1.size() == 0 || keyPoints_2.size() == 0 || keyPoints_3.size() == 0 || keyPoints_4.size() == 0) return 0;
	
	/**************************************特征点描述****************************************/
	//ORB参数已经修改;//默认 ORB(int nfeatures = 500, float scaleFactor = 1.2f, int nlevels = 8, int edgeThreshold = 31,int firstLevel = 0, int WTA_K = 2, int scoreType = ORB::HARRIS_SCORE, int patchSize = 31 );
	OrbDescriptorExtractor OrbDescriptor;
	Mat imageDesc1, imageDesc2,imageDesc4, imageDesc3;
	OrbDescriptor.compute(imggray_1, keyPoints_1, imageDesc1);
	OrbDescriptor.compute(imggray_2, keyPoints_2, imageDesc2);
	OrbDescriptor.compute(imggray_1, keyPoints_3, imageDesc3);
	OrbDescriptor.compute(imggray_2, keyPoints_4, imageDesc4);
	

	 
	/********************************************接下来是匹配和计算得分***********************************************************/
    Ptr<DescriptorMatcher> d_matcher = DescriptorMatcher::create("BruteForce-Hamming(2)");
	BFMatcher b_matcher(NORM_HAMMING2, true);// BFMatcher( int normType= NORM_HAMMING2, bool crossCheck=true );

	if (keyPoints_1.size() == 0 || keyPoints_2.size() == 0||keyPoints_3.size() == 0 || keyPoints_4.size() == 0) {
           return 0;
		}
	/**************************************第一部分是细节点提取的匹配与得分***********************/
	vector<DMatch> matches1(4000);
	vector<DMatch> matches2(4000);
	vector<DMatch> good_matches;	
	/*****************暴力匹配******************/

	/*********************************************/

	/*********************************************/
	b_matcher.match(imageDesc3, imageDesc4, matches1);
	b_matcher.match(imageDesc4, imageDesc3, matches2);
	sort(matches1.begin(), matches1.end());
	sort(matches2.begin(), matches2.end());

	/*********************************************/

    /*********************************************/

	/******************************根据distance计算得分********************************/
	//输出最大的距离和最小的距离
	int sz1 = matches1.size(), sz2 = matches2.size();
	sz1 = sz1 < 5 ? sz1 : 5; sz2 = sz2 < 5 ? sz2 : 5;
	float sum1 = 0, sum2 = 0;//原始数据
	float flag1 = 1, flag2 = 1;


	cout << "hamming距离与对应坐标+angle："<<"\n";
	int index1, index2;
	for (int i = 0; i < sz1; i++)
	{
		double dist = matches1[i].distance;
		index1 = matches1[i].queryIdx;
		index2 = matches1[i].trainIdx;
		if (dist < 29) {
			sum1 = sum1 + flag1 * (29 - dist);
			flag1 = flag1 + 1;
			//cout << dist << "-";		
		}
		cout <<"["<<keyPoints_3.at(index1).pt.x<<","<< keyPoints_3.at(index1).pt.y <<"]ANGLE=" << keyPoints_3.at(index1).angle<<
			"--[" << keyPoints_4.at(index2).pt.x << "," << keyPoints_4.at(index2).pt.y << "]ANGLE=" << keyPoints_4.at(index2).angle <<
			"--距离:" << dist<<"\n";
		
	}//cout << "\n";
	for (int i = 0; i < sz2; i++)
	{
		double dist = matches2[i].distance;
		if (dist < 29) {
			sum2 = sum2 + flag2 * (29 - dist);
			flag2 = flag2 + 1;
		}//cout << dist<<" ";

	}//cout << "\n";
	if (flag1 == 2 || flag2 == 2)return 0;
	
	//***********选择优秀的match************
	vector<Point2f> recognized;
	vector<Point2f> scene;
	//选择是否要输出图片
	if (0) {
	recognized.resize(500);
	scene.resize(500);
	cout << "\n************\n";

	sz1 = matches1.size();
	for (int i = 0; i < sz1; i++)
	{
		if (matches1[i].distance < 30 )//调整这个数字！
		{
			good_matches.push_back(matches1[i]);
			//vector添加数据的方法是。push_back（）函数表示将数据添加到vector的尾部，并按需要来分配内存 例如 向vector<Widget>中添加10个数据，代码：for(int i = 0; i < 10; i++) { vWidgets.push_back(Widget(i)); }
		}
	}

	//画圈
	for (size_t i = 0; i < good_matches.size(); ++i)
	{
		//scene.push_back(keyPoints_4[good_matches[i].trainIdx].pt);
	}

	//for (unsigned int j = 0; j < scene.size(); j++)
		//circle(img_2, scene[j], 2, Scalar(0, 255, 0), 2);//画圈圈函数

	//画出普通匹配结果
	//resize(img_1, img_1, Size(size, size));
	//resize(img_2, img_2, Size(size, size));
	Mat ShowMatches;
	drawMatches(imggray_1, keyPoints_3, imggray_2, keyPoints_4, matches1, ShowMatches);
	//imshow("matches", ShowMatches);
	imwrite("matches.jpg", ShowMatches);

	//画出良好匹配结果
	Mat ShowGoodMatches;
	drawMatches(imggray_1, keyPoints_3, imggray_2, keyPoints_4, good_matches, ShowGoodMatches);
	//imshow("good_matches", ShowGoodMatches);
	imwrite("good_matches.jpg", ShowGoodMatches);
	
	}
	

	/**********************************数据处理部分*************************************/
	sum1 = 0.5*sum1 + 0.5*sum2;
	//return 0;
	return log(sum1+1);//元素个数
	
}


