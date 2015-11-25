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
//    cv::resize(mImage,mImage,cv::Size(mImage.size().width/2,mImage.size().height/2),0,0,CV_INTER_NN);
}
PageDetector::~PageDetector() {
}
cv::Mat PageDetector::DetectPage(){
    cv::Mat flood,colors[mImage.channels()],dme;
    cv::split(mImage,colors);
    cv::threshold(colors[0],dme,0,255,CV_THRESH_BINARY|CV_THRESH_OTSU);
    cv::erode(dme,dme,cv::Mat());
    cv::dilate(dme,dme,cv::Mat());
    /*
     * TODO: Connected components and FloodFill to make sure full page is found
     */
//    cv::floodFill(dme,cv::Point(1,1),cv::Scalar(0));
    return dme;
}

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
