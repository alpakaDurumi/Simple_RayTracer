#pragma once

#include <memory>       // shared_ptr의 사용을 위해
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

class Object;           // 전방 선언

class Hit
{
public:
    float d;            // Ray의 시작 지점부터 충돌 지점까지의 거리
    glm::vec3 point;    // 충돌한 위치
    glm::vec3 normal;   // 충돌한 위치에서 표면의 노멀 벡터
    glm::vec2 uv;       // 충돌 지점에 대응되는 텍스처 좌표

    std::shared_ptr<Object> obj; // 나중에 물체의 재질 등을 가져오기 위한 포인터
};