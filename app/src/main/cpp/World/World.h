//
// Created by realxie on 2019-10-25.
//

#ifndef ALITA_WORLD_H
#define ALITA_WORLD_H

#include "../Base/Macros.h"
#include "SceneComponent.h"
#include "Level.h"

NS_RX_BEGIN

class World
{
public:
    World();

protected:
    std::vector<Level*> levels_;
};

NS_RX_END

#endif //ALITA_WORLD_H
