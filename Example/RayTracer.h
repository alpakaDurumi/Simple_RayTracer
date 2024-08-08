#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Light.h"

class Hit;
class Object;
class Ray;
class CubeMap;

struct Camera {
	glm::vec3 position	{ 0.0f, 0.0f, 0.0f };

	// 기준 방향 벡터
	glm::vec3 forward	{ 0.0f, 0.0f, 1.0f };	// +z
	glm::vec3 right		{ 1.0f, 0.0f, 0.0f };	// +x
	glm::vec3 up		{ 0.0f, 1.0f, 0.0f };	// +y

	glm::mat4 viewMatrix = glm::mat4(1.0f);

	float fov = 90.0f;		// 시야각
	int width;				// 가로 해상도
	int height;				// 세로 해상도
	float aspectRatio;		// 종횡비

	Camera(const int& width, const int& height)
		: width(width), height(height) {
		aspectRatio = static_cast<float>(width) / height;
	}

	// 회전 행렬 업데이트 함수
	// 글로벌 회전으로 구현
	void updateViewMatrix(const float& deltaYaw, const float& deltaPitch) {
		viewMatrix = glm::rotate(viewMatrix, glm::radians(deltaYaw), glm::vec3(0.0f, 1.0f, 0.0f));
		viewMatrix = glm::rotate(viewMatrix, glm::radians(deltaPitch), glm::vec3(1.0f, 0.0f, 0.0f));
	}
};

class RayTracer {
public:
	Light light;											// 광원
	std::vector<std::shared_ptr<Object>> objects;			// 물체 목록
	std::shared_ptr<CubeMap> skyBox;						// 스카이박스
	Camera camera;											// 카메라

	RayTracer(const int& width, const int& height);
	Hit FindClosestCollision(const Ray& ray);
	glm::vec3 traceRay(const Ray& ray, const int recurseLevel);
	void Render(std::vector<glm::vec4>& pixels);
	glm::vec3 TransformScreenToWorld(const glm::vec2& screenPos);
	glm::vec3 SuperSample4x(const glm::vec3& cameraPos, const glm::vec3& pixelPos, const float dx);
};