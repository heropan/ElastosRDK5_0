#ifndef __ELASTOS_DROID_WIDGET_CTABLEROW_H__
#define __ELASTOS_DROID_WIDGET_CTABLEROW_H__

#include "_Elastos_Droid_Widget_CTableRow.h"
#include "elastos/droid/widget/TableRow.h"

namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CTableRow), public TableRow
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

    CARAPI GetVirtualChildAt(
        /* [in] */  Int32 index,
        /* [out] */  IView** child);

    CARAPI GetVirtualChildCount(
        /* [out] */ Int32* count);
};


}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CTABLEROW_H__
