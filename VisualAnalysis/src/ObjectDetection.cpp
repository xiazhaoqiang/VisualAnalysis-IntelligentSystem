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