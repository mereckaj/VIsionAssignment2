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
//            "BookView25.JPG","BookView26.JPG","BookView27.JPG","BookView28.JPG",
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
            "BackProjectSample.png",
            "BackProjectSample2.png"
        };

void LoadAllImages(){
    pageImages = LoadImages(IMAGE_LOCATION,pageFiles);
    viewImages = LoadImages(IMAGE_LOCATION,viewFiles);
    backProjectSample = LoadImages(IMAGE_LOCATION,backProjectFiles);
    debugMessage("Loaded all images successfully");
}

/*
 * Back project
 * Threshold
 * Erode & Dilate
 * Contours
 *
 */
int main() {
    LoadAllImages();
    cv::Mat detectedPage,maskedImage,dots,drawingWithCorners;
    std::vector<cv::Point> corners,closest;
    for(size_t imageIndex = 0; imageIndex < viewFiles.size();imageIndex++){
        /*
         * Detect the white page and apply it as a mask to the original image.
         */
        PageDetector pageDetector(viewImages[imageIndex]);
        detectedPage = pageDetector.DetectPage();
        maskedImage = pageDetector.ApplyMask(viewImages[imageIndex],detectedPage);

        /*
         * Search for blue dots in the masked image
         */
        PointDetector pointDetector(maskedImage,15,std::to_string(imageIndex));
        dots = pointDetector.DetectPoints(backProjectSample[0]);
        /*
         * Transform the image
         */
        Transformer transformer(maskedImage);
        corners = transformer.FindCorners(dots);

        /*
         * Draw the corners that were found
         */
        drawingWithCorners = transformer.Draw(viewImages[imageIndex],corners);
//        ShowImage("Corners",drawingWithCorners);
    }
    return EXIT_SUCCESS;
}