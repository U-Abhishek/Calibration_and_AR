/*
Test file workspace for Aaron Pan
*/
#include <iostream>
#include "../include/aaron.h"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {
    cv::VideoCapture *capdev;

    // open the video device
    capdev = new cv::VideoCapture(1);
    if( !capdev->isOpened() ) {
            printf("Unable to open video device\n");
            return(-1);
    }

    // get some properties of the image
    cv::Size refS( (int) capdev->get(cv::CAP_PROP_FRAME_WIDTH ),
                    (int) capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
    printf("Expected size: %d %d\n", refS.width, refS.height);

    cv::namedWindow("Video", 1); // identifies a window
    cv::Mat frame;

    for(;;) {
            *capdev >> frame; // get a new frame from the camera, treat as a stream
            if( frame.empty() ) {
                printf("frame is empty\n");
                break;
            }
            cv::Mat frame_copy = frame.clone();
            cv::Mat gray;
            cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
            cv::Mat harris_corners;
            cv::cornerHarris(gray, harris_corners, 3, 3, 0.1);

            // Normalizing the harris corners
            cv::Mat harris_corners_norm, harris_corners_norm_scaled;
            cv::normalize(harris_corners, harris_corners_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat());
            cv::convertScaleAbs(harris_corners_norm, harris_corners_norm_scaled);

            // Thresholding
            std::vector<cv::Point> corner_locations;
            double threshold = 180;
            std::cout << "Harris cornors detected" << std::endl;
            for (int i = 0; i < harris_corners_norm.rows; ++i) {
                for (int j = 0; j < harris_corners_norm.cols; ++j) {
                    if (static_cast<int>(harris_corners_norm.at<float>(i, j)) > threshold) {
                        corner_locations.push_back(cv::Point(j, i));
                    }
                }
            }

            // Displaying harris cornors on image
            int num_points = (100 < corner_locations.size()) ? 100 : corner_locations.size();
            for (int i=0; i< num_points ; ++i) {
                cv::circle(frame_copy, corner_locations[i], 3, cv::Scalar(0, 0, 255), 2);
            }
            cv::imshow("Harris Cornars", frame_copy);
            cv::imshow("Video", harris_corners_norm_scaled);

            // see if there is a waiting keystroke
            char key = cv::waitKey(10);
            if( key == 'q') {
                break;
            }
    }

    delete capdev;
    return(0);
}