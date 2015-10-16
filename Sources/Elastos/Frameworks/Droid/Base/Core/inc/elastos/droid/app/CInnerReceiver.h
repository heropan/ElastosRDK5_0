#ifndef __ELASTOS_DROID_APP_CINNERRECEIVER_H__
#define __ELASTOS_DROID_APP_CINNERRECEIVER_H__

#include "_Elastos_Droid_App_CInnerReceiver.h"
#include "elastos/droid/app/LoadedPkg.h"

using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace App {

CarClass(CInnerReceiver)
{
public:
    CARAPI constructor(
        /* [in] */ Handle32 rd,
        /* [in] */ Boolean strong);

    CARAPI PerformReceive(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 resultCode,
        /* [in] */ const String& data,
        /* [in] */ IBundle* extras,
        /* [in] */ Boolean ordered,
        /* [in] */ Boolean sticky,
        /* [in] */ Int32 sendingUser);

    CARAPI ToString(
        /* [out] */ String* str);

private:
    AutoPtr<IWeakReference> mDispatcher; // LoadedPkg::ReceiverDispatcher
    AutoPtr<LoadedPkg::ReceiverDispatcher> mStrongRefRef;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_APP_CINNERRECEIVER_H__
