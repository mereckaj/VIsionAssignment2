//
// Created by mereckaj on 11/19/15.
//

#ifndef VISIONASSIGNMENT2_LINEDETECTOR_HPP
#define VISIONASSIGNMENT2_LINEDETECTOR_HPP

#include <opencv2/core/mat.hpp>

class LineDetector{
public:
    LineDetector();
    LineDetector(cv::Mat dots);
    ~LineDetector();
    cv::Mat HoughLines();
    cv::Mat DrawLines(cv::Mat dest, std::vector<cv::Vec2f> lines);
    cv::Mat LineFit(std::vector<cv::Point> dots,cv::Mat mImage);
    std::vector<cv::Point> FindClosePoints(cv::Mat dots, int direction, cv::Point);
private:
    cv::Mat mImage;
};
#endif //VISIONASSIGNMENT2_LINEDETECTOR_HPP
