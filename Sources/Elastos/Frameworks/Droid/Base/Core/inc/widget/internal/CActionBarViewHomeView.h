#include "_CActionBarViewHomeView.h"
#include "widget/internal/ActionBarViewHomeView.h"
#include "view/ViewMacro.h"
#include "view/ViewGroupLayoutParamsMacro.h"
#include "widget/FrameLayoutMacro.h"

namespace Elastos{
namespace Droid{
namespace Widget{
namespace Internal{

CarClass(CActionBarViewHomeView) , public ActionBarViewHomeView
{
public:
    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IFRAMELAYOUT_METHODS_DECL()
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

    CARAPI SetUp(
        /* [in] */ Boolean isUp);

    CARAPI SetIcon(
        /* [in] */ IDrawable* d);

    CARAPI GetStartOffset(
        /* [out] */ Int32* offset);
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos