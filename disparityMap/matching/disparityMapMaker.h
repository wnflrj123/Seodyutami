#ifndef SEODYUTAMI_DISPARITYMAP_MATCHING_MATCHING_H
#define SEODYUTAMI_DISPARITYMAP_MATCHING_MATCHING_H

#include <string>
#include <opencv\cv.h>
#include <memory>
#include "estimationType.h"

class DisparityMapMaker {
public:
	DisparityMapMaker(const std::string leftImagePath, const std::string rightImagePath, const int& windowSize, const int& dRange, IEstimationType* type);
	~DisparityMapMaker();
	IplImage* getDisparitMapPtr();

private:
	const std::unique_ptr<IEstimationType> pEstimationType;
	IplImage* leftImage;
	IplImage* rightImage;
	IplImage* disparityMap;
	int windowSize;
	int dRange;

	long long int getDisparityValue(const int& x, const int& y);
	bool isSameImageSize();
};

#endif