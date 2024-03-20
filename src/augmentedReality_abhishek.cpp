/*
Test file workspace for Aaron Pan
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

    // Output variables for camera calibration
    // int calibration_flag = 1;

    cv::Size image_size(refS.width, refS.height); 

    // Initializing the camera matrix and distortion coefficients
    cv::Mat camera_matrix = cv::Mat::eye(3, 3, CV_64FC1);
    camera_matrix.at<double>(0, 2) = 327.4888231884889;
    camera_matrix.at<double>(1, 2) = 236.3120762355374;
    camera_matrix.at<double>(0, 0) = 465.7752569140609;
    camera_matrix.at<double>(1, 1) = 465.7752569140609;
    camera_matrix.at<double>(2, 2) = 1;

    std::vector<double> distortion_coefficients = {0.245898, -0.895857, -0.00232721, 0.00635609, 0.908045};

    for(;;) {
        *capdev >> frame; 
        if( frame.empty() ) {
            printf("frame is empty/n");
            break;
        }
        char key = cv::waitKey(1);
        //////////////////////// TASK 1 ////////////////////////
        
        //convert frame to grayscale to pass to functions
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        success = findChessboardCorners(gray, sizeChessboard, corner_set, CALIB_CB_FAST_CHECK);

        //runs if chessboard detected
        if(success){
            //get corners
            cornerSubPix(gray, corner_set, Size(11,11), Size(-1,-1), TermCriteria( TermCriteria::EPS+TermCriteria::COUNT, 30, 0.0001));

            //draw chessboard corners on frame
            drawChessboardCorners(frame, sizeChessboard, corner_set, success);
            
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

            ////////////////////////// Task 5 ////////////////////////
            const float scale = 5.0f;
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

        //////////////////////// TASK 2 ////////////////////////
        // if(key == 's' && success == true){
        //     //appending to corner_list and point_list
        //     select_images(corner_set, corner_list, point_set, point_list);

        //     //print err checking
        //     if(corner_list.size() != point_list.size()){
        //         cout << "WARNING: corner list and point list do not match in dimension, check program." << endl;
        //     }
        //     else{
        //         cout << corner_list.size() << " of 5 required calibration images saved." << endl;
        //     }
        // }

        //////////////////////// TASK 3 ////////////////////////

        // if(point_list.size() == 5 && calibration_flag == 0){
        //     vector<cv::Mat> rotations, translations;

        //     // Perform camera calibration
        //     double reprojection_error = cv::calibrateCamera(point_list, corner_list, image_size, camera_matrix, distortion_coefficients,
        //                                                 rotations, translations, cv::CALIB_FIX_ASPECT_RATIO);

        //     cout << "Calibration Results:" << endl;
        //     cout << "Camera Matrix:\n" << camera_matrix << endl;
        //     cout << "Distortion Coefficients:" << endl;
        //     for (int i = 0; i < distortion_coefficients.size(); i++){
        //         cout << distortion_coefficients[i] << ", ";
        //     }
        //     cout << endl;
        //     for (size_t i = 0; i < rotations.size(); ++i) {
        //         cout << "Image " << i + 1 << ":\n";
        //         cout << "Rotation Vector:\n" << rotations[i] << endl;
        //         cout << "Translation Vector:\n" << translations[i] << endl;
        //     }
        //     cout << "Total Reprojection Error: " << reprojection_error << endl;
            
        //     calibration_flag++;
        // }
        
        

        if(key == 'q') {
            break;
        }
        imshow("Video", frame);
        
    }
    delete capdev;
    return(0);
}