//
// Created by realxie on 2019-10-29.
//

#include "Level.h"

NS_RX_BEGIN

void Level::AddObject(SceneComponent* sceneComponent)
{
    sceneObjects_.insert(sceneObjects_.end(), sceneComponent);
}

void Level::AddModel(Model* model)
{
    auto sceneComponent = model->GetComponent<SceneComponent>();
    if (sceneComponent)
    {
        sceneObjects_.insert(sceneObjects_.end(), sceneComponent);
    }
}

NS_RX_END