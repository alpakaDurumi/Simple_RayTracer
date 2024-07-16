#include "RayTracer.h"
#include "Sphere.h"
#include "Hit.h"
#include "Ray.h"
#include "Triangle.h"
#include "Square.h"
#include "Texture.h"

RayTracer::RayTracer(const int& width, const int& height)
	: width(width), height(height) {
	// -z 방향에 광원
	light = Light{ {0.0f, 0.3f, -0.5f} };

	//// 초록색 구
	//auto sphere1 = std::make_shared<Sphere>(glm::vec3(-1.0f, 0.0f, 1.0f), 0.5f);
	//sphere1->setColor(glm::vec3{ 0.0f, 1.0f, 0.0f });
	//sphere1->configureSpecular(40.0f, 1.0f);
	//objects.push_back(sphere1);

	//// 보라색 구
	//auto sphere2 = std::make_shared<Sphere>(glm::vec3(0.5f, 0.0f, 1.0f), 0.5f);
	//sphere2->setColor(glm::vec3{ 1.0f, 0.0f, 1.0f });
	//sphere2->configureSpecular(40.0f, 1.0f);
	//objects.push_back(sphere2);

	//// 흰색 사각형
	//auto square1 = std::make_shared<Square>(glm::vec3(-4.0f, -1.0f, 0.0f), glm::vec3(-4.0f, -1.0f, 4.0f), glm::vec3(4.0f, -1.0f, 4.0f), glm::vec3(4.0f, -1.0f, 0.0f));
	//objects.push_back(square1);
	
	//// 삼각형
	//auto triangle1 = std::make_shared<Triangle>(glm::vec3(-3.0f, -3.0f, 4.0f), glm::vec3(-3.0f, 3.0f, 4.0f), glm::vec3(3.0f, -3.0f, 4.0f));
	//triangle1->configureSpecular(0.0f, 0.0f);
	//triangle1->useTexture = false;
	//objects.push_back(triangle1);

	// 텍스처링 테스트용
	std::vector<glm::vec3> textureImage(4 * 4);
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			if (i % 4 == 0)
				textureImage[i + 4 * j] = glm::vec3(1.0f, 0.0f, 0.0f) * (1.0f + j) * 0.25f;
			else if (i % 4 == 1)
				textureImage[i + 4 * j] = glm::vec3(0.0f, 1.0f, 0.0f) * (1.0f + j) * 0.25f;
			else if (i % 4 == 2)
				textureImage[i + 4 * j] = glm::vec3(0.0f, 0.0f, 1.0f) * (1.0f + j) * 0.25f;
			else
				textureImage[i + 4 * j] = glm::vec3(1.0f, 1.0f, 1.0f) * (1.0f + j) * 0.25f;
		}
	}

	auto testTexture = std::make_shared<Texture>(4, 4, textureImage);

	auto squareTestTexture = std::make_shared<Square>(
		glm::vec3(-2.0f, 2.0f, 2.0f), glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(2.0f, -2.0f, 2.0f), glm::vec3(-2.0f, -2.0f, 2.0f),
		glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f));
	squareTestTexture->material.ambTexture = testTexture;
	squareTestTexture->material.amb = glm::vec3(1.0f);
	squareTestTexture->material.dif = glm::vec3(0.0f);
	squareTestTexture->configureSpecular(0.0f, 0.0f);
	objects.push_back(squareTestTexture);
}

// ray가 충돌한 지점 중 가장 가까운 지점에 대한 Hit 반환
Hit RayTracer::FindClosestCollision(Ray& ray) {
	float closestDistance = 1000.0; // inf
	Hit closestHit = Hit{ -1.0, glm::vec3(0.0), glm::vec3(0.0) };
	
	// 모든 물체에 대해 검사
	for (int i = 0; i < objects.size(); i++) {
		auto hit = objects[i]->CheckRayCollision(ray);
		// 충돌했다면
		if (hit.d >= 0.0f) {
			if (hit.d < closestDistance) {
				closestDistance = hit.d;
				closestHit = hit;
				closestHit.obj = objects[i];		// 물체의 머티리얼 정보
			}
		}
	}

	return closestHit;
}

// 광선이 물체에 닿으면 그 물체의 색 반환
glm::vec3 RayTracer::traceRay(Ray& ray, const int recurseLevel) {
	if (recurseLevel < 0)
		return glm::vec3(0.0f);

	// 가장 가까운 충돌에 대한 Hit
	const auto hit = FindClosestCollision(ray);

	// 충돌한 경우에 대해서만 동작
	// hit.d가 0보다 작다는 것은 어느 물체와도 충돌하지 않았다는 뜻
	if (hit.d >= 0.0f) {
		glm::vec3 objectColor(0.0f);

		glm::vec3 phongColor;

		// ambient texture가 지정되어 있다면
		if (hit.obj->material.ambTexture) {
			// 텍스처에서 샘플링한 값으로 설정
			// 텍스처 필터링 방식을 변수로 조절할 수 있도록 수정하기
			phongColor = hit.obj->material.amb * hit.obj->material.ambTexture->SamplePoint(hit.uv);
		}
		else {
			// material에 지정된 단일 색상으로 설정
			phongColor = hit.obj->material.amb;
		}

		const glm::vec3 dirToLight = glm::normalize(light.pos - hit.point);

		// 그림자 계산을 위한 Ray
		Ray shadowRay{ hit.point + dirToLight * 1e-4f, dirToLight };
		// 충돌 지점에서 광원 사이를 막는 물체까지의 거리
		float distanceToObstacle = FindClosestCollision(shadowRay).d;

		// 충돌 지점과 광원 사이에 가로막는 물체가 없다면 diffuse와 specular를 추가로 계산
		if (distanceToObstacle < 0.0f || glm::length(light.pos - hit.point) < distanceToObstacle) {
			// Diffuse 계산
			const float diff = glm::max(dot(hit.normal, dirToLight), 0.0f);

			// diffuse texture가 지정되어 있다면
			if (hit.obj->material.difTexture) {
				phongColor += hit.obj->material.dif * diff * hit.obj->material.difTexture->SampleLinear(hit.uv);
			}
			else {
				phongColor += hit.obj->material.dif * diff;
			}

			// Specular 계산
			const glm::vec3 reflectDir = 2.0f * glm::dot(dirToLight, hit.normal) * hit.normal - dirToLight;
			const float specular = glm::pow(glm::max(glm::dot(-ray.dir, reflectDir), 0.0f), hit.obj->material.specularPower);

			// specular texture는 사용 X
			phongColor += hit.obj->material.spec * specular * hit.obj->material.specularCoefficient;
		}

		objectColor += phongColor * (1.0f - hit.obj->material.reflection - hit.obj->material.transparency);

		return objectColor;
	}

	return glm::vec3(0.0f);
}

// 각 픽셀에 해당하는 월드 좌표계 위치에서 ray를 쏴서 얻은 색상값으로 각 픽셀을 설정
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

// 스크린 좌표계 -> 월드 좌표계 변환
// 대상 월드 좌표계의 범위는 [-aspect, aspect] * [1, -1]
// 픽셀의 중심이 가장자리와 화면의 가장자리가 일치하는 구조 사용
glm::vec3 RayTracer::TransformScreenToWorld(glm::vec2 posScreen) {
	const float xScale = 2.0f / this->width;
	const float yScale = 2.0f / this->height;
	const float aspect = static_cast<float>(this->width) / this->height;

	return glm::vec3(((posScreen.x + 0.5f) * xScale - 1.0f) * aspect, -(posScreen.y + 0.5f) * yScale + 1.0f, 0.0f);
}