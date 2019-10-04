//
// Created by realxie on 2019-10-04.
//

#ifndef VULKANDEMO_SHADER_H
#define VULKANDEMO_SHADER_H


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

#endif //VULKANDEMO_SHADER_H
