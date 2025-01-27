#include <gst/gst.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>

class CameraApp {
public:
    enum class Mode { CPU, GPU };

    CameraApp(Mode mode) : mode_(mode) {
        configurePipeline();
    }

    void run() {
        if (mode_ == Mode::CPU) {
            runCpuMode();
        } else if (mode_ == Mode::GPU) {
            runGpuMode();
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
            // GPU-based pipeline
            pipeline_ = "libcamerasrc ! video/x-raw,format=NV12,width=1280,height=720 ! "
                        "glupload ! glcolorconvert ! glcolorscale ! glimagesink";
        }
    }

    void runCpuMode() {
        // OpenCV CPU-based frame processing
        cv::VideoCapture cap(pipeline_, cv::CAP_GSTREAMER);
        if (!cap.isOpened()) {
            throw std::runtime_error("Error: Unable to open the CPU pipeline.");
        }

        cv::Mat frame;
        int frameCount = 0;
        auto startTime = std::chrono::high_resolution_clock::now();

        while (true) {
            cap >> frame;
            if (frame.empty()) {
                std::cerr << "Error: Captured empty frame." << std::endl;
                break;
            }

            cv::imshow("CPU Mode", frame);
            frameCount++;

            // Calculate FPS
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
            if (elapsedTime > 0) {
                double fps = frameCount / static_cast<double>(elapsedTime);
                std::cout << "CPU Mode FPS: " << fps << "\r" << std::flush;
            }

            if (cv::waitKey(1) == 'q') {
                break;
            }
        }

        cap.release();
        cv::destroyAllWindows();
    }

    void runGpuMode() {
        // GStreamer-based GPU pipeline
        GstElement *pipeline = gst_parse_launch(pipeline_.c_str(), nullptr);
        if (!pipeline) {
            throw std::runtime_error("Error: Failed to create GStreamer pipeline.");
        }

        GstBus *bus = gst_element_get_bus(pipeline);
        GstMessage *msg = nullptr;

        gst_element_set_state(pipeline, GST_STATE_PLAYING);

        int frameCount = 0;
        auto startTime = std::chrono::high_resolution_clock::now();

        // Event loop to monitor the pipeline
        while (true) {
            msg = gst_bus_timed_pop_filtered(bus, GST_MSECOND * 100,
                                             static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS | GST_MESSAGE_ELEMENT));

            if (msg) {
                if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_ERROR) {
                    GError *err;
                    gchar *debug;
                    gst_message_parse_error(msg, &err, &debug);
                    std::cerr << "GStreamer Error: " << err->message << std::endl;
                    g_error_free(err);
                    g_free(debug);
                    break;
                } else if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_EOS) {
                    std::cout << "End of stream." << std::endl;
                    break;
                } else if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_ELEMENT) {
                    if (gst_message_has_name(msg, "frame-rendered")) {
                        frameCount++;

                        // Calculate FPS
                        auto currentTime = std::chrono::high_resolution_clock::now();
                        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
                        if (elapsedTime > 0) {
                            double fps = frameCount / static_cast<double>(elapsedTime);
                            std::cout << "GPU Mode FPS: " << fps << "\r" << std::flush;
                        }
                    }
                }

                gst_message_unref(msg);
            }

            if (frameCount > 0 && cv::waitKey(1) == 'q') {
                break;
            }
        }

        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(bus);
        gst_object_unref(pipeline);
    }
};

// Helper function to parse mode from user input
CameraApp::Mode parseMode(const std::string &modeStr) {
    if (modeStr == "cpu") {
        return CameraApp::Mode::CPU;
    } else if (modeStr == "gpu") {
        return CameraApp::Mode::GPU;
    } else {
        throw std::invalid_argument("Invalid mode. Use 'cpu' or 'gpu'.");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <mode> (cpu/gpu)" << std::endl;
        return -1;
    }

    gst_init(&argc, &argv); // Initialize GStreamer

    try {
        CameraApp::Mode mode = parseMode(argv[1]);
        CameraApp app(mode);
        app.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    gst_deinit(); // Deinitialize GStreamer
    return 0;
}

