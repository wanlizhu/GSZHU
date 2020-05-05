#include "scene/camera/camera.h"
#include <glm/gtc/type_ptr.hpp>

static_assert(sizeof(glm::CameraData) % (sizeof(glm::vec4)) == 0, "CameraData size should be a multiple of 16");

namespace djv
{
    
}