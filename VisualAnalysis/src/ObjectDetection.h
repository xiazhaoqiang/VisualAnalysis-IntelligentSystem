/**************************************************************
* Type: .h file
* Class Name: CObjectDetection
* Function: detect objects in an image, such as pedestrian, face and son
* Create date: 2014.4.26
* Creator£º Zhaoqiang Xia (xiazhaoqiang@gmail.com)
***************************************************************/
#pragma once

#include <opencv2/opencv.hpp> 

// construct a color table for displaying
static CvScalar colors[ ] = {
        {{0,0,255}},
        {{0,128,255}},
        {{0,255,255}},
        {{0,255,0}},
        {{255,128,0}},
        {{255,255,0}},
        {{255,0,0}},
        {{255,0,255}}
    };

class CObjectDetection
{
private:

public:
	CObjectDetection();
	void DetectPedestrian(IplImage * pImg);	// detect pedestrian
	void DetectFace(IplImage* pImg);		// detect face
};
