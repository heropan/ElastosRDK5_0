
#ifndef __CAPPWIDGETMANAGER_H__
#define __CAPPWIDGETMANAGER_H__

#include "ext/frameworkext.h"
#include "_CAppWidgetManager.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Widget::IRemoteViews;
using Elastos::Droid::Utility::IDisplayMetrics;

namespace Elastos {
namespace Droid {
namespace AppWidget {

CarClass(CAppWidgetManager)
{
public:
    CARAPI constructor(
        /* [in] */ IContext * context);

    /**
     * Get the AppWidgetManager instance to use for the supplied {@link android.content.Context
     * Context} object.
     */
    static CARAPI GetInstance(
        /* [in] */ IContext* context,
        /* [out] */ IAppWidgetManager** appWidgetManager);

    /**
     * Set the RemoteViews to use for the specified appWidgetIds.
     *
     * Note that the RemoteViews parameter will be cached by the AppWidgetService, and hence should
     * contain a complete representation of the widget. For performing partial widget updates, see
     * {@link #partiallyUpdateAppWidget(int[], RemoteViews)}.
     *
     * <p>
     * It is okay to call this method both inside an {@link #ACTION_APPWIDGET_UPDATE} broadcast,
     * and outside of the handler.
     * This method will only work when called from the uid that owns the AppWidget provider.
     *
     * <p>
     * The total Bitmap memory used by the RemoteViews object cannot exceed that required to
     * fill the screen 1.5 times, ie. (screen width x screen height x 4 x 1.5) bytes.
     *
     * @param appWidgetIds     The AppWidget instances for which to set the RemoteViews.
     * @param views         The RemoteViews object to show.
     */
    CARAPI UpdateAppWidget(
        /* [in] */ ArrayOf<Int32>* appWidgetIds,
        /* [in] */ IRemoteViews * views);

    /**
     * Update the extras for a given widget instance.
     *
     * The extras can be used to embed additional information about this widget to be accessed
     * by the associated widget's AppWidgetProvider.
     *
     * @see #getAppWidgetOptions(int)
     *
     * @param appWidgetId    The AppWidget instances for which to set the RemoteViews.
     * @param options         The options to associate with this widget
     */
    CARAPI UpdateAppWidgetOptions(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IBundle* options);

    /**
     * Get the extras associated with a given widget instance.
     *
     * The extras can be used to embed additional information about this widget to be accessed
     * by the associated widget's AppWidgetProvider.
     *
     * @see #updateAppWidgetOptions(int, Bundle)
     *
     * @param appWidgetId     The AppWidget instances for which to set the RemoteViews.
     * @return                The options associated with the given widget instance.
     */
    CARAPI GetAppWidgetOptions(
        /* [in] */ Int32 appWidgetId,
        /* [out] */ IBundle** options);

    /**
     * Set the RemoteViews to use for the specified appWidgetIds.
     *
     * Note that the RemoteViews parameter will be cached by the AppWidgetService, and hence should
     * contain a complete representation of the widget. For performing partial widget updates, see
     * {@link #partiallyUpdateAppWidget(int[], RemoteViews)}.
     *
     * <p>
     * It is okay to call this method both inside an {@link #ACTION_APPWIDGET_UPDATE} broadcast,
     * and outside of the handler.
     * This method will only work when called from the uid that owns the AppWidget provider.
     *
     * <p>
     * The total Bitmap memory used by the RemoteViews object cannot exceed that required to
     * fill the screen 1.5 times, ie. (screen width x screen height x 4 x 1.5) bytes.
     *
     * @param appWidgetIds     The AppWidget instances for which to set the RemoteViews.
     * @param views         The RemoteViews object to show.
     */
    CARAPI UpdateAppWidget(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IRemoteViews* views);

    /**
     * Perform an incremental update or command on the widget(s) specified by appWidgetIds.
     *
     * This update  differs from {@link #updateAppWidget(int[], RemoteViews)} in that the
     * RemoteViews object which is passed is understood to be an incomplete representation of the
     * widget, and hence does not replace the cached representation of the widget. As of API
     * level 17, the new properties set within the views objects will be appended to the cached
     * representation of the widget, and hence will persist.
     *
     * Use with {@link RemoteViews#showNext(int)}, {@link RemoteViews#showPrevious(int)},
     * {@link RemoteViews#setScrollPosition(int, int)} and similar commands.
     *
     * <p>
     * It is okay to call this method both inside an {@link #ACTION_APPWIDGET_UPDATE} broadcast,
     * and outside of the handler.
     * This method will only work when called from the uid that owns the AppWidget provider.
     *
     * <p>
     * This method will be ignored if a widget has not received a full update via
     * {@link #updateAppWidget(int[], RemoteViews)}.
     *
     * @param appWidgetIds     The AppWidget instances for which to set the RemoteViews.
     * @param views            The RemoteViews object containing the incremental update / command.
     */
    CARAPI PartiallyUpdateAppWidget(
        /* [in] */ ArrayOf<Int32>* appWidgetIds,
        /* [in] */ IRemoteViews* views);

    /**
     * Perform an incremental update or command on the widget specified by appWidgetId.
     *
     * This update  differs from {@link #updateAppWidget(int, RemoteViews)} in that the RemoteViews
     * object which is passed is understood to be an incomplete representation of the widget, and
     * hence is not cached by the AppWidgetService. Note that because these updates are not cached,
     * any state that they modify that is not restored by restoreInstanceState will not persist in
     * the case that the widgets are restored using the cached version in AppWidgetService.
     *
     * Use with {@link RemoteViews#showNext(int)}, {@link RemoteViews#showPrevious(int)},
     * {@link RemoteViews#setScrollPosition(int, int)} and similar commands.
     *
     * <p>
     * It is okay to call this method both inside an {@link #ACTION_APPWIDGET_UPDATE} broadcast,
     * and outside of the handler.
     * This method will only work when called from the uid that owns the AppWidget provider.
     *
     * <p>
     * This method will be ignored if a widget has not received a full update via
     * {@link #updateAppWidget(int[], RemoteViews)}.
     *
     * @param appWidgetId      The AppWidget instance for which to set the RemoteViews.
     * @param views            The RemoteViews object containing the incremental update / command.
     */
    CARAPI PartiallyUpdateAppWidget(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IRemoteViews* views);

    /**
     * Set the RemoteViews to use for the specified appWidgetId.
     *
     * Note that the RemoteViews parameter will be cached by the AppWidgetService, and hence should
     * contain a complete representation of the widget. For performing partial widget updates, see
     * {@link #partiallyUpdateAppWidget(int, RemoteViews)}.
     *
     * <p>
     * It is okay to call this method both inside an {@link #ACTION_APPWIDGET_UPDATE} broadcast,
     * and outside of the handler.
     * This method will only work when called from the uid that owns the AppWidget provider.
     *
     * <p>
     * The total Bitmap memory used by the RemoteViews object cannot exceed that required to
     * fill the screen 1.5 times, ie. (screen width x screen height x 4 x 1.5) bytes.
     *
     * @param appWidgetId      The AppWidget instance for which to set the RemoteViews.
     * @param views         The RemoteViews object to show.
     */
    CARAPI UpdateAppWidget(
        /* [in] */ IComponentName* provider,
        /* [in] */ IRemoteViews* views);

    /**
     * Notifies the specified collection view in all the specified AppWidget instances
     * to invalidate their currently data.
     *
     * @param appWidgetIds  The AppWidget instances for which to notify of view data changes.
     * @param viewId        The collection view id.
     */
    CARAPI NotifyAppWidgetViewDataChanged(
        /* [in] */ ArrayOf<Int32>* appWidgetIds,
        /* [in] */ Int32 viewId);

    /**
     * Notifies the specified collection view in all the specified AppWidget instance
     * to invalidate it's currently data.
     *
     * @param appWidgetId  The AppWidget instance for which to notify of view data changes.
     * @param viewId        The collection view id.
     */
    CARAPI NotifyAppWidgetViewDataChanged(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ Int32 viewId);

    /**
     * Return a list of the AppWidget providers that are currently installed.
     */
    CARAPI GetInstalledProviders(
        /* [out] */ IObjectContainer** providerInfos);

    /**
     * Return a list of the AppWidget providers that are currently installed.
     *
     * @param categoryFilter Will only return providers which register as any of the specified
     *        specified categories. See {@link AppWidgetProviderInfo#widgetCategory}.
     * @hide
     */
    CARAPI GetInstalledProviders(
        /* [in] */ Int32 categoryFilter,
        /* [out] */ IObjectContainer** providerInfos);

    /**
     * Get the available info about the AppWidget.
     *
     * @return A appWidgetId.  If the appWidgetId has not been bound to a provider yet, or
     * you don't have access to that appWidgetId, null is returned.
     */
    CARAPI GetAppWidgetInfo(
        /* [in] */ Int32 appWidgetId,
        /* [out] */ IAppWidgetProviderInfo** info);

    /**
     * Set the component for a given appWidgetId.
     *
     * <p class="note">You need the BIND_APPWIDGET permission or the user must have enabled binding
     *         widgets always for your component. This method is used by the AppWidget picker and
     *         should not be used by other apps.
     *
     * @param appWidgetId     The AppWidget instance for which to set the RemoteViews.
     * @param provider      The {@link android.content.BroadcastReceiver} that will be the AppWidget
     *                      provider for this AppWidget.
     * @hide
     */
    CARAPI BindAppWidgetId(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IComponentName* provider);

    /**
     * Set the component for a given appWidgetId.
     *
     * <p class="note">You need the BIND_APPWIDGET permission or the user must have enabled binding
     *         widgets always for your component. This method is used by the AppWidget picker and
     *         should not be used by other apps.
     *
     * @param appWidgetId     The AppWidget instance for which to set the RemoteViews.
     * @param provider      The {@link android.content.BroadcastReceiver} that will be the AppWidget
     *                      provider for this AppWidget.
     * @param options       Bundle containing options for the AppWidget. See also
     *                      {@link #updateAppWidgetOptions(int, Bundle)}
     *
     * @hide
     */
    CARAPI BindAppWidgetId(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IComponentName* provider,
        /* [in] */ IBundle* options);

    /**
     * Set the component for a given appWidgetId.
     *
     * <p class="note">You need the BIND_APPWIDGET permission or the user must have enabled binding
     *         widgets always for your component. Should be used by apps that host widgets; if this
     *         method returns false, call {@link #ACTION_APPWIDGET_BIND} to request permission to
     *         bind
     *
     * @param appWidgetId     The AppWidget instance for which to set the RemoteViews.
     * @param provider      The {@link android.content.BroadcastReceiver} that will be the AppWidget
     *                      provider for this AppWidget.
     * @return true if this component has permission to bind the AppWidget
     */
    CARAPI BindAppWidgetIdIfAllowed(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IComponentName* provider,
        /* [out] */ Boolean* result);

    /**
     * Set the component for a given appWidgetId.
     *
     * <p class="note">You need the BIND_APPWIDGET permission or the user must have enabled binding
     *         widgets always for your component. Should be used by apps that host widgets; if this
     *         method returns false, call {@link #ACTION_APPWIDGET_BIND} to request permission to
     *         bind
     *
     * @param appWidgetId     The AppWidget instance for which to set the RemoteViews.
     * @param provider      The {@link android.content.BroadcastReceiver} that will be the AppWidget
     *                      provider for this AppWidget.
     * @param options       Bundle containing options for the AppWidget. See also
     *                      {@link #updateAppWidgetOptions(int, Bundle)}
     *
     * @return true if this component has permission to bind the AppWidget
     */
    CARAPI BindAppWidgetIdIfAllowed(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IComponentName* provider,
        /* [in] */ IBundle* options,
        /* [out] */ Boolean* result);

    /**
     * Query if a given package was granted permission by the user to bind app widgets
     *
     * <p class="note">You need the MODIFY_APPWIDGET_BIND_PERMISSIONS permission
     *
     * @param packageName        The package for which the permission is being queried
     * @return true if the package was granted permission by the user to bind app widgets
     * @hide
     */
    CARAPI HasBindAppWidgetPermission(
        /* [in] */ const String& packageName,
        /* [out] */ Boolean* result);

    /**
     * Changes any user-granted permission for the given package to bind app widgets
     *
     * <p class="note">You need the MODIFY_APPWIDGET_BIND_PERMISSIONS permission
     *
     * @param provider        The package whose permission is being changed
     * @param permission      Whether to give the package permission to bind widgets
     * @hide
     */
    CARAPI SetBindAppWidgetPermission(
        /* [in] */ const String& packageName,
        /* [in] */ Boolean permission);

    /**
     * Binds the RemoteViewsService for a given appWidgetId and intent.
     *
     * The appWidgetId specified must already be bound to the calling AppWidgetHost via
     * {@link android.appwidget.AppWidgetManager#bindAppWidgetId AppWidgetManager.bindAppWidgetId()}.
     *
     * @param appWidgetId   The AppWidget instance for which to bind the RemoteViewsService.
     * @param intent        The intent of the service which will be providing the data to the
     *                      RemoteViewsAdapter.
     * @param connection    The callback interface to be notified when a connection is made or lost.
     * @hide
     */
    CARAPI BindRemoteViewsService(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IIntent* intent,
        /* [in] */ IBinder* connection,
        /* [in] */ IUserHandle* userHandle);

    /**
     * Unbinds the RemoteViewsService for a given appWidgetId and intent.
     *
     * The appWidgetId specified muse already be bound to the calling AppWidgetHost via
     * {@link android.appwidget.AppWidgetManager#bindAppWidgetId AppWidgetManager.bindAppWidgetId()}.
     *
     * @param appWidgetId   The AppWidget instance for which to bind the RemoteViewsService.
     * @param intent        The intent of the service which will be providing the data to the
     *                      RemoteViewsAdapter.
     * @hide
     */
    CARAPI UnbindRemoteViewsService(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* userHandle);

    /**
     * Get the list of appWidgetIds that have been bound to the given AppWidget
     * provider.
     *
     * @param provider The {@link android.content.BroadcastReceiver} that is the
     *            AppWidget provider to find appWidgetIds for.
     */
    CARAPI GetAppWidgetIds(
        /* [in] */ IComponentName* provider,
        /* [out, callee] */ ArrayOf<Int32>** appWidgetIds);

public:
    static const String TAG;

    static Object sManagerCacheLock;
    // WeakHashMap<Context, WeakReference<AppWidgetManager> >
    typedef HashMap<AutoPtr<IContext>, AutoPtr<IWeakReference> > CacheMap;
    typedef typename HashMap<AutoPtr<IContext>, AutoPtr<IWeakReference> >::Iterator CacheMapIterator;
    static CacheMap sManagerCache;
    static AutoPtr<IIAppWidgetService> sService;

    AutoPtr<IContext> mContext;

private:
    AutoPtr<IDisplayMetrics> mDisplayMetrics;
};

} // namespace AppWidget
} // namespace Droid
} // namespace Elastos

#endif //__CAPPWIDGETMANAGER_H__
