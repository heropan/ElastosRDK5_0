#include "elastos/droid/app/CSearchManager.h"

namespace Elastos {
namespace Droid {
namespace App {

CAR_INTERFACE_IMPL_3(CSearchManager, Object, ISearchManager, IDialogInterfaceOnDismissListener, IDialogInterfaceOnCancelListener)

CAR_OBJECT_IMPL()

CSearchManager::CSearchManager()
{
}

CSearchManager::~CSearchManager()
{
}

ECode CSearchManager::constructor(
    /* [in] */ IContext *context,
    /* [in] */ IHandler *handler)
{
    mContext = context;
    mHandler = handler;
    Elastos::GetServiceManager((IServiceManager**)&serviceManager);
    (serviceManager->GetService(IContext::SEARCH_SERVICE, (IInterface**)&mService));
    return NOERROR;
}
ECode CSearchManager::StartSearch(
    /* [in] */ const String &initialQuery,
    /* [in] */ Boolean selectInitialQuery,
    /* [in] */ IComponentName *launchActivity,
    /* [in] */ IBundle *appSearchData,
    /* [in] */ Boolean globalSearch)
{
    return StartSearch(initialQuery, selectInitialQuery, launchActivity,
            appSearchData, globalSearch, NULL);
}

ECode CSearchManager::StartSearch(
    /* [in] */ const String &initialQuery,
    /* [in] */ Boolean selectInitialQuery,
    /* [in] */ IComponentName launchActivity,
    /* [in] */ IBundle *appSearchData,
    /* [in] */ Boolean globalSearch,
    /* [in] */ IRect *sourceBounds)
{
    if (globalSearch) {
        return StartGlobalSearch(initialQuery, selectInitialQuery, appSearchData, sourceBounds);
    }

    UiModeManager uiModeManager = new UiModeManager();
    // Don't show search dialog on televisions.
    if (uiModeManager.getCurrentModeType() != Configuration.UI_MODE_TYPE_TELEVISION) {
        ensureSearchDialog();

        mSearchDialog.show(initialQuery, selectInitialQuery, launchActivity, appSearchData);
    }
    return NOERROR;
}

ECode CSearchManager::GetGlobalSearchActivities(
    /* [out] */ IObjectContainer **apps)
{
    //try {
        return mService->GetGlobalSearchActivities();
    //} catch (RemoteException ex) {
    //    Log.e(TAG, "getGlobalSearchActivities() failed: " + ex);
    //    return null;
    //}
}

ECode CSearchManager::GetGlobalSearchActivity(
    /* [out] */ IComponentName **name)
{
    //try {
        return mService->GetGlobalSearchActivity();
    //} catch (RemoteException ex) {
    //    Log.e(TAG, "getGlobalSearchActivity() failed: " + ex);
    //    return null;
    //}
}

ECode CSearchManager::GetWebSearchActivity(
    /* [out] */ IComponentName **name)
{
    //try {
        return mService->GetWebSearchActivity();
    //} catch (RemoteException ex) {
    //    Log.e(TAG, "getWebSearchActivity() failed: " + ex);
    //    return null;
    //}
}

ECode CSearchManager::TriggerSearch(
    /* [in] */ const String &query,
    /* [in] */ IComponentName *launchActivity,
    /* [in] */ IBundle *appSearchData)
{
    String name;
    launchActivity->GetPackageName(&name);
    if (!mAssociatedPackage.Equals(name)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        //throw new IllegalArgumentException("invoking app search on a different package " +
        //        "not associated with this search manager");
    }
    if (query == NULL || TextUtils::GetTrimmedLength(query) == 0) {
        //Log.w(TAG, "triggerSearch called with empty query, ignoring.");
        return;
    }
    StartSearch(query, false, launchActivity, appSearchData, false);
    mSearchDialog->LaunchQuerySearch();
    return NOERROR;
}

ECode CSearchManager::StopSearch()
{
    if (mSearchDialog != NULL) {
        mSearchDialog->Cancel();
    }
    return NOERROR;
}

ECode CSearchManager::IsVisible(
    /* [out] */ Boolean *visible)
{
    if(mSearchDialog == NULL)
    {
        *visible = FALSE;
    }
    else
    {
        mSearchDialog->IsShowing(visible);
    }
    return NOERROR;
}

ECode CSearchManager::SetOnDismissListener(
    /* [in] */ ISearchManagerOnDismissListener *listener)
{
    mDismissListener = listener;
    return NOERROR;
}

ECode CSearchManager::SetOnCancelListener(
    /* [in] */ ISearchManagerOnCancelListener *listener)
{
    mCancelListener = listener;
    return NOERROR;
}

ECode CSearchManager::GetSearchableInfo(
    /* [in] */ IComponentName *componentName,
    /* [out] */ ISearchableInfo **info)
{
    //try {
        return mService->GetSearchableInfo(componentName, info);
    //} catch (RemoteException ex) {
    //    Log.e(TAG, "getSearchableInfo() failed: " + ex);
    //    return null;
    //}
}

ECode CSearchManager::GetSuggestions(
    /* [in] */ ISearchableInfo *searchable,
    /* [in] */ const String &query,
    /* [out] */ ICursor **cursor)
{

    return GetSuggestions(searchable, query, -1, cursor);
}

ECode CSearchManager::GetSuggestions(
    /* [in] */ ISearchableInfo *searchable,
    /* [in] */ const String &query,
    /* [in] */ Int32 limit,
    /* [out] */ ICursor **cursor)
{
    if (searchable == NULL) {
        *cursor = NULL;
        return NOERROR;
    }

    String authority;
    searchable->GetSuggestAuthority(&authority);
    if (authority == NULL) {
        *cursor = NULL;
        return NOERROR;
    }

    AutoPtr<IUriBuilder> uriBuilder;
    CUriBuilder::New(&uriBuilder);
    assert(0 && "TODO");
    // Uri.Builder uriBuilder = new Uri.Builder()
    //         .scheme(ContentResolver.SCHEME_CONTENT)
    //         .authority(authority)
    //         .query("")  // TODO: Remove, workaround for a bug in Uri.writeToParcel()
    //         .fragment("");  // TODO: Remove, workaround for a bug in Uri.writeToParcel()

    // // if content path provided, insert it now
    // final String contentPath = searchable.getSuggestPath();
    // if (contentPath != NULL) {
    //     uriBuilder.appendEncodedPath(contentPath);
    // }

    // // append standard suggestion query path
    // uriBuilder.appendPath(SearchManager.SUGGEST_URI_PATH_QUERY);

    // // get the query selection, may be NULL
    // String selection = searchable.getSuggestSelection();
    // // inject query, either as selection args or inline
    // String[] selArgs = NULL;
    // if (selection != NULL) {    // use selection if provided
    //     selArgs = new String[] { query };
    // } else {                    // no selection, use REST pattern
    //     uriBuilder.appendPath(query);
    // }

    // if (limit > 0) {
    //     uriBuilder.appendQueryParameter(SUGGEST_PARAMETER_LIMIT, String.valueOf(limit));
    // }

    // Uri uri = uriBuilder.build();

    // // finally, make the query
    // return mContext.getContentResolver().query(uri, NULL, selection, selArgs, NULL);
    return E_NOT_IMPLEMENTED;
}

ECode CSearchManager::GetSearchablesInGlobalSearch(
    /* [out] */ IObjectContainer **info)
{
        // try {
            return mService->GetSearchablesInGlobalSearch(info);
        // } catch (RemoteException e) {
        //     Log.e(TAG, "getSearchablesInGlobalSearch() failed: " + e);
        //     return null;
        // }
}

ECode CSearchManager::GetAssistIntent(
    /* [in] */ IContext *context,
    /* [in] */ Boolean inclContext,
    /* [out] */ IIntent **intent)
{
    return GetAssistIntent(context, inclContext, UserHandle::MyUserId(), intent);
}

ECode CSearchManager::GetAssistIntent(
    /* [in] */ IContext *context,
    /* [in] */ Boolean inclContext,
    /* [in] */ Int32 userHandle,
    /* [out] */ IIntent **intent)
{
    VALIDATE_NOT_NULL(intent);
    //try {
    if (mService == NULL) {
        *intent = NULL;
        return NOERROR;
    }

    AutoPtr<IComponentName> comp;
    mService->GetAssistIntent(userHandle, &comp);
    if (comp == NULL) {
        *intent = NULL;
        return NOERROR;
    }
    AutoPtr<IIntent> pIntent;
    CIntent::New(IIntent::ACTION_ASSIST, &pIntent);
    pIntent->SetComponent(comp);

    if (inclContext) {
        AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();
        AutoPtr<IBundle> extras;
        am->GetAssistContextExtras(0, (IBundle**)&extras);
        if (extras != NULL) {
            intent->ReplaceExtras(extras);
        }
    }

    *intent = pIntent;
    REFCOUNT_ADD(*intent);
    //} catch (RemoteException re) {
    //    Log.e(TAG, "getAssistIntent() failed: " + re);
    //    return null;
    //}
    return NOERROR;
}

ECode CSearchManager::LaunchAssistAction(
    /* [in] */ Int32 requestType,
    /* [in] */ const String& hint,
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* result)
{
    try {
        if (mService == null) {
            return false;
        }
        return mService.launchAssistAction(requestType, hint, userHandle);
    } catch (RemoteException re) {
        Log.e(TAG, "launchAssistAction() failed: " + re);
        return false;
    }
}

void CSearchManager::EnsureSearchDialog()
{
    if (mSearchDialog == NULL) {
        SearchDialog::New(mContext, this, &mSearchDialog);
        mSearchDialog->SetOnCancelListener(this);
        mSearchDialog->SetOnDismissListener(this);
    }
}

void CSearchManager::StartGlobalSearch(
    /* [in] */ const String& initialQuery,
    /* [in] */ Boolean selectInitialQuery,
    /* [in] */ IBundle* appSearchData,
    /* [in] */ IRect* sourceBounds)
{
    AutoPtr<IComponentName> globalSearchActivity;
    GetGlobalSearchActivity(&globalSearchActivity);
    if (globalSearchActivity == NULL) {
        //Log.w(TAG, "No global search activity found.");
        return;
    }
    AutoPtr<IIntent> intent;
    CIntent::New(ISearchManager::INTENT_ACTION_GLOBAL_SEARCH, &intent);
    intent->AddFlags(IIntent::FLAG_ACTIVITY_NEW_TASK);
    intent->SetComponent(globalSearchActivity);
    // Make sure that we have a Bundle to put source in
    if (appSearchData == NULL) {
        CBundle::New(&appSearchData);
    } else {
        CBundle::New(&appSearchData, &appSearchData);
    }
    // Set source to package name of app that starts global search, if not set already.
    Boolean b;
    appSearchData->ContainsKey("source", &b);
    if (!b) {
        String name;
        mContext->GetPackageName(&name);
        appSearchData->PutString("source", name);
    }
    intent->PutExtra(ISearchManager::APP_DATA, appSearchData);
    if (!TextUtils::IsEmpty(initialQuery)) {
        intent->PutExtra(ISearchManager::QUERY, initialQuery);
    }
    if (selectInitialQuery) {
        intent->PutExtra(ISearchManager::EXTRA_SELECT_QUERY, selectInitialQuery);
    }
    intent->SetSourceBounds(sourceBounds);
    //try {
        //if (DBG) Log.d(TAG, "Starting global search: " + intent.toUri(0));
        mContext->StartActivity(intent);
    //} catch (ActivityNotFoundException ex) {
    //    Log.e(TAG, "Global search activity not found: " + globalSearchActivity);
    //}
}

}
}
}
