
#include "CDrmRightsManagerHelper.h"
#include "CDrmRightsManager.h"

namespace Elastos {
namespace Droid {
namespace Drm {
namespace Mobile1 {

ECode CDrmRightsManagerHelper::GetInstance(
    /* [out] */ IDrmRightsManager** manager)
{
    VALIDATE_NOT_NULL(manager);
    return CDrmRightsManager::GetInstance(manager);
}

} // namespace mobile1
} // namespace drm
} // namepsace Droid
} // namespace Elastos
