
// VisualAnalysisDlg.h : 头文件
//

#pragma once

#include <string>
#include <opencv2/opencv.hpp>
#include "src\CvvImage.h"
#include "src\ObjectDetection.h"
#include "src\tinyxml.h"
#include "src\tinystr.h"
#include "paramconf.h"

// CVisualAnalysisDlg 对话框
class CVisualAnalysisDlg : public CDialogEx
{
// 构造
public:
	CVisualAnalysisDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_VISUALANALYSIS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	IplImage* m_pImg;	// A pointer to an image
	CObjectDetection m_od;//
public:
	afx_msg void OnBnClickedBtnOpenfile();
	afx_msg void OnBnClickedBtnPeddetectionSingle();
	afx_msg void OnBnClickedBtnFacedetectionSingle();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnFacedetectionMultiple();
	afx_msg void OnClickedChkConfigure();
	afx_msg void OnBnClickedBtnConfigure();
	afx_msg void OnBnClickedBtnPeddetectionMultiple();
	afx_msg void OnBnClickedBtnFaceRecMultiple();
private:
	CString m_imgDir;
public:
	CProgressCtrl m_ctrlProcess; // control variable for progress control
	CString m_filePath;
	// Draw image to the HDC of a picture control
	void DrawImgtoHDC(IplImage * img, UINT ID);
	void SaveDetectionResults(vector<CvRect>* r, CString mediaFile, TiXmlElement* itemsNode, int type = 0);
	void CreateXMLFile(char* taskType);

	int m_radioValue;
	CString m_cbRecMethods;
	CParamConf m_pc;
};
