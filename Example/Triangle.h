#pragma once

// glm은 Object.h에서 이미 include 되어있지만, 명시적으로 해주었음. 이는 Square.h 에서도 동일
// 헤더가드 덕분에 중복정의 문제는 피할 수 있음
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
        const glm::vec3& color = glm::vec3(1.0f));
    virtual Hit CheckRayCollision(const Ray& ray);
    void IntersectRayTriangle(const Ray& ray, Hit& hit);
};