# Practical 01

# Hello World in OpenCV

## Installation on Linux

### Install `OpenCV` library and `Qt Creator` IDE from repository

- Arch based systems (OpenCV version: 4.1.1 - latest):

`$ sudo pacman -S opencv qtcreator `

- Debian/Ubuntu based systems (OpenCV version 3.2.0 - old, but may be sufficient):

`$ sudo apt install libopencv-dev qtcreator build-essential`

- check if your environment is ready (in case of OpenCV 3.x.x use `opencv` instead of `opencv4`):

`$ pkg-config --cflags --libs opencv4`

- the command should return something like this:

`-I/usr/include/opencv4 -lopencv_gapi -lopencv_stitching -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dnn_objdetect -lopencv_dpm -lopencv_face -lopencv_freetype -lopencv_fuzzy -lopencv_hdf -lopencv_hfs -lopencv_img_hash -lopencv_line_descriptor -lopencv_quality -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_superres -lopencv_optflow -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_highgui -lopencv_dnn -lopencv_plot -lopencv_videostab -lopencv_video -lopencv_videoio -lopencv_viz -lopencv_xfeatures2d -lopencv_shape -lopencv_ml -lopencv_ximgproc -lopencv_xobjdetect -lopencv_objdetect -lopencv_calib3d -lopencv_imgcodecs -lopencv_features2d -lopencv_flann -lopencv_xphoto -lopencv_photo -lopencv_imgproc -lopencv_core`

### Create the first project

- run `qtcreator`
- `File` menu -> `New File or Project...`
- `Non-Qt Project` -> `Plain C++ Application` -> `✓ Choose...`
- change name (e.g. `Practical_01`) and correct location -> `Next >`
- (qmake) -> `Next >`
- (all kits should be selected) -> `Next >`
- `Finish`

### Setup project to use OpenCV

- open `Practical_01.pro` file
- add lines following lines before `SOURCES` (in case of OpenCV 3.x.x use `opencv` instead of `opencv4`):

```
CONFIG += link_pkgconfig
PKGCONFIG += opencv4
```

### Hello World in `Qt Creator`

- download sample image (e.g. [lenna](https://upload.wikimedia.org/wikipedia/en/7/7d/Lenna_%28test_image%29.png))
- open `main.cpp` file
- change it to following lines:

```
#include <opencv2/core.hpp>      // it imports image representation structure
#include <opencv2/imgcodecs.hpp> // it imports image loading from file 
#include <opencv2/highgui.hpp>   // it imports simple GUI for showing an image

using namespace cv; // all OpenCV structures/functions are in `cv` namespace (package) 

int main(){
    Mat image = imread("../Lenna.png"); // load image from file
    imshow("image", image);             // show image in simple GUI
    waitKey();                          // waiting for key press (we want to see something)
    return 0;
}
```

- CTRL+R to run the project

### Troubleshooting

- compilation error: `libhdf5.so.103, needed by /usr/lib/gcc/x86_64-pc-linux-gnu/9.1.0/../../../../lib/libopencv_hdf.so, not found (try using -rpath or -rpath-link)`
  - you need to install missing library from repository; it's name is `hdf5`
  - `hdf5` for Arch based systems (look at [Package Search](https://www.archlinux.org/packages/)
  - `libhdf5-dev` for Debian/Ubuntu based systems (look at [Ubuntu Packages Seatch](https://packages.ubuntu.com/)

## Installation on Windows 10 (TBD)

### Install `OpenCV` library and `Visual Studio IDE`

- Install [Visual Studio IDE](https://visualstudio.microsoft.com/) with C++ support
- go to [OpenCV page](https://opencv.org/releases/)
- select proper version and click on `Windows` button
- run or extract the downloaded file ideally to drive root (e.g. `C:\`) - all files will be extracted into `C:\opencv` directory

### Set the environment

- in Search, search for `System` (Control Panel)
- `Advanced system settings` link
- `Advanced` tab -> `Environment Variables` button
- `System variables` section -> `New...` button
- create variable `OPENCV_DIR` with value `C:\opencv`
- create variable `OPENCV_VER` with value `411`

### Create the first project (TBD)
