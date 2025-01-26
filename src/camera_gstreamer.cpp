#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Define the GStreamer pipeline for the camera
    std::string pipeline = "libcamerasrc ! video/x-raw,format=I420,width=1280,height=720 ! videoconvert ! appsink";

    // Initialize the VideoCapture object with the GStreamer pipeline
    cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);

    // Check if the camera opened successfully
    if (!cap.isOpened()) {
        std::cerr << "Error: Unable to open camera with the GStreamer pipeline: " << pipeline << std::endl;
        return -1;
    }

    std::cout << "Camera successfully opened. Press 'q' to exit." << std::endl;

    cv::Mat frame;
    while (true) {
        // Capture a frame
        cap >> frame;

        // Check if the frame is empty
        if (frame.empty()) {
            std::cerr << "Error: Captured empty frame." << std::endl;
            break;
        }

        // Display the frame
        cv::imshow("Camera Feed", frame);

        // Break the loop if 'q' is pressed
        char key = (char)cv::waitKey(30);
        if (key == 'q' || key == 27) { // 'q' or ESC
            break;
        }
    }

    // Release the resources and close the windows
    cap.release();
    cv::destroyAllWindows();

    return 0;
}
