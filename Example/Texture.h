#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

enum class TextureAddressMode {
	Clamp,
	Wrap,
	Mirror,
	Border
};

enum class TextureFilterMode {
	Point,
	Bilinear
};

class Texture {
public:
	int width, height, channels;
	std::vector<uint8_t> image;

	TextureAddressMode addressMode;
	TextureFilterMode filterMode;

	// 생성자에서 addressMode와 filterMode를 지정 가능
	// 기본값은 각각 Clamp과 Nearest
	Texture(const std::string& filename, TextureAddressMode addressMode = TextureAddressMode::Clamp, TextureFilterMode filterMode = TextureFilterMode::Point);
	Texture(const int& width, const int& height, const std::vector<glm::vec3>& pixels, TextureAddressMode addressMode = TextureAddressMode::Clamp, TextureFilterMode filterMode = TextureFilterMode::Point);

	void SetAddressMode(TextureAddressMode addressMode);
	void SetFilterMode(TextureFilterMode filterMode);
	glm::vec3 Sample(const glm::vec2& uv);
	glm::vec3 GetClamped(int i, int j);
	glm::vec3 GetWrapped(int i, int j);
	glm::vec3 GetMirrored(int i, int j);
	glm::vec3 GetBordered(int i, int j);
	glm::vec3 InterpolateBilinear(
		const float& dx,
		const float& dy,
		const glm::vec3& c00,
		const glm::vec3& c10,
		const glm::vec3& c01,
		const glm::vec3& c11);
	glm::vec3 FilterNearest(const glm::vec2& samplingPoint);
	glm::vec3 FilterBilinear(const glm::vec2& samplingPoint);
};