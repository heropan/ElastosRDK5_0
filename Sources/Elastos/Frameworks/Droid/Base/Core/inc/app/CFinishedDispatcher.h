#ifndef __CFINISHDISPATCHERH__
#define __CFINISHDISPATCHERH__

#include "ext/frameworkext.h"
#include "_CFinishedDispatcher.h"

using Elastos::Core::IRunnable;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::App::IPendingIntent;
using Elastos::Droid::App::IPendingIntentOnFinished;

namespace Elastos {
namespace Droid {
namespace App {

CarClass(CFinishedDispatcher)
{
public:
    CARAPI constructor(
        /* [in] */ IPendingIntent* pi,
        /* [in] */ IPendingIntentOnFinished* who,
        /* [in] */ IHandler* handler);

    CFinishedDispatcher();

    ~CFinishedDispatcher();

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

#endif //__CFINISHDISPATCHERH__