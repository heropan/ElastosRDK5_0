#ifndef __CGRIDLAYOUT_H__
#define __CGRIDLAYOUT_H__

#include "_CGridLayout.h"
#include "view/ViewMacro.h"
#include "view/ViewGroupLayoutParamsMacro.h"
#include "widget/GridLayout.h"

namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CGridLayout), public GridLayout
{
public:
    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

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

public:
    CARAPI GetOrientation(
        /* [out] */ Int32* orientation);

    CARAPI SetOrientation(
        /* [in] */ Int32 orientation);

    CARAPI GetRowCount(
        /* [out] */ Int32* count);

    CARAPI SetRowCount(
        /* [in] */ Int32 rowCount);

    CARAPI GetColumnCount(
        /* [out] */ Int32* count);

    CARAPI SetColumnCount(
        /* [in] */ Int32 columnCount);

    CARAPI GetUseDefaultMargins(
        /* [out] */ Boolean* margins);

    CARAPI SetUseDefaultMargins(
        /* [in] */ Boolean useDefaultMargins);

    CARAPI GetAlignmentMode(
        /* [out] */ Int32* mode);

    CARAPI SetAlignmentMode(
        /* [in] */ Int32 alignmentMode);

    CARAPI IsRowOrderPreserved(
        /* [out] */ Boolean* preserved);

    CARAPI SetRowOrderPreserved(
        /* [in] */ Boolean rowOrderPreserved);

    CARAPI IsColumnOrderPreserved(
        /* [out] */ Boolean* preserved);

    CARAPI SetColumnOrderPreserved(
        /* [in] */ Boolean columnOrderPreserved);

    CARAPI GetMeasurementIncludingMargin(
    /* [in] */ IView* c,
    /* [in] */ Boolean horizontal,
    /* [out] */ Int32* margin);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__CGRIDLAYOUT_H__