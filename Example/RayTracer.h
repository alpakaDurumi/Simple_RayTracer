#pragma once

#include <memory>
#include <vector>
#define GLM_FORCE_LEFT_HANDED			// 회전 동작 등에 대해 왼손 좌표계로 동작하도록 하기 위한 매크로
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Light.h"

class Hit;
class Object;
class Ray;
class CubeMap;

struct Camera {
	glm::vec3 position	{ 0.0f, 0.0f, 0.0f };

	// 카메라의 로컬 방향 벡터
	glm::vec3 forward	{ 0.0f, 0.0f, 1.0f };	// +z
	glm::vec3 right		{ 1.0f, 0.0f, 0.0f };	// +x
	glm::vec3 up		{ 0.0f, 1.0f, 0.0f };	// +y

	float yaw = 0.0f;		// y축 회전값
	float pitch = 0.0f;		// x축 회전값

	float fov = 90.0f;		// 시야각
	int width;				// 가로 해상도
	int height;				// 세로 해상도
	float aspectRatio;		// 종횡비

	Camera(const int& width, const int& height);
	
	void updateRotation(const float& deltaYaw, const float& deltaPitch);
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
	float fresnelSchlick(const float& n1, const float& n2, const float& cosTheta);
	void calculateReflectanceAndTransmittance(const float& n1, const float& n2, const float& cosTheta, float& reflectance, float& transmittance);
	void Render(std::vector<glm::vec4>& pixels);
	glm::vec3 TransformScreenToWorld(const glm::vec2& screenPos);
	glm::vec3 SuperSample4x(const glm::vec3& cameraPos, const glm::vec3& pixelPos, const float dx);
};