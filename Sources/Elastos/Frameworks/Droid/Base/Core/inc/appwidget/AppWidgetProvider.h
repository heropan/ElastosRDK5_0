
#ifndef __APPWIDGETPROVIDER_H__
#define __APPWIDGETPROVIDER_H__

#include "ext/frameworkext.h"
#include "content/BroadcastReceiver.h"

using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;

namespace Elastos {
namespace Droid {
namespace AppWidget {

/**
 * A convenience class to aid in implementing an AppWidget provider.
 * Everything you can do with AppWidgetProvider, you can do with a regular {@link BroadcastReceiver}.
 * AppWidgetProvider merely parses the relevant fields out of the Intent that is received in
 * {@link #onReceive(Context,Intent) onReceive(Context,Intent)}, and calls hook methods
 * with the received extras.
 *
 * <p>Extend this class and override one or more of the {@link #onUpdate}, {@link #onDeleted},
 * {@link #onEnabled} or {@link #onDisabled} methods to implement your own AppWidget functionality.
 * </p>
 *
 * <div class="special reference">
 * <h3>Developer Guides</h3>
 * <p>For more information about how to write an app widget provider, read the
 * <a href="{@docRoot}guide/topics/appwidgets/index.html#AppWidgetProvider">App Widgets</a>
 * developer guide.</p>
 * </div>
 */
class AppWidgetProvider: public BroadcastReceiver
{
public:
    /**
     * Constructor to initialize AppWidgetProvider.
     */
    AppWidgetProvider();

    /**
     * Implements {@link BroadcastReceiver#onReceive} to dispatch calls to the various
     * other methods on AppWidgetProvider.
     *
     * @param context The Context in which the receiver is running.
     * @param intent The Intent being received.
     */
    // BEGIN_INCLUDE(onReceive)
    virtual CARAPI OnReceive(
        /* [in] */ IContext* context,
        /* [in] */ IIntent* intent);

    CARAPI ToString(
        /* [out] */ String* info)
    {
        VALIDATE_NOT_NULL(info);
        *info = String("AppWidgetProvider:");
        (*info).AppendFormat("%p", this);
        return NOERROR;
    }

    /**
     * Called in response to the {@link AppWidgetManager#ACTION_APPWIDGET_UPDATE} broadcast when
     * this AppWidget provider is being asked to provide {@link android.widget.RemoteViews RemoteViews}
     * for a set of AppWidgets.  Override this method to implement your own AppWidget functionality.
     *
     * {@more}
     *
     * @param context   The {@link android.content.Context Context} in which this receiver is
     *                  running.
     * @param appWidgetManager A {@link AppWidgetManager} object you can call {@link
     *                  AppWidgetManager#updateAppWidget} on.
     * @param appWidgetIds The appWidgetIds for which an update is needed.  Note that this
     *                  may be all of the AppWidget instances for this provider, or just
     *                  a subset of them.
     *
     * @see AppWidgetManager#ACTION_APPWIDGET_UPDATE
     */
    virtual CARAPI OnUpdate(
        /* [in] */ IContext* context,
        /* [in] */ IAppWidgetManager* appWidgetManager,
        /* [in] */ const ArrayOf<Int32>& appWidgetIds);

    /**
     * Called in response to the {@link AppWidgetManager#ACTION_APPWIDGET_OPTIONS_CHANGED}
     * broadcast when this widget has been layed out at a new size.
     *
     * {@more}
     *
     * @param context   The {@link android.content.Context Context} in which this receiver is
     *                  running.
     * @param appWidgetManager A {@link AppWidgetManager} object you can call {@link
     *                  AppWidgetManager#updateAppWidget} on.
     * @param appWidgetId The appWidgetId of the widget who's size changed.
     * @param newOptions The appWidgetId of the widget who's size changed.
     *
     * @see AppWidgetManager#ACTION_APPWIDGET_OPTIONS_CHANGED
     */
    virtual CARAPI OnAppWidgetOptionsChanged(
        /* [in] */ IContext* context,
        /* [in] */ IAppWidgetManager* appWidgetManager,
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IBundle* newOptions);

    /**
     * Called in response to the {@link AppWidgetManager#ACTION_APPWIDGET_DELETED} broadcast when
     * one or more AppWidget instances have been deleted.  Override this method to implement
     * your own AppWidget functionality.
     *
     * {@more}
     *
     * @param context   The {@link android.content.Context Context} in which this receiver is
     *                  running.
     * @param appWidgetIds The appWidgetIds that have been deleted from their host.
     *
     * @see AppWidgetManager#ACTION_APPWIDGET_DELETED
     */
    virtual CARAPI OnDeleted(
        /* [in] */ IContext* context,
        /* [in] */ const ArrayOf<Int32>& appWidgetIds);

    /**
     * Called in response to the {@link AppWidgetManager#ACTION_APPWIDGET_ENABLED} broadcast when
     * the a AppWidget for this provider is instantiated.  Override this method to implement your
     * own AppWidget functionality.
     *
     * {@more}
     * When the last AppWidget for this provider is deleted,
     * {@link AppWidgetManager#ACTION_APPWIDGET_DISABLED} is sent by the AppWidget manager, and
     * {@link #onDisabled} is called.  If after that, an AppWidget for this provider is created
     * again, onEnabled() will be called again.
     *
     * @param context   The {@link android.content.Context Context} in which this receiver is
     *                  running.
     *
     * @see AppWidgetManager#ACTION_APPWIDGET_ENABLED
     */
    virtual CARAPI OnEnabled(
        /* [in] */ IContext* context);

    /**
     * Called in response to the {@link AppWidgetManager#ACTION_APPWIDGET_DISABLED} broadcast, which
     * is sent when the last AppWidget instance for this provider is deleted.  Override this method
     * to implement your own AppWidget functionality.
     *
     * {@more}
     *
     * @param context   The {@link android.content.Context Context} in which this receiver is
     *                  running.
     *
     * @see AppWidgetManager#ACTION_APPWIDGET_DISABLED
     */
    virtual CARAPI OnDisabled(
        /* [in] */ IContext* context);
};

} // namespace AppWidget
} // namespace Droid
} // namespace Elastos
#endif //__APPWIDGETPROVIDER_H__
