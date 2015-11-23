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


cv::Mat Transformer::Transform(cv::Mat src, std::vector<cv::Point> corners,std::vector<cv::Point> dst) {
    cv::Mat matrix;
    int x1 = corners[TOP_MOST_POINT].x;
    int y1 = corners[TOP_MOST_POINT].y;
    int x2 = corners[LEFT_MOST_POINT].x;
    int y2 = corners[LEFT_MOST_POINT].y;
    int x3 = corners[BOTTOM_MOST_POINT].x;
    int y3 = corners[BOTTOM_MOST_POINT].y;
    int x4 = corners[RIGHT_MOST_POINT].x;
    int y4 = corners[RIGHT_MOST_POINT].y;
    cv::Mat result;
    cv::Point2f srcTransf[4],dstTransf[4];

    srcTransf[0] = cv::Point2f(x1,y1);
    srcTransf[1] = cv::Point2f(x2,y2);
    srcTransf[2] = cv::Point2f(x3,y3);
    srcTransf[3] = cv::Point2f(x4,y4);

    dstTransf[2] = cv::Point2f(dst[0].x,dst[0].y);
    dstTransf[1] = cv::Point2f(dst[1].x, dst[1].y);
    dstTransf[0] = cv::Point2f(dst[2].x, dst[2].y);
    dstTransf[3] = cv::Point2f(dst[3].x,dst[3].y);
    matrix = cv::getPerspectiveTransform(srcTransf,dstTransf);
    cv::warpPerspective(src,result,matrix,result.size());

    return result;
}
cv::Mat Transformer::UnsharpMask(cv::Mat im)
{
    cv::Mat tmp;
    cv::GaussianBlur(im, tmp, cv::Size(5,5), 5);
    cv::addWeighted(im, 1.5, tmp, -0.5, 0, im);
    return im;
}

std::vector<cv::Point> Transformer::FindTemplateCorners(cv::Mat dots) {
    cv::Mat whitePx;
    cv::findNonZero(dots,whitePx);
    int highestX = 0, highestY =0,lowestX = std::numeric_limits<int>::max(),lowestY=std::numeric_limits<int>::max();
    std::vector<cv::Point> result;
    for (size_t i = 0; i < whitePx.total(); i++ ) {
        cv::Point p = whitePx.at<cv::Point>((int) i);
        int x = p.x,y=p.y;
        if(x>highestX){
            highestX = x;
        }
        if(x < lowestX){
            lowestX = x;
        }
        if(y> highestY){
            highestY = y;
        }
        if(y < lowestY){
            lowestY = y;
        }
    }
    result.push_back(cv::Point(lowestX,lowestY));
    result.push_back(cv::Point(lowestX,highestY));
    result.push_back(cv::Point(highestX,highestY));
    result.push_back(cv::Point(highestX,lowestY));
    return result;
}
cv::Mat Transformer::Crop(cv::Mat src, cv::Mat ref){
    cv::Mat res;
    cv::Rect cropRect(0,0,ref.cols,ref.rows);
    res = src(cropRect);
    return res;
}

cv::Mat Transformer::Sharpen(cv::Mat src) {
    cv::Mat img32,lap,sharp,res;
    src.convertTo(img32,CV_32F);
    cv::Laplacian(src,lap,CV_32F,3);
    sharp = img32 - (0.3*lap);
    sharp.convertTo(res,CV_8U);
    return res;
}
