#ifndef ASSIGNMENT4_H
#define ASSIGNMENT4_H

/**********************************************************
* Libraries and gen. declarations for 4420 Assignment 4   *
* Author: Allie Robbins                                   *
* Language: C++ with OpenCV libraries                     *
**********************************************************/

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <bitset>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <unistd.h>
#include <vector>

using namespace cv;
using namespace std;

/* Variable Declarations */
const double PI = 3.141592653589793238463;
int structuringElement[3][3] = {{0, 1, 0}, {1, 1, 1}, {0, 1, 0}};

/* Function Declarations */
double findDistance(Point, Point);
int findPastelValue(int, int, double);
void getPixel(int &, int &, int &, Mat);
void printImage(Mat, const char*);
void reflect(int &, int, int);

#endif
