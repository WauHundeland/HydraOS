//
// Created by WauHu on 05.06.2024.
//
#include "App.h"
#include "AppManager.h"
#include "M5Cardputer.h"
#include "ScreenManager.h"
#include <string>
#include "Utils.h"
#include <Adafruit_NeoPixel.h>
#include "icon.h"
#include "Preferences.h"

#pragma once

class LEDApp : public App {
private:
    int r = 255;
    int g = 255;
    int b = 255;
    int selectedColor;
    bool needsRedraw = true;
    Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, 21, NEO_GRB + NEO_KHZ800);
    Preferences preferences;
public:
    const char *getName() override { return "LEDApp"; }

    void onAppOpen() override {
        preferences.begin("hydra_ledapp", false);
        canvas.fillScreen(BLACK);
        StatusBar::draw(true);
        needsRedraw = true;
        r = 0;
        g = 0;
        b = 0;
        if (preferences.isKey("r")) {
            r = preferences.getInt("r");
            g = preferences.getInt("g");
            b = preferences.getInt("b");
        }
    }

    void onAppClose() override {
        pixels.setPixelColor(0, Adafruit_NeoPixel::Color(0, 0, 0));
        pixels.show();
    }

    void onAppTick() override {
        if (needsRedraw) {
            Utils::initCanvas();
            StatusBar::draw(true);
            // draw the color picker
            canvas.setTextSize(1.5);
            canvas.drawCenterString(String(r), 60, 67);
            canvas.drawCenterString(String(g), 120, 67);
            canvas.drawCenterString(String(b), 180, 67);
            // draw change indicator
            canvas.drawCenterString("/\\", 60 + (selectedColor * 60), 40);
            canvas.drawCenterString("\\/", 60 + (selectedColor * 60), 90);
            canvas.pushSprite(0, 0);
            needsRedraw = false;
            pixels.setBrightness(255);
            pixels.setPixelColor(0, Adafruit_NeoPixel::Color(r, g, b));
            pixels.show();
            preferences.putInt("r", r);
            preferences.putInt("g", g);
            preferences.putInt("b", b);
        }
        Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
        if (M5Cardputer.Keyboard.isChange()) {
            if (M5Cardputer.Keyboard.isKeyPressed(',')) {
                selectedColor--;
                if (selectedColor < 0) {
                    selectedColor = 2;
                }
                needsRedraw = true;
            }
            if (M5Cardputer.Keyboard.isKeyPressed('/')) {
                selectedColor++;
                if (selectedColor >= 3) {
                    selectedColor = 0;
                }
                needsRedraw = true;
            }
            if (M5Cardputer.Keyboard.isKeyPressed(';')) {
                if (selectedColor == 0) {
                    r+= 5;
                    if (r > 255) {
                        r = 0;
                    }
                } else if (selectedColor == 1) {
                    g+= 5;
                    if (g > 255) {
                        g = 0;
                    }
                } else if (selectedColor == 2) {
                    b+= 5;
                    if (b > 255) {
                        b = 0;
                    }
                }
            }
            if (M5Cardputer.Keyboard.isKeyPressed('.')) {
                if (selectedColor == 0) {
                    r-= 5;
                    if (r < 0) {
                        r = 255;
                    }
                } else if (selectedColor == 1) {
                    g-= 5;
                    if (g < 0) {
                        g = 255;
                    }
                } else if (selectedColor == 2) {
                    b-= 5;
                    if (b < 0) {
                        b = 255;
                    }
                }
            }
        }
        if (M5Cardputer.Keyboard.isPressed()) {
            needsRedraw = true;
            delay(35);
        }
    }

    void draw() override {

    }

    const uint8_t *getIcon() override {
        return ledapp_icon;
    }

    int getIconSize() override {
        return sizeof(ledapp_icon);
    }
};