//
// Created by mereckaj on 11/17/15.
//

#ifndef VISIONASSIGNMENT2_POINTDETECTOR_H
#define VISIONASSIGNMENT2_POINTDETECTOR_H

class PointDetector{
public:
    PointDetector(cv::Mat);
    ~PointDetector();
    cv::Mat Thinning(cv::Mat);
private:
    cv::Mat mImage,mHlsImage;
    cv::Mat ThinningIterator(cv::Mat,int);
    cv::Mat BackProjectBluePixels(cv::Mat backProjectSample);

    cv::Mat ThresholdDilateErode(cv::Mat backProjectSample, int thresholdValue);
};

#endif //VISIONASSIGNMENT2_POINTDETECTOR_H
