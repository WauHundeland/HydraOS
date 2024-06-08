//
// Created by WauHu on 13.01.2024.
//

#ifndef UTILS_H
#define UTILS_H
#include <Arduino.h>
#include <vector>


class Utils {
public:
    // wait for input and return the input
    // x: x position of the input
    // hdl_t: height of the input
    // passwordMode: if true, the input will be hidden
    static void waitForInput(String &input, int x = 27, int hdl_t = 10, bool passwordMode = false);

    // wait for a key to be pressed
    static void waitForKey();

    // popup a message
    // text: the message to be displayed
    // waitMode: 0: wait for a key to be pressed, 1: wait for 5 seconds
    // textSize: the size of the text
    static void popup(const String& text, int waitMode = 0, float textSize = 1);

    // don't use this function, use the popup function instead
    static void sdPopup(const String &text, const String &path, int waitMode = 0);

    // initialize the canvas
    static void initCanvas();

    // split a string by a delimiter
    static std::vector<String> splitString(const String &s, char delimiter);

    static void centerInput(String &input, const int x, const int hdl_t, const bool passwordMode);
};



#endif //UTILS_H
