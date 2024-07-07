#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Object.h"

class Sphere : public Object
{
public:
    glm::vec3 center;       // 구의 중심 좌표
    float radius;           // 구의 반지름

    Sphere(const glm::vec3& center, const float radius, const glm::vec3& color = glm::vec3(1.0f));
    Hit CheckRayCollision(Ray& ray);
};
