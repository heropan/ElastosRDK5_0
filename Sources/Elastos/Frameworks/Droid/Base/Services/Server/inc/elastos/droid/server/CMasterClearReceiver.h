
#ifndef __ELASTOS_DROID_SERVER_CMASTERCLEARRECEIVER_H__
#define __ELASTOS_DROID_SERVER_CMASTERCLEARRECEIVER_H__

#include "_Elastos_Droid_Server_CMasterClearReceiver.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/content/BroadcastReceiver.h"

using Elastos::Core::IThread;
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

#endif //__ELASTOS_DROID_SERVER_CMASTERCLEARRECEIVER_H__
