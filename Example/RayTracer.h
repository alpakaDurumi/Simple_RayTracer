#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Light.h"

class Hit;
class Light;
class Object;
class Ray;

class RayTracer
{
public:
	int width, height;											// 해상도
	Light light;												// 광원
	std::vector<std::shared_ptr<Object>> objects;				// 물체 목록

	RayTracer(const int& width, const int& height);
	Hit FindClosestCollision(const Ray& ray);
	glm::vec3 traceRay(const Ray& ray);
	void Render(std::vector<glm::vec4>& pixels);
	glm::vec3 TransformScreenToWorld(const glm::vec2& screenPos);
	glm::vec3 SuperSample4x(const glm::vec3& cameraPos, const glm::vec3& pixelPos, const float dx);
};