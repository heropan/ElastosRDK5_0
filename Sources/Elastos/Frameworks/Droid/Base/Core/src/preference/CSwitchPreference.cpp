
#include "CSwitchPreference.h"
#include "R.h"

using Elastos::Core::IBoolean;
using Elastos::Core::CBoolean;
using Elastos::Core::CString;
using Elastos::Droid::Widget::EIID_ICompoundButtonOnCheckedChangeListener;
using Elastos::Droid::Widget::ICheckable;
using Elastos::Droid::Widget::ISwitch;

namespace Elastos {
namespace Droid {
namespace Preference {

/////////////////////////////////////////////////////
// CSwitchPreference::Listener
/////////////////////////////////////////////////////

CAR_INTERFACE_IMPL(CSwitchPreference::Listener, ICompoundButtonOnCheckedChangeListener)

CSwitchPreference::Listener::Listener(
    /* [in] */ CSwitchPreference* host)
    : mHost(host)
{}

ECode CSwitchPreference::Listener::OnCheckedChanged(
    /* [in] */ ICompoundButton* buttonView,
    /* [in] */ Boolean isChecked)
{
    AutoPtr<IBoolean> b;
    CBoolean::New(isChecked, (IBoolean**)&b);
    Boolean result;
    if (mHost->CallChangeListener(b, &result), !result) {
        // Listener didn't like it, change it back.
        // CompoundButton will make sure we don't recurse.
        ICheckable::Probe(buttonView)->SetChecked(!isChecked);
        return NOERROR;
    }

    mHost->SetChecked(isChecked);
    return NOERROR;
}


/////////////////////////////////////////////////////
// CSwitchPreference
/////////////////////////////////////////////////////

CSwitchPreference::CSwitchPreference()
{
    mListener = new Listener(this);
}

PInterface CSwitchPreference::Probe(
    /* [in] */REIID riid)
{
    if (riid == EIID_ISwitchPreference) {
        return (ISwitchPreference*)this;
    }
    else if (riid == EIID_ITwoStatePreference) {
        return (ITwoStatePreference*)this;
    }

    return TwoStatePreference::Probe(riid);
}

UInt32 CSwitchPreference::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CSwitchPreference::Release()
{
    return ElRefBase::Release();
}

ECode CSwitchPreference::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface *)(ISwitchPreference*)this) {
        *pIID = EIID_ISwitchPreference;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(ITwoStatePreference*)this) {
        *pIID = EIID_ITwoStatePreference;
        return NOERROR;
    }
    return TwoStatePreference::GetInterfaceID(pObject, pIID);
}

ECode CSwitchPreference::OnBindView(
    /* [in] */ IView* view)
{
    FAIL_RETURN(TwoStatePreference::OnBindView(view))

    AutoPtr<IView> checkableView;
    view->FindViewById(R::id::switchWidget, (IView**)&checkableView);
    if (checkableView != NULL && ICheckable::Probe(checkableView) != NULL) {
        AutoPtr<ICheckable> checkable = ICheckable::Probe(checkableView);
        checkable->SetChecked(mChecked);

        SendAccessibilityEvent(checkableView);

        if (ISwitch::Probe(checkableView) != NULL) {
            AutoPtr<ISwitch> switchView = ISwitch::Probe(checkableView);
            switchView->SetTextOn(mSwitchOn);
            switchView->SetTextOff(mSwitchOff);
            switchView->SetOnCheckedChangeListener(mListener);
        }
    }

    SyncSummaryView(view);
    return NOERROR;
}

ECode CSwitchPreference::SetSwitchTextOn(
    /* [in] */ ICharSequence* onText)
{
    mSwitchOn = onText;
    return NotifyChanged();
}

ECode CSwitchPreference::SetSwitchTextOff(
    /* [in] */ ICharSequence* offText)
{
    mSwitchOff = offText;
    return NotifyChanged();
}

ECode CSwitchPreference::SetSwitchTextOn(
    /* [in] */ Int32 resId)
{
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    String textOnStr;
    context->GetString(resId, &textOnStr);
    AutoPtr<ICharSequence> textOn;
    CString::New(textOnStr, (ICharSequence**)&textOn);
    return SetSwitchTextOn(textOn);
}

ECode CSwitchPreference::SetSwitchTextOff(
    /* [in] */ Int32 resId)
{
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    String textOffStr;
    context->GetString(resId, &textOffStr);
    AutoPtr<ICharSequence> textOff;
    CString::New(textOffStr, (ICharSequence**)&textOff);
    return SetSwitchTextOn(textOff);
}

ECode CSwitchPreference::GetSwitchTextOn(
    /* [out] */ ICharSequence** textOn)
{
    VALIDATE_NOT_NULL(textOn)
    *textOn = mSwitchOn;
    REFCOUNT_ADD(*textOn)
    return NOERROR;
}

ECode CSwitchPreference::GetSwitchTextOff(
    /* [out] */ ICharSequence** textOff)
{
    VALIDATE_NOT_NULL(textOff)
    *textOff = mSwitchOff;
    REFCOUNT_ADD(*textOff)
    return NOERROR;
}

ECode CSwitchPreference::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    TwoStatePreference::Init(context, attrs, defStyle);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::SwitchPreference),
            ARRAY_SIZE(R::styleable::SwitchPreference));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds, defStyle, 0, (ITypedArray**)&a);
    String summaryOn;
    a->GetString(R::styleable::SwitchPreference_summaryOn, &summaryOn);
    AutoPtr<ICharSequence> summaryOnCs;
    CString::New(summaryOn, (ICharSequence**)&summaryOnCs);
    SetSummaryOn(summaryOnCs);

    String summaryOff;
    a->GetString(R::styleable::SwitchPreference_summaryOff, &summaryOff);
    AutoPtr<ICharSequence> summaryOffCs;
    CString::New(summaryOff, (ICharSequence**)&summaryOffCs);
    SetSummaryOff(summaryOffCs);

    String switchTextOn;
    a->GetString(R::styleable::SwitchPreference_switchTextOn, &switchTextOn);
    AutoPtr<ICharSequence> textOn;
    CString::New(switchTextOn, (ICharSequence**)&textOn);
    SetSwitchTextOn(textOn);

    String switchTextOff;
    a->GetString(R::styleable::SwitchPreference_switchTextOff, &switchTextOff);
    AutoPtr<ICharSequence> textOff;
    CString::New(switchTextOff, (ICharSequence**)&textOff);
    SetSwitchTextOff(textOff);

    Boolean disable;
    a->GetBoolean(R::styleable::SwitchPreference_disableDependentsState, FALSE, &disable);
    SetDisableDependentsState(disable);
    a->Recycle();

    return NOERROR;
}

ECode CSwitchPreference::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(context, attrs, R::attr::switchPreferenceStyle);
}

ECode CSwitchPreference::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, NULL);
}

ECode CSwitchPreference::SetChecked(
    /* [in] */ Boolean checked)
{
    return TwoStatePreference::SetChecked(checked);
}

ECode CSwitchPreference::IsChecked(
    /* [out] */ Boolean* isChecked)
{
    return TwoStatePreference::IsChecked(isChecked);
}

ECode CSwitchPreference::SetSummaryOn(
    /* [in] */ ICharSequence* summary)
{
    return TwoStatePreference::SetSummaryOn(summary);
}

ECode CSwitchPreference::SetSummaryOn(
    /* [in] */ Int32 summaryResId)
{
    return TwoStatePreference::SetSummaryOn(summaryResId);
}

ECode CSwitchPreference::GetSummaryOn(
    /* [out] */ ICharSequence** summary)
{
    return TwoStatePreference::GetSummaryOn(summary);
}

ECode CSwitchPreference::SetSummaryOff(
    /* [in] */ ICharSequence* summary)
{
    return TwoStatePreference::SetSummaryOff(summary);
}

ECode CSwitchPreference::SetSummaryOff(
    /* [in] */ Int32 summaryResId)
{
    return TwoStatePreference::SetSummaryOff(summaryResId);
}

ECode CSwitchPreference::GetSummaryOff(
    /* [out] */ ICharSequence** summary)
{
    return TwoStatePreference::GetSummaryOff(summary);
}

ECode CSwitchPreference::GetDisableDependentsState(
    /* [out] */ Boolean* state)
{
    return TwoStatePreference::GetDisableDependentsState(state);
}

ECode CSwitchPreference::SetDisableDependentsState(
    /* [in] */ Boolean disableDependentsState)
{
    return TwoStatePreference::SetDisableDependentsState(disableDependentsState);
}

}
}
}
