//
// Created by WauHu on 13.01.2024.
//

#include "Utils.h"

#include <AppManager.h>
#include <M5Cardputer.h>
#include "ScreenManager.h"
#include "SD.h"
#include "assets/hydra.h"

void Utils::waitForInput(String &input, const int x, const int hdl_t, const bool passwordMode) {
    // if key is pressed, wait for release
    if (M5Cardputer.Keyboard.isPressed()) {
        while (M5Cardputer.Keyboard.isPressed()) {
            M5Cardputer.update();
            delay(50);
        };
    }

    static int cursorY = 0;
    unsigned long lastKeyPressMillis = 0;
    const unsigned long debounceDelay = 130; // Adjust debounce delay as needed
    String currentInput = input;

    while (true) {
        M5Cardputer.update();
        if (M5Cardputer.Keyboard.isChange()) {
            Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();

            if (status.del && currentInput.length() > 0) {
                // Handle backspace key
                currentInput.remove(currentInput.length() - 1);
                canvas.fillRect(7, cursorY, 230, hdl_t, BLACK);
                canvas.setCursor(8, cursorY);
                if (passwordMode) {
                    for (int i = 0; i < currentInput.length(); i++) {
                        canvas.print("*");
                    }
                } else {
                    canvas.print(currentInput);
                }
                canvas.drawRoundRect(5, 5, 230, 125, 7, WHITE);
                canvas.pushSprite(0, 0);
                cursorY = canvas.getCursorY();
                lastKeyPressMillis = millis();
            }

            for (auto i: status.word) {
                if (millis() - lastKeyPressMillis >= debounceDelay) {
                    if (currentInput.length() >= x) {
                        continue;
                    }
                    currentInput += i;
                    if (passwordMode) {
                        canvas.print("*");
                    } else {
                        canvas.print(i);
                    }
                    canvas.pushSprite(0, 0);
                    cursorY = canvas.getCursorY();
                    lastKeyPressMillis = millis();
                }
            }

            if (status.enter) {
                canvas.println(); // Move to the next line
                input = currentInput;
                break;
            }
        }
        if (M5.BtnA.isPressed()) {
            input = "exit";
            break;
        }
    }
}

void Utils::waitForKey() {
    M5Cardputer.update();
    // if key is pressed, wait for release
    if (M5Cardputer.Keyboard.isPressed()) {
        while (M5Cardputer.Keyboard.isPressed()) {
            M5Cardputer.update();
            delay(50);
        };
    }
    while (!M5Cardputer.Keyboard.isPressed() && !M5.BtnA.isPressed()) {
        M5Cardputer.update();
        delay(50);
    };
}

void Utils::initCanvas() {
    canvas.setTextSize(1);
    canvas.setTextColor(WHITE);
    canvas.setTextWrap(true);
    canvas.fillScreen(BLACK);
    canvas.setCursor(0, 0);
    // reset font
    canvas.setFont(&Font0);
}

void Utils::popup(const String &text, int waitMode, float textSize) {
    canvas.setTextSize(textSize);
    canvas.fillRoundRect(15, 15, 205, 105, 10, BLACK);
    canvas.drawRoundRect(20, 20, 200, 100, 10, WHITE);
    // each newline in text is a new line using canvas.drawCenterString(text, 120, 60 + (i * 8));
    int i = 0;
    for (const String &line : splitString(text, '\n')) {
        canvas.drawCenterString(line, 120, 30 + (i * 8 * textSize));
        i++;
    }
    if (waitMode == 0) {
        canvas.setTextSize(1);
        canvas.drawCenterString("Press any key to continue", 120, 110);
    }
    canvas.pushSprite(0, 0);
    if (waitMode == 0) {
        waitForKey();
    } else {
        delay(waitMode);
    }
    canvas.setTextSize(1);
}

void Utils::sdPopup(const String &text, const String &path, int waitMode) {
    canvas.fillRoundRect(15, 15, 205, 105, 10, BLACK);
    canvas.drawRoundRect(20, 20, 200, 100, 10, WHITE);
    // show image
    int x = ((240 - 50) / 2);
    canvas.drawRoundRect(x, 30, 50, 50, 5, WHITE);
    if (SD.exists(path.c_str())) {
        File file = SD.open(path.c_str(), FILE_READ);
        canvas.drawPng(&file, x, 30, 50, 50, 0, 0);
    } else {
        canvas.drawPng(hydra, 435, x, 30, 50, 50, 14, 14, 2, 2);
    }
    canvas.drawCenterString(text, 120, 90);
    canvas.pushSprite(0, 0);
    if (waitMode == 0) {
        waitForKey();
    } else {
        delay(waitMode);
    }
}

std::vector<String> Utils::splitString(const String &s, char delimiter) {
    std::vector<String> result;
    int start = 0;
    int end = s.indexOf(delimiter);
    while (end != -1) {
        result.push_back(s.substring(start, end));
        start = end + 1;
        end = s.indexOf(delimiter, start);
    }
    result.push_back(s.substring(start));
    return result;
}