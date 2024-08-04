#pragma once

#include <memory>                           // shared_ptr의 사용을 위해
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

class Hit;
class Ray;
class Texture;

// 머티리얼(Phong reflection model)
struct Material {
    // 기본값은 흰색
    glm::vec3 baseColor = glm::vec3(1.0f);
    glm::vec3 amb = glm::vec3(0.2f);        // Ambient color
    glm::vec3 dif = glm::vec3(0.8f);        // Diffuse color
    glm::vec3 spec = glm::vec3(1.0f);       // Specular color

    float ambientFactor = 0.2f;             // Ambient 계수
    float specularPower = 40.0f;            // Specular 강도
    float specularCoefficient = 1.0f;       // Specular 계수
    float reflection = 0.0f;                // 반사
    float refraction = 0.0f;                // 굴절

    std::shared_ptr<Texture> ambTexture;    // Ambient 텍스처
    std::shared_ptr<Texture> difTexture;    // Diffuse 텍스처
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