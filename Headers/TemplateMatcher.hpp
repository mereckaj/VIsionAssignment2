//
// Created by mereckaj on 11/21/15.
//

#ifndef VISIONASSIGNMENT2_TEMPLATEMATCHER_HPP
#define VISIONASSIGNMENT2_TEMPLATEMATCHER_HPP

#include <opencv2/core/mat.hpp>

class TemplateMatcher{
public:
    TemplateMatcher();
    TemplateMatcher(cv::Mat src,cv::Mat* pages, int pageCount);
    cv::Mat RemoveBlueLines(cv::Mat src);
    int Match();

    double mCor;
private:
    cv::Mat mImage,*mPages;
    int mPageCount;

    cv::Mat crep(cv::Mat mat);
};

#endif //VISIONASSIGNMENT2_TEMPLATEMATCHER_HPP
