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
    }

    file.close();
}


int main() {
    // Load object points and image points
    vector<vector<cv::Point3f>> object_points; // Assuming initialized and populated
    vector<vector<cv::Point2f>> image_points;  // Assuming initialized and populated
    cv::Size image_size; // Assuming initialized

    // Initialize camera matrix and distortion coefficients
    cv::Mat camera_matrix = cv::Mat::eye(3, 3, CV_64F);
    camera_matrix.at<double>(0, 2) = image_size.width / 2;
    camera_matrix.at<double>(1, 2) = image_size.height / 2;
    cv::Mat distortion_coefficients = cv::Mat::zeros(1, 1, CV_64F);

    // Output variables
    vector<cv::Mat> rotations, translations;
    vector<float> per_view_errors;

    // Perform camera calibration
    double reprojection_error = cv::calibrateCamera(object_points, image_points, image_size,
                                                     camera_matrix, distortion_coefficients,
                                                     rotations, translations,
                                                     per_view_errors, cv::CALIB_FIX_ASPECT_RATIO);

    // Print calibration results
    cout << "Calibration Results:" << endl;

    cout << "Camera Matrix:\n" << camera_matrix << endl;
    cout << "Distortion Coefficients:\n" << distortion_coefficients << endl;

    for (size_t i = 0; i < rotations.size(); ++i) {
        cout << "Image " << i + 1 << ":\n";
        cout << "Rotation Vector:\n" << rotations[i] << endl;
        cout << "Translation Vector:\n" << translations[i] << endl;
        cout << "Per-View Reprojection Error: " << per_view_errors[i] << endl;
    }
    cout << "Total Reprojection Error: " << reprojection_error << endl;
    // Specify the filename for saving rotations and translations
    string filename = "rotations_and_translations.csv";
    // Save rotations and translations to CSV file
    saveRotationsAndTranslations(rotations, translations, filename);
    cout << "Rotations and translations saved to " << filename << endl;
    return 0;
}
