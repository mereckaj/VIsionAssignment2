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
    // Use a magical bin count value. The higher the bin count the worse the algorithm works
    mBinCount = 4;
}

PointDetector::PointDetector(cv::Mat srcImage,int thresholdValue,std::string windowTitle) : PointDetector() {
    mImage = srcImage;
    mThresholdValue = thresholdValue;
    mWindowTitle = windowTitle;
}

/*
 * Given a back projection sample back project it onto the image passed via the constructor
 * Threshold the image and open and close it to get rid of some of the noise.
 *
 * This should return the possible blue dot positions
 */
cv::Mat PointDetector::DetectPoints(cv::Mat backProjectSample){
    cv::Mat backProjectProb,binary,ed,d2;
    backProjectProb= BackProjectBluePixels(backProjectSample,mBinCount);
    binary = Threshold(backProjectProb);
    ed = Close(binary);
    d2 = Open(ed);
    return d2;
}

/*
 * Given a binary image of possible locations of blue dots, perform the connected components operation
 *
 * This should return a vector of points that are part of a component, can then use it to find moments
 * of these components and determine where the centre of the component is
 *
 * Without this step line fitting works but with some glitches
 *
 * Return a vector of vectors of points that form a component
 */
std::vector<std::vector<cv::Point>> PointDetector::BlueDotsToContours(cv::Mat src){
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(src,contours,CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
    return contours;
}
/*
 * Given an image and a back projection sample and a number of bins (Which is always 4 in this case, as define in the
 * constructor) this function will perform the back projection
 *
 * Return a probability matrix
 */
cv::Mat PointDetector::BackProjectBluePixels(cv::Mat backProjectionSample,int binCount){
    cv::Mat backProjectSampleHLS, hlsImage;

    //Convert back projection sample to HLS
    cv::cvtColor(backProjectionSample, backProjectSampleHLS, CV_BGR2HLS);

    //Create a 3D histogram of back projection sample
    ColourHistogram histogram3D(backProjectSampleHLS,binCount);
    histogram3D.NormaliseHistogram();

    //Convert the image to HLS
    cvtColor(mImage, hlsImage, CV_BGR2HLS);

    //Back project the sample onto the image
    cv::Mat backProjectionProbabilities = histogram3D.BackProject(hlsImage);
    backProjectionProbabilities = StretchImage(backProjectionProbabilities);

    //Return the probability matrix
    return backProjectionProbabilities;
}

/*
 * Threshold the image, nothing more.
 *
 * Cleaner wrapper since OpenCV has terrible documentation that is confusing
 */
cv::Mat PointDetector::Threshold(cv::Mat probMatrix){
    cv::Mat mBinary;
    //Threshold the image;
    cv::threshold(probMatrix,mBinary,mThresholdValue,256,CV_THRESH_BINARY);
    return mBinary;
}

/*
 * Again, wrappers
 */
cv::Mat PointDetector::Close(cv::Mat src){
    cv::Mat mEroded,mDilated;

    //Erode the image
    cv::erode(src,mEroded,cv::Mat());

    //Dilate the image
    cv::dilate(mEroded, mDilated,cv::Mat());

    //Return the dilated image matrix
    return mDilated;
}

/*
 * Once more, wrappers
 */
cv::Mat PointDetector::Open(cv::Mat src){
    cv::Mat mEroded,mDilated;

    //Dilate the image
    cv::dilate(src, mDilated,cv::Mat());

    //Erode the image
    cv::erode(mDilated,mEroded,cv::Mat());

    //Return the dilated image matrix
    return mEroded;
}

/*
 * Given a vector of vectors of points that form a component find where the centres are.
 *
 * Calculate it by getting the moments of each component
 * Code straight out Compton ... I mean straight out of of the OpenCV website.
 */
std::vector<cv::Point> PointDetector::GetCentres(std::vector<std::vector<cv::Point>> contours) {

    // Get moments
    std::vector<cv::Moments> moments(contours.size());
    for (size_t i = 0; i < contours.size();i++){
        moments[i] = cv::moments( contours[i], false );
    }
    // Get centres
    std::vector<cv::Point> mc( contours.size() );
    for( int i = 0; i < contours.size(); i++ ) {
        mc[i] = cv::Point((int) (moments[i].m10/moments[i].m00), (int) (moments[i].m01/moments[i].m00));
    }

    return mc;
}