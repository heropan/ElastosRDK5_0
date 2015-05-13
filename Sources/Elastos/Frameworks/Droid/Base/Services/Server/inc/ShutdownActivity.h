
#ifndef __SHUTDOWNACTIVITY_H__
#define __SHUTDOWNACTIVITY_H__

#include "app/Activity.h"
#include <elastos/Thread.h>
#include <elastos/ThreadBase.h>

using Elastos::Core::Threading::Thread;
using Elastos::Core::Threading::ThreadBase;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::App::Activity;

namespace Elastos {
namespace Droid {
namespace Server {

class ShutdownActivity
    : public Activity
{
private:
    class LocalThread
        : public ThreadBase
    {
    public:
        LocalThread(
            /* [in] */ ShutdownActivity* host,
            /* [in] */ const String& name)
            : mHost(host)
        {
            Thread::Init(name);
        }

        CARAPI Run();

    private:
        ShutdownActivity* mHost;
    };

public:
    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

private:
    static const String TAG;
    Boolean mReboot;
    Boolean mConfirm;
};

} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__SHUTDOWNACTIVITY_H__