
#ifndef __ELASTOS_DROID_CONTENT_PM_CPERMISSIONINFOHELPER_H__
#define __ELASTOS_DROID_CONTENT_PM_CPERMISSIONINFOHELPER_H__

#include "_CPermissionInfoHelper.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CarClass(CPermissionInfoHelper)
{
public:
    /** @hide */
    CARAPI FixProtectionLevel(
        /* [in] */ Int32 level,
        /* [out] */ Int32* retLevel);

    /** @hide */
    CARAPI ProtectionToString(
        /* [in] */ Int32 level,
        /* [out] */ String* protLevel);
};

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_CONTENT_PM_CPERMISSIONINFOHELPER_H__
