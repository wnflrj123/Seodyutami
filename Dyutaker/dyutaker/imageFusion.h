#pragma once

#include <iostream>
#include <omp.h>
#include "opencv\cv.h"
#include "opencv\highgui.h"

using namespace std;

class Fusion
{
	IplImage* img1;
	IplImage* img2;
	IplImage* img1Gray;
	IplImage* img2Gray;

public:
	Fusion();
	~Fusion();
	void setImg(IplImage* image1, IplImage* image2);
	void sml(IplImage** dst);
};