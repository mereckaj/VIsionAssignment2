//
// Created by mereckaj on 11/17/15.
//

#include <opencv2/core/mat.hpp>
#include <opencv2/core.hpp>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "Headers/Utils.hpp"

/*
 * KDH's code
 */
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
/*
 * KDH's code
 */
cv::Mat JoinImagesHorizontally(cv::Mat image1, cv::Mat image2, int spacing, int scale)
{
    cv::Size s;
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
    if(scale==0){
        s = cv::Size(result.cols,result.rows);
    }else if(scale < 1){
        scale = scale * -1;
        s = cv::Size((result.cols/scale),(result.rows/scale));
    }else if(scale > 1){
        s = cv::Size((result.cols*scale),(result.rows*scale));
    }
    cv::resize(result,result,s);
    return result;
}
/*
 * KDH's code
 */
cv::Mat JoinImagesVertically( cv::Mat& image1,cv::Mat& image2, int spacing, int scale )
{
    cv::Size s;
    cv::Mat result( image1.rows + image2.rows + spacing,
                (image1.cols > image2.cols) ? image1.cols : image2.cols,
                image1.type() );
    result.setTo(cv::Scalar(255,255,255));
    cv::Mat imageROI;
    imageROI = result(cv::Rect(0,0,image1.cols,image1.rows));
    image1.copyTo(imageROI);
    if (spacing > 0)
    {
        imageROI = result(cv::Rect(0,image1.rows,image1.cols,spacing));
        imageROI.setTo(cv::Scalar(255,255,255));
    }
    imageROI = result(cv::Rect(0,image1.rows+spacing,image2.cols,image2.rows));
    image2.copyTo(imageROI);
    if(scale==0){
        s = cv::Size(result.cols,result.rows);
    }else if(scale < 1){
        scale = scale * -1;
        s = cv::Size((result.cols/scale),(result.rows/scale));
    }else if(scale > 1){
        s = cv::Size((result.cols*scale),(result.rows*scale));
    }
    cv::resize(result,result,s);
    return result;
}
/*
 * Again, a wrapper for an OpenCV function since their functions are so bad.
 *
 * Creates a window with the image in it and a specified title.
 * Makes sure the image is at the left most corner
 * Wait for a keyclick and then destroy the image window
 */
void ShowImage(std::string title,cv::Mat src){
    cv::imshow(title,src);
    cv::moveWindow(title,0,0);
    cv::waitKey(0);
    cv::destroyWindow(title);
}
/*
 * printing out nice messages without having to drop that cout << << shit all over the place (Should rename C++ to C--)
 */
void debugMessage(std::string s){
    std::cout << s << std::endl;
}
/*
 * Given a location, a vector of file names loads those into the program
 */
cv::Mat * LoadImages(const std::string imageLocation,std::vector<std::string> imageNames){

    debugMessage("Loading: " + std::to_string(imageNames.size()) + " images from: " + imageLocation);

    cv::Mat *images = new cv::Mat[imageNames.size()];
    for (size_t i = 0; i < imageNames.size(); i++) {

        std::string filename(imageLocation);
        filename.append(imageNames[i]);
        images[i] = cv::imread(filename, cv::IMREAD_ANYCOLOR);

        if (images[i].empty()) {
            std::cerr << "Failed to load image: " << filename << std::endl;
            exit(EXIT_FAILURE);
        }else{

            std::cout << "Successfully loaded image: " << filename << std::endl;
        }
    }
    return images;
}