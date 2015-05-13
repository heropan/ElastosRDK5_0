
#include "CCheckBoxPreference.h"
#include "R.h"

using Elastos::Core::CStringWrapper;

namespace Elastos {
namespace Droid {
namespace Preference {

PInterface CCheckBoxPreference::Probe(
    /* [in] */REIID riid)
{
    if (riid == EIID_ICheckBoxPreference) {
        return (ICheckBoxPreference*)this;
    }
    else if (riid == EIID_ITwoStatePreference) {
        return (ITwoStatePreference*)this;
    }

    return Preference::Probe(riid);
}

UInt32 CCheckBoxPreference::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCheckBoxPreference::Release()
{
    return ElRefBase::Release();
}

ECode CCheckBoxPreference::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface *)(ICheckBoxPreference*)this) {
        *pIID = EIID_ICheckBoxPreference;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(ITwoStatePreference*)this) {
        *pIID = EIID_ITwoStatePreference;
        return NOERROR;
    }
    return Preference::GetInterfaceID(pObject, pIID);
}

ECode CCheckBoxPreference::OnBindView(
    /* [in] */ IView* view)
{
    TwoStatePreference::OnBindView(view);

    AutoPtr<IView> checkboxView;
    view->FindViewById(R::id::checkbox, (IView**)&checkboxView);
    if (checkboxView != NULL && ICheckable::Probe(checkboxView) != NULL) {
        AutoPtr<ICheckable> checkable = ICheckable::Probe(checkboxView);
        checkable->SetChecked(mChecked);
        SendAccessibilityEvent(checkboxView);
    }

    SyncSummaryView(view);
    return NOERROR;
}

ECode CCheckBoxPreference::SetChecked(
    /* [in] */ Boolean checked)
{
    return TwoStatePreference::SetChecked(checked);
}

ECode CCheckBoxPreference::IsChecked(
    /* [out] */ Boolean* isChecked)
{
    return TwoStatePreference::IsChecked(isChecked);
}

ECode CCheckBoxPreference::SetSummaryOn(
    /* [in] */ ICharSequence* summary)
{
    return TwoStatePreference::SetSummaryOn(summary);
}

ECode CCheckBoxPreference::SetSummaryOnEx(
    /* [in] */ Int32 summaryResId)
{
    return TwoStatePreference::SetSummaryOnEx(summaryResId);
}

ECode CCheckBoxPreference::GetSummaryOn(
    /* [out] */ ICharSequence** summary)
{
    return TwoStatePreference::GetSummaryOn(summary);
}

ECode CCheckBoxPreference::SetSummaryOff(
    /* [in] */ ICharSequence* summary)
{
    return TwoStatePreference::SetSummaryOff(summary);
}

ECode CCheckBoxPreference::SetSummaryOffEx(
    /* [in] */ Int32 summaryResId)
{
    return TwoStatePreference::SetSummaryOffEx(summaryResId);
}

ECode CCheckBoxPreference::GetSummaryOff(
    /* [out] */ ICharSequence** summary)
{
    return TwoStatePreference::GetSummaryOff(summary);
}

ECode CCheckBoxPreference::GetDisableDependentsState(
    /* [out] */ Boolean* state)
{
    return TwoStatePreference::GetDisableDependentsState(state);
}

ECode CCheckBoxPreference::SetDisableDependentsState(
    /* [in] */ Boolean disableDependentsState)
{
    return TwoStatePreference::SetDisableDependentsState(disableDependentsState);
}

ECode CCheckBoxPreference::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    TwoStatePreference::Init(context, attrs, defStyle);

    AutoPtr< ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::CheckBoxPreference),
            ARRAY_SIZE(R::styleable::CheckBoxPreference));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx3(attrs, attrIds, defStyle, 0, (ITypedArray**)&a);

    String summaryOn;
    a->GetString(R::styleable::CheckBoxPreference_summaryOn, &summaryOn);
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(summaryOn, (ICharSequence**)&cs);
    TwoStatePreference::SetSummaryOn(cs);

    String summaryOff;
    a->GetString(R::styleable::CheckBoxPreference_summaryOff, &summaryOff);
    cs = NULL;
    CStringWrapper::New(summaryOff, (ICharSequence**)&cs);
    TwoStatePreference::SetSummaryOff(cs);

    Boolean disableDependentsState;
    a->GetBoolean(R::styleable::CheckBoxPreference_disableDependentsState, FALSE, &disableDependentsState);

    SetDisableDependentsState(disableDependentsState);
    a->Recycle();

    return NOERROR;

}

ECode CCheckBoxPreference::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(context, attrs, R::attr::checkBoxPreferenceStyle);
}

ECode CCheckBoxPreference::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, NULL);
}

}
}
}

