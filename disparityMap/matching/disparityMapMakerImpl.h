#ifndef SEODYUTAMI_DISPARITYMAP_MATCHING_DISPARITYMAPMAKERIMPL_H
#define SEODYUTAMI_DISPARITYMAP_MATCHING_DISPARITYMAPMAKERIMPL_H

#include <opencv\cv.h>
#include <memory>
#include "estimationType.h"

class DisparityMapMakerImpl {
public:
	DisparityMapMakerImpl(const int& windowSize, const int& dRange, IEstimationType* type);
	~DisparityMapMakerImpl();
	std::shared_ptr<IplImage> getDisparityMapPtr(const IplImage* leftGrayImage, const IplImage* rightGrayImage);

private:
	const std::unique_ptr<IEstimationType> pEstimationType;
	int windowSize;
	int dRange;

	long long int getDisparityValue(const IplImage* leftGrayImage, const IplImage* rightGrayImage, const int& x, const int& y);
};

#endif