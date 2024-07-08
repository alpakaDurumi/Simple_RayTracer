#include "Triangle.h"
#include "Hit.h"
#include "Ray.h"

Triangle::Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& color)
    : v0(v0), v1(v1), v2(v2), Object(color) {}

Hit Triangle::CheckRayCollision(Ray& ray) {
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

    // �浹 ��ġ�� �̿��Ͽ� ���� �ﰢ�� 3���� ��� ���� ���
    const glm::vec3 normal0 = glm::normalize(glm::cross(point - v0, v2 - v0));
    const glm::vec3 normal1 = glm::normalize(glm::cross(point - v1, v0 - v1));
    const glm::vec3 normal2 = glm::normalize(glm::cross(point - v2, v1 - v2));

    // �ϳ��� ������ �ٸ��ٸ� �浹���� ���� ��
    if (glm::dot(normal0, faceNormal) < 0.0f ||
        glm::dot(normal1, faceNormal) < 0.0f ||
        glm::dot(normal2, faceNormal) < 0.0f)
        collided = false;

    // �浹�� ��쿡�� Hit�� ������ ����
    if (collided) {
        hit.d = distance;
        hit.point = point;
        hit.normal = faceNormal;
    }
}