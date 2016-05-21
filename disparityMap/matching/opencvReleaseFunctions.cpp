#include "opencvReleaseFunctions.h"

void releaseImageForSharedPtr(IplImage* image) {
	cvReleaseImage(&image);
}

void releaseMatForSharedPtr(CvMat* mat) {
	cvReleaseMat(&mat);
}