//
// Created by WauHu on 05.06.2024.
//

#ifndef HYDRAOS_SETTINGS_CPP
#define HYDRAOS_SETTINGS_CPP

#include <WString.h>
#include <vector>
#include "ComponentApp.h"
#include "Action.h"
#include "components/MenuComponent.h"
#include "Utils.h"
#include <WiFi.h>
#include "ScreenManager.h"
#include "Preferences.h"
#include "icon.h"

class Settings : public ComponentApp {
private:
    Preferences preferences;
public:
    const char *getName() override {
        return "Settings";
    }

    Component &initApp() override {
        preferences.begin("hydra_base", false);
        std::vector<Action> actions;

        actions.emplace_back("wifiinfo", "WiFi Information", "", "", [this]() {
            String preparedString;
            if (WiFiClass::status() != WL_CONNECTED) {
                preparedString = "WiFi Information\nSSID: Not connected";
            } else {
                preparedString =
                        "WiFi Information\nSSID: " + WiFi.SSID() + "\nIP: " + WiFi.localIP().toString() + "\nRSSI: " +
                        WiFi.RSSI() + " dBm";
            }
            Utils::popup(preparedString, 0, 1.5);
            this->currentComponent->forceRerender();
        });

        actions.emplace_back("wifi", "Connect to WiFi", "", "", [this]() {
            if (WiFi.status() == WL_CONNECTED) {
                Utils::popup("Already connected\nSSID:"
                                + WiFi.SSID() + "\nIP: " + WiFi.localIP().toString(), 0, 1.5);
                this->currentComponent->forceRerender();
                return;
            }
            Utils::initCanvas();
            StatusBar::draw(true);
            canvas.setTextSize(1.5);
            canvas.drawString("SSID", 7, 30);
            canvas.drawFastHLine(5, 45, 230, WHITE);
            canvas.drawFastHLine(5, 60, 230, WHITE);
            canvas.setCursor(8, 47);
            canvas.drawString("Press G0 to exit", 7, 65);
            canvas.pushSprite(0, 0);
            String ssid = "";
            Utils::waitForInput(ssid);
            if (ssid == "exit" || ssid == "") {
                this->currentComponent->forceRerender();
                return;
            }
            Utils::initCanvas();
            StatusBar::draw(true);
            canvas.setTextSize(1.5);
            canvas.drawString("SSID: " + ssid, 7, 30);
            canvas.drawString("Password", 7, 50);
            canvas.drawFastHLine(5, 65, 230, WHITE);
            canvas.drawFastHLine(5, 80, 230, WHITE);
            canvas.setCursor(8, 67);
            canvas.drawString("Press G0 to exit", 7, 85);
            canvas.pushSprite(0, 0);
            String pass = "";
            Utils::waitForInput(pass);
            if (pass == "exit" || pass == "") {
                this->currentComponent->forceRerender();
                return;
            }
            Utils::popup("Connecting to WiFi...", 1, 1.5);
            WiFi.mode(WIFI_STA);
            WiFi.begin(ssid.c_str(), pass.c_str());
            int i = 0;
            while (WiFi.status() != WL_CONNECTED && i < 10) {
                delay(1000);
                i++;
            }
            if (WiFi.status() == WL_CONNECTED) {
                Utils::popup("Connected to WiFi\nIP: " + WiFi.localIP().toString(), 0, 1.5);
                // save to esp32 psram
                preferences.putString("ssid", ssid);
                preferences.putString("pass", pass);
            } else {
                Utils::popup("Failed to connect\n to WiFi", 0, 1.5);
            }
            this->currentComponent->forceRerender();
        });

        actions.emplace_back("scan", "Scan WiFi", "", "", [this]() {
            Utils::initCanvas();
            StatusBar::draw(true);
            canvas.drawString("Scanning WiFi... Please wait", 7, 30);
            canvas.pushSprite(0, 0);
            int n = WiFi.scanNetworks();
            Utils::initCanvas();
            StatusBar::draw(true);
            canvas.drawString("Found " + String(n) + " networks", 7, 30);
            canvas.setCursor(8, 42);
            for (int i = 0; i < n; i++) {
                canvas.print(i + 1);
                canvas.print(": ");
                canvas.print(WiFi.SSID(i));
                canvas.print(" (");
                canvas.print(WiFi.RSSI(i));
                canvas.println(" dBm)");
                canvas.setCursor(8, canvas.getCursorY());
                if (i == 8) {
                    break;
                }
            }
            canvas.println("Press any key to continue");
            canvas.pushSprite(0, 0);
            Utils::waitForKey();
            this->currentComponent->forceRerender();
        });

        actions.emplace_back("about", "About HydraOS", "", "", [this]() {
            canvas.setTextColor(WHITE);
            Utils::popup("HydraOS 2.0.0\n"
                         "Created by WauHundeland\n\n"
                         "This is a simple operating system\n for the M5Cardputer.", 0);
            this->currentComponent->forceRerender();
        });

        actions.emplace_back("pin", "Set pin code", "", "", [this]() {
            if (preferences.isKey("pin")) {
                Utils::popup("Pin code already set", 0);
                this->currentComponent->forceRerender();
                return;
            }
            Utils::initCanvas();
            StatusBar::draw(true);
            canvas.setTextSize(1.5);
            canvas.drawString("Enter 4 digit pin code", 7, 30);
            canvas.drawFastHLine(5, 45, 230, WHITE);
            canvas.drawFastHLine(5, 65, 230, WHITE);
            canvas.setCursor(8, 47);
            canvas.drawString("Press G0 to exit", 7, 70);
            canvas.drawString("You have to enter the pin on boot", 7, 84);
            canvas.pushSprite(0, 0);
            String pin = "";
            canvas.setTextSize(2);
            Utils::centerInput(pin, 4, 16, true);
            if (pin == "exit" || pin == "") {
                this->currentComponent->forceRerender();
                return;
            }
            // check if pin is numeric
            for (int i = 0; i < pin.length(); i++) {
                if (!isDigit(pin[i])) {
                    Utils::popup("Pin code must be numeric", 0, 1.5);
                    this->currentComponent->forceRerender();
                    return;
                }
            }
            // confirm pin
            Utils::initCanvas();
            StatusBar::draw(true);
            canvas.setTextSize(1.5);
            canvas.drawString("Confirm pin code", 7, 30);
            canvas.drawFastHLine(5, 45, 230, WHITE);
            canvas.drawFastHLine(5, 65, 230, WHITE);
            canvas.setCursor(8, 47);
            canvas.drawString("Press G0 to abort", 7, 70);
            canvas.pushSprite(0, 0);
            String confirmPin = "";
            canvas.setTextSize(2);
            Utils::centerInput(confirmPin, 4, 16, true);
            if (confirmPin == "exit" || confirmPin == "") {
                this->currentComponent->forceRerender();
                return;
            }
            if (pin != confirmPin) {
                Utils::popup("Pin codes do not match", 0, 1.5);
                this->currentComponent->forceRerender();
                return;
            }
            preferences.putString("pin", pin);
            Utils::popup("Pin code set\n" + pin, 0, 1.5);
            this->currentComponent->forceRerender();
        });

        actions.emplace_back("removepin", "Remove pin code", "", "", [this]() {
            if (!preferences.isKey("pin")) {
                Utils::popup("No pin code set", 0);
                this->currentComponent->forceRerender();
                return;
            }
            Utils::initCanvas();
            StatusBar::draw(true);
            canvas.setTextSize(1.5);
            canvas.drawString("Enter 4 digit pin code", 7, 30);
            canvas.drawFastHLine(5, 45, 230, WHITE);
            canvas.drawFastHLine(5, 65, 230, WHITE);
            canvas.setCursor(8, 47);
            canvas.drawString("Press G0 to exit", 7, 60);
            canvas.pushSprite(0, 0);
            String pin = "";
            canvas.setTextSize(2);
            Utils::centerInput(pin, 4, 16, true);
            if (pin == "exit" || pin == "") {
                this->currentComponent->forceRerender();
                return;
            }
            if (pin != preferences.getString("pin")) {
                Utils::popup("Wrong pin code", 0, 1.5);
                this->currentComponent->forceRerender();
                return;
            }
            preferences.remove("pin");
            Utils::popup("Pin code removed", 0, 1.5);
            this->currentComponent->forceRerender();
        });

        return *new MenuComponent(*this, MenuType::MENU_TYPE_LIST, actions, "");
    }

    const uint8_t *getIcon() override {
        return settings_icon;
    }

    int getIconSize() override {
        return sizeof(settings_icon);
    }
};

#endif //HYDRAOS_SETTINGS_CPP