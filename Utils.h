//
// Created by mereckaj on 11/17/15.
//

#ifndef VISIONASSIGNMENT2_UTILS_H
#define VISIONASSIGNMENT2_UTILS_H


cv::Mat StretchImage( cv::Mat& image );
cv::Mat JoinImagesHorizontally( cv::Mat& image1, cv::Mat& image2, int spacing, cv::Scalar passed_colour/*=-1.0*/ );
void debugMessage(std::string s);
#endif //VISIONASSIGNMENT2_UTILS_H
