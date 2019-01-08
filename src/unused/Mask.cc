#include <iostream>

std::vector<cv::Mat> create_mask(std::vector<cv::Mat> images, std::vector<cv::Mat> diffs){

  std::vector<cv::Mat> maskedImages;
  double disp = (images.size() - diffs.size())/2;
  int len = images.size();
  for(int i=disp; i<len-disp; i++){
    cv::Mat maskedImage;
    cv::Mat eightBitImage;

    // converting the image to 8 bit unsigned
    images[i].convertTo(eightBitImage, CV_8U);
    eightBitImage.copyTo(maskedImage, diffs[i-disp]);

    maskedImages.push_back(maskedImage);
  }

  return maskedImages;

}
