//
// Created by basil-conto on 13/11/15.
//

#include <cstdlib>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>  // namedWindow, imshow, waitKey, destroyWindow

#include "Images.h"

int main() {
    std::vector<cv::Mat> images = LoadImages(IMAGE_LOCATION, VIEW_FILES, 1);

    for (size_t i = 0; i < images.size(); i++) {
        const std::string filename{IMAGE_LOCATION + VIEW_FILES[i]};
        cv::namedWindow(filename, cv::WINDOW_NORMAL);
        cv::imshow(filename, images[i]);
        cv::waitKey();
        cv::destroyWindow(filename);
    }

    return EXIT_SUCCESS;
}
