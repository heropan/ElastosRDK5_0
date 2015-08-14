#ifndef __ELASTOS_DROID_WIDGET_CSEARCHVIEW_H__
#define __ELASTOS_DROID_WIDGET_CSEARCHVIEW_H__

#include "_CSearchView.h"
#include "widget/SearchView.h"




namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CSearchView), public SearchView
{
public:
    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    ILINEARLAYOUT_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

public:
    CARAPI SetSearchableInfo(
        /* [in] */ ISearchableInfo* searchable);

    CARAPI SetAppSearchData(
        /* [in] */ IBundle* appSearchData);

    CARAPI SetImeOptions(
        /* [in] */ Int32 imeOptions);

    CARAPI GetImeOptions(
        /* [out] */ Int32* imeOptions);

    CARAPI SetInputType(
        /* [in] */ Int32 inputType);

    CARAPI GetInputType(
        /* [out] */ Int32* type);

    CARAPI SetOnQueryTextListener(
        /* [in] */ ISearchViewOnQueryTextListener* listener);

    CARAPI SetOnCloseListener(
        /* [in] */ IOnCloseListener* listener);

    CARAPI SetOnQueryTextFocusChangeListener(
        /* [in] */ IViewOnFocusChangeListener* listener);

    CARAPI SetOnSuggestionListener(
        /* [in] */ ISearchViewOnSuggestionListener* listener);

    CARAPI SetOnSearchClickListener(
        /* [in] */ IViewOnClickListener* listener);

    CARAPI GetQuery(
        /* [out] */ ICharSequence** str);

    CARAPI SetQuery(
        /* [in] */ ICharSequence* query,
        /* [in] */ Boolean submit);

    CARAPI SetQueryHint(
        /* [in] */ ICharSequence* hint);

    CARAPI GetQueryHint(
        /* [out] */ ICharSequence** hint);

    CARAPI SetIconifiedByDefault(
        /* [in] */ Boolean iconified);

    CARAPI IsIconfiedByDefault(
        /* [out] */ Boolean* res);

    CARAPI SetIconified(
        /* [in] */ Boolean iconify);

    CARAPI IsIconified(
        /* [out] */ Boolean* res);

    CARAPI SetSubmitButtonEnabled(
        /* [in] */ Boolean enabled);

    CARAPI IsSubmitButtonEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI SetQueryRefinementEnabled(
        /* [in] */ Boolean isEnable);

    CARAPI IsQueryRefinementEnabled(
        /* [out] */ Boolean* res);

    CARAPI SetSuggestionsAdapter(
        /* [in] */ ICursorAdapter* adapter);

    CARAPI GetSuggestionsAdapter(
        /* [out] */ ICursorAdapter** adapter);

    CARAPI SetMaxWidth(
        /* [in] */ Int32 maxpixels);

    CARAPI GetMaxWidth(
        /* [out] */ Int32* width);

    CARAPI OnTextFocusChanged();

    CARAPI OnActionViewExpanded();

    CARAPI OnActionViewCollapsed();
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CSEARCHVIEW_H__
