#include "elastos/droid/widget/CToggleButton.h"

using Elastos::Droid::View::EIID_View;

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEW_METHODS_IMPL(CToggleButton, ToggleButton);
IDRAWABLECALLBACK_METHODS_IMPL(CToggleButton, ToggleButton);
IKEYEVENTCALLBACK_METHODS_IMPL(CToggleButton, ToggleButton);
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CToggleButton, ToggleButton);
ITEXTVIEW_METHODS_IMPL(CToggleButton, ToggleButton);
ICOMPOUNDBUTTON_METHODS_IMPL(CToggleButton, ToggleButton);
ICHECKABLE_METHODS_IMPL(CToggleButton, ToggleButton);

PInterface CToggleButton::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_TextView) {
        return reinterpret_cast<PInterface>((TextView*)this);
    }
    return _CToggleButton::Probe(riid);
}

ECode CToggleButton::constructor(
    /* [in] */ IContext * pCtx)
{
    return ToggleButton::Init(pCtx);
}

ECode CToggleButton::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs)
{
    return ToggleButton::Init(pCtx, pAttrs);
}

ECode CToggleButton::constructor(
    /* [in] */ IContext * pContext,
    /* [in] */ IAttributeSet * pAttrs,
    /* [in] */ Int32 defStyle)
{
    return ToggleButton::Init(pContext, pAttrs, defStyle);
}

/**
 * Callback method to be invoked when the view tree is about to be drawn. At this point, all
 * views in the tree have been measured and given a frame. Clients can use this to adjust
 * their scroll bounds or even to request a new layout before drawing occurs.
 *
 * @return Return true to proceed with the current drawing pass, or false to cancel.
 *
 * @see android.view.View#onMeasure
 * @see android.view.View#onLayout
 * @see android.view.View#onDraw
 */
ECode CToggleButton::OnPreDraw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = ToggleButton::OnPreDraw();

    return NOERROR;
}

/**
 * Returns the text for when the button is in the checked state.
 *
 * @return The text.
 */
ECode CToggleButton::GetTextOn(
    /* [out] */ ICharSequence** on)
{
    VALIDATE_NOT_NULL(on);
    AutoPtr<ICharSequence> csq = ToggleButton::GetTextOn();
    *on = csq;
    REFCOUNT_ADD(*on);
    return NOERROR;
}

/**
 * Sets the text for when the button is in the checked state.
 *
 * @param textOn The text.
 */
ECode CToggleButton::SetTextOn(
    /* [in] */ ICharSequence* textOn)
{
    return ToggleButton::SetTextOn(textOn);
}

/**
 * Returns the text for when the button is not in the checked state.
 *
 * @return The text.
 */
ECode CToggleButton::GetTextOff(
    /* [out] */ ICharSequence** on)
{
    VALIDATE_NOT_NULL(on);
    AutoPtr<ICharSequence> csq = ToggleButton::GetTextOff();
    *on = csq;
    REFCOUNT_ADD(*on);
    return NOERROR;
}

/**
 * Sets the text for when the button is not in the checked state.
 *
 * @param textOff The text.
 */
ECode CToggleButton::SetTextOff(
    /* [in] */ ICharSequence* textOff)
{
    return ToggleButton::SetTextOff(textOff);
}

ECode CToggleButton::GetButtonDrawable(
    /* [out] */ IDrawable** d)
{
    AutoPtr<IDrawable> dTemp = ToggleButton::GetButtonDrawable();
    *d = dTemp;
    REFCOUNT_ADD(*d);
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
