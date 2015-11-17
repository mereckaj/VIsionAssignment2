#include <iostream>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "Images.hpp"
#include "Utils.h"
#include "PointDetector.h"

#define NUMBER_OF_PAGES 13
#define NUMBER_OF_VIEWS 50
#define NUMBER_OF_BINS 256

void debugMessage(std::string s);

cv::Mat *pageImages,*viewImages,*backProjectSample;
char * IMAGE_LOCATION = "/home/mereckaj/Dev/ClionProjects/VisionAssignment2/Images/";
char * viewFiles[] =
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
char * pageFiles[] =
        {
            "Page01.jpg","Page02.jpg","Page03.jpg","Page04.jpg",
            "Page05.jpg","Page06.jpg","Page07.jpg","Page08.jpg",
            "Page09.jpg","Page10.jpg","Page11.jpg","Page12.jpg",
            "Page13.jpg"
        };
char * backProjectFiles[] =
        {
            "BackProjectSample.png"
        };
void LoadAllImages(){
    pageImages = LoadImages(IMAGE_LOCATION,pageFiles,NUMBER_OF_PAGES);
    viewImages = LoadImages(IMAGE_LOCATION,viewFiles,NUMBER_OF_VIEWS);
    backProjectSample = LoadImages(IMAGE_LOCATION,backProjectFiles,1);
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
    cv::Mat dots;
    int threshold_value = 20;
    LoadAllImages();
    for(int imageIndex = 0; imageIndex < NUMBER_OF_VIEWS;imageIndex++){
        PointDetector* pointDetector;
        pointDetector = new PointDetector(viewImages[imageIndex],backProjectSample[0]);
        dots = pointDetector->DetectPoints();
        DisplayImage("",dots,true);
    }
}