#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Object.h"

// 전방 선언
class Hit;
class Ray;

class Triangle : public Object {
public:
    glm::vec3 v0, v1, v2;       // 세 정점의 좌표
    glm::vec2 uv0, uv1, uv2;    // 텍스처 좌표

public:
    Triangle(
        const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
        const glm::vec2& uv0 = glm::vec2(0.0f), const glm::vec2& uv1 = glm::vec2(0.0f), const glm::vec2& uv2 = glm::vec2(0.0f),
        const std::shared_ptr<Material>& mat = nullptr);
    virtual Hit CheckRayCollision(const Ray& ray);
    void IntersectRayTriangle(const Ray& ray, Hit& hit);
};