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

Model::~Model()
{
    RX_SAFE_RELEASE(mesh_);
}

void Model::SetMesh(Mesh* mesh)
{
    RX_SAFE_RETAIN(mesh);
    RX_SAFE_RELEASE(mesh_);
    mesh_ = mesh;
}

NS_RX_END