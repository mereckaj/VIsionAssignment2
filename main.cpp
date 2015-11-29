#include <iostream>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "Headers/Utils.hpp"
#include "Headers/PointDetector.hpp"
#include "Headers/PageDetector.hpp"
#include "Headers/Transformer.hpp"
#include "Headers/TemplateMatcher.hpp"

/*
 * 15 just works
 */

#define MAGIC_THRESHOLD_VALUE 15

/*
 * Performance Metrics
 */
int correct = 0;
std::vector<int> groundTruth = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 2, 3, 5, 4, 7, 9, 8, 7, 11, 13, 12, 2};

/*
 * Image arrays
 */
cv::Mat *pageImages, *viewImages, *backProjectSample;

/*
 * Image names and locations
 */
const std::string IMAGE_LOCATION = "/home/mereckaj/Dev/ClionProjects/VisionAssignment2/Images/";
const std::vector<std::string> viewFiles =
        {
                "BookView01.JPG", "BookView02.JPG", "BookView03.JPG", "BookView04.JPG", "BookView05.JPG",
                "BookView06.JPG", "BookView07.JPG", "BookView08.JPG", "BookView09.JPG", "BookView10.JPG",
                "BookView11.JPG", "BookView12.JPG", "BookView13.JPG", "BookView14.JPG",  "BookView15.JPG",
                "BookView16.JPG", "BookView17.JPG", "BookView18.JPG", "BookView19.JPG", "BookView20.JPG",
                "BookView21.JPG", "BookView22.JPG", "BookView23.JPG", "BookView24.JPG", "BookView25.JPG",
//                "BookView26.JPG", "BookView27.JPG", "BookView28.JPG", "BookView29.JPG", "BookView30.JPG",
//                "BookView31.JPG", "BookView32.JPG", "BookView33.JPG", "BookView34.JPG", "BookView35.JPG",
//                "BookView36.JPG", "BookView37.JPG", "BookView38.JPG", "BookView39.JPG", "BookView40.JPG",
//                "BookView41.JPG", "BookView42.JPG", "BookView43.JPG", "BookView44.JPG", "BookView45.JPG",
//                "BookView46.JPG", "BookView47.JPG", "BookView48.JPG", "BookView49.JPG", "BookView50.JPG"
        };
const std::vector<std::string> pageFiles =
        {
                "Page01.jpg", "Page02.jpg", "Page03.jpg", "Page04.jpg",
                "Page05.jpg", "Page06.jpg", "Page07.jpg", "Page08.jpg",
                "Page09.jpg", "Page10.jpg", "Page11.jpg", "Page12.jpg",
                "Page13.jpg"
        };
const std::vector<std::string> backProjectFiles =
        {
                "BackProjectSample.png",// Blue pixels
                "BackProjectSample2.png"// White page pixels
        };


/*
 * This function loads all of the images into the program
 */
void LoadAllImages() {
    pageImages = LoadImages(IMAGE_LOCATION, pageFiles);
    viewImages = LoadImages(IMAGE_LOCATION, viewFiles);
    backProjectSample = LoadImages(IMAGE_LOCATION, backProjectFiles);
    debugMessage("Loaded all images successfully");
}

/*
 * This function will find the four corners of a template image
 */
std::vector<cv::Point> FindTemplateCorners(cv::Mat templ) {
    cv::Mat points;
    std::vector<cv::Point> corners;
    PointDetector pointDetector(templ, MAGIC_THRESHOLD_VALUE, "Template");
    points = pointDetector.DetectPoints(backProjectSample[0]);
    Transformer transformer(points);
    corners = transformer.FindTemplateCorners(points);
    return corners;
}

/*
 * Magic happens here.
 */
int main() {
    cv::Mat detectedWhitePage, maskedImage, possibleBlueDotLocations, transformedImage;
    std::vector<cv::Point> approxCorners,detectedCorners, cornerPointDestinations, centresOfConnectedComponents;
    std::vector<std::vector<cv::Point>> connectedComponents, closestPoints;
    std::vector<cv::Vec4f> bestFitLines;
    int matchedImageIndex = 0;

    LoadAllImages();

    /*
     * Take a single book picture and find the top left, bottom left, bottom right, top right blue pixels
     * return a vector of those pixels and use them as destination points for transformations
     *
     * This should preserve the aspect ratio of the images
     */
    cornerPointDestinations = FindTemplateCorners(pageImages[0]);

    /*
     * Loop over every single image and perform some vision magic on each
     */
    for (size_t imageIndex = 0; imageIndex < groundTruth.size(); imageIndex++) {

        // Detect the white page, will tell the program to look for blue dots in that page
        PageDetector pageDetector(viewImages[imageIndex]);
        detectedWhitePage = pageDetector.DetectPage();

        // Apply the white page location as a mask to the original image.
        maskedImage = pageDetector.ApplyMask(viewImages[imageIndex], detectedWhitePage);
        Transformer transformer(maskedImage);

        // Search for blue spots in the masked image
        PointDetector pointDetector(maskedImage, MAGIC_THRESHOLD_VALUE, std::to_string(imageIndex));
        possibleBlueDotLocations = pointDetector.DetectPoints(backProjectSample[0]);
        connectedComponents = pointDetector.BlueDotsToContours(possibleBlueDotLocations);
        centresOfConnectedComponents = pointDetector.GetCentres(connectedComponents);
        approxCorners = transformer.FindApproximateCornersFromCentresOfComponents(centresOfConnectedComponents);
        closestPoints = transformer.FindClosestPoints(approxCorners, possibleBlueDotLocations);
        bestFitLines = transformer.LinesOfBestFit(closestPoints);

        // Find the approxCorners that were of the image
        detectedCorners = transformer.FindIntersections(maskedImage, bestFitLines);

        // Transform the image
        transformedImage = transformer.Transform(viewImages[imageIndex], detectedCorners, cornerPointDestinations);

        // Match the image with a template
        TemplateMatcher templateMatcher(transformedImage, pageImages, (int) pageFiles.size());
        matchedImageIndex = templateMatcher.Match();

        // Ground truth starts counting from 1 while program start counting from 0
        matchedImageIndex++;

        if(matchedImageIndex==groundTruth[imageIndex]){
            correct++;
        }
    }
    debugMessage("Correctness: " + std::to_string(correct)+"/" + std::to_string(viewFiles.size()));
    return EXIT_SUCCESS;
}