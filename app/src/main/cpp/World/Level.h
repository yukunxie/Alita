//
// Created by realxie on 2019-10-29.
//

#ifndef ALITA_LEVEL_H
#define ALITA_LEVEL_H

#include "../Base/ObjectBase.h"
#include "SceneComponent.h"
#include "Model.h"

#include <list>

NS_RX_BEGIN

class Level : public ObjectBase
{
public:
    void AddObject(SceneComponent* sceneComponent);
    void AddModel(Model* model);
protected:
    std::list<SceneComponent*> sceneObjects_;
};

NS_RX_END


#endif //ALITA_LEVEL_H
