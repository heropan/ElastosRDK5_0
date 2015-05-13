
#ifndef __TWOSTATEPREFERENCE_H__
#define __TWOSTATEPREFERENCE_H__

#include "Preference.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::View::Accessibility::IAccessibilityManager;

namespace Elastos {
namespace Droid {
namespace Preference {

class TwoStatePreference : public Preference
{
public:
    TwoStatePreference();

    virtual ~TwoStatePreference() {}

    virtual CARAPI OnClick();

    virtual CARAPI SetChecked(
        /* [in] */ Boolean checked);

    virtual CARAPI IsChecked(
        /* [out] */ Boolean* isChecked);

    virtual CARAPI ShouldDisableDependents(
            /* [out] */ Boolean* shouldDisableDependents);

    virtual CARAPI SetSummaryOn(
        /* [in] */ ICharSequence* summary);

    virtual CARAPI SetSummaryOnEx(
        /* [in] */ Int32 summaryResId);

    virtual CARAPI GetSummaryOn(
        /* [out] */ ICharSequence** summary);

    virtual CARAPI SetSummaryOff(
        /* [in] */ ICharSequence* summary);

    virtual CARAPI SetSummaryOffEx(
        /* [in] */ Int32 summaryResId);

    virtual CARAPI GetSummaryOff(
        /* [out] */ ICharSequence** summary);

    virtual CARAPI GetDisableDependentsState(
        /* [in] */ Boolean* state);

    virtual CARAPI SetDisableDependentsState(
        /* [in] */ Boolean disableDependentsState);

    virtual CARAPI_(void) SendAccessibilityEvent(
        /* [in] */ IView* view);

    /**
     * Sync a summary view contained within view's subhierarchy with the correct summary text.
     * @param view View where a summary should be located
     */
    virtual CARAPI_(void) SyncSummaryView(
        /* [in] */ IView* view);

protected:
    CARAPI_(void) Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI_(void) Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI_(void) Init(
        /* [in] */ IContext* context);

    virtual CARAPI OnGetDefaultValue(
        /* [in] */ ITypedArray* a,
        /* [in] */ Int32 index,
        /* [out] */ IInterface** value);

    virtual CARAPI OnSetInitialValue(
        /* [in] */ Boolean restorePersistedValue,
        /* [in] */ IInterface* defaultValue);

    virtual CARAPI OnSaveInstanceState(
            /* [out] */ IParcelable** state);

    virtual CARAPI OnRestoreInstanceState(
            /* [in] */ IParcelable* state);

protected:
    Boolean mChecked;

private:
    AutoPtr<ICharSequence> mSummaryOn;
    AutoPtr<ICharSequence> mSummaryOff;
    Boolean mCheckedSet;
    Boolean mSendClickAccessibilityEvent;
    Boolean mDisableDependentsState;
};

}
}
}

#endif // __TWOSTATEPREFERENCE_H__
