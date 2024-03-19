#pragma once

#include "../include/aaron.h"

int capture_cal_images(){
    cout << "Take 10 pictures of the checkerboard in different orientations" << endl;

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
    Mat frame;

    // Parameters
    int required_images = 10;
    int num_images = 0;

    for(;;){
        if(num_images < required_images){
            *capdev >> frame; 
            if( frame.empty() ) {
                printf("frame is empty/n");
                break;
            }
            char key = cv::waitKey(1);
            if(key == 'c'){
                num_images++;
                string filename = "../../data/images/image_" + to_string(num_images) + ".jpg";
                imwrite(filename, frame);
                cout << num_images << "/" << required_images << " images taken" << endl;
            }
            if(key == 'q') {
                break;
            }
            imshow("Calibration Images", frame);
        }
        else{
            break;
        }
    }
    destroyWindow("Calibration Images");
    cout << "Finished capturing calibration images, now back in main video stream" << endl;
    return(0);
}

int select_images(vector<Point2f> corner_set, vector<vector<Point2f>> &corner_list, vector<Vec3f> &point_set, vector<vector<Vec3f>> &point_list){
    corner_list.push_back(corner_set);
    point_list.push_back(point_set);
    return(0);
}

int calibrate_images(string filepath){
    
    return(0);
}