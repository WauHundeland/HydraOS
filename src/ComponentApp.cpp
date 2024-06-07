#include "ComponentApp.h"

const char *ComponentApp::getName() {
    return nullptr;
}

void ComponentApp::onAppOpen() {
    currentComponent = &initApp();
    currentComponent->initComponent();
}

void ComponentApp::onAppClose() {
    currentComponent->closeComponent();
}

void ComponentApp::onAppTick() {
    currentComponent->updateComponent();
}

void ComponentApp::draw() {
    currentComponent->renderComponent();
}

void ComponentApp::setComponent(Component *component) {
    currentComponent->closeComponent();
    currentComponent = component;
    currentComponent->initComponent();
}
