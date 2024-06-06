//
// Created by Wau Hundeland on 26.01.24.
//

#ifndef CARDPUTERTOOLBOX_READERCOMPONENT_H
#define CARDPUTERTOOLBOX_READERCOMPONENT_H

#include "Component.h"
#include "ComponentApp.h"
#include "Action.h"
#include <M5Cardputer.h>
#include <map>
#include <string>
#include <cstdio>
#include "SD.h"
#include "../AppManager.h"

// typedefs
typedef enum {
    READER_TYPE_LIST,
    READER_TYPE_LIST_WITH_DESCRIPTION,
    READER_TYPE_LIST_WITH_ICON,
    READER_TYPE_LIST_WITH_DESCRIPTION_AND_ICON,
} ReaderType;

class ReaderComponent : public Component {
private:
    int scrollOffset = 0;  // Add this line
    std::vector<String> lines;  // Add this line

    ComponentApp &app;
    ReaderType readerType;
    Action closeAction;
    boolean needRedraw = true;

public:
    ReaderComponent(ComponentApp &currentapp, ReaderType readerType, const std::vector<String> &lines,
                 Action action);

    void initComponent() override;
    void renderComponent() override;
    void updateComponent() override;
    void closeComponent() override;
};



#endif //CARDPUTERTOOLBOX_READERCOMPONENT_H
