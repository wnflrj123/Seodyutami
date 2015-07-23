#include <iostream>
#include <windows.h>
#include <EDSDK.h>
#include <string>
#include <time.h>
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include "Camera.h"

void runLive(Camera camera1, Camera camera2);
void recordStartAll(Camera camera1, Camera camera2);
EdsError downloadImage(EdsDirectoryItemRef directoryItem, int num_camera);
EdsError EDSCALLBACK handleObjectEvent(EdsObjectEvent event, EdsBaseRef object, EdsVoid * context);
EdsError EDSCALLBACK handleObjectEvent2(EdsObjectEvent event, EdsBaseRef object, EdsVoid * context);
string getNowTime();
string nowTime;

int main()
{
	EdsError err = EDS_ERR_OK;
	err = EdsInitializeSDK();
	if (err != EDS_ERR_OK) {
		return 0;
	}
	Camera cameraObj[2];

	//cameraObj[0].edsdkInit();
	//cameraObj[1].edsdkInit();
	if (cameraObj[0].err == EDS_ERR_DEVICE_NOT_FOUND) {
		return 0;
	}

	cameraObj[0].cameraInit(0);
	cameraObj[1].cameraInit(1);

	//cameraObj[0].CheckLeft();
	cameraObj[0].startLive();

	cout << "Is this camera is LeftCamera? 1:yes, 2:no" << endl;
	int answer;
	cin >> answer;
	if (answer == 1) {
		cameraObj[0].left = cameraObj[0].numOfCamera;
		cameraObj[1].left = cameraObj[0].numOfCamera;
	}
	else {
		cameraObj[0].left = cameraObj[1].numOfCamera;
		cameraObj[1].left = cameraObj[1].numOfCamera;
	}
	cameraObj[1].startLive();
	runLive(cameraObj[0], cameraObj[1]);


	return 0;	//여기서 런타임오류가 발생하네;;
}




void runLive(Camera camera1, Camera camera2)
{
	char c;

	while (1) {
		camera1.liveView();
		camera2.liveView();

		if (camera1.left == camera1.numOfCamera) {
			cvShowImage("LeftLive", camera1.img);
			cvShowImage("RightLive", camera2.img);
		}
		else {
			cvShowImage("LeftLive", camera2.img);
			cvShowImage("RightLive", camera1.img);
		}

		cvReleaseImage(&camera1.img);
		cvReleaseImage(&camera2.img);


		c = cvWaitKey(10);
		if (c == 27) {
			camera1.finish();
			camera2.finish();
			break;
		}
		else if (c == 49)	//창에 1입력
		{
			if (camera1.recording == false) {
				cvDestroyWindow("LeftLive");
				cvDestroyWindow("RightLive");
				recordStartAll(camera1, camera2);
				break;
			}
		}
		else if (c == 50)	//창에 2입력
		{
			if (camera1.recording == true) {
				camera1.recordStop();
				camera2.recordStop();
				cvDestroyWindow("LeftLive");
				cvDestroyWindow("RightLive");
				nowTime = getNowTime();
				camera1.finish();
				camera2.finish();
				break;
			}
			else {
				camera1.finish();
				camera2.finish();
				break;
			}
		}
		else if (c == 48)	//창에 0입력
		{
			camera1.finish();
			camera2.finish();
			//finish(camera1, camera2);
			break;
		}
	}

}

void Camera::finish()
{
	err = EDS_ERR_OK;
	err = EdsCloseSession(camera);
	if (numOfCamera == left) {
		err = EdsSetObjectEventHandler(camera, kEdsObjectEvent_All, handleObjectEvent, NULL);
	}
	else {
		err = EdsSetObjectEventHandler(camera, kEdsObjectEvent_All, handleObjectEvent2, NULL);
	}
	err = EdsOpenSession(camera);
	err = EdsCloseSession(camera);

	if (camera != NULL) {
		EdsRelease(camera);
	}

}

void Camera::downEvent()
{
	err = EDS_ERR_OK;
	if (numOfCamera == left) {
		err = EdsSetObjectEventHandler(camera, kEdsObjectEvent_All, handleObjectEvent, NULL);
	}
	else {
		err = EdsSetObjectEventHandler(camera, kEdsObjectEvent_All, handleObjectEvent2, NULL);
	}
}

EdsError downloadImage(EdsDirectoryItemRef directoryItem, int num_camera)
{
	cout << "Download 함수 실행" << endl;
	EdsError err = EDS_ERR_OK;
	EdsStreamRef stream = NULL;
	// Get directory item information
	EdsDirectoryItemInfo dirItemInfo;
	err = EdsGetDirectoryItemInfo(directoryItem, &dirItemInfo);
	// Create file stream for transfer destination
	if (err == EDS_ERR_OK) {
		string str_path;
		if (num_camera == 1) {
			str_path = "[LEFT]" + nowTime + ".MOV";
		}
		else {
			str_path = "[RIGHT]" + nowTime + ".MOV";
		}

		const char* ch_dest = str_path.c_str();
		err = EdsCreateFileStream(ch_dest,
			kEdsFileCreateDisposition_CreateAlways,
			kEdsAccess_ReadWrite, &stream);

	}
	// Download image
	if (err == EDS_ERR_OK) {
		cout << dirItemInfo.szFileName << " downaload Start!" << endl;
		err = EdsDownload(directoryItem, dirItemInfo.size, stream);
	}
	// Issue notification that download is complete
	if (err == EDS_ERR_OK) {
		err = EdsDownloadComplete(directoryItem);
		cout << "download complete" << endl;
	}
	// Release stream
	if (stream != NULL) {
		EdsRelease(stream);
		stream = NULL;
	}
	return err;
}

EdsError EDSCALLBACK handleObjectEvent(EdsObjectEvent event, EdsBaseRef object, EdsVoid * context)
{

	EdsDirectoryItemInfo objectInfo;
	EdsError err = EDS_ERR_OK;
	cout << "Handler 실행" << endl;

	switch (event) {
	case kEdsObjectEvent_DirItemCreated:
		cout << "Created" << endl;
		err = EdsGetDirectoryItemInfo(object, &objectInfo);
		downloadImage(object, 1);
		break;
	case  kEdsObjectEvent_DirItemRequestTransfer:
		cout << "RequestTransfer" << endl;
		err = EdsGetDirectoryItemInfo(object, &objectInfo);
		downloadImage(object, 1);
		break;
	default:
		break;
	}
	//Release object
	if (object) {
		EdsRelease(object);
	}
	return err;
}

EdsError EDSCALLBACK handleObjectEvent2(EdsObjectEvent event, EdsBaseRef object, EdsVoid * context)
{

	EdsDirectoryItemInfo objectInfo;
	EdsError err = EDS_ERR_OK;
	cout << "Handler 실행" << endl;

	switch (event) {
	case kEdsObjectEvent_DirItemCreated:
		cout << "Created" << endl;
		err = EdsGetDirectoryItemInfo(object, &objectInfo);
		downloadImage(object, 2);
		break;
	case  kEdsObjectEvent_DirItemRequestTransfer:
		cout << "RequestTransfer" << endl;
		err = EdsGetDirectoryItemInfo(object, &objectInfo);
		downloadImage(object, 2);
		break;
	default:
		break;
	}
	//Release object
	if (object) {
		EdsRelease(object);
	}
	return err;
}

void recordStartAll(Camera camera1, Camera camera2)
{
	DWORD startT;
	DWORD endT;

	camera1.recordStart();
	startT = GetTickCount();
	camera2.recordStart();
	endT = GetTickCount();
	cout << "Delay : " << endT - startT << "(" << (endT - startT) / 30 << " frame)" << endl;
	runLive(camera1, camera2);	//재귀적으로 되어있네ㅠㅠㅠㅠ
}

string getNowTime()
{
	time_t now;
	time(&now);
	char buff[20];
	strftime(buff, 20, "%Y%m%d%H%M%S", localtime(&now));
	return (string)buff;
}