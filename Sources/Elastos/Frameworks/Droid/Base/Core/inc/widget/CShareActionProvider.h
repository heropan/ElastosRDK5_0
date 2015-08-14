#ifndef __ELASTOS_DROID_WIDGET_CSHAREACTIONPROVIDER_H__
#define __ELASTOS_DROID_WIDGET_CSHAREACTIONPROVIDER_H__

#include "_CShareActionProvider.h"
#include "widget/ShareActionProvider.h"

using Elastos::Droid::View::ISubUiVisibilityListener;
using Elastos::Droid::View::IVisibilityListener;

namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CShareActionProvider), public ShareActionProvider
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI SetOnShareTargetSelectedListener(
        /* [in] */ IOnShareTargetSelectedListener* listener);

    CARAPI SetShareHistoryFileName(
        /* [in] */ const String& shareHistoryFile);

    CARAPI SetShareIntent(
        /* [in] */ IIntent* shareIntent);

    CARAPI OnCreateActionView(
        /* [out] */ IView** view);

    CARAPI OnCreateActionView(
        /* [in] */ IMenuItem* forItem,
        /* [out] */ IView** view);

    CARAPI OverridesItemVisibility(
        /* [out] */ Boolean* res);

    CARAPI IsVisible(
        /* [out] */ Boolean* res);

    CARAPI RefreshVisibility();

    CARAPI OnPerformDefaultAction(
        /* [out] */ Boolean* res);

    CARAPI HasSubMenu(
        /* [out] */ Boolean* res);

    CARAPI OnPrepareSubMenu(
        /* [in] */ ISubMenu* submenu);

    CARAPI SubUiVisibilityChanged(
        /* [in] */ Boolean isVisible);

    CARAPI SetSubUiVisibilityListener(
        /* [in] */ ISubUiVisibilityListener* listener);

    CARAPI SetVisibilityListener(
        /* [in] */ IVisibilityListener* listener);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CSHAREACTIONPROVIDER_H__
