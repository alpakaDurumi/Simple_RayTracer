#pragma once

#include <array>
#include <string>

#include "Square.h"
#include "Texture.h"

// 큐브의 정점 8개
const std::array<glm::vec3, 8> cubeVertices = { {
	{ -1.0f, -1.0f, -1.0f }, // v0 : 왼쪽 아래 뒤
	{  1.0f, -1.0f, -1.0f }, // v1 : 오른쪽 아래 뒤
	{  1.0f,  1.0f, -1.0f }, // v2 : 오른쪽 위 뒤
	{ -1.0f,  1.0f, -1.0f }, // v3 : 왼쪽 위 뒤
	{ -1.0f, -1.0f,  1.0f }, // v4 : 왼쪽 아래 앞
	{  1.0f, -1.0f,  1.0f }, // v5 : 오른쪽 아래 앞
	{  1.0f,  1.0f,  1.0f }, // v6 : 오른쪽 위 앞
	{ -1.0f,  1.0f,  1.0f }  // v7 : 왼쪽 위 앞
} };

class CubeMap : public Object {
public:
	glm::vec3 center;
	float size = 10.0f;
	std::vector<Square> faces;

	CubeMap(const std::array<std::string, 6>& textureFiles, const glm::vec3& center = glm::vec3(0.0f), const float& size = 10.0f);
	virtual Hit CheckRayCollision(const Ray& ray);
};