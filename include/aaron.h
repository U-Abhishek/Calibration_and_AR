#pragma once
#include <iostream>
#include <stdio.h>
#include <cmath>
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