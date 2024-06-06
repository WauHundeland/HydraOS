//
// Created by WauHu on 13.01.2024.
//

#ifndef UTILS_H
#define UTILS_H
#include <Arduino.h>
#include <vector>


class Utils {
public:
    static void waitForInput(String &input, int x = 27, int hdl_t = 10, bool passwordMode = false);

    static void waitForKey();

    static void popup(const String& text, int waitMode = 0, float textSize = 1);

    static void sdPopup(const String &text, const String &path, int waitMode = 0);

    static void initCanvas();

    static std::vector<String> splitString(const String &s, char delimiter);
};



#endif //UTILS_H
