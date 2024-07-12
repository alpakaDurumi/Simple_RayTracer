#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Object.h"
#include "Triangle.h"

class Square : public Object {
public:
    Triangle triangle1, triangle2;

    Square(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, const glm::vec3& color = glm::vec3(1.0f));
    virtual Hit CheckRayCollision(Ray& ray);
};