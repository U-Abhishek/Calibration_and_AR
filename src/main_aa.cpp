/*
Test file workspace for Aaron Pan
*/
#include <iostream>
#include "../include/aaron.h"

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

    cv::namedWindow("Video", 1); // identifies a window
    Mat frame, gray;

    // char key_flag = 'o';
    
    Size sizeChessboard(6,8);
    vector<Point2f> corner_set;
    bool success;

    for(;;) {
        *capdev >> frame; 
        if( frame.empty() ) {
            printf("frame is empty/n");
            break;
        }
        char key = cv::waitKey(1);
        //////////////////////// TASK 1 ////////////////////////
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        success = findChessboardCorners(gray, sizeChessboard, corner_set, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);

        if(success){
            cornerSubPix(gray, corner_set, Size(11,11), Size(-1,-1), TermCriteria( TermCriteria::EPS+TermCriteria::COUNT, 30, 0.0001 ));
            drawChessboardCorners(frame, sizeChessboard, corner_set, success);

            if(key == 's'){
                imwrite("../../data/images/image_sample.jpg", frame);
            }
        }

        
        if(key == 'c'){
            capture_cal_images();
        }

        //filepath for camera calibration images
        string cal_images_path = "../../data/images/*.jpg";
        string calibration_path =  "../../data/calibration/";

        if(key == 'q') {
            break;
        }
        imshow("Video", frame);
        
    }

    delete capdev;
    return(0);
}