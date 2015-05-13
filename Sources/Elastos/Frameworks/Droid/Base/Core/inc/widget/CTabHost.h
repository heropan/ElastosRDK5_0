
#ifndef __CTABHOST_H_
#define __CTABHOST_H_

#include "_CTabHost.h"
#include "view/ViewMacro.h"
#include "view/ViewGroupLayoutParamsMacro.h"
#include "widget/FrameLayoutMacro.h"
#include "widget/TabHost.h"


namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CTabHost), public TabHost
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
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI NewTabSpec(
        /* [in] */ const String& tag,
        /* [out] */ ITabSpec** tabspec);

    CARAPI Setup();

    CARAPI SetupEx(
        /* [in] */ ILocalActivityManager* activityGroup);

    CARAPI AddTab(
        /* [in] */ ITabSpec* tabSpec);

    CARAPI ClearAllTabs();

    CARAPI GetTabWidget(
        /* [out] */ ITabWidget** tabWidget);

    CARAPI GetCurrentTab(
        /* [out] */ Int32* tab);

    CARAPI GetCurrentTabTag(
        /* [out] */ String* tag);

    CARAPI GetCurrentTabView(
        /* [out] */ IView** view);

    CARAPI GetCurrentView(
        /* [out] */ IView** view);

    CARAPI SetCurrentTabByTag(
        /* [in] */ const String& tag);

    CARAPI GetTabContentView(
        /* [out] */ IFrameLayout** view);

    CARAPI SetCurrentTab(
        /* [in] */ Int32 index);

    CARAPI SetOnTabChangedListener(
        /* [in] */ ITabHostOnTabChangeListener* l);

    // IOnTouchModeChangeListener

    /**
     * Callback method to be invoked when the touch mode changes.
     *
     * @param isInTouchMode True if the view hierarchy is now in touch mode, false  otherwise.
     */
    CARAPI OnTouchModeChanged(
        /* [in] */ Boolean isInTouchMode);
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__CTABHOST_H_
