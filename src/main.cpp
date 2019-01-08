#include "Config.cc"

#include "Preprocess.cc"
#include "Differential.cpp"
#include "Filters.cc"
#include "Utils.cc"

#include <vector>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <sstream>

int main(int argc, char** argv ){


std::map<std::string,std::string> config = loadConfig("./../src/config.cfg");

// path to images files
//const char *path = config["path"];
const char *path = "toys-pgm";

std::vector<std::string> image_filenames = get_files(path);
std::sort(image_filenames.begin(), image_filenames.end());

// removing . and .. from the files list
image_filenames.erase(image_filenames.begin(), image_filenames.begin() + 2);


/**
 * Read images and convert to grayscale double
 *
 */
 std::vector<cv::Mat> grayImages = get_grayscale_images(image_filenames);

/**
 * Creating downsized layer of image pyramid
 */
std::vector<cv::Mat> smallImages;
smallImages = resize_images(grayImages);


/**
 *  Smoothing with a gaussian filter
 *  and computing spatial intensity gradients
 *
 */
double gaussian_filter_sigma = 2;
std::vector<cv::Mat> smoothedBigImages = applyGaussianFilterToImageArray(grayImages,
                                                                      5,
                                                                      5,
                                                                      gaussian_filter_sigma,
                                                                      gaussian_filter_sigma);
std::vector<cv::Mat> smoothedSmallImages = applyGaussianFilterToImageArray(smallImages,
                                                                         5,
                                                                         5,
                                                                         gaussian_filter_sigma,
                                                                         gaussian_filter_sigma);
std::vector<std::vector<cv::Mat>> bigGradients =
    spatialIntensityGradients(smoothedBigImages);
std::vector<std::vector<cv::Mat>> smallGradients =
    spatialIntensityGradients(smoothedSmallImages);

/**
 *  Temporal gradients
 */
int threshold = 0;
std::vector<cv::Mat> bigDiffs =
    compute1DDifferential(smoothedBigImages, threshold);
std::vector<cv::Mat> smallDiffs =
    compute1DDifferential(smoothedSmallImages, threshold);


/**
 *  Solve for flow components at each pixel
 */
int nSize = 11;
std::vector<std::vector<cv::Mat>> bigOpticalFlow =
    findOpticalFlow(bigGradients, bigDiffs, nSize);
std::vector<std::vector<cv::Mat>> smallOpticalFlow =
    findOpticalFlow(smallGradients, smallDiffs, nSize);


/**
 *  Draw flow vectors on image
 */
int bigStep = 6;
int smallStep = 3;
std::vector<cv::Mat> bigFlowImages;
std::vector<cv::Mat> smallFlowImages;
bigFlowImages = drawFlowArrows(grayImages, bigOpticalFlow, bigStep, nSize);
smallFlowImages = drawFlowArrows(smallImages, smallOpticalFlow, smallStep, nSize);

display(bigFlowImages[0]);
display(smallFlowImages[0]);
// TODO: utilize config file
// NOTE: FLOW SOLUTION AT NOV-2 21:40 LECTURE
// TODO: debug color of flow images. Values of RGB are weird..

/**
 *  Represent flow vectors as HSV image
 */
 std::cout << "this is a test" << std::endl;

std::vector<cv::Mat> bigRGBImages = RGBFromFlow(bigOpticalFlow);
std::vector<cv::Mat> smallRGBImages = RGBFromFlow(smallOpticalFlow);
display(bigRGBImages[0]);
display(smallRGBImages[0]);

//std::cout << smallRGBImages[0] << std::endl;
//double min, max;
//cv::minMaxLoc(bigRGBImages[0], &min, &max);
//std::cout << min << "\t" << max << std::endl;

// Saving images flow images
cv::imwrite("report/images/big-arrows.png", bigFlowImages[0]);
cv::imwrite("report/images/small-arrows.png", smallFlowImages[0]);
cv::imwrite("report/images/big-color.png", bigRGBImages[0]);
cv::imwrite("report/images/small-color.png", smallRGBImages[0]);
}
