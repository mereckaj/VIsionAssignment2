//
// Created by mereckaj on 11/17/15.
//

#ifndef VISIONASSIGNMENT2_UTILS_H
#define VISIONASSIGNMENT2_UTILS_H


cv::Mat StretchImage( cv::Mat& image );
cv::Mat JoinImagesHorizontally(cv::Mat image1, cv::Mat image2, int spacing, int scale=0);
cv::Mat JoinImagesVertically( cv::Mat& image1,cv::Mat& image2, int spacing, int scale=0);
void debugMessage(std::string s);
void ShowImage(std::string title,cv::Mat src);
cv::Mat * LoadImages(const std::string imageLocation,std::vector<std::string> imageNames);
#endif //VISIONASSIGNMENT2_UTILS_H
