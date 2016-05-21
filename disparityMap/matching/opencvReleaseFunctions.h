#ifndef SEODYUTAMI_DISPARITYMAP_OPENCV_RELEASE_FUNCTIONS
#define SEODYUTAMI_DISPARITYMAP_OPENCV_RELEASE_FUNCTIONS

#include <opencv\cv.h>

void releaseImageForSharedPtr(IplImage* image);

void releaseMatForSharedPtr(CvMat* mat);

#endif