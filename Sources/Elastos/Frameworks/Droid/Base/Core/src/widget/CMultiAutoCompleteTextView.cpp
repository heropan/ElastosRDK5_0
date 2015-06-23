#include "ext/frameworkext.h"
#include "widget/CMultiAutoCompleteTextView.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::Widget::EIID_TextView;

namespace Elastos{
namespace Droid{
namespace Widget{

IVIEW_METHODS_IMPL(CMultiAutoCompleteTextView, MultiAutoCompleteTextView)
IDRAWABLECALLBACK_METHODS_IMPL(CMultiAutoCompleteTextView, MultiAutoCompleteTextView)
IKEYEVENTCALLBACK_METHODS_IMPL(CMultiAutoCompleteTextView, MultiAutoCompleteTextView)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CMultiAutoCompleteTextView, MultiAutoCompleteTextView)
ITEXTVIEW_METHODS_IMPL(CMultiAutoCompleteTextView, MultiAutoCompleteTextView)
IAUTOCOMPLETETEXTVIEW_METHODS_IMPL(CMultiAutoCompleteTextView, MultiAutoCompleteTextView)

PInterface CMultiAutoCompleteTextView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    } else if (riid == EIID_TextView) {
        return reinterpret_cast<PInterface>((TextView*)this);
    }
    return _CMultiAutoCompleteTextView::Probe(riid);
}

ECode CMultiAutoCompleteTextView::constructor(
    /* [in] */ IContext* context)
{
    return MultiAutoCompleteTextView::Init(context);
}

ECode CMultiAutoCompleteTextView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return MultiAutoCompleteTextView::Init(context, attrs);
}

ECode CMultiAutoCompleteTextView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return MultiAutoCompleteTextView::Init(context, attrs, defStyle);
}

ECode CMultiAutoCompleteTextView::SetTokenizer(
    /* [in] */ ITokenizer* t)
{
    return MultiAutoCompleteTextView::SetTokenizer(t);
}

ECode CMultiAutoCompleteTextView::OnFilterComplete(
        /* [in] */ Int32 count)
{
    return MultiAutoCompleteTextView::OnFilterComplete(count);
}

ECode CMultiAutoCompleteTextView::OnPreDraw(
    /* [out] */ Boolean* result)
{
    *result = MultiAutoCompleteTextView::OnPreDraw();
    return NOERROR;
}

ECode CMultiAutoCompleteTextView::SetSelection(
    /* [in] */ Int32 start,
    /* [in] */ Int32 stop)
{
    return MultiAutoCompleteTextView::SetSelection(start, stop);
}

ECode CMultiAutoCompleteTextView::SetSelection(
    /* [in] */ Int32 index)
{
    return MultiAutoCompleteTextView::SetSelection(index);
}

ECode CMultiAutoCompleteTextView::SelectAll()
{
    return MultiAutoCompleteTextView::SelectAll();
}

ECode CMultiAutoCompleteTextView::ExtendSelection(
    /* [in] */ Int32 index)
{
    return MultiAutoCompleteTextView::ExtendSelection(index);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos