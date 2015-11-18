//
// Created by mereckaj on 11/17/15.
//

#ifndef VISIONASSIGNMENT2_UTILS_H
#define VISIONASSIGNMENT2_UTILS_H


cv::Mat StretchImage( cv::Mat& image );
cv::Mat JoinImagesHorizontally( cv::Mat& image1, cv::Mat& image2, int spacing);
void debugMessage(std::string s);
void ShowImage(std::string title,cv::Mat src);
void ShowImage(std::string title,cv::Mat src,cv::Mat src2);
void ShowImage(std::string title,cv::Mat src,cv::Mat src2,cv::Size scale);
#endif //VISIONASSIGNMENT2_UTILS_H
