#include <iostream>
#include "imageFusion.h"
#include "changeViewPoint.h"

using namespace std;

int main()
{
	Fusion fusion;
	CVP cvp;
	IplImage* disparity = cvLoadImage("image/test_disparity.jpg");
	IplImage* rightImage = cvLoadImage("image/test_R.jpg");
	IplImage* leftImage = cvLoadImage("image/test_L.jpg");
	IplImage* convertImage = NULL;
	IplImage* multiFocusImage=NULL;

	//cvp class --> 시차 문제 해결
	cvp.setimg(disparity, leftImage, rightImage);	//Image 셋팅(디스패리티, 왼쪽, 오른쪽)
	cvp.runcvt(&convertImage);						//시차문제 해결하여 dst이미지에 저장

	//fusion class --> Multi Focus Image Fusion
	fusion.setimg(rightImage, convertImage);		//Image 셋팅(각각의 다른 초점을 가진 두개의 이미지)
	fusion.sml(&multiFocusImage);					//Multi Focus Image Fusion을 수행한 결과를  dst이미지에 저장

	cvShowImage("MultiFocusImage", multiFocusImage);
	cvShowImage("convertImage", convertImage);
	cvShowImage("disparity", disparity);
	cvShowImage("leftImage", leftImage);
	cvShowImage("rightImage", rightImage);
	cvWaitKey(0);
	cvDestroyAllWindows();
	cvReleaseImage(&disparity);
	cvReleaseImage(&rightImage);
	cvReleaseImage(&leftImage);
	cvReleaseImage(&convertImage);
	cvReleaseImage(&multiFocusImage);


	return 0;
}