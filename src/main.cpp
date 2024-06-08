#include <Arduino.h>
#include "assets/hydra.h"
#include "M5Cardputer.h"
#include "SPI.h"
#include "AppManager.h"
#include "ScreenManager.h"
#include "SD.h"
#include "apps/Calculator/Calculator.cpp"
#include "apps/Launcher/Launcher.cpp"
#include "apps/Settings/Settings.cpp"
#include "apps/LEDApp/LEDApp.cpp"
#include "apps/DinoRunner/DinoRunner.cpp"
#include "apps/AudioRecorder/AudioRec.cpp"
#include "Preferences.h"

void base(const String &text = "") {
    canvas.fillScreen(BLACK);
    canvas.setTextSize(1);
    canvas.drawRoundRect(5, 5, 230, 125, 7, WHITE);
    canvas.drawFastHLine(5, 20, 230, WHITE);
    canvas.drawString("HydraOS 2.0.0", 10, 9);
    canvas.drawRightString(text, 230, 9);
}
// the remaining area is 230x105 pixels

void hydraOSLogoAnimation(int percent) {
    base();
    // Draw the rabbitOS logo partially as by the given percent
    int width = 64;
    // get top left coordinates for img (screen is M5Cardputer.Display.width() x M5Cardputer.Display.height() pixels)
    int x = ((240 - width) / 2);
    int y = ((135 - width) / 2) + 5;
    canvas.drawPng(hydra, 435, x, y, width, width, 0, 0, 2, 2);
    // draw gradient line as bottom border based on percent
    int lineWidth = 230 * percent / 100;
    for (int i = 0; i < 5; i++) {
        canvas.drawGradientHLine(5, 130 - 1 - i, lineWidth + 3, GREEN, CYAN);
    }
    canvas.drawCenterString("Press any key to skip", 120, 110);
    canvas.pushSprite(0, 0);
}

SPIClass SPI2;

void setup() {
    Serial.begin(115200);
    Serial.println("Starting HydraOS");
    Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, 21, NEO_GRB + NEO_KHZ800);
    pixels.setPixelColor(0, Adafruit_NeoPixel::Color(0, 0, 0));
    pixels.show();
    Preferences preferences;
    auto cfg = m5::M5Unified::config();
    M5Cardputer.begin(cfg, true);
    SPI2.begin(
            m5::M5Unified::getPin(m5::pin_name_t::sd_spi_sclk),
            m5::M5Unified::getPin(m5::pin_name_t::sd_spi_miso),
            m5::M5Unified::getPin(m5::pin_name_t::sd_spi_mosi),
            m5::M5Unified::getPin(m5::pin_name_t::sd_spi_ss)
    );
    M5Cardputer.update();
    M5Cardputer.Display.setColorDepth(8);
    M5Cardputer.Display.setRotation(1);
    canvas.createSprite(240, 135);
    if (!SD.begin(m5::M5Unified::getPin(m5::pin_name_t::sd_spi_ss), SPI2)) {
        Serial.println("SD Card Mount Failed");
        Utils::popup("SD Card Mount Failed\n"
                     "You don't need an SD anyway", 0);
    }
    preferences.begin("hydra_base", false);
    if (preferences.isKey("ssid")) {
        WiFiClass::mode(WIFI_STA);
        WiFi.begin(preferences.getString("ssid").c_str(), preferences.getString("pass").c_str());
    }
    pin:
    // ask for pin code
    if (preferences.isKey("pin")) {
        Utils::initCanvas();
        base("Security");
        canvas.setTextSize(1.5);
        canvas.drawString("Enter pin code", 7, 30);
        canvas.drawFastHLine(5, 50, 230, WHITE);
        canvas.drawFastHLine(5, 74, 230, WHITE);
        canvas.setCursor(7, 53);
        canvas.setTextSize(2);
        canvas.pushSprite(0, 0);
        String pin = "";
        Utils::centerInput(pin, 4, 18, true);
        if (pin != preferences.getString("pin")) {
            Utils::popup("Wrong pin code", 0);
            goto pin;
        }
    }
    //for (int i = 0; i < 100; i++) {
    //    hydraOSLogoAnimation(i);
    //    delay(15);
    //    M5Cardputer.update();
    //    if (M5Cardputer.Keyboard.isPressed()) {
    //        break;
    //    }
    //}
    Utils::initCanvas();
    AppManager::getInstance().addApp("AppLauncher", new Launcher());
    AppManager::getInstance().addApp("Calculator", new Calculator());
    AppManager::getInstance().addApp("Settings", new Settings());
    AppManager::getInstance().addApp("LEDApp", new LEDApp());
    AppManager::getInstance().addApp("DinoRunner", new DinoRunner());
    AppManager::getInstance().addApp("AudioRecorder", new AudioRecorder());
    // add more apps here
    AppManager::getInstance().openApp("AppLauncher");
}

void loop() {
    M5Cardputer.update();

    if (M5.BtnA.wasPressed() && AppManager::getInstance().getCurrentAppName() != "AppLauncher") {
        AppManager::getInstance().closeCurrentApp();
        AppManager::getInstance().openApp("AppLauncher");
    }

    AppManager::getInstance().tickCurrentApp();
    AppManager::getInstance().draw(); // Add this line

    delay(15);
}