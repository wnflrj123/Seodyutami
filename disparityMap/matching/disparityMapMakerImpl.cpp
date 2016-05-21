#include <iostream>
#include <omp.h>
#include "DisparityMapMakerImpl.h"
#include "opencvReleaseFunctions.h"

DisparityMapMakerImpl::DisparityMapMakerImpl(const int& windowSize, const int& dRange, IEstimationType* type)
: pEstimationType(type), windowSize(windowSize), dRange(dRange) {
}

DisparityMapMakerImpl::~DisparityMapMakerImpl() {
}

std::shared_ptr<IplImage> DisparityMapMakerImpl::getDisparityMapPtr(const IplImage* leftGrayImage, const IplImage* rightGrayImage) {
	if (pEstimationType == 0) {
		std::cout << "Please set estimation type of disparity map" << std::endl;
		return 0;
	}

	IplImage* disparityMap = cvCreateImage(cvGetSize(leftGrayImage), IPL_DEPTH_8U, 1);

	for (int i = windowSize + 1; i < disparityMap->height - windowSize; i++) {
		uchar* ptr = (uchar*)(disparityMap->imageData + i*disparityMap->widthStep);
		for (int j = windowSize + 1; j < disparityMap->width - windowSize - dRange; j++) {
			ptr[j] = (uchar)(getDisparityValue(leftGrayImage, rightGrayImage, j, i) * 255 / dRange);
		}
	}

	// TODO 객체 생성 및 복사로 인한 속도 이슈 없는지 확인
	return std::shared_ptr<IplImage>(disparityMap, releaseImageForSharedPtr);
}

long long int DisparityMapMakerImpl::getDisparityValue(const IplImage* leftGrayImage, const IplImage* rightGrayImage, const int& x, const int& y) {
	long long int minSADvalue = 0x7fffffffffffffff;
	long long int disparity = 0;

#pragma omp parallel for schedule(dynamic)
	for (int d = 0; d < dRange; d++) {
		long long int value = pEstimationType->getValue(leftGrayImage, rightGrayImage, x, y, windowSize, d);
		if (minSADvalue > value) {
			minSADvalue = value;
			disparity = d;
		}
	}

	return disparity;
}