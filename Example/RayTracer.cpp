#include "RayTracer.h"
#include "Sphere.h"
#include "Hit.h"
#include "Ray.h"
#include "Triangle.h"
#include "Square.h"
#include "Texture.h"
#include "CubeMap.h"

RayTracer::RayTracer(const int& width, const int& height)
	: width(width), height(height) {
	// -z ���⿡ ����
	light = Light{ {0.0f, 0.3f, -0.5f} };

	// �ʷϻ� ��
	//auto sphere1 = std::make_shared<Sphere>(glm::vec3(-1.0f, 0.0f, 1.0f), 0.5f);
	//sphere1->setColor(glm::vec3{ 0.0f, 1.0f, 0.0f });
	//objects.push_back(sphere1);

	// ����� ��
	//auto sphere2 = std::make_shared<Sphere>(glm::vec3(0.5f, 0.0f, 1.0f), 0.5f);
	//sphere2->setColor(glm::vec3{ 1.0f, 0.0f, 1.0f });
	//objects.push_back(sphere2);

	// ��� �簢��
	//auto square1 = std::make_shared<Square>(glm::vec3(-4.0f, -1.0f, 0.0f), glm::vec3(-4.0f, -1.0f, 4.0f), glm::vec3(4.0f, -1.0f, 4.0f), glm::vec3(4.0f, -1.0f, 0.0f));
	//objects.push_back(square1);

	// �ؽ�ó�� �׽�Ʈ�� 1
	//std::vector<glm::vec3> textureImage(4 * 4);
	//for (int j = 0; j < 4; j++) {
	//	for (int i = 0; i < 4; i++) {
	//		if (i % 4 == 0)
	//			textureImage[i + 4 * j] = glm::vec3(1.0f, 0.0f, 0.0f) * (1.0f + j) * 0.25f;
	//		else if (i % 4 == 1)
	//			textureImage[i + 4 * j] = glm::vec3(0.0f, 1.0f, 0.0f) * (1.0f + j) * 0.25f;
	//		else if (i % 4 == 2)
	//			textureImage[i + 4 * j] = glm::vec3(0.0f, 0.0f, 1.0f) * (1.0f + j) * 0.25f;
	//		else
	//			textureImage[i + 4 * j] = glm::vec3(1.0f, 1.0f, 1.0f) * (1.0f + j) * 0.25f;
	//	}
	//}
	//auto testTexture = std::make_shared<Texture>(4, 4, textureImage);
	//testTexture->SetAddressMode(TextureAddressMode::Clamp);
	//testTexture->SetFilterMode(TextureFilterMode::Point);

	// �ؽ�ó�� �׽�Ʈ�� 2
	//Image by freepik https://www.freepik.com/free-ai-image/geometric-seamless-pattern_94949548.htm#fromView=search&page=1&position=0&uuid=4bfff6fb-3412-4780-9681-b7f2c31eaa3b
	//auto testTexture = std::make_shared<Texture>("geometric-seamless-pattern.jpg");
	//testTexture->SetAddressMode(TextureAddressMode::Clamp);
	//testTexture->SetFilterMode(TextureFilterMode::Point);

	//auto squareTest = std::make_shared<Square>(
	//	glm::vec3(-2.0f, 2.0f, 2.0f), glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(2.0f, -2.0f, 2.0f), glm::vec3(-2.0f, -2.0f, 2.0f),
	//	glm::vec2(0.0f, 0.0f), glm::vec2(4.0f, 0.0f), glm::vec2(4.0f, 4.0f), glm::vec2(0.0f, 4.0f));
	//squareTest->setTexture(testTexture);
	//squareTest->configureSpecular(0.0f, 0.0f);
	//squareTest->setAmbientFactor(1.0f);
	//objects.push_back(squareTest);

	// ť��� �׽�Ʈ
	std::array<std::string, 6> cubeMapTextureFiles{ "posz.jpg","negz.jpg","posx.jpg","negx.jpg","posy.jpg", "negy.jpg" };
	auto skyBox = std::make_shared<CubeMap>(cubeMapTextureFiles, glm::vec3(0.0f, 0.0f, 10.0f), 10.0f);
	objects.push_back(skyBox);
}

// ray�� �浹�� ���� �� ���� ����� ������ ���� Hit ��ȯ
Hit RayTracer::FindClosestCollision(const Ray& ray) {
	float closestDistance = 1000.0; // inf
	Hit closestHit = Hit{ -1.0, glm::vec3(0.0), glm::vec3(0.0) };
	
	// ��� ��ü�� ���� �˻�
	for (int i = 0; i < objects.size(); i++) {
		auto hit = objects[i]->CheckRayCollision(ray);
		// �浹�ߴٸ�
		if (hit.d >= 0.0f) {
			if (hit.d < closestDistance) {
				closestDistance = hit.d;
				closestHit = hit;
			}
		}
	}

	return closestHit;
}

// ������ ��ü�� ������ �� ��ü�� �� ��ȯ
glm::vec3 RayTracer::traceRay(const Ray& ray) {
	// ���� ����� �浹�� ���� Hit
	const auto hit = FindClosestCollision(ray);

	// �浹�� ��쿡 ���ؼ��� ����
	// hit.d�� 0���� �۴ٴ� ���� ��� ��ü�͵� �浹���� �ʾҴٴ� ��
	if (hit.d >= 0.0f) {
		glm::vec3 objectColor(0.0f);

		glm::vec3 phongColor;

		// ambient texture�� �����Ǿ� �ִٸ�
		if (hit.material->ambTexture) {
			// �ؽ�ó���� ���ø��� ������ ����
			phongColor = hit.material->amb * hit.material->ambTexture->Sample(hit.uv);
		}
		else {
			// material�� ������ ���� �������� ����
			phongColor = hit.material->amb;
		}

		const glm::vec3 dirToLight = glm::normalize(light.pos - hit.point);

		// �׸��� ����� ���� Ray
		Ray shadowRay{ hit.point + dirToLight * 1e-4f, dirToLight };
		// �浹 �������� ���� ���̸� ���� ��ü������ �Ÿ�
		float distanceToObstacle = FindClosestCollision(shadowRay).d;

		// �浹 ������ ���� ���̿� ���θ��� ��ü�� ���ٸ� diffuse�� specular�� �߰��� ���
		if (distanceToObstacle < 0.0f || glm::length(light.pos - hit.point) < distanceToObstacle) {
			// Diffuse ���
			const float diff = glm::max(dot(hit.normal, dirToLight), 0.0f);

			// diffuse texture�� �����Ǿ� �ִٸ�
			if (hit.material->difTexture) {
				phongColor += hit.material->dif * diff * hit.material->difTexture->Sample(hit.uv);
			}
			else {
				phongColor += hit.material->dif * diff;
			}

			// Specular ���
			const glm::vec3 reflectDir = 2.0f * glm::dot(dirToLight, hit.normal) * hit.normal - dirToLight;
			const float specular = glm::pow(glm::max(glm::dot(-ray.dir, reflectDir), 0.0f), hit.material->specularPower);

			// specular texture�� ��� X
			phongColor += hit.material->spec * specular * hit.material->specularCoefficient;
		}

		objectColor += phongColor * (1.0f - hit.material->reflection - hit.material->transparency);

		return objectColor;
	}

	return glm::vec3(0.0f);
}

// �� �ȼ��� �ش��ϴ� ���� ��ǥ�� ��ġ���� ray�� ���� ���� �������� �� �ȼ��� ����
void RayTracer::Render(std::vector<glm::vec4>& pixels) {
	std::fill(pixels.begin(), pixels.end(), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	const glm::vec3 cameraPos(0.0f, 0.0f, -1.5f);

	// �ȼ��� ũ��
	const float dx = 2.0f / height;

#pragma omp parallel for
	for (int j = 0; j < height; j++)
		for (int i = 0; i < width; i++) {
			const glm::vec3 pixelPosWorld = TransformScreenToWorld(glm::vec2(i, j));
			// For perspective projection

			// ray�� �ȼ� �� �ϳ��� ��� �ڵ�
			Ray pixelRay{ pixelPosWorld, glm::normalize(pixelPosWorld - cameraPos) };
			pixels[i + width * j] = glm::vec4(glm::clamp(traceRay(pixelRay), 0.0f, 1.0f), 1.0f);

			// ray�� ������ ���� ���ۻ��ø��ϴ� �ڵ�
			//const auto pixelColor = SuperSample4x(cameraPos, pixelPosWorld, dx);
			//pixels[i + width * j] = glm::vec4(glm::clamp(pixelColor, 0.0f, 1.0f), 1.0f);
		}
}

// ��ũ�� ��ǥ�� -> ���� ��ǥ�� ��ȯ
// ��� ���� ��ǥ���� ������ [-aspect, aspect] * [1, -1]
// �ȼ��� �߽��� �����ڸ��� ȭ���� �����ڸ��� ��ġ�ϴ� ���� ���
glm::vec3 RayTracer::TransformScreenToWorld(const glm::vec2& screenPos) {
	const float xScale = 2.0f / this->width;
	const float yScale = 2.0f / this->height;
	const float aspect = static_cast<float>(this->width) / this->height;

	return glm::vec3(((screenPos.x + 0.5f) * xScale - 1.0f) * aspect, -(screenPos.y + 0.5f) * yScale + 1.0f, 0.0f);
}

// �� �ȼ��� ���� 4��� ���ۻ��ø�
glm::vec3 RayTracer::SuperSample4x(const glm::vec3& cameraPos, const glm::vec3& pixelPos, const float dx) {
	glm::vec3 pixelColor(0.0f);

	const float subdx = 0.5f * dx;

	const float hor[4] = {-0.5f, 0.5f, -0.5f, 0.5f};
	const float ver[4] = {0.5f, 0.5f, -0.5f, -0.5f};

	for(int i = 0; i < 4; i++) {
		glm::vec3 subPos(pixelPos.x + hor[i] * subdx, pixelPos.y + ver[i] * subdx, pixelPos.z);
		Ray subRay{ subPos, glm::normalize(subPos - cameraPos) };
		pixelColor += traceRay(subRay);
	}

	// ����� ���� ��ȯ
	return pixelColor * 0.25f;
}