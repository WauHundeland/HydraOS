//
// Created by Wau Hundeland on 26.01.24.
//

#ifndef CARDPUTERTOOLBOX_ACTION_H
#define CARDPUTERTOOLBOX_ACTION_H

#include <WString.h>
#include <string>
#include "App.h"
#include <Component.h>
#include "ComponentApp.h"
#include "AppManager.h"

// action types
typedef enum {
    ACTION_TYPE_NONE,
    ACTION_TYPE_APP,
    ACTION_TYPE_FUNCTION,
    ACTION_TYPE_COMPONENT,
} ActionType;

class Action {
private:
    ActionType type = ACTION_TYPE_NONE;
    std::string id;
    std::string name;
    std::string description;
    std::string icon;

    // specific targets
    std::string app;
    std::function<void()> function;
    Component *component = nullptr;

public:
    //Action(std::string id, std::string name, std::string description, std::string icon, Component *component);
    Action(std::string id, std::string name, std::string description, std::string icon, std::function<void()> function);
    //Action(std::string id, std::string name, std::string description, std::string icon, std::string app);

    // getters
    ActionType getType() const;
    std::string getId() const;
    std::string getName() const;
    std::string getDescription() const;
    std::string getIcon() const;
    std::basic_string<char> getApp() const;
    std::function<void()> getFunction() const;
    Component *getComponent();

    // executer
    void execute(ComponentApp &componentApp) const;
};


#endif //CARDPUTERTOOLBOX_ACTION_H
