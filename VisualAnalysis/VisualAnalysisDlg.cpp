
// VisualAnalysisDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VisualAnalysis.h"
#include "VisualAnalysisDlg.h"
#include "afxdialogex.h"
//#include "opencv.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//using namespace cv;	// opencv2.x usage

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
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


// CVisualAnalysisDlg 对话框




CVisualAnalysisDlg::CVisualAnalysisDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVisualAnalysisDlg::IDD, pParent)
	, m_pImg(NULL)
	, m_imgDir(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// Initialize the class variables
	m_pImg = NULL;
	m_filePath = _T("文件路径");
	m_imgDir = _T("E:\\Projects\\Dataset\\PKU-SVD-B_V2.0\\1_1_05(06)_0\\prob\\dongnanmeneast_15_1920x1080_30_R1\\");
}

void CVisualAnalysisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_FILEPATH, m_filePath);
	DDX_Control(pDX, IDC_PROGRESS_IMG, m_ctrlProcess);
}

BEGIN_MESSAGE_MAP(CVisualAnalysisDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPENFILE, &CVisualAnalysisDlg::OnBnClickedBtnOpenfile)
	ON_BN_CLICKED(IDC_BTN_PEDDETECTION, &CVisualAnalysisDlg::OnBnClickedBtnPeddetection)
	ON_BN_CLICKED(IDC_BTN_FACEDETECTION_S, &CVisualAnalysisDlg::OnBnClickedBtnFacedetectionSingle)
	ON_BN_CLICKED(IDCANCEL, &CVisualAnalysisDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_FACEDETECTION_M, &CVisualAnalysisDlg::OnBnClickedBtnFacedetectionMultiple)
END_MESSAGE_MAP()


// CVisualAnalysisDlg 消息处理程序

BOOL CVisualAnalysisDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	GetDlgItem(IDC_BTN_PEDDETECTION)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_FACEDETECTION_S)->EnableWindow(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CVisualAnalysisDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVisualAnalysisDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CVisualAnalysisDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Draw image to the HDC of a picture control
void CVisualAnalysisDlg::DrawImgtoHDC(IplImage * img, UINT ID)
{
	// get a dc
	CDC* pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	CvvImage cvImg;
	cvImg.CopyOf(img);
	cvImg.DrawToHDC(hDC,&rect);
	ReleaseDC(pDC);
}

/****************** 消息响应函数 **********************/
void CVisualAnalysisDlg::OnBnClickedBtnOpenfile()
{
	// TODO: Add your control notification handler code here
	// Open File Dialog
	CString filepath;
	CFileDialog dlg( TRUE, _T(".jpg"), NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image files (*.bmp; *.jpg) |*.bmp;*.jpg| All Files (*.*) |*.*||"), NULL );
	dlg.m_ofn.lpstrTitle = _T("Load Image"); 
	if( dlg.DoModal() == IDOK )
	{
		filepath = dlg.GetPathName();
		m_filePath = filepath;
	}
	else return;
 
    //载入图像
	if(m_pImg != NULL){
		cvReleaseImage(&m_pImg);
		m_pImg = NULL;
	}
    if( (m_pImg = cvLoadImage((LPSTR)(LPCTSTR)filepath, 1)) != 0 )//[[此处的argc==2是否需要改成argc==1？我改了之后才能运行成功。求大牛解惑]] //  wmzzzz : 在"属性"|"debug"|里的command arguments 里加入参数(一个路径:要打开的文件路径) 这时 argc==2 就合理了...可以试试多加几个
    {
		DrawImgtoHDC(m_pImg,IDC_SHOWIMG);
		GetDlgItem(IDC_BTN_PEDDETECTION)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_FACEDETECTION_S)->EnableWindow(TRUE);
		UpdateData(FALSE);
    }
 
	return;
}



void CVisualAnalysisDlg::OnBnClickedBtnPeddetection()
{
	// TODO: Add your control notification handler code here
	IplImage * pImg = cvCloneImage(m_pImg);
	m_od.DetectPedestrian(pImg);
	DrawImgtoHDC(pImg,IDC_SHOWIMG);
}


void CVisualAnalysisDlg::OnBnClickedBtnFacedetectionSingle()
{
	// TODO: Add your control notification handler code here
	IplImage * pImg = cvCloneImage(m_pImg);
	m_od.DetectFace(pImg);
	DrawImgtoHDC(pImg,IDC_SHOWIMG);
}


void CVisualAnalysisDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CVisualAnalysisDlg::OnBnClickedBtnFacedetectionMultiple()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BTN_FACEDETECTION_M)->EnableWindow(FALSE);

	// check the formatting of  image directory
	CString imgPath;
	if(m_imgDir.Right(1) != "\\"){m_imgDir = m_imgDir + "\\";}
	imgPath = m_imgDir + "*.jpg";
	// define a CFileFind class to search images
	CFileFind fileFinder;
	// obtain the total number of images in a directory
	bool isAll = fileFinder.FindFile(imgPath);
	int totalNum = 0;
	while(isAll)
	{
		isAll = fileFinder.FindNextFile();
		totalNum ++;
	}
	fileFinder.Close();
	m_ctrlProcess.SetRange(0,totalNum);
	m_ctrlProcess.SetPos(0);

	// detect face image-by-image
	IplImage * pImg = NULL;
	isAll = fileFinder.FindFile(imgPath);
	int i = 0;
	while(isAll)
	{
		isAll = fileFinder.FindNextFile();
		imgPath = fileFinder.GetFilePath();
		m_filePath = imgPath + _T("\r\n");
		if( (pImg = cvLoadImage((LPSTR)(LPCTSTR)imgPath, 1)) != 0 )//
		{
			m_od.DetectFace(pImg);
			cvReleaseImage(&pImg);
			pImg = NULL;
			i++;
			m_ctrlProcess.SetPos(i);
			UpdateData(FALSE);
		}
	}
	fileFinder.Close();

	GetDlgItem(IDC_BTN_FACEDETECTION_M)->EnableWindow(TRUE);
}
