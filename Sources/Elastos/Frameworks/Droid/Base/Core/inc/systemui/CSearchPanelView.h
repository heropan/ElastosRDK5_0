#ifndef __CSEARCHPANELVIEW_H_
#define __CSEARCHPANELVIEW_H_

#include "_CSearchPanelView.h"
#include "view/ViewMacro.h"
#include "view/ViewGroupLayoutParamsMacro.h"
#include "widget/FrameLayoutMacro.h"
#include "systemui/SearchPanelView.h"

namespace Elastos {
namespace Droid {
namespace SystemUI {

CarClass(CSearchPanelView), public SearchPanelView
{
public:
    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    IFRAMELAYOUT_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI OnAnimationStarted();

    CARAPI IsInContentArea(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [out] */ Boolean* result);

    CARAPI Show(
        /* [in] */ Boolean show,
        /* [in] */ Boolean animate);

    CARAPI Hide(
        /* [in] */ Boolean animate);

    CARAPI IsShowing(
        /* [out] */ Boolean* result);

    CARAPI SetBar(
        /* [in] */ IBaseStatusBar* bar);

    CARAPI SetStatusBarView(
        /* [in] */ IView* statusBarView);

    CARAPI IsAssistantAvailable(
        /* [out] */ Boolean* result);
};

}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif //__CSEARCHPANELVIEW_H_
