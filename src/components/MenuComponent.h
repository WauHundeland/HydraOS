//
// Created by Wau Hundeland on 26.01.24.
//

#ifndef CARDPUTERTOOLBOX_MENUCOMPONENT_H
#define CARDPUTERTOOLBOX_MENUCOMPONENT_H

#include "Component.h"
#include "Action.h"
#include <M5Cardputer.h>
#include <map>
#include <string>
#include <cstdio>
#include "SD.h"
#include "../AppManager.h"

// typedefs
typedef enum {
    MENU_TYPE_LIST,
    MENU_TYPE_LIST_WITH_DESCRIPTION,
    MENU_TYPE_LIST_WITH_ICON,
    MENU_TYPE_LIST_WITH_DESCRIPTION_AND_ICON,
} MenuType;

class MenuComponent : public Component {
private:
    std::vector<Action> functions{};
    std::vector<Action>::iterator currentFunctionIterator;

    int selectIndex;
    MenuType menuType;
    std::string iconNamespace;

    ComponentApp &app;
public:
    boolean needRedraw;
    MenuComponent(ComponentApp &currentapp, MenuType menuType, const std::vector<Action> &functions, std::string iconNamespace = "");

    void initComponent() override;
    void renderComponent() override;
    void updateComponent() override;
    void closeComponent() override;
    void forceRerender() override;
};



#endif //CARDPUTERTOOLBOX_MENUCOMPONENT_H
