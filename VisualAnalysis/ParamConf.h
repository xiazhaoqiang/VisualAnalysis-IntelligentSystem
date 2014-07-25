#pragma once


// CParamConf dialog

class CParamConf : public CDialogEx
{
	DECLARE_DYNAMIC(CParamConf)

public:
	CParamConf(CWnd* pParent = NULL);   // standard constructor
	virtual ~CParamConf();

// Dialog Data
	enum { IDD = IDD_PARAMCONF_M };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_imgDir;
	CString m_paramScale;
	CString m_paramFactor;
	afx_msg void OnBnClickedBtnOpendir();
	afx_msg void OnBnClickedOk();
	CString m_skinModelTH;
	CString m_winSize;
	int m_altNum;
};
