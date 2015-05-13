
#ifndef __CUSERSTOPUSERCALLBACK_H__
#define __CUSERSTOPUSERCALLBACK_H__

#include "_CUserStopUserCallback.h"
#include "pm/CUserManagerService.h"

using Elastos::Droid::Os::IIUserManager;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {

CarClass(CUserStopUserCallback)
{
public:
    CARAPI constructor(
        /* [in] */ IIUserManager* mUm);

    CARAPI UserStopped(
        /* [in] */ Int32 userId);

    CARAPI UserStopAborted(
        /* [in] */ Int32 userId);

private:
    CUserManagerService* mUm;
};

} // namespace Pm
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif // __CUSERSTOPUSERCALLBACK_H__
