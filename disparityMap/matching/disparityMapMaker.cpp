#include <iostream>
#include <opencv\highgui.h>
#include <omp.h>
#include "disparityMapMaker.h"
#include "disparityMapMakerImpl.h"

DisparityMapMaker::DisparityMapMaker(const int& windowSize, const int& dRange, IEstimationType* type) 
:pImpl(new DisparityMapMakerImpl(windowSize, dRange, type))
{}

std::shared_ptr<IplImage> DisparityMapMaker::getDisparityMapPtr(const IplImage* leftGrayImage, const IplImage* rightGrayImage) {
	return pImpl->getDisparityMapPtr(leftGrayImage, rightGrayImage);
}