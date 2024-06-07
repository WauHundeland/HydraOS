#include "AppManager.h"
#include <SD.h>
#include <M5Cardputer.h>
#include "assets/hydra.h"
#include "ScreenManager.h"
#include "Utils.h"

AppManager::AppManager() = default;

AppManager& AppManager::getInstance() {
    static AppManager instance;
    return instance;
}

void AppManager::addApp(const std::string& name, App* app) {
    apps[name] = app;
}

void AppManager::openApp(const std::string& name) {
    if (currentApp != nullptr) {
        currentApp->onAppClose();
    }

    // try to find the app by name
    if (apps.find(name) == apps.end()) {
        Utils::popup("Error: App not found", 5000);
        return;
    }

    currentApp = apps[name];
    Utils::initCanvas();
    currentApp->onAppOpen();
}

void AppManager::closeCurrentApp() {
    if (currentApp != nullptr) {
        currentApp->onAppClose();
        currentApp = nullptr;
    }
}

void AppManager::tickCurrentApp() const {
    if (currentApp != nullptr) {
        currentApp->onAppTick();
    }
}

void AppManager::draw() const {
    if (currentApp != nullptr) {
        currentApp->draw();
    }
}

std::map<std::string, App *> AppManager::listApps() {
    return apps;
}

String AppManager::getCurrentAppName() const {
    if (currentApp != nullptr) {
        return currentApp->getName();
    } else {
        return "Loading...";
    }
}

App *AppManager::getApp(const std::string &name) {
    return apps[name];
}
