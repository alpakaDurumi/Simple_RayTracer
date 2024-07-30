#include "Object.h"

// mat이 주어지면 그대로 지정하고, 주어지지 않으면 머티리얼을 새로 생성
Object::Object(const std::shared_ptr<Material>& mat) {
    if (mat)
        material = mat;
    else
        material = std::make_shared<Material>();
}

// 머티리얼의 색상을 지정하는 함수
void Object::setColor(const glm::vec3& color) {
    material->baseColor = color;
    updatePhong();
}

// ambient와 diffuse 값의 비율을 조절하는 함수
void Object::setAmbientFactor(const float& ambientFactor) {
    material->ambientFactor = ambientFactor;
    updatePhong();
}

// 위 두 함수에서 수정하는 값을 토대로 phong reflection model의 각 색상 값을 다시 설정하는 함수
// specular는 일단 1.0f로 고정
void Object::updatePhong() {
    material->amb = material->baseColor * material->ambientFactor;
    material->dif = material->baseColor * (1.0f - material->ambientFactor);
    material->spec = glm::vec3(1.0f);
}

// specularPower와 specularCoefficient를 설정하는 함수
void Object::configureSpecular(const float& specularPower, const float& specularCoefficient) {
    material->specularPower = specularPower;
    material->specularCoefficient = specularCoefficient;
}