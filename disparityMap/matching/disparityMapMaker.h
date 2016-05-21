#ifndef SEODYUTAMI_DISPARITYMAP_MATCHING_DISPARITYMAPMAKER_H
#define SEODYUTAMI_DISPARITYMAP_MATCHING_DISPARITYMAPMAKER_H

#include <string>
#include <vector>
#include <opencv\cv.h>
#include <memory>
#include "estimationType.h"

class DisparityMapMakerImpl;

class DisparityMapMaker {
public:
	DisparityMapMaker(const int& windowSize, const int& dRange, IEstimationType* type);
	std::shared_ptr<IplImage> getDisparityMapPtr(const IplImage* leftGrayImage, const IplImage* rightGrayImage);

private:
	const std::shared_ptr<DisparityMapMakerImpl> pImpl;
};

#endif