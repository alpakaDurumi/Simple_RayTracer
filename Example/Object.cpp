#include "Object.h"

Object::Object(const glm::vec3& color)
    : material(std::make_shared<Material>()) {
    setColor(color);
}

Object::Object(const std::shared_ptr<Material>& mat)
    : material(mat) {}

// 주어진 색상 값으로 ambient, diffuse 값을 결정
// 필요 시 ambientFactor 값 조절
// specular는 일단 1.0f로 설정되어있음
void Object::setColor(const glm::vec3& color) {
    float ambientFactor = 0.2f;
    material->amb = color * ambientFactor;
    material->dif = color * (1.0f - ambientFactor);
    material->spec = glm::vec3{ 1.0f, 1.0f, 1.0f };
}

// specularPower와 specularCoefficient를 설정하는 함수
void Object::configureSpecular(const float& specularPower, const float& specularCoefficient) {
    material->specularPower = specularPower;
    material->specularCoefficient = specularCoefficient;
}