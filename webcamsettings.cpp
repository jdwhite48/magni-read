#ifndef WEBCAMSETTINGS_CXX
#define WEBCAMSETTINGS_CXX

/*
 * Data structure containing image processing settings for webcam
 */
struct webcamSettings {
    // Index of webcam device
    int device;
    double brightness;
    double contrast;
};
#endif
