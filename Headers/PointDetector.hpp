//
// Created by mereckaj on 11/17/15.
//

#ifndef VISIONASSIGNMENT2_POINTDETECTOR_H
#define VISIONASSIGNMENT2_POINTDETECTOR_H

//TODO: Make point detector take an image and produce a set of points
class PointDetector{
public:
    PointDetector();
    PointDetector(cv::Mat,cv::Mat,int,std::string);
    ~PointDetector();
    void Show(cv::Mat img);
    std::vector<std::vector<cv::Point>> DetectPoints();

private:
    cv::Mat mImage,mHlsImage,mBackProjectSampleHLS,mBinary,mDilated,mEroded,mBackProjectionSample,mThin;
    int mThresholdValue,mBinCount;
    std::string mWindowTitle;
    cv::Mat ThinningIterator(cv::Mat,int);
    cv::Mat ErodeDilate(cv::Mat);
    cv::Mat DilateErode(cv::Mat);
    cv::Mat Thinning(cv::Mat);
    cv::Mat BackProjectBluePixels(int);
    cv::Mat Threshold(cv::Mat);

};

#endif //VISIONASSIGNMENT2_POINTDETECTOR_H
