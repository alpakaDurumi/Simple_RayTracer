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

public:
    Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& color = glm::vec3(1.0f));
    virtual Hit CheckRayCollision(Ray& ray);
    void IntersectRayTriangle(const Ray& ray, Hit& hit);
};