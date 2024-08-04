#pragma once

#include <memory>                           // shared_ptr�� ����� ����
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

class Hit;
class Ray;
class Texture;

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
    float reflection = 0.0f;                // �ݻ�
    float refraction = 0.0f;                // ����

    std::shared_ptr<Texture> ambTexture;    // Ambient �ؽ�ó
    std::shared_ptr<Texture> difTexture;    // Diffuse �ؽ�ó
};

class Object {
public:
    std::shared_ptr<Material> material;

    Object(const std::shared_ptr<Material>& mat = nullptr);
    
    void setColor(const glm::vec3& color);
    void setAmbientFactor(const float& ambientFactor);
    void updatePhong();
    void configureSpecular(const float& specularPower, const float& specularCoefficient);
    virtual Hit CheckRayCollision(const Ray& ray) = 0;
};