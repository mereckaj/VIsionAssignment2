//
// Created by mereckaj on 11/17/15.
//

#include <opencv2/core/mat.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "Headers/PointDetector.hpp"
#include "Headers/Utils.hpp"
#include "Histogram.cpp"

PointDetector::~PointDetector() {
    //TODO: implement
}
PointDetector::PointDetector(){

}
PointDetector::PointDetector(cv::Mat srcImage) : PointDetector() {
    mImage = srcImage;
}
cv::Mat PointDetector::DetectPoints(){
    //TODO: implement
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
cv::Mat PointDetector::BackProjectBluePixels(cv::Mat backProjectSample,int binCount){
    cv::Mat backProjectSampleHLS;

    //Convert back projection sample to HLS
    cvtColor(backProjectSample, backProjectSampleHLS, CV_BGR2HLS);
    //Create a 3D historgram of back projection sample
    ColourHistogram histogram3D(backProjectSampleHLS,binCount);
    histogram3D.NormaliseHistogram();

    //Convert the image to HLS
    cvtColor(mImage, mHlsImage, CV_BGR2HLS);

    //Back project the sample onto the image
    cv::Mat backProjectionProbabilities = histogram3D.BackProject(mHlsImage);
    backProjectionProbabilities = StretchImage(backProjectionProbabilities);

    //Return the probability matrix
    return backProjectionProbabilities;
}
cv::Mat PointDetector::ThresholdDilateErode(int thresholdValue){

    //Threshold the image
    cv::threshold(mImage,mBinary,thresholdValue,256,CV_THRESH_BINARY);

    //Erode the image
    cv::erode(mBinary,mEroded,cv::Mat());

    //Dilate the image
    cv::dilate(mEroded, mDilated,cv::Mat());

    //Return the dilated image matrix
    return mDilated;
}
void PointDetector::Show(cv::Mat img){
    if(img.empty()){
        std::cerr << "Attempted to show an empty image" << std::endl;
    }else {
        cv::imshow(mWindowTitle, img);
        cv::moveWindow(mWindowTitle, 0, 0);
        cv::waitKey(0);
    }
}