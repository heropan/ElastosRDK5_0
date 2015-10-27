
#ifndef __ELASTOS_DROID_APP_CPENDINGINTENT_H__
#define __ELASTOS_DROID_APP_CPENDINGINTENT_H__

#include "elastos/droid/ext/frameworkext.h"
#include "_Elastos_Droid_App_CPendingIntent.h"

using Elastos::Core::IRunnable;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentSender;
using Elastos::Droid::Content::IIIntentSender;
using Elastos::Droid::Content::IIntentReceiver;

namespace Elastos {
namespace Droid {
namespace App {

CarClass(CPendingIntent)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IIIntentSender* target);

    CARAPI constructor(
        /* [in] */ IBinder* target);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI GetIntentSender(
        /* [out] */ IIntentSender** intentSender);

    CARAPI Cancel();

    CARAPI Send();

    CARAPI Send(
        /* [in] */ Int32 code);

    CARAPI Send(
        /* [in] */ IContext* context,
        /* [in] */ Int32 code,
        /* [in] */ IIntent* intent);

    CARAPI Send(
        /* [in] */ Int32 code,
        /* [in] */ IPendingIntentOnFinished* onFinished,
        /* [in] */ IHandler* handler);

    CARAPI Send(
        /* [in] */ IContext* context,
        /* [in] */ Int32 code,
        /* [in] */ IIntent* intent,
        /* [in] */ IPendingIntentOnFinished* onFinished,
        /* [in] */ IHandler* handler);

    CARAPI Send(
        /* [in] */ IContext* context,
        /* [in] */ Int32 code,
        /* [in] */ IIntent* intent,
        /* [in] */ IPendingIntentOnFinished* onFinished,
        /* [in] */ IHandler* handler,
        /* [in] */ const String& requiredPermission);

    CARAPI GetTargetPackage(
        /* [out] */ String* targetPackage);

    CARAPI GetCreatorPackage(
        /* [out] */ String* targetPackage);

    CARAPI GetCreatorUid(
        /* [out] */ Int32* uid);

    CARAPI GetCreatorUserHandle(
        /* [out] */ IUserHandle** userHandle);

    CARAPI IsTargetedToPackage(
        /* [out] */ Boolean* result);

    CARAPI IsActivity(
        /* [out] */ Boolean* result);

    CARAPI GetIntent(
        /* [out] */ IIntent** intent);

    CARAPI GetTag(
        /* [in] */ const String& prefix,
        /* [out] */ String* tag);

    CARAPI GetTarget(
        /* [out] */ IIIntentSender** target);

    CARAPI GetHashCode(
        /* [out] */ Int32* jash);

    CARAPI Equals(
        /* [in] */ IInterface* o,
        /* [out] */ Boolean* rst);

    CARAPI ToString(
        /* [out] */ String* info);

private:
    AutoPtr<IIIntentSender> mTarget;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APP_CPENDINGINTENT_H__
