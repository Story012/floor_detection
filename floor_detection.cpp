#define _USE_MATH_DEFINES

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include <iostream>
#include <cmath>
#include <tgmath.h>

// Input frames 
cv::Mat src, dst;
// Canny frames & parameters
cv::Mat blur_lines, canny_lines,kern;
int threshold1 = 850;
int threshold2 = 1400;
int kernel_size = 5;
// Probabilistic Hough lines frames & parameters
cv::Mat hough_lines;
std::vector<cv::Vec4i> linesP;
// Line filter frames & parameters
cv::Mat filter_lines;
float slope = 0;
float angle = 0;
std::vector<cv::Vec4i> right_lines;
std::vector<cv::Vec4i> left_lines;

void cannyThreshold(int, void*) {
    cv::blur( src, blur_lines, cv::Size(5,5) );
    kern = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(7,7),cv::Point(-1,-1));
    cv::dilate(blur_lines,blur_lines,kern);
    cv::Canny( blur_lines, canny_lines, threshold1, threshold2, kernel_size );
    cv::imshow("Canny Lines",canny_lines);
}

void houghProb(int, void*) {
    hough_lines = src.clone();
//  cv::HoughLinesP (image, lines, rho, theta, threshold, minLineLength = 0, maxLineGap = 0)
    cv::HoughLinesP(canny_lines, linesP, 1, CV_PI/180, 20, 50, 10 );
//     for (auto &l : linesP)
//         std::cout << l << std::endl;
    for( auto &l : linesP )
        cv::line( hough_lines, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 3, cv::LINE_AA);
    cv::imshow("Hough Lines",hough_lines);
}

void lineFilter(int, void*) {
    for(auto &l : linesP){
        //l = <x1, y1, x2, y2>
//         std::cout << l[1] << std::endl;
        slope = (l[3]-l[1])/(l[2]-l[0]);
        //angle = (atan(slope))*(180/M_PI);
        angle = (atan2((l[3]-l[1]),(l[2]-l[0])))*(180/M_PI);
        std::cout << angle << std::endl;
        //horizontal lines
        if (angle >= 25 && angle <= 50)
            right_lines.push_back(l);
        if (angle <= -25 && angle >= -50)
            left_lines.push_back(l);
    }
    filter_lines = src.clone();
//     filter_linesv = src.clone();
//     for( auto &l : vert_lines ) {
//         cv::line( filter_linesv, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,255,0), 3, cv::LINE_AA);
//         cv::line( filter_lines, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(255,255,0), 3, cv::LINE_AA);
//     }
    for(auto &l : left_lines ) {
        cv::line( filter_lines, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,255,0), 3, cv::LINE_AA);
//         cv::line( filter_lines, cv::Point(i[0], i[1]), cv::Point(i[2], i[3]), cv::Scalar(255,255,0), 3, cv::LINE_AA);
    }
    for(auto &l : right_lines ) 
        cv::line( filter_lines, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(255,0,0), 3, cv::LINE_AA);
    cv::imshow("Filtered Lines", filter_lines);;
//     cv::imshow("Filtered Vertical Lines", filter_linesv);
    //cv::imshow("F  iltered Lines", filter_lines);
    left_lines.clear();
    right_lines.clear();
}    

int main( int argc, char** argv ) {
    cv::VideoCapture cap("/home/sebas/Desktop/vision/Media/Minirover.mp4");
    if(!cap.isOpened())
    return -1;
    
    while(1){
        cap >> src;
        src = src(cv::Range(80,1000),cv::Range(300,1620));
        //cv::imshow("Source",src);
        cannyThreshold(0,0);
        houghProb(0,0);
        lineFilter(0,0);
        char c = (char)cv::waitKey(10);
        if( c == 27 )
            break;
    }
  }
