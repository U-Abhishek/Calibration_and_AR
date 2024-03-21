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


    cv::Mat image1 = cv::imread("C:/Users/uabhi/my_cpp/prcv/Calibration_and_AR/data/snight.jpg");
    if (image1.empty()) {
        std::cerr << "Error: Could not read image1" << std::endl;
        return -1;
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
                cv::Point3f(-1, 1, 0), cv::Point3f(9, 1, 0),cv::Point3f(9, -6, 0), cv::Point3f(-1, -6, 0)
        
            };

            std::vector<cv::Point2f> imagePoints;
            cv::projectPoints(objectPoints, rotations, translations, camera_matrix, distortion_coefficients, imagePoints);

            // // Draw projected points on an image (assuming a blank image for demonstration)
            // for (const auto& point : imagePoints) {
            //     cv::circle(frame, point, 5, cv::Scalar(255, 255, 255), -1);
            //     }

            ///////////////////////// Task 6 ///////////////////////////

            // std::vector<cv::Point> points;
            // for (const auto& point : imagePoints) {
            //     points.push_back(cv::Point(static_cast<int>(point.x), static_cast<int>(point.y)));
            // }

            // // Converting points to a vector of vectors (required by fillPoly function)
            // std::vector<std::vector<cv::Point>> pts = {points};
            // // Drawing and filling the polygon
            // cv::polylines(frame, pts, true, cv::Scalar(0, 0, 0), 10);
            // //cv::fillPoly(frame, pts, cv::Scalar(255, 255, 255));


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

        imshow("Video", frame);
        if(key == 'q') {
            break;
        }
    
        
    }
    delete capdev;
    return(0);
}