#ifndef MATCHING_H
#define MATCHING_H

#include <iostream>
#include <string>
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <memory>
#include <omp.h>

struct IEstimationType {
	virtual long long int getValue(IplImage* leftImage, IplImage* rightImage, int x, int y, int windowSize, int d) = 0;
};

struct SADtype :public IEstimationType {
	inline long long int getValue(IplImage* leftImage, IplImage* rightImage, int x, int y, int windowSize, int d) {
		long long int SAD = 0;
		for (int i = -windowSize; i < windowSize; i++) {
			uchar* leftImagePtr = (uchar*)(leftImage->imageData + (y + i)*leftImage->widthStep);
			uchar* rightImagePtr = (uchar*)(rightImage->imageData + (y + i)*rightImage->widthStep);

			for (int j = -windowSize; j < windowSize; j++) {
				SAD += (long long int)abs(leftImagePtr[x + j] - rightImagePtr[x + j + d]);
			}
		}

		return SAD;
	}
};

struct SSDtype :public IEstimationType {
	inline long long int getValue(IplImage* leftImage, IplImage* rightImage, int x, int y, int windowSize, int d) {
		long long int SSD = 0;
		for (int i = -windowSize; i < windowSize; i++) {
			uchar* leftImagePtr = (uchar*)(leftImage->imageData + (y + i)*leftImage->widthStep);
			uchar* rightImagePtr = (uchar*)(rightImage->imageData + (y + i)*rightImage->widthStep);

			for (int j = -windowSize; j < windowSize; j++) {
				int temp = (long long int)(leftImagePtr[x + j] - rightImagePtr[x + j + d]);
				SSD += temp * temp;
			}
		}

		return SSD;
	}
};

struct NCCtype :public IEstimationType {
	inline long long int getValue(IplImage* leftImage, IplImage* rightImage, int x, int y, int windowSize, int d) {
		long long int num = 0;
		long long int denA = 0;
		long long int denB = 0;
		float NCC = 0;
		for (int i = -windowSize; i < windowSize; i++) {
			uchar* leftImagePtr = (uchar*)(leftImage->imageData + (y + i)*leftImage->widthStep);
			uchar* rightImagePtr = (uchar*)(rightImage->imageData + (y + i)*rightImage->widthStep);

			for (int j = -windowSize; j < windowSize; j++) {
				num += (long long int)(leftImagePtr[x + j] * rightImagePtr[x + j + d]);
				denA += (long long int)leftImagePtr[x + j] * leftImagePtr[x + j];
				denB += (long long int)rightImagePtr[x + j + d] * rightImagePtr[x + j + d];
			}
		}
		NCC = (float)(num * num) / denA / denB;

		return (long long int)(NCC * 10);
	}
};

class DisparityMapMaker {
public:
	DisparityMapMaker(const std::string leftImagePath, const std::string rightImagePath, const int& windowSize, const int& dRange, IEstimationType* type);
	~DisparityMapMaker();
	IplImage* getDisparitMapPtr();

private:
	const std::shared_ptr<IEstimationType> pEstimationType;
	IplImage* leftImage;
	IplImage* rightImage;
	IplImage* disparityMap;
	int windowSize;
	int dRange;

	long long int getDisparityValue(const int& x, const int& y);
	bool isSameImageSize();
};

#endif