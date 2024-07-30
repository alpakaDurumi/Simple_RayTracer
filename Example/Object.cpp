#include "Object.h"

// mat�� �־����� �״�� �����ϰ�, �־����� ������ ��Ƽ������ ���� ����
Object::Object(const std::shared_ptr<Material>& mat) {
    if (mat)
        material = mat;
    else
        material = std::make_shared<Material>();
}

// ��Ƽ������ ������ �����ϴ� �Լ�
void Object::setColor(const glm::vec3& color) {
    material->baseColor = color;
    updatePhong();
}

// ambient�� diffuse ���� ������ �����ϴ� �Լ�
void Object::setAmbientFactor(const float& ambientFactor) {
    material->ambientFactor = ambientFactor;
    updatePhong();
}

// �� �� �Լ����� �����ϴ� ���� ���� phong reflection model�� �� ���� ���� �ٽ� �����ϴ� �Լ�
// specular�� �ϴ� 1.0f�� ����
void Object::updatePhong() {
    material->amb = material->baseColor * material->ambientFactor;
    material->dif = material->baseColor * (1.0f - material->ambientFactor);
    material->spec = glm::vec3(1.0f);
}

// specularPower�� specularCoefficient�� �����ϴ� �Լ�
void Object::configureSpecular(const float& specularPower, const float& specularCoefficient) {
    material->specularPower = specularPower;
    material->specularCoefficient = specularCoefficient;
}