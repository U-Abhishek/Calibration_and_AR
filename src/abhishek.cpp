#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;

// Task 3: camera calibration

void saveRotationsAndTranslations(const vector<cv::Mat>& rotations, const vector<cv::Mat>& translations, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file for writing." << endl;
        return;
    }
    file << "Image,Rotation_Vector,Translation_Vector" << endl;
    for (size_t i = 0; i < rotations.size(); ++i) {
        file << i + 1 << ",";
        file << rotations[i].at<double>(0, 0) << "," << rotations[i].at<double>(1, 0) << "," << rotations[i].at<double>(2, 0) << ",";
        file << translations[i].at<double>(0, 0) << "," << translations[i].at<double>(1, 0) << "," << translations[i].at<double>(2, 0) << endl;

    file.close();
}


int main() {
    
    return 0;
}
