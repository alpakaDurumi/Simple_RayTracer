#include "CubeMap.h"
#include "Hit.h"

#include <numeric>
#include <iostream>

// +z : 7 6 5 4
// -z : 2 3 0 1
// +x : 6 2 1 5
// -x : 3 7 4 0
// +y : 3 2 6 7
// -y : 4 5 1 0

// 텍스처 파일 이름 배열을 받아 큐브맵을 생성하는 생성자
CubeMap::CubeMap(const std::array<std::string, 6>& textureFiles, const glm::vec3& center, const float& size)
	: center(center), size(size) {
	// +z, -z, +x, -x, +y, -y 순으로 지정
	faces.push_back(Square(center + cubeVertices[7] * size, center + cubeVertices[6] * size, center + cubeVertices[5] * size, center + cubeVertices[4] * size));
	faces.push_back(Square(center + cubeVertices[2] * size, center + cubeVertices[3] * size, center + cubeVertices[0] * size, center + cubeVertices[1] * size));
	faces.push_back(Square(center + cubeVertices[6] * size, center + cubeVertices[2] * size, center + cubeVertices[1] * size, center + cubeVertices[5] * size));
	faces.push_back(Square(center + cubeVertices[3] * size, center + cubeVertices[7] * size, center + cubeVertices[4] * size, center + cubeVertices[0] * size));
	faces.push_back(Square(center + cubeVertices[3] * size, center + cubeVertices[2] * size, center + cubeVertices[6] * size, center + cubeVertices[7] * size));
	faces.push_back(Square(center + cubeVertices[4] * size, center + cubeVertices[5] * size, center + cubeVertices[1] * size, center + cubeVertices[0] * size));

	// 텍스처 설정
	for (int i = 0; i < 6; i++) {
		faces[i].setTexture(std::make_shared<Texture>(textureFiles[i]));
		faces[i].setAmbientFactor(1.0f);
		faces[i].configureSpecular(0.0f, 0.0f);
	}
}

Hit CubeMap::CheckRayCollision(const Ray& ray) {
	Hit hit = Hit{ -1.0, glm::vec3(0.0), glm::vec3(0.0) };

	// 여러 면에 충돌할 수 있기 때문에 가장 가까운 곳을 찾는다
	float min_distance = std::numeric_limits<float>::max();
	
	for (int i = 0; i < 6; i++) {
		Hit h = faces[i].CheckRayCollision(ray);
		if (0.0f <= h.d && h.d < min_distance) {
			min_distance = h.d;
			hit = h;
		}
	}

	return hit;
}