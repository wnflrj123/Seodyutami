#ifndef SEODYUTAMI_DISPARITYMAP_MATCHING_ESTIMATIONTYPE_H
#define SEODYUTAMI_DISPARITYMAP_MATCHING_ESTIMATIONTYPE_H
#define interface struct

#include <opencv\cv.h>

//Empty Base Object
class Empty {};

interface IEstimationType : private Empty{
	virtual long long int getValue(const IplImage* leftImage, const IplImage* rightImage, int x, int y, int windowSize, int d) const = 0;
};

interface SADtype :public IEstimationType {
	long long int getValue(const IplImage* leftImage, const IplImage* rightImage, int x, int y, int windowSize, int d) const;
	static SADtype* getInstance();
};

interface SSDtype :public IEstimationType {
	long long int getValue(const IplImage* leftImage, const IplImage* rightImage, int x, int y, int windowSize, int d) const;
	static SSDtype* getInstance();
};

interface NCCtype :public IEstimationType {
	long long int getValue(const IplImage* leftImage, const IplImage* rightImage, int x, int y, int windowSize, int d) const;
	static NCCtype* getInstance();
};

interface SIMDintrinsicSSDtype :public IEstimationType {
	long long int getValue(const IplImage* leftImage, const IplImage* rightImage, int x, int y, int windowSize, int d) const;
	static SIMDintrinsicSSDtype* getInstance();
};

#endif