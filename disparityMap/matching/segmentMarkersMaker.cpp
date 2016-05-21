#include "segmentMarkersMaker.h"
#include "segmentMarkersMakerImpl.h"
#include "opencvReleaseFunctions.h"

SegmentMarkersMaker::SegmentMarkersMaker(int seedStep) :pImpl(new SegmentMarkersMakerImpl(seedStep)) {}

std::shared_ptr<CvMat> SegmentMarkersMaker::getSegmentMarkers(const IplImage* colorImage) {
	return pImpl->getSegmentMarkers(colorImage);
}

std::shared_ptr<IplImage> SegmentMarkersMaker::getSegmentedDisparityMap(const IplImage* disparityMap, const CvMat* markers) {
	return pImpl->getSegmentedDisparityMap(disparityMap, markers);
}