#include <iostream>
#include <opencv\highgui.h>
#include <omp.h>
#include "disparityMapMaker.h"
#include "disparityMapMakerImpl.h"

DisparityMapMaker::DisparityMapMaker(const std::string leftImagePath, const std::string rightImagePath, const int& windowSize, const int& dRange, IEstimationType* type) 
:pImpl(new DisparityMapMakerImpl(leftImagePath, rightImagePath, windowSize, dRange, type))
{}

IplImage* DisparityMapMaker::getDisparityMapPtr() {
	return pImpl->getDisparityMapPtr();
}