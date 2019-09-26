//
// Created by realxie on 2019-09-24.
//

#ifndef VULKANDEMO_TYPES_H
#define VULKANDEMO_TYPES_H

#include "../external/glm/glm.hpp"

using TVector4 = glm::vec4;
using TVector3 = glm::vec3;
using TVector2 = glm::vec2;

struct TVertex {
    TVector2 pos;
    TVector3 color;
};

#endif //VULKANDEMO_TYPES_H
