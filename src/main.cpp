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
    capdev = new cv::VideoCapture(0);
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

    // Parameters
    Size sizeChessboard(9,6);
    bool success;
    vector<Point2f> corner_set;
    vector<Vec3f> point_set;
    vector<vector<Vec3f>> point_list;
    vector<vector<Point2f>> corner_list;
    for(int row = 0; row > (sizeChessboard.height)*-1; row--){
                for(int col = 0; col < sizeChessboard.width; col++){
                    point_set.push_back(Vec3f(col,row,0));
                }
            }

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
            //setting global coordinates of chessboard with X (pointing left), Y (pointing up), and Z (pointing out the screen)
            //global coordinates: (0,0,0) start at upper left internal corner of checkerboard
            

            //get corners
            cornerSubPix(gray, corner_set, Size(11,11), Size(-1,-1), TermCriteria( TermCriteria::EPS+TermCriteria::COUNT, 30, 0.0001));

            //draw chessboard corners on frame
            drawChessboardCorners(frame, sizeChessboard, corner_set, success);
        }

        //////////////////////// TASK 2 ////////////////////////
        if(key == 's'){
            //appending to corner_list and point_list
            select_images(corner_set, corner_list, point_set, point_list);

            //print err checking
            if(corner_list.size() != point_list.size()){
                cout << "WARNING: corner list and point list do not match in dimension, check program." << endl;
            }
            else{
                cout << corner_list.size() << " of 5 required calibration images saved." << endl;
            }
        }

        //////////////////////// TASK 3 ////////////////////////
        if(point_list.size() == 5){
            cv::Size image_size(frame.cols, frame.rows); 

            // Initializing the camera matrix and distortion coefficients
            cv::Mat camera_matrix = cv::Mat::eye(3, 3, CV_64FC1);
            camera_matrix.at<double>(0, 2) = image_size.width / 2;
            camera_matrix.at<double>(1, 2) = image_size.height / 2;
            std::vector<double> distortion_coefficients;

            // Output variables
            vector<cv::Mat> rotations, translations;

            // Perform camera calibration
            double reprojection_error = cv::calibrateCamera(point_list, corner_list, image_size, camera_matrix, distortion_coefficients,
                                                        rotations, translations, cv::CALIB_FIX_ASPECT_RATIO);

            cout << "Calibration Results:" << endl;
            cout << "Camera Matrix:\n" << camera_matrix << endl;
            //cout << "Distortion Coefficients:\n" << distortion_coefficients << endl;

            for (size_t i = 0; i < rotations.size(); ++i) {
                cout << "Image " << i + 1 << ":\n";
                cout << "Rotation Vector:\n" << rotations[i] << endl;
                cout << "Translation Vector:\n" << translations[i] << endl;
            }
            cout << "Total Reprojection Error: " << reprojection_error << endl;
            delete capdev;
            return(0);
        }
        

        //////////////////////// TASK 4 ////////////////////////

        //////////////////////// TASK 5 ////////////////////////

        //////////////////////// TASK 6 ////////////////////////
        if(key == 'q') {
            break;
        }
        imshow("Video", frame);
        
    }
    delete capdev;
    return(0);
}