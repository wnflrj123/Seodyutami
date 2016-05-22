
// dyutakerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "dyutaker.h"
#include "dyutakerDlg.h"
#include "afxdialogex.h"
#include "dp_cam.h"
#include "ProgressDlg.h"
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//파일분리하고싶다!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//파일분리하고싶다!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//파일분리하고싶다!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//파일분리하고싶다!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//파일분리하고싶다!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//파일분리하고싶다!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//파일분리하고싶다!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
class THREADPARAM
{
	Camera* camera;
	bool* bThreadContitnue;
	bool* bThreadIsKilled;
public:
	THREADPARAM();
	Camera* getCameraHandle();
	bool* getThreadContinueFlag();
	bool* getThreadKilledFlag();
};
THREADPARAM::THREADPARAM()
{
	CdyutakerApp* app = (CdyutakerApp*)AfxGetApp();
	CdyutakerDlg* pDlg = (CdyutakerDlg*)(app->m_pMainWnd);

	camera = &(pDlg->cameraObj);

	bThreadContitnue = &(pDlg->bThreadContinue);
	bThreadIsKilled = &(pDlg->bThreadIsKilled);
}
Camera* THREADPARAM::getCameraHandle()
{
	return this->camera;
}
bool* THREADPARAM::getThreadContinueFlag()
{
	return this->bThreadContitnue;
}
bool* THREADPARAM::getThreadKilledFlag()
{
	return this->bThreadIsKilled;
}
//파일분리하고싶다!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//파일분리하고싶다!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//파일분리하고싶다!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//파일분리하고싶다!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//파일분리하고싶다!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//파일분리하고싶다!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//파일분리하고싶다!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!





// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CdyutakerDlg 대화 상자



CdyutakerDlg::CdyutakerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdyutakerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CdyutakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAMERA_VALIDAITION, m_validationButton);
	DDX_Control(pDX, IDC_SHOOT, m_shootButton);
	DDX_Control(pDX, IDC_STOP, m_stopButton);
}

BEGIN_MESSAGE_MAP(CdyutakerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CAMERA_VALIDAITION, &CdyutakerDlg::OnBnClickedCameraValidaition)
	ON_BN_CLICKED(IDC_SHOOT, &CdyutakerDlg::OnBnClickedShoot)
	ON_BN_CLICKED(IDC_STOP, &CdyutakerDlg::OnBnClickedStop)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_QUIT_PROGRAM, &CdyutakerDlg::OnBnClickedExitProgram)
//	ON_WM_CTLCOLOR()
//ON_WM_DRAWITEM()
//ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CdyutakerDlg 메시지 처리기

BOOL CdyutakerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_quit_button.Attach(((CBitmapButton *)GetDlgItem(IDC_QUIT_PROGRAM))->m_hWnd);
	m_quit_button.LoadBitmaps(IDB_QUIT, IDB_QUIT, NULL, NULL);
	m_quit_button.SizeToContent();

	m_validationButton.Attach(((CBitmapButton *)GetDlgItem(IDC_CAMERA_VALIDAITION))->m_hWnd);
	m_validationButton.LoadBitmaps(IDB_CONNECT_VISIBLE, IDB_CONNECT_CLICK, NULL, IDB_CONNECT_DISABLED);
	m_validationButton.SizeToContent();

	m_shootButton.Attach(((CBitmapButton *)GetDlgItem(IDC_SHOOT))->m_hWnd);
	m_shootButton.LoadBitmaps(IDB_SHOOT_VISIBLE, IDB_SHOOT_CLICK, NULL, IDB_SHOOT_DISABLED);
	m_shootButton.SizeToContent();

	m_stopButton.Attach(((CBitmapButton *)GetDlgItem(IDC_STOP))->m_hWnd);
	m_stopButton.LoadBitmaps(IDB_STOP_VISIBLE, IDB_STOP_CLICK, NULL, IDB_STOP_DISABLED);
	m_stopButton.SizeToContent();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CdyutakerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CdyutakerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CdyutakerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CdyutakerDlg::OnBnClickedCameraValidaition()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_validationButton.EnableWindow(FALSE);

	if (cameraObj.edsdkInit()) {
		AfxMessageBox(_T("EDSDK가 제대로 시작하지 못했습니다."));
	}
	else {
		if (cameraObj.cameraSetup()) {
			AfxMessageBox(_T("카메라 연결을 확인하세요"));
		}
		else {
			cameraObj.checkLeft();
			wait(1000);
			cameraObj.liveViewSetup();
			// 버튼 활성화
			m_shootButton.EnableWindow(TRUE);
		}
	}

	m_validationButton.EnableWindow(TRUE);
}


void CdyutakerDlg::OnBnClickedShoot()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// shoot 버튼과 활성화버튼을 비활성화 하고 stop버튼을 활성화합니다.
	m_validationButton.EnableWindow(FALSE);
	m_shootButton.EnableWindow(FALSE);
	bThreadContinue = TRUE;
	bThreadIsKilled = FALSE;
	// 영상 촬영을 시작합니다.
	cameraObj.recordStart();
	// 영상 촬영 스레드 생성 
	THREADPARAM* pParams = new THREADPARAM;
	AfxBeginThread(getFrameThread, pParams);

	m_stopButton.EnableWindow(TRUE);
}


void CdyutakerDlg::OnBnClickedStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 영상 촬영 스레드 종료합니다.
	m_stopButton.EnableWindow(FALSE);
	bThreadContinue = FALSE;

	while (bThreadIsKilled == FALSE){
		wait(100);
	}
	cameraObj.recordStop();

	// 영상을 저장할 지 확인합니다.
 	if (AfxMessageBox(_T("Do you want to save video?"), MB_YESNO) == IDYES) {
		CProgressDlg* progressDlg = new CProgressDlg;
		progressDlg->Create(CProgressDlg::IDD);
		progressDlg->ShowWindow(SW_SHOW);
		progressDlg->routine();

		progressDlg->DestroyWindow();
		delete progressDlg;
		
		cameraObj.liveViewSetup();
		m_shootButton.EnableWindow(TRUE);
		m_validationButton.EnableWindow(TRUE);
	}
	else {
		wait(500);
		cameraObj.downEventCancle();
		cameraObj.liveViewSetup();
		m_shootButton.EnableWindow(TRUE);
		m_validationButton.EnableWindow(TRUE);
	}
}

UINT getFrameThread(LPVOID pParam)
{
	THREADPARAM* pParams = (THREADPARAM*)pParam;
	Camera* camera = pParams->getCameraHandle();

	bool* bThreadContinue = pParams->getThreadContinueFlag();
	bool* bThreadIsKilled = pParams->getThreadKilledFlag();
	delete pParams;

	while (*bThreadContinue) {
		(*camera).liveViewAll();

		std::cout << "Thread is running!" << endl;
	}

	(*bThreadIsKilled) = TRUE;
	std::cout << "Thread is stopped." << endl;
	return 0;
}


void wait(DWORD dwMillisecond)
{
	MSG msg;
	DWORD dwStart;
	dwStart = GetTickCount();

	while (GetTickCount() - dwStart < dwMillisecond)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

BOOL CdyutakerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// 다이얼로그 배경 색 바꾸기
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(255, 255, 255));

	return TRUE;
}


void CdyutakerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// 다이얼로그 영역을 선택하여 창 움직이기
	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);

	CDialog::OnLButtonDown(nFlags, point);
}


BOOL CdyutakerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	// ESC 종료 막기
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}


void CdyutakerDlg::OnBnClickedExitProgram()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnDestroy(); 
	OnOK();
}