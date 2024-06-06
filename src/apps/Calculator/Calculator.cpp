//
// Created by WauHu on 13.01.2024.
//
#include "App.h"
#include "AppManager.h"
#include "M5Cardputer.h"
#include "ScreenManager.h"
#include <string>
#include "Utils.h"
#include "tinyexpr.h"
#include "icon.h"

#pragma once

class Calculator : public App {
private:
    std::vector<String> history;
    double x;
    double y;
    double z;
public:
    const char *getName() override { return "Calculator"; }

    void onAppOpen() override {
        canvas.fillScreen(BLACK);
        StatusBar::draw(true);
    }

    void onAppClose() override {
    }

    void onAppResume() override {
        // Code to run when App1 resumes
    }

    void onAppTick() override {
        Utils::initCanvas();
        StatusBar::draw(true);
        String input = "";
        canvas.setTextSize(1);
        canvas.setTextColor(WHITE);
        canvas.drawFastHLine(5, 115, 230, WHITE);
        // draw last 4 lines of history
        for (int i = 0; i < 8 && i < history.size(); i++) {
            canvas.setCursor(7, 30 + (7 - i) * 8);
            canvas.print(history[history.size() - i - 1]);
        }
        canvas.setCursor(7, 107);
        canvas.print("X = " + String(x) + " Y = " + String(y) + " Z = " + String(z));
        canvas.setCursor(7, 117);
        canvas.pushSprite(0, 0);
        Utils::waitForInput(input);
        if (input == "exit") {
            AppManager::getInstance().closeCurrentApp();
            AppManager::getInstance().openApp("AppLauncher");
            StatusBar::draw(true);
        } else {
            // if input contains ONE "=" then assign the var on the left with the value on the right
            if (input.indexOf("=") != -1) {
                String var = input.substring(0, input.indexOf("="));
                String value = input.substring(input.indexOf("=") + 1);
                double result = te_interp(value.c_str(), 0);
                if (var == "x") {
                    x = result;
                } else if (var == "y") {
                    y = result;
                } else if (var == "z") {
                    z = result;
                } else {
                    history.emplace_back("Please use x or y or z, got " + var);
                    return;
                }
                history.emplace_back(var + " = " + String(result));
                return;
            }

            te_variable varsArray[] = {
                {"x", &x},
                {"y", &y},
                {"z", &z},
            };
            int err;
            te_expr *expr = te_compile(input.c_str(), varsArray, 3, &err);
            if (expr) {
                double result = te_eval(expr);
                history.emplace_back(input + " = " + String(result));
                te_free(expr);
            } else {
                char buffer[100];
                sprintf(buffer, "\t%*s^ Error", err - 1, "");
                history.emplace_back(input);
                history.emplace_back(buffer);
            }
        }
    }

    void draw() override {
    }

    const uint8_t *getIcon() override {
        return calc_icon;
    }

    int getIconSize() override {
        return sizeof(calc_icon);
    }
};