
#ifndef __CMASTERCLEARRECEIVER_H__
#define __CMASTERCLEARRECEIVER_H__

#include "_CMasterClearReceiver.h"
#include "ext/frameworkext.h"
#include "content/BroadcastReceiver.h"
#include <elastos/ThreadBase.h>

using Elastos::Core::Threading::IThread;
using Elastos::Core::Threading::ThreadBase;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;

namespace Elastos {
namespace Droid {
namespace Server {

class CMasterClearReceiver : public BroadcastReceiver
{
private:
    class RebootThread
        : public ThreadBase
    {
        friend class FileObserver;
    public:
        RebootThread();

        CARAPI Run();
    };

public:
    CARAPI OnReceive(
        /* [in] */ IContext* context,
        /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("CMasterClearReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
private:
    AutoPtr<RebootThread> mRebootThread;
};

} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__CMASTERCLEARRECEIVER_H__
