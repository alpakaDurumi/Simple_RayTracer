#include <cmath>

#include "CubeMap.h"
#include "Ray.h"
#include "Texture.h"

// 텍스처 파일 이름 배열을 받아 큐브맵을 생성하는 생성자
CubeMap::CubeMap(const std::array<std::string, 6>& textureFiles) {
	// 텍스처 설정
	for (int i = 0; i < 6; i++)
		faces[i] = std::make_shared<Texture>(textureFiles[i]);
}

glm::vec3 CubeMap::sampleColor(const Ray& ray) {
	int faceIndex = getFaceIndex(ray.dir);
	glm::vec2 uv = getUVCoordinates(ray.dir, faceIndex);
	
	return faces[faceIndex]->Sample(uv);
}

// Ray의 방향을 받아 6개의 면 중 하나를 선택
int CubeMap::getFaceIndex(const glm::vec3& direction) {
	float absX = std::abs(direction.x);
	float absY = std::abs(direction.y);
	float absZ = std::abs(direction.z);

	if (absX > absY && absX > absZ)
		return direction.x > 0 ? 2 : 3;		// +x 또는 -x
	else if (absY > absZ)
		return direction.y > 0 ? 4 : 5;		// +y 또는 -y
	else
		return direction.z > 0 ? 0 : 1;		// +z 또는 -z
}

// Ray의 방향과 면 인덱스를 받아 uv 좌표 계산
glm::vec2 CubeMap::getUVCoordinates(const glm::vec3& direction, int faceIndex) {
	// 방향 벡터의 각 성분에 절댓값 취하기
	glm::vec3 absDir = glm::abs(direction);

	float u, v;

	// 지정된 면을 기준으로 좌표계 변환
	switch (faceIndex) {
	case 0:		// +z
		u = 0.5f * (direction.x / absDir.z + 1.0f);
		v = 1.0f - 0.5f * (direction.y / absDir.z + 1.0f);
		break;
	case 1:		// -z
		u = 0.5f * (-direction.x / absDir.z + 1.0f);
		v = 1.0f - 0.5f * (direction.y / absDir.z + 1.0f);
		break;
	case 2:		// +x
		u = 0.5f * (-direction.z / absDir.x + 1.0f);
		v = 1.0f - 0.5f * (direction.y / absDir.x + 1.0f);
		break;
	case 3:		// -x
		u = 0.5f * (direction.z / absDir.x + 1.0f);
		v = 1.0f - 0.5f * (direction.y / absDir.x + 1.0f);
		break;
	case 4:		// +y
		u = 0.5f * (direction.x / absDir.y + 1.0f);
		v = 1.0f - 0.5f * (-direction.z / absDir.y + 1.0f);
		break;
	case 5:		// -y
		u = 0.5f * (direction.x / absDir.y + 1.0f);
		v = 1.0f - 0.5f * (direction.z / absDir.y + 1.0f);
		break;
	}

	return glm::vec2(u, v);
}