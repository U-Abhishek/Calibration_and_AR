/*
Aaron Pan, Abhishek Uddaraju
3/ 16 / 2024
This file consists of all the code to perform Project 4
*/
#include <iostream>
#include "../include/aaron.h"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


int main(int argc, char *argv[]) {
    VideoCapture *capdev;

    // open the video device
    capdev = new cv::VideoCapture(1);
    if( !capdev->isOpened() ) {
            printf("Unable to open video device\n");
            return(-1);
    }

    // get some properties of the image
    Size refS( (int) capdev->get(CAP_PROP_FRAME_WIDTH ),
                    (int) capdev->get(CAP_PROP_FRAME_HEIGHT));
    printf("Expected size: %d %d\n", refS.width, refS.height);

    cv::namedWindow("Video", 1); 
    Mat frame, gray;

    // Parameters for chessboard coordinates
    Size sizeChessboard(9,6);
    bool success;
    vector<Point2f> corner_set;
    vector<Vec3f> point_set;
    vector<vector<Vec3f>> point_list;
    vector<vector<Point2f>> corner_list;

    //setting global coordinates of chessboard with X (pointing left), Y (pointing up), and Z (pointing out the screen)
    //global coordinates: (0,0,0) start at upper left internal corner of checkerboard
    for(int row = 0; row > (sizeChessboard.height)*-1; row--){
        for(int col = 0; col < sizeChessboard.width; col++){
            point_set.push_back(Vec3f(col,row,0));
        }
    }

    // Initializing the camera matrix and distortion coefficients
    Mat camera_matrix;
    vector<double> distortion_coefficients;
    // reading camera matrix and distortion coefficients from yaml file
    read_camera_calibration(camera_matrix, distortion_coefficients);

    //global variables for drawing sphere
    int count_flag = 0;
    float radius = 1.0;
    vector<float> origin = {8,-5,3};

    //key flag
    char key_flag = 'o';

    // Reading image for AR
    cv::Mat image1 = cv::imread("../../data/snight.jpg");
    if (image1.empty()) {
        std::cerr << "Error: Could not read image1" << std::endl;
        return -1;
    }

    //main camera video feed loop
    for(;;) {
        *capdev >> frame; 
        if( frame.empty() ) {
            printf("frame is empty/n");
            break;
        }
        char key = cv::waitKey(1);
        //For Task 5: drawing simple object
        if(key == '5'){
            key_flag = '5';
        }
        //For Task 6: drawing simple object
        if(key == '6'){
            key_flag = '6';
        }
        //For Task 7: Harris corners
        if(key == '7'){
            key_flag = '7';
        }
        //For Extension: expanding/contracting circle
        if(key == 'c'){
            key_flag = 'c';
        }
        //For Extension: Overlay painting onto chessboard
        if(key == 'p'){
            key_flag = 'p';
        }
        //Default: Don't overlay anything
        if(key == 'o'){
            key_flag = 'o';
        }
        if(key == 'a'){
            key_flag = 'a';
        }

        //////////////////////// TASK 1 ////////////////////////
        
        //convert frame to grayscale to pass to functions
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        success = findChessboardCorners(gray, sizeChessboard, corner_set, CALIB_CB_FAST_CHECK);

        //runs if chessboard detected
        if(success){
            //get corners
            cornerSubPix(gray, corner_set, Size(11,11), Size(-1,-1), TermCriteria( TermCriteria::EPS+TermCriteria::COUNT, 30, 0.0001));

            //draw chessboard corners on frame
            //drawChessboardCorners(frame, sizeChessboard, corner_set, success);
            
            //////////////////////// TASK 4 ////////////////////////
            Mat rotations, translations;

            bool innerSuccess = cv::solvePnP(point_set, corner_set, camera_matrix, distortion_coefficients, rotations, translations);
            
            if(innerSuccess){
                cout << "Rotation Vector:\n" << rotations << endl;
                cout << "Translation Vector:\n" << translations << endl;
            }
            else if(innerSuccess = false){
                cout << "solvePnP FAILED" << endl;
            }

            if (key_flag == '5'){
                ////////////////////////// Task 5 ////////////////////////
                std::vector<cv::Point3f> objectPoints = {
                    cv::Point3f(0, 0, 0), cv::Point3f(8, 0, 0),cv::Point3f(8, -5, 0), cv::Point3f(0, -5, 0)
                };

                std::vector<cv::Point2f> imagePoints;
                cv::projectPoints(objectPoints, rotations, translations, camera_matrix, distortion_coefficients, imagePoints);

                // Draw projected points on an image (assuming a blank image for demonstration)
                for (const auto& point : imagePoints) {
                    cv::circle(frame, point, 5, cv::Scalar(255, 0, 255), -1);
                    }
            }
            if(key_flag == '6' || key_flag == 'a'){
                ////////////////////////// Task 6 ////////////////////////
                cout << "dsfadsfdsafdsafdsafdsafdsafdsaf" << endl;
                const float scale = 3.0f;
                std::vector<cv::Point3f> objectPoints = {
                    cv::Point3f(0, 0, scale/2+3), 
                    cv::Point3f(scale / 1, -scale / 2, 3),  
                    cv::Point3f(scale / 2.5, scale / 1, 3),
                    cv::Point3f(-scale / 3, scale / 3, 3),
                    cv::Point3f(0, 0, scale+3),
                    cv::Point3f(0, 0, scale/5+3), 
                    cv::Point3f(scale, -scale / 2, 3),  
                    cv::Point3f(scale / 2.5, scale / 1, 3),
                    cv::Point3f(-scale, scale / 3, 3),
                    cv::Point3f(0, 0, scale+3)           
                };

                std::vector<cv::Point2f> imagePoints;
                cv::projectPoints(objectPoints, rotations, translations, camera_matrix, distortion_coefficients, imagePoints);

                // Draw projected points on an image (assuming a blank image for demonstration)
                for (const auto& point : imagePoints) {
                    cv::circle(frame, point, 5, cv::Scalar(255, 0, 255), -1);
                    }

                ///////////////////////// Task 6 ///////////////////////////
                cv::Scalar color(100, 25, 150);
                cv::line(frame, imagePoints[0], imagePoints[1], color, 2);
                cv::line(frame, imagePoints[1], imagePoints[2], color, 2);
                cv::line(frame, imagePoints[2], imagePoints[3], color, 2);
                cv::line(frame, imagePoints[3], imagePoints[0], color, 2);
                cv::line(frame, imagePoints[3], imagePoints[1], color, 2);
                cv::line(frame, imagePoints[2], imagePoints[0], color, 2);

                cv::line(frame, imagePoints[0], imagePoints[4], color, 2);
                cv::line(frame, imagePoints[1], imagePoints[4], color, 2);
                cv::line(frame, imagePoints[2], imagePoints[4], color, 2);
                cv::line(frame, imagePoints[3], imagePoints[4], color, 2);


                cv::line(frame, imagePoints[0+4], imagePoints[1+4], color, 2);
                cv::line(frame, imagePoints[1+4], imagePoints[2+4], color, 2);
                cv::line(frame, imagePoints[2+4], imagePoints[3+4], color, 2);
                cv::line(frame, imagePoints[3+4], imagePoints[0+4], color, 2);
                cv::line(frame, imagePoints[3+4], imagePoints[1+4], color, 2);
                cv::line(frame, imagePoints[2+4], imagePoints[0+4], color, 2);

                cv::line(frame, imagePoints[0+4], imagePoints[4+4], color, 2);
                cv::line(frame, imagePoints[1+4], imagePoints[4+4], color, 2);
                cv::line(frame, imagePoints[2+4], imagePoints[4+4], color, 2);
                cv::line(frame, imagePoints[3+4], imagePoints[4+4], color, 2);
            }
            if(key_flag == '7'){
                ////////////////////////// Task 7 ////////////////////////
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
                double threshold = 150;
                std::cout << "Harris cornors detected" << std::endl;
                for (int i = 0; i < harris_corners_norm.rows; ++i) {
                    for (int j = 0; j < harris_corners_norm.cols; ++j) {
                        if (static_cast<int>(harris_corners_norm.at<float>(i, j)) > threshold) {
                            corner_locations.push_back(cv::Point(j, i));
                        }
                    }
                }

                // Displaying harris cornors on image
                int num_points = corner_locations.size();
                for (int i=0; i< num_points ; ++i) {
                    cv::circle(frame_copy, corner_locations[i], 3, cv::Scalar(0, 0, 255), 2);
                }
                cv::imshow("Harris Cornars", frame_copy);
                cv::imshow("Harris", harris_corners_norm_scaled);

            }
            if(key_flag == 'c' || key_flag == 'a'){
                ///////////////////////// Extension: Expanding/contracting sphere ///////////////////////////

                // parameters for plotting sphere
                vector<Point3f> sphere_points;
                int N = 20;

                // expanding/contracting circle:
                // generates points shaped in a sphere in regular angular intervals
                generate_sphere_points(N, count_flag, radius, origin, sphere_points);

                // display the points
                std::vector<cv::Point2f> imagePoints;
                cv::projectPoints(sphere_points, rotations, translations, camera_matrix, distortion_coefficients, imagePoints);
                for (const auto& point : imagePoints){
                    cv::circle(frame, point, 1, cv::Scalar(255, 0, 0), -1);
                }
            }
            if(key_flag == 'p'){
                ////////////////////////// Extension: Overlaying painting on top of chessboard ////////////////////////

                const float scale = 5.0f;
                std::vector<cv::Point3f> objectPoints = {
                    cv::Point3f(-1, 1, 0), cv::Point3f(9, 1, 0),cv::Point3f(9, -6, 0), cv::Point3f(-1, -6, 0)
            
                };

                std::vector<cv::Point2f> imagePoints;
                cv::projectPoints(objectPoints, rotations, translations, camera_matrix, distortion_coefficients, imagePoints);


                std::vector<cv::Point2f> newPoints = imagePoints;
                std::vector<cv::Point2f> originalPoints = {
                                        cv::Point2f(0, 0),
                                        cv::Point2f(image1.cols - 1, 0),
                                        cv::Point2f(image1.cols - 1, image1.rows - 1),
                                        cv::Point2f(0, image1.rows - 1)
                                        };

                // Compute the perspective transform matrix
                cv::Mat transformMatrix = cv::getPerspectiveTransform(originalPoints, newPoints);
                
                // Warp the first image
                cv::Mat warpedImage;
                cv::warpPerspective(image1, warpedImage, transformMatrix, frame.size());
                std::cout<< "Before perspectinve transform"<< std::endl;
                // Overlaying the warped image on top of the frame
                for (int y = 0; y < warpedImage.rows; ++y) {
                    for (int x = 0; x < warpedImage.cols; ++x) {
                        cv::Vec3b pixel = warpedImage.at<cv::Vec3b>(y, x);
                        if (pixel != cv::Vec3b(0, 0, 0)) {
                            frame.at<cv::Vec3b>(y, x) = pixel;
                        }
                    }
                }
            }
        }

        if(key == 'q') {
            break;
        }
        imshow("Video", frame);
        
    }
    delete capdev;
    return(0);
}