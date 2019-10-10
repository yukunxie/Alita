//
// Created by realxie on 2019-09-24.
//

#ifndef ALITA_TYPES_H
#define ALITA_TYPES_H

#include "../external/glm/glm.hpp"
#include "../external/glm/gtc/matrix_transform.hpp"

using TVector4 = glm::vec4;
using TVector3 = glm::vec3;
using TVector2 = glm::vec2;

struct TVertex {
    TVector2 pos;
    TVector3 color;
    TVector2 texCoord;
};

#endif //ALITA_TYPES_H
