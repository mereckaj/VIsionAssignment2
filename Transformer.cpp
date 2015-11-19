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

Transformer::Transformer(cv::Mat src) : Transformer() {
    mImage = src;
}
std::vector<cv::Point> Transformer::WhiteToPoints(cv::Mat src){
    cv::Mat whitePx;
    cv::findNonZero(src,whitePx);
    std::vector<cv::Point> result(whitePx.total());
    for(size_t i = 0; i < whitePx.total();i++){
        result[i] = whitePx.at<cv::Point>(i);
    }
    return result;
}
/*
 * Return 4 points: right most, left most, top most and bottom most (In that order)
 */
std::vector<cv::Point> Transformer::FindCorners(cv::Mat dots){
    cv::Mat whitePx;
    cv::findNonZero(dots,whitePx);
    int highestX = 0, highestY =0,lowestX = std::numeric_limits<int>::max(),lowestY=std::numeric_limits<int>::max();
    std::vector<cv::Point> result(4);
    for (size_t i = 0; i < whitePx.total(); i++ ) {
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
    }
    return result;
}

cv::Mat Transformer::DrawCorners(cv::Mat src,std::vector<cv::Point> corners) {
    cv::Mat result;
    src.copyTo(result);
    cv::circle(result,corners[LEFT_MOST_POINT],5,cv::Scalar(0,0,255));
    cv::circle(result,corners[RIGHT_MOST_POINT],5,cv::Scalar(0,0,255));
    cv::circle(result,corners[TOP_MOST_POINT],5,cv::Scalar(0,0,255));
    cv::circle(result,corners[BOTTOM_MOST_POINT],5,cv::Scalar(0,0,255));
    cv::line(result,corners[LEFT_MOST_POINT],corners[BOTTOM_MOST_POINT],cv::Scalar(0,0,255));
    cv::line(result,corners[BOTTOM_MOST_POINT],corners[RIGHT_MOST_POINT],cv::Scalar(0,0,255));
    cv::line(result,corners[RIGHT_MOST_POINT],corners[TOP_MOST_POINT],cv::Scalar(0,0,255));
    cv::line(result,corners[TOP_MOST_POINT],corners[LEFT_MOST_POINT],cv::Scalar(0,0,255));
    return result;
}

/*
 * Given a set of white pixels that are believed to be blue
 * Given an element
 * Given a direction
 * Find the closest point from set to element looking in direction
 *  where direction is:
 *      if element is left most
 *          then above right
 *      if element is right most
 *          then bellow left
 *      if element is top most
 *          then bellow right
 *      if element is bottom most
 *          then above left
 */
cv::Point Transformer::FindClosestPoint(cv::Mat set, cv::Point element, int direction) {
    if(direction==LEFT_MOST_POINT){

    }else if(direction==RIGHT_MOST_POINT){

    }else if(direction==TOP_MOST_POINT){

    }else if(direction==BOTTOM_MOST_POINT){

    }
    return cv::Point_<int>();
}

cv::Mat Transformer::Draw(cv::Mat src,std::vector<cv::Point> points) {
    for(size_t i = 0; i < points.size();i++){
        cv::circle(src,points.at(i),5,cv::Scalar(0,0,255),-1);
    }
    return src;
}
