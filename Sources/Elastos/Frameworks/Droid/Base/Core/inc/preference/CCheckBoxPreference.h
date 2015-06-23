
#ifndef __CCHECKBOXPREFERENCE_H__
#define __CCHECKBOXPREFERENCE_H__

#include "_CCheckBoxPreference.h"
#include "TwoStatePreference.h"

using Elastos::Droid::Preference::IPreference;
using Elastos::Droid::Widget::ICheckable;

namespace Elastos {
namespace Droid {
namespace Preference {

CarClass(CCheckBoxPreference)
    , public TwoStatePreference
    , public ICheckBoxPreference
{
public:
    CAR_INTERFACE_DECL()

    CARAPI OnBindView(
        /* [in]  */ IView* view);

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
};

}
}
}

#endif // __CCHECKBOXPREFERENCE_H__
