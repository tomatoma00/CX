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
	/***************************��ȡ�����㣬����ϸ�ڵ���ȡ����**********************/
	float score;
	unsigned char TempFeature1[1202], TempFeature2[1202];

	Ffeature(file_1, TempFeature1);
	Ffeature(file_2, TempFeature2);//��������Ϣ��ȡ

	int len1 = (int)TempFeature1[0], len2 = (int)TempFeature2[0];
	vector<KeyPoint> keyPoints_1(len1), keyPoints_2(len2);
	cvtKeyPoint(TempFeature1, &keyPoints_1);
	cvtKeyPoint(TempFeature2, &keyPoints_2);//��������Ϣת��������

	Mat img_1 = imread(file_1);
	Mat img_2 = imread(file_2);
	if (!img_1.data || !img_2.data)
	{
		cout << "error reading images " << endl;
		return 0;
	}//��ͼ��
	
	Mat imggray_1, imggray_2, imggray_01,imggray_02;
	cvtColor(img_1, imggray_1, CV_BGR2GRAY);
	cvtColor(img_2, imggray_2, CV_BGR2GRAY);//��ͼƬת���ɻҶ�ͼ

	/********************************������Ե���Ӱ�����Ż�*********************************/
	bianyuandian(imggray_1, &keyPoints_1);
	bianyuandian(imggray_2, &keyPoints_2);
	if (keyPoints_1.size() == 0 || keyPoints_2.size() == 0)  return 0;
	/************************orb��������ȡ���Ż�����������ͼƬ**************************************************/
	vector<KeyPoint> keyPoints_3(8000), keyPoints_4(8000);
	OrbFeatureDetector orbdetect;
	/*
	orbdetect.detect(imggray_1, keyPoints_3);
	orbdetect.detect(imggray_2, keyPoints_4);
	bianyuandian(imggray_1, &keyPoints_3);
	bianyuandian(imggray_2, &keyPoints_4);*/

	youhua2(&keyPoints_3, &keyPoints_1,5, imggray_1);
	youhua2(&keyPoints_4, &keyPoints_2,5, imggray_2);//��ϸ�ڵ�12ȥget�Ż���34
	printkp(imggray_1, &keyPoints_1);
	printkp(imggray_2, &keyPoints_2);
	if (keyPoints_1.size() == 0 || keyPoints_2.size() == 0 || keyPoints_3.size() == 0 || keyPoints_4.size() == 0) return 0;
	
	/**************************************����������****************************************/
	//ORB�����Ѿ��޸�;//Ĭ�� ORB(int nfeatures = 500, float scaleFactor = 1.2f, int nlevels = 8, int edgeThreshold = 31,int firstLevel = 0, int WTA_K = 2, int scoreType = ORB::HARRIS_SCORE, int patchSize = 31 );
	OrbDescriptorExtractor OrbDescriptor;
	Mat imageDesc1, imageDesc2,imageDesc4, imageDesc3;
	OrbDescriptor.compute(imggray_1, keyPoints_1, imageDesc1);
	OrbDescriptor.compute(imggray_2, keyPoints_2, imageDesc2);
	OrbDescriptor.compute(imggray_1, keyPoints_3, imageDesc3);
	OrbDescriptor.compute(imggray_2, keyPoints_4, imageDesc4);
	

	 
	/********************************************��������ƥ��ͼ���÷�***********************************************************/
    Ptr<DescriptorMatcher> d_matcher = DescriptorMatcher::create("BruteForce-Hamming(2)");
	BFMatcher b_matcher(NORM_HAMMING2, true);// BFMatcher( int normType= NORM_HAMMING2, bool crossCheck=true );

	if (keyPoints_1.size() == 0 || keyPoints_2.size() == 0||keyPoints_3.size() == 0 || keyPoints_4.size() == 0) {
           return 0;
		}
	/**************************************��һ������ϸ�ڵ���ȡ��ƥ����÷�***********************/
	vector<DMatch> matches1(4000);
	vector<DMatch> matches2(4000);
	vector<DMatch> good_matches;	
	/*****************����ƥ��******************/

	/*********************************************/

	/*********************************************/
	b_matcher.match(imageDesc3, imageDesc4, matches1);
	b_matcher.match(imageDesc4, imageDesc3, matches2);
	sort(matches1.begin(), matches1.end());
	sort(matches2.begin(), matches2.end());

	/*********************************************/

    /*********************************************/

	/******************************����distance����÷�********************************/
	//������ľ������С�ľ���
	int sz1 = matches1.size(), sz2 = matches2.size();
	sz1 = sz1 < 5 ? sz1 : 5; sz2 = sz2 < 5 ? sz2 : 5;
	float sum1 = 0, sum2 = 0;//ԭʼ����
	float flag1 = 1, flag2 = 1;


	cout << "hamming�������Ӧ����+angle��"<<"\n";
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
			"--����:" << dist<<"\n";
		
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
	
	//***********ѡ�������match************
	vector<Point2f> recognized;
	vector<Point2f> scene;
	//ѡ���Ƿ�Ҫ���ͼƬ
	if (0) {
	recognized.resize(500);
	scene.resize(500);
	cout << "\n************\n";

	sz1 = matches1.size();
	for (int i = 0; i < sz1; i++)
	{
		if (matches1[i].distance < 30 )//����������֣�
		{
			good_matches.push_back(matches1[i]);
			//vector������ݵķ����ǡ�push_back����������ʾ��������ӵ�vector��β����������Ҫ�������ڴ� ���� ��vector<Widget>�����10�����ݣ����룺for(int i = 0; i < 10; i++) { vWidgets.push_back(Widget(i)); }
		}
	}

	//��Ȧ
	for (size_t i = 0; i < good_matches.size(); ++i)
	{
		//scene.push_back(keyPoints_4[good_matches[i].trainIdx].pt);
	}

	//for (unsigned int j = 0; j < scene.size(); j++)
		//circle(img_2, scene[j], 2, Scalar(0, 255, 0), 2);//��ȦȦ����

	//������ͨƥ����
	//resize(img_1, img_1, Size(size, size));
	//resize(img_2, img_2, Size(size, size));
	Mat ShowMatches;
	drawMatches(imggray_1, keyPoints_3, imggray_2, keyPoints_4, matches1, ShowMatches);
	//imshow("matches", ShowMatches);
	imwrite("matches.jpg", ShowMatches);

	//��������ƥ����
	Mat ShowGoodMatches;
	drawMatches(imggray_1, keyPoints_3, imggray_2, keyPoints_4, good_matches, ShowGoodMatches);
	//imshow("good_matches", ShowGoodMatches);
	imwrite("good_matches.jpg", ShowGoodMatches);
	
	}
	

	/**********************************���ݴ�����*************************************/
	sum1 = 0.5*sum1 + 0.5*sum2;
	//return 0;
	return log(sum1+1);//Ԫ�ظ���
	
}


