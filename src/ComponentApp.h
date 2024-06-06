//
// Created by Wau Hundeland on 26.01.24.
//

#ifndef CARDPUTERTOOLBOX_COMPONENTAPP_H
#define CARDPUTERTOOLBOX_COMPONENTAPP_H

#include "App.h"
#include <Component.h>
#include <M5Cardputer.h>
#include <cstdint>

class ComponentApp : public App {
protected:
    Component *currentComponent = nullptr;

public:
    const char *getName() override;

    void onAppOpen() override;

    void onAppClose() override;

    void onAppResume() override;

    void onAppTick() override;

    void draw() override;

    void setComponent(Component *component);

    virtual Component& initApp() = 0;
};


#endif //CARDPUTERTOOLBOX_COMPONENTAPP_H
