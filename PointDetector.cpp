//
// Created by mereckaj on 11/17/15.
//

#include <opencv2/core/mat.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "PointDetector.h"
#include "Utils.h"
#include "Histogram.cpp"

PointDetector::~PointDetector() {
    mImage.~Mat();
}

PointDetector::PointDetector(cv::Mat mat) {
    PointDetector::mImage = mat;
}

cv::Mat PointDetector::Thinning(cv::Mat im) {
    /*
     * Source: http://opencv-code.com/quick-tips/implementation-of-thinning-algorithm-in-opencv/
     */
    im /= 255;
    cv::Mat prev = cv::Mat::zeros(im.size(), CV_8UC1);
    cv::Mat diff;
    do {
        PointDetector::ThinningIterator(im, 0);
        PointDetector::ThinningIterator(im, 1);
        cv::absdiff(im, prev, diff);
        im.copyTo(prev);
    }
    while (cv::countNonZero(diff) > 0);
    im *= 255;
    return im;
}

    /**
     * Perform one thinning iteration.
     * Normally you wouldn't call this function directly from your code.
     *
     * @param  im    Binary image with range = 0-1
     * @param  iter  0=even, 1=odd
     */
cv::Mat PointDetector::ThinningIterator(cv::Mat im, int iter) {
    /*
     * Source: http://opencv-code.com/quick-tips/implementation-of-thinning-algorithm-in-opencv/
     */
    cv::Mat marker = cv::Mat::zeros(im.size(), CV_8UC1);
    for (int i = 1; i < im.rows-1; i++)
    {
        for (int j = 1; j < im.cols-1; j++)
        {
            uchar p2 = im.at<uchar>(i-1, j);
            uchar p3 = im.at<uchar>(i-1, j+1);
            uchar p4 = im.at<uchar>(i, j+1);
            uchar p5 = im.at<uchar>(i+1, j+1);
            uchar p6 = im.at<uchar>(i+1, j);
            uchar p7 = im.at<uchar>(i+1, j-1);
            uchar p8 = im.at<uchar>(i, j-1);
            uchar p9 = im.at<uchar>(i-1, j-1);
            int A  = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
                     (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
                     (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
                     (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);
            int B  = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
            int m1 = iter == 0 ? (p2 * p4 * p6) : (p2 * p4 * p8);
            int m2 = iter == 0 ? (p4 * p6 * p8) : (p2 * p6 * p8);

            if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
                marker.at<uchar>(i,j) = 1;
        }
    }
    im &= ~marker;
}


cv::Mat PointDetector::BackProjectBluePixels(cv::Mat backProjectSample){
    cv::Mat backProjectSampleHLS;
    cvtColor(backProjectSample, backProjectSampleHLS, CV_BGR2HLS);
    ColourHistogram histogram3D(backProjectSampleHLS,8);
    histogram3D.NormaliseHistogram();
    cvtColor(mImage, mHlsImage, CV_BGR2HLS);
    cv::Mat back_projection_probabilities = histogram3D.BackProject(mHlsImage);
    back_projection_probabilities = StretchImage( back_projection_probabilities );
    return back_projection_probabilities;
}
cv::Mat PointDetector::ThresholdDilateErode(cv::Mat backProjectSample,int thresholdValue){
    cv::Mat binary,eroded, dilated;
    cv::threshold(mImage,binary,thresholdValue,256,CV_THRESH_BINARY);
    cv::erode(binary,eroded,cv::Mat());
    cv::dilate(eroded, dilated,cv::Mat());
    return dilated;
}