//
// Created by mereckaj on 11/18/15.
//

#include <opencv2/core/mat.hpp>
#include <opencv2/core.hpp>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include "Headers/Transformer.hpp"
#include "Headers/Utils.hpp"

#define RIGHT_MOST_POINT 0
#define LEFT_MOST_POINT 1
#define TOP_MOST_POINT 2
#define BOTTOM_MOST_POINT 3

cv::RNG rng(12345);

Transformer::Transformer() {

}

Transformer::~Transformer() {

}

Transformer::Transformer(cv::Mat src) : Transformer() {
    mImage = src;
}

/*
 * Given a binbary image of possible blue dot locations, return those as Point objects
 */
std::vector<cv::Point> Transformer::WhiteToPoints(cv::Mat src) {
    cv::Mat whitePx;
    cv::findNonZero(src, whitePx);
    std::vector<cv::Point> result(whitePx.total());
    for (int i = 0; i < whitePx.total(); i++) {
        result[i] = whitePx.at<cv::Point>(i);
    }
    return result;
}

/*
 * Given a binary image of dots, find corners by looking for the pixels that are white and have the highest
 * X, lowest X, highest Y and lowest Y.
 *
 * Return 4 points: right most, left most, top most and bottom most (In that order)
 */
std::vector<cv::Point> Transformer::FindCorners(cv::Mat dots) {
    cv::Mat whitePx;
    cv::findNonZero(dots, whitePx);
    int highestX = 0, highestY = 0, lowestX = std::numeric_limits<int>::max(), lowestY = std::numeric_limits<int>::max();
    std::vector<cv::Point> result(4);
    for (size_t i = 0; i < whitePx.total(); i++) {
        cv::Point p = whitePx.at<cv::Point>((int) i);
        int x = p.x, y = p.y;
        if (x > highestX) {
            highestX = x;
            result[RIGHT_MOST_POINT] = p;
        }
        if (x < lowestX) {
            lowestX = x;
            result[LEFT_MOST_POINT] = p;
        }
        if (y > highestY) {
            highestY = y;
            result[BOTTOM_MOST_POINT] = p;
        }
        if (y < lowestY) {
            lowestY = y;
            result[TOP_MOST_POINT] = p;
        }
    }
    return result;
}

/*
 * Given the original image, a vector of corners and a vector of destinations
 * Use perspective transformations to transform the image
 */
cv::Mat Transformer::Transform(cv::Mat src, std::vector<cv::Point> corners, std::vector<cv::Point> dst) {
    cv::Mat matrix;
    int x1 = corners[TOP_MOST_POINT].x;
    int y1 = corners[TOP_MOST_POINT].y;
    int x2 = corners[LEFT_MOST_POINT].x;
    int y2 = corners[LEFT_MOST_POINT].y;
    int x3 = corners[BOTTOM_MOST_POINT].x;
    int y3 = corners[BOTTOM_MOST_POINT].y;
    int x4 = corners[RIGHT_MOST_POINT].x;
    int y4 = corners[RIGHT_MOST_POINT].y;
    cv::Mat result;
    cv::Point2f srcTransf[4], dstTransf[4];

    srcTransf[0] = cv::Point2f(x1, y1);
    srcTransf[1] = cv::Point2f(x2, y2);
    srcTransf[2] = cv::Point2f(x3, y3);
    srcTransf[3] = cv::Point2f(x4, y4);

    dstTransf[0] = cv::Point2f(dst[0].x, dst[0].y);
    dstTransf[1] = cv::Point2f(dst[1].x, dst[1].y);
    dstTransf[2] = cv::Point2f(dst[2].x, dst[2].y);
    dstTransf[3] = cv::Point2f(dst[3].x, dst[3].y);
    matrix = cv::getPerspectiveTransform(srcTransf, dstTransf);
    cv::warpPerspective(src, result, matrix, result.size());

    return result;
}

/*
 * Gind the corners of the template image (Different from above method as these corners could have the same X or Y
 * values where as in the method above the page is always at an angle
 *
 * Push the corners in the order:
 * Top left, bot left, bot right, top right
 */
std::vector<cv::Point> Transformer::FindTemplateCorners(cv::Mat dots) {
    cv::Mat whitePx;
    cv::findNonZero(dots, whitePx);
    int highestX = 0, highestY = 0, lowestX = std::numeric_limits<int>::max(), lowestY = std::numeric_limits<int>::max();
    std::vector<cv::Point> result;
    for (size_t i = 0; i < whitePx.total(); i++) {
        cv::Point p = whitePx.at<cv::Point>((int) i);
        int x = p.x, y = p.y;
        if (x > highestX) {
            highestX = x;
        }
        if (x < lowestX) {
            lowestX = x;
        }
        if (y > highestY) {
            highestY = y;
        }
        if (y < lowestY) {
            lowestY = y;
        }
    }
    result.push_back(cv::Point(lowestX, lowestY));
    result.push_back(cv::Point(lowestX, highestY));
    result.push_back(cv::Point(highestX, highestY));
    result.push_back(cv::Point(highestX, lowestY));
    return result;
}

/*
 * This where a lot of the magic happens
 *
 * Given the four possible corner points
 * Find points that are on a possible line starting at that corner point and between the other corner point.
 *
 * Set the scene: Image is at an angle.
 * Given the top most point, and the left most point look for points in between those two points
 *
 * This is done by looking for points that have a higher X than the left most point, but smaller X than the top most point
 * and higher Y than the top point and lower Y than the left most point.
 *
 * This creates a sort of a rectangle where the top and left most points are in the opposite corners. All of the possible
 * points will lie within this rectangle.
 *
 * Better explanation ill be in the report.
 */
std::vector<std::vector<cv::Point>> Transformer::FindClosestPoints(std::vector<cv::Point> corners, cv::Mat dots) {
    std::vector<std::vector<cv::Point>> result(4);
    std::vector<cv::Point> topLine, botLine, leftLine, rightLine;
    std::vector<cv::Point> points = WhiteToPoints(dots);
    cv::Point currentPoint;
    for (size_t i = 0; i < corners.size(); i++) {
        switch (i) {
            case TOP_MOST_POINT:
                for (size_t j = 0; j < points.size(); j++) {
                    currentPoint = points[j];
                    if ((currentPoint.x != corners[TOP_MOST_POINT].x) &&
                        (currentPoint.y != corners[TOP_MOST_POINT].y)) {
                        if ((currentPoint.x != corners[RIGHT_MOST_POINT].x) &&
                            (currentPoint.y != corners[RIGHT_MOST_POINT].y)) {
                            if ((currentPoint.x > corners[TOP_MOST_POINT].x) &&
                                (currentPoint.x < corners[RIGHT_MOST_POINT].x)) {
                                if ((currentPoint.y > corners[TOP_MOST_POINT].y) &&
                                    (currentPoint.y < corners[RIGHT_MOST_POINT].y)) {
                                    topLine.push_back(currentPoint);
                                }
                            }
                        }
                    }
                }
                break;
            case LEFT_MOST_POINT:
                for (size_t j = 0; j < points.size(); j++) {
                    currentPoint = points[j];
                    if ((currentPoint.x != corners[TOP_MOST_POINT].x) &&
                        (currentPoint.y != corners[TOP_MOST_POINT].y)) {
                        if ((currentPoint.x != corners[LEFT_MOST_POINT].x) &&
                            (currentPoint.y != corners[LEFT_MOST_POINT].y)) {
                            if ((currentPoint.x > corners[LEFT_MOST_POINT].x) &&
                                (currentPoint.x < corners[TOP_MOST_POINT].x)) {
                                if ((currentPoint.y < corners[LEFT_MOST_POINT].y) &&
                                    (currentPoint.y > corners[TOP_MOST_POINT].y)) {
                                    leftLine.push_back(currentPoint);
                                }
                            }
                        }
                    }
                }
                break;
            case BOTTOM_MOST_POINT:
                for (size_t j = 0; j < points.size(); j++) {
                    currentPoint = points[j];
                    if ((currentPoint.x != corners[BOTTOM_MOST_POINT].x) &&
                        (currentPoint.y != corners[BOTTOM_MOST_POINT].y)) {
                        if ((currentPoint.x != corners[LEFT_MOST_POINT].x) &&
                            (currentPoint.y != corners[LEFT_MOST_POINT].y)) {
                            if ((currentPoint.x < corners[BOTTOM_MOST_POINT].x) &&
                                (currentPoint.x > corners[LEFT_MOST_POINT].x)) {
                                if ((currentPoint.y > corners[LEFT_MOST_POINT].y) &&
                                    (currentPoint.y < corners[BOTTOM_MOST_POINT].y)) {
                                    botLine.push_back(currentPoint);
                                }
                            }
                        }
                    }
                }
                break;
            case RIGHT_MOST_POINT:
                for (size_t j = 0; j < points.size(); j++) {
                    currentPoint = points[j];
                    if ((currentPoint.x != corners[BOTTOM_MOST_POINT].x) &&
                        (currentPoint.y != corners[BOTTOM_MOST_POINT].y)) {
                        if ((currentPoint.x != corners[RIGHT_MOST_POINT].x) &&
                            (currentPoint.y != corners[RIGHT_MOST_POINT].y)) {
                            if ((currentPoint.x > corners[BOTTOM_MOST_POINT].x) &&
                                (currentPoint.x < corners[RIGHT_MOST_POINT].x)) {
                                if ((currentPoint.y < corners[BOTTOM_MOST_POINT].y) &&
                                    (currentPoint.y > corners[RIGHT_MOST_POINT].y)) {
                                    rightLine.push_back(currentPoint);
                                }
                            }
                        }
                    }
                }
                break;
        }
    }
    result[TOP_MOST_POINT] = topLine;
    result[BOTTOM_MOST_POINT] = botLine;
    result[LEFT_MOST_POINT] = leftLine;
    result[RIGHT_MOST_POINT] = rightLine;
    return result;
}

/*
 * Given the output of the above function find a line that best fits those points using Hubers method (No real reason,
 * it just worked better than the others.
 *
 */
std::vector<cv::Vec4f> Transformer::LinesOfBestFit(std::vector<std::vector<cv::Point>> lines) {

    std::vector<cv::Vec4f> result;
    for (size_t i = 0; i < lines.size(); i++) {
        auto current = lines[i];
        if(lines[i].size()<2){
            continue;
        }
        cv::Vec4f line;
        cv::fitLine(current, line, CV_DIST_HUBER, 0, 0.01, 0.01);
        result.push_back(line);
    }
    return result;
}

/*
 * Given a line, which is define by a point and a direction vector, draw it on the image.
 * Sinde the direction vector is only pointing in one way draw the line in the negative
 * direction too
 *
 * point has x as [2] and y as [3] in the array
 * direction vector for x is [0] and [1] for y
 * t is a scalar (Length of line)
 *
 * Draw the line between (x,y) and (x+ xd*t, y + yd*t) where xd and yd are the directions
 * fitLine documentation in OpenCV has a better explanation
 */
cv::Mat DrawLineItr(cv::Mat src, cv::Vec4f vect, int t, cv::Scalar color) {
    cv::line(src, cv::Point(vect[2], vect[3]),
             cv::Point(vect[2] + vect[0] * t, vect[3] + vect[1] * t),
             color,3);
    cv::line(src, cv::Point(vect[2], vect[3]),
             cv::Point(vect[2] + vect[0] * (t * -1), vect[3] + vect[1] * (t * -1)),
             color,3);
    return src;
}

/*
 * Draw all of the lines using avove method
 */
cv::Mat Transformer::DrawVectorLines(cv::Mat src, std::vector<cv::Vec4f> vecs) {
    cv::Mat r;
    src.copyTo(r);
    int t = 1000;
    for (size_t i = 0; i < vecs.size(); i++) {
        cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        src = DrawLineItr(r, vecs[i], t, color);

    }
    return r;
}

/*
 * given moments, find the corners. The function name explains it all, or so I hope.
 */
std::vector<cv::Point> Transformer::FindApproximateCornersFromCentresOfComponents(std::vector<cv::Point> momentCentres) {
    std::vector<cv::Point> result(4);
    int highestX = 0, highestY = 0, lowestX = std::numeric_limits<int>::max(), lowestY = std::numeric_limits<int>::max();
    for (size_t i = 0; i < momentCentres.size(); i++) {
        auto currentMoment = momentCentres[i];
        if (currentMoment.x > highestX) {
            highestX = currentMoment.x;
            result[RIGHT_MOST_POINT] = currentMoment;
        }
        if (currentMoment.x < lowestX) {
            lowestX = currentMoment.x;
            result[LEFT_MOST_POINT] = currentMoment;
        }
        if (currentMoment.y > highestY) {
            highestY = currentMoment.y;
            result[BOTTOM_MOST_POINT] = currentMoment;
        }
        if (currentMoment.y < lowestY) {
            lowestY = currentMoment.y;
            result[TOP_MOST_POINT] = currentMoment;
        }
    }
    return result;
}
/*
 * Get the intersection of two rays a, b
 *
 * where a ray is degine as a point and a direction vector (normalized)
 */
cv::Point Transformer::Intersection(cv::Vec4f a, cv::Vec4f b) {
    double a0x = a[2];
    double a0y = a[3];
    double b0x = b[2];
    double b0y = b[3];
    double adx = a[0];
    double ady = a[1];
    double bdx = b[0];
    double bdy = b[1];
    double t1 = ((bdx * (a0y - b0y) - bdy * (a0x - b0x))) / (adx * bdy - ady * bdx);
    cv::Point plz2work((int) (a[2] + (a[0] * t1)), (int) (a[3] + (a[1] * t1)));
    return plz2work;
}

/*
 * Given 4 rays that define the page, find the 4 intersections of corners
 * Assumes 4 lines were passed in.
 */
std::vector<cv::Point> Transformer::FindIntersections(cv::Mat src, std::vector<cv::Vec4f> rays) {
    std::vector<cv::Point> result(4);
    result[TOP_MOST_POINT] = Intersection(rays[2],rays[1]);
    result[LEFT_MOST_POINT] = Intersection(rays[1],rays[3]);
    result[BOTTOM_MOST_POINT] = Intersection(rays[3],rays[0]);
    result[RIGHT_MOST_POINT] = Intersection(rays[0],rays[2]);
    return  result;
}