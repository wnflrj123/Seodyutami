#ifndef SEODYUTAMI_DISPARITYMAP_MATCHING_DISPARITYMAPMAKERIMPL_H
#define SEODYUTAMI_DISPARITYMAP_MATCHING_DISPARITYMAPMAKERIMPL_H

#include <string>
#include <opencv\cv.h>
#include <memory>
#include "estimationType.h"

class DisparityMapMakerImpl {
public:
	DisparityMapMakerImpl(const std::string leftImagePath, const std::string rightImagePath, const int& windowSize, const int& dRange, IEstimationType* type);
	~DisparityMapMakerImpl();
	IplImage* getDisparityMapPtr();
	void deleteMap();

private:
	const std::unique_ptr<IEstimationType> pEstimationType;
	IplImage* leftImage;
	IplImage* rightImage;
	IplImage* disparityMap;
	int windowSize;
	int dRange;
	bool isMapDeleted;

	long long int getDisparityValue(const int& x, const int& y);
	bool isSameImageSize();
};

#endif