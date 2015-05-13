#include "CSearchManager.h"

namespace Elastos{
namespace Droid{
namespace App{

CSearchManager::CSearchManager()
    : mAssociatedPackage(NULL)
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

/**
 * Launch search UI.
 *
 * <p>The search manager will open a search widget in an overlapping
 * window, and the underlying activity may be obscured.  The search
 * entry state will remain in effect until one of the following events:
 * <ul>
 * <li>The user completes the search.  In most cases this will launch
 * a search intent.</li>
 * <li>The user uses the back, home, or other keys to exit the search.</li>
 * <li>The application calls the {@link #stopSearch}
 * method, which will hide the search window and return focus to the
 * activity from which it was launched.</li>
 *
 * <p>Most applications will <i>not</i> use this interface to invoke search.
 * The primary method for invoking search is to call
 * {@link android.app.Activity#onSearchRequested Activity.onSearchRequested()} or
 * {@link android.app.Activity#startSearch Activity.startSearch()}.
 *
 * @param initialQuery A search string can be pre-entered here, but this
 * is typically null or empty.
 * @param selectInitialQuery If true, the intial query will be preselected, which means that
 * any further typing will replace it.  This is useful for cases where an entire pre-formed
 * query is being inserted.  If false, the selection point will be placed at the end of the
 * inserted query.  This is useful when the inserted query is text that the user entered,
 * and the user would expect to be able to keep typing.  <i>This parameter is only meaningful
 * if initialQuery is a non-empty string.</i>
 * @param launchActivity The ComponentName of the activity that has launched this search.
 * @param appSearchData An application can insert application-specific
 * context here, in order to improve quality or specificity of its own
 * searches.  This data will be returned with SEARCH intent(s).  Null if
 * no extra data is required.
 * @param globalSearch If false, this will only launch the search that has been specifically
 * defined by the application (which is usually defined as a local search).  If no default
 * search is defined in the current application or activity, global search will be launched.
 * If true, this will always launch a platform-global (e.g. web-based) search instead.
 *
 * @see android.app.Activity#onSearchRequested
 * @see #stopSearch
 */
ECode CSearchManager::StartSearch(
    /* [in] */ const String &initialQuery,
    /* [in] */ Boolean selectInitialQuery,
    /* [in] */ IComponentName *launchActivity,
    /* [in] */ IBundle *appSearchData,
    /* [in] */ Boolean globalSearch)
{
    return StartSearchEx(initialQuery, selectInitialQuery, launchActivity,
            appSearchData, globalSearch, NULL);
}

/**
 * As {@link #startSearch(String, boolean, ComponentName, Bundle, boolean)} but including
 * source bounds for the global search intent.
 *
 * @hide
 */
ECode CSearchManager::StartSearchEx(
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

    EnsureSearchDialog();

    // mSearchDialog.show(initialQuery, selectInitialQuery, launchActivity, appSearchData);
    return NOERROR;
}

/**
 * Returns a list of installed apps that handle the global search
 * intent.
 *
 * @hide
 */
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

/**
 * Gets the name of the global search activity.
 */
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

/**
 * Gets the name of the web search activity.
 *
 * @return The name of the default activity for web searches. This activity
 *         can be used to get web search suggestions. Returns {@code null} if
 *         there is no default web search activity.
 *
 * @hide
 */
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

/**
 * Similar to {@link #startSearch} but actually fires off the search query after invoking
 * the search dialog.  Made available for testing purposes.
 *
 * @param query The query to trigger.  If empty, request will be ignored.
 * @param launchActivity The ComponentName of the activity that has launched this search.
 * @param appSearchData An application can insert application-specific
 * context here, in order to improve quality or specificity of its own
 * searches.  This data will be returned with SEARCH intent(s).  Null if
 * no extra data is required.
 *
 * @see #startSearch
 */
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

/**
 * Terminate search UI.
 *
 * <p>Typically the user will terminate the search UI by launching a
 * search or by canceling.  This function allows the underlying application
 * or activity to cancel the search prematurely (for any reason).
 *
 * <p>This function can be safely called at any time (even if no search is active.)
 *
 * @see #startSearch
 */
ECode CSearchManager::StopSearch()
{
    if (mSearchDialog != NULL) {
        mSearchDialog->Cancel();
    }
    return NOERROR;
}

/**
 * Determine if the Search UI is currently displayed.
 *
 * This is provided primarily for application test purposes.
 *
 * @return Returns true if the search UI is currently displayed.
 *
 * @hide
 */
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

/**
 * Set or clear the callback that will be invoked whenever the search UI is dismissed.
 *
 * @param listener The {@link OnDismissListener} to use, or null.
 */
ECode CSearchManager::SetOnDismissListener(
    /* [in] */ ISearchManagerOnDismissListener *listener)
{
    mDismissListener = listener;
    return NOERROR;
}

/**
 * Set or clear the callback that will be invoked whenever the search UI is canceled.
 *
 * @param listener The {@link OnCancelListener} to use, or null.
 */
ECode CSearchManager::SetOnCancelListener(
    /* [in] */ ISearchManagerOnCancelListener *listener)
{
    mCancelListener = listener;
    return NOERROR;
}

/**
 * Gets information about a searchable activity.
 *
 * @param componentName The activity to get searchable information for.
 * @return Searchable information, or <code>null</code> if the activity does not
 *         exist, or is not searchable.
 */
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

/**
 * Gets a cursor with search suggestions.
 *
 * @param searchable Information about how to get the suggestions.
 * @param query The search text entered (so far).
 * @return a cursor with suggestions, or <code>null</null> the suggestion query failed.
 *
 * @hide because SearchableInfo is not part of the API.
 */
ECode CSearchManager::GetSuggestions(
    /* [in] */ ISearchableInfo *searchable,
    /* [in] */ const String &query,
    /* [out] */ ICursor **cursor)
{

    return GetSuggestionsEx(searchable, query, -1, cursor);
}

/**
 * Gets a cursor with search suggestions.
 *
 * @param searchable Information about how to get the suggestions.
 * @param query The search text entered (so far).
 * @param limit The query limit to pass to the suggestion provider. This is advisory,
 *        the returned cursor may contain more rows. Pass {@code -1} for no limit.
 * @return a cursor with suggestions, or <code>null</null> the suggestion query failed.
 *
 * @hide because SearchableInfo is not part of the API.
 */
ECode CSearchManager::GetSuggestionsEx(
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

/**
 * Returns a list of the searchable activities that can be included in global search.
 *
 * @return a list containing searchable information for all searchable activities
 *         that have the <code>android:includeInGlobalSearch</code> attribute set
 *         in their searchable meta-data.
 */
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

/**
 * Gets an intent for launching installed assistant activity, or null if not available.
 * @return The assist intent.
 *
 * @hide
 */
ECode CSearchManager::GetAssistIntent(
    /* [in] */ IContext *context,
    /* [out] */ IIntent **intent)
{
    return GetAssistIntentEx(context, UserHandle::MyUserId(), intent);
}

/**
 * Gets an intent for launching installed assistant activity, or null if not available.
 * @return The assist intent.
 *
 * @hide
 */
ECode CSearchManager::GetAssistIntentEx(
    /* [in] */ IContext *context,
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
    *intent = pIntent;
    INTERFACE_ADDREF(*intent);
    //} catch (RemoteException re) {
    //    Log.e(TAG, "getAssistIntent() failed: " + re);
    //    return null;
    //}
    return NOERROR;
}

void EnsureSearchDialog()
{
    if (mSearchDialog == NULL) {
        SearchDialog::New(mContext, this, &mSearchDialog);
        mSearchDialog->SetOnCancelListener(this);
        mSearchDialog->SetOnDismissListener(this);
    }
}

void StartGlobalSearch(
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
