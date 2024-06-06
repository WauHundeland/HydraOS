#include "ScreenManager.h"
#include "AppManager.h"

M5Canvas canvas = M5Canvas(&M5Cardputer.Lcd);

void StatusBar::draw(bool force) {
    canvas.fillScreen(BLACK);
    canvas.setTextSize(1);
    canvas.setTextColor(WHITE);
    canvas.createSprite(240, 130);
    canvas.drawRoundRect(5, 5, 230, 125, 7, WHITE);
    canvas.drawFastHLine(5, 20, 230, WHITE);
    canvas.drawString("HydraOS 2.0.0", 10, 9);
    canvas.drawRightString(AppManager::getInstance().getCurrentAppName(), 230, 9);
}

M5Canvas ScreenManager::getCanvas() {
    return canvas;
}