#ifndef __ELASTOS_DROID_WIDGET_INTERNAL_CLOCKPATTERNVIEWSAVEDSTATE_H__
#define __ELASTOS_DROID_WIDGET_INTERNAL_CLOCKPATTERNVIEWSAVEDSTATE_H__

#include "_Elastos_Droid_Widget_Internal_CLockPatternViewSavedState.h"
#include "widget/internal/LockPatternViewSavedState.h"

namespace Elastos{
namespace Droid{
namespace Widget{
namespace Internal{

CarClass(CLockPatternViewSavedState), public LockPatternViewSavedState
{
public:
    CARAPI constructor(
        /* [in] */ IParcel* in);

    CARAPI constructor(
        /* [in] */ IParcelable* superState,
        /* [in] */ const String& serializedPattern,
        /* [in] */ Int32 displayMode,
        /* [in] */ Boolean inputEnabled,
        /* [in] */ Boolean inStealthMode,
        /* [in] */ Boolean tactileFeedbackEnabled);

    CARAPI GetSerializedPattern(
        /* [out] */ String* result);

    CARAPI GetDisplayMode(
        /* [out] */ Int32* result);

    CARAPI IsInputEnabled(
        /* [out] */ Boolean* result);

    CARAPI IsInStealthMode(
        /* [out] */ Boolean* result);

    CARAPI IsTactileFeedbackEnabled(
        /* [out] */ Boolean* result);

    CARAPI GetSuperState(
        /* [out] */ IParcelable** superState);
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_INTERNAL_CLOCKPATTERNVIEWSAVEDSTATE_H__
