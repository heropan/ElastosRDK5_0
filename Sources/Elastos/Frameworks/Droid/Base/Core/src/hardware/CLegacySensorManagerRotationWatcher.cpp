#include "hardware/CLegacySensorManagerRotationWatcher.h"
#include "hardware/LegacySensorManager.h"

namespace Elastos {
namespace Droid {
namespace Hardware {

CLegacySensorManagerRotationWatcher::CLegacySensorManagerRotationWatcher()
{}

ECode CLegacySensorManagerRotationWatcher::constructor()
{
    return NOERROR;
}

ECode CLegacySensorManagerRotationWatcher::OnRotationChanged(
    /* [in] */ Int32 rotation)
{
    LegacySensorManager::OnRotationChanged(rotation);
    return NOERROR;
}

} // namespace Hardware
} // namespace Droid
} // namespace Elastos