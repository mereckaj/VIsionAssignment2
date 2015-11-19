//
// Created by mereckaj on 11/18/15.
//

#ifndef VISIONASSIGNMENT2_TRANSFORMER_H
#define VISIONASSIGNMENT2_TRANSFORMER_H

#include <opencv2/core/mat.hpp>

class Transformer{
public:
    Transformer();
    Transformer(cv::Mat src);
    ~Transformer();
    std::vector<cv::Point> FindCorners(cv::Mat);
    std::vector<cv::Point> WhiteToPoints(cv::Mat src);
    cv::Mat Draw(cv::Mat src,std::vector<cv::Point> vector);

    cv::Mat Transform(cv::Mat src, std::vector<cv::Point> corners);

private:
    cv::Mat mImage;
    std::vector<std::vector<cv::Point>> mPoints;

};
#endif //VISIONASSIGNMENT2_TRANSFORMER_H
