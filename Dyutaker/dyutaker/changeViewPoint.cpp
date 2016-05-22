#include "stdafx.h"
#include "changeViewPoint.h"
#include <chrono>


CVP::CVP()
{
	disparity = NULL;
	imgR = NULL;
	imgL = NULL;
}

CVP::~CVP()
{
}

void CVP::setImg(IplImage* dis, IplImage* left, IplImage* right)
{
	if (dis != NULL&&left != NULL&&right != NULL) {
		disparity = dis;
		imgR = right;
		imgL = left;
	}
	else {
		cout << "No input Image!!!" << endl;
	}

	return;
}


void CVP::runcvt(IplImage** dst)
{
	CvSize size = cvGetSize(imgR);
	IplImage* cvtR2L = cvCloneImage(imgL);

	for (int i = 0; i < size.height; i++) {
		uchar* pDispG = (uchar*)(disparity->imageData + i*disparity->widthStep);
		uchar* prtCvtR2L = (uchar*)(cvtR2L->imageData + i*cvtR2L->widthStep);
		uchar* prtCopyImgR = (uchar*)(imgR->imageData + i*imgR->widthStep);
		for (int j = 0; j <size.width; j++) {
			int tempDisp = pDispG[j];
			int tempVal = j;
			tempVal += tempDisp / PixelDiff;
			if (tempVal <size.width && prtCopyImgR[3 * tempVal] != 0) {
				prtCvtR2L[3 * j] = prtCopyImgR[3 * tempVal];
				prtCvtR2L[3 * j + 1] = prtCopyImgR[3 * tempVal + 1];
				prtCvtR2L[3 * j + 2] = prtCopyImgR[3 * tempVal + 2];

				prtCopyImgR[3 * tempVal] = 0;
			}
		}
	}

	cvReleaseImage(dst);
	*dst = cvCloneImage(cvtR2L);

	cvReleaseImage(&cvtR2L);
	return;
}

//void CVP::runcvt(IplImage** dst)
//{
//	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
//	CvSize size = cvGetSize(imgR);
//	IplImage* cvtR2L = cvCloneImage(imgL);
//
//	//	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
//	//#pragma omp parallel for schedule(dynamic)
//	for (int i = 0; i < size.height; i++) {
//		uchar* pDispG = (uchar*)(disparity->imageData + i*disparity->widthStep);
//		uchar* prtCvtR2L = (uchar*)(cvtR2L->imageData + i*cvtR2L->widthStep);
//		uchar* prtCopyImgR = (uchar*)(imgR->imageData + i*imgR->widthStep);
//		//for (int j = size.width - 1; j >= 0; j--) {
//		for (int j = 0; j <size.width; j++) {
//			int tempDisp = pDispG[j];
//			int tempVal = j;
//			//tempVal -= tempDisp / PixelDiff;
//			tempVal += tempDisp / PixelDiff;
//			//if (tempVal >= 0 && prtCopyImgR[3 * tempVal] != 0) {
//			if (tempVal <size.width && prtCopyImgR[3 * tempVal] != 0) {
//				prtCvtR2L[3 * j] = prtCopyImgR[3 * tempVal];
//				prtCvtR2L[3 * j + 1] = prtCopyImgR[3 * tempVal + 1];
//				prtCvtR2L[3 * j + 2] = prtCopyImgR[3 * tempVal + 2];
//
//				prtCopyImgR[3 * tempVal] = 0;
//				//prtCopyImgR[3 * tempVal + 1] = 0;
//				//prtCopyImgR[3 * tempVal + 2] = 0;
//			}
//		}
//	}
//
//	std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
//	//std::cout << "시차변환 걸린 시간(초) : " << sec.count() << " seconds" << std::endl;
//
//	*dst = cvCloneImage(cvtR2L);
//
//	cvReleaseImage(&cvtR2L);
//	return;
//
//}