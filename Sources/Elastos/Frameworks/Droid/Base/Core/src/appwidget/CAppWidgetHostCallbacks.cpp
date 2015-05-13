
#include "appwidget/CAppWidgetHostCallbacks.h"
#include "os/Handler.h"

using Elastos::Droid::Os::Handler;

namespace Elastos {
namespace Droid {
namespace AppWidget {

ECode CAppWidgetHostCallbacks::constructor(
    /* [in] */ IAppWidgetHost* host)
{
    mHost = (CAppWidgetHost*)host;
    return NOERROR;
}

ECode CAppWidgetHostCallbacks::UpdateAppWidget(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IParcelable* _views)
{
    AutoPtr<IRemoteViews> views = IRemoteViews::Probe(_views);
    if (mHost->IsLocalBinder() && views != NULL) {
        AutoPtr<IRemoteViews> views2;
        views->Clone((IRemoteViews**)&views2);
        views->SetUser(mHost->mUser);
        views = views2;
    }
    return SendToTarget(CAppWidgetHost::HANDLE_UPDATE, appWidgetId, 0, views);
}

ECode CAppWidgetHostCallbacks::ProviderChanged(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IAppWidgetProviderInfo* _info)
{
    AutoPtr<IAppWidgetProviderInfo> info = _info;
    if (mHost->IsLocalBinder() && info != NULL) {
        AutoPtr<IAppWidgetProviderInfo> info2;
        info->Clone((IAppWidgetProviderInfo**)&info2);
        info = info2;
    }
    return SendToTarget(CAppWidgetHost::HANDLE_PROVIDER_CHANGED, appWidgetId, 0, info);
}

ECode CAppWidgetHostCallbacks::ProvidersChanged()
{
    return SendToTarget(CAppWidgetHost::HANDLE_PROVIDERS_CHANGED);
}

ECode CAppWidgetHostCallbacks::ViewDataChanged(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ Int32 viewId)
{
    return SendToTarget(CAppWidgetHost::HANDLE_VIEW_DATA_CHANGED, appWidgetId, viewId);
}

ECode CAppWidgetHostCallbacks::SendToTarget(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ IInterface* obj)
{
    AutoPtr<IMessage> msg;
    mHost->mHandler->ObtainMessageEx3(what, arg1, arg2, obj, (IMessage**)&msg);
    return msg->SendToTarget();
}

} // namespace AppWidget
} // namespace Droid
} // namespace Elastos
