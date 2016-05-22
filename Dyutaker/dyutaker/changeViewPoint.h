#pragma once

#include <iostream>
#include "opencv\cv.h"
#include "opencv\highgui.h"
#include <omp.h>

using namespace std;

#define PixelDiff 15;

class CVP{
	IplImage* disparity;
	IplImage* imgR;
	IplImage* imgL;

public:
	CVP();
	~CVP();
	void setImg(IplImage* dis, IplImage* left, IplImage* right);
	void runcvt(IplImage** dst);
};