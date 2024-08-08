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

	// ���� ���� ����
	glm::vec3 forward	{ 0.0f, 0.0f, 1.0f };	// +z
	glm::vec3 right		{ 1.0f, 0.0f, 0.0f };	// +x
	glm::vec3 up		{ 0.0f, 1.0f, 0.0f };	// +y

	glm::mat4 viewMatrix = glm::mat4(1.0f);

	float fov = 90.0f;		// �þ߰�
	int width;				// ���� �ػ�
	int height;				// ���� �ػ�
	float aspectRatio;		// ��Ⱦ��

	Camera(const int& width, const int& height)
		: width(width), height(height) {
		aspectRatio = static_cast<float>(width) / height;
	}

	// ȸ�� ��� ������Ʈ �Լ�
	// �۷ι� ȸ������ ����
	void updateViewMatrix(const float& deltaYaw, const float& deltaPitch) {
		viewMatrix = glm::rotate(viewMatrix, glm::radians(deltaYaw), glm::vec3(0.0f, 1.0f, 0.0f));
		viewMatrix = glm::rotate(viewMatrix, glm::radians(deltaPitch), glm::vec3(1.0f, 0.0f, 0.0f));
	}
};

class RayTracer {
public:
	Light light;											// ����
	std::vector<std::shared_ptr<Object>> objects;			// ��ü ���
	std::shared_ptr<CubeMap> skyBox;						// ��ī�̹ڽ�
	Camera camera;											// ī�޶�

	RayTracer(const int& width, const int& height);
	Hit FindClosestCollision(const Ray& ray);
	glm::vec3 traceRay(const Ray& ray, const int recurseLevel);
	void Render(std::vector<glm::vec4>& pixels);
	glm::vec3 TransformScreenToWorld(const glm::vec2& screenPos);
	glm::vec3 SuperSample4x(const glm::vec3& cameraPos, const glm::vec3& pixelPos, const float dx);
};