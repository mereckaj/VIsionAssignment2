#include <iostream>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "Headers/Images.hpp"
#include "Headers/Utils.hpp"
#include "Headers/PointDetector.hpp"

#define NUMBER_OF_PAGES 13
#define NUMBER_OF_VIEWS 50

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
            "BookView25.JPG","BookView26.JPG","BookView27.JPG","BookView28.JPG",
            "BookView29.JPG","BookView30.JPG","BookView31.JPG","BookView32.JPG",
            "BookView33.JPG","BookView34.JPG","BookView35.JPG","BookView36.JPG",
            "BookView37.JPG","BookView38.JPG","BookView39.JPG","BookView40.JPG",
            "BookView41.JPG","BookView42.JPG","BookView43.JPG","BookView44.JPG",
            "BookView45.JPG","BookView46.JPG","BookView47.JPG","BookView48.JPG",
            "BookView49.JPG","BookView50.JPG"
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
            "BackProjectSample.png"
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
    //TODO: find reason for crash
    for(int imageIndex = 0; imageIndex < NUMBER_OF_VIEWS;imageIndex++){
        cv::Mat dots;
        PointDetector pointDetector(viewImages[imageIndex],backProjectSample[0],15,std::to_string(imageIndex));
        dots = pointDetector.DetectPoints();
        pointDetector.Draw25centBox(dots);
        pointDetector.~PointDetector();
//        pointDetector.Show(dots);
//        pointDetector.DropOutliers(dots);
    }
    return EXIT_SUCCESS;
}