# MagniRead

An assistive reading application designed for individuals with impaired vision. MagniRead can take a snapshot of the webcam's live video footage, giving users a stable image that they can comfortably read from. Unlike many other assistive readers, it isn't necessary to physically move the camera or reading material to read magnified text on the screen. Just take a snapshot, then zoom and drag the image! MagniRead can also modify the image's brightness, contrast, filters, and other settings to enhance readability.

## Pre-Requisites

* Windows 7 or above (tested on Windows 7 and Windows 10)
* A high-resolution webcam (recommended: 1080p or 4K, not interpolated) that can view reading material

## Installation

### Run

If you only want to use the application, go to the "Releases" tab and download the latest version! To run the application, first extract the .zip file for its contents:

```
Open "MagniRead.zip" > Extract all... > Choose folder to extract to
```

Open the folder you just extracted and double-click the "MagniRead.exe" file to run the application. If you want to move the file somewhere else, create a shortcut:

```
Right click "MagniRead.exe" > Create Shortcut > Move shortcut
```
__*WARNING:*__ Do NOT move the original file outside of this folder, otherwise the application will break. 

### Modify
***NOTE:** only recommended if you want to modify or contribute code*

In order to build the program from source code, you will need to install the dependencies below.

Windows:
1. Download the Qt Open Source framework [here](https://www.qt.io/download). This will give you access to the QtCreator IDE and many other tools. During installation, be sure to include MinGW as a component. The installation may take a while.
2. Download OpenCV source code [here](https://opencv.org/releases.html). Follow [these instructions](https://wiki.qt.io/How_to_setup_Qt_and_openCV_on_Windows) to compile, build, and install OpenCV from source code.
3. Create project in the QtCreator IDE by selecting the "Qt Widgets Application" option. Remove/delete every file except the .pro file.
4. Clone or download the application's source code from the [repository URL](https://github.com/jdwhite88/magni-read) to the project directory.
5. Add these lines of code to the .pro file (where path\to\X is the location of the folder X):
```
QT       += multimedia

INCLUDEPATH += path\to\opencv-build\install\include

LIBS += path\to\opencv-build\bin\libopencv_core320.dll
LIBS += path\to\opencv-build\bin\libopencv_videoio320.dll
LIBS += path\to\opencv-build\bin\libopencv_highgui320.dll
LIBS += path\to\opencv-build\bin\libopencv_imgproc320.dll
LIBS += path\to\opencv-build\bin\libopencv_imgcodecs320.dll

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    webcamview.cpp \
    settingsdialog.cpp \
    webcamplayer.cpp

HEADERS += \
    mainwindow.h \
    webcamview.h \
    settingsdialog.h \
    webcamplayer.h

RESOURCES += resources.qrc

win32 {
	RC_FILE = MagniRead.rc
}
```
6. Now you should be able to modify and run the project's source code from the QtCreator IDE

## TODO
### Upcoming
* Performance improvements and bug fixes
* Pinch on touchscreen to zoom image in and out
* Add horizontal guide bar over image to help users read

### Future Ideas
* Allow user to sharpen and denoise image
* Save snapshots to the user's computer
* Choose resolution of image
* Add color filters for those with colorblindness
* Cross-platform compatibility (Linux && OS X)

### Known Bugs
* When changing webcams, the application acts unpredictably (changes video footage as intended, stops, or crashes). If the video footage stops, re-enter preview mode to get video from new webcam
* Can sometimes drag image too far && see whitespace past image
* Tooltips use system-dependent font size instead of application-defined font size
* If the available webcams change, the selected webcam isn't always correct

## Dependencies
* [Qt 5.11.0](https://www.qt.io/) - An open-source framework for developing graphical user interface (GUI) applications.
* [OpenCV 3.2.0](https://opencv.org/) - An open-source computer vision library, used for video capture and image processing.

### Developer Dependencies
* MinGW 32bit (used v5.3.0) - Development environment that provides tools for compiling and debugging C++ programs in Windows (included with Qt installation)
* CMake (used v3.11.4) - A tool for compiling and building programs from source, used for configuring OpenCV build.
* CMake-GUI (Optional) - A GUI interface for CMake.
* Git (Optional) - Version control system for obtaining and submitting changes to repository files

## Author
* **[Jacob White](https://github.com/jdwhite88)**

## Acknowledgments
* My father, for inspiring the project and providing relevant feedback
* My great uncle, for whom this project was originally created
