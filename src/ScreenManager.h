//
// Created by WauHu on 13.01.2024.
//

#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#pragma once

#include "M5Cardputer.h"
#include "M5GFX.h"

class StatusBar {
public:
    static void draw(bool force = false);
};

class ScreenManager {
public:
    static M5Canvas getCanvas();
};

extern M5Canvas canvas; // NOLINT(*-interfaces-global-init)

#endif //SCREENMANAGER_H
