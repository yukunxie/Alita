//
// Created by realxie on 2019-10-29.
//

#ifndef ALITA_MODEL_H
#define ALITA_MODEL_H

#include "../Base/ObjectBase.h"
#include "../Base/Entity.h"
#include "../Meshes/Mesh.h"

NS_RX_BEGIN

class Model : public Entity
{
public:
    Model();
    
    virtual ~Model();
    
    void SetMesh(Mesh* mesh);

protected:
    Mesh* mesh_ = nullptr;
};

NS_RX_END

#endif //ALITA_MODEL_H
