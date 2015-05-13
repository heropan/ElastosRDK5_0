
#include "appwidget/AppWidgetProvider.h"
#ifdef DROID_CORE
#include "appwidget/CAppWidgetManagerHelper.h"
#else
#include "Elastos.Droid.Core.h"
#endif

namespace Elastos {
namespace Droid {
namespace AppWidget {

AppWidgetProvider::AppWidgetProvider()
{}

ECode AppWidgetProvider::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    // Protect against rogue update broadcasts (not really a security issue,
    // just filter bad broacasts out so subclasses are less likely to crash).
    String action;
    intent->GetAction(&action);
    if (IAppWidgetManager::ACTION_APPWIDGET_UPDATE.Equals(action)) {
        AutoPtr<IBundle> extras;
        intent->GetExtras((IBundle**)&extras);
        if (extras != NULL) {
            AutoPtr< ArrayOf<Int32> > appWidgetIds;
            extras->GetInt32Array(
                    IAppWidgetManager::EXTRA_APPWIDGET_IDS, (ArrayOf<Int32>**)&appWidgetIds);
            if (appWidgetIds != NULL && appWidgetIds->GetLength() > 0) {
                AutoPtr<IAppWidgetManagerHelper> helper;
                CAppWidgetManagerHelper::AcquireSingleton((IAppWidgetManagerHelper**)&helper);
                AutoPtr<IAppWidgetManager> manager;
                helper->GetInstance(context, (IAppWidgetManager**)&manager);
                OnUpdate(context, manager, *appWidgetIds);
            }
        }
    }
    else if (IAppWidgetManager::ACTION_APPWIDGET_DELETED.Equals(action)) {
        AutoPtr<IBundle> extras;
        intent->GetExtras((IBundle**)&extras);
        Boolean result;
        if (extras != NULL && (extras->ContainsKey(IAppWidgetManager::EXTRA_APPWIDGET_ID, &result), result)) {
            Int32 appWidgetId;
            extras->GetInt32(IAppWidgetManager::EXTRA_APPWIDGET_ID, &appWidgetId);
            AutoPtr< ArrayOf<Int32> > idsArray = ArrayOf<Int32>::Alloc(1);
            (*idsArray)[0] = appWidgetId;
            OnDeleted(context, *idsArray);
        }
    }
    else if (IAppWidgetManager::ACTION_APPWIDGET_OPTIONS_CHANGED.Equals(action)) {
        AutoPtr<IBundle> extras;
        intent->GetExtras((IBundle**)&extras);
        Boolean result;
        if (extras != NULL && (extras->ContainsKey(IAppWidgetManager::EXTRA_APPWIDGET_ID, &result), result)
                && (extras->ContainsKey(IAppWidgetManager::EXTRA_APPWIDGET_OPTIONS, &result), result)) {
            Int32 appWidgetId;
            extras->GetInt32(IAppWidgetManager::EXTRA_APPWIDGET_ID, &appWidgetId);
            AutoPtr<IBundle> widgetExtras;
            extras->GetBundle(IAppWidgetManager::EXTRA_APPWIDGET_OPTIONS, (IBundle**)&widgetExtras);
            AutoPtr<IAppWidgetManagerHelper> helper;
            CAppWidgetManagerHelper::AcquireSingleton((IAppWidgetManagerHelper**)&helper);
            AutoPtr<IAppWidgetManager> manager;
            helper->GetInstance(context, (IAppWidgetManager**)&manager);
            OnAppWidgetOptionsChanged(context, manager, appWidgetId, widgetExtras);
        }
    }
    else if (IAppWidgetManager::ACTION_APPWIDGET_ENABLED.Equals(action)) {
        OnEnabled(context);
    }
    else if (IAppWidgetManager::ACTION_APPWIDGET_DISABLED.Equals(action)) {
        OnDisabled(context);
    }

    return NOERROR;
}

ECode AppWidgetProvider::OnUpdate(
    /* [in] */ IContext* context,
    /* [in] */ IAppWidgetManager* appWidgetManager,
    /* [in] */ const ArrayOf<Int32>& appWidgetIds)
{
    return NOERROR;
}

ECode AppWidgetProvider::OnAppWidgetOptionsChanged(
    /* [in] */ IContext* context,
    /* [in] */ IAppWidgetManager* appWidgetManager,
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IBundle* newOptions)
{
    return NOERROR;
}

ECode AppWidgetProvider::OnDeleted(
    /* [in] */ IContext* context,
    /* [in] */ const ArrayOf<Int32>& appWidgetIds)
{
    return NOERROR;
}

ECode AppWidgetProvider::OnEnabled(
    /* [in] */ IContext* context)
{
    return NOERROR;
}

ECode AppWidgetProvider::OnDisabled(
    /* [in] */ IContext* context)
{
    return NOERROR;
}

} // namespace AppWidget
} // namespace Droid
} // namespace Elastos
