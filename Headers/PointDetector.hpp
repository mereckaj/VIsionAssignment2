//
// Created by mereckaj on 11/17/15.
//

#ifndef VISIONASSIGNMENT2_POINTDETECTOR_H
#define VISIONASSIGNMENT2_POINTDETECTOR_H

//TODO: Make point detector take an image and produce a set of points
class PointDetector{
public:
    PointDetector();
    PointDetector(cv::Mat srcImage,int thresholdValue,std::string windowTitle);
    ~PointDetector();
    cv::Mat DetectPoints(cv::Mat backProjectSample);
    cv::Mat DrawContours(std::vector<std::vector<cv::Point>> contours, cv::Mat src);
    cv::Mat Threshold(cv::Mat);
    cv::Mat BackProjectBluePixels(cv::Mat backProjectionSample, int binCount);
    std::vector<std::vector<cv::Point>> DotsToPoints(cv::Mat src);
private:
    cv::Mat mImage;
    int mBinCount,mThresholdValue;
    std::string mWindowTitle;
    cv::Mat ThinningIterator(cv::Mat,int);
    cv::Mat ErodeDilate(cv::Mat);
    cv::Mat DilateErode(cv::Mat);
    cv::Mat Thinning(cv::Mat);

};

#endif //VISIONASSIGNMENT2_POINTDETECTOR_H
