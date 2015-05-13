
#include "widget/ToggleButton.h"

using Elastos::Core::CStringWrapper;
using Elastos::Droid::Graphics::Drawable::ILayerDrawable;

namespace Elastos {
namespace Droid {
namespace Widget {

const Int32 ToggleButton::NO_ALPHA;

ToggleButton::ToggleButton()
    : mDisabledAlpha(0)
{}

ToggleButton::ToggleButton(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : CompoundButton(context, attrs, defStyle)
    , mDisabledAlpha(0)
{
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs, defStyle));
}

ECode ToggleButton::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
        const_cast<Int32 *>(R::styleable::ToggleButton),
        ARRAY_SIZE(R::styleable::ToggleButton));
    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(context->ObtainStyledAttributesEx3(
            attrs, attrIds, defStyle, 0, (ITypedArray**)&a));

    a->GetText(R::styleable::ToggleButton_textOn, (ICharSequence**)&mTextOn);
    a->GetText(R::styleable::ToggleButton_textOff, (ICharSequence**)&mTextOff);
    a->GetFloat(R::styleable::ToggleButton_disabledAlpha, 0.5f, &mDisabledAlpha);
    SyncTextState();
    a->Recycle();
    return NOERROR;
}

ECode ToggleButton::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    FAIL_RETURN(CompoundButton::Init(context, attrs, defStyle));
    return InitFromAttributes(context, attrs, defStyle);
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

/**
 * Returns the text for when the button is in the checked state.
 *
 * @return The text.
 */
AutoPtr<ICharSequence> ToggleButton::GetTextOn()
{
    return mTextOn;
}

/**
 * Sets the text for when the button is in the checked state.
 *
 * @param textOn The text.
 */
ECode ToggleButton::SetTextOn(
    /* [in] */ ICharSequence* textOn)
{
    mTextOn = textOn;

    return NOERROR;
}

/**
 * Returns the text for when the button is not in the checked state.
 *
 * @return The text.
 */
AutoPtr<ICharSequence> ToggleButton::GetTextOff()
{
    return mTextOff;
}

/**
 * Sets the text for when the button is not in the checked state.
 *
 * @param textOff The text.
 */
ECode ToggleButton::SetTextOff(
    /* [in] */ ICharSequence* textOff)
{
    mTextOff = textOff;

    return NOERROR;
}

ECode ToggleButton::OnFinishInflate()
{
    CompoundButton::OnFinishInflate();

    UpdateReferenceToIndicatorDrawable(GetBackground());

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
        mIndicatorDrawable->SetAlpha(IsEnabled() ? NO_ALPHA : (Int32)(NO_ALPHA * mDisabledAlpha));
    }

    return NOERROR;
}

ECode ToggleButton::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    CompoundButton::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> s;
    CStringWrapper::New(String("CToggleButton"), (ICharSequence**)&s);
    event->SetClassName(s);
    return NOERROR;
}

ECode ToggleButton::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    CompoundButton::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> s;
    CStringWrapper::New(String("CToggleButton"), (ICharSequence**)&s);
    info->SetClassName(s);
    return NOERROR;
}


} // namespace Widget
} // namespace Droid
} // namespace Elastos
