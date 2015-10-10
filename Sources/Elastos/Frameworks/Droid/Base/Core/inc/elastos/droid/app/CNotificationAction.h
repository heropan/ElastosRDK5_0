#ifndef __ELASTOS_DROID_APP_CNOTIFICATIONACTION_H__
#define __ELASTOS_DROID_APP_CNOTIFICATIONACTION_H__

#include "_Elastos_Droid_App_CNotificationAction.h"
#include "elastos/droid/ext/frameworkext.h"

using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace App {

CarClass(CNotificationAction)
{
public:
    friend class CNotificationBuilder;

    CNotificationAction();

    ~CNotificationAction();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 icon_,
        /* [in] */ ICharSequence* title_,
        /* [in] */ IPendingIntent* intent_);

    CARAPI Clone(
        /* [out] */ INotificationAction** action);

    CARAPI GetIcon(
        /* [out] */ Int32* icon);

    CARAPI GetTitle(
        /* [out] */ ICharSequence** title);

    CARAPI GetActionIntent(
        /* [out] */ IPendingIntent** actionIntent);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* out);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* in);

public:
    Int32 mIcon;
    AutoPtr<ICharSequence> mTitle;
    AutoPtr<IPendingIntent> mActionIntent;
};

}
}
}

#endif // __ELASTOS_DROID_APP_CNOTIFICATIONACTION_H__
