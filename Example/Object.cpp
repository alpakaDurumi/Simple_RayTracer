#include "Object.h"

// mat�� �־����� �״�� �����ϰ�, �־����� ������ ��Ƽ������ ���� ����
Object::Object(const std::shared_ptr<Material>& mat) {
    if (mat)
        material = mat;
    else
        material = std::make_shared<Material>();
}

// �־��� ���� ������ ambient, diffuse ���� ����
// �ʿ� �� ambientFactor �� ����
// specular�� 1.0f�� ����
void Object::setColor(const glm::vec3& color) {
    float ambientFactor = 0.2f;
    material->amb = color * ambientFactor;
    material->dif = color * (1.0f - ambientFactor);
    material->spec = glm::vec3(1.0f);
}

// specularPower�� specularCoefficient�� �����ϴ� �Լ�
void Object::configureSpecular(const float& specularPower, const float& specularCoefficient) {
    material->specularPower = specularPower;
    material->specularCoefficient = specularCoefficient;
}