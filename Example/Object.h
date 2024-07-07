#pragma once

#include <memory>                           // shared_ptr�� ����� ����
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

// ���� ����
class Hit;
class Ray;
class Texture;

// ��Ƽ����(Phong reflection model)
struct Material {
    glm::vec3 amb = glm::vec3(0.0f);        // Ambient color
    glm::vec3 dif = glm::vec3(0.0f);        // Diffuse color
    glm::vec3 spec = glm::vec3(0.0f);       // Specular color

    float specularPower = 10.0f;            // Specular ����
    float specularCoefficient = 0.5f;            // Specular ���
    float reflection = 0.0f;                // �ݻ�
    float transparency = 0.0f;              // ����

    //std::shared_ptr<Texture> ambTexture;    // Ambient �ؽ�ó
    //std::shared_ptr<Texture> difTexture;    // Diffuse �ؽ�ó
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