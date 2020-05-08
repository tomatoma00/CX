#include <windows.h>
#include <iostream>
#include <fstream>
#include <signal.h>
#include <vector>
#include <stdio.h>
#include <direct.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

using namespace cv;
using namespace std;


float Fmatch(char * file_1, char * file_2, float rate);

float compareAndWrite(char* head_1, int begin_1, int end_1, char* head_2, int begin_2, int end_2, char* record_file, char* result_file)
{
	float score;
	//char head[] = "图像库/05001-3400";
	char pic_1[] = "图像库/00000-0000000000", pic_2[] = "图像库/00000-0000000000";
	int i, j;
	char number[] = "00";//用于存放个位数和十位数
	//以下为测试时用的变量
	int score_sum;
	int scoreall_sum=0,scoretrue_sum=0;//用于记录0和非0的比例
	float truerate=0;

	//open the file and write at its end
	//fout_1:record every match of other pics
	//fout_2:record the score of each fingerprint
	ofstream fout_1(record_file, ios_base::out | ios_base::app);
	ofstream fout_2(result_file, ios_base::out | ios_base::app);

	
	for (i = begin_1; i <= end_1; i++)
	{
		score_sum = 0;
		//接下来的if是在生成文件名的末两位数字
		if (i / 10 && i % 10)
		{
			number[0] = (char)('0' + i / 10);
			number[1] = (char)('0' + i % 10);
		}
		else if (!(i % 10) && (i / 10))
		{
			number[0] = (char)('0' + i / 10);
			number[1] = '0';
		}
		else if ((i % 10) && !(i / 10))
		{
			number[0] = '0';
			number[1] = (char)('0' + i % 10);
		}
		else
		{
			number[0] = '0';
			number[1] = '0';
		}
		//有了末两位数，就可以生成正式文件名了
		strcpy(pic_1, head_1);
		strcat(pic_1, number);
		strcat(pic_1, ".bmp");

		//第二个文件也是一样
		for (j = begin_2; j <= end_2; j++)
		{
			if (j / 10 && j % 10)
			{
				number[0] = (char)('0' + j / 10);
				number[1] = (char)('0' + j % 10);
			}
			else if (!(j % 10) && (j / 10))
			{
				number[0] = (char)('0' + j / 10);
				number[1] = '0';
			}
			else if ((j % 10) && !(j / 10))
			{
				number[0] = '0';
				number[1] = (char)('0' + j % 10);
			}
			else
			{
				number[0] = '0';
				number[1] = '0';
			}
			//有了末两位数，就可以生成正式文件名了
			strcpy(pic_2, head_2);
			strcat(pic_2, number);
			strcat(pic_2, ".bmp");

			if (strcmp(pic_1, pic_2) != 0) {
			//去除相同手指的匹配
                score = Fmatch(pic_1, pic_2, 0.65f);
			    score_sum += score;

               if(score>-0.01){
			     //write txt
			      if (strcmp(head_1, head_2) == 0) {cout << "zhen ";fout_1 << "zhen ";}
			      else { cout << "jia  "; fout_1 << "jia  "; }

                  cout << pic_1 << "       " << pic_2 << "   " << score << endl;
                  fout_1 << pic_1 << "       " << pic_2 << "   " << score << endl;
				}
				 
			}
			

		}


		fout_2 << pic_1 << "   " << score_sum << "   " << head_2 << endl;
		//fout_2 << score_sum << endl;

		//调试程序
		//scoreall_sum++;
		//if (score_sum) scoretrue_sum++;//用来计算匹配值为非0的几率，可以算得拒真率与存假率
		//truerate = scoretrue_sum * 1.0 / scoreall_sum;
	}
	fout_1.close();
	fout_2.close();
	return truerate;
	
}
