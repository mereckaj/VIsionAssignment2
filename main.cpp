#include <iostream>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "Headers/Images.hpp"
#include "Headers/Utils.hpp"
#include "Headers/PointDetector.hpp"
#include "Headers/PageDetector.hpp"
#include "Headers/Transformer.hpp"
#include "Headers/LineDetector.hpp"
#include "Headers/TemplateMatcher.hpp"


#define RIGHT_MOST_POINT 0
#define LEFT_MOST_POINT 1
#define TOP_MOST_POINT 2
#define BOTTOM_MOST_POINT 3

cv::Mat *pageImages,*viewImages,*backProjectSample;

const std::string IMAGE_LOCATION = "/home/mereckaj/Dev/ClionProjects/VisionAssignment2/Images/";
const std::vector<std::string> viewFiles =
        {
            "BookView02.JPG","BookView03.JPG","BookView04.JPG","BookView05.JPG",
            "BookView06.JPG","BookView07.JPG","BookView08.JPG","BookView09.JPG",
            "BookView10.JPG","BookView11.JPG","BookView12.JPG","BookView13.JPG",
            "BookView14.JPG","BookView15.JPG","BookView16.JPG","BookView17.JPG",
            "BookView01.JPG","BookView18.JPG","BookView19.JPG","BookView20.JPG",
            "BookView21.JPG","BookView22.JPG","BookView23.JPG","BookView24.JPG",
            "BookView25.JPG"
//                            ,"BookView26.JPG","BookView27.JPG","BookView28.JPG",
//            "BookView29.JPG","BookView30.JPG","BookView31.JPG","BookView32.JPG",
//            "BookView33.JPG","BookView34.JPG","BookView35.JPG","BookView36.JPG",
//            "BookView37.JPG","BookView38.JPG","BookView39.JPG","BookView40.JPG",
//            "BookView41.JPG","BookView42.JPG","BookView43.JPG","BookView44.JPG",
//            "BookView45.JPG","BookView46.JPG","BookView47.JPG","BookView48.JPG",
//            "BookView49.JPG","BookView50.JPG"
        };
const std::vector<std::string> pageFiles =
        {
            "Page01.jpg","Page02.jpg","Page03.jpg","Page04.jpg",
            "Page05.jpg","Page06.jpg","Page07.jpg","Page08.jpg",
            "Page09.jpg","Page10.jpg","Page11.jpg","Page12.jpg",
            "Page13.jpg"
        };
const std::vector<std::string> backProjectFiles =
        {
            "BackProjectSample.png",// Blue pixels
            "BackProjectSample2.png"// White page pixels
        };

void LoadAllImages(){
    pageImages = LoadImages(IMAGE_LOCATION,pageFiles);
    viewImages = LoadImages(IMAGE_LOCATION,viewFiles);
    backProjectSample = LoadImages(IMAGE_LOCATION,backProjectFiles);
    debugMessage("Loaded all images successfully");
}
/*
 * return points in the order:
 *      top left
 *      bottom left
 *      bottom right
 *      top right
 */
std::vector<cv::Point> FindTemplateCorners(cv::Mat templ){
    cv::Mat points,cornerDrawing;
    std::vector<cv::Point> corners;
    PointDetector pointDetector(templ,15,"Template");
    points = pointDetector.DetectPoints(backProjectSample[0]);
    Transformer transformer(points);
    corners = transformer.FindTemplateCorners(points);
    return corners;
}
/*
 * Back project
 * Threshold
 * Erode & Dilate
 * Contours
 *
 */
void rect(cv::Mat src,cv::Mat s2) {
    using namespace cv;
    using namespace std;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    RNG rng(12345);
    findContours( src, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    vector<RotatedRect> minRect( contours.size() );

    for(int i = 0; i < contours.size(); i++ )
    {
        minRect[i] = minAreaRect( Mat(contours[i]) );
    }

    Mat drawing = Mat::zeros( src.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        // contour
        drawContours( drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
        // rotated rectangle
        Point2f rect_points[4]; minRect[i].points( rect_points );
        for( int j = 0; j < 4; j++ ) {
            line(s2, rect_points[j], rect_points[(j + 1) % 4], color, 1, 8);
        }
    }
    ShowImage("",drawing);
}

int main() {
    std::vector<int> receivedResults(viewFiles.size());
    int actaulResults[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,2,3,5,4,7,9,8,7,11,13,12,2};
    cv::Mat detectedPage,maskedImage,dots,drawingWithCorners,transformedImage,transformedCropped,sharpened;
    std::vector<cv::Point> corners,refCornerPoints;

    LoadAllImages();

    /*
     * Take a single book picture and find the top left, bottom left, bottom right, top right blue pixels
     * return a vector of those pixels and use them as destination points for transformations
     *
     * This should preserve the aspect ratio of the images
     */
    refCornerPoints = FindTemplateCorners(pageImages[0]);

    for(size_t imageIndex = 0; imageIndex < viewFiles.size();imageIndex++){
        /*
         * Detect the white page and apply it as a mask to the original image.
         */
        PageDetector pageDetector(viewImages[imageIndex]);
        detectedPage = pageDetector.DetectPage();

        maskedImage = pageDetector.ApplyMask(viewImages[imageIndex],detectedPage);
        Transformer transformer(maskedImage);
        /*
         * Search for blue dots in the masked image
         */
        PointDetector pointDetector(maskedImage,15,std::to_string(imageIndex));
        dots = pointDetector.DetectPoints(backProjectSample[0]);
//        rect(dots,maskedImage);
        /*
         * Transform the image
         */
        corners = transformer.FindCorners(dots);

        /*
         * Draw the corners that were dfound
         */
//        drawingWithCorners = transformer.Draw(viewImages[imageIndex],corners);
        transformedImage = transformer.Transform(viewImages[imageIndex],corners,refCornerPoints);
//        transformedCropped = transformer.Crop(transformedImage,pageImages[0]);
//        sharpened = transformer.Sharpen(transformedImage);
        sharpened = transformedImage;
        /*
         * Match the image with a template
         *
    //     */
        TemplateMatcher templateMatcher(sharpened,pageImages, (int) pageFiles.size());
        int matchedPage = templateMatcher.Match();
//        debugMessage("Matched with image: " + std::to_string(matchedPage));
//        ShowImage("Result",JoinImagesHorizontally(sharpened,pageImages[matchedPage],10));
        receivedResults[imageIndex] = matchedPage;
    }
    for(int i = 0; i < viewFiles.size();i++){
        debugMessage("Matched: " + std::to_string(receivedResults[i]) + "\tActual: " + std::to_string(actaulResults[i]) + "\tCorrect: " + std::to_string(receivedResults[i]==actaulResults[i]));
    }
    return EXIT_SUCCESS;
}