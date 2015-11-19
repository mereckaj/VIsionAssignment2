//
// Created by mereckaj on 11/17/15.
//

#include <opencv2/core/mat.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <opencv2/photo.hpp>
#include "Headers/PointDetector.hpp"
#include "Headers/Utils.hpp"
#include "Histogram.cpp"
PointDetector::~PointDetector() {
    mImage.deallocate();
}
PointDetector::PointDetector(){
    mBinCount = 4;
}
PointDetector::PointDetector(cv::Mat srcImage,int thresholdValue,std::string windowTitle) : PointDetector() {
    mImage = srcImage;
    mThresholdValue = thresholdValue;
    mWindowTitle = windowTitle;
}
//std::vector<std::vector<cv::Point>> PointDetector::DetectPoints(cv::Mat backProjectSample){

cv::Mat PointDetector::DetectPoints(cv::Mat backProjectSample){
    cv::Mat backProjectProb,binary,thin,ed,d2,t2,lines,liness;
    backProjectProb= BackProjectBluePixels(backProjectSample,mBinCount);
    binary = Threshold(backProjectProb);
    ed = ErodeDilate(binary);
    thin = Thinning(ed);
    d2 = DilateErode(thin);
    t2 = Thinning(d2);
    return t2;
}
std::vector<std::vector<cv::Point>> PointDetector::DotsToPoints(cv::Mat src){
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(src,contours,CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
    return contours;
}
/**
 * Perform one thinning iteration.
 * Normally you wouldn't call this function directly from your code.
 *
 * @param  im    Binary image with range = 0-1
 * @param  iter  0=even, 1=odd
 */
cv::Mat PointDetector::ThinningIterator(cv::Mat image, int iter) {
    /*
     * Source: http://opencv-code.com/quick-tips/implementation-of-thinning-algorithm-in-opencv/
     */
    cv::Mat marker = cv::Mat::zeros(image.size(), CV_8UC1);
    for (int i = 1; i < image.rows-1; i++)
    {
        for (int j = 1; j < image.cols-1; j++)
        {
            uchar p2 = image.at<uchar>(i-1, j);
            uchar p3 = image.at<uchar>(i-1, j+1);
            uchar p4 = image.at<uchar>(i, j+1);
            uchar p5 = image.at<uchar>(i+1, j+1);
            uchar p6 = image.at<uchar>(i+1, j);
            uchar p7 = image.at<uchar>(i+1, j-1);
            uchar p8 = image.at<uchar>(i, j-1);
            uchar p9 = image.at<uchar>(i-1, j-1);
            int A  = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
                     (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
                     (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
                     (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);
            int B  = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
            int m1 = iter == 0 ? (p2 * p4 * p6) : (p2 * p4 * p8);
            int m2 = iter == 0 ? (p4 * p6 * p8) : (p2 * p6 * p8);

            if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0) {
                marker.at<uchar>(i, j) = 1;
            }
        }
    }
    image &= ~marker;
    return image;
}
cv::Mat PointDetector::Thinning(cv::Mat image) {
    /*
     * Source: http://opencv-code.com/quick-tips/implementation-of-thinning-algorithm-in-opencv/
     */
    image /= 255;
    cv::Mat prev = cv::Mat::zeros(image.size(), CV_8UC1);
    cv::Mat diff;
    do {
        ThinningIterator(image, 0);
        ThinningIterator(image, 1);
        cv::absdiff(image, prev, diff);
        image.copyTo(prev);
    }
    while (cv::countNonZero(diff) > 0);
    image *= 255;
    return image;
}
cv::Mat PointDetector::BackProjectBluePixels(cv::Mat backProjectionSample,int binCount){
    cv::Mat mBackProjectSampleHLS,mHlsImage;

    //Convert back projection sample to HLS
    cv::cvtColor(backProjectionSample, mBackProjectSampleHLS, CV_BGR2HLS);
    //Create a 3D historgram of back projection sample
    ColourHistogram histogram3D(mBackProjectSampleHLS,binCount);
    histogram3D.NormaliseHistogram();

    //Convert the image to HLS
    cvtColor(mImage, mHlsImage, CV_BGR2HLS);

    //Back project the sample onto the image
    cv::Mat backProjectionProbabilities = histogram3D.BackProject(mHlsImage);
    backProjectionProbabilities = StretchImage(backProjectionProbabilities);

    //Return the probability matrix
    return backProjectionProbabilities;
}
cv::Mat PointDetector::Threshold(cv::Mat probMatrix){
    cv::Mat mBinary;
    //Threshold the image;
    cv::threshold(probMatrix,mBinary,mThresholdValue,256,CV_THRESH_BINARY);
    return mBinary;
}
cv::Mat PointDetector::ErodeDilate(cv::Mat src){
    cv::Mat mEroded,mDilated;

    //Erode the image
    cv::erode(src,mEroded,cv::Mat());

    //Dilate the image
    cv::dilate(mEroded, mDilated,cv::Mat());

    //Return the dilated image matrix
    return mDilated;
}
cv::Mat PointDetector::DilateErode(cv::Mat src){
    cv::Mat mEroded,mDilated;

    //Dilate the image
    cv::dilate(src, mDilated,cv::Mat());

    //Erode the image
    cv::erode(mDilated,mEroded,cv::Mat());

    //Return the dilated image matrix
    return mEroded;
}