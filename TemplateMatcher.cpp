//
// Created by mereckaj on 11/21/15.
//

#include <opencv2/imgproc.hpp>
#include "Headers/TemplateMatcher.hpp"
#include "Headers/Utils.hpp"
#define SCALE 2
using namespace cv;
cv::Mat convert_32bit_image_for_display(cv::Mat& passed_image, double zero_maps_to=0.0, double passed_scale_factor=-1.0)
{
    cv::Mat display_image;
    double scale_factor = passed_scale_factor;
    if (passed_scale_factor == -1.0)
    {
        double minimum,maximum;
        minMaxLoc(passed_image,&minimum,&maximum);
        scale_factor = (255.0-zero_maps_to)/cv::max(-minimum,maximum);
    }
    passed_image.convertTo(display_image, CV_8U, scale_factor, zero_maps_to);
    return display_image;
}
void DrawMatchingTemplateRectangles( Mat& display_image, Mat& matched_template_map, Mat& template_image, Scalar passed_colour=-1.0 )
{
    int image_channels = matched_template_map.channels();
    int values_on_each_row = matched_template_map.cols;
    for (int row=0; row < matched_template_map.rows; row++) {
        uchar* curr_point = matched_template_map.ptr<uchar>(row);
        for (int column=0; column < values_on_each_row; column++)
        {
            if (*curr_point > 0)
            {
                Scalar colour( rand()&0xFF, rand()&0xFF, rand()&0xFF );
                Point location( column, row );
                rectangle( display_image, location, Point( column + template_image.cols , row + template_image.rows ), (passed_colour.val[0] == -1.0) ? colour : passed_colour, 1, 8, 0 );
            }
            curr_point += image_channels;
        }
    }
}
void FindLocalMaxima( Mat& input_image, Mat& local_maxima, double threshold_value )
{
    Mat dilated_input_image,thresholded_input_image,thresholded_input_8bit;
    dilate(input_image,dilated_input_image,Mat());
    compare(input_image,dilated_input_image,local_maxima,CMP_EQ);
    threshold( input_image, thresholded_input_image, threshold_value, 255, THRESH_BINARY );
    thresholded_input_image.convertTo( thresholded_input_8bit, CV_8U );
    bitwise_and( local_maxima, thresholded_input_8bit, local_maxima );
}
TemplateMatcher::TemplateMatcher() {
}
int TemplateMatcher::Match() {
    Mat matched_template_map,display_image, correlation_image,tmplt;
    int result_columns,result_rows;
    std::vector<double> max;
    double min_correlation, max_correlation;
    for(size_t i = 0; i < mPageCount; i++) {
        resize(mPages[i], tmplt, Size((mPages[i].cols / SCALE), (mPages[i].rows / SCALE)));
        mImage.copyTo(display_image);
        result_columns = mImage.cols - tmplt.cols + 1;
        result_rows = mImage.rows - tmplt.rows + 1;
        correlation_image.create(result_columns, result_rows, CV_32FC1);
        matchTemplate(mImage, tmplt, correlation_image, CV_TM_CCORR_NORMED);
        minMaxLoc(correlation_image, &min_correlation, &max_correlation);
//        debugMessage("[" + std::to_string(i) +"]" + "Min: " + std::to_string(min_correlation) + "\tMax:" + std::to_string(max_correlation));
        max.push_back(max_correlation);
        FindLocalMaxima(correlation_image, matched_template_map, max_correlation * 0.99);
//        Mat matched_template_display1;
//        cvtColor(matched_template_map, matched_template_display1, CV_GRAY2BGR);
//        DrawMatchingTemplateRectangles(display_image, matched_template_map, tmplt, Scalar(0, 0, 255));
//        ShowImage("matched",JoinImagesHorizontally(display_image,tmplt,10));
    }

    int res = 0;
    for(size_t i = 0; i < max.size();i++){
        if(max[i] > max[res]){
            res = (int) i;
        }
    }
//    debugMessage(std::to_string(res) + ":" + std::to_string(max[res]));
    return res;
}

TemplateMatcher::TemplateMatcher(cv::Mat src,cv::Mat* pages, int pageCount) : TemplateMatcher(){
    cv::Mat tmp2;
    resize(src,tmp2,Size((src.cols/SCALE), (src.rows/SCALE)));
    mImage = tmp2;
    mPages = pages;
    mPageCount = pageCount;
}
