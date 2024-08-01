#pragma once

#include <array>
#include <string>

#include "Square.h"
#include "Texture.h"

// ť���� ���� 8��
const std::array<glm::vec3, 8> cubeVertices = { {
	{ -1.0f, -1.0f, -1.0f }, // v0 : ���� �Ʒ� ��
	{  1.0f, -1.0f, -1.0f }, // v1 : ������ �Ʒ� ��
	{  1.0f,  1.0f, -1.0f }, // v2 : ������ �� ��
	{ -1.0f,  1.0f, -1.0f }, // v3 : ���� �� ��
	{ -1.0f, -1.0f,  1.0f }, // v4 : ���� �Ʒ� ��
	{  1.0f, -1.0f,  1.0f }, // v5 : ������ �Ʒ� ��
	{  1.0f,  1.0f,  1.0f }, // v6 : ������ �� ��
	{ -1.0f,  1.0f,  1.0f }  // v7 : ���� �� ��
} };

class CubeMap : public Object {
public:
	glm::vec3 center;
	float size = 10.0f;
	std::vector<Square> faces;

	CubeMap(const std::array<std::string, 6>& textureFiles, const glm::vec3& center = glm::vec3(0.0f), const float& size = 10.0f);
	virtual Hit CheckRayCollision(const Ray& ray);
};