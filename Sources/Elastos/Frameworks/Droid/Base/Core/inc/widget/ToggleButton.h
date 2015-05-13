
#ifndef __RADIOBUTTON_H__
#define __RADIOBUTTON_H__

#include "widget/CompoundButton.h"

using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::View::Accessibility::IAccessibilityNodeInfo;

namespace Elastos {
namespace Droid {
namespace Widget {


class ToggleButton : public CompoundButton
{
public:
    ToggleButton(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = R::attr::buttonStyleToggle);

    //@Override
    virtual CARAPI SetChecked(
        /* [in] */ Boolean checked);

    virtual CARAPI_(AutoPtr<ICharSequence>) GetTextOn();

    virtual CARAPI SetTextOn(
        /* [in] */ ICharSequence* textOn);

    virtual CARAPI_(AutoPtr<ICharSequence>) GetTextOff();

    virtual CARAPI SetTextOff(
        /* [in] */ ICharSequence* textOff);

    //@Override
    virtual CARAPI SetBackgroundDrawable(
        /* [in] */ IDrawable* d);

    virtual CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

protected:
    ToggleButton();

    //@Override
    virtual CARAPI OnFinishInflate();

    //@Override
    virtual CARAPI DrawableStateChanged();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0x0101004b/*com.android.internal.R.attr.buttonStyleToggle*/);

private:
    CARAPI_(void) UpdateReferenceToIndicatorDrawable(
        /* [in] */ IDrawable* backgroundDrawable);

    CARAPI_(void) SyncTextState();

    CARAPI InitFromAttributes(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

private:
    AutoPtr<ICharSequence> mTextOn;
    AutoPtr<ICharSequence> mTextOff;

    AutoPtr<IDrawable> mIndicatorDrawable;

    static const Int32 NO_ALPHA = 0xFF;
    Float mDisabledAlpha;
};


} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__BUTTON_H__
