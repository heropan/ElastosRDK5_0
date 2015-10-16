#ifndef __ELASTOS_DROID_WIDGET_CTWOLINELISTITEM_H__
#define __ELASTOS_DROID_WIDGET_CTWOLINELISTITEM_H__

#include "_Elastos_Droid_Widget_CTwoLineListItem.h"
#include "elastos/droid/widget/TwoLineListItem.h"




namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CTwoLineListItem), public TwoLineListItem
{
public:
    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    IRELATIVELAYOUT_METHODS_DECL()

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

    CARAPI GetText1(
        /* [out] */ ITextView** tv);

    CARAPI GetText2(
        /* [out] */ ITextView** tv);
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CTWOLINELISTITEM_H__
