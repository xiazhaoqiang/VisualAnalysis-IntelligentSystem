
// VisualAnalysisDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VisualAnalysis.h"
#include "VisualAnalysisDlg.h"
#include "afxdialogex.h"
#include "ParamConf.h"
//#include "Test_ObjectsDetection.h" // official library for saving results

#include <vector>




using namespace std;


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
	, m_radioValue(0)
	, m_cbRecMethods(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// Initialize the class variables
	m_pImg = NULL;
	m_filePath = _T("文件路径");
	// dongnanmeneast_15_1920x1080_30_R1
	m_imgDir = _T("E:\\Projects\\Dataset\\PKU-SVD-B_V2.0\\1_1_05(06)_0\\prob\\dongnanmeneast_15_1920x1080_30_R1");
}

void CVisualAnalysisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_FILEPATH, m_filePath);
	DDX_Control(pDX, IDC_PROGRESS_IMG, m_ctrlProcess);
	DDX_Radio(pDX, IDC_RADIO_TRAIN, m_radioValue);
	DDX_CBString(pDX, IDC_COMBO_RECMETHODS, m_cbRecMethods);
}

BEGIN_MESSAGE_MAP(CVisualAnalysisDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPENFILE, &CVisualAnalysisDlg::OnBnClickedBtnOpenfile)
	ON_BN_CLICKED(IDC_BTN_PEDDETECTION, &CVisualAnalysisDlg::OnBnClickedBtnPeddetectionSingle)
	ON_BN_CLICKED(IDC_BTN_FACEDETECTION_S, &CVisualAnalysisDlg::OnBnClickedBtnFacedetectionSingle)
	ON_BN_CLICKED(IDCANCEL, &CVisualAnalysisDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_FACEDETECTION_M, &CVisualAnalysisDlg::OnBnClickedBtnFacedetectionMultiple)
	ON_BN_CLICKED(IDC_CHK_CONFIGURE, &CVisualAnalysisDlg::OnClickedChkConfigure)
	ON_BN_CLICKED(IDC_BTN_CONFIGURE, &CVisualAnalysisDlg::OnBnClickedBtnConfigure)
	ON_BN_CLICKED(IDC_BTN_PEDDETECTION_M, &CVisualAnalysisDlg::OnBnClickedBtnPeddetectionMultiple)
	ON_BN_CLICKED(IDC_BTN_FACEREC_M, &CVisualAnalysisDlg::OnBnClickedBtnFaceRecMultiple)
	ON_BN_CLICKED(IDC_BTN_FACEREC_S, &CVisualAnalysisDlg::OnBnClickedBtnFacerecS)
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

/****************** 类内成员函数 **********************/
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

void CVisualAnalysisDlg::CreateXMLFile(char* taskType)
{
	// create a xml file for saving detection reuslts
	int type;
	if(taskType == "pd")	//type: pedestrian detection 
		type = 1;
	else if(taskType == "fd")	//type: face detection 
		type = 5;
	else if(taskType == "fr")
		type = 6;
	else
		type = 0;


	int pos = m_imgDir.ReverseFind(_T('\\'));
	CString metaFile = m_imgDir.Right(m_imgDir.GetLength()-1-pos);
	CString xmlName = _T(".\\Result\\") + metaFile + _T("_") + taskType + _T(".xml");
	TiXmlDocument* xmlDocument = new TiXmlDocument();	//创建一个根结点
	TiXmlDeclaration* xmlDeclaration=new TiXmlDeclaration("1.0","gb2312","");	//创建xml文件头
	xmlDocument->LinkEndChild(xmlDeclaration);	//加入文件头
	
	// 创建xml meta信息
	TiXmlElement* messageNode = new TiXmlElement("Message");	//创建根节点
	messageNode->SetAttribute("Version","1.0");
	xmlDocument->LinkEndChild(messageNode);

	TiXmlElement* infoNode = new TiXmlElement("Info");
	infoNode->SetAttribute("evaluateType",type);
	infoNode->SetAttribute("mediaFile",(LPSTR)(LPCTSTR)metaFile);
	messageNode->LinkEndChild(infoNode);

	//TiXmlElement* itemsNode = new TiXmlElement("Items");
	//messageNode->LinkEndChild(itemsNode);

	// 写入文件
	xmlDocument->SaveFile((LPSTR)(LPCTSTR)xmlName);
	// 释放内存
	delete xmlDocument;
	//delete xmlDeclaration;
	xmlDocument = NULL;
	//xmlDeclaration = NULL;
}

// 保存检测结果
void CVisualAnalysisDlg::SaveDetectionResults(vector<CvRect>* r, CString mediaFile, TiXmlElement* itemsNode, int type)
{
	TiXmlElement* itemNode = new TiXmlElement("Item");
	itemNode->SetAttribute("imageName",(LPSTR)(LPCTSTR)mediaFile);
	itemsNode->LinkEndChild(itemNode);

	int len = r->size();
	CvRect* rec;
	for(int i=0;i<len;i++)
	{
		rec = &(r->at(i));
		TiXmlElement* labelNode = new TiXmlElement("Label");
		labelNode->SetAttribute("type",type);
		labelNode->SetAttribute("l",rec->x);
		labelNode->SetAttribute("t",rec->y);
		labelNode->SetAttribute("r",rec->x+rec->width);
		labelNode->SetAttribute("b",rec->y+rec->height);
		itemNode->LinkEndChild(labelNode);
	}
	//xml_DetectionResults("fd",infoResults,"ppx_fd.xml",1);
}

// 保存识别结果
void CVisualAnalysisDlg::SaveRecResults(vector<CvRect>* r, CString mediaFile, TiXmlElement* itemsNode, vector<int>* id)
{
	TiXmlElement* itemNode = new TiXmlElement("Item");
	itemNode->SetAttribute("imageName",(LPSTR)(LPCTSTR)mediaFile);
	itemsNode->LinkEndChild(itemNode);

	int len = r->size();
	CvRect* rec;
	for(int i=0;i<len;i++)
	{
		rec = &(r->at(i));
		TiXmlElement* labelNode = new TiXmlElement("Label");
		labelNode->SetAttribute("id",id->at(i));
		labelNode->SetAttribute("score","0.800000");
		labelNode->SetAttribute("l",rec->x);
		labelNode->SetAttribute("t",rec->y);
		labelNode->SetAttribute("r",rec->x+rec->width);
		labelNode->SetAttribute("b",rec->y+rec->height);
		itemNode->LinkEndChild(labelNode);
	}
	//xml_DetectionResults("fd",infoResults,"ppx_fd.xml",1);
}

void CVisualAnalysisDlg::TrainFR(string methodType)
{
	// define variables for images and labels
	vector<int> labels;
	vector<Mat> images;
	CStdioFile file;
	CString str;	// a buffer for texts
	int faceWidth, faceHeight;
	
	if( methodType==_T("EigenFace") || methodType==_T("FisherFace") || methodType==_T("LBPHFace"))
	{
		CFileStatus tmp;
		if(!CFile::GetStatus(_T(".\\Data\\TrainingsetFR_CV\\labelsTrain.txt"),tmp))
		{
			// image files
			vector<string> imgDir;
			string imgdir(_T("E:\\Projects\\Dataset\\PKU-SVD-B_V2.0\\1_1_05(06)_0\\prob\\dongnanmeneast_15_1920x1080_30_R1\\"));
			imgDir.push_back(imgdir);
			imgdir = _T("E:\\Projects\\Dataset\\PKU-SVD-B_V2.0\\1_1_05(06)_0\\prob\\dongnanmenwest_16_1920x1080_30_R1\\");
			imgDir.push_back(imgdir);
			// annotation files
			vector<string> labelFile;
			string labelfile(_T(".\\Data\\GroundTruth\\dongnanmeneast_15_1920x1080_30_R1.xml"));
			labelFile.push_back(labelfile);
			labelfile = _T(".\\Data\\GroundTruth\\dongnanmenwest_16_1920x1080_30_R1.xml");
			labelFile.push_back(labelfile);
			// normalize the dataset formatting
			m_od.NormalizeDataset(imgDir,labelFile,"FR");
		}
	}
	// load data
	// load labels for images
	if( methodType == _T("CRClassifier"))
		file.Open(".\\Data\\TrainingsetFR_CRC\\labelsTrain.txt",CFile::modeRead);
	else
		file.Open(".\\Data\\TrainingsetFR_CV\\labelsTrain.txt",CFile::modeRead);
	while(file.ReadString(str)!= FALSE)
		labels.push_back(atoi(str));
	file.Close();
	// load the images
	string prefix;
	if( methodType == _T("CRClassifier")){
		prefix = _T(".\\Data\\\\TrainingsetFR_CRC\\");
		file.Open(_T(".\\Data\\TrainingsetFR_CRC\\imagesTrain.txt"),CFile::modeRead);
		faceWidth = 10;
		faceHeight = 12;
	}
	else
	{
		prefix = _T(".\\Data\\\\TrainingsetFR_CV\\");
		file.Open(_T(".\\Data\\TrainingsetFR_CV\\imagesTrain.txt"),CFile::modeRead);
		faceWidth = 30;
		faceHeight = 36;
	}
	Mat srcImg,scaledImg;
	while(file.ReadString(str)!= FALSE)
	{
		m_filePath = str;
		srcImg = imread(prefix+str.GetBuffer(0),CV_LOAD_IMAGE_GRAYSCALE);
		equalizeHist(srcImg,srcImg);
		resize(srcImg,scaledImg,Size(faceWidth,faceHeight));
		images.push_back(scaledImg);
		UpdateData(FALSE);
	}
	file.Close();

	// training
	m_filePath = _T("trainging......");
	UpdateData(FALSE);
	m_od.TrainModelFR(images,labels,methodType);
	m_filePath = _T("done......");
	UpdateData(FALSE);
}


void CVisualAnalysisDlg::PredictFR(string methodType)
{
	// generate a folder for the detection results
	if(!PathIsDirectory(_T(".\\Result"))){CreateDirectory(_T(".\\Result"),NULL);};
	CreateXMLFile("fr");

	// check the formatting of  image directory
	int pos = m_imgDir.ReverseFind('\\');
	CString imgPath;
	CString xmlName = m_imgDir.Right(m_imgDir.GetLength()-1-pos);
	xmlName = _T(".\\Result\\") + xmlName + _T("_fr.xml");
	// load xml file
	TiXmlDocument* xmlDocument = new TiXmlDocument((LPSTR)(LPCTSTR)xmlName);
	xmlDocument->LoadFile();
	TiXmlElement * messageNode = xmlDocument->FirstChildElement();
	TiXmlElement* itemsNode = new TiXmlElement("Items");
	messageNode->LinkEndChild(itemsNode);

	if(m_imgDir.Right(1) != "\\") m_imgDir = m_imgDir + "\\";
	imgPath = m_imgDir + "*.jpg";
	// define a CFileFind class to search images
	CFileFind fileFinder;
	// obtain the total number of images in a directory
	BOOL isAll = fileFinder.FindFile(imgPath);
	int totalNum = 0;
	while(isAll)
	{
		isAll = fileFinder.FindNextFile();
		totalNum ++;
	}
	fileFinder.Close();
	m_ctrlProcess.SetRange(0,totalNum);
	m_ctrlProcess.SetPos(0);

	// detect faces image-by-image
	IplImage * pImg = NULL;
	CString imgName;
	isAll = fileFinder.FindFile(imgPath);
	int i = 0;
	vector<CvRect> faces;
	vector<int> id;
	// traversing the whole folder
	while(isAll)
	{
		isAll = fileFinder.FindNextFile();
		imgPath = fileFinder.GetFilePath();
		imgName = fileFinder.GetFileName();
		m_filePath = imgPath + _T("\r\n");
		if( (pImg = cvLoadImage((LPSTR)(LPCTSTR)imgPath, 1)) != NULL )//
		{
			// detect faces
			m_od.PredictFR(pImg,&faces,&id,methodType);
			// save the detection results
			imgPath = _T(".\\Result\\") + imgName;
			SaveRecResults(&faces,imgName,itemsNode,&id);
			cvReleaseImage(&pImg);
			pImg = NULL;
			// update the display of UI:processing bar
			i++;
			m_ctrlProcess.SetPos(i);
			UpdateData(FALSE);
		}
		faces.clear();
		xmlDocument->SaveFile((LPSTR)(LPCTSTR)xmlName);
	}
	fileFinder.Close();
	// 释放内存
	delete xmlDocument;
	xmlDocument = NULL;
	UpdateData(FALSE);
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
    if( (m_pImg = cvLoadImage((LPSTR)(LPCTSTR)filepath, 1)) != NULL )
    {
		DrawImgtoHDC(m_pImg,IDC_SHOWIMG);
		GetDlgItem(IDC_BTN_PEDDETECTION)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_FACEDETECTION_S)->EnableWindow(TRUE);
		UpdateData(FALSE);
    }
 
	return;
}

// 配置文件修改
void CVisualAnalysisDlg::OnClickedChkConfigure()
{
	// TODO: Add your control notification handler code here
	
}


void CVisualAnalysisDlg::OnBnClickedBtnConfigure()
{
	// TODO: Add your control notification handler code here
	if(m_pc.DoModal()==IDOK)
	{
		m_imgDir = m_pc.m_imgDir;
		m_od.scaling = atof((LPTSTR)(LPCTSTR)m_pc.m_paramScale);
		m_od.pyramidCoef = atof((LPTSTR)(LPCTSTR)m_pc.m_paramFactor);
	}
}


// 单幅行人检测
void CVisualAnalysisDlg::OnBnClickedBtnPeddetectionSingle()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BTN_PEDDETECTION)->EnableWindow(FALSE);
	IplImage * pImg = cvCloneImage(m_pImg);
	vector<CvRect> regions;
	m_od.DetectPedestrian(pImg,&regions);

	CvPoint pt_tl, pt_br;
	// 
	for(int i =0;i<(int)regions.size();i++)
	{
		pt_tl.x = regions.at(i).x; 
		pt_tl.y = regions.at(i).y;
		pt_br.x = regions.at(i).x+regions.at(i).width;
		pt_br.y = regions.at(i).y+regions.at(i).height;
		cvRectangle(pImg,pt_tl,pt_br,cvScalar(255,0,0),5);
	}
	DrawImgtoHDC(pImg,IDC_SHOWIMG);
	GetDlgItem(IDC_BTN_PEDDETECTION)->EnableWindow(TRUE);
}

// 单幅人脸检测
void CVisualAnalysisDlg::OnBnClickedBtnFacedetectionSingle()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BTN_FACEDETECTION_S)->EnableWindow(FALSE);
	IplImage * pImg = cvCloneImage(m_pImg);
	vector<CvRect> faces;
	m_od.DetectFace(pImg,&faces);
	CvPoint pt_tl, pt_br;
	// 
	for(int i =0;i<(int)faces.size();i++)
	{
		pt_tl.x = faces.at(i).x; 
		pt_tl.y = faces.at(i).y;
		pt_br.x = faces.at(i).x+faces.at(i).width;
		pt_br.y = faces.at(i).y+faces.at(i).height;
		cvRectangle(pImg,pt_tl,pt_br,cvScalar(0,0,255),5);
	}
	DrawImgtoHDC(pImg,IDC_SHOWIMG);
	GetDlgItem(IDC_BTN_FACEDETECTION_S)->EnableWindow(TRUE);
}

// 单幅人脸识别
void CVisualAnalysisDlg::OnBnClickedBtnFacerecS()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BTN_FACEREC_S)->EnableWindow(FALSE);
	IplImage * pImg = cvCloneImage(m_pImg);
	m_od.DetectFace(pImg);
	DrawImgtoHDC(pImg,IDC_SHOWIMG);
	GetDlgItem(IDC_BTN_FACEREC_S)->EnableWindow(TRUE);
}

// 多幅图像的行人检测函数
void CVisualAnalysisDlg::OnBnClickedBtnPeddetectionMultiple()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BTN_PEDDETECTION_M)->EnableWindow(FALSE);

	// generate a folder for the detection results
	if(!PathIsDirectory(_T(".\\Result"))){CreateDirectory(_T(".\\Result"),NULL);};
	CreateXMLFile("pd");

	// check the formatting of  image directory
	int pos = m_imgDir.ReverseFind('\\');
	CString imgPath;
	CString xmlName = m_imgDir.Right(m_imgDir.GetLength()-1-pos);
	xmlName = _T(".\\Result\\") + xmlName + _T("_pd.xml");
	// load xml file
	TiXmlDocument* xmlDocument = new TiXmlDocument((LPSTR)(LPCTSTR)xmlName);
	xmlDocument->LoadFile();
	TiXmlElement * messageNode = xmlDocument->FirstChildElement();
	TiXmlElement* itemsNode = new TiXmlElement("Items");
	messageNode->LinkEndChild(itemsNode);
	
	if(m_imgDir.Right(1) != "\\") m_imgDir = m_imgDir + "\\";
	imgPath = m_imgDir + "*.jpg";
	// define a CFileFind class to search images
	CFileFind fileFinder;
	// obtain the total number of images in a directory
	BOOL isAll = fileFinder.FindFile(imgPath);
	int totalNum = 0;
	while(isAll)
	{
		isAll = fileFinder.FindNextFile();
		totalNum ++;
	}
	fileFinder.Close();
	m_ctrlProcess.SetRange(0,totalNum);
	m_ctrlProcess.SetPos(0);

	// detect faces image-by-image
	IplImage * pImg = NULL;
	CString imgName;
	isAll = fileFinder.FindFile(imgPath);
	int i = 0;
	vector<CvRect> regions;
	// traversing the whole folder
	while(isAll)
	{
		isAll = fileFinder.FindNextFile();
		imgPath = fileFinder.GetFilePath();
		imgName = fileFinder.GetFileName();
		m_filePath = imgPath + _T("\r\n");
		if( (pImg = cvLoadImage((LPSTR)(LPCTSTR)imgPath, 1)) != NULL )//
		{
			// detect people
			m_od.DetectPedestrian(pImg,&regions);
			// save the detection results
			imgPath = _T(".\\Result\\") + imgName;
			//cvSaveImage((LPSTR)(LPCTSTR)imgPath,pImg);
			SaveDetectionResults(&regions,imgName,itemsNode,2);
			cvReleaseImage(&pImg);
			pImg = NULL;
			// update the display of UI:processing bar
			i++;
			m_ctrlProcess.SetPos(i);
			UpdateData(FALSE);
		}
		regions.clear();
		xmlDocument->SaveFile((LPSTR)(LPCTSTR)xmlName);
	}
	fileFinder.Close();
	// 释放内存
	delete xmlDocument;
	xmlDocument = NULL;

	GetDlgItem(IDC_BTN_PEDDETECTION_M)->EnableWindow(TRUE);
}


// 多幅图像的人脸检测函数
void CVisualAnalysisDlg::OnBnClickedBtnFacedetectionMultiple()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BTN_FACEDETECTION_M)->EnableWindow(FALSE);

	// generate a folder for the detection results
	if(!PathIsDirectory(_T(".\\Result"))){CreateDirectory(_T(".\\Result"),NULL);};
	CreateXMLFile("fd");

	// check the formatting of  image directory
	int pos = m_imgDir.ReverseFind('\\');
	CString imgPath;
	CString xmlName = m_imgDir.Right(m_imgDir.GetLength()-1-pos);
	xmlName = _T(".\\Result\\") + xmlName + _T("_fd.xml");
	// load xml file
	TiXmlDocument* xmlDocument = new TiXmlDocument((LPSTR)(LPCTSTR)xmlName);
	xmlDocument->LoadFile();
	TiXmlElement * messageNode = xmlDocument->FirstChildElement();
	TiXmlElement* itemsNode = new TiXmlElement("Items");
	messageNode->LinkEndChild(itemsNode);
	
	if(m_imgDir.Right(1) != "\\") m_imgDir = m_imgDir + "\\";
	imgPath = m_imgDir + "*.jpg";
	// define a CFileFind class to search images
	CFileFind fileFinder;
	// obtain the total number of images in a directory
	BOOL isAll = fileFinder.FindFile(imgPath);
	int totalNum = 0;
	while(isAll)
	{
		isAll = fileFinder.FindNextFile();
		totalNum ++;
	}
	fileFinder.Close();
	m_ctrlProcess.SetRange(0,totalNum);
	m_ctrlProcess.SetPos(0);

	// detect faces image-by-image
	IplImage * pImg = NULL;
	CString imgName;
	isAll = fileFinder.FindFile(imgPath);
	int i = 0;
	vector<CvRect> faces;
	// traversing the whole folder
	while(isAll)
	{
		isAll = fileFinder.FindNextFile();
		imgPath = fileFinder.GetFilePath();
		imgName = fileFinder.GetFileName();
		m_filePath = imgPath + _T("\r\n");
		if( (pImg = cvLoadImage((LPSTR)(LPCTSTR)imgPath, 1)) != 0 )//
		{
			// detect faces
			m_od.DetectFace(pImg,&faces);
			// save the detection results
			imgPath = _T(".\\Result\\") + imgName;
			//cvSaveImage((LPSTR)(LPCTSTR)imgPath,pImg);
			SaveDetectionResults(&faces,imgName,itemsNode,0);
			cvReleaseImage(&pImg);
			pImg = NULL;
			// update the display of UI:processing bar
			i++;
			m_ctrlProcess.SetPos(i);
			UpdateData(FALSE);
		}
		faces.clear();
		xmlDocument->SaveFile((LPSTR)(LPCTSTR)xmlName);
	}
	fileFinder.Close();
	// 释放内存
	delete xmlDocument;
	xmlDocument = NULL;

	GetDlgItem(IDC_BTN_FACEDETECTION_M)->EnableWindow(TRUE);
}

// 多幅人脸的识别函数
void CVisualAnalysisDlg::OnBnClickedBtnFaceRecMultiple()
{
	// TODO: Add your control notification handler code here
	if( m_radioValue == -1 ){
		MessageBox(_T("Error"),_T("Error"));
		return;
	}
	GetDlgItem(IDC_BTN_FACEREC_M)->EnableWindow(FALSE);
	UpdateData(TRUE);

	string methodType = m_cbRecMethods.GetBuffer(0);
	// training procedure
	if( m_radioValue ==  0 )
	{
		TrainFR(methodType);
	}
	else // valuation procedure
	{
		PredictFR(methodType);
	}
	GetDlgItem(IDC_BTN_FACEREC_M)->EnableWindow(TRUE);
}

// responce function for Cancel button
void CVisualAnalysisDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}
