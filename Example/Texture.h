#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

class Texture {
public:
	int width, height, channels;
	std::vector<uint8_t> image;

	Texture(const std::string& filename);
	Texture(const int& width, const int& height, const std::vector<glm::vec3>& pixels);

	glm::vec3 GetClamped(int i, int j);
	glm::vec3 GetWrapped(int i, int j);
	glm::vec3 InterpolateBilinear(
		const float& dx,
		const float& dy,
		const glm::vec3& c00,
		const glm::vec3& c10,
		const glm::vec3& c01,
		const glm::vec3& c11);
	glm::vec3 SamplePoint(const glm::vec2& uv);
	glm::vec3 SampleLinear(const glm::vec2& uv);
};