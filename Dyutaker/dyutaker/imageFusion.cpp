#include "stdafx.h"
#include "imageFusion.h"
#include <chrono>

Fusion::Fusion()
{
	img1 = NULL;
	img2 = NULL;
	img1Gray = NULL;
	img2Gray = NULL;
}

Fusion::~Fusion()
{
}

void Fusion::setImg(IplImage* image1, IplImage* image2)
{
	if (image1 != NULL && image2 != NULL) {
		//img1 = cvCloneImage(image1);
		//img2 = cvCloneImage(image2);
		img1 = image1;
		img2 = image2;
		img1Gray = cvCreateImage(cvGetSize(img1), IPL_DEPTH_8U, 1);
		img2Gray = cvCreateImage(cvGetSize(img2), IPL_DEPTH_8U, 1);
		cvCvtColor(img1, img1Gray, CV_RGB2GRAY);
		cvCvtColor(img2, img2Gray, CV_RGB2GRAY);
	}
	else {
		cout << "No Input Image!!" << endl;
		exit(0);
	}

	return;
}

void Fusion::sml(IplImage** dst)
{
	if (img1 == NULL) {
		cout << "No Setted Image!!" << endl;
		return;
	}
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	CvSize size = cvGetSize(img1);

	//IplImage* sml1 = cvCloneImage(img1Gray);
	//IplImage* sml2 = cvCloneImage(img2Gray);
	IplImage* sml1 = cvCreateImage(size, IPL_DEPTH_8U, 1);
	IplImage* sml2 = cvCreateImage(size, IPL_DEPTH_8U, 1);
	cvSetZero(sml1);
	cvSetZero(sml2);
	IplImage* imgAvg = cvCreateImage(size, IPL_DEPTH_8U, 3);
	cvSetZero(imgAvg);

	for (int i = 1; i < size.height - 1; i++) {
		uchar* ptrImg1 = (uchar*)(img1Gray->imageData + (i*img1Gray->widthStep));
		uchar* ptrImg1up = (uchar*)(img1Gray->imageData + ((i - 1)*img1Gray->widthStep));
		uchar* ptrImg1down = (uchar*)(img1Gray->imageData + ((i + 1)*img1Gray->widthStep));

		uchar* ptrImg2 = (uchar*)(img2Gray->imageData + (i*img2Gray->widthStep));
		uchar* ptrImg2up = (uchar*)(img2Gray->imageData + ((i - 1)*img2Gray->widthStep));
		uchar* ptrImg2down = (uchar*)(img2Gray->imageData + ((i + 1)*img2Gray->widthStep));

		uchar* ptrsml1 = (uchar*)(sml1->imageData + (i*sml1->widthStep));
		uchar* ptrsml2 = (uchar*)(sml2->imageData + (i*sml2->widthStep));

		uchar* ptrOriImg1 = (uchar*)(img1->imageData + (i*img1->widthStep));
		uchar* ptrOriImg2 = (uchar*)(img2->imageData + (i*img2->widthStep));
		uchar* ptrAvg = (uchar*)(imgAvg->imageData + (i*imgAvg->widthStep));
		for (int j = 1; j < size.width - 1; j++) {
			uchar a1 = abs(2 * ptrImg1[j] - ptrImg1[j - size.width] - ptrImg1[j + size.width]);
			uchar b1 = abs(2 * ptrImg1[j] - ptrImg1[j - 1] - ptrImg1[j + 1]);
			uchar a2 = abs(2 * ptrImg2[j] - ptrImg2[j - size.width] - ptrImg2[j + size.width]);
			uchar b2 = abs(2 * ptrImg2[j] - ptrImg2[j - 1] - ptrImg2[j + 1]);

			ptrsml1[j] = a1 + b1;
			ptrsml2[j] = a2 + b2;
			ptrAvg[3 * j] = (ptrOriImg1[3 * j] + ptrOriImg2[3 * j]) / 2;
			ptrAvg[3 * j + 1] = (ptrOriImg1[3 * j + 1] + ptrOriImg2[3 * j + 1]) / 2;
			ptrAvg[3 * j + 2] = (ptrOriImg1[3 * j + 2] + ptrOriImg2[3 * j + 2]) / 2;
		}
	}

	std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
	//std::cout << "SML MAP 제작 까지 걸린 시간(초) : " << sec.count() << " seconds" << std::endl;


	CvScalar sml1Avg = cvAvg(sml1);
	CvScalar sml2Avg = cvAvg(sml2);
	double dAvg = sml1Avg.val[0] + sml2Avg.val[0];

	IplImage* sml1Thresh = cvCreateImage(size, IPL_DEPTH_8U, 1);
	IplImage* sml2Thresh = cvCreateImage(size, IPL_DEPTH_8U, 1);
	cvSetZero(sml1Thresh);
	cvSetZero(sml2Thresh);
	cvThreshold(sml1, sml1Thresh, dAvg, 255, CV_THRESH_BINARY);
	cvThreshold(sml2, sml2Thresh, dAvg, 255, CV_THRESH_BINARY);

	CvMat* markers = cvCreateMat(size.height, size.width, CV_32SC1);
	cvSetZero(markers);
	int seedNum = 1;
	int seedStep = size.width / 10;
	for (int row = seedStep; row < size.height; row += seedStep) {
		int* ptr = (int*)(markers->data.ptr + row * markers->step);
		for (int col = seedStep; col < size.width; col += seedStep) {
			ptr[col] = seedNum;
			seedNum++;
		}
	}

	// watershed 알고리즘 실행
	cvWatershed(imgAvg, markers);

	sec = std::chrono::system_clock::now() - start;
	//std::cout << "Segmentation 까지 걸린 시간(초) : " << sec.count() << " seconds" << std::endl;

	long long int arrSml1[7000] = { 0, };
	long long int arrSml2[7000] = { 0, };

	//for (int i = 1; i < size.height - 1; i++)
	//{
	//	int* ptrMarkers = (int*)(markers->data.ptr + i * markers->step);
	//	uchar* ptrsml1 = (uchar*)(sml1->imageData + (i*sml1->widthStep));
	//	uchar* ptrsml2 = (uchar*)(sml2->imageData + (i*sml2->widthStep));
	//	for (int j = 1; j < size.width - 1; j++)
	//	{
	//		if (ptrMarkers[j]>0)
	//		{
	//			if (ptrsml1[j] == ptrsml2[j])
	//			{
	//				arrSml1[ptrMarkers[j]]++;
	//				arrSml2[ptrMarkers[j]]++;
	//			}
	//			else if (ptrsml1[j]>ptrsml2[j])
	//			{
	//				arrSml1[ptrMarkers[j]]++;
	//			}
	//			else
	//			{
	//				arrSml2[ptrMarkers[j]]++;
	//			}
	//		}
	//	}
	//}
	for (int i = 1; i < size.height - 1; i++) {
		int* ptrMarkers = (int*)(markers->data.ptr + i * markers->step);
		uchar* ptrsml1 = (uchar*)(sml1Thresh->imageData + (i*sml1Thresh->widthStep));
		uchar* ptrsml2 = (uchar*)(sml2Thresh->imageData + (i*sml2Thresh->widthStep));
		for (int j = 1; j < size.width - 1; j++) {
			if (ptrMarkers[j]>0) {
				if (ptrsml1[j] == ptrsml2[j]) {
					arrSml1[ptrMarkers[j]]++;
					arrSml2[ptrMarkers[j]]++;
				}
				else if (ptrsml1[j]>ptrsml2[j]) {
					arrSml1[ptrMarkers[j]]++;
				}
				else {
					arrSml2[ptrMarkers[j]]++;
				}
			}
		}
	}

	IplImage* fusionImage = cvCloneImage(img1);
	for (int i = 1; i < size.height - 1; i++) {
		int* ptrMarkers = (int*)(markers->data.ptr + i * markers->step);
		uchar* ptrimg2 = (uchar*)(img2->imageData + (i*img2->widthStep));
		uchar* ptrFimg = (uchar*)(fusionImage->imageData + (i*fusionImage->widthStep));
		for (int j = 1; j < size.width - 1; j++) {
			if (ptrMarkers[j] == -1) {
				if (j - 1 >= 0) {
					ptrFimg[3 * j] = ptrFimg[3 * (j - 1)];
					ptrFimg[3 * j + 1] = ptrFimg[3 * (j - 1) + 1];
					ptrFimg[3 * j + 2] = ptrFimg[3 * (j - 1) + 2];
				}
			}
			else {
				if (arrSml1[ptrMarkers[j]] < arrSml2[ptrMarkers[j]]) {
					ptrFimg[3 * j] = ptrimg2[3 * j];
					ptrFimg[3 * j + 1] = ptrimg2[3 * j + 1];
					ptrFimg[3 * j + 2] = ptrimg2[3 * j + 2];
				}
			}
		}
	}

	std::chrono::duration<double> sec2 = std::chrono::system_clock::now() - start;
	//std::cout << "SML Fusion 걸린 시간(초) : " << sec2.count() << " seconds" << std::endl;

	cvReleaseImage(dst);
	*dst = cvCloneImage(fusionImage);

	cvReleaseImage(&sml1);
	cvReleaseImage(&sml2);
	cvReleaseImage(&sml1Thresh);
	cvReleaseImage(&sml2Thresh);
	cvReleaseImage(&imgAvg);
	cvReleaseImage(&fusionImage);
	cvReleaseMat(&markers);
	cvReleaseImage(&img1Gray);
	cvReleaseImage(&img2Gray);
}