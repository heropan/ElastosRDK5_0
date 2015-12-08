#ifndef __ELASTOS_DROID_WIDGET_CMULTIAUTOCOMPLETETEXTVIEW_H__
#define __ELASTOS_DROID_WIDGET_CMULTIAUTOCOMPLETETEXTVIEW_H__

#include "_Elastos_Droid_Widget_CMultiAutoCompleteTextView.h"
#include "elastos/droid/widget/MultiAutoCompleteTextView.h"




namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CMultiAutoCompleteTextView), public MultiAutoCompleteTextView
{
public:
    IVIEW_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    ITEXTVIEW_METHODS_DECL()
    IAUTOCOMPLETETEXTVIEW_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI SetTokenizer(
        /* [in] */ ITokenizer* t);

    CARAPI OnFilterComplete(
        /* [in] */ Int32 count);

    CARAPI OnPreDraw(
        /* [out] */ Boolean* result);

    CARAPI SetSelection(
        /* [in] */ Int32 start,
        /* [in] */ Int32 stop);

    CARAPI SetSelection(
        /* [in] */ Int32 index);

    CARAPI SelectAll();

    CARAPI ExtendSelection(
        /* [in] */ Int32 index);

};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CMULTIAUTOCOMPLETETEXTVIEW_H__
