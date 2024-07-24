#pragma once

// glm�� Object.h���� �̹� include �Ǿ�������, ��������� ���־���. �̴� Square.h ������ ����
// ������� ���п� �ߺ����� ������ ���� �� ����
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Object.h"

// ���� ����
class Hit;
class Ray;

class Triangle : public Object {
public:
    glm::vec3 v0, v1, v2;       // �� ������ ��ǥ
    glm::vec2 uv0, uv1, uv2;    // �ؽ�ó ��ǥ

public:
    Triangle(
        const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
        const glm::vec2& uv0 = glm::vec2(0.0f), const glm::vec2& uv1 = glm::vec2(0.0f), const glm::vec2& uv2 = glm::vec2(0.0f),
        const glm::vec3& color = glm::vec3(1.0f));
    virtual Hit CheckRayCollision(const Ray& ray);
    void IntersectRayTriangle(const Ray& ray, Hit& hit);
};