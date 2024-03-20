#pragma once

#define _USE_MATH_DEFINES

#include <iostream>
#include <stdio.h>
#include <cmath>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int capture_cal_images();

int select_images(vector<Point2f> corner_set, vector<vector<Point2f>> &corner_list, vector<Vec3f> &point_set, vector<vector<Vec3f>> &point_list);

int generate_sphere_points(int N, int &count_flag, float &radius, vector<float> origin, vector<Point3f> &objectPoints);

int read_camera_calibration(Mat &camera_matrix, vector<double> &distortion_coefficients);

int write_camera_calibration(Mat camera_matrix, vector<double> distortion_coefficients);