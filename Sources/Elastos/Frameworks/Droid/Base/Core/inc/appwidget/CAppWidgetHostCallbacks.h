
#ifndef  __CAPPWIDGETHOSTCALLBACKS_H__
#define  __CAPPWIDGETHOSTCALLBACKS_H__

#include "_CAppWidgetHostCallbacks.h"
#include "appwidget/CAppWidgetHost.h"

namespace Elastos {
namespace Droid {
namespace AppWidget {

CarClass(CAppWidgetHostCallbacks)
{
public:
    CARAPI constructor(
        /* [in] */ IAppWidgetHost* host);

    CARAPI UpdateAppWidget(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IParcelable* views);

    CARAPI ProviderChanged(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IAppWidgetProviderInfo* info);

    CARAPI ProvidersChanged();

    CARAPI ViewDataChanged(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ Int32 viewId);

private:
    CARAPI SendToTarget(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1= 0,
        /* [in] */ Int32 arg2 = 0,
        /* [in] */ IInterface* obj = NULL);

private:
    CAppWidgetHost* mHost;
};

} // namespace AppWidget
} // namespace Droid
} // namespace Elastos

#endif  // __CAPPWIDGETHOSTCALLBACKS_H__
