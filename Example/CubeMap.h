#pragma once

#include <memory>
#include <array>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

class Texture;
class Ray;

class CubeMap {
public:
	std::array<std::shared_ptr<Texture>, 6> faces;

	CubeMap(const std::array<std::string, 6>& textureFiles);
	glm::vec3 sampleColor(const Ray& ray);
	int getFaceIndex(const glm::vec3& direction);
	glm::vec2 getUVCoordinates(const glm::vec3& direction, int faceIndex);
};