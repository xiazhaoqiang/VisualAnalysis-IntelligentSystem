#pragma once

#ifdef ANALYSISSDKDEMO
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif

#include "Common.h"
void test_SinglePedestrianDetection_float();
DLL void xml_DetectionResults(string directoryName,vector<SaveInfo> detection_results,string xml_name,int evaluateTyte);