#ifndef SEODYUTAMI_DISPARITYMAP_MATCHING_DISPARITYMAPMAKER_H
#define SEODYUTAMI_DISPARITYMAP_MATCHING_DISPARITYMAPMAKER_H

#include <string>
#include <opencv\cv.h>
#include <memory>
#include "estimationType.h"

class DisparityMapMakerImpl;

class DisparityMapMaker {
public:
	DisparityMapMaker(const std::string leftImagePath, const std::string rightImagePath, const int& windowSize, const int& dRange, IEstimationType* type);
	IplImage* getDisparityMapPtr();

private:
	const std::unique_ptr<DisparityMapMakerImpl> pImpl;
};

#endif