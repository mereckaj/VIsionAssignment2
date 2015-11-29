/*
 * Code t
 */

#include <opencv2/imgproc.hpp>
#include "Headers/TemplateMatcher.hpp"
#include "Headers/Utils.hpp"
#define SCALE 2
TemplateMatcher::TemplateMatcher() {
}

TemplateMatcher::TemplateMatcher(cv::Mat src,cv::Mat* pages, int pageCount) : TemplateMatcher(){
    cv::Mat tmp2;

    /*
     * Resize the original image to half the size to increase the shpeeeeeed of the program (Have to go fast)
     */
    resize(src,tmp2,cv::Size((src.cols/SCALE), (src.rows/SCALE)));
    mImage = tmp2;
    mPages = pages;
    mPageCount = pageCount;
}
int TemplateMatcher::Match() {
    /*
     * Code straight out of KDH's examples for template matching, the only difference is that I don't draw
     * the red rectangles and I scale down the templates to half the size to match the size of the mImage
     */
    cv::Mat croppedTemplate,display_image, correlation_image,tmplt;
    int result_columns,result_rows;
    std::vector<double> correlationResults;
    double min_correlation, max_correlation;
    for(size_t i = 0; i < mPageCount; i++) {
        croppedTemplate = mPages[i];
        cv::resize(croppedTemplate, tmplt, cv::Size((croppedTemplate.cols / SCALE), (croppedTemplate.rows / SCALE)));
        mImage.copyTo(display_image);
        result_columns = mImage.cols - tmplt.cols + 1;
        result_rows = mImage.rows - tmplt.rows + 1;
        correlation_image.create(result_columns, result_rows, CV_32FC1);
        matchTemplate(mImage, tmplt, correlation_image, CV_TM_CCORR_NORMED);
        cv::minMaxLoc(correlation_image, &min_correlation, &max_correlation);
        correlationResults.push_back(max_correlation);
    }

    /*
     * Pick the highest correlation and find it's index, that + 1 is the result
     *
     * +1 is necessary, and done outside of this functions call, since ground truth
     * starts counting at 1 where as program starts at 0
     */
    int res = 0;
    for(size_t i = 0; i < correlationResults.size();i++){
        if(correlationResults[i] > correlationResults[res]){
            res = (int) i;
            mCor = correlationResults[res];
        }
    }
    /*
     * If the match is bellow 95% it's a bad match
     */
    if(correlationResults[res]<0.95){
        return -1;
    }
    return res;
}