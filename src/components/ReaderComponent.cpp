//
// Created by Wau Hundeland on 26.01.24.
//

#include "ReaderComponent.h"

#include <utility>
#include <Utils.h>

#include "ScreenManager.h"



void ReaderComponent::initComponent() {
    canvas.setTextSize(1.7);
}

void ReaderComponent::renderComponent() {
    canvas.pushSprite(0, 20);
}

void ReaderComponent::updateComponent() {
    if (needRedraw) {
        canvas.setTextSize(1.7);
        canvas.fillScreen(TFT_BLACK);
        canvas.setCursor(0, 0);
        canvas.setTextSize(1);
        for (int i = scrollOffset; i < lines.size() && i < scrollOffset + 14; i++) {
            canvas.println(lines[i]);
        }
        needRedraw = false;
    }

    M5Cardputer.update();
    if (M5Cardputer.Keyboard.isPressed()) {
        if (M5Cardputer.Keyboard.isKeyPressed('`')) {
            closeAction.execute(app);
        }
        if (M5Cardputer.Keyboard.isKeyPressed(';')) {
            scrollOffset--;  // Scroll up when arrow up is pressed
            if (scrollOffset < 0) {
                scrollOffset = 0;  // Don't scroll past the first line
            }
            needRedraw = true;
        }
        if (M5Cardputer.Keyboard.isKeyPressed('.')) {
            scrollOffset++;  // Scroll down when arrow down is pressed
            if (scrollOffset >= lines.size()) {
                scrollOffset = lines.size() - 1;  // Don't scroll past the last line
            }
            needRedraw = true;
        }
    }

}

void ReaderComponent::closeComponent() {
}

ReaderComponent::ReaderComponent(ComponentApp &currentapp, const ReaderType readerType, const std::vector<String> &lines,
                             Action action)
    : app(currentapp), readerType(readerType), lines(lines), closeAction(std::move(action)) {
    // reset all fields
    scrollOffset = 0;
};
