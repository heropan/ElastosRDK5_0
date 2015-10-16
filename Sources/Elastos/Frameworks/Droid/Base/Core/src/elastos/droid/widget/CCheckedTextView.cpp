
#include "elastos/droid/widget/CCheckedTextView.h"

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEW_METHODS_IMPL(CCheckedTextView, CheckedTextView)
IDRAWABLECALLBACK_METHODS_IMPL(CCheckedTextView, CheckedTextView)
IKEYEVENTCALLBACK_METHODS_IMPL(CCheckedTextView, CheckedTextView)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CCheckedTextView, CheckedTextView)
ITEXTVIEW_METHODS_IMPL(CCheckedTextView, CheckedTextView)

PInterface CCheckedTextView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_TextView) {
        return reinterpret_cast<PInterface>((TextView*)this);
    }
    return _CCheckedTextView::Probe(riid);
}

ECode CCheckedTextView::Toggle()
{
    // TODO: Add your code here
    return CheckedTextView::Toggle();
}

ECode CCheckedTextView::IsChecked(
    /* [out] */ Boolean * pChecked)
{
    // TODO: Add your code here
    *pChecked = CheckedTextView::IsChecked();
    return NOERROR;
}

ECode CCheckedTextView::SetChecked(
    /* [in] */ Boolean checked)
{
    // TODO: Add your code here
    return CheckedTextView::SetChecked(checked);
}

ECode CCheckedTextView::SetCheckMarkDrawable(
    /* [in] */ Int32 resid)
{
    // TODO: Add your code here
    return CheckedTextView::SetCheckMarkDrawable(resid);
}

ECode CCheckedTextView::SetCheckMarkDrawable(
    /* [in] */ IDrawable * pD)
{
    // TODO: Add your code here
    return CheckedTextView::SetCheckMarkDrawable(pD);
}

ECode CCheckedTextView::GetCheckMarkDrawable(
    /* [out] */ IDrawable** drawable)
{
    AutoPtr<IDrawable> temp = CheckedTextView::GetCheckMarkDrawable();
    *drawable = temp;
    REFCOUNT_ADD(*drawable);
    return NOERROR;
}

ECode CCheckedTextView::OnPreDraw(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    *pResult = CheckedTextView::OnPreDraw();
    return NOERROR;
}

ECode CCheckedTextView::constructor(
    /* [in] */ IContext * pCtx)
{
    // TODO: Add your code here
    return CheckedTextView::Init(pCtx);
}

ECode CCheckedTextView::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs)
{
    // TODO: Add your code here
    return CheckedTextView::Init(pCtx, pAttrs);
}

ECode CCheckedTextView::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs,
    /* [in] */ Int32 defStyle)
{
    // TODO: Add your code here
    return CheckedTextView::Init(pCtx, pAttrs, defStyle);
}

}// namespace Elastos
}// namespace Droid
}// namespace Widget
