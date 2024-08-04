#include "RayTracer.h"
#include "Sphere.h"
#include "Hit.h"
#include "Ray.h"
#include "Triangle.h"
#include "Square.h"
#include "Texture.h"
#include "CubeMap.h"

RayTracer::RayTracer(const int& width, const int& height)
	: width(width), height(height) {
	// -z 방향에 광원
	light = Light{ {0.0f, 0.3f, -0.5f} };

	// 초록색 구
	auto sphere1 = std::make_shared<Sphere>(glm::vec3(-1.0f, 0.0f, 1.0f), 0.5f);
	sphere1->setColor(glm::vec3{ 0.0f, 1.0f, 0.0f });
	sphere1->material->refraction = 0.5f;
	objects.push_back(sphere1);

	// 보라색 구
	auto sphere2 = std::make_shared<Sphere>(glm::vec3(0.5f, 0.0f, 1.0f), 0.5f);
	sphere2->setColor(glm::vec3{ 1.0f, 0.0f, 1.0f });
	sphere2->material->reflection = 0.5f;
	objects.push_back(sphere2);

	// 흰색 사각형
	//auto square1 = std::make_shared<Square>(glm::vec3(-4.0f, -1.0f, 0.0f), glm::vec3(-4.0f, -1.0f, 4.0f), glm::vec3(4.0f, -1.0f, 4.0f), glm::vec3(4.0f, -1.0f, 0.0f));
	//objects.push_back(square1);

	// 텍스처링 테스트용 1
	//std::vector<glm::vec3> textureImage(4 * 4);
	//for (int j = 0; j < 4; j++) {
	//	for (int i = 0; i < 4; i++) {
	//		if (i % 4 == 0)
	//			textureImage[i + 4 * j] = glm::vec3(1.0f, 0.0f, 0.0f) * (1.0f + j) * 0.25f;
	//		else if (i % 4 == 1)
	//			textureImage[i + 4 * j] = glm::vec3(0.0f, 1.0f, 0.0f) * (1.0f + j) * 0.25f;
	//		else if (i % 4 == 2)
	//			textureImage[i + 4 * j] = glm::vec3(0.0f, 0.0f, 1.0f) * (1.0f + j) * 0.25f;
	//		else
	//			textureImage[i + 4 * j] = glm::vec3(1.0f, 1.0f, 1.0f) * (1.0f + j) * 0.25f;
	//	}
	//}
	//auto testTexture = std::make_shared<Texture>(4, 4, textureImage);
	//testTexture->SetAddressMode(TextureAddressMode::Clamp);
	//testTexture->SetFilterMode(TextureFilterMode::Point);

	// 텍스처링 테스트용 2
	//Image by freepik https://www.freepik.com/free-ai-image/geometric-seamless-pattern_94949548.htm#fromView=search&page=1&position=0&uuid=4bfff6fb-3412-4780-9681-b7f2c31eaa3b
	//auto testTexture = std::make_shared<Texture>("geometric-seamless-pattern.jpg");
	//testTexture->SetAddressMode(TextureAddressMode::Clamp);
	//testTexture->SetFilterMode(TextureFilterMode::Point);

	//auto squareTest = std::make_shared<Square>(
	//	glm::vec3(-2.0f, 2.0f, 2.0f), glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(2.0f, -2.0f, 2.0f), glm::vec3(-2.0f, -2.0f, 2.0f),
	//	glm::vec2(0.0f, 0.0f), glm::vec2(4.0f, 0.0f), glm::vec2(4.0f, 4.0f), glm::vec2(0.0f, 4.0f));
	//squareTest->setTexture(testTexture);
	//squareTest->configureSpecular(0.0f, 0.0f);
	//squareTest->setAmbientFactor(1.0f);
	//objects.push_back(squareTest);

	// 큐브맵
	std::array<std::string, 6> cubeMapTextureFiles{ "posz.jpg","negz.jpg","posx.jpg","negx.jpg","posy.jpg", "negy.jpg" };
	skyBox = std::make_shared<CubeMap>(cubeMapTextureFiles);
}

// ray가 충돌한 지점 중 가장 가까운 지점에 대한 Hit 반환
Hit RayTracer::FindClosestCollision(const Ray& ray) {
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
			}
		}
	}

	return closestHit;
}

// 광선이 물체에 닿으면 그 물체의 색 반환
glm::vec3 RayTracer::traceRay(const Ray& ray, const int recurseLevel) {
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
		if (hit.material->ambTexture) {
			// 텍스처에서 샘플링한 값으로 설정
			phongColor = hit.material->amb * hit.material->ambTexture->Sample(hit.uv);
		}
		else {
			// material에 지정된 단일 색상으로 설정
			phongColor = hit.material->amb;
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
			if (hit.material->difTexture) {
				phongColor += hit.material->dif * diff * hit.material->difTexture->Sample(hit.uv);
			}
			else {
				phongColor += hit.material->dif * diff;
			}

			// Specular 계산
			const glm::vec3 reflectDir = 2.0f * glm::dot(dirToLight, hit.normal) * hit.normal - dirToLight;
			const float specular = glm::pow(glm::max(glm::dot(-ray.dir, reflectDir), 0.0f), hit.material->specularPower);

			// specular texture는 사용 X
			phongColor += hit.material->spec * specular * hit.material->specularCoefficient;
		}

		objectColor += phongColor * (1.0f - hit.material->reflection - hit.material->refraction);

		// reflection이 설정되어 있다면
		if (hit.material->reflection > 0.0f) {
			const glm::vec3 reflectedDir = 2.0f * dot(hit.normal, -ray.dir) * hit.normal + ray.dir;
			Ray reflectionRay{ hit.point + reflectedDir * 1e-4f, reflectedDir };
			objectColor += traceRay(reflectionRay, recurseLevel - 1) * hit.material->reflection;
		}

		// transparency가 설정되어 있다면
		if (hit.material->refraction > 0.0f) {
			// 물체의 밖에서 안으로 굴절되는 경우의 값
			float relativeRefractiveIndex = 1.5f;
			glm::vec3 normal = hit.normal;

			// 물체의 안에서 밖으로 굴절되는 경우, 위 두 값을 반대로 설정
			if (glm::dot(ray.dir, hit.normal) >= 0.0f) {
				relativeRefractiveIndex = 1.0f / relativeRefractiveIndex;
				normal = -normal;
			}

			// 입사각
			const float cosThetaIn = glm::dot(-ray.dir, normal);
			const float sinThetaIn = glm::sqrt(1.0f - cosThetaIn * cosThetaIn);

			// 굴절각
			const float sinThetaRef = sinThetaIn / relativeRefractiveIndex;
			const float cosThetaRef = glm::sqrt(1.0f - sinThetaRef * sinThetaRef);

			// 굴절 방향의 x 성분(방향 * 크기)
			const glm::vec3 x = glm::normalize(cosThetaIn * normal + ray.dir) * sinThetaRef;
			// 굴절 방향의 y 성분(방향 * 크기)
			const glm::vec3 y = -normal * cosThetaRef;

			// 굴절 방향
			const glm::vec3 refractedDir = glm::normalize(x + y);

			Ray refractionRay{ hit.point + refractedDir * 1e-4f, refractedDir };
			objectColor += traceRay(refractionRay, recurseLevel) * hit.material->refraction;
		}

		return objectColor;
	}

	// 물체에 부딪히지 않았고, 스카이박스가 존재한다면 스카이박스 샘플링
	if (skyBox)
		return skyBox->sampleColor(ray);

	return glm::vec3(0.0f);
}

// 각 픽셀에 해당하는 월드 좌표계 위치에서 ray를 쏴서 얻은 색상값으로 각 픽셀을 설정
void RayTracer::Render(std::vector<glm::vec4>& pixels) {
	std::fill(pixels.begin(), pixels.end(), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	const glm::vec3 cameraPos(0.0f, 0.0f, -1.5f);

	// 픽셀의 크기
	const float dx = 2.0f / height;

#pragma omp parallel for
	for (int j = 0; j < height; j++)
		for (int i = 0; i < width; i++) {
			const glm::vec3 pixelPosWorld = TransformScreenToWorld(glm::vec2(i, j));
			// For perspective projection

			// ray를 픽셀 당 하나만 쏘는 코드
			Ray pixelRay{ pixelPosWorld, glm::normalize(pixelPosWorld - cameraPos) };
			pixels[i + width * j] = glm::vec4(glm::clamp(traceRay(pixelRay, 5), 0.0f, 1.0f), 1.0f);

			// ray를 여러개 쏴서 슈퍼샘플링하는 코드
			//const auto pixelColor = SuperSample4x(cameraPos, pixelPosWorld, dx);
			//pixels[i + width * j] = glm::vec4(glm::clamp(pixelColor, 0.0f, 1.0f), 1.0f);
		}
}

// 스크린 좌표계 -> 월드 좌표계 변환
// 대상 월드 좌표계의 범위는 [-aspect, aspect] * [1, -1]
// 픽셀의 중심이 가장자리와 화면의 가장자리가 일치하는 구조 사용
glm::vec3 RayTracer::TransformScreenToWorld(const glm::vec2& screenPos) {
	const float xScale = 2.0f / this->width;
	const float yScale = 2.0f / this->height;
	const float aspect = static_cast<float>(this->width) / this->height;

	return glm::vec3(((screenPos.x + 0.5f) * xScale - 1.0f) * aspect, -(screenPos.y + 0.5f) * yScale + 1.0f, 0.0f);
}

// 한 픽셀에 대해 4배로 슈퍼샘플링
glm::vec3 RayTracer::SuperSample4x(const glm::vec3& cameraPos, const glm::vec3& pixelPos, const float dx) {
	glm::vec3 pixelColor(0.0f);

	const float subdx = 0.5f * dx;

	const float hor[4] = {-0.5f, 0.5f, -0.5f, 0.5f};
	const float ver[4] = {0.5f, 0.5f, -0.5f, -0.5f};

	for(int i = 0; i < 4; i++) {
		glm::vec3 subPos(pixelPos.x + hor[i] * subdx, pixelPos.y + ver[i] * subdx, pixelPos.z);
		Ray subRay{ subPos, glm::normalize(subPos - cameraPos) };
		pixelColor += traceRay(subRay, 5);
	}

	// 평균을 내서 반환
	return pixelColor * 0.25f;
}