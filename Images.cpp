//
// Created by mereckaj on 11/11/15.
//

#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <opencv2/highgui.hpp>
#include "Headers/Images.hpp"
#include "Headers/Utils.hpp"
cv::Mat * LoadImages(const std::string imageLocation,std::vector<std::string> imageNames){

    debugMessage("Loading: " + std::to_string(imageNames.size()) + " images from: " + imageLocation);

    cv::Mat *images = new cv::Mat[imageNames.size()];
    for (int i = 0; i < imageNames.size(); i++) {

        std::string filename(imageLocation);
        filename.append(imageNames[i]);
        images[i] = cv::imread(filename, cv::IMREAD_ANYCOLOR);

        if (images[i].empty()) {
            std::cerr << "Failed to load image: " << filename << std::endl;
            exit(EXIT_FAILURE);
        }else{

            std::cout << "Successfully loaded image: " << filename << std::endl;
        }
    }
    return images;
}

void DisplayImage(std::string title,cv::Mat img,bool block){
    cv::imshow(title,img);
    cv::moveWindow(title,0,0);
    if(block){
        cv::waitKey(0);
    }
}