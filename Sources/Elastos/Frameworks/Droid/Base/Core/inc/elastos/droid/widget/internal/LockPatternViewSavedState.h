
#ifndef __ELASTOS_DROID_WIDGET_INTERNAL_LOCKPATTERNVIEWSAVEDSTATE_H__
#define __ELASTOS_DROID_WIDGET_INTERNAL_LOCKPATTERNVIEWSAVEDSTATE_H__

#include "elastos/droid/view/ViewBaseSavedState.h"

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

class LockPatternViewSavedState : public Elastos::Droid::View::ViewBaseSavedState
{

public:
    virtual CARAPI_(String) GetSerializedPattern();

    virtual CARAPI_(Int32) GetDisplayMode();

    virtual CARAPI_(Boolean) IsInputEnabled();

    virtual CARAPI_(Boolean) IsInStealthMode();

    virtual CARAPI_(Boolean) IsTactileFeedbackEnabled();


    virtual CARAPI WriteToParcel(
        /* [in] */ IParcel* dest,
        /* [in] */ Int32 flags);

protected:
    LockPatternViewSavedState();

    CARAPI Init(
        /* [in] */ IParcelable* superState,
        /* [in] */ const String& serializedPattern,
        /* [in] */ Int32 displayMode,
        /* [in] */ Boolean inputEnabled,
        /* [in] */ Boolean inStealthMode,
        /* [in] */ Boolean tactileFeedbackEnabled);

    CARAPI Init(
        /* [in] */ IParcel* in);

private:
    LockPatternViewSavedState(
        /* [in] */ IParcelable* superState,
        /* [in] */ const String& serializedPattern,
        /* [in] */ Int32 displayMode,
        /* [in] */ Boolean inputEnabled,
        /* [in] */ Boolean inStealthMode,
        /* [in] */ Boolean tactileFeedbackEnabled);

    LockPatternViewSavedState(
        /* [in] */ IParcel* in);

private:
    String mSerializedPattern;
    Int32 mDisplayMode;
    Boolean mInputEnabled;
    Boolean mInStealthMode;
    Boolean mTactileFeedbackEnabled;

};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_INTERNAL_LOCKPATTERNVIEWSAVEDSTATE_H__
