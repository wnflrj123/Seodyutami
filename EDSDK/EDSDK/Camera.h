#include <iostream>
#include <windows.h>
#include <EDSDK.h>
#include <string>
#include <time.h>
#include <opencv\cv.h>
#include <opencv\highgui.h>

using namespace std;

class Camera
{
	EdsCameraListRef cameraList = NULL;
	EdsUInt32 count = 0;
	EdsCameraRef camera = NULL;
	EdsUInt32 device;
	EdsStreamRef stream = NULL;
	EdsEvfImageRef evfImage = NULL;

public:
	EdsError err = EDS_ERR_OK;
	int numOfCamera = 0;
	bool recording = false;
	int left;
	IplImage *img;

public:
	Camera();
	~Camera();
	//void edsdkInit();
	void cameraInit(int num);	//카메라 초기 세팅함수
	void startLive();
	void liveView();
	void recordStart();
	void recordStop();
	void finish();
	void downEvent();
};