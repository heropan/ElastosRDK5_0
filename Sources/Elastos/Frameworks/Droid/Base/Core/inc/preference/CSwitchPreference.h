
#ifndef __CSWITCHPREFERENCE_H__
#define __CSWITCHPREFERENCE_H__

#include "_CSwitchPreference.h"
#include "TwoStatePreference.h"

using Elastos::Droid::Widget::ICompoundButton;
using Elastos::Droid::Widget::ICompoundButtonOnCheckedChangeListener;

namespace Elastos {
namespace Droid {
namespace Preference {

CarClass(CSwitchPreference)
    , public TwoStatePreference
    , public ISwitchPreference
{
public:
    class Listener
        : public ElRefBase
        , public ICompoundButtonOnCheckedChangeListener
    {
    public:
        Listener(
            /* [in] */ CSwitchPreference* host);

        CAR_INTERFACE_DECL()

        ECode OnCheckedChanged(
            /* [in] */ ICompoundButton* buttonView,
            /* [in] */ Boolean isChecked);

     private:
        CSwitchPreference* mHost;
     };

public:
    CSwitchPreference();

    CAR_INTERFACE_DECL()

    CARAPI SetSwitchTextOn(
        /* [in] */ ICharSequence* onText);

    CARAPI SetSwitchTextOff(
        /* [in] */ ICharSequence* offText);

    CARAPI SetSwitchTextOn(
        /* [in] */ Int32 resId);

    CARAPI SetSwitchTextOff(
        /* [in] */ Int32 resId);

    CARAPI GetSwitchTextOn(
        /* [out] */ ICharSequence** textOn);

    CARAPI GetSwitchTextOff(
        /* [out] */ ICharSequence** textOff);

    CARAPI SetChecked(
        /* [in] */ Boolean checked);

    CARAPI IsChecked(
        /* [out] */ Boolean* isChecked);

    CARAPI SetSummaryOn(
        /* [in] */ ICharSequence* summary);

    CARAPI SetSummaryOn(
        /* [in] */ Int32 summaryResId);

    CARAPI GetSummaryOn(
        /* [out] */ ICharSequence** summary);

    CARAPI SetSummaryOff(
        /* [in] */ ICharSequence* summary);

    CARAPI SetSummaryOff(
        /* [in] */ Int32 summaryResId);

    CARAPI GetSummaryOff(
        /* [out] */ ICharSequence** summary);

    CARAPI GetDisableDependentsState(
        /* [out] */ Boolean* state);

    CARAPI SetDisableDependentsState(
        /* [in] */ Boolean disableDependentsState);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context);

protected:
    CARAPI OnBindView(
        /* [in] */ IView* view);

private:
    // Switch text for on and off states
    AutoPtr<ICharSequence> mSwitchOn;
    AutoPtr<ICharSequence> mSwitchOff;
    AutoPtr<ICompoundButtonOnCheckedChangeListener> mListener;

};

}
}
}

#endif // __CSWITCHPREFERENCE_H__
