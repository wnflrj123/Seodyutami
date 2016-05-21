#ifndef SEODYUTAMI_DISPARITYMAP_SEGMENT_MARKERS_MAKER_IMPL
#define SEODYUTAMI_DISPARITYMAP_SEGMENT_MARKERS_MAKER_IMPL

#include <memory>
#include <vector>
#include <opencv\cv.h>

class SegmentMarkersMakerImpl {
private:
	int seedStep;

	void prepareMarkers(CvMat* markers);
	void getSegmentAverageTable(const IplImage* grayImage, const CvMat* markers, std::vector<std::vector<int> >& averageTable);

public:
	SegmentMarkersMakerImpl(int seedStep);
	std::shared_ptr<CvMat> getSegmentMarkers(const IplImage* colorImage);
	std::shared_ptr<IplImage> getSegmentedDisparityMap(const IplImage* disparityMap, const CvMat* markers);
};


#endif