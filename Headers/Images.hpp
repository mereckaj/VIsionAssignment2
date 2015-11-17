//
// Created by mereckaj on 11/11/15.
//

#ifndef VISIONASSIGNMENT2_IMAGES_H
#define VISIONASSIGNMENT2_IMAGES_H
cv::Mat * LoadImages(const std::string imageLocation, const std::vector<std::string> imageNames);
void DisplayImage(std::string title,cv::Mat img,bool block);
#endif //VISIONASSIGNMENT2_IMAGES_H
