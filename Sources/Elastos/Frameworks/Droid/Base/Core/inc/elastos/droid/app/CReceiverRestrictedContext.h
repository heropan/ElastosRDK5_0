
#ifndef __ELASTOS_DROID_APP_CRECEIVERRESTRICTEDCONTEXT_H__
#define __ELASTOS_DROID_APP_CRECEIVERRESTRICTEDCONTEXT_H__

#include "_Elastos_Droid_App_CReceiverRestrictedContext.h"
#include "elastos/droid/ext/frameworkext.h"
#include "ReceiverRestrictedContext.h"

using Elastos::Droid::App::ReceiverRestrictedContext;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::IServiceConnection;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IComponentCallbacks;
using Elastos::Droid::Content::ISharedPreferences;
using Elastos::Droid::Content::IIntentSender;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::IHandler;


namespace Elastos {
namespace Droid {
namespace App {

CarClass(CReceiverRestrictedContext), public ReceiverRestrictedContext
{

public:
    ICONTEXT_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    /**
     * @return the base context as set by the constructor or setBaseContext
     */
    CARAPI GetBaseContext(
        /* [out] */ IContext** context);

private:
};

}
}
}

#endif // __ELASTOS_DROID_APP_CRECEIVERRESTRICTEDCONTEXT_H__
