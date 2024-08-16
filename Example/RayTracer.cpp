#include "RayTracer.h"
#include "Sphere.h"
#include "Hit.h"
#include "Ray.h"
#include "Triangle.h"
#include "Square.h"
#include "Texture.h"
#include "CubeMap.h"

Camera::Camera(const int& width, const int& height)
	: width(width), height(height) {
	aspectRatio = static_cast<float>(width) / height;
}

// ȸ�� ������Ʈ �Լ�. �ð� ���� �۷ι� ȸ��
// �Ź� �� ȸ�� ����� ����ϴ� ���
// ȸ�� ����� �����ϴ� ��Ŀ� ���� ��������, ��ġ ������ �������� �ʾ� ���е��� ����
void Camera::updateRotation(const float& deltaYaw, const float& deltaPitch) {
	// yaw�� pitch ������Ʈ
	yaw += deltaYaw;
	pitch += deltaPitch;

	// ȸ�� ��� ���
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));	// yaw
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));				// pitch

	// ī�޶� ���� ���� ������Ʈ
	forward = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));
	right = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));
	// roll ȸ���� �������� ������, forward�� right�� ����ǹǷ� �׿� �°� ������Ʈ�� �ʿ�
	up = glm::normalize(glm::cross(forward, right));
}

RayTracer::RayTracer(const int& width, const int& height)
	: camera(width, height) {
	// -z ���⿡ ����
	light = Light{ {0.0f, 0.3f, -0.5f} };

	// ť���
	std::array<std::string, 6> cubeMapTextureFiles{ "posz.jpg","negz.jpg","posx.jpg","negx.jpg","posy.jpg", "negy.jpg" };
	skyBox = std::make_shared<CubeMap>(cubeMapTextureFiles);

	//auto sphere1 = std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, 3.0f), 1.5f);
	//sphere1->setReflection(true);
	//sphere1->setRefraction(true);
	//sphere1->setMaterialType(MaterialType::Glass);
	//objects.push_back(sphere1);

	// ��� �� �׽�Ʈ
	auto square1 = std::make_shared<Square>(glm::vec3(-5.0f, -3.0f, -5.0f), glm::vec3(-5.0f, -3.0f, 5.0f), glm::vec3(5.0f, -3.0f, 5.0f), glm::vec3(4.0f, -3.0f, -5.0f));
	objects.push_back(square1);
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
glm::vec3 RayTracer::traceRay(const Ray& ray, const int recurseLevel) {
	if (recurseLevel < 0)
		return glm::vec3(0.0f);

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

		// ������ �������� ��ü�� �ٱ����� ������ �����ϱ� ����
		const bool isFromOutside = glm::dot(ray.dir, hit.normal) < 0.0f;

		// ���� �ٻ�, ���� ��꿡 ����ϱ� ���� ��� ����
		const glm::vec3 normal = (isFromOutside) ? hit.normal : -hit.normal;

		// �Ի簢�� ���� cos
		const float cosThetaIn = glm::dot(-ray.dir, normal);

		// �ݻ����� ������
		float reflectance = 0.0f, transmittance = 0.0f;

		// ��Ƽ������ �ݻ� ������ ���� ������ ���� �ݻ����� ������ ����
		if (hit.material->hasReflection && hit.material->hasRefraction) {
			// �� ��� �����ϴ� ��� ���� �ٻ�� ������ ��� ���
			if(isFromOutside)
				calculateReflectanceAndTransmittance(1.0f, hit.material->refractiveIndex, cosThetaIn, reflectance, transmittance);
			else
				calculateReflectanceAndTransmittance(hit.material->refractiveIndex, 1.0f, cosThetaIn, reflectance, transmittance);
		}
		else {
			reflectance = (hit.material->hasReflection) ? 1.0f : 0.0f;
			transmittance = (hit.material->hasRefraction) ? 1.0f : 0.0f;
		}

		// ��Ƽ������ �ݻ� ������ ���� ������ ���� phongColor�� ���� ����
		// �ݻ�� ������ �� �� ���� ��� �״�� �����ָ�, �� ���� ��� 0.1�� ������ ���Ѵ�
		objectColor += phongColor * ((hit.material->hasReflection || hit.material->hasRefraction) ? 0.1f : 1.0f);

		// �ݻ簡 �����Ǿ� �ִٸ�
		if (hit.material->hasReflection) {
			const glm::vec3 reflectedDir = 2.0f * dot(hit.normal, -ray.dir) * hit.normal + ray.dir;
			Ray reflectionRay{ hit.point + reflectedDir * 1e-4f, reflectedDir };
			objectColor += traceRay(reflectionRay, recurseLevel - 1) * reflectance;
		}

		// ������ �����Ǿ� �ִٸ�
		if (hit.material->hasRefraction) {
			// ��� ������
			const float relativeRefractiveIndex = (isFromOutside) ? hit.material->refractiveIndex / 1.0f : 1.0f / hit.material->refractiveIndex;

			// �Ի簢�� ���� sin
			const float sinThetaIn = glm::sqrt(1.0f - cosThetaIn * cosThetaIn);

			// �������� ���� sin, cos
			const float sinThetaRef = sinThetaIn / relativeRefractiveIndex;
			const float cosThetaRef = glm::sqrt(1.0f - sinThetaRef * sinThetaRef);

			// ���� ������ x ����(���� * ũ��)
			const glm::vec3 x = glm::normalize(cosThetaIn * normal + ray.dir) * sinThetaRef;
			// ���� ������ y ����(���� * ũ��)
			const glm::vec3 y = -normal * cosThetaRef;

			// ���� ����
			const glm::vec3 refractedDir = glm::normalize(x + y);

			Ray refractionRay{ hit.point + refractedDir * 1e-4f, refractedDir };
			objectColor += traceRay(refractionRay, recurseLevel) * transmittance;
		}

		return objectColor;
	}

	// ��ü�� �ε����� �ʾҰ�, ��ī�̹ڽ��� �����Ѵٸ� ��ī�̹ڽ� ���ø�
	if (skyBox)
		return skyBox->sampleColor(ray);

	return glm::vec3(0.0f);
}

// Schlick's approximation
float RayTracer::fresnelSchlick(const float& n1, const float& n2, const float& cosTheta) {
	float r0 = glm::pow((n1 - n2) / (n1 + n2), 2.0f);
	return r0 + (1 - r0) * glm::pow((1 - cosTheta), 5.0f);
}

// �ݻ���(reflectance)�� ������(transmittance) ���
void RayTracer::calculateReflectanceAndTransmittance(const float& n1, const float& n2, const float& cosTheta, float& reflectance, float& transmittance) {
	reflectance = fresnelSchlick(n1, n2, cosTheta);
	transmittance = 1.0f - reflectance;
}

// �� �ȼ��� �ش��ϴ� ���� ��ǥ�� ��ġ���� ray�� ���� ���� �������� �� �ȼ��� ����
void RayTracer::Render(std::vector<glm::vec4>& pixels) {
	std::fill(pixels.begin(), pixels.end(), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	// �ȼ��� ũ��
	const float dx = 2.0f / camera.height;

#pragma omp parallel for
	for (int j = 0; j < camera.height; j++)
		for (int i = 0; i < camera.width; i++) {
			const glm::vec3 pixelPosWorld = TransformScreenToWorld(glm::vec2(i, j));
			// For perspective projection

			// ray�� �ȼ� �� �ϳ��� ��� �ڵ�
			Ray pixelRay{ pixelPosWorld, glm::normalize(pixelPosWorld - camera.position) };
			pixels[i + camera.width * j] = glm::vec4(glm::clamp(traceRay(pixelRay, 5), 0.0f, 1.0f), 1.0f);

			// ray�� ������ ���� ���ۻ��ø��ϴ� �ڵ�
			//const auto pixelColor = SuperSample4x(camera.position, pixelPosWorld, dx);
			//pixels[i + width * j] = glm::vec4(glm::clamp(pixelColor, 0.0f, 1.0f), 1.0f);
		}
}

// ��ũ�� ��ǥ�� -> ���� ��ǥ�� ��ȯ
glm::vec3 RayTracer::TransformScreenToWorld(const glm::vec2& screenPos) {
	// NDC ��ǥ�� ��ȯ
	// �ȼ� �߽� ��ǥ�� ����Ͽ� 0.5 ������ �߰�
	const float ndcX = 2.0f * (screenPos.x + 0.5f) / camera.width - 1.0f;
	const float ndcY = 1.0f - 2.0f * (screenPos.y + 0.5f) / camera.height;

	// ���� ��ǥ�� ��ȯ
	// �þ߰�(fov)�� ��Ⱦ�� ���
	// fov�� ������ �־����� ������ ���������� ��ȯ�� �ʿ�
	const float worldX = ndcX * camera.aspectRatio * tan(glm::radians(camera.fov / 2.0f));
	const float worldY = ndcY * tan(glm::radians(camera.fov / 2.0f));

	// ī�޶� �� ���
	glm::mat4 viewMatrix = glm::lookAt(camera.position, camera.position + camera.forward, camera.up);

	// ������� �����Ͽ� ���� ��ǥ�迡���� ȸ��(����) ���
	glm::vec4 worldDir = glm::inverse(viewMatrix) * glm::vec4(worldX, worldY, 1.0f, 0.0f);
	
	// ī�޶� ��ġ�� ���Ͽ� ���� ���� ��ǥ ���
	glm::vec3 worldPos = camera.position + glm::vec3(worldDir);
	return worldPos;
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
		pixelColor += traceRay(subRay, 5);
	}

	// ����� ���� ��ȯ
	return pixelColor * 0.25f;
}