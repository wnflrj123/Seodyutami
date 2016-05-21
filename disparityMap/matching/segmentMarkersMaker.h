#ifndef SEODYUTAMI_DISPARITYMAP_SEGMENT_MARKERS_MAKER
#define SEODYUTAMI_DISPARITYMAP_SEGMENT_MARKERS_MAKER

#include <memory>
#include <opencv\cv.h>

class SegmentMarkersMakerImpl;

class SegmentMarkersMaker {
private:
	std::shared_ptr<SegmentMarkersMakerImpl> pImpl;

public:
	SegmentMarkersMaker(int seedStep);
	std::shared_ptr<CvMat> getSegmentMarkers(const IplImage* colorImage);
	std::shared_ptr<IplImage> getSegmentedDisparityMap(const IplImage* disparityMap, const CvMat* markers);
};


#endif