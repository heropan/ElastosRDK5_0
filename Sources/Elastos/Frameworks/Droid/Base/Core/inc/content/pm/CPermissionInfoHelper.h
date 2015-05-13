
#ifndef __CPERMISSIONINFOHELPER_H__
#define __CPERMISSIONINFOHELPER_H__

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

#endif // __CPERMISSIONINFOHELPER_H__
