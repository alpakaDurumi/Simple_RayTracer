#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Object.h"

class Sphere : public Object
{
public:
    glm::vec3 center;       // 구의 중심 좌표
    float radius;           // 구의 반지름

    Sphere(const glm::vec3& center, const float radius, const std::shared_ptr<Material>& mat = nullptr);

    Hit CheckRayCollision(const Ray& ray);
};
