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
    for(int i = 0; i < whitePx.total();i++){
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
        cv::Point p = whitePx.at<cv::Point>((int) i);
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

cv::Mat Transformer::Draw(cv::Mat src,std::vector<cv::Point> points) {
//    for(size_t i = 0; i < points.size();i++){
        cv::circle(src,points.at(0),10,cv::Scalar(0,0,255),-1);
        cv::circle(src,points.at(1),10,cv::Scalar(0,255,0),-1);
        cv::circle(src,points.at(2),10,cv::Scalar(255,0,0),-1);
        cv::circle(src,points.at(3),10,cv::Scalar(0,255,255),-1);
//    }
    return src;
}
int dst(int x1, int y1, int x2, int y2){
    int xs = x2-x1;
    int ys = y2-y1;
    int distance = (int) std::sqrt((xs*xs)+(ys*ys));
    return distance;
}


cv::Mat Transformer::Transform(cv::Mat src, std::vector<cv::Point> corners) {
    cv::Mat matrix;
    int x1 = corners[TOP_MOST_POINT].x;
    int y1 = corners[TOP_MOST_POINT].y;
    int x2 = corners[LEFT_MOST_POINT].x;
    int y2 = corners[LEFT_MOST_POINT].y;
    int x3 = corners[BOTTOM_MOST_POINT].x;
    int y3 = corners[BOTTOM_MOST_POINT].y;
    int x4 = corners[RIGHT_MOST_POINT].x;
    int y4 = corners[RIGHT_MOST_POINT].y;
//    int height1 = dst(x1,y1,x2,y2);
//    int height2 = dst(x4,y4,x3,y3);
//    int width1 = dst(x1,y1,x4,y4);
//    int width2 = dst(x2,y2,x3,y3);
    int width1 = 408;
    int height1 = 598;
//    debugMessage(std::to_string(height1) +"x" + std::to_string(width1));
//    debugMessage(std::to_string(height2) +"x" + std::to_string(width2));
//    debugMessage("-----------------------------");
//    if(height1 < height2){
//        height1 = height2;
//    }
//    if(width1 < width2){
//        width1 = width2;
//    }
    cv::Mat result(height1,width1,src.type());
    cv::Point2f srcTransf[4],dstTransf[4];

    srcTransf[0] = cv::Point2f(x1,y1);
    srcTransf[1] = cv::Point2f(x2,y2);
    srcTransf[2] = cv::Point2f(x3,y3);
    srcTransf[3] = cv::Point2f(x4,y4);

    dstTransf[2] = cv::Point2f(0,0);//x1
    dstTransf[1] = cv::Point2f(0, height1);//x2
    dstTransf[0] = cv::Point2f(width1, height1);//x3
    dstTransf[3] = cv::Point2f(width1,0);//x4

//    matrix = cv::getAffineTransform(srcTransf,dstTransf);
//    cv::warpAffine(src, result,matrix, result.size());
    matrix = cv::getPerspectiveTransform(srcTransf,dstTransf);
    cv::warpPerspective(src,result,matrix,result.size());

    return result;
}
