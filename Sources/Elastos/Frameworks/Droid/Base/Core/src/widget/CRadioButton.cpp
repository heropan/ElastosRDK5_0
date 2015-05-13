#include "widget/CRadioButton.h"

using Elastos::Droid::View::EIID_View;

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEW_METHODS_IMPL(CRadioButton, RadioButton);
IDRAWABLECALLBACK_METHODS_IMPL(CRadioButton, RadioButton);
IKEYEVENTCALLBACK_METHODS_IMPL(CRadioButton, RadioButton);
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CRadioButton, RadioButton);
ITEXTVIEW_METHODS_IMPL(CRadioButton, RadioButton);
ICOMPOUNDBUTTON_METHODS_IMPL(CRadioButton, RadioButton);
ICHECKABLE_METHODS_IMPL(CRadioButton, RadioButton);

PInterface CRadioButton::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_TextView) {
        return reinterpret_cast<PInterface>((TextView*)this);
    }
    return _CRadioButton::Probe(riid);
}

ECode CRadioButton::constructor(
    /* [in] */ IContext * pCtx)
{
    return RadioButton::Init(pCtx);
}

ECode CRadioButton::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs)
{
    return RadioButton::Init(pCtx, pAttrs);
}

ECode CRadioButton::constructor(
    /* [in] */ IContext * pContext,
    /* [in] */ IAttributeSet * pAttrs,
    /* [in] */ Int32 defStyle)
{
    return RadioButton::Init(pContext, pAttrs, defStyle);
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
ECode CRadioButton::OnPreDraw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = RadioButton::OnPreDraw();

    return NOERROR;
}

ECode CRadioButton::GetButtonDrawable(
    /* [out] */ IDrawable** d)
{
    VALIDATE_NOT_NULL(d);
    AutoPtr<IDrawable> dTemp = RadioButton::GetButtonDrawable();
    *d = dTemp;
    INTERFACE_ADDREF(*d);
    return NOERROR;

}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
