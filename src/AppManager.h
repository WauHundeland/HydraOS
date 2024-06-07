#pragma once

#include <map>
#include <string>
#include <WString.h>
#include "App.h"

class AppManager {
private:
    std::map<std::string, App *> apps{};
    AppManager();
public:
    App *currentApp = nullptr;

    static AppManager& getInstance();
    void addApp(const std::string& name, App* app);
    void openApp(const std::string& name);
    void closeCurrentApp();
    void tickCurrentApp() const;
    void draw() const;
    String getCurrentAppName() const;
    std::map<std::string, App *> listApps();
    App* getApp(const std::string& name);

};