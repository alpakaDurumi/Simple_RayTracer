#pragma once

#include <memory>       // shared_ptr�� ����� ����
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

class Object;           // ���� ����

class Hit
{
public:
    float d;            // Ray�� ���� �������� �浹 ���������� �Ÿ�
    glm::vec3 point;    // �浹�� ��ġ
    glm::vec3 normal;   // �浹�� ��ġ���� ǥ���� ��� ����
    glm::vec2 uv;       // �浹 ������ �����Ǵ� �ؽ�ó ��ǥ

    std::shared_ptr<Object> obj; // ���߿� ��ü�� ���� ���� �������� ���� ������
};