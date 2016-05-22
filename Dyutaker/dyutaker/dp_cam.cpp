#pragma once
#include "stdafx.h"
#define SEGMENTATION_ENABLE	1

#include "dp_cam.h"

void DisparityMapMaker::getDisparitMap()
{
	cvSet(disparityMap, cvScalar(205));
	cvFindStereoCorrespondence(grayRightImage, grayLeftImage, CV_DISPARITY_BIRCHFIELD, disparityMap, dRange, 30, 10, 3, 8, 15);
	int scaleFactor = 255 / dRange;
	cvConvertScale(disparityMap, disparityMap, scaleFactor);
}


void DisparityMapMaker::getSegmenatationMarkers()
{
	// seedStep 간격마다 seed를 뿌려서 markers 생성
	int height = leftImageSize.height;
	int width = leftImageSize.width;
	cvSetZero(markers);
	int seedNum = 1;
	for (int row = seedStep; row < height; row += seedStep) {
		int* ptr = (int*)(markers->data.ptr + row * markers->step);
		for (int col = seedStep; col < width; col += seedStep) {
			ptr[col] = seedNum;
			seedNum++;
		}
	}

	// watershed 알고리즘 실행
	cvWatershed(leftImage, markers);
}

void DisparityMapMaker::getSegmentBasedDisparityAverage()
{
	// marker에 해당하는 인덱스에 값을 누적하여 평균을 계산
	for (int i = 0; i < nMarker; i++) {
		// 픽셀 카운트
		markerTable[0][i] = 0;
		// dispairty 값 누적
		markerTable[1][i] = 0;
	}

	// disparity 값 누적 및 픽셀 카운트
	int height = leftImageSize.height;
	int width = leftImageSize.width;

	for (int i = 1; i < height; i++) {
		uchar* pDispairty = (uchar*)(disparityMap->imageData + i*disparityMap->widthStep);
		int* pMarkers = (int*)(markers->data.ptr + i * markers->step);
		for (int j = 0; j < width; j++) {
			if (pDispairty[j] != 0) {
				if (pMarkers[j] != -1) {
					markerTable[0][pMarkers[j]]++;
					markerTable[1][pMarkers[j]] += pDispairty[j];
				}
			}
		}
	}

	// 평균계산
	for (int i = 0; i < nMarker; i++) {
		if (markerTable[0][i] != 0) {
			markerTable[1][i] /= markerTable[0][i];
		}
	}
}

IplImage* DisparityMapMaker::drawSegmentBasedDisparityMap()
{
	// 계산한 평균 값으로 disparity map 다시 그리기
	int height = leftImageSize.height;
	int width = leftImageSize.width;
	for (int i = 0; i < height; i++) {
		uchar* pDispairty = (uchar*)(disparityMap->imageData + i*disparityMap->widthStep);
		int* pMarkers = (int*)(markers->data.ptr + i * markers->step);
		for (int j = 0; j < width; j++) {
			if (pMarkers[j] == -1) {
				pDispairty[j] = pDispairty[j - 1];
			}
			else {
				pDispairty[j] = markerTable[1][pMarkers[j]];
			}
		}
	}

	return disparityMap;
}

DisparityMapMaker::DisparityMapMaker()
{
	CvSize size;
	size.width = 240;
	size.height = 360;

	this->leftImage = cvCreateImage(size, IPL_DEPTH_8U, 3);
	this->rightImage = cvCreateImage(size, IPL_DEPTH_8U, 3);
	this->grayLeftImage = cvCreateImage(size, IPL_DEPTH_8U, 1);
	this->grayRightImage = cvCreateImage(size, IPL_DEPTH_8U, 1);
	CvSize leftImageSize = cvGetSize(leftImage);
	disparityMap = cvCreateImage(leftImageSize, IPL_DEPTH_8U, 1);
	markers = cvCreateMat(leftImageSize.height, leftImageSize.width, CV_32SC1);
}

DisparityMapMaker::~DisparityMapMaker()
{
	cvReleaseImage(&grayLeftImage);
	cvReleaseImage(&grayRightImage);
	cvReleaseMat(&markers);

	for (int i = 0; i < 2; i++) {
		delete[] markerTable[i];
	}
	delete[] markerTable;
}


void DisparityMapMaker::initialSetting(int seedStep, int windowSize, int dRange)
{
	this->seedStep = seedStep;
	this->windowSize = windowSize;
	this->dRange = dRange;

	this->leftImageSize = cvGetSize(leftImage);
	this->rightImageSize = cvGetSize(leftImage);

	this->nMarker = leftImageSize.height * leftImageSize.width / (seedStep * seedStep);

	markerTable = new int*[2];
	for (int i = 0; i < 2; i++) {
		markerTable[i] = new int[nMarker];
	}
}

void DisparityMapMaker::setImage(IplImage* leftImage, IplImage* rightImage)
{
	IplImage* temp = cvCreateImage(cvGetSize(leftImage), IPL_DEPTH_8U, 1);
	cvCvtColor(leftImage, temp, CV_BGR2GRAY);
	cvResize(temp, this->grayLeftImage, 1);
	cvCvtColor(rightImage, temp, CV_BGR2GRAY);
	cvResize(temp, this->grayRightImage, 1);

	cvResize(leftImage, this->leftImage, 1);
	cvResize(rightImage, this->rightImage, 1);
	cvReleaseImage(&temp);
}

IplImage* DisparityMapMaker::getSegmentBasedDisparityMap()
{
#if SEGMENTATION_ENABLE
	getDisparitMap();
	getSegmenatationMarkers();
	getSegmentBasedDisparityAverage();
	disparityMap = drawSegmentBasedDisparityMap();
#else
	getDisparitMap();
#endif
	return disparityMap;
}




























// 아래 함수를 어디로 보낼지 고민중
#include "changeViewPoint.h"
#include "imageFusion.h"

void getDisparityMapAndMultifocusVideos(const char* path1, const char* path2, int delayFrame)
{
	clock_t start_time, end_time;
	CvMat* mx1 = (CvMat*)cvLoad("mx1.xml");
	CvMat* my1 = (CvMat*)cvLoad("my1.xml");
	CvMat* mx2 = (CvMat*)cvLoad("mx2.xml");
	CvMat* my2 = (CvMat*)cvLoad("my2.xml");

	CvCapture* capture1 = cvCreateFileCapture(path1);
	CvCapture* capture2 = cvCreateFileCapture(path2);
	IplImage* leftImage = cvQueryFrame(capture1);
	IplImage* rightImage = cvQueryFrame(capture2);
	CvSize size = cvGetSize(leftImage);
	CvSize RotateSize = cvSize(size.height, size.width);
	IplImage* leftImageRotation = cvCreateImage(RotateSize, IPL_DEPTH_8U, 3);
	IplImage* rightImageRotation = cvCreateImage(RotateSize, IPL_DEPTH_8U, 3);
	cvFlip(leftImage, leftImage, 0);
	cvFlip(leftImage, leftImage, 1);
	cvTranspose(leftImage, leftImageRotation);
	cvTranspose(rightImage, rightImageRotation);

	DisparityMapMaker maker;
	Fusion fusion;
	CVP cvp;
	maker.initialSetting(8, 1, 60);
	maker.setImage(leftImageRotation, rightImageRotation);

	IplImage* disparityMap = maker.getSegmentBasedDisparityMap();
	CvSize disparityMapSize = cvGetSize(disparityMap);
	IplImage* preDisparityMap1 = cvCloneImage(disparityMap);
	IplImage* preDisparityMap2 = cvCloneImage(disparityMap);
	IplImage* preDisparityMap3 = cvCloneImage(disparityMap);
	IplImage* filtered1 = cvCreateImage(cvGetSize(disparityMap), IPL_DEPTH_8U, 1);
	IplImage* filtered2 = cvCreateImage(cvGetSize(disparityMap), IPL_DEPTH_8U, 1);
	IplImage* filteredDisaprityMap = cvCreateImage(cvGetSize(disparityMap), IPL_DEPTH_8U, 1);
	IplImage* filteredDisaprityMapRotation = cvCreateImage(cvSize(disparityMapSize.height, disparityMapSize.width), IPL_DEPTH_8U, 1);

	IplImage* resizedDisparityMap = cvCreateImage(RotateSize, IPL_DEPTH_8U, 1);
	IplImage* convertImage = cvCreateImage(size, IPL_DEPTH_8U, 3);
	IplImage* multiFocusImage = cvCreateImage(size, IPL_DEPTH_8U, 3);

	IplImage* tempOut1 = cvCreateImage(size, IPL_DEPTH_8U, 1);
	IplImage* tempOut2 = cvCreateImage(size, IPL_DEPTH_8U, 3);

	int fourcc = CV_FOURCC('M', 'J', 'P', 'G');
	CvVideoWriter* disparityMapVideoOut = cvCreateVideoWriter("dspMap.avi", fourcc, 24, size, 0);
	CvVideoWriter* multifocusVideoOut = cvCreateVideoWriter("multifocus.avi", fourcc, 24, size, 1);

	// 딜레이된 프레임을 제거함
	int nCutFrame = delayFrame;
	for (int i = 0; i < nCutFrame; i++) {
		rightImage = cvQueryFrame(capture1);
	}


	while (1) {
		start_time = clock();

		//getting images
		leftImage = cvQueryFrame(capture1);
		rightImage = cvQueryFrame(capture2);
		if (!leftImage || !rightImage)
			break;
		cvFlip(leftImage, leftImage, 0);
		cvFlip(leftImage, leftImage, 1);
		cvTranspose(leftImage, leftImageRotation);
		cvTranspose(rightImage, rightImageRotation);


		//disparity map image generation
		maker.setImage(leftImageRotation, rightImageRotation);
		preDisparityMap3 = cvCloneImage(preDisparityMap2);
		preDisparityMap2 = cvCloneImage(preDisparityMap1);
		preDisparityMap1 = cvCloneImage(disparityMap);
		disparityMap = maker.getSegmentBasedDisparityMap();
		cvAddWeighted(preDisparityMap1, 0.25, disparityMap, 0.25, 0, filtered1);
		cvAddWeighted(preDisparityMap2, 0.25, preDisparityMap3, 0.25, 0, filtered2);
		cvAddWeighted(filtered1, 1, filtered2, 1, 0, filteredDisaprityMap);
		cvResize(filteredDisaprityMap, resizedDisparityMap);


		// getting view point changed image and focus fusion
		cvp.setImg(resizedDisparityMap, leftImageRotation, rightImageRotation);
		cvp.runcvt(&convertImage);
		fusion.setImg(leftImageRotation, convertImage);
		fusion.sml(&multiFocusImage);


		//file save
		cvTranspose(resizedDisparityMap, tempOut1);
		cvWriteFrame(disparityMapVideoOut, tempOut1);
		cvTranspose(multiFocusImage, tempOut2);
		cvWriteFrame(multifocusVideoOut, tempOut2);


		cvWaitKey(1);
		end_time = clock();
		cout << (double)1000 / (end_time - start_time) << "	FPS" << endl;

	}
	cvReleaseVideoWriter(&disparityMapVideoOut);
	cvReleaseVideoWriter(&multifocusVideoOut);

	cvReleaseImage(&tempOut1);
	cvReleaseImage(&tempOut2);


	cvReleaseImage(&resizedDisparityMap);

	cvReleaseImage(&filtered1);
	cvReleaseImage(&filtered2);
	cvReleaseImage(&filteredDisaprityMap);
	cvReleaseImage(&filteredDisaprityMapRotation);
	cvReleaseImage(&preDisparityMap1);
	cvReleaseImage(&preDisparityMap2);
	cvReleaseImage(&preDisparityMap3);
	cvReleaseImage(&disparityMap);

	cvReleaseImage(&leftImageRotation);
	cvReleaseImage(&rightImageRotation);

	cvReleaseCapture(&capture1);
	cvReleaseCapture(&capture2);

	cvReleaseMat(&mx1);
	cvReleaseMat(&my1);
	cvReleaseMat(&mx2);
	cvReleaseMat(&my2);

	return;
}