#include "Camera.h"

Camera::Camera()
{	
	cameraList = NULL;
	count = 0;
	camera = NULL;
	device;
	stream = NULL;
	evfImage = NULL;

	// EDSDI Init
	err = EdsGetCameraList(&cameraList);
	if (err == EDS_ERR_OK) {
		err = EdsGetChildCount(cameraList, &count);
		if (count != 2) {
			cout << "카메라 연결을 확인하세요" << endl;
			err = EDS_ERR_DEVICE_NOT_FOUND;
			exit(0);
		}
	}


}


Camera::~Camera()
{
	cvReleaseImage(&img);
}

//void Camera::edsdkInit()
//{
//
//}

void Camera::cameraInit(int num)
{
	numOfCamera = num;
	err = EdsGetChildAtIndex(cameraList, numOfCamera, &camera);
	if (err == EDS_ERR_OK) {
		cout << "camera" << numOfCamera + 1 << " is on" << endl;
	}

	if (cameraList != NULL) {
		EdsRelease(cameraList);
		cameraList = NULL;
	}

	//openSession
	if (err == EDS_ERR_OK) {
		err = EdsOpenSession(camera);
	}
}

void Camera::startLive()
{
	err = EdsGetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);
	if (err == EDS_ERR_OK) {
		device |= kEdsEvfOutputDevice_PC;
		err = EdsSetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);
	}

	EdsUInt32 saveTo = kEdsSaveTo_Camera;
	err = EdsSetPropertyData(camera, kEdsPropID_SaveTo, 0, sizeof(saveTo), &saveTo);
}

void Camera::liveView()
{
	err = EdsCreateMemoryStream(0, &stream);
	char *fileName;
	if (left == numOfCamera) {
		fileName = "LeftLiveView.jpg";
	}
	else {
		fileName = "RightLiveView.jpg";
	}
	err = EdsCreateFileStream(fileName, kEdsFileCreateDisposition_CreateAlways, kEdsAccess_ReadWrite, &stream);
	err = EdsCreateEvfImageRef(stream, &evfImage);
	err = EdsDownloadEvfImage(camera, evfImage);
	EdsRelease(stream);
	EdsRelease(evfImage);

	img = cvLoadImage(fileName);
}

void Camera::recordStart()
{
	err = EDS_ERR_OK;
	EdsUInt32 record_start = 4;
	err = EdsSetPropertyData(camera, kEdsPropID_Record, 0, sizeof(record_start), &record_start);
	recording = true;
}

void Camera::recordStop()
{
	err = EDS_ERR_OK;
	EdsUInt32 record_stop = 0;
	err = EdsSetPropertyData(camera, kEdsPropID_Record, 0, sizeof(record_stop), &record_stop);
}