#include "Triangle.h"
#include "Hit.h"
#include "Ray.h"

// uv 좌표도 clockwise로 주어져야 함
Triangle::Triangle(
    const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
    const glm::vec2& uv0, const glm::vec2& uv1, const glm::vec2& uv2,
    const std::shared_ptr<Material>& mat)
    : v0(v0), v1(v1), v2(v2), uv0(uv0), uv1(uv1), uv2(uv2), Object(mat) {}

Hit Triangle::CheckRayCollision(const Ray& ray) {
    Hit hit = Hit{ -1.0, glm::vec3(0.0), glm::vec3(0.0) };
    IntersectRayTriangle(ray, hit);

    return hit;
}

// 충돌을 검사하고, 여부에 따라 Hit을 설정하는 함수
void Triangle::IntersectRayTriangle(const Ray& ray, Hit& hit) {
    // Ray와 삼각형의 충돌 여부
    bool collided = true;

    // 삼각형의 세 정점으로 정의되는 평면의 노멀 벡터 계산
    // v0, v1, v2가 clockwise 순서여야 한다
    const glm::vec3 faceNormal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

    // Backface culling
    if (glm::dot(-ray.dir, faceNormal) < 0.0f) collided = false;

    // 평면과 Ray의 각도가 평행에 매우 가깝다면 충돌하지 못하는 것으로 판단
    // d 계산시 divide by zero를 방지하는 목적도 있음
    if (glm::abs(glm::dot(ray.dir, faceNormal)) < 1e-2f) collided = false;
    
    // Ray의 시작 지점부터 충돌 지점까지의 거리
    const float distance = (glm::dot(v0, faceNormal) - glm::dot(ray.start, faceNormal)) / (glm::dot(ray.dir, faceNormal));

    // Ray의 시작점 이전에 충돌이 감지된 경우
    if (distance < 0.0f) collided = false;

    // 평면과 Ray의 충돌 위치 계산
    const glm::vec3 point = ray.start + distance * ray.dir;

    // 충돌 위치를 이용하여 각 변을 기준으로 cross product 계산
    const glm::vec3 cross0 = glm::cross(point - v2, v1 - v2);
    const glm::vec3 cross1 = glm::cross(point - v0, v2 - v0);
    const glm::vec3 cross2 = glm::cross(point - v1, v0 - v1);

    // 하나라도 방향이 다르다면 충돌하지 않은 것
    if (glm::dot(cross0, faceNormal) < 0.0f ||
        glm::dot(cross1, faceNormal) < 0.0f ||
        glm::dot(cross2, faceNormal) < 0.0f)
        collided = false;

    // 충돌한 경우에만 Hit의 값들을 설정
    if (collided) {
        hit.d = distance;
        hit.point = point;
        hit.normal = faceNormal;

        // 텍스처를 사용한다면 텍스처 좌표 계산(Barycentric interpolation)
        const float area0 = glm::length(cross0) * 0.5f;    // v1v2 기준
        const float area1 = glm::length(cross1) * 0.5f;    // v0v2 기준
        const float area2 = glm::length(cross2) * 0.5f;    // v0v1 기준

        const float u = area0 / (area0 + area1 + area2);
        const float v = area1 / (area0 + area1 + area2);
        const float w = 1.0f - u - v;

        hit.uv = uv0 * u + uv1 * v + uv2 * w;

        // material 지정
        hit.material = material;
    }
}