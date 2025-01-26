cmake_minimum_required(VERSION 3.10)
project(camera_gstreamer)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include OpenCV
find_package(OpenCV REQUIRED)

# Add the executable
add_executable(camera_gstreamer src/camera_gstreamer.cpp)
add_executable(camera_gstreamer_gpu src/camera_gstreamer_gpu.cpp)

# Include OpenCV directories
include_directories(${OpenCV_INCLUDE_DIRS})

# Link the libraries manually
target_link_libraries(camera_gstreamer
    opencv_gapi opencv_stitching opencv_aruco opencv_bgsegm opencv_bioinspired
    opencv_ccalib opencv_dnn_objdetect opencv_dnn_superres opencv_dpm opencv_face
    opencv_freetype opencv_fuzzy opencv_hdf opencv_hfs opencv_img_hash
    opencv_intensity_transform opencv_line_descriptor opencv_mcc opencv_quality
    opencv_rapid opencv_reg opencv_rgbd opencv_saliency opencv_signal
    opencv_stereo opencv_structured_light opencv_phase_unwrapping opencv_superres
    opencv_optflow opencv_surface_matching opencv_tracking opencv_highgui
    opencv_datasets opencv_text opencv_plot opencv_videostab opencv_videoio
    opencv_wechat_qrcode opencv_xfeatures2d opencv_shape opencv_ml
    opencv_ximgproc opencv_video opencv_xobjdetect opencv_objdetect
    opencv_calib3d opencv_imgcodecs opencv_features2d opencv_dnn
    opencv_flann opencv_xphoto opencv_photo opencv_imgproc opencv_core
)

