#ifndef __CLOCKPATTERNVIEWSAVEDSTATE__H_
#define __CLOCKPATTERNVIEWSAVEDSTATE__H_

#include "_CLockPatternViewSavedState.h"
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

#endif //__CLOCKPATTERNVIEWSAVEDSTATE__H_