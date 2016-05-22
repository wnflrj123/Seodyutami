
// dyutakerDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "afxext.h"
#include "Camera.h"

//static Camera cameraObj[2];

// CdyutakerDlg 대화 상자
class CdyutakerDlg : public CDialogEx
{
// 생성입니다.
public:
	CdyutakerDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DYUTAKER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCameraValidaition();
	afx_msg void OnBnClickedShoot();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedButton1();
	CBitmapButton m_validationButton;
	CBitmapButton m_shootButton;
	CBitmapButton m_stopButton;
	CBitmapButton m_quit_button;
	Camera cameraObj;
	bool bThreadContinue;
	bool bThreadIsKilled;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedExitProgram();
};

UINT getFrameThread(LPVOID pParam);
void wait(DWORD dwMillisecond);