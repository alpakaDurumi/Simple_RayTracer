#include "Object.h"

// mat이 주어지면 그대로 지정하고, 주어지지 않으면 머티리얼을 새로 생성
Object::Object(const std::shared_ptr<Material>& mat) {
    if (mat)
        material = mat;
    else
        material = std::make_shared<Material>();
}

// 주어진 색상 값으로 ambient, diffuse 값을 결정
// 필요 시 ambientFactor 값 조절
// specular는 1.0f로 설정
void Object::setColor(const glm::vec3& color) {
    float ambientFactor = 0.2f;
    material->amb = color * ambientFactor;
    material->dif = color * (1.0f - ambientFactor);
    material->spec = glm::vec3(1.0f);
}

// specularPower와 specularCoefficient를 설정하는 함수
void Object::configureSpecular(const float& specularPower, const float& specularCoefficient) {
    material->specularPower = specularPower;
    material->specularCoefficient = specularCoefficient;
}