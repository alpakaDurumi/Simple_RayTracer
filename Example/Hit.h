#pragma once

#include <memory>       // shared_ptr의 사용을 위해
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

// 전방 선언
class Object;           
struct Material;

class Hit {
public:
    float d;            // Ray의 시작 지점부터 충돌 지점까지의 거리
    glm::vec3 point;    // 충돌한 위치
    glm::vec3 normal;   // 충돌한 위치에서 표면의 노멀 벡터
    glm::vec2 uv;       // 충돌 지점에 대응되는 텍스처 좌표

    std::shared_ptr<Material> material; // 충돌 물체의 머터리얼
};