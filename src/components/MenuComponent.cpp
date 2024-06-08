//
// Created by Wau Hundeland on 26.01.24.
//

#include "MenuComponent.h"

#include <utility>
#include "ScreenManager.h"
#include "Utils.h"
#include "assets/hydra.h"


void MenuComponent::initComponent() {
    Utils::initCanvas();
    StatusBar::draw();
    canvas.setTextSize(2);

    needRedraw = true;
    drawFinished = false;
    selectIndex = 0;
}

void MenuComponent::renderComponent() {
    if (!drawFinished) {
        return;
    }
    canvas.pushSprite(0, 0);
}

void MenuComponent::updateComponent() {
    if (needRedraw) {
        StatusBar::draw();
        canvas.setTextSize(2);
        canvas.setTextColor(WHITE);

        // Get the app at the selectIndex
        auto it = functions.begin();
        std::advance(it, selectIndex);
        auto const &function = *it;

        int x = 10;
        int y = 28;
        int offset = 0;

        // if mode is one of the icon modes, draw the icon
        if (menuType == MENU_TYPE_LIST_WITH_ICON || menuType == MENU_TYPE_LIST_WITH_DESCRIPTION_AND_ICON) {
            // Draw the app icon and name
            std::string iconPath = "/icons/" + iconNamespace + "/" + function.getIcon() + ".png";
            int y2 = ((135 - 50) / 2);

            if (SD.exists(iconPath.c_str())) {
                File file = SD.open(iconPath.c_str(), FILE_READ);
                canvas.drawPng(&file, x, y2, 50, 50);
                file.close();
            } else {
                // If the icon does not exist, use a default icon
                canvas.drawPng(hydra, 435, x, y2, 50, 50, 14, 14, 2, 2);
            }
            // Draw a square around the icon
            canvas.drawRect(x, y2, 50, 50, WHITE);
            offset = 60;
        }

        // Draw the list of apps
        canvas.setCursor(x + offset, y);

        int i = 0;
        canvas.setTextSize(2);
        int appsSize = functions.size();
        for (auto const &function: functions) {
            if (menuType == MENU_TYPE_LIST_WITH_DESCRIPTION || menuType == MENU_TYPE_LIST_WITH_DESCRIPTION_AND_ICON) {
                // display the last, the current and the next app
                // if the selected app is the first or last, adjust the display to still show 5 apps
                if ((selectIndex < 1 && i < 3) ||
                    (selectIndex >= 1 && selectIndex < appsSize - 1 && i >= selectIndex - 1 && i <= selectIndex + 1) ||
                    (selectIndex >= appsSize - 1 && i >= appsSize - 3)) {
                    // current app: green
                    if (i == selectIndex) {
                        canvas.setTextColor(GREEN);
                    } else {
                        canvas.setTextColor(WHITE);
                    }
                    canvas.println(function.getName().c_str());
                    canvas.setCursor(x + offset, canvas.getCursorY());
                }
            } else {
                // display the 2 last, the current and the next 2 apps
                if ((selectIndex < 3 && i < 6) ||
                    (selectIndex >= 3 && selectIndex < appsSize - 2 && i >= selectIndex - 3 && i <= selectIndex + 2) ||
                    (selectIndex >= appsSize - 2 && i >= appsSize - 6)) {
                    // current app: green
                    if (i == selectIndex) {
                        canvas.setTextColor(GREEN);
                    } else {
                        canvas.setTextColor(WHITE);
                    }
                    canvas.println(function.getName().c_str());
                    canvas.setCursor(x + offset, canvas.getCursorY());
                }
            }
            i++;
        }

        if (menuType == MENU_TYPE_LIST_WITH_DESCRIPTION || menuType == MENU_TYPE_LIST_WITH_DESCRIPTION_AND_ICON) {
            // Draw the description of the app
            canvas.setTextColor(LIGHTGREY);
            canvas.setTextSize(1.5);
            canvas.setCursor(x + offset, canvas.getCursorY() + 10);
            // split the description into lines of 17 characters
            std::string description = function.getDescription();
            int descriptionLength = description.length();
            int descriptionIndex = 0;
            while (descriptionIndex < descriptionLength) {
                if (menuType == MENU_TYPE_LIST_WITH_DESCRIPTION_AND_ICON) {
                    canvas.println(description.substr(descriptionIndex, 17).c_str());
                    descriptionIndex += 17;
                } else {
                    canvas.println(description.substr(descriptionIndex, 25).c_str());
                    descriptionIndex += 25;
                }
                canvas.setCursor(x + offset, canvas.getCursorY());
            }
        }

        needRedraw = false;
        drawFinished = true;
    }
    M5Cardputer.update();
    if (M5Cardputer.Keyboard.isChange()) {
        if (M5Cardputer.Keyboard.isPressed()) {
            Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
            if (status.enter) {
                // Get the action at the selectIndex
                auto it = functions.begin();
                std::advance(it, selectIndex);
                Action function = *it;
                function.execute(app);
            }
            for (auto i: status.hid_keys) {
                switch (i) {
                    case 0x33: //";" LeftArrow
                        selectIndex--;
                        if (selectIndex < 0) {
                            selectIndex = functions.size() - 1;
                        }
                        needRedraw = true;
                        break;
                    case 0x37: //"." RightArrow
                        selectIndex++;
                        if (selectIndex >= functions.size()) {
                            selectIndex = 0;
                        }
                        needRedraw = true;
                        break;
                }
            }
        }
    }
    delay(10);
}

void MenuComponent::closeComponent() {
}

void MenuComponent::forceRerender() {
    needRedraw = true;
}

MenuComponent::MenuComponent(ComponentApp &currentapp, const MenuType menuType, const std::vector<Action> &functions,
                             std::string iconNamespace)
    : app(currentapp), menuType(menuType), functions(functions), iconNamespace(std::move(iconNamespace)) {
    // reset all fields
    selectIndex = 0;
    needRedraw = true;
};
