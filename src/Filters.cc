#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>


cv::Mat applyGaussianFilterToImage(cv::Mat image,
				   cv::Size size,
				   double sigmaX,
				   double sigmaY){
  cv::Mat result;
  cv::GaussianBlur(image, result, size, sigmaX, sigmaY);

  return result;
}


std::vector<cv::Mat> applyGaussianFilterToImageArray(std::vector<cv::Mat> grayImages, int size_w, int size_h, double sigmaX, double sigmaY){

  std::vector<cv::Mat> smoothedImages;
  cv::Size maskSize = cv::Size(size_w, size_h);

  for(int i=0;i < grayImages.size();i++){
    cv::Mat result = applyGaussianFilterToImage(grayImages[i], maskSize, sigmaX, sigmaY);
    smoothedImages.push_back(result);
  }
  return smoothedImages;
}


/**
 *  Vector of vector (two mats) for every image input
 *  Two Mats are - Ix and Iy respectively
 */
std::vector<std::vector<cv::Mat>> spatialIntensityGradients(std::vector<cv::Mat> images){

  std::vector<std::vector<cv::Mat>>  gradients;

  cv::Mat prewitt_x = (cv::Mat_<char>(3, 1) << -1, 0, 1);
  cv::Mat prewitt_y = (cv::Mat_<char>(1, 3) << -1, 0, 1);

  for(int i=0;i<images.size();i++){
    std::vector<cv::Mat> gradientsForImage;

    cv::Mat src_image = images[i];
    cv::Mat Ix, Iy;

    filter2D(src_image, Ix, src_image.depth(), prewitt_x);
    filter2D(src_image, Iy, src_image.depth(), prewitt_y);

    gradientsForImage.push_back(Ix);
    gradientsForImage.push_back(Iy);


    gradients.push_back(gradientsForImage);
    gradientsForImage.clear();
  }
  return gradients;
}


std::vector<std::vector<cv::Mat>> findOpticalFlow(
                std::vector<std::vector<cv::Mat>> spatialGradients,
                std::vector<cv::Mat> temporalGradients,
                int nSize){
    /**
     *  Calculates Lucas-Kanade dense optical flow for every image in a vector
     *  of images
     */
    std::vector<std::vector<cv::Mat>> opticalFlow;

    for(int i=0; i<temporalGradients.size(); i++){

        int rows = temporalGradients[i].rows;
        int cols = temporalGradients[i].cols;
        std::vector<cv::Mat> flowXY;
        // initalize optical flow output channels with (nSize-1) fewer rows
        // and colmns -  no padding is applied to image during calculations
        cv::Mat Fx = cv::Mat::zeros(cv::Size(rows-(nSize-1), cols-(nSize-1)), CV_64FC1);
        cv::Mat Fy = cv::Mat::zeros(cv::Size(rows-(nSize-1), cols-(nSize-1)), CV_64FC1);

        for(int row=0; row<(rows-nSize+1); row++){
            for(int col=0; col<(cols-nSize+1); col++){
                cv::Rect nHood(col, row, nSize, nSize);
                cv::Mat A;
                cv::Mat Ax;
                cv::Mat Ay;
                cv::Mat b;
                // grab neighborhood spatial/temporal gradients and
                // reshape for linear algebra operations
                b = temporalGradients[i](nHood).clone();
                b = b.reshape(1, b.rows*b.cols);
                b = b*-1;
                Ax = spatialGradients[i+1][0](nHood).clone();
                Ax = Ax.reshape(1, Ax.rows*Ax.cols);
                Ay = spatialGradients[i+1][1](nHood).clone();
                Ay = Ay.reshape(1, Ay.rows*Ay.cols);
                cv::hconcat(Ax, Ay, A);

                // least squares solution for optical flow vector at
                // current pixel, given neighborhood
                cv::Mat v = (A.t()*A).inv()*A.t()*b;

                Fx.at<double>(row, col) = v.at<double>(0,0);
                Fy.at<double>(row, col) = v.at<double>(1,0);
            }
        }
        flowXY.push_back(Fx);
        flowXY.push_back(Fy);
        opticalFlow.push_back(flowXY);
    }
    return opticalFlow;
}
