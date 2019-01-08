#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>


std::vector<cv::Mat> compute1DDifferential(std::vector<cv::Mat> images, int threshold){

  std::vector<cv::Mat> diffs;

  for (std::vector<cv::Mat>::iterator it = images.begin(); it != images.end()-1; ++it){

    int i = std::distance(images.begin(), it);
    cv::Mat diff = images[i+1]-images[i];
    diffs.push_back(diff);

  }
  return diffs;
}
