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
        const glm::vec2& uv0 = glm::vec2(0.0f), const glm::vec2& uv1 = glm::vec2(1.0f, 0.0f), const glm::vec2& uv2 = glm::vec2(1.0f), const glm::vec2& uv3 = glm::vec2(0.0f, 1.0f),
        const std::shared_ptr<Material>& mat = nullptr);

    Hit CheckRayCollision(const Ray& ray) override;
    void setTexture(
        const std::shared_ptr<Texture>& ambTexture = nullptr,
        const std::shared_ptr<Texture>& difTexture = nullptr,
        const std::shared_ptr<Texture>& normalMap = nullptr);
};