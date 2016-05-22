#pragma once
#include "afxcmn.h"


// CProgressDlg 대화 상자입니다.

class CProgressDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProgressDlg)

public:
	CProgressDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CProgressDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PROGRESS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl progressBar;
	Camera* cameraObj;
	bool isThreadRunning;
	int progressValue;
	virtual BOOL OnInitDialog();
	void routine();
	virtual void PostNcDestroy();
	bool isThreadKilled;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

UINT pregressBarThread(LPVOID parameter);