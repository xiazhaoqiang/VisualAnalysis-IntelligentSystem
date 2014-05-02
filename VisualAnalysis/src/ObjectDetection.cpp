/**************************************************************
* Type: .cpp file
* Class Name: CObjectDetection
* Function: detect objects in an image, such as pedestrian, face and son
* Create date: 2014.4.26
* Creator£∫ Zhaoqiang Xia (xiazhaoqiang@gmail.com)
***************************************************************/
#include "stdafx.h"
#include "ObjectDetection.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace cv;
using namespace std;

CObjectDetection::CObjectDetection()
{
}

void CObjectDetection::DetectPedestrian(IplImage* pImg)
{
	Mat imgMat(pImg);
	vector<Rect> found, found_filtered;
	HOGDescriptor hog;
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());//µ√µΩºÏ≤‚∆˜
	hog.detectMultiScale(imgMat, found, 0, Size(8,8), Size(32,32), 1.05, 2);
	//*// œ‘ æ  
    for (size_t i = 0; i < found.size(); i++)
    {  
        cv::rectangle(imgMat, found[i], cv::Scalar(255,0,0), 2);  
    }//*/
	/*size_t i, j;  
    for( i = 0; i < found.size(); i++ )
	{  
		Rect r = found[i];  
		for( j = 0; j < found.size(); j++ )
		{
			if( j != i && (r & found[j]) == r)  
				break;  
            if( j == found.size() )  
                found_filtered.push_back(r);  
        }
	}
    for( i = 0; i < found_filtered.size(); i++ )  
    {  
		Rect r = found_filtered[i];  
        // the HOG detector returns slightly larger rectangles than the real objects.  
        // so we slightly shrink the rectangles to get a nicer output.  
		r.x += cvRound(r.width*0.1);  
        r.width = cvRound(r.width*0.8);  
        r.y += cvRound(r.height*0.07);  
        r.height = cvRound(r.height*0.8);  
        rectangle(imgMat, r.tl(), r.br(), cv::Scalar(0,0,255), 3);  
    } //*/

	//imshow("people detector",imgMat);
}

void CObjectDetection::DetectFace(IplImage* pImg)
{
	CvHaarClassifierCascade* cascade = NULL;
	CvMemStorage* storage = NULL;
	// load the trained files for adaboost classifier
	cascade = (CvHaarClassifierCascade*)cvLoad( ".\\Data\\haarcascades\\haarcascade_frontalface_alt2.xml", 0, 0, 0 );
	storage = cvCreateMemStorage(0);
	if( !cascade )
    {
		MessageBox(NULL,_T("Missed Cascade Files!"),_T("Error Message"),MB_YESNO|MB_ICONQUESTION);
        return ;
    }
	// transform the original image to a smaller image
	double scale = 4.5; // scale = 1.3;
	IplImage* grayImg = cvCreateImage( cvSize(pImg->width,pImg->height), 8, 1 );
    IplImage* smallImg = cvCreateImage( cvSize( cvRound(pImg->width/scale),
                         cvRound(pImg->height/scale)),8, 1 );
    cvCvtColor( pImg, grayImg, CV_BGR2GRAY );
    cvResize( grayImg, smallImg, CV_INTER_LINEAR ); // the size of small image 
    cvEqualizeHist( smallImg, smallImg );
	cvClearMemStorage( storage );

	CvSeq* faces = cvHaarDetectObjects( smallImg, cascade, storage,
                                            1.1, 2, 0/*CV_HAAR_DO_CANNY_PRUNING*/,
                                            cvSize(1, 1) );
	CvPoint pt_tl, pt_br;
	for( int i = 0; i < (faces ? faces->total : 0); i++ )
    {
		CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
		pt_tl.x = cvRound( r->x*scale ); 
		pt_tl.y = cvRound( r->y*scale );
		pt_br.x = cvRound( (r->x+r->width)*scale );
		pt_br.y = cvRound( (r->y+r->height)*scale );
		cvRectangle(pImg,pt_tl,pt_br,cvScalar(0,0,255),5);
    }
	cvReleaseImage( &grayImg );
    cvReleaseImage( &smallImg );
}
