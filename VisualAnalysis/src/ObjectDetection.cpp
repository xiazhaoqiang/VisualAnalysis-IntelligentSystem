/**************************************************************
* Type: .cpp file
* Class Name: CObjectDetection
* Function: detect objects in an image, such as pedestrian, face and on
* Create date: 2014.4.26
* Creator： Zhaoqiang Xia (xiazhaoqiang@gmail.com)
***************************************************************/
#include "stdafx.h"
#include "ObjectDetection.h"
#include "facerec.hpp"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CObjectDetection::CObjectDetection()
{
	// 初始化类成员参数
	scaling = 4.0;
	pyramidCoef = 1.1;
	frontalFaceCascade = NULL;
	bodyCascade = NULL;
	hogType = 0;
	srcRows = 12; // height
	srcCols = 10; // width
}

////////////////////////////////////////////////////////////////////////////////////
//
// Pedestrian Detection Functions
//
////////////////////////////////////////////////////////////////////////////////////
void CObjectDetection::DetectPedestrian(IplImage* pImg, vector<CvRect>* peopleRegion)
{
	int i=0, j=0;
	// 转换为灰度图像，缩放图像
	Mat imgMat(pImg);
	int rows = cvRound(imgMat.rows/scaling);
	int cols = cvRound(imgMat.cols/scaling);
	Mat grayMat(rows,cols,CV_8UC1);
	cvtColor(imgMat,grayMat,CV_RGB2GRAY);
	Mat smallMat(rows,cols,CV_8UC1);
	resize(grayMat,smallMat,Size(cols,rows));
	equalizeHist(smallMat,smallMat);
	
	vector<Rect> found, found_filtered;
	// 检测行人 - HOGDetector
	if(0 == hogType)
	{
		hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
		hogType = 1;
	}
	hog.detectMultiScale(smallMat, found, 0, Size(8,8), Size(0,0), 1.05, 2);
	// Haar-Adaboost
	CvMemStorage* storage = NULL;
	// load the trained files for adaboost classifier
	// option: haarcascade_mcs_upperbody, haarcascade_fullbody
	if(NULL == bodyCascade)
	{
		bodyCascade = (CvHaarClassifierCascade*)cvLoad( ".\\Data\\haarcascades\\haarcascade_fullbody.xml", 0, 0, 0 );
		if( !bodyCascade )
		{
			MessageBox(NULL,_T("Missed Cascade File for Human Body!"),_T("Error Message"),MB_YESNO|MB_ICONQUESTION);
			return ;
		}
	}
	storage = cvCreateMemStorage(0);
	cvClearMemStorage( storage );
	// detect faces
	IplImage smallImg = smallMat;
	CvSeq* regions = cvHaarDetectObjects( &smallImg, bodyCascade, storage,
                                            pyramidCoef, 2, /*0*/CV_HAAR_DO_CANNY_PRUNING,
                                            cvSize(10, 20) );
	for( i = 0; i < (regions ? regions->total : 0); i++ )
    {
		CvRect* r = (CvRect*)cvGetSeqElem( regions, i );
		// re-detect by a skin color model
		Rect roi = *r;
		found.push_back(roi);
	}
	// 检测重复区域
	int recLen;
	recLen = found.size();
    for( i = 0; i < recLen; i++ )
	{  
		Rect r = found[i];  
		for( j = 0; j < recLen; j++ )
		{
			if( j != i && (r & found[j]) == r)  break;  
            if(j == recLen-1)found_filtered.push_back(r);  
        }
	}
    for( i = 0; i < (int)found_filtered.size(); i++ )  
    {  
		Rect r = found_filtered[i];
        // the HOG detector returns slightly larger rectangles than the real objects.  
        // so we slightly shrink the rectangles to get a nicer output.  
		r.width = max(0,cvRound(r.width*scaling*0.8));  
		r.x = max(0,cvRound(r.x*scaling + r.width*0.1));
        r.height = max(0,cvRound(r.height*scaling*0.8));  
		r.y = max(0,cvRound(r.y*scaling + r.height*0.1)); 

		if(peopleRegion != NULL)
		{
			CvRect pr = (CvRect) r;
			//pr.x = r.x; pr.y = r.y;
			//pr.width = r.width;
			//pr.height = r.height;
			peopleRegion->push_back(pr); 
		}
    }
	// release the memory
	cvReleaseMemStorage( &storage );
}


////////////////////////////////////////////////////////////////////////////////////
//
// Face Detection Functions
//
////////////////////////////////////////////////////////////////////////////////////
void CObjectDetection::DetectFace(IplImage* pImg, vector<CvRect>* faceRegion)
{
	//
	// use opencv 1.x style to implement the face detection
	//
	CvMemStorage* storage = NULL;
	// load the trained files for adaboost classifier
	if(NULL == frontalFaceCascade)
	{
		frontalFaceCascade = (CvHaarClassifierCascade*)cvLoad( ".\\Data\\haarcascades\\haarcascade_frontalface_alt2.xml", 0, 0, 0 );
		if( !frontalFaceCascade )
		{
			MessageBox(NULL,_T("Missed Cascade File for Frantal Face!"),_T("Error Message"),MB_YESNO|MB_ICONQUESTION);
			return ;
		}
	}
	
	// transform the original image to a smaller image
	double scale = scaling; // scale = 1.3; 4.5,4,
	IplImage* grayImg = cvCreateImage( cvSize(pImg->width,pImg->height), 8, 1 );
    IplImage* smallImg = cvCreateImage( cvSize( cvRound(pImg->width/scale),
                         cvRound(pImg->height/scale)),8, 1 );
	IplImage* colorImg = cvCreateImage( cvSize( cvRound(pImg->width/scale),
                         cvRound(pImg->height/scale)),8, 3 );
    cvCvtColor( pImg, grayImg, CV_BGR2GRAY );
    cvResize( grayImg, smallImg, CV_INTER_LINEAR ); // the size of small image 
	cvResize( pImg, colorImg, CV_INTER_LINEAR );
    cvEqualizeHist( smallImg, smallImg );
	storage = cvCreateMemStorage(0);
	cvClearMemStorage( storage );
	// detect faces
	CvSeq* faces = cvHaarDetectObjects( smallImg, frontalFaceCascade, storage,
                                            pyramidCoef, 2, /*0*/CV_HAAR_DO_CANNY_PRUNING,
                                            cvSize(15, 15) );
	// obtain the original position by inverse transform
	Mat imgM(colorImg);

	double ratio = 0.0;
	for( int i = 0; i < (faces ? faces->total : 0); i++ )
    {
		CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
		// re-detect by a skin color model
		Rect roi = *r;
		Mat faceRect(imgM,roi);
		// debug code
		//namedWindow("MyPicture");
		//imshow("MyPicture",faceRect);
		//waitKey(0);
		ratio = SkinColorModelFilter(faceRect);
		if(ratio < 0.5)
			continue;
		// adjust the locations of 
		r->x = cvRound( r->x*scale );
		r->y = cvRound( r->y*scale );
		r->width = cvRound( r->width*scale );
		r->height = cvRound( r->height*scale );
		// save the face region to a vector
		if(faceRegion != NULL)
			faceRegion->push_back(*r);
    }
	// release memories
	cvReleaseImage( &grayImg );
    cvReleaseImage( &smallImg );
	cvReleaseImage( &colorImg );
	cvReleaseMemStorage( &storage );
}


double CObjectDetection::SkinColorModelFilter(Mat& faceRect)
{
	// set up default parameters
	double maxValue=0., minValue=0.;
	Mat invC = (Mat_<double>(2,2) << 0.15318, 0.0858835, 0.0858835, 0.08183);
	Mat m = (Mat_<double>(2,1) << 116.740471076247,147.503047826355);
	Mat x = (Mat_<double>(2,1) << 0.0,0.0);
	Mat imgYCrCb;
	cvtColor(faceRect,imgYCrCb,CV_RGB2YCrCb);
	Mat p = Mat::zeros(imgYCrCb.rows,imgYCrCb.cols,CV_64F);
	Mat pe = Mat_<double>(1,1);
	for(int i = 0;i<imgYCrCb.rows;i++)
		for(int j =0;j<imgYCrCb.cols;j++)
		{
			Vec3b v = imgYCrCb.at<Vec3b>(i,j);
			x.at<double>(0,0) = (double)v[1];	// cb
			x.at<double>(1,0) = (double)v[2];	// cr
			Mat x_u = x - m;
			pe = x_u.t()*invC*x_u;
			p.at<double>(i,j) = exp(-0.5*pe.at<double>(0,0));
		}
	minMaxLoc(p,&minValue,&maxValue);
	p /= maxValue; 
	p *= 255.0;
	double threshold = 0.6;
	int count = 0;
	for(int i = 0;i<p.rows;i++)
		for(int j =0;j<p.cols;j++)
		{
			if(p.at<double>(i,j) > threshold)
				count++;
		}
	double ratio = (double)count/(double)(p.rows*p.cols);
	return ratio;
}


////////////////////////////////////////////////////////////////////////////////////
//
// Face Recognition Functions
//
////////////////////////////////////////////////////////////////////////////////////
void CObjectDetection::NormalizeDataset(vector<string> imgDirs, vector<string> labelFiles, char* taskName)
{
	string defaultDir(".\\Data\\TrainingSet");
	if(NULL != taskName)
		defaultDir += taskName;
	defaultDir += "\\";
	// parse the dataset
	int numInputDirs = imgDirs.size();
	FileStorage fs;
	for(int i =0;i<numInputDirs;i++)
	{
		if(!fs.open(labelFiles[i], FileStorage::READ))
		{
			MessageBox(NULL,_T("Missed Annotation Files!"),_T("Error Message"),MB_YESNO|MB_ICONQUESTION);
			fs.release();
			return;
		}
		// read the annotation data
		FileNode itemsNode = fs["Item"];
		fs.release();
	}
}

void CObjectDetection::TrainModelFR(vector<Mat>& imgMats, vector<int>& labelMats, string recMethod)
{
	// create a class for specified method
	Ptr<FaceRecognizer> model;
	if(recMethod == _T("CRClassifier"))
		model = new CRClassifier();
	else if(recMethod == _T("EigenFace"))
		model = createEigenFaceRecognizer();
	else if(recMethod == _T("FisherFace"))
		model = createFisherFaceRecognizer();
	else if(recMethod == _T("LBPHFace"))
		model = createLBPHFaceRecognizer();
	else
		return;
	model->train(imgMats, labelMats);
	// save model
	string filePath(_T(".\\Data\\"));
	filePath += recMethod;
	filePath += _T(".xml");
	model->save(filePath);

}

void CObjectDetection::PredictFR(IplImage* pImg, vector<CvRect>* faceRegion, vector<int>* id, string recMethod)
{
	Ptr<FaceRecognizer> model;
	if(recMethod == _T("CRClassifier"))
		model = new CRClassifier();
	else if(recMethod == _T("EigenFace"))
		model =  createEigenFaceRecognizer();
	else if(recMethod == _T("FisherFace"))
		model = createFisherFaceRecognizer();
	else if(recMethod == _T("LBPHFace"))
		model =  createLBPHFaceRecognizer();
	else
		return ;
	// load model
	string filePath(_T(".\\Data\\"));
	filePath += recMethod;
	filePath += _T(".xml");
	model->load(filePath);
	// detect face
	vector<CvRect> faces;
	DetectFace(pImg,&faces);
	// predict
	// preprocessing
	Mat imgMat(pImg);
	Mat grayMat(imgMat.rows,imgMat.cols,CV_8UC1);
	cvtColor(imgMat,grayMat,CV_RGB2GRAY);
	Mat smallMat(Size(srcCols,srcRows),CV_8UC1);
	equalizeHist(grayMat,grayMat);

	int predLabel = 0;
	double coef = 0.;
	for(int i = 0;i<(int)faces.size();i++)
	{
		Rect roi(faces.at(i));
		Mat rdata(grayMat,roi);
		resize(rdata,smallMat,Size(srcCols,srcRows));
		model->predict(smallMat,predLabel,coef);
		if(coef > 0.05)
		{
			faceRegion->push_back(faces.at(i));
			id->push_back(predLabel);
		}
	}

	return ;
}
