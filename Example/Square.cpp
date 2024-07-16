#include "Triangle.h"
#include "Square.h"
#include "Hit.h"

// clockwise ������ �µ��� �� �ﰢ���� vertex ����
Square::Square(
    const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3,
    const glm::vec2& uv0, const glm::vec2& uv1, const glm::vec2& uv2, const glm::vec2& uv3,
    const glm::vec3& color)
    : triangle1(v0, v1, v2, uv0, uv1, uv2), triangle2(v0, v2, v3, uv0, uv2, uv3), Object(color) {}

Hit Square::CheckRayCollision(Ray& ray) {
    auto hit1 = triangle1.CheckRayCollision(ray);
    auto hit2 = triangle2.CheckRayCollision(ray);
    
    // 1. �� �ﰢ�� ��ο� �浹�ϴ� ���, ����� ���� ����
    // 2. triangle1�� �浹�ϴ� ���
    // 3. triangle3�� �浹�ϴ� ���
    if (hit1.d >= 0.0f && hit2.d >= 0.0f)
        return hit1.d < hit2.d ? hit1 : hit2;
    else if (hit1.d >= 0.0f)
        return hit1;
    else
        return hit2;
}