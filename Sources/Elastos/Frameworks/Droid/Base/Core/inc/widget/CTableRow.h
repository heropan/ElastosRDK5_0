#ifndef __CTABLEROW_H_
#define __CTABLEROW_H_

#include "_CTableRow.h"
#include "view/ViewMacro.h"
#include "view/ViewGroupLayoutParamsMacro.h"
#include "widget/LinearLayoutMacro.h"
#include "widget/TableRow.h"

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

#endif //__CTABLEROW_H_
