// ParamConf.cpp : implementation file
//

#include "stdafx.h"
#include "VisualAnalysis.h"
#include "ParamConf.h"
#include "afxdialogex.h"


// CParamConf dialog

IMPLEMENT_DYNAMIC(CParamConf, CDialogEx)

CParamConf::CParamConf(CWnd* pParent /*=NULL*/)
	: CDialogEx(CParamConf::IDD, pParent)
	, m_imgDir(_T("E:\\Projects\\Dataset\\PKU-SVD-B_Eval\\1_2_06_1\\prob\\dongnanmeneast_15_1920x1080_30_R1"))
	, m_paramScale(_T("4"))
	, m_paramFactor(_T("1.1"))
{

	m_skinModelTH = _T("0.8");
	m_winSize = _T("0.8");
	m_altNum = 2;
}

CParamConf::~CParamConf()
{
}

void CParamConf::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_IMGDIR, m_imgDir);
	DDX_Text(pDX, IDC_EDIT_SCALE, m_paramScale);
	DDX_Text(pDX, IDC_EDIT_FACTOR, m_paramFactor);
	DDX_Text(pDX, IDC_EDIT_SMTH, m_skinModelTH);
	DDX_Text(pDX, IDC_EDIT_WINSIZE, m_winSize);
	DDX_Text(pDX, IDC_EDIT_ALTNUM, m_altNum);
}


BEGIN_MESSAGE_MAP(CParamConf, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_OPENDIR, &CParamConf::OnBnClickedBtnOpendir)
	ON_BN_CLICKED(IDOK, &CParamConf::OnBnClickedOk)
END_MESSAGE_MAP()


// CParamConf message handlers


void CParamConf::OnBnClickedBtnOpendir()
{
	// TODO: Add your control notification handler code here
	// replace the shell functions with IFileDialog in Windows vista or later
	TCHAR pszPath[MAX_PATH];  
    BROWSEINFO bi;   
    bi.hwndOwner      = this->GetSafeHwnd();  
    bi.pidlRoot       = NULL;  
    bi.pszDisplayName = NULL;   
    bi.lpszTitle      = TEXT("ÇëÑ¡ÔñÎÄ¼þ¼Ð");   
    bi.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;  
    bi.lpfn           = NULL;   
    bi.lParam         = 0;  
    bi.iImage         = 0;   
  
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);  
    if (pidl == NULL)  
    {  
        return;  
    }  
  
    if (SHGetPathFromIDList(pidl, pszPath))  
    {  
        m_imgDir = pszPath;
		UpdateData(FALSE);  
    }  
	
}


void CParamConf::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CDialogEx::OnOK();
}
