#include "ext/frameworkext.h"
#include "widget/CSearchView.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos{
namespace Droid{
namespace Widget{

IVIEW_METHODS_IMPL(CSearchView, SearchView)
IVIEWGROUP_METHODS_IMPL(CSearchView, SearchView)
IVIEWPARENT_METHODS_IMPL(CSearchView, SearchView)
IVIEWMANAGER_METHODS_IMPL(CSearchView, SearchView)
IDRAWABLECALLBACK_METHODS_IMPL(CSearchView, SearchView)
IKEYEVENTCALLBACK_METHODS_IMPL(CSearchView, SearchView)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CSearchView, SearchView)
ILINEARLAYOUT_METHODS_IMPL(CSearchView, SearchView)

PInterface CSearchView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    else if (riid == EIID_SearchView) {
        return reinterpret_cast<PInterface>((SearchView*)this);
    }
    return _CSearchView::Probe(riid);
}

ECode CSearchView::constructor(
    /* [in] */ IContext* context)
{
    return SearchView::Init(context);
}

ECode CSearchView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return SearchView::Init(context, attrs);
}

ECode CSearchView::OnActionViewExpanded()
{
    return SearchView::OnActionViewExpanded();
}

ECode CSearchView::OnActionViewCollapsed()
{
    return SearchView::OnActionViewCollapsed();
}

ECode CSearchView::SetSearchableInfo(
    /* [in] */ ISearchableInfo* searchable)
{
    return SearchView::SetSearchableInfo(searchable);
}

ECode CSearchView::SetAppSearchData(
    /* [in] */ IBundle* appSearchData)
{
    return SearchView::SetAppSearchData(appSearchData);
}

ECode CSearchView::SetImeOptions(
    /* [in] */ Int32 imeOptions)
{
    return SearchView::SetImeOptions(imeOptions);
}

ECode CSearchView::GetImeOptions(
    /* [out] */ Int32* imeOptions)
{
    VALIDATE_NOT_NULL(imeOptions);
    *imeOptions = SearchView::GetImeOptions();
    return NOERROR;
}

ECode CSearchView::SetInputType(
    /* [in] */ Int32 inputType)
{
    return SearchView::SetInputType(inputType);
}

ECode CSearchView::GetInputType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = SearchView::GetInputType();
    return NOERROR;
}

ECode CSearchView::SetOnQueryTextListener(
    /* [in] */ ISearchViewOnQueryTextListener* listener)
{
    return SearchView::SetOnQueryTextListener(listener);
}

ECode CSearchView::SetOnCloseListener(
    /* [in] */ IOnCloseListener* listener)
{
    return SearchView::SetOnCloseListener(listener);
}

ECode CSearchView::SetOnQueryTextFocusChangeListener(
    /* [in] */ IViewOnFocusChangeListener* listener)
{
    return SearchView::SetOnQueryTextFocusChangeListener(listener);
}

ECode CSearchView::SetOnSuggestionListener(
    /* [in] */ ISearchViewOnSuggestionListener* listener)
{
    return SearchView::SetOnSuggestionListener(listener);
}

ECode CSearchView::SetOnSearchClickListener(
    /* [in] */ IViewOnClickListener* listener)
{
    return SearchView::SetOnSearchClickListener(listener);
}

ECode CSearchView::GetQuery(
    /* [out] */ ICharSequence** str)
{
    VALIDATE_NOT_NULL(str);
    AutoPtr<ICharSequence> csq = SearchView::GetQuery();
    *str = csq;
    INTERFACE_ADDREF(*str);
    return NOERROR;
}

ECode CSearchView::SetQuery(
    /* [in] */ ICharSequence* query,
    /* [in] */ Boolean submit)
{
    return SearchView::SetQuery(query, submit);
}

ECode CSearchView::SetQueryHint(
    /* [in] */ ICharSequence* hint)
{
    return SearchView::SetQueryHint(hint);
}

ECode CSearchView::GetQueryHint(
    /* [out] */ ICharSequence** hint)
{
    VALIDATE_NOT_NULL(hint);
    AutoPtr<ICharSequence> csq = SearchView::GetQueryHint();
    *hint = csq;
    INTERFACE_ADDREF(*hint);
    return NOERROR;
}

ECode CSearchView::SetIconifiedByDefault(
    /* [in] */ Boolean iconified)
{
    return SearchView::SetIconifiedByDefault(iconified);
}

ECode CSearchView::IsIconfiedByDefault(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = SearchView::IsIconfiedByDefault();
    return NOERROR;
}

ECode CSearchView::SetIconified(
    /* [in] */ Boolean iconify)
{
    return SearchView::SetIconified(iconify);
}

ECode CSearchView::IsIconified(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = SearchView::IsIconified();
    return NOERROR;
}

ECode CSearchView::SetSubmitButtonEnabled(
    /* [in] */ Boolean enabled)
{
    return SearchView::SetSubmitButtonEnabled(enabled);
}

ECode CSearchView::IsSubmitButtonEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = SearchView::IsSubmitButtonEnabled();
    return NOERROR;
}

ECode CSearchView::SetQueryRefinementEnabled(
    /* [in] */ Boolean isEnable)
{
    return SearchView::SetQueryRefinementEnabled(isEnable);
}

ECode CSearchView::IsQueryRefinementEnabled(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = SearchView::IsQueryRefinementEnabled();
    return NOERROR;
}

ECode CSearchView::SetSuggestionsAdapter(
    /* [in] */ ICursorAdapter* adapter)
{
    return SearchView::SetSuggestionsAdapter(adapter);
}

ECode CSearchView::GetSuggestionsAdapter(
    /* [out] */ ICursorAdapter** adapter)
{
    VALIDATE_NOT_NULL(adapter);
    AutoPtr<ICursorAdapter> a = SearchView::GetSuggestionsAdapter();
    *adapter = a;
    INTERFACE_ADDREF(*adapter);
    return NOERROR;
}

ECode CSearchView::SetMaxWidth(
    /* [in] */ Int32 maxpixels)
{
    return SearchView::SetMaxWidth(maxpixels);
}

ECode CSearchView::GetMaxWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = SearchView::GetMaxWidth();
    return NOERROR;
}

ECode CSearchView::OnTextFocusChanged()
{
    return SearchView::OnTextFocusChanged();
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos