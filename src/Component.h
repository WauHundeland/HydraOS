//
// Created by Wau Hundeland on 26.01.24.
//

#ifndef CARDPUTERTOOLBOX_COMPONENT_H
#define CARDPUTERTOOLBOX_COMPONENT_H

class Component {
public:
    virtual ~Component() = default;

    virtual void initComponent() = 0;
    virtual void renderComponent() = 0;
    virtual void updateComponent() = 0;
    virtual void closeComponent() = 0;
    virtual void forceRerender() {
        renderComponent();
    }
};


#endif //CARDPUTERTOOLBOX_COMPONENT_H
