#include <iostream>
#include <vector>


void display(cv::Mat image){

    if(! image.data )
    {
        std::cout <<  "Could not open or find the image" << std::endl ;
    }

    cv::namedWindow( "window", cv::WINDOW_AUTOSIZE );
    cv::imshow( "window", image );

    cv::waitKey(0);
}


std::vector<cv::Mat> RGBFromFlow(std::vector<std::vector<cv::Mat>> opticalFlow){

// Hue is direction of flow = inv-tan(y/x)
// Saturation is magnitude of flow = sqrt(sum(x^2 + y^2))

    std::vector<cv::Mat> RGBImages;
    for (int i=0; i<opticalFlow.size(); i++){
        cv::Mat angle;
        cv::Mat mag;
        cv::Mat value = cv::Mat::ones(cv::Size(opticalFlow[i][0].rows,
                                               opticalFlow[i][0].cols), CV_32F);
        // choose arbitrary constant for value channel
        value = value*122;

        std::vector<cv::Mat> channels;
        cv::Mat HSVImage;
        cv::Mat RGBFlow;
        bool angleInDegrees = true;

        cv::phase(opticalFlow[i][0], opticalFlow[i][1], angle, angleInDegrees);
        // scale to range [0,179], opencv standard
        angle = angle/2.0118;
        cv::magnitude(opticalFlow[i][0], opticalFlow[i][1], mag);

        mag.convertTo(mag, CV_32F);
        angle.convertTo(angle, CV_32F);

        channels ={angle, mag, value};
        cv::merge(channels, HSVImage);
        cv::cvtColor(HSVImage, RGBFlow, CV_HSV2RGB);
        RGBFlow.convertTo(RGBFlow, CV_8U);
        RGBImages.push_back(RGBFlow);
   }
    return RGBImages;
}


std::vector<cv::Mat> drawFlowArrows(std::vector<cv::Mat> images,
                                    std::vector<std::vector<cv::Mat>> opticalFlow,
                                    int step, int nSize){
    std::vector<cv::Mat> flowImages;
    int disp = std::floor(nSize/2);
    // iterate through images
    for (int i=0; i<opticalFlow.size(); i++){
        cv::Mat flowImage;
        images[i+1].convertTo(flowImage, CV_8U);
        // iterating through rows and columns by step size
        for (int row=0; row<opticalFlow[i][0].rows-step; row+=step){
            for (int col=0; col<opticalFlow[i][0].cols-step; col+=step){
                // head of arrow at pixel location
                // displaced to account for difference in image size between
                // original and flow image
                cv::Point head = cv::Point(col+disp, row+disp);
                cv::Point tail;
                cv::Point delta;
                int deltaX = std::floor(opticalFlow[i][0].at<double>(row, col));
                int deltaY = std::floor(opticalFlow[i][1].at<double>(row, col));
                delta = cv::Point(deltaX, deltaY);
                // tail displaced from head by flow X and Y components
                tail = head + delta;
                // draw arrow
                cv::arrowedLine(flowImage, tail, head, cv::Scalar(0, 255, 0));
            }
        }
        flowImages.push_back(flowImage);
    }
    return flowImages;
}
