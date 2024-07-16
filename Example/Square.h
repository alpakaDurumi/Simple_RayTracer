#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Object.h"
#include "Triangle.h"

class Square : public Object {
public:
    Triangle triangle1, triangle2;

    Square(
        const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3,
        const glm::vec2& uv0 = glm::vec2(0.0f), const glm::vec2& uv1 = glm::vec2(0.0f), const glm::vec2& uv2 = glm::vec2(0.0f), const glm::vec2& uv3 = glm::vec2(0.0f),
        const glm::vec3& color = glm::vec3(1.0f));
    virtual Hit CheckRayCollision(Ray& ray);
};