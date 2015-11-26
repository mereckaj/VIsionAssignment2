//
// Created by mereckaj on 11/19/15.
//

#include <opencv2/imgproc.hpp>
#include <iostream>
#include "Headers/LineDetector.hpp"
#include "Headers/Utils.hpp"
#define RIGHT_MOST_POINT 0
#define LEFT_MOST_POINT 1
#define TOP_MOST_POINT 2
#define BOTTOM_MOST_POINT 3
LineDetector::LineDetector() {
}
LineDetector::LineDetector(cv::Mat dots) : LineDetector() {
    mImage = dots;
}
LineDetector::~LineDetector() {
}
cv::Mat LineDetector::DrawLines(cv::Mat cdst, std::vector<cv::Vec2f> lines){

    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        cv::Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        line( cdst, pt1, pt2, cv::Scalar(0,0,255), 3, CV_AA);
    }
    return cdst;
}
cv::Mat LineDetector::HoughLines(){
    cv::Mat result,mImageCanny;
    std::vector<cv::Vec2f> lines;
    cv::Canny(mImage,mImageCanny,50,200,3);
//    cv::cvtColor(mImageCanny,mImageCanny,CV_BGR2GRAY);
    cv::HoughLines(mImageCanny,lines,1,CV_PI/180,0,0);
    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        cv::Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        line( mImage, pt1, pt2, cv::Scalar(0,0,255), 3, CV_AA);
    }
    return result;
}
std::vector<cv::Point> LineDetector::FindClosePoints(cv::Mat dots,int direction,std::vector<cv::Point> corners){
    cv::Mat whitePx;
    cv::findNonZero(dots,whitePx);
    cv::Point referencePoint, currentPoint;
    std::vector<cv::Point> found;
    switch(direction){
        case RIGHT_MOST_POINT:
            referencePoint = corners[RIGHT_MOST_POINT];
            for(int i = 0 ; i < whitePx.total();i++) {
                // Look for points above and to the left
                currentPoint = whitePx.at<cv::Point>(i);
                if(currentPoint != referencePoint) {
                    if (currentPoint.x < referencePoint.x && currentPoint.y < referencePoint.y
                        && currentPoint.x < corners[TOP_MOST_POINT].x) {
                        found.push_back(currentPoint);
                    }
                }
            }
            break;
        case LEFT_MOST_POINT:
            for(int i = 0 ; i < whitePx.total();i++) {
                // Look for points above and to the left
                currentPoint = whitePx.at<cv::Point>((int) i);
                if(currentPoint != referencePoint) {
                    if (currentPoint.x > referencePoint.x && currentPoint.y > referencePoint.y
                        && currentPoint.x < corners[BOTTOM_MOST_POINT].x) {
                        found.push_back(currentPoint);
                    }
                }
            }
            break;
        case TOP_MOST_POINT:
            for(int i = 0 ; i < whitePx.total();i++) {
                // Look for points above and to the left
                currentPoint = whitePx.at<cv::Point>((int) i);
                if(currentPoint != referencePoint) {
                    if (currentPoint.x < referencePoint.x && currentPoint.y > referencePoint.y
                        && currentPoint.y < corners[LEFT_MOST_POINT].y) {
                        found.push_back(currentPoint);
                    }
                }
            }
            break;
        case BOTTOM_MOST_POINT:
            for(int i = 0 ; i < whitePx.total();i++) {
                // Look for points above and to the left
                currentPoint = whitePx.at<cv::Point>((int) i);
                if(currentPoint != referencePoint) {
                    if (currentPoint.x > referencePoint.x && currentPoint.y < referencePoint.y
                        && currentPoint.y < corners[RIGHT_MOST_POINT].y) {
                        found.push_back(currentPoint);
                    }
                }
            }
            break;
        default:
            std::cerr << "Bad direction" << std::endl;
            exit(EXIT_FAILURE);
    }
    return found;
}
cv::Mat LineDetector::LineFit(std::vector<cv::Point> dots,cv::Mat mImage){
    cv::Mat output, whitePx;
    cv::findNonZero(dots, whitePx);
    std::vector<cv::Point> whitePoints;
    for(size_t i = 0; i < whitePx.total();i++){
        whitePoints.push_back(whitePx.at<cv::Point>((int) i));
    }
    cv::fitLine(whitePoints,output,CV_DIST_L1,2,0.01,0.01);
    cv::Vec4f t = output.at<cv::Vec4f>(0);
    cv::line(mImage,cv::Point((int) t[2], (int) t[3]),cv::Point((int) (t[2]+t[0]*1000), (int) (t[3]+t[1]*1000)),cv::Scalar(0,0,255));
    cv::line(mImage,cv::Point((int) t[2], (int) t[3]),cv::Point((int) (t[2]+t[0]*-1000), (int) (t[3]+t[1]*-1000)),cv::Scalar(0,0,255));
    cv::circle(mImage,cv::Point((int) t[2], (int) t[3]),4,cv::Scalar(0,0,255));
//    ShowImage("",mImage);
    return output;
}