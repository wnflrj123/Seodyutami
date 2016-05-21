#include "segmentMarkersMakerImpl.h"
#include "opencvReleaseFunctions.h"

void SegmentMarkersMakerImpl::prepareMarkers(CvMat* markers) {
	CvSize size = cvGetSize(markers);
	cvSetZero(markers);
	int seedNum = 1;
	for (int i = seedStep; i < size.height; i += seedStep) {
		int* ptr = (int*)(markers->data.ptr + i * markers->step);
		for (int j = seedStep; j < size.width; j += seedStep) {
			ptr[j] = seedNum;
			seedNum++;
		}
	}
}

void SegmentMarkersMakerImpl::getSegmentAverageTable(const IplImage* grayImage, const CvMat* markers, std::vector<std::vector<int> >& averageTable) {
	CvSize size = cvGetSize(grayImage);

	int height = size.height;
	int width = size.width;
	for (int i = 1; i < height; i++) {
		uchar* pDispairty = (uchar*)(grayImage->imageData + i*grayImage->widthStep);
		int* pMarkers = (int*)(markers->data.ptr + i * markers->step);
		for (int j = 0; j < width; j++) {
			if (pMarkers[j] != -1) {
				averageTable[0][pMarkers[j]]++;
				averageTable[1][pMarkers[j]] += pDispairty[j];
			}
		}
	}

	for (size_t i = 0; i < averageTable[0].size(); i++) {
		if (averageTable[0][i] != 0) {
			averageTable[1][i] /= averageTable[0][i];
		}
	}
}

SegmentMarkersMakerImpl::SegmentMarkersMakerImpl(int seedStep) :seedStep(seedStep) {}

std::shared_ptr<CvMat> SegmentMarkersMakerImpl::getSegmentMarkers(const IplImage* colorImage) {
	CvSize size = cvGetSize(colorImage);
	CvMat* markers = cvCreateMat(size.height, size.width, CV_32SC1);
	prepareMarkers(markers);
	cvWatershed(colorImage, markers);

	return std::shared_ptr<CvMat>(markers, releaseMatForSharedPtr);
}

std::shared_ptr<IplImage> SegmentMarkersMakerImpl::getSegmentedDisparityMap(const IplImage* disparityMap, const CvMat* markers) {
	CvSize size = cvGetSize(disparityMap);
	IplImage* segmentedDisparityMap = cvCreateImage(size, IPL_DEPTH_8U, 1);

	// TODO 이미지의 크기가 변하지 않는데 markerTable을 매번 만들어야 하나?
	std::vector<std::vector<int> > markerTable(2, std::vector<int>(size.height * size.width / (seedStep * seedStep), 0));
	getSegmentAverageTable(disparityMap, markers, markerTable);

	int height = size.height;
	int width = size.width;
	for (int i = 0; i < height; i++) {
		uchar* pDispairty = (uchar*)(segmentedDisparityMap->imageData + i*segmentedDisparityMap->widthStep);
		int* pMarkers = (int*)(markers->data.ptr + i * markers->step);
		for (int j = 0; j < width; j++) {
			if (pMarkers[j] == -1) {
				pDispairty[j] = pDispairty[j - 1];
			}
			else {
				pDispairty[j] = markerTable[1][pMarkers[j]];
			}
		}
	}

	return std::shared_ptr<IplImage>(segmentedDisparityMap, releaseImageForSharedPtr);
}