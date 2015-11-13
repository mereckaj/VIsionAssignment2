//
// Created by mereckaj on 11/11/15.
//

#ifndef VISIONASSIGNMENT2_IMAGES_H
#define VISIONASSIGNMENT2_IMAGES_H

#include <cstddef>
#include <string>
#include <vector>

extern const std::string IMAGE_LOCATION;
extern const std::vector<std::string> VIEW_FILES;
extern const std::vector<std::string> PAGE_FILES;

std::vector<cv::Mat> LoadImages(const std::string imageLocation,
                                const std::vector<std::string> imageNames);
std::vector<cv::Mat> LoadImages(const std::string imageLocation,
                                const std::vector<std::string> imageNames,
                                const size_t imageCount);

#endif //VISIONASSIGNMENT2_IMAGES_H
