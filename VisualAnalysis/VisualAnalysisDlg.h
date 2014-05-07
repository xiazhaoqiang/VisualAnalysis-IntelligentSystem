
// VisualAnalysisDlg.h : 头文件
//

#pragma once

#include <cv.h>
#include <highgui.h>
#include "src\CvvImage.h"
#include "src\ObjectDetection.h"
#include "src\tinyxml.h"
#include "src\tinystr.h"

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
public:
	afx_msg void OnBnClickedBtnOpenfile();
	// Draw image to the HDC of a picture control
	void DrawImgtoHDC(IplImage * img, UINT ID);
private:
	IplImage* m_pImg;	// A pointer to an image
	CObjectDetection m_od;//
public:
	afx_msg void OnBnClickedBtnPeddetection();
	afx_msg void OnBnClickedBtnFacedetectionSingle();
	afx_msg void OnBnClickedCancel();
	CString m_filePath;
	afx_msg void OnBnClickedBtnFacedetectionMultiple();
private:
	CString m_imgDir;
public:
	CProgressCtrl m_ctrlProcess;
	void SaveDetectionResults(vector<CvRect>* r, CString mediaFile, TiXmlElement* itemsNode, int type = 0);
	void CreateXMLFile(char* taskType);
};
