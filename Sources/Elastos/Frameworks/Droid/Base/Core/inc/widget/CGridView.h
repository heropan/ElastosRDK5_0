#ifndef __CGRIDVIEW_H_
#define __CGRIDVIEW_H_

#include "_CGridView.h"
#include "widget/GridView.h"







namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CGridView), public GridView
{
public:
    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    IABSLISTVIEW_METHODS_DECL()
    IABSLISTVIEW_INTERFACE_METHODS_DECL()
    IADAPTERVIEW_METHODS_DECL()

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
    CARAPI GetGravity(
        /* [out] */ Int32* gravity);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    CARAPI SetHorizontalSpacing(
        /* [in] */ Int32 horizontalSpacing);

    CARAPI SetVerticalSpacing(
        /* [in] */ Int32 verticalSpacing);

    CARAPI SetStretchMode(
        /* [in] */ Int32 stretchMode);

    CARAPI GetStretchMode(
        /* [out] */ Int32* mode);

    CARAPI SetColumnWidth(
        /* [in] */ Int32 columnWidth);

    CARAPI SetNumColumns(
        /* [in] */ Int32 numColumns);

    CARAPI SmoothScrollByOffset(
        /* [in] */ Int32 offset);

    CARAPI GetHorizontalSpacing(
        /* [out] */ Int32* spacing);

    CARAPI GetRequestedHorizontalSpacing(
        /* [out] */ Int32* spacing);

    CARAPI GetVerticalSpacing(
        /* [out] */ Int32* spacing);

    CARAPI GetColumnWidth(
        /* [out] */ Int32* width);

    CARAPI GetRequestedColumnWidth(
        /* [out] */ Int32* width);

    CARAPI GetNumColumns(
        /* [out] */ Int32* columns);

};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__CGRIDVIEW_H_
