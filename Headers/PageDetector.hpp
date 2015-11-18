//
// Created by mereckaj on 11/18/15.
//

#ifndef VISIONASSIGNMENT2_PAGEDETECTOR_H
#define VISIONASSIGNMENT2_PAGEDETECTOR_H

#include <opencv2/core/mat.hpp>

class PageDetector{
public:
    PageDetector();
    PageDetector(cv::Mat);
    cv::Mat DetectPage();
    cv::Mat DetectPage(cv::Mat);
    cv::Mat BackProjectWhitePixels(cv::Mat mBackProjectionSample, int binCount);
    void Show(cv::Mat img);
    cv::Mat mImage;

    cv::Mat ApplyMask(cv::Mat src);

};
#endif //VISIONASSIGNMENT2_PAGEDETECTOR_H
