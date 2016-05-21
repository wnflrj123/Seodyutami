#include <iostream>
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include "disparityMapMaker.h"
#include "timePrinter.h"
#include "segmentMarkersMaker.h"

void showImageAndStop(IplImage* image) {
	cvShowImage("test", image);
	cvWaitKey(0);
	cvDestroyWindow("test");
}

int main() {
	IplImage* leftGrayImage = cvLoadImage("tsukuba_l.png", 0);
	IplImage* leftColorImage = cvLoadImage("tsukuba_l.png", 1);
	IplImage* rightGrayImage = cvLoadImage("tsukuba_r.png", 0);

	DisparityMapMaker disparityMapMaker(8, 16, SIMDintrinsicSSDtype::getInstance());
	SegmentMarkersMaker segmentMarkersMaker(10);

	TimePrinter time;
	std::shared_ptr<IplImage> disparityMap(disparityMapMaker.getDisparityMapPtr(leftGrayImage, rightGrayImage));
	std::shared_ptr<CvMat> segmentMarkers(segmentMarkersMaker.getSegmentMarkers(leftColorImage));
	std::shared_ptr<IplImage> segmentedDisparityMap(segmentMarkersMaker.getSegmentedDisparityMap(disparityMap.get(), segmentMarkers.get()));
	time.end();

	cvSaveImage("matching.png", disparityMap.get());
	showImageAndStop(segmentedDisparityMap.get());
	
	return 0;
}