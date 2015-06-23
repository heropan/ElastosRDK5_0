#include "CCompoundButton.h"

using Elastos::Droid::View::EIID_View;

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEW_METHODS_IMPL(CCompoundButton, CompoundButton)
IDRAWABLECALLBACK_METHODS_IMPL(CCompoundButton, CompoundButton)
IKEYEVENTCALLBACK_METHODS_IMPL(CCompoundButton, CompoundButton)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CCompoundButton, CompoundButton)
ITEXTVIEW_METHODS_IMPL(CCompoundButton, CompoundButton)

PInterface CCompoundButton::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_TextView) {
        return reinterpret_cast<PInterface>((TextView*)this);
    }
    return _CCompoundButton::Probe(riid);
}

ECode CCompoundButton::constructor(
    /* [in] */ IContext * pCtx)
{
    return CompoundButton::Init(pCtx);
}

ECode CCompoundButton::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs)
{
    return CompoundButton::Init(pCtx, pAttrs);
}

ECode CCompoundButton::constructor(
    /* [in] */ IContext * pContext,
    /* [in] */ IAttributeSet * pAttrs,
    /* [in] */ Int32 defStyle)
{
    return CompoundButton::Init(pContext, pAttrs, defStyle);
}

/**
 * Change the checked state of the view
 *
 * @param checked The new checked state
 */
ECode CCompoundButton::SetChecked(
    /* [in] */ Boolean checked)
{
    return CompoundButton::SetChecked(checked);
}

/**
 * @return The current checked state of the view
 */
ECode CCompoundButton::IsChecked(
    /* [out] */ Boolean* isChecked)
{
    VALIDATE_NOT_NULL(isChecked);
    *isChecked = CompoundButton::IsChecked();

    return NOERROR;
}

/**
 * Change the checked state of the view to the inverse of its current state
 *
 */
ECode CCompoundButton::Toggle()
{
    return CompoundButton::Toggle();
}

/**
 * Register a callback to be invoked when the checked state of this button
 * changes.
 *
 * @param listener the callback to call on checked state change
 */
ECode CCompoundButton::SetOnCheckedChangeListener(
    /* [in] */ ICompoundButtonOnCheckedChangeListener* listener)
{
    return CompoundButton::SetOnCheckedChangeListener(listener);
}

/**
 * Register a callback to be invoked when the checked state of this button
 * changes. This callback is used for internal purpose only.
 *
 * @param listener the callback to call on checked state change
 * @hide
 */
ECode CCompoundButton::SetOnCheckedChangeWidgetListener(
    /* [in] */ ICompoundButtonOnCheckedChangeListener* listener)
{
    return CompoundButton::SetOnCheckedChangeWidgetListener(listener);
}

/**
 * Set the background to a given Drawable, identified by its resource id.
 *
 * @param resid the resource id of the drawable to use as the background
 */
ECode CCompoundButton::SetButtonDrawable(
    /* [in] */ Int32 resid)
{
    return CompoundButton::SetButtonDrawable(resid);
}

/**
 * Set the background to a given Drawable
 *
 * @param d The Drawable to use as the background
 */
ECode CCompoundButton::SetButtonDrawable(
    /* [in] */ IDrawable* d)
{
    return CompoundButton::SetButtonDrawable(d);
}

ECode CCompoundButton::GetButtonDrawable(
    /* [out] */ IDrawable** d)
{
    AutoPtr<IDrawable> dTemp = CompoundButton::GetButtonDrawable();
    *d = dTemp;
    INTERFACE_ADDREF(*d);
    return NOERROR;
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
ECode CCompoundButton::OnPreDraw(
    /* [out] */ Boolean* result)
{

    VALIDATE_NOT_NULL(result);
    *result = CompoundButton::OnPreDraw();

    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
