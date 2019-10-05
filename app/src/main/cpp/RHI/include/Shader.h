//
// Created by realxie on 2019-10-04.
//

#ifndef ALITA_SHADER_H
#define ALITA_SHADER_H


#include "Macros.h"

#include <memory>

NS_RHI_BEGIN

class Shader;

typedef std::shared_ptr<Shader> ShaderHnd;

class Shader
{
public:
    virtual ~Shader()
    {
    }
};

NS_RHI_END

#endif //ALITA_SHADER_H
