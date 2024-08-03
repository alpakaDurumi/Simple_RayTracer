#include <cmath>

#include "CubeMap.h"
#include "Ray.h"
#include "Texture.h"

// �ؽ�ó ���� �̸� �迭�� �޾� ť����� �����ϴ� ������
CubeMap::CubeMap(const std::array<std::string, 6>& textureFiles) {
	// �ؽ�ó ����
	for (int i = 0; i < 6; i++)
		faces[i] = std::make_shared<Texture>(textureFiles[i]);
}

glm::vec3 CubeMap::sampleColor(const Ray& ray) {
	int faceIndex = getFaceIndex(ray.dir);
	glm::vec2 uv = getUVCoordinates(ray.dir, faceIndex);
	
	return faces[faceIndex]->Sample(uv);
}

// Ray�� ������ �޾� 6���� �� �� �ϳ��� ����
int CubeMap::getFaceIndex(const glm::vec3& direction) {
	float absX = std::abs(direction.x);
	float absY = std::abs(direction.y);
	float absZ = std::abs(direction.z);

	if (absX > absY && absX > absZ)
		return direction.x > 0 ? 2 : 3;		// +x �Ǵ� -x
	else if (absY > absZ)
		return direction.y > 0 ? 4 : 5;		// +y �Ǵ� -y
	else
		return direction.z > 0 ? 0 : 1;		// +z �Ǵ� -z
}

// Ray�� ����� �� �ε����� �޾� uv ��ǥ ���
glm::vec2 CubeMap::getUVCoordinates(const glm::vec3& direction, int faceIndex) {
	// ���� ������ �� ���п� ���� ���ϱ�
	glm::vec3 absDir = glm::abs(direction);

	float u, v;

	// ������ ���� �������� ��ǥ�� ��ȯ
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