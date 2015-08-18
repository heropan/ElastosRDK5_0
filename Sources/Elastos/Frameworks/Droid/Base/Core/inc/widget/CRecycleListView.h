#include "_Elastos_Droid_Widget_CRecycleListView.h"
#include "widget/RecycleListView.h"





namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CRecycleListView) , public RecycleListView
{
public:
    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    IADAPTERVIEW_METHODS_DECL()
    IABSLISTVIEW_METHODS_DECL()
    IABSLISTVIEW_INTERFACE_METHODS_DECL()
    ILISTVIEW_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* ctx);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI GetRecycleOnMeasure(
        /* [out] */ Boolean* recycleOnMeasure);

    CARAPI SetRecycleOnMeasure(
        /* [in] */ Boolean recycleOnMeasure);
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos
