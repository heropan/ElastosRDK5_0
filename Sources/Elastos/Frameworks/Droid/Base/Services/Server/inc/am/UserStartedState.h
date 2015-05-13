
#ifndef __USERSTARTEDSTATE_H__
#define __USERSTARTEDSTATE_H__

#include "ext/frameworkext.h"
#include <elastos/List.h>

using Elastos::IO::IPrintWriter;
using Elastos::Utility::List;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::App::IStopUserCallback;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

extern "C" const InterfaceID EIID_UserStartedState;

class UserStartedState
    : public ElRefBase
    , public IInterface
{
public:
    CAR_INTERFACE_DECL()

    UserStartedState(
        /* [in] */ IUserHandle * handle,
        /* [in] */ Boolean initial);

    virtual ~UserStartedState();

    CARAPI_(void) Dump(
        /* [in] */ const String& prefix,
        /* [in] */ IPrintWriter* pw);

public:
    // User is first coming up.
    static const Int32 STATE_BOOTING;
    // User is in the normal running state.
    static const Int32 STATE_RUNNING;
    // User is in the initial process of being stopped.
    static const Int32 STATE_STOPPING;
    // User is in the final phase of stopping, sending Intent.ACTION_SHUTDOWN.
    static const Int32 STATE_SHUTDOWN;

    AutoPtr<IUserHandle> mHandle;
    List<AutoPtr<IStopUserCallback> > mStopCallbacks;

    Int32 mState;
    Boolean mSwitching;
    Boolean mInitializing;
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__USERSTARTEDSTATE_H__
