
#include "pm/CUserStopUserCallback.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {

CARAPI CUserStopUserCallback::constructor(
    /* [in] */ IIUserManager* um)
{
    mUm = (CUserManagerService*)um;
    return NOERROR;
}

ECode CUserStopUserCallback::UserStopped(
    /* [in] */ Int32 userId)
{
    mUm->FinishRemoveUser(userId);
    return NOERROR;
}

ECode CUserStopUserCallback::UserStopAborted(
    /* [in] */ Int32 userId)
{
    return NOERROR;
}

} // namespace Pm
} // namespace Server
} // namespace Droid
} // namespace Elastos
