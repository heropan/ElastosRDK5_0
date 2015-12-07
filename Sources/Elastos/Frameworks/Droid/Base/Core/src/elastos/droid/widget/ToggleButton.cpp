
#include "elastos/droid/widget/ToggleButton.h"

using Elastos::Droid::Graphics::Drawable::ILayerDrawable;
using Elastos::Droid::View::Accessibility::IAccessibilityRecord;
using Elastos::Core::CString;

namespace Elastos {
namespace Droid {
namespace Widget {

const Int32 ToggleButton::NO_ALPHA = 0xFF;

CAR_INTERFACE_IMPL(ToggleButton, CompoundButton, IToggleButton);
ToggleButton::ToggleButton()
    : mDisabledAlpha(0)
{}

ECode ToggleButton::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, NULL);
}

ECode ToggleButton::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(context, attrs, R::attr::buttonStyleToggle);
}

ECode ToggleButton::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr)
{
    return constructor(context, attrs, defStyleAttr, 0);
}

ECode ToggleButton::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes)
{
    CompoundButton::constructor(context, attrs, defStyleAttr, defStyleRes);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
        const_cast<Int32 *>(R::styleable::ToggleButton),
        ARRAY_SIZE(R::styleable::ToggleButton));
    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(context->ObtainStyledAttributes(
            attrs, attrIds, defStyleAttr, defStyleRes, (ITypedArray**)&a));

    a->GetText(R::styleable::ToggleButton_textOn, (ICharSequence**)&mTextOn);
    a->GetText(R::styleable::ToggleButton_textOff, (ICharSequence**)&mTextOff);
    a->GetFloat(R::styleable::ToggleButton_disabledAlpha, 0.5f, &mDisabledAlpha);
    SyncTextState();
    a->Recycle();
    return NOERROR;
}

ECode ToggleButton::SetChecked(
    /* [in] */ Boolean checked)
{
    CompoundButton::SetChecked(checked);
    SyncTextState();

    return NOERROR;
}

void ToggleButton::SyncTextState()
{
    Boolean checked = IsChecked();
    if (checked && mTextOn != NULL) {
        SetText(mTextOn);
    }
    else if (!checked && mTextOff != NULL) {
        SetText(mTextOff);
    }
}

ECode ToggleButton::GetTextOn(
    /* [out] */ ICharSequence** on)
{
    VALIDATE_NOT_NULL(on);
    *on = mTextOn;
    REFCOUNT_ADD(*on);
    return NOERROR;
}

ECode ToggleButton::SetTextOn(
    /* [in] */ ICharSequence* textOn)
{
    mTextOn = textOn;

    return NOERROR;
}

ECode ToggleButton::GetTextOff(
    /* [out] */ ICharSequence** off)
{
    VALIDATE_NOT_NULL(off);
    *off = mTextOff;
    REFCOUNT_ADD(*off);
    return NOERROR;
}

ECode ToggleButton::SetTextOff(
    /* [in] */ ICharSequence* textOff)
{
    mTextOff = textOff;

    return NOERROR;
}

ECode ToggleButton::OnFinishInflate()
{
    CompoundButton::OnFinishInflate();

    AutoPtr<IDrawable> drawable;
    GetBackground((IDrawable**)&drawable);
    UpdateReferenceToIndicatorDrawable(drawable);

    return NOERROR;
}

ECode ToggleButton::SetBackgroundDrawable(
    /* [in] */ IDrawable* d)
{
    CompoundButton::SetBackgroundDrawable(d);

    UpdateReferenceToIndicatorDrawable(d);

    return NOERROR;
}

void ToggleButton::UpdateReferenceToIndicatorDrawable(
    /* [in] */ IDrawable* backgroundDrawable)
{
    AutoPtr<ILayerDrawable> layerDrawable = ILayerDrawable::Probe(backgroundDrawable);
    if (layerDrawable) {
        layerDrawable->FindDrawableByLayerId(
            R::id::toggle, (IDrawable**)&mIndicatorDrawable);
    }
    else {
        mIndicatorDrawable = NULL;
    }
}

ECode ToggleButton::DrawableStateChanged()
{
    CompoundButton::DrawableStateChanged();

    if (mIndicatorDrawable != NULL) {
        Boolean enabled = FALSE;
        IsEnabled(&enabled);
        mIndicatorDrawable->SetAlpha(enabled ? NO_ALPHA : (Int32)(NO_ALPHA * mDisabledAlpha));
    }

    return NOERROR;
}

ECode ToggleButton::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    CompoundButton::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> s;
    CString::New(String("CToggleButton"), (ICharSequence**)&s);
    IAccessibilityRecord::Probe(event)->SetClassName(s);
    return NOERROR;
}

ECode ToggleButton::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    CompoundButton::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> s;
    CString::New(String("CToggleButton"), (ICharSequence**)&s);
    info->SetClassName(s);
    return NOERROR;
}


} // namespace Widget
} // namespace Droid
} // namespace Elastos
