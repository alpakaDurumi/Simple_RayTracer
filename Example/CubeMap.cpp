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

// �ؽ�ó ���� �̸� �迭�� �޾� ť����� �����ϴ� ������
CubeMap::CubeMap(const std::array<std::string, 6>& textureFiles, const glm::vec3& center, const float& size)
	: center(center), size(size) {
	// +z, -z, +x, -x, +y, -y ������ ����
	faces.push_back(Square(center + cubeVertices[7] * size, center + cubeVertices[6] * size, center + cubeVertices[5] * size, center + cubeVertices[4] * size));
	faces.push_back(Square(center + cubeVertices[2] * size, center + cubeVertices[3] * size, center + cubeVertices[0] * size, center + cubeVertices[1] * size));
	faces.push_back(Square(center + cubeVertices[6] * size, center + cubeVertices[2] * size, center + cubeVertices[1] * size, center + cubeVertices[5] * size));
	faces.push_back(Square(center + cubeVertices[3] * size, center + cubeVertices[7] * size, center + cubeVertices[4] * size, center + cubeVertices[0] * size));
	faces.push_back(Square(center + cubeVertices[3] * size, center + cubeVertices[2] * size, center + cubeVertices[6] * size, center + cubeVertices[7] * size));
	faces.push_back(Square(center + cubeVertices[4] * size, center + cubeVertices[5] * size, center + cubeVertices[1] * size, center + cubeVertices[0] * size));

	// �ؽ�ó ����
	for (int i = 0; i < 6; i++) {
		faces[i].setTexture(std::make_shared<Texture>(textureFiles[i]));
		faces[i].setAmbientFactor(1.0f);
		faces[i].configureSpecular(0.0f, 0.0f);
	}
}

Hit CubeMap::CheckRayCollision(const Ray& ray) {
	Hit hit = Hit{ -1.0, glm::vec3(0.0), glm::vec3(0.0) };

	// ���� �鿡 �浹�� �� �ֱ� ������ ���� ����� ���� ã�´�
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