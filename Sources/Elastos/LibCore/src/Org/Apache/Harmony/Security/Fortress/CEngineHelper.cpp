
#include "CEngineHelper.h"
#include <CEngine.h>

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Fortress {


ECode CEngineHelper::SetDoor(
    /* [in] */ ISecurityAccess* door)
{
    CEngine::mDoor = door;
    return NOERROR;
}

ECode CEngineHelper::GetDoor(
    /* [out] */ ISecurityAccess** door)
{
    VALIDATE_NOT_NULL(door)
    *door = CEngine::mDoor;
    INTERFACE_ADDREF(*door)
    return NOERROR;
}

} // namespace Fortress
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

