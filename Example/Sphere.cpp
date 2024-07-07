#include "Sphere.h"
#include "Hit.h"
#include "Ray.h"

Sphere::Sphere(const glm::vec3& center, const float radius, const glm::vec3& color)
    : center(center), radius(radius), Object(color) {
}

Hit Sphere::CheckRayCollision(Ray& ray) {
    Hit hit = Hit{ -1.0f, glm::vec3(0.0f), glm::vec3(0.0f) };

    const float b = 2.0f * glm::dot(ray.dir, ray.start - center);
    const float c = dot(ray.start - center, ray.start - center) - radius * radius;
    
    const float det = b * b - 4.0f * c;
    // 한 점에서 충돌하거나, 두 점에서 충돌하는 경우
    if (det >= 0.0f) {
        const float d1 = (-b - sqrt(det)) / 2.0f;
        const float d2 = (-b + sqrt(det)) / 2.0f;

        // 광선의 시작점이 구 내부에 위치하는 경우 고려
        // 물체 안에서 밖으로 나가는 방향의 충돌 선택
        if (d1 < 0) hit.d = d2;
        // 그 외의 경우 더 가까운 쪽 선택
        else hit.d = d1;

        hit.point = ray.start + ray.dir * hit.d;
        hit.normal = glm::normalize(hit.point - center);
        // hit.uv는 Sphere에 대해서는 따로 지정하지 않음
        // hit.obj도 여기서 지정하고 싶지만, std::shared_ptr은 따로 this같은 키워드가 없기 때문에 RayTracer에서 지정
    }

    return hit;
}