#ifndef __SEARCHVIEWSEARCHAUTOCOMPLETE_H__
#define __SEARCHVIEWSEARCHAUTOCOMPLETE_H__

#include "_CSearchViewSearchAutoComplete.h"
#include "widget/SearchView.h"

namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CSearchViewSearchAutoComplete), public SearchView::_SearchAutoComplete
{
public:
    IVIEW_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    ITEXTVIEW_METHODS_DECL()
    IAUTOCOMPLETETEXTVIEW_METHODS_DECL()

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI OnPreDraw(
        /* [out] */ Boolean* result);

    CARAPI OnFilterComplete(
        /* [in] */ Int32 count);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI SetSelection(
        /* [in] */ Int32 start,
        /* [in] */ Int32 stop);

    CARAPI SetSelection(
        /* [in] */ Int32 index);

    CARAPI SelectAll();

    CARAPI ExtendSelection(
        /* [in] */ Int32 index);

    CARAPI SetSearchView(
        /* [in] */ ISearchView* searchView);
};

}
}
}
#endif