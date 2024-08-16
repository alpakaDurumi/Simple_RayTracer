#include "RayTracer.h"
#include "Sphere.h"
#include "Hit.h"
#include "Ray.h"
#include "Triangle.h"
#include "Square.h"
#include "Texture.h"
#include "CubeMap.h"

Camera::Camera(const int& width, const int& height)
	: width(width), height(height) {
	aspectRatio = static_cast<float>(width) / height;
}

// 회전 업데이트 함수. 시계 방향 글로벌 회전
// 매번 새 회전 행렬을 계산하는 방식
// 회전 행렬을 누적하는 방식에 비해 느리지만, 수치 오차가 누적되지 않아 정밀도가 높다
void Camera::updateRotation(const float& deltaYaw, const float& deltaPitch) {
	// yaw와 pitch 업데이트
	yaw += deltaYaw;
	pitch += deltaPitch;

	// 회전 행렬 계산
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));	// yaw
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));				// pitch

	// 카메라 방향 벡터 업데이트
	forward = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));
	right = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));
	// roll 회전은 수행하지 않지만, forward와 right가 변경되므로 그에 맞게 업데이트가 필요
	up = glm::normalize(glm::cross(forward, right));
}

RayTracer::RayTracer(const int& width, const int& height)
	: camera(width, height) {
	// -z 방향에 광원
	light = Light{ {0.0f, 0.3f, -0.5f} };

	// 큐브맵
	std::array<std::string, 6> cubeMapTextureFiles{ "posz.jpg","negz.jpg","posx.jpg","negx.jpg","posy.jpg", "negy.jpg" };
	skyBox = std::make_shared<CubeMap>(cubeMapTextureFiles);

	//auto sphere1 = std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, 3.0f), 1.5f);
	//sphere1->setReflection(true);
	//sphere1->setRefraction(true);
	//sphere1->setMaterialType(MaterialType::Glass);
	//objects.push_back(sphere1);

	// 노멀 맵 테스트
	auto square1 = std::make_shared<Square>(glm::vec3(-5.0f, -3.0f, -5.0f), glm::vec3(-5.0f, -3.0f, 5.0f), glm::vec3(5.0f, -3.0f, 5.0f), glm::vec3(4.0f, -3.0f, -5.0f));
	objects.push_back(square1);
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

		// 레이의 시작점이 물체의 바깥인지 안인지 구분하기 위함
		const bool isFromOutside = glm::dot(ray.dir, hit.normal) < 0.0f;

		// 슐릭 근사, 굴절 계산에 사용하기 위한 노멀 벡터
		const glm::vec3 normal = (isFromOutside) ? hit.normal : -hit.normal;

		// 입사각에 대한 cos
		const float cosThetaIn = glm::dot(-ray.dir, normal);

		// 반사율과 투과율
		float reflectance = 0.0f, transmittance = 0.0f;

		// 머티리얼의 반사 유무와 굴절 유무에 따라 반사율과 투과율 지정
		if (hit.material->hasReflection && hit.material->hasRefraction) {
			// 둘 모두 존재하는 경우 슐릭 근사로 프레넬 계수 계산
			if(isFromOutside)
				calculateReflectanceAndTransmittance(1.0f, hit.material->refractiveIndex, cosThetaIn, reflectance, transmittance);
			else
				calculateReflectanceAndTransmittance(hit.material->refractiveIndex, 1.0f, cosThetaIn, reflectance, transmittance);
		}
		else {
			reflectance = (hit.material->hasReflection) ? 1.0f : 0.0f;
			transmittance = (hit.material->hasRefraction) ? 1.0f : 0.0f;
		}

		// 머티리얼의 반사 유무와 굴절 유무에 따라 phongColor의 배율 지정
		// 반사와 굴절이 둘 다 없는 경우 그대로 더해주며, 그 외의 경우 0.1의 비율로 더한다
		objectColor += phongColor * ((hit.material->hasReflection || hit.material->hasRefraction) ? 0.1f : 1.0f);

		// 반사가 설정되어 있다면
		if (hit.material->hasReflection) {
			const glm::vec3 reflectedDir = 2.0f * dot(hit.normal, -ray.dir) * hit.normal + ray.dir;
			Ray reflectionRay{ hit.point + reflectedDir * 1e-4f, reflectedDir };
			objectColor += traceRay(reflectionRay, recurseLevel - 1) * reflectance;
		}

		// 굴절이 설정되어 있다면
		if (hit.material->hasRefraction) {
			// 상대 굴절률
			const float relativeRefractiveIndex = (isFromOutside) ? hit.material->refractiveIndex / 1.0f : 1.0f / hit.material->refractiveIndex;

			// 입사각에 대한 sin
			const float sinThetaIn = glm::sqrt(1.0f - cosThetaIn * cosThetaIn);

			// 굴절각에 대한 sin, cos
			const float sinThetaRef = sinThetaIn / relativeRefractiveIndex;
			const float cosThetaRef = glm::sqrt(1.0f - sinThetaRef * sinThetaRef);

			// 굴절 방향의 x 성분(방향 * 크기)
			const glm::vec3 x = glm::normalize(cosThetaIn * normal + ray.dir) * sinThetaRef;
			// 굴절 방향의 y 성분(방향 * 크기)
			const glm::vec3 y = -normal * cosThetaRef;

			// 굴절 방향
			const glm::vec3 refractedDir = glm::normalize(x + y);

			Ray refractionRay{ hit.point + refractedDir * 1e-4f, refractedDir };
			objectColor += traceRay(refractionRay, recurseLevel) * transmittance;
		}

		return objectColor;
	}

	// 물체에 부딪히지 않았고, 스카이박스가 존재한다면 스카이박스 샘플링
	if (skyBox)
		return skyBox->sampleColor(ray);

	return glm::vec3(0.0f);
}

// Schlick's approximation
float RayTracer::fresnelSchlick(const float& n1, const float& n2, const float& cosTheta) {
	float r0 = glm::pow((n1 - n2) / (n1 + n2), 2.0f);
	return r0 + (1 - r0) * glm::pow((1 - cosTheta), 5.0f);
}

// 반사율(reflectance)와 투과율(transmittance) 계산
void RayTracer::calculateReflectanceAndTransmittance(const float& n1, const float& n2, const float& cosTheta, float& reflectance, float& transmittance) {
	reflectance = fresnelSchlick(n1, n2, cosTheta);
	transmittance = 1.0f - reflectance;
}

// 각 픽셀에 해당하는 월드 좌표계 위치에서 ray를 쏴서 얻은 색상값으로 각 픽셀을 설정
void RayTracer::Render(std::vector<glm::vec4>& pixels) {
	std::fill(pixels.begin(), pixels.end(), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	// 픽셀의 크기
	const float dx = 2.0f / camera.height;

#pragma omp parallel for
	for (int j = 0; j < camera.height; j++)
		for (int i = 0; i < camera.width; i++) {
			const glm::vec3 pixelPosWorld = TransformScreenToWorld(glm::vec2(i, j));
			// For perspective projection

			// ray를 픽셀 당 하나만 쏘는 코드
			Ray pixelRay{ pixelPosWorld, glm::normalize(pixelPosWorld - camera.position) };
			pixels[i + camera.width * j] = glm::vec4(glm::clamp(traceRay(pixelRay, 5), 0.0f, 1.0f), 1.0f);

			// ray를 여러개 쏴서 슈퍼샘플링하는 코드
			//const auto pixelColor = SuperSample4x(camera.position, pixelPosWorld, dx);
			//pixels[i + width * j] = glm::vec4(glm::clamp(pixelColor, 0.0f, 1.0f), 1.0f);
		}
}

// 스크린 좌표계 -> 월드 좌표계 변환
glm::vec3 RayTracer::TransformScreenToWorld(const glm::vec2& screenPos) {
	// NDC 좌표로 변환
	// 픽셀 중심 좌표를 고려하여 0.5 오프셋 추가
	const float ndcX = 2.0f * (screenPos.x + 0.5f) / camera.width - 1.0f;
	const float ndcY = 1.0f - 2.0f * (screenPos.y + 0.5f) / camera.height;

	// 월드 좌표로 변환
	// 시야각(fov)와 종횡비 고려
	// fov는 각도로 주어지기 때문에 라디안으로의 변환이 필요
	const float worldX = ndcX * camera.aspectRatio * tan(glm::radians(camera.fov / 2.0f));
	const float worldY = ndcY * tan(glm::radians(camera.fov / 2.0f));

	// 카메라 뷰 행렬
	glm::mat4 viewMatrix = glm::lookAt(camera.position, camera.position + camera.forward, camera.up);

	// 역행렬을 적용하여 월드 좌표계에서의 회전(방향) 계산
	glm::vec4 worldDir = glm::inverse(viewMatrix) * glm::vec4(worldX, worldY, 1.0f, 0.0f);
	
	// 카메라 위치를 더하여 최종 월드 좌표 계산
	glm::vec3 worldPos = camera.position + glm::vec3(worldDir);
	return worldPos;
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