//
// Created by mereckaj on 11/18/15.
//

#ifndef VISIONASSIGNMENT2_PAGEDETECTOR_H
#define VISIONASSIGNMENT2_PAGEDETECTOR_H

#include <opencv2/core/mat.hpp>

class PageDetector{
public:
    PageDetector();
    PageDetector(cv::Mat src);
    ~PageDetector();
    cv::Mat DetectPage();

    cv::Mat ApplyMask(cv::Mat src, cv::Mat mask);

private:
    cv::Mat mImage;
};
#endif //VISIONASSIGNMENT2_PAGEDETECTOR_H
