//
// Created by realxie on 2019-10-29.
//

#include "Model.h"
#include "SceneComponent.h"

NS_RX_BEGIN

Model::Model()
{
    AddComponment(new SceneComponent());
}

NS_RX_END