#include <opencv2/opencv.hpp>
#include <ImageMagick-6/Magick++.h> 
#include <iostream> 

using namespace std; 
using namespace Magick; 
using namespace cv;

int main(int argc,char **argv) 
{ 
   // Initialise ImageMagick library
   InitializeMagick(*argv);

   // Create Magick++ Image object and read image file
   Image image("/home/sree/code/computer-vision/project-3/toys/group1/toys1.gif");

   // Get dimensions of Magick++ Image
   int w=image.columns();
   int h=image.rows();

   // Make OpenCV Mat of same size with 8-bit and 3 channels
   Mat opencvImage(h,w,CV_8UC3);

   std::cout<<opencvImage;

   // // Unpack Magick++ pixels into OpenCV Mat structure
   image.write(0,0,w,h,"BGR",Magick::CharPixel,opencvImage.data);

   // // Save opencvImage
   imwrite("result.png",opencvImage);
}
