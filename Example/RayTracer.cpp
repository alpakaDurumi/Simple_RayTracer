#include "RayTracer.h"
#include "Sphere.h"
#include "Hit.h"
#include "Ray.h"
#include "Triangle.h"

RayTracer::RayTracer(const int& width, const int& height)
	: width(width), height(height) {
	// -z ���⿡ ����
	light = Light{ {0.0f, 0.3f, -0.5f} };

	// �ʷϻ� ��
	auto sphere1 = std::make_shared<Sphere>(glm::vec3(-1.0f, 0.0f, 1.0f), 0.5f);
	sphere1->setColor(glm::vec3{ 0.0f, 1.0f, 0.0f });
	sphere1->configureSpecular(40.0f, 1.0f);
	objects.push_back(sphere1);

	// ����� ��
	auto sphere2 = std::make_shared<Sphere>(glm::vec3(0.5f, 0.0f, 1.0f), 0.5f);
	sphere2->setColor(glm::vec3{ 1.0f, 0.0f, 1.0f });
	sphere2->configureSpecular(40.0f, 1.0f);
	objects.push_back(sphere2);

	// ��� �ﰢ��
	auto triangle1 = std::make_shared<Triangle>(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(-4.0f, -1.0f, 6.0f), glm::vec3(4.0f, -1.0f, 6.0f));
	objects.push_back(triangle1);
}

// ray�� �浹�� ���� �� ���� ����� ������ ���� Hit ��ȯ
Hit RayTracer::FindClosestCollision(Ray& ray) {
	float closestDistance = 1000.0; // inf
	Hit closestHit = Hit{ -1.0, glm::vec3(0.0), glm::vec3(0.0) };
	
	// ��� ��ü�� ���� �˻�
	for (int i = 0; i < objects.size(); i++) {
		auto hit = objects[i]->CheckRayCollision(ray);
		// �浹�ߴٸ�
		if (hit.d >= 0.0f) {
			if (hit.d < closestDistance) {
				closestDistance = hit.d;
				closestHit = hit;
				closestHit.obj = objects[i];		// ��ü�� ��Ƽ���� ����

				// �ؽ�ó ��ǥ	// Triangle::CheckRayCollision���� �̹� �����Ǿ�����, �� �� ������ ����Ǿ��� ������ �ʿ���ٰ� ������
				//closestHit.uv = hit.uv;
			}
		}
	}

	return closestHit;
}

// ������ ��ü�� ������ �� ��ü�� �� ��ȯ
glm::vec3 RayTracer::traceRay(Ray& ray, const int recurseLevel) {
	if (recurseLevel < 0)
		return glm::vec3(0.0f);

	// ���� ����� �浹�� ���� Hit
	const auto hit = FindClosestCollision(ray);

	// �浹�� ��쿡 ���ؼ��� ����
	// hit.d�� 0���� �۴ٴ� ���� ��� ��ü�͵� �浹���� �ʾҴٴ� ��
	if (hit.d >= 0.0f) {
		glm::vec3 objectColor(0.0f);
		glm::vec3 phongColor(hit.obj->material.amb);	// ambient ������ �ʱ�ȭ

		const glm::vec3 dirToLight = glm::normalize(light.pos - hit.point);

		// �׸��� ����� ���� Ray
		Ray shadowRay{ hit.point + dirToLight * 1e-4f, dirToLight };
		// �浹 �������� ���� ���̸� ���� ��ü������ �Ÿ�
		float distanceToObstacle = FindClosestCollision(shadowRay).d;

		// �浹 ������ ���� ���̿� ���θ��� ��ü�� ���ٸ� diffuse�� specular�� �߰��� ���
		if (distanceToObstacle < 0.0f || glm::length(light.pos - hit.point) < distanceToObstacle) {
			// Diffuse ���
			const float diff = glm::max(dot(hit.normal, dirToLight), 0.0f);

			// Specular ���
			const glm::vec3 reflectDir = 2.0f * glm::dot(dirToLight, hit.normal) * hit.normal - dirToLight;
			const float specular = glm::pow(glm::max(glm::dot(-ray.dir, reflectDir), 0.0f), hit.obj->material.specularPower);

			phongColor += hit.obj->material.dif * diff + hit.obj->material.spec * specular * hit.obj->material.specularCoefficient;
		}

		objectColor += phongColor * (1.0f - hit.obj->material.reflection - hit.obj->material.transparency);

		return objectColor;
	}

	return glm::vec3(0.0f);
}

// �� �ȼ��� �ش��ϴ� ���� ��ǥ�� ��ġ���� ray�� ���� ���� �������� �� �ȼ��� ����
void RayTracer::Render(std::vector<glm::vec4>& pixels) {
	std::fill(pixels.begin(), pixels.end(), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	const glm::vec3 eyePos(0.0f, 0.0f, -1.5f);

#pragma omp parallel for
	for (int j = 0; j < height; j++)
		for (int i = 0; i < width; i++) {
			const glm::vec3 pixelPosWorld = TransformScreenToWorld(glm::vec2(i, j));
			// For perspective projection
			Ray pixelRay{ pixelPosWorld, glm::normalize(pixelPosWorld - eyePos) };
			pixels[i + width * j] = glm::vec4(glm::clamp(traceRay(pixelRay, 5), 0.0f, 1.0f), 1.0f);
		}
}

// ��ũ�� ��ǥ�� -> ���� ��ǥ�� ��ȯ
// ��� ���� ��ǥ���� ������ [-aspect, aspect] * [1, -1]
// �ȼ��� �߽��� �����ڸ��� ȭ���� �����ڸ��� ��ġ�ϴ� ���� ���
glm::vec3 RayTracer::TransformScreenToWorld(glm::vec2 posScreen) {
	const float xScale = 2.0f / this->width;
	const float yScale = 2.0f / this->height;
	const float aspect = static_cast<float>(this->width) / this->height;

	return glm::vec3(((posScreen.x + 0.5f) * xScale - 1.0f) * aspect, -(posScreen.y + 0.5f) * yScale + 1.0f, 0.0f);
}