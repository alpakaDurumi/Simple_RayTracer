#include "Triangle.h"
#include "Hit.h"
#include "Ray.h"

// uv ��ǥ�� clockwise�� �־����� ��
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

// �浹�� �˻��ϰ�, ���ο� ���� Hit�� �����ϴ� �Լ�
void Triangle::IntersectRayTriangle(const Ray& ray, Hit& hit) {
    // Ray�� �ﰢ���� �浹 ����
    bool collided = true;

    // �ﰢ���� �� �������� ���ǵǴ� ����� ��� ���� ���
    // v0, v1, v2�� clockwise �������� �Ѵ�
    const glm::vec3 faceNormal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

    // Backface culling
    if (glm::dot(-ray.dir, faceNormal) < 0.0f) collided = false;

    // ���� Ray�� ������ ���࿡ �ſ� �����ٸ� �浹���� ���ϴ� ������ �Ǵ�
    // d ���� divide by zero�� �����ϴ� ������ ����
    if (glm::abs(glm::dot(ray.dir, faceNormal)) < 1e-2f) collided = false;
    
    // Ray�� ���� �������� �浹 ���������� �Ÿ�
    const float distance = (glm::dot(v0, faceNormal) - glm::dot(ray.start, faceNormal)) / (glm::dot(ray.dir, faceNormal));

    // Ray�� ������ ������ �浹�� ������ ���
    if (distance < 0.0f) collided = false;

    // ���� Ray�� �浹 ��ġ ���
    const glm::vec3 point = ray.start + distance * ray.dir;

    // �浹 ��ġ�� �̿��Ͽ� �� ���� �������� cross product ���
    const glm::vec3 cross0 = glm::cross(point - v2, v1 - v2);
    const glm::vec3 cross1 = glm::cross(point - v0, v2 - v0);
    const glm::vec3 cross2 = glm::cross(point - v1, v0 - v1);

    // �ϳ��� ������ �ٸ��ٸ� �浹���� ���� ��
    if (glm::dot(cross0, faceNormal) < 0.0f ||
        glm::dot(cross1, faceNormal) < 0.0f ||
        glm::dot(cross2, faceNormal) < 0.0f)
        collided = false;

    // �浹�� ��쿡�� Hit�� ������ ����
    if (collided) {
        hit.d = distance;
        hit.point = point;
        hit.normal = faceNormal;

        // �ؽ�ó�� ����Ѵٸ� �ؽ�ó ��ǥ ���(Barycentric interpolation)
        const float area0 = glm::length(cross0) * 0.5f;    // v1v2 ����
        const float area1 = glm::length(cross1) * 0.5f;    // v0v2 ����
        const float area2 = glm::length(cross2) * 0.5f;    // v0v1 ����

        const float u = area0 / (area0 + area1 + area2);
        const float v = area1 / (area0 + area1 + area2);
        const float w = 1.0f - u - v;

        hit.uv = uv0 * u + uv1 * v + uv2 * w;

        // material ����
        hit.material = material;
    }
}