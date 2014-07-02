/*
 * Copyright (c) 2014. Zhaoqiang Xia <xiazhaoqiang[at]gmail[dot]com>.
 * Released to public domain under terms of the BSD Simplified license.
 *
 */

#pragma once

#include <opencv2/opencv.hpp> 
#include <vector>
#include <string>
namespace cv{

static Mat asRowMatrix(InputArrayOfArrays src, int rtype, double alpha=1, double beta=0) {
    // make sure the input data is a vector of matrices or vector of vector
    if(src.kind() != _InputArray::STD_VECTOR_MAT && src.kind() != _InputArray::STD_VECTOR_VECTOR) {
        string error_message = "The data is expected as InputArray::STD_VECTOR_MAT (a std::vector<Mat>) or _InputArray::STD_VECTOR_VECTOR (a std::vector< vector<...> >).";
        CV_Error(CV_StsBadArg, error_message);
    }
    // number of samples
    size_t n = src.total();
    // return empty matrix if no matrices given
    if(n == 0)
        return Mat();
    // dimensionality of (reshaped) samples
    size_t d = src.getMat(0).total();
    // create data matrix
    Mat data((int)n, (int)d, rtype);
    // now copy data
    for(unsigned int i = 0; i < n; i++) {
        // make sure data can be reshaped, throw exception if not!
        if(src.getMat(i).total() != d) {
            string error_message = format("Wrong number of elements in matrix #%d! Expected %d was %d.", i, d, src.getMat(i).total());
            CV_Error(CV_StsBadArg, error_message);
        }
        // get a hold of the current row
        Mat xi = data.row(i);
        // make reshape happy by cloning for non-continuous matrices
        if(src.getMat(i).isContinuous()) {
            src.getMat(i).reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        } else {
            src.getMat(i).clone().reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        }
    }
    return data;
}


class CRClassifier:public FaceRecognizer
{
private:
	Mat _dicMat;
	Mat _labelMat;
public:
	// Initializes an empty model.
	CRClassifier(){}
	CRClassifier(InputArrayOfArrays src,InputArrayOfArrays labels){
		train(src,labels);
	}

	using FaceRecognizer::save;
    using FaceRecognizer::load;
	// construct a dictionary for face model
	void train(InputArrayOfArrays src, InputArray labels);
	
	// Predicts the label of a query image in src.
    int predict(InputArray src) const;

    // Predicts the label and confidence for a given sample.
    void predict(InputArray src, int &label, double &dist) const;

    // load trained model.
    void load(const FileStorage& fs);

    // save trained model.
    void save(FileStorage& fs) const;

    //AlgorithmInfo* info() const;
};

void CRClassifier::train(InputArrayOfArrays src, InputArray labels)
{
	if(src.total() == 0) {
		string error_message = format("Empty training data was given. You'll need more than one sample to learn a model.");
		CV_Error(CV_StsBadArg, error_message);
	}

	// get labels
    _labelMat = labels.getMat();
    // observations in row
    _dicMat = asRowMatrix(src, CV_64FC1);
}


void CRClassifier::predict(InputArray src, int &minClass, double &minDist) const {
    // calculate the coefficients
    double gamma = 0.01;
	Mat D = _dicMat.t();
	Mat tmp =_dicMat*D + Mat::eye(D.cols,D.cols,D.type())*gamma;
	Mat proj = tmp.inv()*_dicMat;

	tmp = src.getMat();
	Mat y = tmp.reshape(1,1);
	y.convertTo(y,CV_64FC1);
	y = y.t();
	Mat coef = proj*y;

	// calculate residuals
	Mat res = Mat::zeros(1,D.cols,D.type());
	int idx = -1;
	double nu,de;
	minDist = numeric_limits<double>::max();
	for(int i=0; i<D.cols; i++)
	{
		double cc = coef.at<double>(i,0);
		nu = norm(y - D.col(i)*coef.at<double>(i,0));
		de = sqrt(pow(coef.at<double>(i,0),2));
		res.at<double>(0,i) = nu/de;
		if(res.at<double>(0,i) < minDist){
			minDist = res.at<double>(0,i);
			idx = i;
		}
	}
	minClass = _labelMat.at<int>(0,idx);
	minDist = coef.at<double>(idx,0);
}

int CRClassifier::predict(InputArray src) const {
   return 0;
}

void CRClassifier::load(const FileStorage& fs) {
    //read matrices
	fs["Dictionary"] >> _dicMat;
    fs["Labels"] >> _labelMat;
}

void CRClassifier::save(FileStorage& fs) const {
    // write matrices
	fs << "Dictionary" << _dicMat;
	fs << "Labels" << _labelMat;
}

}