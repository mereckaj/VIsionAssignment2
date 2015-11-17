//
// Created by mereckaj on 11/17/15.
//

#ifndef VISIONASSIGNMENT2_POINTDETECTOR_H
#define VISIONASSIGNMENT2_POINTDETECTOR_H

class PointDetector{
public:
    PointDetector();

    PointDetector(cv::Mat srcImage);

    ~PointDetector();
    cv::Mat DetectPoints();
    void Show(cv::Mat img);
private:
    cv::Mat mImage,mHlsImage,mBinary,mDilated,mEroded;
    std::string mWindowTitle;
    cv::Mat ThinningIterator(cv::Mat,int);
    cv::Mat ThresholdDilateErode(int thresholdValue);
    cv::Mat Thinning(cv::Mat);
    cv::Mat BackProjectBluePixels(cv::Mat backProjectSample, int binCount);
};

#endif //VISIONASSIGNMENT2_POINTDETECTOR_H
