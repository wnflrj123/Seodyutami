#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
{
	cameraList = NULL;
	count = 0;
	device = NULL;
	stream = NULL;
	evfImage = NULL;

	err = EDS_ERR_OK;
	numOfCamera = 0;
	recording = false;
	delayFrame = 0;

	tempCamera = NULL;
	leftCamera = NULL;
	rightCamera = NULL;
}

Camera::~Camera()
{
	EdsTerminateSDK();
}

int Camera::edsdkInit()
{
	EdsTerminateSDK();
	err = EdsInitializeSDK();
	if (err != EDS_ERR_OK)
	{
		//exit(0);
		return 1;
	}
	return 0;
}

int Camera::cameraSetup()
{
	err = EdsGetCameraList(&cameraList);
	if (err == EDS_ERR_OK) {
		err = EdsGetChildCount(cameraList, &count);
		if (count != 2) {
			err = EDS_ERR_DEVICE_NOT_FOUND;
			//exit(0);
			return 1;
		}
	}

	err = EdsGetChildAtIndex(cameraList, 0, &leftCamera);
	err = EdsGetChildAtIndex(cameraList, 1, &rightCamera);

	if (leftCamera != NULL&&rightCamera != NULL)
	{
		cout << "2 cameras are on" << endl;
	}

	if (cameraList != NULL) {
		EdsRelease(cameraList);
		cameraList = NULL;
	}
	return 0;
}

void Camera::checkLeft()
{
	//left camera 확인을 위해 모든 카메라의 세션을 닫는다.
	err = EdsOpenSession(leftCamera);
	err = EdsCloseSession(leftCamera);
	err = EdsOpenSession(rightCamera);
	err = EdsCloseSession(rightCamera);
	startLive(leftCamera);
	if (AfxMessageBox(_T("Is this camera Located in Left?"), MB_YESNO) == IDYES) {
	}
	else
	{
		//여기에 문제가 있는 것 같음!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//여기에 문제가 있는 것 같음!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//여기에 문제가 있는 것 같음!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//여기에 문제가 있는 것 같음!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//여기에 문제가 있는 것 같음!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//여기에 문제가 있는 것 같음!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//여기에 문제가 있는 것 같음!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//여기에 문제가 있는 것 같음!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//여기에 문제가 있는 것 같음!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//여기에 문제가 있는 것 같음!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//여기에 문제가 있는 것 같음!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//여기에 문제가 있는 것 같음!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		tempCamera = leftCamera;
		leftCamera = rightCamera;
		rightCamera = tempCamera;
		tempCamera = NULL;
		//여기에 문제가 있는 것 같음!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//여기에 문제가 있는 것 같음!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//여기에 문제가 있는 것 같음!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//여기에 문제가 있는 것 같음!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//여기에 문제가 있는 것 같음!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//여기에 문제가 있는 것 같음!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//여기에 문제가 있는 것 같음!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//여기에 문제가 있는 것 같음!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//여기에 문제가 있는 것 같음!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//여기에 문제가 있는 것 같음!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//여기에 문제가 있는 것 같음!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//여기에 문제가 있는 것 같음!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}
}

void Camera::startLive(EdsCameraRef camera)
{
	//openSession, 만약 session이 열려있다며 닫았다가 다시 연다
	err = EdsOpenSession(camera);
	err = EdsCloseSession(camera);
	err = EdsOpenSession(camera);

	err = EdsGetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);
	if (err == EDS_ERR_OK) {
		device |= kEdsEvfOutputDevice_PC;
		err = EdsSetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);
	}

	EdsUInt32 saveTo = kEdsSaveTo_Camera;
	err = EdsSetPropertyData(camera, kEdsPropID_SaveTo, 0, sizeof(saveTo), &saveTo);
}

void Camera::liveViewSetup()
{
	startLive(leftCamera);
	startLive(rightCamera);
}

void Camera::liveViewAll()
{
	liveView(leftCamera);
	liveView(rightCamera);
}

void Camera::liveView(EdsCameraRef camera)
{
	err = EdsCreateMemoryStream(0, &stream);
	char *fileName;
	if (leftCamera == camera) {
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
}

void Camera::recordStart()
{
	DWORD startT;
	DWORD endT;

	err = EDS_ERR_OK;
	EdsUInt32 record_start = 4;
	err = EdsSetPropertyData(leftCamera, kEdsPropID_Record, 0, sizeof(record_start), &record_start);
	startT = GetTickCount();
	err = EDS_ERR_OK;
	record_start = 4;
	err = EdsSetPropertyData(rightCamera, kEdsPropID_Record, 0, sizeof(record_start), &record_start);
	endT = GetTickCount();
	delayFrame = (endT - startT) / 24;
	cout << "Delay : " << endT - startT << "(" << delayFrame << " frame)" << endl;
	recording = true;
}

void Camera::recordStop()
{
	err = EDS_ERR_OK;
	EdsUInt32 record_stop = 0;
	err = EdsSetPropertyData(rightCamera, kEdsPropID_Record, 0, sizeof(record_stop), &record_stop);
	err = EDS_ERR_OK;
	record_stop = 0;
	err = EdsSetPropertyData(leftCamera, kEdsPropID_Record, 0, sizeof(record_stop), &record_stop);
	err = EdsCloseSession(rightCamera);
	err = EdsCloseSession(leftCamera);
}

void Camera::downEvent()
{
	err = EDS_ERR_OK;
	err = EdsSetObjectEventHandler(leftCamera, kEdsObjectEvent_All, handleObjectEvent, NULL);
	err = EdsSetObjectEventHandler(rightCamera, kEdsObjectEvent_All, handleObjectEvent2, NULL);

	err = EdsOpenSession(leftCamera);
	err = EdsOpenSession(rightCamera);

}

void Camera::downEventCancle()
{
	err = EDS_ERR_OK;
	err = EdsCloseSession(leftCamera);
	err = EdsCloseSession(rightCamera);
	err = EdsSetObjectEventHandler(leftCamera, kEdsObjectEvent_All, handleObjectEventBypass, NULL);
	err = EdsSetObjectEventHandler(rightCamera, kEdsObjectEvent_All, handleObjectEventBypass, NULL);

	err = EdsOpenSession(leftCamera);
	err = EdsOpenSession(rightCamera);

}

void Camera::finish()
{
	err = EDS_ERR_OK;
	err = EdsCloseSession(leftCamera);
	err = EdsCloseSession(rightCamera);
	if (leftCamera != NULL) {
		EdsRelease(leftCamera);
	}
	if (rightCamera != NULL) {
		EdsRelease(rightCamera);
	}
}

void Camera::edsdkStop()
{
	EdsTerminateSDK();
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
			str_path = "LEFT.MOV";
		}
		else {
			str_path = "RIGHT.MOV";
		}

		const char* ch_dest = str_path.c_str();
		err = EdsCreateFileStream(ch_dest,
			kEdsFileCreateDisposition_CreateAlways,
			kEdsAccess_ReadWrite, &stream);

	}
	// Download image
	CString out;
	if (err == EDS_ERR_OK) {
		cout << dirItemInfo.szFileName << " downaload Start!" << endl;
		err = EdsDownload(directoryItem, dirItemInfo.size, stream);
	}
	// Issue notification that download is complete
	if (err == EDS_ERR_OK) {
		cout << "download complete" << endl;
		err = EdsDownloadComplete(directoryItem);
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
	cout << "Handler1 실행" << endl;

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
	cout << "Handler2 실행" << endl;

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

EdsError EDSCALLBACK handleObjectEventBypass(EdsObjectEvent event, EdsBaseRef object, EdsVoid * context)
{
	EdsError err = EDS_ERR_OK;
	cout << "우회 Handler 실행" << endl;

	//Release object
	if (object) {
		EdsRelease(object);
	}
	return err;
}