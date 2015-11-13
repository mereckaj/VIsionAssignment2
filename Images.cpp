//
// Created by mereckaj on 11/11/15.
//

#include <cstdlib>
#include <algorithm>              // min
#include <string>
#include <vector>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>  // imread

#include "Images.h"

const std::string IMAGE_LOCATION{"Images/"};

const std::vector<std::string> VIEW_FILES{
    "BookView01.JPG", "BookView02.JPG", "BookView03.JPG", "BookView04.JPG",
    "BookView05.JPG", "BookView06.JPG", "BookView07.JPG", "BookView08.JPG",
    "BookView09.JPG", "BookView10.JPG", "BookView11.JPG", "BookView12.JPG",
    "BookView13.JPG", "BookView14.JPG", "BookView15.JPG", "BookView16.JPG",
    "BookView17.JPG", "BookView18.JPG", "BookView19.JPG", "BookView20.JPG",
    "BookView21.JPG", "BookView22.JPG", "BookView23.JPG", "BookView24.JPG",
    "BookView25.JPG", "BookView26.JPG", "BookView27.JPG", "BookView28.JPG",
    "BookView29.JPG", "BookView30.JPG", "BookView31.JPG", "BookView32.JPG",
    "BookView33.JPG", "BookView34.JPG", "BookView35.JPG", "BookView36.JPG",
    "BookView37.JPG", "BookView38.JPG", "BookView39.JPG", "BookView40.JPG",
    "BookView41.JPG", "BookView42.JPG", "BookView43.JPG", "BookView44.JPG",
    "BookView45.JPG", "BookView46.JPG", "BookView47.JPG", "BookView48.JPG",
    "BookView49.JPG", "BookView50.JPG"
};

const std::vector<std::string> PAGE_FILES{
    "Page01.jpg", "Page02.jpg", "Page03.jpg", "Page04.jpg", "Page05.jpg",
    "Page06.jpg", "Page07.jpg", "Page08.jpg", "Page09.jpg", "Page10.jpg",
    "Page11.jpg", "Page12.jpg", "Page13.jpg"
};

std::vector<cv::Mat> LoadImages(const std::string imageLocation,
                                const std::vector<std::string> imageNames) {
    return LoadImages(imageLocation, imageNames, imageNames.size());
}

std::vector<cv::Mat> LoadImages(const std::string imageLocation,
                                const std::vector<std::string> imageNames,
                                const size_t imageCount) {

    const size_t nimages = std::min(imageCount, imageNames.size());
    std::vector<cv::Mat> images(nimages);

    for (size_t i = 0; i < nimages; i++) {
        const std::string filename{IMAGE_LOCATION + imageNames[i]};
        images[i] = cv::imread(filename, cv::IMREAD_ANYCOLOR);
        if (!images[i].data) {
            std::cerr << "Failed to load image: " << filename << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    return images;
}
