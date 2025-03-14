#include "Vector.h"

const FVector FVector::ZeroVector = {};
const FVector FVector::OneVector = {};

const FVector FVector::ForwardVector = { 1.0f, 0.0f, 0.0f };
const FVector FVector::RightVector = { 0.0f, 1.0f, 0.0f };
const FVector FVector::UpVector = { 0.0f, 0.0f, 1.0f };

const FVector4 FVector4::WHITE = { 1.0f, 1.0f, 1.0f, 1.0f };
const FVector4 FVector4::RED = { 1.0f, 0.0f, 0.0f, 1.0f };
const FVector4 FVector4::GREEN = { 0.0f, 1.0f, 0.0f, 1.0f };
const FVector4 FVector4::BLUE = { 0.0f, 0.0f, 1.0f, 1.0f };
const FVector4 FVector4::BLACK = { 0.0f, 0.0f, 0.0f, 1.0f };

// 두 개의 채널이 1인 색상들
const FVector4 FVector4::YELLOW = { 1.0f, 1.0f, 0.0f, 1.0f };    // 빨강 + 초록 = 노랑
const FVector4 FVector4::CYAN = { 0.0f, 1.0f, 1.0f, 1.0f };      // 초록 + 파랑 = 청록색/시안
const FVector4 FVector4::MAGENTA = { 1.0f, 0.0f, 1.0f, 1.0f };   // 빨강 + 파랑 = 자홍색/마젠타

// 추가적인 색상들
const FVector4 FVector4::ORANGE = { 1.0f, 0.5f, 0.0f, 1.0f };    // 주황색
const FVector4 FVector4::PURPLE = { 0.5f, 0.0f, 0.5f, 1.0f };    // 보라색
const FVector4 FVector4::TEAL = { 0.0f, 0.5f, 0.5f, 1.0f };      // 틸(짙은 청록색)

const FVector4 FVector4::ONE = { 1.0f, 1.0f, 1.0f, 1.0f };
const FVector4 FVector4::ONENULL = { 1.0f, 1.0f, 1.0f, 0.0f };
const FVector4 FVector4::ZERO = { 0.0f, 0.0f, 0.0f, 1.0f };
const FVector4 FVector4::ZERONULL = { 0.0f, 0.0f, 0.0f, 0.0f };

const FVector4 FVector4::LEFT = { -1.0f, 0.0f, 0.0f, 0.0f };
const FVector4 FVector4::RIGHT = { 1.0f, 0.0f, 0.0f, 0.0f };
const FVector4 FVector4::UP = { 0.0f, 1.0f, 0.0f, 0.0f };
const FVector4 FVector4::DOWN = { 0.0f, -1.0f, 0.0f, 0.0f };
const FVector4 FVector4::FORWARD = { 0.0f, 0.0f, 1.0f, 0.0f };
const FVector4 FVector4::BACKWARD = { 0.0f, 0.0f, -1.0f, 0.0f };