//
// Created by mereckaj on 11/11/15.
//

#ifndef VISIONASSIGNMENT2_IMAGES_H
#define VISIONASSIGNMENT2_IMAGES_H

#define NUMBER_OF_PAGES 13
#define NUMBER_OF_VIEWS 50

char * IMAGE_LOCATION = "/home/mereckaj/Dev/ClionProjects/VissionAssignment2/Images";
char * viewFiles[] = {"BookView02.JPG","BookView03.JPG","BookView04.JPG","BookView05.JPG",
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
                      "BookView49.JPG","BookView50.JPG"};
char * pageFiles[] = {"Page01.jpg","Page02.jpg","Page03.jpg","Page04.jpg",
                      "Page05.jpg","Page06.jpg","Page07.jpg","Page08.jpg",
                      "Page09.jpg","Page10.jpg","Page11.jpg","Page12.jpg",
                      "Page13.jpg"};


cv::Mat* LoadImages(char* imageLocation,char*imageNames[],int imageCount);
#endif //VISIONASSIGNMENT2_IMAGES_H
