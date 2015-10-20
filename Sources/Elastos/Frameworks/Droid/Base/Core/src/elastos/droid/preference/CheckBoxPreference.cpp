
#include "elastos/droid/preference/CheckBoxPreference.h"
#include "elastos/droid/R.h"

using Elastos::Core::CString;
using Elastos::Droid::R;

namespace Elastos {
namespace Droid {
namespace Preference {
CAR_INTERFACE_IMPL(CheckBoxPreference, TwoStatePreference, ICheckBoxPreference)

ECode CheckBoxPreference::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr)
{
    return constructor(context, attrs, defStyleAttr, 0);
}

ECode CheckBoxPreference::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes)
{
    TwoStatePreference::Init(context, attrs, defStyleAttr, 0);

    AutoPtr< ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::CheckBoxPreference),
            ARRAY_SIZE(R::styleable::CheckBoxPreference));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds, defStyleAttr, defStyleRes, (ITypedArray**)&a);

    String summaryOn;
    a->GetString(R::styleable::CheckBoxPreference_summaryOn, &summaryOn);
    AutoPtr<ICharSequence> cs;
    CString::New(summaryOn, (ICharSequence**)&cs);
    TwoStatePreference::SetSummaryOn(cs);

    String summaryOff;
    a->GetString(R::styleable::CheckBoxPreference_summaryOff, &summaryOff);
    cs = NULL;
    CString::New(summaryOff, (ICharSequence**)&cs);
    TwoStatePreference::SetSummaryOff(cs);

    Boolean disableDependentsState;
    a->GetBoolean(R::styleable::CheckBoxPreference_disableDependentsState, FALSE, &disableDependentsState);

    SetDisableDependentsState(disableDependentsState);
    a->Recycle();

    return NOERROR;
}

ECode CheckBoxPreference::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(context, attrs, R::attr::checkBoxPreferenceStyle);
}

ECode CheckBoxPreference::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, NULL);
}

ECode CheckBoxPreference::OnBindView(
    /* [in] */ IView* view)
{
    TwoStatePreference::OnBindView(view);

    AutoPtr<IView> checkboxView;
    view->FindViewById(R::id::checkbox, (IView**)&checkboxView);
    if (checkboxView != NULL && ICheckable::Probe(checkboxView) != NULL) {
        AutoPtr<ICheckable> checkable = ICheckable::Probe(checkboxView);
        checkable->SetChecked(mChecked);
    }

    SyncSummaryView(view);
    return NOERROR;
}

}
}
}

