#pragma once

#include <memory>                           // shared_ptr의 사용을 위해
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

// 전방 선언
class Hit;
class Ray;
class Texture;

// 머티리얼(Phong reflection model)
struct Material {
    glm::vec3 amb = glm::vec3(0.0f);        // Ambient color
    glm::vec3 dif = glm::vec3(0.0f);        // Diffuse color
    glm::vec3 spec = glm::vec3(0.0f);       // Specular color

    float specularPower = 10.0f;            // Specular 강도
    float specularCoefficient = 0.5f;            // Specular 계수
    float reflection = 0.0f;                // 반사
    float transparency = 0.0f;              // 투명도

    //std::shared_ptr<Texture> ambTexture;    // Ambient 텍스처
    //std::shared_ptr<Texture> difTexture;    // Diffuse 텍스처
};

class Object
{
public:
    Material material;

    Object(const glm::vec3& color = { 1.0f, 1.0f, 1.0f });
    
    void setColor(const glm::vec3& color);
    void configureSpecular(const float& specularPower, const float& specularCoefficient);
    virtual Hit CheckRayCollision(Ray& ray) = 0;
};