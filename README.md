# Raspberry Pi 5 - GStreamer and Testing

This repository contains source code, scripts, and configurations used for testing and development on the Raspberry Pi 5, focusing on GStreamer and related tools. The goal is to maintain reusable and modular resources for various Raspberry Pi testing scenarios.


## 📁 Repository Structure
```
├── scripts/               # Custom scripts for GStreamer testing and automation
├── configs/               # Configuration files for different setups
├── tests/                 # Test cases and validation files
├── logs/                  # Example log files (excluded in .gitignore)
├── docs/                  # Documentation and related guides
└── README.md              # Repository overview and usage instructions
```

## 🔧 Prerequisites
Before using the code in this repository, ensure you have the following:
- **Raspberry Pi 5** with a compatible OS installed (e.g., Raspberry Pi OS)
- **GStreamer** (built from source or installed via package manager)
- Required libraries/tools:
  - OpenCV
  - Python 3
  - Additional packages: `v4l-utils`, `gst-plugins-base`, `gst-plugins-good`, etc.

## 🚀 Getting Started
1. **Clone the repository:**
   ```bash
   git clone https://github.com/<your-username>/rpi5-gstreamer-testing.git
   cd rpi5-gstreamer-testing
   ```

2. **Install dependencies:**
   Refer to the [docs/setup-guide.md](docs/setup-guide.md) for detailed instructions on setting up your environment.

3. **Run a sample GStreamer pipeline:**
   ```bash
   ./scripts/sample_gstreamer_pipeline.sh
   ```

4. **Execute a test:**
   Navigate to the `tests/` directory and follow instructions for specific test cases.

## 📚 Documentation
- [Setup Guide](docs/setup-guide.md): Step-by-step instructions for setting up the environment.
- [Sample Pipelines](docs/sample-pipelines.md): Examples of GStreamer pipelines for testing.

## 📜 Usage Examples
### GStreamer Pipeline
To test video input using GStreamer:
```bash
gst-launch-1.0 v4l2src device=/dev/video0 ! videoconvert ! autovideosink
```

### Custom Script
Run the script to capture a video stream and save it to a file:
```bash
python3 scripts/capture_video.py --output test.mp4
```

## 🔧 Tools Used
- **Raspberry Pi 5**
- **GStreamer**
- **OpenCV**
- **Python**
- Additional tools: `v4l-utils`, `Tera Term`, etc.

## 🤝 Contributing
Contributions are welcome! Feel free to fork the repository, create a new branch, and submit a pull request.
