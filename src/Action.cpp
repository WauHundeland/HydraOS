//
// Created by Wau Hundeland on 26.01.24.
//

#include "Action.h"

#include <utility>

// constructors
Action::Action(std::string id, std::string name, std::string description, std::string icon, std::function<void()> function)
    : id(std::move(id)), name(std::move(name)), description(std::move(description)), icon(std::move(icon)), function(std::move(function)) {
    type = ACTION_TYPE_FUNCTION;
}

// getters
ActionType Action::getType() const {
    return type;
}

std::string Action::getId() const {
    return id;
}

std::string Action::getName() const {
    return name;
}

std::string Action::getDescription() const {
    return description;
}

std::string Action::getIcon() const {
    return icon;
}

std::basic_string<char> Action::getApp() const {
    return app;
}

std::function<void()> Action::getFunction() const {
    return function;
}

Component *Action::getComponent() {
    return component;
}

// executer

void Action::execute(ComponentApp &componentApp) const {
    switch (type) {
        case ACTION_TYPE_APP:
            AppManager::getInstance().openApp(app);
            break;
        case ACTION_TYPE_FUNCTION:
            function();
            break;
        case ACTION_TYPE_COMPONENT:
            componentApp.setComponent(component);
            break;
        default:
            break;
    }
}

