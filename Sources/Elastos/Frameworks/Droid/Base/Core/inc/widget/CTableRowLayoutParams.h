#ifndef __CTABLEROWLAYOUTPRARAMS_H_
#define __CTABLEROWLAYOUTPRARAMS_H_

#include "_CTableRowLayoutParams.h"
#include "view/ViewMacro.h"
#include "view/ViewGroupLayoutParamsMacro.h"
#include "widget/TableRowLayoutParams.h"
#include "widget/LinearLayoutMacro.h"

namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CTableRowLayoutParams), public TableRowLayoutParams
{
public:

    IVIEWGROUPLP_METHODS_DECL()
    IVIEWGROUPMARGINLP_METHODS_DECL()
    ILINEARLAYOUTLP_METHODS_DECL()

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Float weight);

    CARAPI constructor(
        /* [in] */ Int32 column);

    CARAPI constructor(
        /* [in] */ IViewGroupLayoutParams* source);

    CARAPI constructor(
        /* [in] */ IViewGroupMarginLayoutParams* source);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI SetColumn(
        /* [in] */ Int32 column);

    CARAPI GetColumn(
        /* [out] */ Int32* column);

    CARAPI SetSpan(
        /* [in] */ Int32 span);

    CARAPI GetSpan(
        /* [out] */ Int32* span);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__CTABLEROWLAYOUTPRARAMS_H_
