#include "CubeMap.h"

// +z : 7 6 5 4
// -z : 2 3 0 1
// +x : 6 2 1 5
// -x : 3 7 4 0
// +y : 3 2 6 7
// -y : 4 5 1 0

// +z, -z, +x, -x, +y, -y 순으로 지정
CubeMap::CubeMap(const std::array<std::string, 6>& textureFiles, const glm::vec3& center)
	: center(center) {
	faces[0] = Square(cubeVertices[7], cubeVertices[6], cubeVertices[5], cubeVertices[4]);
	faces[1] = Square(cubeVertices[2], cubeVertices[3], cubeVertices[0], cubeVertices[1]);
	faces[2] = Square(cubeVertices[6], cubeVertices[2], cubeVertices[1], cubeVertices[5]);
	faces[3] = Square(cubeVertices[3], cubeVertices[7], cubeVertices[4], cubeVertices[0]);
	faces[4] = Square(cubeVertices[3], cubeVertices[2], cubeVertices[6], cubeVertices[7]);
	faces[5] = Square(cubeVertices[4], cubeVertices[5], cubeVertices[1], cubeVertices[0]);


	for (int i = 0; i < 6; i++) {
		faces[i].material.ambTexture = std::make_shared<Texture>(textureFiles[i]);
	}
}
