//
// Created by WauHu on 29.02.2024.
//

#ifndef LAPPCOMPONENT_H
#define LAPPCOMPONENT_H
#include <ComponentApp.h>
#include <WString.h>


class LAppComponent : public Component {
    ComponentApp &app;
    std::function<void()> openfunc;
    std::function<void()> closefunc;
    std::function<void()> tickfunc;
    std::function<void()> drawfunc;
public:
    explicit LAppComponent(ComponentApp &currentapp,
        const std::function<void()> &openfunc,
        const std::function<void()> &closefunc,
        const std::function<void()> &tickfunc,
        const std::function<void()> &drawfunc
    );

    void initComponent() override;
    void renderComponent() override;
    void updateComponent() override;
    void closeComponent() override;
};



#endif //LAPPCOMPONENT_H
