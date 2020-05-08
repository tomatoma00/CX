// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <signal.h>
#include <opencv.hpp>
#include "opencv2/core/core.hpp"
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include <direct.h>
#define _CRT_SECURE_NO_WARNINGS

using namespace cv;
using namespace std;

int Fmatch(char * file_1, char * file_2, float rate);
float compareAndWrite(char* head_1, int begin_1, int end_1, char* head_2, int begin_2, int end_2, char* record_file, char* result_file);

int main()
{
	char head_1[] = "图像库/05001-3400";
	char head_2[] = "图像库/05001-5200";
	char head_3[] = "图像库/05052-2600";
	char head_4[] = "图像库/05052-2400";
	char head_5[] = "图像库/05046-4900";
	char head_6[] = "图像库/05055-1200";
	char head_7[] = "图像库/05048-1900";
	char head_8[] = "unused05048-1800";
	char head_9[] = "unused05009-1400";
	char head_10[] = "unused05046-1000";
	char* head[10];
	head[0] = head_1;
	head[1] = head_2;
	head[2] = head_3;
	head[3] = head_4;
	head[4] = head_5;
	head[5] = head_6;
	head[6] = head_7;
	head[7] = head_8;
	head[8] = head_9;
	head[9] = head_10;
	//定义输出文件名
	char shuchu1[25];
	char shuchu2[25];

	

	if (1) {
      strcpy(shuchu1, "zuofei1-1结果.txt");
	  strcpy(shuchu2, "zuofei1-图库结果.txt");
	  compareAndWrite(head[3],3, 3, head[3],61,61, shuchu1, shuchu2);
	  //compareAndWrite(head[3], 1, 1, head[3], 61, 61, shuchu1, shuchu2);
	}
	else {
        strcpy(shuchu1, "0418结果.txt");
	    strcpy(shuchu2, "418图库结果.txt");
	    for (int num = 3; num < 8; num++)
	    {
		
		  int i;
		  for (i = 0; i < 7; i++) {
             compareAndWrite(head[1], num, num, head[i], 0,99,shuchu1 ,shuchu2 );
		  }

	    }
	
	}
    
	
	//printf("%f", rate_sum / 120);
	system("pause");
	return 0;
	
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
