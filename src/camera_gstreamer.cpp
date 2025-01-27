#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdexcept>

class CameraApp {
public:
    enum class Mode { CPU, GPU };

    CameraApp(Mode mode) : mode_(mode) {
        configurePipeline();
    }

    void run() {
        std::cout << "Using GStreamer pipeline: " << pipeline_ << std::endl;

        if (mode_ == Mode::CPU) {
            // CPU mode: OpenCV frame processing
            cv::VideoCapture cap(pipeline_, cv::CAP_GSTREAMER);

            if (!cap.isOpened()) {
                throw std::runtime_error("Error: Unable to open the camera pipeline for processing.");
            }

            std::cout << "Camera successfully opened for frame processing. Press 'q' to exit." << std::endl;

            cv::Mat frame;
            while (true) {
                cap >> frame;
                if (frame.empty()) {
                    std::cerr << "Error: Captured empty frame." << std::endl;
                    break;
                }

                // Display the frame
                cv::imshow("Camera Feed (CPU Mode)", frame);

                // Exit on 'q' or 'ESC'
                char key = (char)cv::waitKey(30);
                if (key == 'q' || key == 27) {
                    break;
                }
            }
            cap.release();
            cv::destroyAllWindows();
        } else if (mode_ == Mode::GPU) {
            // GPU rendering mode: Launch GStreamer pipeline
            std::cout << "Launching pipeline for GPU rendering..." << std::endl;
            std::string gstCommand = "gst-launch-1.0 " + pipeline_;
            int ret = std::system(gstCommand.c_str());
            if (ret != 0) {
                throw std::runtime_error("Error: Failed to launch GPU rendering pipeline.");
            }
        }
    }

private:
    Mode mode_;
    std::string pipeline_;

    void configurePipeline() {
        if (mode_ == Mode::CPU) {
            // CPU-based pipeline with OpenCV
            pipeline_ = "libcamerasrc ! video/x-raw,format=NV12,width=1280,height=720 ! "
                        "videoconvert ! videoscale ! video/x-raw,width=640,height=480,format=BGR ! appsink";
        } else if (mode_ == Mode::GPU) {
            // GPU-based pipeline for rendering
            pipeline_ = "libcamerasrc ! video/x-raw,format=NV12,width=1280,height=720 ! "
                        "glcolorconvert ! glcolorscale ! glimagesink";
        }
    }
};

// Helper function to parse mode from user input
CameraApp::Mode parseMode(const std::string& modeStr) {
    if (modeStr == "cpu") {
        return CameraApp::Mode::CPU;
    } else if (modeStr == "gpu") {
        return CameraApp::Mode::GPU;
    } else {
        throw std::invalid_argument("Invalid mode. Use 'cpu' for CPU processing or 'gpu' for GPU rendering.");
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

        // Create and run the CameraApp
        CameraApp app(mode);
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}

