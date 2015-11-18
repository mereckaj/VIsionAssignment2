//
// Created by mereckaj on 11/18/15.
//

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "Headers/PageDetector.hpp"
#include "Headers/Utils.hpp"
#include "Histogram.cpp"

PageDetector::PageDetector() {
}
PageDetector::PageDetector(cv::Mat src) : PageDetector() {
    mImage = src;
    cv::resize(mImage,mImage,cv::Size(mImage.size().width/2,mImage.size().height/2),0,0,CV_INTER_NN);
}
cv::Mat PageDetector::DetectPage(cv::Mat backProjSample){
    cv::Mat grayImage,blured,result,dilated,eroded,dme,bin,red;
    cv::Mat colors[mImage.channels()];
    cv::split(mImage,colors);
    cv::threshold(colors[0],dme,0,255,CV_THRESH_BINARY|CV_THRESH_OTSU);
    cv::erode(dme,dme,cv::Mat());
    cv::dilate(dme,dme,cv::Mat());
//    cv::cvtColor(mImage,grayImage,CV_BGR2GRAY);
//    cv::medianBlur(grayImage,blured,5);
//    cv::dilate(blured,dilated,cv::Mat());
//    cv::erode(dilated,eroded,cv::Mat());
//    dme = dilated - eroded;
//    cv::threshold(dme,bin,0,255,CV_THRESH_BINARY|CV_THRESH_OTSU);
//    Show(bin);
    return dme;
}
void PageDetector::Show(cv::Mat img){
    cv::imshow("",img);
    cv::moveWindow("",50,50);
    cv::waitKey(0);
}

cv::Mat PageDetector::ApplyMask(cv::Mat mask) {
    cv::Mat res;
    mImage.copyTo(res,mask);
    return res;
}