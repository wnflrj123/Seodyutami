#pragma once

#include <windows.h>
#include <EDSDK.h>
#include <time.h>
#include <iostream>

using namespace std;

EdsError downloadImage(EdsDirectoryItemRef directoryItem, int num_camera);
EdsError EDSCALLBACK handleObjectEvent(EdsObjectEvent event, EdsBaseRef object, EdsVoid * context);
EdsError EDSCALLBACK handleObjectEvent2(EdsObjectEvent event, EdsBaseRef object, EdsVoid * context);
EdsError EDSCALLBACK handleObjectEventBypass(EdsObjectEvent event, EdsBaseRef object, EdsVoid * context);

class Camera
{
	EdsCameraListRef cameraList;
	EdsUInt32 count;
	EdsUInt32 device;
	EdsStreamRef stream;
	EdsEvfImageRef evfImage;

public:
	EdsError err;
	int numOfCamera;
	bool recording;
	int delayFrame;
	EdsCameraRef leftCamera;
	EdsCameraRef rightCamera;
	EdsCameraRef tempCamera;

public:
	Camera();
	~Camera();
	int edsdkInit();
	int cameraSetup();
	void checkLeft();
	void liveViewSetup();
	void startLive(EdsCameraRef camera);
	void liveViewAll();
	void liveView(EdsCameraRef camera);
	void recordStart();
	void recordStop();
	void downEvent();
	void downEventCancle();
	void finish();
	void edsdkStop();

};