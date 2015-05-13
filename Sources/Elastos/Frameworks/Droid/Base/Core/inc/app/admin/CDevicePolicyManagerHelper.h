
#ifndef __CDEVICEPOLICYMANAGERHELPER_H__
#define __CDEVICEPOLICYMANAGERHELPER_H__

#include "_CDevicePolicyManagerHelper.h"


namespace Elastos {
namespace Droid {
namespace App {
namespace Admin {

CarClass(CDevicePolicyManagerHelper)
{
public:
    /** @hide */
    CARAPI Create(
        /* [in] */ IContext* context,
        /* [in] */ IHandler* handler,
        /* [out] */ IDevicePolicyManager** policymanager);
};

} // namespace Admin
} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__CDEVICEPOLICYMANAGERHELPER_H__
