
#include "elastos/droid/server/lights/Light.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Lights {

const Int32 Light::LIGHT_FLASH_NONE = 0;
const Int32 Light::LIGHT_FLASH_TIMED = 1;
const Int32 Light::LIGHT_FLASH_HARDWARE = 2;
const Int32 Light::BRIGHTNESS_MODE_USER = 0;
const Int32 Light::BRIGHTNESS_MODE_SENSOR = 1;

Light::~Light()
{
    Logger::I("Light", "Light::~Light, Light is abstract class");
}

} // Lights
} // Server
} // Droid
} // Elastos
