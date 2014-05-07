#pragma once

#include <string>
#include "cv.h"

typedef struct FrameInfo
{
	//const char *mediaFile;/*视频文件名*/
	std::string mediaFile; /*视频文件名*/
	int frameSeq; 	/*图像帧序号*/
	cv::Mat img;

	FrameInfo()
	:mediaFile("")
	{
		frameSeq = -1; 
		img = cv::Mat();
	}

}FrameInfo;


typedef struct SaveInfo
{
	//const char *mediaFile; /*视频文件名*/
	std::string mediaFile; /*视频文件名*/
	int type;		/*保存结果类型，0-人脸，1-车，2-行人*/
	int frameSeq; 	/*图像帧序号*/
	int left;		/*目标左上x 坐标*/
	int top;		/*目标左上y 坐标*/
	int right;		/*目标右下X 坐标*/
	int bottom;		/*目标右下y 坐标*/
	int id;			/*跟踪目标*/
	float score;

}SaveInfo;

typedef struct FaceRecogItem
{
public:
	FaceRecogItem()
	{
		img = cv::Mat();
		leftTopX = -1;
		leftTopY = -1;
		rightBottomX = -1;
		rightBottomY = -1;
	}
	~FaceRecogItem() {}

public:
	cv::Mat img;
	int leftTopX;
	int leftTopY;
	int rightBottomX;
	int rightBottomY;

}FaceRecogItem;



//参赛者只需填写id，score两项
class FaceRecogSaveInfo
{
public:
	FaceRecogSaveInfo()
	{
		
		id = "";
		score = 0.00;
		picName = "";
	}

public:
	std::string id;
	float score;
	std::string picName;
	std::string left;
	std::string right;
	std::string top;
	std::string bottom;
};