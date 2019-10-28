#include "Precomp.h"
#include "BufferObject.h"

/// index buffer objects
RegisterIndexBufferObject(uint8_t, GL_UNSIGNED_BYTE, 1)
RegisterIndexBufferObject(uint32_t, GL_UNSIGNED_INT, 1)

/// buffer objects
RegisterBufferObject(uint8_t, GL_UNSIGNED_BYTE, 1)
RegisterBufferObject(uint32_t, GL_UNSIGNED_INT, 1)

RegisterBufferObject(float, GL_FLOAT, 1)

RegisterBufferObject(glm::vec2, GL_FLOAT, 2)
RegisterBufferObject(glm::vec3, GL_FLOAT, 3)
RegisterBufferObject(glm::vec4, GL_FLOAT, 4)

RegisterBufferObject(glm::tvec4<uint8_t>, GL_UNSIGNED_BYTE, 4)