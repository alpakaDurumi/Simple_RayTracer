#include "Object.h"

Object::Object(const glm::vec3& color)
    : material(std::make_shared<Material>()) {
    setColor(color);
}

Object::Object(const std::shared_ptr<Material>& mat)
    : material(mat) {}

// �־��� ���� ������ ambient, diffuse ���� ����
// �ʿ� �� ambientFactor �� ����
// specular�� �ϴ� 1.0f�� �����Ǿ�����
void Object::setColor(const glm::vec3& color) {
    float ambientFactor = 0.2f;
    material->amb = color * ambientFactor;
    material->dif = color * (1.0f - ambientFactor);
    material->spec = glm::vec3{ 1.0f, 1.0f, 1.0f };
}

// specularPower�� specularCoefficient�� �����ϴ� �Լ�
void Object::configureSpecular(const float& specularPower, const float& specularCoefficient) {
    material->specularPower = specularPower;
    material->specularCoefficient = specularCoefficient;
}