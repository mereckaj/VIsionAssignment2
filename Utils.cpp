//
// Created by mereckaj on 11/17/15.
//

#include <opencv2/core/mat.hpp>
#include <opencv2/core.hpp>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "Headers/Utils.hpp"
cv::Mat StretchImage( cv::Mat& image )
{
    cv::Mat result = image.clone();
    // Find max value
    int image_rows = image.rows;
    int image_channels = image.channels();
    int values_on_each_row = image.cols * image_channels;
    uchar max = 0;
    for (int row=0; row < image_rows; row++) {
        uchar* value = image.ptr<uchar>(row);
        for (int column=0; column < values_on_each_row; column++)
        {
            if (*value > max)
                max= *value;
            value++;
        }
    }
    // Stretch values using a lookup-table
    int entries(256);
    cv::Mat lut(1,entries,CV_8U);
    for (int i=0; (i<256); i++)
        lut.at<uchar>(i) = (uchar) ((255*i)/max);
    cv::LUT(image,lut,result);

    return result;
}
cv::Mat JoinImagesHorizontally( cv::Mat& image1, cv::Mat& image2, int spacing)
{
    cv::Mat result( (image1.rows > image2.rows) ? image1.rows : image2.rows,
                image1.cols + image2.cols + spacing,
                image1.type() );
    result.setTo(cv::Scalar(255,255,255));
    cv::Mat imageROI;
    imageROI = result(cv::Rect(0,0,image1.cols,image1.rows));
    image1.copyTo(imageROI);
    if (spacing > 0)
    {
        imageROI = result(cv::Rect(image1.cols,0,spacing,image1.rows));
        imageROI.setTo(cv::Scalar(255,255,255));
    }
    imageROI = result(cv::Rect(image1.cols+spacing,0,image2.cols,image2.rows));
    image2.copyTo(imageROI);
    return result;
}
void ShowImage(std::string title,cv::Mat src){
    cv::imshow(title,src);
    cv::moveWindow(title,50,10);
    cv::waitKey(0);
    cv::destroyWindow(title);
}
void ShowImage(std::string title,cv::Mat src,cv::Mat src2){
    ShowImage(title,JoinImagesHorizontally(src,src2,5));
}
void ShowImage(std::string title,cv::Mat src,cv::Mat src2,cv::Size scale){
    cv::Mat joinedScaled,joined = JoinImagesHorizontally(src,src2,5);
    cv::resize(joined,joinedScaled,scale,0,0,CV_INTER_NN);
    ShowImage(title,joinedScaled);
}
void debugMessage(std::string s){
    std::cout << s << std::endl;
}