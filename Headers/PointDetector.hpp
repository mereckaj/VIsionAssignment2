//
// Created by mereckaj on 11/17/15.
//

#ifndef VISIONASSIGNMENT2_POINTDETECTOR_H
#define VISIONASSIGNMENT2_POINTDETECTOR_H

class PointDetector{
public:
    PointDetector(cv::Mat srcImage, cv::Mat backProjectionSample);
    ~PointDetector();
    cv::Mat DetectPoints();
private:
    cv::Mat mImage,mBackProjectionProbabilty,mHlsImage,mBackProjectionSample,mThin;
    cv::Mat Thinning(cv::Mat);
    cv::Mat ThinningIterator(cv::Mat,int);
    cv::Mat BackProjectBluePixels(cv::Mat backProjectSample);
    cv::Mat ThresholdDilateErode(cv::Mat backProjectSample, int thresholdValue);
};

#endif //VISIONASSIGNMENT2_POINTDETECTOR_H
