//
// Created by WauHu on 29.02.2024.
//

#include <functional>
#include "LAppComponent.h"

LAppComponent::LAppComponent(ComponentApp &currentapp,
        const std::function<void()> &openfunc,
        const std::function<void()> &closefunc,
        const std::function<void()> &tickfunc,
        const std::function<void()> &drawfunc
    ) : app(currentapp), openfunc(openfunc), closefunc(closefunc), tickfunc(tickfunc), drawfunc(drawfunc) {

}


void LAppComponent::initComponent() {
    openfunc();
}

void LAppComponent::renderComponent() {
    drawfunc();
}

void LAppComponent::updateComponent() {
    tickfunc();
}

void LAppComponent::closeComponent() {
    closefunc();
}
