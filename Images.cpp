//
// Created by mereckaj on 11/11/15.
//

#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include "Images.h"


cv::Mat* LoadImages(char* imageLocation,char*imageNames[],int imageCount){
    cv::Mat *images = new cv::Mat[imageCount];
    for (int i = 0; i < imageCount; i++) {
        std::string filename(IMAGE_LOCATION);
        filename.append(imageNames[i]);
        images[i] = cv::imread(filename, cv::IMREAD_ANYCOLOR);
        if (images[i].empty()) {
            std::cerr << "Failed to load image: " << filename << std::endl;
            exit(1);
        }
        return images;
    }
}