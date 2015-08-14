
#ifndef __ELASTOS_DROID_APPWIDGET_CAPPWIDGETHOST_H__
#define  __ELASTOS_DROID_APPWIDGET_CAPPWIDGETHOST_H__

#include "_CAppWidgetHost.h"
#include "ext/frameworkext.h"
#include "os/HandlerBase.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;
using Elastos::Droid::AppWidget::IAppWidgetHostView;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Utility::IDisplayMetrics;
using Elastos::Droid::Widget::IRemoteViewsOnClickHandler;
using Elastos::Droid::Widget::IRemoteViews;

namespace Elastos {
namespace Droid {
namespace AppWidget {

/**
 * AppWidgetHost provides the interaction with the AppWidget service for apps,
 * like the home screen, that want to embed AppWidgets in their UI.
 */
CarClass(CAppWidgetHost)
{
private:
    class MyHandler : public HandlerBase
    {
    public:
        MyHandler(
            /* [in] */ ILooper* looper,
            /* [in] */ CAppWidgetHost* host)
            : HandlerBase(looper)
            , mHost(host)
        {}

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);
    private:
        CAppWidgetHost* mHost;
    };

public:
    CAppWidgetHost();

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ Int32 hostId);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ Int32 hostId,
        /* [in] */ IRemoteViewsOnClickHandler* handler,
        /* [in] */ ILooper* looper);

    /** @hide */
    CARAPI_(void) SetUserId(
        /* [in] */ Int32 userId);

    /**
     * Start receiving onAppWidgetChanged calls for your AppWidgets.  Call this when your activity
     * becomes visible, i.e. from onStart() in your Activity.
     */
    CARAPI StartListening();

    /**
     * Start receiving onAppWidgetChanged calls for your AppWidgets.  Call this when your activity
     * becomes visible, i.e. from onStart() in your Activity.
     * @hide
     */
    CARAPI StartListeningAsUser(
        /* [in] */ Int32 userId);

    /**
     * Stop receiving onAppWidgetChanged calls for your AppWidgets.  Call this when your activity is
     * no longer visible, i.e. from onStop() in your Activity.
     */
    CARAPI StopListening();

    /**
     * Stop receiving onAppWidgetChanged calls for your AppWidgets.  Call this when your activity is
     * no longer visible, i.e. from onStop() in your Activity.
     * @hide
     */
    CARAPI StopListeningAsUser(
        /* [in] */ Int32 userId);

    /**
     * Get a appWidgetId for a host in the calling process.
     *
     * @return a appWidgetId
     */
    CARAPI AllocateAppWidgetId(
        /* [out] */ Int32* appWidgetId);

    /**
     * Get a appWidgetId for a host in the calling process.
     *
     * @return a appWidgetId
     * @hide
     */
    static CARAPI AllocateAppWidgetIdForSystem(
        /* [in] */ Int32 hostId,
        /* [out] */ Int32* id);

    /**
     * Gets a list of all the appWidgetIds that are bound to the current host
     *
     * @hide
     */
    CARAPI GetAppWidgetIds(
        /* [out] */ ArrayOf<Int32>** ids);

    /**
     * Stop listening to changes for this AppWidget.
     */
    CARAPI DeleteAppWidgetId(
        /* [in] */ Int32 appWidgetId);

    /**
     * Stop listening to changes for this AppWidget.
     * @hide
     */
    static CARAPI DeleteAppWidgetIdForSystem(
        /* [in] */ Int32 appWidgetId);

    /**
     * Remove all records about this host from the AppWidget manager.
     * <ul>
     *   <li>Call this when initializing your database, as it might be because of a data wipe.</li>
     *   <li>Call this to have the AppWidget manager release all resources associated with your
     *   host.  Any future calls about this host will cause the records to be re-allocated.</li>
     * </ul>
     */
    CARAPI DeleteHost();

    /**
     * Remove all records about all hosts for your package.
     * <ul>
     *   <li>Call this when initializing your database, as it might be because of a data wipe.</li>
     *   <li>Call this to have the AppWidget manager release all resources associated with your
     *   host.  Any future calls about this host will cause the records to be re-allocated.</li>
     * </ul>
     */
    static CARAPI DeleteAllHosts();

    /**
     * Create the AppWidgetHostView for the given widget.
     * The AppWidgetHost retains a pointer to the newly-created View.
     */
    CARAPI CreateView(
        /* [in] */ IContext* context,
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IAppWidgetProviderInfo* appWidget,
        /* [out] */ IAppWidgetHostView** hostView);

    CARAPI UpdateAppWidgetView(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IRemoteViews* views);

    CARAPI ViewDataChanged(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ Int32 viewId);

protected:
    /**
     * Called to create the AppWidgetHostView.  Override to return a custom subclass if you
     * need it.  {@more}
     */
    CARAPI_(AutoPtr<IAppWidgetHostView>) OnCreateView(
        /* [in] */ IContext* context,
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IAppWidgetProviderInfo* appWidget);

    /**
     * Called when the AppWidget provider for a AppWidget has been upgraded to a new apk.
     */
    CARAPI_(void) OnProviderChanged(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IAppWidgetProviderInfo* appWidget);


    /**
     * Called when the set of available widgets changes (ie. widget containing packages
     * are added, updated or removed, or widget components are enabled or disabled.)
     */
    CARAPI_(void) OnProvidersChanged();

    /**
     * Clear the list of Views that have been created by this AppWidgetHost.
     */
    CARAPI_(void) ClearViews();

private:
    CARAPI Init(
        /* [in] */ IContext* ctx,
        /* [in] */ Int32 hostId,
        /* [in] */ IRemoteViewsOnClickHandler* handler,
        /* [in] */ ILooper* looper);

    static CARAPI_(void) BindService();

    static CARAPI CheckCallerIsSystem();

    CARAPI_(Boolean) IsLocalBinder();

protected:
    static const Int32 HANDLE_UPDATE = 1;
    static const Int32 HANDLE_PROVIDER_CHANGED = 2;
    static const Int32 HANDLE_PROVIDERS_CHANGED = 3;
    static const Int32 HANDLE_VIEW_DATA_CHANGED = 4;

    static Object sServiceLock;
    static AutoPtr<IIAppWidgetService> sService;
    AutoPtr<IDisplayMetrics> mDisplayMetrics;
    AutoPtr<IContext> mContext;
    String mPackageName;

    AutoPtr<IHandler> mHandler;

    Int32 mHostId;
    AutoPtr<IIAppWidgetHost> mCallbacks;
    HashMap<Int32, AutoPtr<IAppWidgetHostView> > mViews;
    Object mViewsLock;

private:
    AutoPtr<IRemoteViewsOnClickHandler> mOnClickHandler;
    // Optionally set by lockscreen
    AutoPtr<IUserHandle> mUser;
    friend class CAppWidgetHostCallbacks;
};

} // namespace AppWidget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APPWIDGET_CAPPWIDGETHOST_H__
