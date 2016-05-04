#ifndef SEODYUTAMI_DISPARITYMAP_MATCHING_ESTIMATIONTYPE_H
#define SEODYUTAMI_DISPARITYMAP_MATCHING_ESTIMATIONTYPE_H
#define interface struct

#include <opencv\cv.h>

//Empty Base Object
class Empty {};

interface IEstimationType : private Empty{
	virtual long long int getValue(IplImage* leftImage, IplImage* rightImage, int x, int y, int windowSize, int d) = 0;
};

interface SADtype :public IEstimationType {
	long long int getValue(IplImage* leftImage, IplImage* rightImage, int x, int y, int windowSize, int d);
};

interface SSDtype :public IEstimationType {
	long long int getValue(IplImage* leftImage, IplImage* rightImage, int x, int y, int windowSize, int d);
};

interface NCCtype :public IEstimationType {
	long long int getValue(IplImage* leftImage, IplImage* rightImage, int x, int y, int windowSize, int d);
};

interface SIMDintrinsicSSDtype :public IEstimationType {
	long long int getValue(IplImage* leftImage, IplImage* rightImage, int x, int y, int windowSize, int d);
};

#endif