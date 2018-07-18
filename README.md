# MagniRead

An assistive reading application designed for individuals with impaired vision. MagniRead can take a snapshot from the webcam's live video footage, giving users a stable image that they can comfortably read from. Unlike many other assistive reading technologies, it isn't necessary to physically move the camera or reading material to view a magnified image. Just take a snapshot, then zoom and drag the image wherever you please! MagniRead also includes settings for modifying the image's brightness, contrast, etc.

## Getting Started 
### Pre-requisites

* Windows Operating System
* A webcam (recommended: external and mountable)

### Installing

#### Run

If you only want to use the application, go to the "Releases" tab and download the latest version! To run the application, unzip the downloaded folder:

*Windows 7 and above:*
```
Right click > Extract all... > Choose folder
```
*Windows Vista and below:*
Install a file archiver such as 7-zip or WinRAR, then extract the files similarly.

Open the folder you just created and double-click the "MagniRead.exe" file to run the application. If you want to move the executible file somewhere else:
```
Right click > Create Shortcut > Move shortcut
```
Do **NOT** move the original file outside of this folder, otherwise the application will break. 

*Release coming soon...*

#### Build from source instead
***NOTE:** only recommended if you want to modify or contribute code*

In order to build the program from source, you will need to install the dependencies below, as well as a few other development tools.

Windows:
1. Download Qt Open Source [here](https://www.qt.io/download). This will give you access to the QtCreator IDE and many other tools. During installation, be sure to include MinGW as a component. Note that the installation may take a long time, as the framkework is many gigabytes in size.
2. Download OpenCV source code [here](https://opencv.org/releases.html). Follow [these instructions](https://wiki.qt.io/How_to_setup_Qt_and_openCV_on_Windows) to compile, build, and install OpenCV from the source code. From the default compilation settings, I enabled With Qt and With OpenGL, set CMAKE_BUILD_TYPE to Release, and disabled the objdetect, ts, & features2d build modules. Make sure BUILD_SHARED_LIBS is enabled. Compile and generate, then open the Command prompt and run `mingw32-make` and `mingw32-make install` in the build directory to install OpenCV.
3. Create project in the QtCreator IDE by selecting the "Qt Widgets Application" option. Remove/delete all except the .pro files.
4. Clone or download the application's source code from the [repository URL](https://github.com/jdwhite88/magni-read) to the project directory.
5. Add these lines of code to the .pro file (where path\to\X is the location of the folder X):
```
QT       += multimedia

INCLUDEPATH += path\to\opencv-build\install\include

LIBS += path\to\opencv-build\bin\libopencv_core320.dll
LIBS += path\to\opencv-build\bin\libopencv_videoio320.dll
LIBS += path\to\opencv-build\bin\libopencv_highgui320.dll
LIBS += path\to\opencv-build\bin\libopencv_imgproc320.dll

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    webcamview.cpp \
    settingsdialog.cpp \
    webcamsettings.cpp \
    webcamplayer.cpp

HEADERS += \
    mainwindow.h \
    webcamview.h \
    settingsdialog.h \
    webcamplayer.h

RESOURCES += resources.qrc
```

## TODO
### Upcoming
* Redesign sliders' look-and-feel for easier use
* Show sample images in settings window
* Configure player to play from webcam
* Fullscreen mode
* Implement display adjustments (zoom, brightness, contrast, etc.)

### Future Ideas
* Also drag image by clicking mouse button (like ClickLock on Windows)
* Cross-platform compatibility (Linux && OS X)

## Dependencies
* [Qt 5.11.0](https://www.qt.io/) - An open-source graphical user interface (GUI) framework.
* [OpenCV 3.2.0](https://opencv.org/) - An open-source computer vision library, used for video capture and image processing.

### Developer Dependencies
* MinGW 32bit (used v5.3.0) - Development environment that provides tools for compiling and debugging C++ programs (include with Qt installation)
* CMake (used v3.11.4) - A tool for compiling and building programs from source, used for configuring OpenCV build.
* CMake-GUI (Optional, used v3.11.4) - A GUI interface for CMake.
* Git (Optional, used v2.18.0) - Version control system for obtaining and submitting changes to repository files

## Author
* **[Jacob White](https://github.com/jdwhite88)**

## Acknowledgments
* My father, for inspiring the project and providing relevant feedback
* My great uncle, for whom this project was originally created
