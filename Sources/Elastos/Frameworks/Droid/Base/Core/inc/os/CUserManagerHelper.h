
#ifndef __CUSERMANAGERHELPER_H__
#define __CUSERMANAGERHELPER_H__

#include "_CUserManagerHelper.h"

namespace Elastos {
namespace Droid {
namespace Os {

CarClass(CUserManagerHelper)
{
public:
    /**
     * Returns whether the system supports multiple users.
     * @return true if multiple users can be created, false if it is a single user device.
     * @hide
     */
    CARAPI SupportsMultipleUsers(
        /* [out] */ Boolean* result);

    /**
     * Returns the maximum number of users that can be created on this device. A return value
     * of 1 means that it is a single user device.
     * @hide
     * @return a value greater than or equal to 1
     */
    CARAPI GetMaxSupportedUsers(
        /* [out] */ Int32* number);
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__CUSERMANAGERHELPER_H__
