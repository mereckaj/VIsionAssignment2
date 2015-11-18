//
// Created by mereckaj on 11/18/15.
//

#include <opencv2/core/mat.hpp>
#include <opencv2/core.hpp>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include "Headers/Transformer.hpp"
#include "Headers/Utils.hpp"
#define RIGHT_MOST_POINT 0
#define LEFT_MOST_POINT 1
#define TOP_MOST_POINT 2
#define BOTTOM_MOST_POINT 3
Transformer::Transformer() {

}
Transformer::~Transformer() {

}

Transformer::Transformer(cv::Mat src, std::vector <std::vector<cv::Point>> points) : Transformer() {
    mImage = src;
    mPoints = points;
}

/*
 * Return 4 points: right most, left most, top most and bottom most (In that order)
 */
std::vector<cv::Point> Transformer::FindCorners(cv::Mat dots){
    cv::Mat whitePx;
    cv::findNonZero(dots,whitePx);
//    ShowImage("Dots",dots);
    int highestX = 0, highestY =0,lowestX = std::numeric_limits<int>::max(),lowestY=std::numeric_limits<int>::max();
    std::vector<cv::Point> result(4);
    for (int i = 0; i < whitePx.total(); i++ ) {
        cv::Point p = whitePx.at<cv::Point>(i);
        int x = p.x,y=p.y;
        if(x>highestX){
            highestX = x;
            result[RIGHT_MOST_POINT] = p;
        }
        if(x < lowestX){
            lowestX = x;
            result[LEFT_MOST_POINT] = p;
        }
        if(y> highestY){
            highestY = y;
            result[TOP_MOST_POINT] = p;
        }
        if(y < lowestY){
            lowestY = y;
            result[BOTTOM_MOST_POINT] = p;
        }
//        std::cout << "Zero#" << i << ": " << whitePx.at<cv::Point>(i).x << ", " << whitePx.at<cv::Point>(i).y << std::endl;
    }

//    for(size_t x = 0; x < whitePx.rows; x++){
//        for(size_t y = 0; whitePx.cols;y++){
//            int t = whitePx.at<int>(x,y);
//            debugMessage("(" + std::to_string(x)+","+ std::to_string(x)+")="+ std::to_string(t));
//        }
//    }
    return result;
}

cv::Mat Transformer::DrawCorners(cv::Mat src,std::vector<cv::Point> corners) {
    cv::Mat result;
    cv::circle(src,corners[LEFT_MOST_POINT],5,cv::Scalar(0,0,255));
    cv::circle(src,corners[RIGHT_MOST_POINT],5,cv::Scalar(0,0,255));
    cv::circle(src,corners[TOP_MOST_POINT],5,cv::Scalar(0,0,255));
    cv::circle(src,corners[BOTTOM_MOST_POINT],5,cv::Scalar(0,0,255));
    ShowImage("Dots",src);
    return result;
}
