#ifndef DP_CAM_H
#define DP_CAM_H

#pragma warning( disable: 4996 )
#include <iostream>
#include <time.h>
#include <Windows.h>
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <vector>
#include <omp.h>
#include <emmintrin.h>
#include <opencv\cvaux.h>
using namespace std;

class DisparityMapMaker
{
	IplImage* leftImage;
	IplImage* rightImage;
	IplImage* grayLeftImage;
	IplImage* grayRightImage;
	IplImage* disparityMap;
	CvMat* markers;
	CvSize leftImageSize;
	CvSize rightImageSize;

	int windowSize;
	int dRange;
	int seedStep;
	int nMarker;
	int** markerTable;

	void getDisparitMap();
	void getSegmenatationMarkers();
	void getSegmentBasedDisparityAverage();
	IplImage* drawSegmentBasedDisparityMap();

public:
	DisparityMapMaker();
	~DisparityMapMaker();
	void initialSetting(int seedStep, int windowSize, int dRange);
	void setImage(IplImage* leftImage, IplImage* rightImage);
	IplImage* getSegmentBasedDisparityMap();
};

#endif

void getDisparityMapAndMultifocusVideos(const char* path1, const char* path2, int delayFrame);