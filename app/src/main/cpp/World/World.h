//
// Created by realxie on 2019-10-25.
//

#ifndef ALITA_WORLD_H
#define ALITA_WORLD_H

#include "../Base/Macros.h"
#include "SceneComponent.h"
#include "Level.h"

NS_RX_BEGIN

class World : public ObjectBase
{
public:
    World();
    
    void Update(float dt);
    
    void AddLevel(Level* level)
    {
        levels_.push_back(level);
    }

protected:
    std::vector<Level*> levels_;
};

NS_RX_END

#endif //ALITA_WORLD_H
