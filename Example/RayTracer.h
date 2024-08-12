#pragma once

#include <memory>
#include <vector>
#define GLM_FORCE_LEFT_HANDED			// ȸ�� ���� � ���� �޼� ��ǥ��� �����ϵ��� �ϱ� ���� ��ũ��
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Light.h"

class Hit;
class Object;
class Ray;
class CubeMap;

struct Camera {
	glm::vec3 position	{ 0.0f, 0.0f, 0.0f };

	// ī�޶��� ���� ���� ����
	glm::vec3 forward	{ 0.0f, 0.0f, 1.0f };	// +z
	glm::vec3 right		{ 1.0f, 0.0f, 0.0f };	// +x
	glm::vec3 up		{ 0.0f, 1.0f, 0.0f };	// +y

	float yaw = 0.0f;		// y�� ȸ����
	float pitch = 0.0f;		// x�� ȸ����

	float fov = 90.0f;		// �þ߰�
	int width;				// ���� �ػ�
	int height;				// ���� �ػ�
	float aspectRatio;		// ��Ⱦ��

	Camera(const int& width, const int& height);
	
	void updateRotation(const float& deltaYaw, const float& deltaPitch);
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
	float fresnelSchlick(const float& n1, const float& n2, const float& cosTheta);
	void calculateReflectanceAndTransmittance(const float& n1, const float& n2, const float& cosTheta, float& reflectance, float& transmittance);
	void Render(std::vector<glm::vec4>& pixels);
	glm::vec3 TransformScreenToWorld(const glm::vec2& screenPos);
	glm::vec3 SuperSample4x(const glm::vec3& cameraPos, const glm::vec3& pixelPos, const float dx);
};