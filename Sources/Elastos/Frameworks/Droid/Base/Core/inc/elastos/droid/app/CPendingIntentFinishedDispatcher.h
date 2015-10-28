#ifndef __ELASTOS_DROID_APP_CPENDINGINTENT_FINISHDISPATCHER_H__
#define __ELASTOS_DROID_APP_CPENDINGINTENT_FINISHDISPATCHER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "_Elastos_Droid_App_CPendingIntentFinishedDispatcher.h"
#include "elastos/droid/os/Runnable.h"

using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentReceiver;
using Elastos::Droid::App::IPendingIntent;
using Elastos::Droid::App::IPendingIntentOnFinished;

namespace Elastos {
namespace Droid {
namespace App {

CarClass(CPendingIntentFinishedDispatcher)
    , public Runnable
    , public IIntentReceiver
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CPendingIntentFinishedDispatcher();

    ~CPendingIntentFinishedDispatcher();

    CARAPI constructor(
        /* [in] */ IPendingIntent* pi,
        /* [in] */ IPendingIntentOnFinished* who,
        /* [in] */ IHandler* handler);

    CARAPI PerformReceive(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 resultCode,
        /* [in] */ const String& data,
        /* [in] */ IBundle* extras,
        /* [in] */ Boolean ordered,
        /* [in] */ Boolean sticky,
        /* [in] */ Int32 sendingUser);

    CARAPI Run();

    CARAPI ToString(
        /* [out] */ String* str);

private:
    AutoPtr<IPendingIntent> mPendingIntent;
    AutoPtr<IPendingIntentOnFinished> mWho;
    AutoPtr<IHandler> mHandler;
    AutoPtr<IIntent> mIntent;
    Int32 mResultCode;
    String mResultData;
    AutoPtr<IBundle> mResultExtras;

};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APP_CPENDINGINTENT_FINISHDISPATCHER_H__
