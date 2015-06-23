
#ifndef __RECEIVERRESTRICTEDCONTEXT_H__
#define __RECEIVERRESTRICTEDCONTEXT_H__

#include "content/ContextWrapper.h"

using Elastos::Droid::Content::IServiceConnection;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::ContextWrapper;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;

namespace Elastos {
namespace Droid {
namespace App {

class ReceiverRestrictedContext : public ContextWrapper
{
public:
    ReceiverRestrictedContext(
        /* [in] */ IContext* base);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI RegisterReceiver(
        /* [in] */ IBroadcastReceiver* receiver,
        /* [in] */ IIntentFilter* filter,
        /* [out] */ IIntent** intent);

    CARAPI RegisterReceiver(
        /* [in] */ IBroadcastReceiver* receiver,
        /* [in] */ IIntentFilter* filter,
        /* [in] */ const String& broadcastPermission,
        /* [in] */ IHandler* scheduler,
        /* [out] */ IIntent** intent);

    CARAPI RegisterReceiverAsUser(
        /* [in] */ IBroadcastReceiver* receiver,
        /* [in] */ IUserHandle* user,
        /* [in] */ IIntentFilter* filter,
        /* [in] */ const String& broadcastPermission,
        /* [in] */ IHandler* scheduler,
        /* [out] */ IIntent** intent);

    CARAPI BindService(
        /* [in] */ IIntent* service,
        /* [in] */ IServiceConnection* conn,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* succeeded);

protected:
    ReceiverRestrictedContext();

    CARAPI Init(
        /* [in] */ IContext* context);
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif // __RECEIVERRESTRICTEDCONTEXT_H__