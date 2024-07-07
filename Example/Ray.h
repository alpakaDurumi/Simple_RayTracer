#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

class Ray
{
public:
    glm::vec3 start; // ray의 시작 지점
    glm::vec3 dir;   // ray의 방향
};
