
#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/appwidget/CAppWidgetHostView.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos {
namespace Droid {
namespace AppWidget {

ECode CAppWidgetHostView::constructor(
    /* [in] */ IContext* context)
{
    return AppWidgetHostView::Init(context);
}

/**
 * @hide
 */
ECode CAppWidgetHostView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IRemoteViewsOnClickHandler* handler)
{
    return AppWidgetHostView::Init(context, handler);
}

ECode CAppWidgetHostView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 animationIn,
    /* [in] */ Int32 animationOut)
{
    return AppWidgetHostView::Init(context, animationIn, animationOut);
}

PInterface CAppWidgetHostView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CAppWidgetHostView::Probe(riid);
}

IVIEW_METHODS_IMPL(CAppWidgetHostView, AppWidgetHostView);
IVIEWGROUP_METHODS_IMPL(CAppWidgetHostView, AppWidgetHostView);
IVIEWPARENT_METHODS_IMPL(CAppWidgetHostView, AppWidgetHostView);
IVIEWMANAGER_METHODS_IMPL(CAppWidgetHostView, AppWidgetHostView);
IDRAWABLECALLBACK_METHODS_IMPL(CAppWidgetHostView, AppWidgetHostView);
IKEYEVENTCALLBACK_METHODS_IMPL(CAppWidgetHostView, AppWidgetHostView);
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CAppWidgetHostView, AppWidgetHostView);

ECode CAppWidgetHostView::SetUserId(
    /* [in] */ Int32 userId)
{
    return AppWidgetHostView::SetUserId(userId);
}

ECode CAppWidgetHostView::SetOnClickHandler(
    /* [in] */ IRemoteViewsOnClickHandler* handler)
{
    return AppWidgetHostView::SetOnClickHandler(handler);
}

ECode CAppWidgetHostView::SetAppWidget(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IAppWidgetProviderInfo* info)
{
    return AppWidgetHostView::SetAppWidget(appWidgetId, info);
}

ECode CAppWidgetHostView::GetAppWidgetId(
    /* [out] */ Int32* appWidgetId)
{
    return AppWidgetHostView::GetAppWidgetId(appWidgetId);
}

ECode CAppWidgetHostView::GetAppWidgetInfo(
    /* [out] */ IAppWidgetProviderInfo** info)
{
    return AppWidgetHostView::GetAppWidgetInfo(info);
}

ECode CAppWidgetHostView::UpdateAppWidgetSize(
    /* [in] */ IBundle* newOptions,
    /* [in] */ Int32 minWidth,
    /* [in] */ Int32 minHeight,
    /* [in] */ Int32 maxWidth,
    /* [in] */ Int32 maxHeight)
{
    return AppWidgetHostView::UpdateAppWidgetSize(newOptions, minWidth, minHeight, maxWidth, maxHeight);
}

ECode CAppWidgetHostView::UpdateAppWidgetSize(
    /* [in] */ IBundle* newOptions,
    /* [in] */ Int32 minWidth,
    /* [in] */ Int32 minHeight,
    /* [in] */ Int32 maxWidth,
    /* [in] */ Int32 maxHeight,
    /* [in] */ Boolean ignorePadding)
{
    return AppWidgetHostView::UpdateAppWidgetSize(newOptions, minWidth, minHeight, maxWidth, maxHeight, ignorePadding);
}

ECode CAppWidgetHostView::UpdateAppWidgetOptions(
    /* [in] */ IBundle* options)
{
    return AppWidgetHostView::UpdateAppWidgetOptions(options);
}

ECode CAppWidgetHostView::ResetAppWidget(
    /* [in] */ IAppWidgetProviderInfo* info)
{
    return AppWidgetHostView::ResetAppWidget(info);
}

ECode CAppWidgetHostView::UpdateAppWidget(
    /* [in] */ IRemoteViews* remoteViews)
{
    return AppWidgetHostView::UpdateAppWidget(remoteViews);
}

ECode CAppWidgetHostView::ViewDataChanged(
    /* [in] */ Int32 viewId)
{
    return AppWidgetHostView::ViewDataChanged(viewId);
}

ECode CAppWidgetHostView::SetForegroundGravity(
    /* [in] */ Int32 foregroundGravity)
{
    return AppWidgetHostView::SetForegroundGravity(foregroundGravity);
}

ECode CAppWidgetHostView::SetForeground(
    /* [in] */ IDrawable* drawable)
{
    return AppWidgetHostView::SetForeground(drawable);
}

ECode CAppWidgetHostView::GetForeground(
    /* [out] */ IDrawable** foreground)
{
    VALIDATE_NOT_NULL(foreground);
    AutoPtr<IDrawable> d = AppWidgetHostView::GetForeground();
    *foreground = d.Get();
    REFCOUNT_ADD(*foreground);
    return NOERROR;
}

ECode CAppWidgetHostView::SetMeasureAllChildren(
    /* [in] */ Boolean measureAll)
{
    return AppWidgetHostView::SetMeasureAllChildren(measureAll);
}

ECode CAppWidgetHostView::GetConsiderGoneChildrenWhenMeasuring(
    /* [out] */ Boolean* measureAll)
{
    VALIDATE_NOT_NULL(measureAll);
    *measureAll = FrameLayout::GetConsiderGoneChildrenWhenMeasuring();
    return NOERROR;
}

ECode CAppWidgetHostView::GetForegroundGravity(
    /* [out] */ Int32* foregroundGravity)
{
    assert(foregroundGravity != NULL);
    *foregroundGravity = AppWidgetHostView::GetForegroundGravity();
    return NOERROR;
}

ECode CAppWidgetHostView::GetMeasureAllChildren(
    /* [out] */ Boolean* measureAll)
{
    assert(measureAll != NULL);
    *measureAll = AppWidgetHostView::GetMeasureAllChildren();
    return NOERROR;
}

} // namespace AppWidget
} // namespace Droid
} // namespace Elastos
