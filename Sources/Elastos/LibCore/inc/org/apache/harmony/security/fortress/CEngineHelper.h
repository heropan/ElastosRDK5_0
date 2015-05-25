
#ifndef __CENGINEHELPER_H__
#define __CENGINEHELPER_H__

#include "_CEngineHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Fortress {

CarClass(CEngineHelper)
{
public:
    CARAPI SetDoor(
        /* [in] */ ISecurityAccess* door);

    CARAPI GetDoor(
        /* [out] */ ISecurityAccess** door);
};

} // namespace Fortress
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __CENGINEHELPER_H__
