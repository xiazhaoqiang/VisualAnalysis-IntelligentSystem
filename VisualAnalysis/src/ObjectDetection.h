/**************************************************************
* Type: .h file
* Class Name: CObjectDetection
* Function: detect objects in an image, such as pedestrian, face and son
* Create date: 2014.4.26
* Creator£º Zhaoqiang Xia (xiazhaoqiang@gmail.com)
***************************************************************/
#pragma once

#include <opencv2/opencv.hpp> 

class CObjectDetection
{
private:

public:
	CObjectDetection();
	void DetectPedestrian(IplImage * pImg);

};