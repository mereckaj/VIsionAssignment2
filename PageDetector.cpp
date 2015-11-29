//
// Created by mereckaj on 11/18/15.
//

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "Headers/PageDetector.hpp"
#include "Headers/Utils.hpp"
#include "Histogram.cpp"

/*
 * Detect the rough area of where the white page is in the image
 */
PageDetector::PageDetector() {
}
PageDetector::PageDetector(cv::Mat src) : PageDetector() {
    mImage = src;
}
PageDetector::~PageDetector() {
}

/*
 * Take the original image
 * Split it into individual RGB channels
 * Threshold the red channel using OTSU's thresholding. This should get rid of the background really well.
 * Erode and dilate to make sure the noise is minimal
 * Return the result, it will be used as a mask.
 */
cv::Mat PageDetector::DetectPage(){
    cv::Mat colors[mImage.channels()],dme;
    cv::split(mImage,colors);
    cv::threshold(colors[0],dme,0,255,CV_THRESH_BINARY|CV_THRESH_OTSU);
    cv::erode(dme,dme,cv::Mat());
    cv::dilate(dme,dme,cv::Mat());
    return dme;
}

/*
 * Given the result of the above function apply it as a mask to each channel of the original image.
 * This should make sure that only the white page is left in the original image, reducing noise.
 */
cv::Mat PageDetector::ApplyMask(cv::Mat src, cv::Mat mask) {
    cv::Mat result;
    std::vector<cv::Mat> channels((unsigned long) src.channels());

    cv::Mat *masked = new cv::Mat[src.channels()];
    cv::split(src,channels);
    for(size_t i = 0; i < channels.size();i++){
        channels[i].copyTo(masked[i],mask);
    }
    cv::merge(masked, (size_t) src.channels(),result);
    delete [] masked;
    return result;
}
