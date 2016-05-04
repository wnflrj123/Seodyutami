#include "estimationType.h"

long long int SADtype::getValue(IplImage* leftImage, IplImage* rightImage, int x, int y, int windowSize, int d) {
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

long long int SSDtype::getValue(IplImage* leftImage, IplImage* rightImage, int x, int y, int windowSize, int d) {
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

long long int NCCtype::getValue(IplImage* leftImage, IplImage* rightImage, int x, int y, int windowSize, int d) {
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

long long int SIMDintrinsicSSDtype::getValue(IplImage* leftImage, IplImage* rightImage, int x, int y, int windowSize, int d) {

	__m128i* leftImagePtr = (__m128i*)(leftImage->imageData + y*leftImage->widthStep + x);
	__m128i* rightImagePtr = (__m128i*)(rightImage->imageData + y*rightImage->widthStep + (x + d));

	__m128i xmmImage = _mm_sad_epu8(_mm_loadu_si128((leftImagePtr)), _mm_loadu_si128((rightImagePtr)));
	return _mm_extract_epi16(xmmImage, 0) + _mm_extract_epi16(xmmImage, 4);
}