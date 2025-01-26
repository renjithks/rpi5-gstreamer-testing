#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdexcept>

class CameraApp {
public:
    enum class Mode { CPU, GPU };

    // Constructor that takes the mode as an argument
    CameraApp(Mode mode) : mode_(mode) {
        configurePipeline();
    }

    // Initialize and start the application
    void run() {
        // Initialize the VideoCapture object with the configured pipeline
        cv::VideoCapture cap(pipeline_, cv::CAP_GSTREAMER);

        if (!cap.isOpened()) {
            throw std::runtime_error("Error: Unable to open camera with the GStreamer pipeline: " + pipeline_);
        }

        std::cout << "Camera successfully opened using " 
                  << (mode_ == Mode::GPU ? "GPU" : "CPU") << " pipeline. Press 'q' to exit." << std::endl;

        cv::Mat frame;
        while (true) {
            // Capture a frame
            cap >> frame;

            if (frame.empty()) {
                std::cerr << "Error: Captured empty frame." << std::endl;
                break;
            }

            // Display the frame
            cv::imshow("Camera Feed", frame);

            // Break the loop if 'q' or ESC is pressed
            char key = (char)cv::waitKey(30);
            if (key == 'q' || key == 27) {
                break;
            }
        }

        cap.release();
        cv::destroyAllWindows();
    }

private:
    Mode mode_;
    std::string pipeline_;

    // Configure the GStreamer pipeline based on the mode
    void configurePipeline() {
        if (mode_ == Mode::GPU) {
            pipeline_ = "gst-launch-1.0 libcamerasrc ! queue ! videoconvert ! glimagesink";

        } else {
            pipeline_ = "libcamerasrc ! video/x-raw,format=I420,width=1280,height=720 ! videoconvert ! appsink";
        }
    }
};

// Helper function to parse mode from user input
CameraApp::Mode parseMode(const std::string& modeStr) {
    if (modeStr == "gpu") {
        return CameraApp::Mode::GPU;
    } else if (modeStr == "cpu") {
        return CameraApp::Mode::CPU;
    } else {
        throw std::invalid_argument("Invalid mode specified. Use 'cpu' or 'gpu'.");
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <mode> (cpu/gpu)" << std::endl;
        return -1;
    }

    try {
        // Parse the mode from the command-line argument
        CameraApp::Mode mode = parseMode(argv[1]);

        // Create the CameraApp instance and run it
        CameraApp app(mode);
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}

