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
    int Match();
private:
    cv::Mat mImage,*mPages;
    int mPageCount;
};

#endif //VISIONASSIGNMENT2_TEMPLATEMATCHER_HPP
