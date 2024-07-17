#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "Texture.h"

// 이미지 파일 이름을 지정하여 읽어오는 생성자
Texture::Texture(const std::string& filename, TextureAddressMode addressMode, TextureFilterMode filterMode)
	: addressMode(addressMode), filterMode(filterMode) {
	unsigned char* img = stbi_load(filename.c_str(), &width, &height, &channels, 0);

	image.resize(width * height * channels);
	memcpy(image.data(), img, image.size() * sizeof(uint8_t));

	delete img;
}

// 배열에서 색상 값을 읽어오는 생성자
Texture::Texture(const int& width, const int& height, const std::vector<glm::vec3>& pixels, TextureAddressMode addressMode, TextureFilterMode filterMode)
	: width(width), height(height), channels(3), addressMode(addressMode), filterMode(filterMode) {
	image.resize(width * height * channels);

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			const auto& color = pixels[i + j * width];

			image[(i + width * j) * channels + 0] = uint8_t(color.r * 255);
			image[(i + width * j) * channels + 1] = uint8_t(color.g * 255);
			image[(i + width * j) * channels + 2] = uint8_t(color.b * 255);
		}
	}
}

void Texture::SetAddressMode(TextureAddressMode addressMode) {
	this->addressMode = addressMode;
}

void Texture::SetFilterMode(TextureFilterMode filterMode) {
	this->filterMode = filterMode;
}

// texture addressing mode : Clamp
glm::vec3 Texture::GetClamped(int i, int j) {
	i = glm::clamp(i, 0, width - 1);
	j = glm::clamp(j, 0, height - 1);

	const float r = image[(i + width * j) * channels + 0] / 255.0f;
	const float g = image[(i + width * j) * channels + 1] / 255.0f;
	const float b = image[(i + width * j) * channels + 2] / 255.0f;

	return glm::vec3(r, g, b);
}

// texture addressing mode : Wrap
glm::vec3 Texture::GetWrapped(int i, int j) {
	i = (i + width) % width;
	j = (j + height) % height;

	const float r = image[(i + width * j) * channels + 0] / 255.0f;
	const float g = image[(i + width * j) * channels + 1] / 255.0f;
	const float b = image[(i + width * j) * channels + 2] / 255.0f;

	return glm::vec3(r, g, b);
}

// Bilinear interpolation
glm::vec3 Texture::InterpolateBilinear(
	const float& dx,
	const float& dy,
	const glm::vec3& c00,
	const glm::vec3& c10,
	const glm::vec3& c01,
	const glm::vec3& c11) {
	glm::vec3 a = c00 * (1.0f - dx) + c10 * dx;
	glm::vec3 b = c01 * (1.0f - dx) + c11 * dx;
	return a * (1.0f - dy) + b * dy;
}

// 텍스처 좌표의 범위 uv [0.0, 1.0] x [0.0, 1.0]
// 이미지 좌표의 범위 xy [-0.5, width - 1 + 0.5] x [-0.5, height - 1 + 0.5]

// Nearest sampling이라고 부르기도 함
glm::vec3 Texture::SamplePoint(const glm::vec2& uv)  {
	// 좌표계 변환
	glm::vec2 xy = uv * glm::vec2(width, height) - glm::vec2(0.5f);

	// 반올림을 통해 가장 가까운 좌표 계산 : nearest neighbor filtering
	int i = glm::round(xy.x);
	int j = glm::round(xy.y);

	switch (addressMode) {
	case TextureAddressMode::Clamp:
		return GetClamped(i, j);
		break;
	case TextureAddressMode::Wrap:
		return GetWrapped(i, j);
		break;
	}
}

glm::vec3 Texture::SampleBilinear(const glm::vec2& uv) {
	// 좌표계 변환
	glm::vec2 xy = uv * glm::vec2(width, height) - glm::vec2(0.5f);

	// 주변 4개 텍셀 중 좌상단 텍셀의 좌표
	int i = static_cast<int>(glm::floor(xy.x));
	int j = static_cast<int>(glm::floor(xy.y));
	
	// interpolation을 위한 간격 값
	float dx = xy.x - static_cast<float>(i);
	float dy = xy.y - static_cast<float>(j);

	// bilinear filtering
	switch (addressMode) {
	case TextureAddressMode::Clamp:
		return InterpolateBilinear(dx, dy, GetClamped(i, j), GetClamped(i + 1, j), GetClamped(i, j + 1), GetClamped(i + 1, j + 1));
		break;
	case TextureAddressMode::Wrap:
		return InterpolateBilinear(dx, dy, GetWrapped(i, j), GetWrapped(i + 1, j), GetWrapped(i, j + 1), GetWrapped(i + 1, j + 1));
		break;
	}
}