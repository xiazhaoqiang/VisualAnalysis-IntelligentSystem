/**************************************************************
* Type: .h file
* Class Name: CObjectDetection
* Function: detect objects in an image, such as pedestrian, face and son
* Create date: 2014.4.26
* Creator： Zhaoqiang Xia (xiazhaoqiang@gmail.com)
***************************************************************/
#pragma once

#include <opencv2/opencv.hpp> 
#include <vector>
#include <string>

using namespace cv;
using namespace std;

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
	CvHaarClassifierCascade* frontalFaceCascade;
	CvHaarClassifierCascade* profileFaceCascade;
	CvHaarClassifierCascade* bodyCascade;

public:
	// function
	CObjectDetection();
	void DetectPedestrian(IplImage * pImg, vector<CvRect>* peopleRegion = NULL);	// detect pedestrian
	void DetectFace(IplImage* pImg, vector<CvRect>* faceRegion = NULL);		// detect face
	void TrainModelFR(vector<Mat>& imgMats, vector<int>& labelMats, string recMethod = "FisherFace");
	void PredictFR(IplImage* pImg, vector<CvRect>* faceRegion, vector<int>* id, string recMethod = "FisherFace");
	void NormalizeDataset(vector<string> imgDirs, vector<string> labelFiles, char* taskName = NULL);
	double SkinColorModelFilter(Mat& faceRect);
	// data
	double scaling;	// 图像缩放系数
	double pyramidCoef;	// 金字塔图像间缩放比例
	double skinModelTH; // 颜色模型分割阈值
	double winSize;
	HOGDescriptor hog;
	int hogType; /*hogType:0-None;1-human*/
	int srcRows;
	int srcCols;
	int altNum;
};
