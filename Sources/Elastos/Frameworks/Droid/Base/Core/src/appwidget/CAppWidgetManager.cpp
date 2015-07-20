
#include "appwidget/CAppWidgetManager.h"
#include "appwidget/CAppWidgetProviderInfo.h"
#include "util/CTypedValue.h"
#include "os/ServiceManager.h"

using Elastos::Droid::Utility::CTypedValue;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Os::ServiceManager;

namespace Elastos {
namespace Droid {
namespace AppWidget {

const String CAppWidgetManager::TAG("AppWidgetManager");
Mutex CAppWidgetManager::sManagerCacheLock;
HashMap<AutoPtr<IContext>, AutoPtr<IWeakReference> > CAppWidgetManager::sManagerCache;
AutoPtr<IIAppWidgetService> CAppWidgetManager::sService;

ECode CAppWidgetManager::constructor(
    /* [in] */ IContext* context)
{
    mContext = context;
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    res->GetDisplayMetrics((IDisplayMetrics**)&mDisplayMetrics);
    return NOERROR;
}

ECode CAppWidgetManager::GetInstance(
    /* [in] */ IContext* context,
    /* [out] */ IAppWidgetManager** appWidgetManager)
{
    VALIDATE_NOT_NULL(appWidgetManager);
    AutoLock lock(sManagerCacheLock);
    if (sService == NULL) {
        sService = IIAppWidgetService::Probe(ServiceManager::GetService(IContext::APPWIDGET_SERVICE).Get());
    }

    AutoPtr<IAppWidgetManager> result;
    CacheMapIterator it = sManagerCache.Find(context);
    if (it != sManagerCache.End()) {
        AutoPtr<IWeakReference> wr = it->mSecond;
        wr->Resolve(EIID_IAppWidgetManager, (IInterface**)&result);
    }

    if (result == NULL) {
        CAppWidgetManager::New(context, (IAppWidgetManager**)&result);
        IWeakReferenceSource* wrs = IWeakReferenceSource::Probe(result);
        AutoPtr<IWeakReference> wr;
        wrs->GetWeakReference((IWeakReference**)&wr);
        sManagerCache[context] = wr;
    }

    *appWidgetManager = result;
    REFCOUNT_ADD(*appWidgetManager);
    return NOERROR;
}

ECode CAppWidgetManager::UpdateAppWidget(
    /* [in] */ ArrayOf<Int32>* appWidgetIds,
    /* [in] */ IRemoteViews* views)
{
    //try {
    return sService->UpdateAppWidgetIds(appWidgetIds, IParcelable::Probe(views));
    // }
    // catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
}

ECode CAppWidgetManager::UpdateAppWidgetOptions(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IBundle* options)
{
    //try {
    return sService->UpdateAppWidgetOptions(appWidgetId, options);
    //}
    //catch (RemoteException e) {
    //    throw new RuntimeException("system server dead?", e);
    //}
}

ECode CAppWidgetManager::GetAppWidgetOptions(
    /* [in] */ Int32 appWidgetId,
    /* [out] */ IBundle** options)
{
    //try {
    return sService->GetAppWidgetOptions(appWidgetId, options);
    //}
    //catch (RemoteException e) {
    //    throw new RuntimeException("system server dead?", e);
    //}
}

ECode CAppWidgetManager::UpdateAppWidget(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IRemoteViews* views)
{
    AutoPtr< ArrayOf<Int32> > idsArray = ArrayOf<Int32>::Alloc(1);
    (*idsArray)[0] = appWidgetId;
    return UpdateAppWidget(idsArray, views);
}

ECode CAppWidgetManager::PartiallyUpdateAppWidget(
    /* [in] */ ArrayOf<Int32>* appWidgetIds,
    /* [in] */ IRemoteViews* views)
{
    //try {
    return sService->PartiallyUpdateAppWidgetIds(appWidgetIds, IParcelable::Probe(views));
    //} catch (RemoteException e) {
    //    throw new RuntimeException("system server dead?", e);
    //}
}

ECode CAppWidgetManager::PartiallyUpdateAppWidget(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IRemoteViews* views)
{
    AutoPtr< ArrayOf<Int32> > idsArray = ArrayOf<Int32>::Alloc(1);
    (*idsArray)[0] = appWidgetId;
    return PartiallyUpdateAppWidget(idsArray, views);
}

ECode CAppWidgetManager::UpdateAppWidget(
    /* [in] */ IComponentName* provider,
    /* [in] */ IRemoteViews* views)
{
    //try {
    return sService->UpdateAppWidgetProvider(provider, IParcelable::Probe(views));
    // }
    // catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
}

ECode CAppWidgetManager::NotifyAppWidgetViewDataChanged(
    /* [in] */ ArrayOf<Int32>* appWidgetIds,
    /* [in] */ Int32 viewId)
{
    //try {
    return sService->NotifyAppWidgetViewDataChanged(appWidgetIds, viewId);
    //}
    //catch (RemoteException e) {
    //    throw new RuntimeException("system server dead?", e);
    //}
}

ECode CAppWidgetManager::NotifyAppWidgetViewDataChanged(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ Int32 viewId)
{
    AutoPtr< ArrayOf<Int32> > idsArray = ArrayOf<Int32>::Alloc(1);
    (*idsArray)[0] = appWidgetId;
    return NotifyAppWidgetViewDataChanged(idsArray, viewId);
}

ECode CAppWidgetManager::GetInstalledProviders(
    /* [out] */ IObjectContainer** providerInfos)
{
    return GetInstalledProviders(IAppWidgetProviderInfo::WIDGET_CATEGORY_HOME_SCREEN, providerInfos);
}

ECode CAppWidgetManager::GetInstalledProviders(
    /* [in] */ Int32 categoryFilter,
    /* [out] */ IObjectContainer** providerInfos)
{
    VALIDATE_NOT_NULL(providerInfos);
    //try {
    AutoPtr<IObjectContainer> providers;
    FAIL_RETURN(sService->GetInstalledProviders(categoryFilter, (IObjectContainer**)&providers));
    if (providers != NULL) {
        AutoPtr<IObjectEnumerator> ator;
        providers->GetObjectEnumerator((IObjectEnumerator**)&ator);
        Boolean has = FALSE;
        while (ator->MoveNext(&has), has) {
            AutoPtr<IInterface> tmp;
            ator->Current((IInterface**)&tmp);
            AutoPtr<IAppWidgetProviderInfo> info = IAppWidgetProviderInfo::Probe(tmp);
            Int32 minWidth, minHeight, minResizeWidth, minResizeHeight;
            info->GetMinWidth(&minWidth);
            info->GetMinHeight(&minHeight);
            info->GetMinResizeWidth(&minResizeWidth);
            info->GetMinResizeHeight(&minResizeHeight);
            // Converting complex to dp.
            info->SetMinWidth(CTypedValue::ComplexToDimensionPixelSize(minWidth, mDisplayMetrics));
            info->SetMinHeight(CTypedValue::ComplexToDimensionPixelSize(minHeight, mDisplayMetrics));
            info->SetMinResizeWidth(CTypedValue::ComplexToDimensionPixelSize(minResizeWidth, mDisplayMetrics));
            info->SetMinResizeHeight(CTypedValue::ComplexToDimensionPixelSize(minResizeHeight, mDisplayMetrics));
        }
    }
    // }
    // catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
    *providerInfos = providers;
    REFCOUNT_ADD(*providerInfos);
    return NOERROR;
}

ECode CAppWidgetManager::GetAppWidgetInfo(
    /* [in] */ Int32 appWidgetId,
    /* [out] */ IAppWidgetProviderInfo** info)
{
    VALIDATE_NOT_NULL(info);
    //try {
    AutoPtr<IAppWidgetProviderInfo> _info;
    FAIL_RETURN(sService->GetAppWidgetInfo(appWidgetId, (IAppWidgetProviderInfo**)&_info));
    if (_info != NULL) {
        Int32 minWidth, minHeight, minResizeWidth, minResizeHeight;
        _info->GetMinWidth(&minWidth);
        _info->GetMinHeight(&minHeight);
        _info->GetMinResizeWidth(&minResizeWidth);
        _info->GetMinResizeHeight(&minResizeHeight);
        // Converting complex to dp.
        _info->SetMinWidth(CTypedValue::ComplexToDimensionPixelSize(minWidth, mDisplayMetrics));
        _info->SetMinHeight(CTypedValue::ComplexToDimensionPixelSize(minHeight, mDisplayMetrics));
        _info->SetMinResizeWidth(CTypedValue::ComplexToDimensionPixelSize(minResizeWidth, mDisplayMetrics));
        _info->SetMinResizeHeight(CTypedValue::ComplexToDimensionPixelSize(minResizeHeight, mDisplayMetrics));
    }
    *info = _info;
    REFCOUNT_ADD(*info);
    return NOERROR;
    // }
    // catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
}

ECode CAppWidgetManager::BindAppWidgetId(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IComponentName* provider)
{
    //try {
    return sService->BindAppWidgetId(appWidgetId, provider, NULL);
    // }
    // catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
}

ECode CAppWidgetManager::BindAppWidgetId(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IComponentName* provider,
    /* [in] */ IBundle* options)
{
    //try {
    return sService->BindAppWidgetId(appWidgetId, provider, options);
    //}
    //catch (RemoteException e) {
    //    throw new RuntimeException("system server dead?", e);
    //}
}

ECode CAppWidgetManager::BindAppWidgetIdIfAllowed(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IComponentName* provider,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (mContext == NULL) {
        *result = FALSE;
        return NOERROR;
    }
    //try {
    String name;
    mContext->GetPackageName(&name);
    return sService->BindAppWidgetIdIfAllowed(
            name, appWidgetId, provider, NULL, result);
    //}
    //catch (RemoteException e) {
    //    throw new RuntimeException("system server dead?", e);
    //}
}

ECode CAppWidgetManager::BindAppWidgetIdIfAllowed(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IComponentName* provider,
    /* [in] */ IBundle* options,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (mContext == NULL) {
        *result = FALSE;
        return NOERROR;
    }
    //try {
    String name;
    mContext->GetPackageName(&name);
    return sService->BindAppWidgetIdIfAllowed(
            name, appWidgetId, provider, options, result);
    //}
    //catch (RemoteException e) {
    //    throw new RuntimeException("system server dead?", e);
    //}
}

ECode CAppWidgetManager::HasBindAppWidgetPermission(
    /* [in] */ const String& packageName,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    //try {
    return sService->HasBindAppWidgetPermission(packageName, result);
    //}
    //catch (RemoteException e) {
    //    throw new RuntimeException("system server dead?", e);
    //}
}

ECode CAppWidgetManager::SetBindAppWidgetPermission(
    /* [in] */ const String& packageName,
    /* [in] */ Boolean permission)
{
    // try {
    return sService->SetBindAppWidgetPermission(packageName, permission);
    // }
    // catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
}

ECode CAppWidgetManager::BindRemoteViewsService(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IIntent* intent,
    /* [in] */ IBinder* connection,
    /* [in] */ IUserHandle* userHandle)
{
    // try {
    Int32 id = 0;
    userHandle->GetIdentifier(&id);
    return sService->BindRemoteViewsService(appWidgetId, intent, connection, id);
    // }
    // catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
}

ECode CAppWidgetManager::UnbindRemoteViewsService(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* userHandle)
{
    // try {
    Int32 id = 0;
    userHandle->GetIdentifier(&id);
    return sService->UnbindRemoteViewsService(appWidgetId, intent, id);
    // }
    // catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
}

ECode CAppWidgetManager::GetAppWidgetIds(
    /* [in] */ IComponentName* provider,
    /* [out, callee] */ ArrayOf<Int32>** appWidgetIds)
{
    //try {
    return sService->GetAppWidgetIds(provider, appWidgetIds);
    // }
    // catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
}

} // namespace AppWidget
} // namespace Droid
} // namespace Elastos
