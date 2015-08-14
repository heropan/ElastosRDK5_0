
#ifndef __ELASTOS_DROID_APPWIDGET_CAPPWIDGETHOSTVIEW_H__
#define __ELASTOS_DROID_APPWIDGET_CAPPWIDGETHOSTVIEW_H__

#include "_CAppWidgetHostView.h"
#include "appwidget/AppWidgetHostView.h"


namespace Elastos {
namespace Droid {
namespace AppWidget {

CarClass(CAppWidgetHostView), public AppWidgetHostView
{
public:
    /**
     * Create a host view.  Uses default fade animations.
     */
    CARAPI constructor(
        /* [in] */ IContext* context);

    /**
     * @hide
     */
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IRemoteViewsOnClickHandler* handler);

    /**
     * Create a host view. Uses specified animations when pushing
     * {@link #updateAppWidget(RemoteViews)}.
     *
     * @param animationIn Resource ID of in animation to use
     * @param animationOut Resource ID of out animation to use
     */
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 animationIn,
        /* [in] */ Int32 animationOut);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI SetUserId(
        /* [in] */ Int32 userId);

    /**
     * Pass the given handler to RemoteViews when updating this widget. Unless this
     * is done immediatly after construction, a call to {@link #updateAppWidget(RemoteViews)}
     * should be made.
     * @param handler
     * @hide
     */
    CARAPI SetOnClickHandler(
        /* [in] */ IRemoteViewsOnClickHandler* handler);

    /**
     * Set the AppWidget that will be displayed by this view.
     */
    CARAPI SetAppWidget(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IAppWidgetProviderInfo* info);

    CARAPI GetAppWidgetId(
        /* [out] */ Int32* appWidgetId);

    CARAPI GetAppWidgetInfo(
        /* [out] */ IAppWidgetProviderInfo** info);

    /**
     * Provide guidance about the size of this widget to the AppWidgetManager. The widths and
     * heights should correspond to the full area the AppWidgetHostView is given. Padding added by
     * the framework will be accounted for automatically. This information gets embedded into the
     * AppWidget options and causes a callback to the AppWidgetProvider.
     * @see AppWidgetProvider#onAppWidgetOptionsChanged(Context, AppWidgetManager, int, Bundle)
     *
     * @param newOptions The bundle of options, in addition to the size information,
     *          can be null.
     * @param minWidth The minimum width in dips that the widget will be displayed at.
     * @param minHeight The maximum height in dips that the widget will be displayed at.
     * @param maxWidth The maximum width in dips that the widget will be displayed at.
     * @param maxHeight The maximum height in dips that the widget will be displayed at.
     *
     */
    CARAPI UpdateAppWidgetSize(
        /* [in] */ IBundle* newOptions,
        /* [in] */ Int32 minWidth,
        /* [in] */ Int32 minHeight,
        /* [in] */ Int32 maxWidth,
        /* [in] */ Int32 maxHeight);

    /**
     * @hide
     */
    CARAPI UpdateAppWidgetSize(
        /* [in] */ IBundle* newOptions,
        /* [in] */ Int32 minWidth,
        /* [in] */ Int32 minHeight,
        /* [in] */ Int32 maxWidth,
        /* [in] */ Int32 maxHeight,
        /* [in] */ Boolean ignorePadding);

    /**
     * Specify some extra information for the widget provider. Causes a callback to the
     * AppWidgetProvider.
     * @see AppWidgetProvider#onAppWidgetOptionsChanged(Context, AppWidgetManager, int, Bundle)
     *
     * @param options The bundle of options information.
     */
    CARAPI UpdateAppWidgetOptions(
        /* [in] */ IBundle* options);

    /**
     * Update the AppWidgetProviderInfo for this view, and reset it to the
     * initial layout.
     */
    CARAPI ResetAppWidget(
        /* [in] */ IAppWidgetProviderInfo* info);

    /**
     * Process a set of {@link RemoteViews} coming in as an update from the
     * AppWidget provider. Will animate into these new views as needed
     */
    CARAPI UpdateAppWidget(
        /* [in] */ IRemoteViews* remoteViews);

    CARAPI ViewDataChanged(
        /* [in] */ Int32 viewId);

    CARAPI SetForegroundGravity(
        /* [in] */ Int32 foregroundGravity);

    CARAPI SetForeground(
        /* [in] */ IDrawable* drawable);

    CARAPI GetForeground(
        /* [out] */ IDrawable** foreground);

    CARAPI SetMeasureAllChildren(
        /* [in] */ Boolean measureAll);

    CARAPI GetConsiderGoneChildrenWhenMeasuring(
        /* [out] */ Boolean* measureAll);

    CARAPI GetForegroundGravity(
        /* [out] */ Int32* foregroundGravity);

    CARAPI GetMeasureAllChildren(
        /* [out] */ Boolean* measureAll);

private:
    friend class CAppWidgetHost;
};

} // namespace AppWidget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APPWIDGET_CAPPWIDGETHOSTVIEW_H__
