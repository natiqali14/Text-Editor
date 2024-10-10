#pragma once
#include <cstdint>
using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8 = uint8_t;

using i64 = int64_t;
using i32 = int32_t;
using i16 = int16_t;
using i8 = int8_t;

using f32 = float;
using f64 = double;

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "EditorSettings.h"

#define Character char

#define glERROR(msg) {\
    auto e =glGetError();\ 
    if (e != GL_NO_ERROR) std::cout << " " << e <<" "<< msg << " " <<"\n";\
}

#define DEBUG 1