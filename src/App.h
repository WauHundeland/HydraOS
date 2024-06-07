#pragma once
#include "cstdint"

class App {
public:
    // destructor
    virtual ~App() = default;

    // app name
    virtual const char* getName() {
        return nullptr;
    };

    // app lifecycle
    virtual void onAppOpen() = 0;
    virtual void onAppClose() = 0;
    virtual void onAppTick() = 0;
    virtual void draw() = 0;
    virtual const uint8_t* getIcon() {
        return nullptr;
    };
    virtual int getIconSize() {
        return 0;
    };
};

