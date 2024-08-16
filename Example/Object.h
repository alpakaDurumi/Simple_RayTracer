#pragma once

#include <memory>                           // shared_ptr�� ����� ����
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

class Hit;
class Ray;
class Texture;

enum class MaterialType {
	Glass,
	Water,
	Air,
};

// ��Ƽ����(Phong reflection model)
struct Material {
	// �⺻���� ���
	glm::vec3 baseColor = glm::vec3(1.0f);

	glm::vec3 amb = glm::vec3(0.2f);        // Ambient color
	glm::vec3 dif = glm::vec3(0.8f);        // Diffuse color
	glm::vec3 spec = glm::vec3(1.0f);       // Specular color

	float ambientFactor = 0.2f;             // Ambient ���
	float specularPower = 40.0f;            // Specular ����
	float specularCoefficient = 1.0f;       // Specular ���

	std::shared_ptr<Texture> ambTexture;    // Ambient �ؽ�ó
	std::shared_ptr<Texture> difTexture;    // Diffuse �ؽ�ó
	std::shared_ptr<Texture> normalMap;		// ��� �� �ؽ�ó

	bool hasReflection = false;				// �ݻ� ����
	bool hasRefraction = false;				// ���� ����
	float refractiveIndex = 1.0f;			// ������
};

class Object {
public:
	std::shared_ptr<Material> material;

	Object(const std::shared_ptr<Material>& mat = nullptr);
	
	void setColor(const glm::vec3& color);
	void setAmbientFactor(const float& ambientFactor);
	void updatePhong();
	void configureSpecular(const float& specularPower, const float& specularCoefficient);
	void setReflection(const bool& value);
	void setRefraction(const bool& value);
	void setMaterialType(const MaterialType& type);
	virtual Hit CheckRayCollision(const Ray& ray) = 0;
};