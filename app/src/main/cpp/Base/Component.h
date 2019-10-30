//
// Created by realxie on 2019-10-29.
//

#ifndef ALITA_COMPONENT_H
#define ALITA_COMPONENT_H

#include "Macros.h"
#include "ObjectBase.h"

NS_RX_BEGIN

class Entity;

class Component : public ObjectBase
{
public:
    void SetEntity(Entity* entity);

protected:
    Entity* owner_ = nullptr;
};

NS_RX_END

#endif //ALITA_COMPONENT_H
