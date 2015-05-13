
#include "widget/CSearchViewSearchAutoComplete.h"


using Elastos::Droid::View::EIID_View;
using Elastos::Droid::Widget::EIID_TextView;

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEW_METHODS_IMPL(CSearchViewSearchAutoComplete, SearchView::_SearchAutoComplete)
IDRAWABLECALLBACK_METHODS_IMPL(CSearchViewSearchAutoComplete, SearchView::_SearchAutoComplete)
IKEYEVENTCALLBACK_METHODS_IMPL(CSearchViewSearchAutoComplete, SearchView::_SearchAutoComplete)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CSearchViewSearchAutoComplete, SearchView::_SearchAutoComplete)
ITEXTVIEW_METHODS_IMPL(CSearchViewSearchAutoComplete, SearchView::_SearchAutoComplete)
IAUTOCOMPLETETEXTVIEW_METHODS_IMPL(CSearchViewSearchAutoComplete, SearchView::_SearchAutoComplete)

PInterface CSearchViewSearchAutoComplete::Probe(
    /* [in] */ REIID riid)
{

    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    } else if (riid == EIID_TextView) {
        return reinterpret_cast<PInterface>((TextView*)this);
    } else if (riid == EIID_SearchAutoComplete) {
        return reinterpret_cast<PInterface>((SearchView::_SearchAutoComplete*)this);
    }
    return _CSearchViewSearchAutoComplete::Probe(riid);

}

ECode CSearchViewSearchAutoComplete::constructor(
    /* [in] */ IContext* context)
{
    return SearchView::_SearchAutoComplete::Init(context);
}

ECode CSearchViewSearchAutoComplete::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return SearchView::_SearchAutoComplete::Init(context, attrs);
}

ECode CSearchViewSearchAutoComplete::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return SearchView::_SearchAutoComplete::Init(context, attrs, defStyle);
}

ECode CSearchViewSearchAutoComplete::SetSelection(
    /* [in] */ Int32 start,
    /* [in] */ Int32 stop)
{
    return SearchView::_SearchAutoComplete::SetSelection(start, stop);
}

ECode CSearchViewSearchAutoComplete::SetSelectionEx(
    /* [in] */ Int32 index)
{
    return SearchView::_SearchAutoComplete::SetSelection(index);
}

ECode CSearchViewSearchAutoComplete::SelectAll()
{
    return SearchView::_SearchAutoComplete::SelectAll();
}

ECode CSearchViewSearchAutoComplete::ExtendSelection(
    /* [in] */ Int32 index)
{
    return SearchView::_SearchAutoComplete::ExtendSelection(index);
}

ECode CSearchViewSearchAutoComplete::SetSearchView(
    /* [in] */ ISearchView* searchView)
{
    SearchView::_SearchAutoComplete::SetSearchView(searchView);
    return NOERROR;
}

ECode CSearchViewSearchAutoComplete::OnFilterComplete(
        /* [in] */ Int32 count)
{
    return SearchView::_SearchAutoComplete::OnFilterComplete(count);
}

ECode CSearchViewSearchAutoComplete::OnPreDraw(
    /* [out] */ Boolean* result)
{
    *result = SearchView::_SearchAutoComplete::OnPreDraw();
    return NOERROR;
}
}// namespace Widget
}// namespace Droid
}// namespace Elastos