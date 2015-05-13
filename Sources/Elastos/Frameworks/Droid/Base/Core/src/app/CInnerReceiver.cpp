
#include "app/CInnerReceiver.h"
#include "app/ActivityManagerNative.h"
// #include "app/CActivityThread.h"
#include <elastos/Slogger.h>

using Elastos::Droid::App::LoadedPkg;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace App {

ECode CInnerReceiver::constructor(
    /* [in] */ Handle32 rd,
    /* [in] */ Boolean strong)
{
    ((LoadedPkg::ReceiverDispatcher*)rd)->GetWeakReference((IWeakReference**)&mDispatcher);
    mStrongRefRef = strong ? (LoadedPkg::ReceiverDispatcher*)rd : NULL;
    return NOERROR;
}

ECode CInnerReceiver::PerformReceive(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 resultCode,
    /* [in] */ const String& data,
    /* [in] */ IBundle* extras,
    /* [in] */ Boolean ordered,
    /* [in] */ Boolean sticky,
    /* [in] */ Int32 sendingUser)
{
    AutoPtr<IInterface> obj;
    mDispatcher->Resolve(EIID_IInterface, (IInterface**)&obj);
    AutoPtr<LoadedPkg::ReceiverDispatcher> dispatcher;
    if (obj != NULL) {
        dispatcher = (LoadedPkg::ReceiverDispatcher*)obj.Get();
    }

    if (FALSE/*CActivityThread::DEBUG_BROADCAST*/) {
        Int32 seq = -1;
        intent->GetInt32Extra(String("seq"), -1, &seq);
        String action;
        intent->GetAction(&action);
        Slogger::I("CInnerReceiver", "Receiving broadcast %s seq=%d to %p", action.string(), seq, dispatcher.Get());
    }
    if (dispatcher != NULL) {
        dispatcher->PerformReceive(
                intent, resultCode, data, extras, ordered, sticky, sendingUser);
        return NOERROR;
    }
    else {
        // The activity manager dispatched a broadcast to a registered
        // receiver in this process, but before it could be delivered the
        // receiver was unregistered.  Acknowledge the broadcast on its
        // behalf so that the system's broadcast sequence can continue.
        if (FALSE/*CActivityThread::DEBUG_BROADCAST*/) {
            Slogger::I("CInnerReceiver", "Finishing broadcast to unregistered receiver");
        }
        AutoPtr<IIActivityManager> mgr = ActivityManagerNative::GetDefault();
        //try {
        if (extras != NULL) {
            extras->SetAllowFds(FALSE);
        }

        ECode ec = mgr->FinishReceiver((IBinder*)this->Probe(EIID_IBinder),
                resultCode, data, extras, FALSE);
        if (FAILED(ec)) {
            Slogger::W("CInnerReceiver", "Couldn't finish broadcast to unregistered receiver");
        }
        // } catch () {
        //
        // }
    }
    return NOERROR;
}

ECode CInnerReceiver::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = String("CInnerReceiver");
    return NOERROR;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
