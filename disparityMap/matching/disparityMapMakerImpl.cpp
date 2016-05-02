#include <iostream>
#include <opencv\highgui.h>
#include <omp.h>
#include "DisparityMapMakerImpl.h"

DisparityMapMakerImpl::DisparityMapMakerImpl(const std::string leftImagePath, const std::string rightImagePath, const int& windowSize, const int& dRange, IEstimationType* type)
: pEstimationType(type),
leftImage(cvLoadImage(leftImagePath.c_str(), 0)),
rightImage(cvLoadImage(rightImagePath.c_str(), 0)),
disparityMap(cvCreateImage(cvSize(leftImage->width, leftImage->height), IPL_DEPTH_8U, 1)),
windowSize(windowSize),
dRange(dRange) {
	if (!isSameImageSize()) {
		//�̷��� �Ҹ�� ��ü�� ��� �Ǵ°���??? �������� ������ �ϴ°ɱ�?
		//�ƿ� �����ڰ� ���������� �������� �ʵ��� ����ó���� ���ִ°� ���.
		this->~DisparityMapMakerImpl();
	}
}

DisparityMapMakerImpl::~DisparityMapMakerImpl() {
	//cvReleaseImage ���� ���ܰ� �߻��ϸ�??
	if (leftImage) cvReleaseImage(&leftImage);
	if (rightImage) cvReleaseImage(&rightImage);
	if (disparityMap) cvReleaseImage(&disparityMap);
}

IplImage* DisparityMapMakerImpl::getDisparityMapPtr() {
	if (pEstimationType == 0) {
		std::cout << "Please set estimation type of disparity map" << std::endl;
		return 0;
	}

	for (int i = windowSize + 1; i < disparityMap->height - windowSize; i++) {
		uchar* ptr = (uchar*)(disparityMap->imageData + i*disparityMap->widthStep);
		for (int j = windowSize + 1; j < disparityMap->width - windowSize - dRange; j++) {
			ptr[j] = (uchar)(getDisparityValue(j, i) * 255 / dRange);
		}
	}

	return disparityMap;
}

long long int DisparityMapMakerImpl::getDisparityValue(const int& x, const int& y) {
	long long int minSADvalue = 0x7fffffffffffffff;
	long long int disparity = 0;

#pragma omp parallel for schedule(dynamic)
	for (int d = 0; d < dRange; d++) {
		long long int value = pEstimationType->getValue(leftImage, rightImage, x, y, windowSize, d);
		if (minSADvalue > value) {
			minSADvalue = value;
			disparity = d;
		}
	}

	return disparity;
}

bool DisparityMapMakerImpl::isSameImageSize() {
	if ((leftImage->height != rightImage->height) || (leftImage->width != rightImage->width)) {
		std::cout << "Left and Right image must have a same ->" << std::endl;
		return false;
	}

	return true;
}