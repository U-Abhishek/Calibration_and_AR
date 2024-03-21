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

int generate_sphere_points(int N, int &count_flag, float &radius, vector<float> origin, vector<Point3f> &objectPoints){
    float theta, phi;
    for (int i = 0; i < N; ++i) {
        theta = 2 * CV_PI * i / N;
        for (int j = 0; j < N; ++j) {
            phi = CV_PI * j / N;
            float x_val = origin[0]+(radius * cos(theta) * sin(phi));
            float y_val = origin[1]+(radius * sin(theta) * sin(phi));
            float z_val = origin[2]+(radius * cos(phi));
            objectPoints.push_back(Point3f(x_val, y_val, z_val));
        }
    }
    if(count_flag == 0){
        cout << radius << endl;
        if(radius >= 1.5){
            count_flag = 1;
        }
        else{
            radius = radius + 0.1;
        }
        return(0);
    }
    if(count_flag == 1){
        if(radius <= 1.0){
            count_flag = 0;
        }
        else{
            radius = radius - 0.1;
        }
        return(0);
    }
    return(0);
}

int read_camera_calibration(Mat &camera_matrix, vector<double> &distortion_coefficients){
    //reading in filename from terminal
    string filename;
    FileStorage fs;

    while(true){
        cout << "Enter the name of the YAML file containing calibration matrix and distortion coefficients (without file path or extension): ";
        getline(cin, filename);

        //error checking
        if (filename.find(".yaml") == string::npos) {
            filename += ".yaml";
        }
        if(filename.find("../../data/calibration/") == string::npos){
            filename = "../../data/calibration/" + filename;
        }

        // setting to read file
        fs.open(filename, FileStorage::READ);
        // error check read file
        if(fs.isOpened()) {
            break;
        }
        else{
            cerr << "Failed to open " << filename << "Check filename and make sure there is a file in the data/calibration path" << endl;
        }
    }
    

    // read parameters to variables
    fs["camera_matrix"] >> camera_matrix;
    fs["distortion_coefficients"] >> distortion_coefficients;
    
    fs.release();

    // printing camera matrix and distortion coefficients for confirmation
    cout << "Camera matrix:" << endl << camera_matrix << endl;
    cout << "Distortion coefficients:" << endl;
    cout << "[ ";
    for(int i = 0; i < distortion_coefficients.size(); i++){
        if(i != distortion_coefficients.size() - 1){
            cout << distortion_coefficients[i] << ", ";
        }
        else{
            cout << distortion_coefficients[i];
        }
    }
    cout << " ]" << endl;
    
    return 0;
}

int write_camera_calibration(Mat camera_matrix, vector<double> distortion_coefficients){
    //reading in filename from terminal
    string filename;
    FileStorage fs;

    while(true){
        cout << "Enter the name of the YAML file containing calibration matrix and distortion coefficients (without file path or extension): ";
        getline(cin, filename);

        //error checking
        if (filename.find(".yaml") == string::npos) {
            filename += ".yaml";
        }
        if(filename.find("../../data/calibration/") == string::npos){
            filename = "../../data/calibration/" + filename;
        }

        // setting to read file
        fs.open(filename, FileStorage::WRITE);
        // error check read file
        if(fs.isOpened()) {
            break;
        }
        else{
            cerr << "Failed to open " << filename << "\nCheck filename or path location" << endl;
        }
    }

    //write camera matrix and distortion coefficients to YAML
    fs << "camera_matrix" << camera_matrix;
    fs << "distortion_coefficients" << distortion_coefficients;
    fs.release();

    // print successful calibration parameters
    cout << "Calibration parameters saved to " << filename << endl;\

    return 0;
}