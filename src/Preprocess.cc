#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <dirent.h>
#include <string>

std::vector<std::string> get_files(const char* path){

  /**
   *  Get all the image files from a path
   */

  DIR *dir;
  struct dirent *ent;

  std::vector<std::string> images;

  std::string dirname = path;
  if ((dir = opendir (path)) != NULL) {
    while ((ent = readdir (dir)) != NULL) {
      std::string file_name = ent->d_name;
      std::string full_path = dirname + "/" + file_name;

      images.push_back(full_path);
    }
    closedir (dir);
  } else {
    perror ("");
  }
  return images;
}


cv::Mat convertToGrayscale(cv::Mat color_image) {

  /**
   * Convert image to grayscale
   */
  cv::Mat greyMat;
  cv::cvtColor(color_image, greyMat, cv::COLOR_BGR2GRAY);
  return greyMat;
}


std::vector<cv::Mat> get_grayscale_images(std::vector<std::string> filenames){

  /**
   * Read the files as grayscale images and return
   * a vector of grayscale matrices that represents them
   *
   */
  std::vector<cv::Mat> grayimages;
  cv::Mat image;

  for(auto file = filenames.begin(); file != filenames.end(); ++file) {
    image = cv::imread(*file);
    if ( image.empty() ){
      continue;
    }
    cv::Mat grayImage = convertToGrayscale(image);
    cv::Mat convertedGrayImage;
    grayImage.convertTo(convertedGrayImage, CV_64F);
    grayimages.push_back(convertedGrayImage);
  }
  return grayimages;

}


std::vector<cv::Mat> resize_images(std::vector<cv::Mat> originImages){

    /**
     * Resize Mat objects in a vector
     */
    std::vector<cv::Mat> smallImages;
    cv::Size s = cv::Size(0,0);
    for (int i=0; i<originImages.size(); i++){
        cv::Mat smallImage;
        cv::resize(originImages[i], smallImage, s, 0.5, 0.5);
        smallImages.push_back(smallImage);
    }
    return smallImages;
}
