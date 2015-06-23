#include "widget/CRemoteViews.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Widget {

PInterface CRemoteViews::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_RemoteViews) {
        return reinterpret_cast<PInterface>((RemoteViews*)this);
    }
    return _CRemoteViews::Probe(riid);
}

ECode CRemoteViews::constructor()
{
    return NOERROR;
}

ECode CRemoteViews::constructor(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 layoutId)
{
    return RemoteViews::Init(packageName, layoutId);
}

ECode CRemoteViews::constructor(
    /* [in] */ IRemoteViews* landscape,
    /* [in] */ IRemoteViews* portrait)
{
    return RemoteViews::Init(landscape, portrait);
}

ECode CRemoteViews::MergeRemoteViews(
    /* [in] */ IRemoteViews* newRv)
{
    return RemoteViews::MergeRemoteViews(newRv);
}

ECode CRemoteViews::SetUser(
    /* [in] */ IUserHandle* user)
{
    return RemoteViews::SetUser(user);;
}

ECode CRemoteViews::Clone(
    /* [out] */ IRemoteViews** views)
{
    VALIDATE_NOT_NULL(views)
    AutoPtr<IRemoteViews> remote;
    remote = RemoteViews::Clone();
    *views = remote;
    INTERFACE_ADDREF(*views);
    return NOERROR;
}

ECode CRemoteViews::GetPackage(
    /* [out] */ String* package)
{
    VALIDATE_NOT_NULL(package);
    *package = RemoteViews::GetPackage();
    return NOERROR;
}

ECode CRemoteViews::GetLayoutId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = RemoteViews::GetLayoutId();
    return NOERROR;
}

ECode CRemoteViews::EstimateMemoryUsage(
    /* [out] */ Int32* usage)
{
    VALIDATE_NOT_NULL(usage);
    *usage = RemoteViews::EstimateMemoryUsage();
    return NOERROR;
}

ECode CRemoteViews::AddView(
    /* [in] */ Int32 viewId,
    /* [in] */ IRemoteViews* nestedView)
{
    return RemoteViews::AddView(viewId, nestedView);
}

ECode CRemoteViews::RemoveAllViews(
    /* [in] */ Int32 viewId)
{
    return RemoteViews::RemoveAllViews(viewId);
}

ECode CRemoteViews::ShowNext(
    /* [in] */ Int32 viewId)
{
    return RemoteViews::ShowNext(viewId);
}

ECode CRemoteViews::ShowPrevious(
    /* [in] */ Int32 viewId)
{
    return RemoteViews::ShowPrevious(viewId);
}

ECode CRemoteViews::SetDisplayedChild(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 childIndex)
{
    return RemoteViews::SetDisplayedChild(viewId, childIndex);
}

ECode CRemoteViews::SetViewVisibility(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 visibility)
{
    return RemoteViews::SetViewVisibility(viewId, visibility);
}

ECode CRemoteViews::SetTextViewText(
    /* [in] */ Int32 viewId,
    /* [in] */ ICharSequence* text)
{
    return RemoteViews::SetTextViewText(viewId, text);
}

ECode CRemoteViews::SetTextViewTextSize(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 units,
    /* [in] */ Float size)
{
    return RemoteViews::SetTextViewTextSize(viewId, units, size);
}

ECode CRemoteViews::SetTextViewCompoundDrawables(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    return RemoteViews::SetTextViewCompoundDrawables(viewId, left, top, right, bottom);
}

ECode CRemoteViews::SetTextViewCompoundDrawablesRelative(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 start,
    /* [in] */ Int32 top,
    /* [in] */ Int32 end,
    /* [in] */ Int32 bottom)
{
    return RemoteViews::SetTextViewCompoundDrawablesRelative(viewId, start, top, end, bottom);
}

ECode CRemoteViews::SetImageViewResource(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 srcId)
{
    return RemoteViews::SetImageViewResource(viewId, srcId);
}

ECode CRemoteViews::SetImageViewUri(
    /* [in] */ Int32 viewId,
    /* [in] */ IUri* uri)
{
    return RemoteViews::SetImageViewUri(viewId, uri);
}

ECode CRemoteViews::SetImageViewBitmap(
    /* [in] */ Int32 viewId,
    /* [in] */ IBitmap* bitmap)
{
    return RemoteViews::SetImageViewBitmap(viewId, bitmap);
}

ECode CRemoteViews::SetEmptyView(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 emptyViewId)
{
    return RemoteViews::SetEmptyView(viewId, emptyViewId);
}

ECode CRemoteViews::SetChronometer(
    /* [in] */ Int32 viewId,
    /* [in] */ Int64 base,
    /* [in] */ const String& format,
    /* [in] */ Boolean started)
{
    return RemoteViews::SetChronometer(viewId, base, format, started);
}

ECode CRemoteViews::SetProgressBar(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 max,
    /* [in] */ Int32 progress,
    /* [in] */ Boolean indeterminate)
{
    return RemoteViews::SetProgressBar(viewId, max, progress, indeterminate);
}

ECode CRemoteViews::SetOnClickPendingIntent(
    /* [in] */ Int32 viewId,
    /* [in] */ IPendingIntent* pendingIntent)
{
    return RemoteViews::SetOnClickPendingIntent(viewId, pendingIntent);
}

ECode CRemoteViews::SetPendingIntentTemplate(
    /* [in] */ Int32 viewId,
    /* [in] */ IPendingIntent* pendingIntentTemplate)
{
    return RemoteViews::SetPendingIntentTemplate(viewId, pendingIntentTemplate);
}

ECode CRemoteViews::SetOnClickFillInIntent(
    /* [in] */ Int32 viewId,
    /* [in] */ IIntent* fillInIntent)
{
    return RemoteViews::SetOnClickFillInIntent(viewId, fillInIntent);
}

ECode CRemoteViews::SetDrawableParameters(
    /* [in] */ Int32 viewId,
    /* [in] */ Boolean targetBackground,
    /* [in] */ Int32 alpha,
    /* [in] */ Int32 colorFilter,
    /* [in] */ PorterDuffMode mode,
    /* [in] */ Int32 level)
{
    return RemoteViews::SetDrawableParameters(viewId, targetBackground, alpha, colorFilter, mode, level);
}

ECode CRemoteViews::SetTextColor(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 color)
{
    return RemoteViews::SetTextColor(viewId, color);
}

ECode CRemoteViews::SetRemoteAdapter(
    /* [in] */ Int32 viewId,
    /* [in] */ IIntent* intent)
{
    return RemoteViews::SetRemoteAdapter(viewId, intent);
}

//    @Deprecated
ECode CRemoteViews::SetRemoteAdapter(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ Int32 viewId,
    /* [in] */ IIntent* intent)
{
    return RemoteViews::SetRemoteAdapter(appWidgetId, viewId, intent);
}

ECode CRemoteViews::SetScrollPosition(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 position)
{
    return RemoteViews::SetScrollPosition(viewId, position);
}

ECode CRemoteViews::SetRelativeScrollPosition(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 offset)
{
    return RemoteViews::SetRelativeScrollPosition(viewId, offset);
}

ECode CRemoteViews::SetViewPadding(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    return RemoteViews::SetViewPadding(viewId, left, top, right, bottom);
}

ECode CRemoteViews::SetBoolean(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ Boolean value)
{
    return RemoteViews::SetBoolean(viewId, methodName, value);
}

ECode CRemoteViews::SetByte(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ Byte value)
{
    return RemoteViews::SetByte(viewId, methodName, value);
}

ECode CRemoteViews::SetInt16(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ Int16 value)
{
    return RemoteViews::SetInt16(viewId, methodName, value);
}

ECode CRemoteViews::SetInt32(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ Int32 value)
{
    return RemoteViews::SetInt32(viewId, methodName, value);
}

ECode CRemoteViews::SetInt64(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ Int64 value)
{
    return RemoteViews::SetInt64(viewId, methodName, value);
}

ECode CRemoteViews::SetFloat(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ Float value)
{
    return RemoteViews::SetFloat(viewId, methodName, value);
}

ECode CRemoteViews::SetDouble(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ Double value)
{
    return RemoteViews::SetDouble(viewId, methodName, value);
}

ECode CRemoteViews::SetChar(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ Char32 value)
{
    return RemoteViews::SetChar(viewId, methodName, value);
}

ECode CRemoteViews::SetString(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ const String& value)
{
    return RemoteViews::SetString(viewId, methodName, value);
}

ECode CRemoteViews::SetCharSequence(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ ICharSequence* value)
{
    return RemoteViews::SetCharSequence(viewId, methodName, value);
}

ECode CRemoteViews::SetUri(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ IUri* value)
{
    return RemoteViews::SetUri(viewId, methodName, value);
}

ECode CRemoteViews::SetBitmap(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ IBitmap* value)
{
    return RemoteViews::SetBitmap(viewId, methodName, value);
}

ECode CRemoteViews::SetBundle(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ IBundle* value)
{
    return RemoteViews::SetBundle(viewId, methodName, value);
}

ECode CRemoteViews::SetIntent(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ IIntent* value)
{
    return RemoteViews::SetIntent(viewId, methodName, value);
}

ECode CRemoteViews::SetContentDescription(
    /* [in] */ Int32 viewId,
    /* [in] */ ICharSequence* contentDescription)
{
    return RemoteViews::SetContentDescription(viewId, contentDescription);
}

ECode CRemoteViews::SetLabelFor(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 labeledId)
{
    return RemoteViews::SetLabelFor(viewId, labeledId);
}

ECode CRemoteViews::Apply(
    /* [in] */ IContext* context,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IView> v = RemoteViews::Apply(context, parent);
    *view = v;
    INTERFACE_ADDREF(*view);
    return NOERROR;
}

/** @hide */
ECode CRemoteViews::Apply(
    /* [in] */ IContext* context,
    /* [in] */ IViewGroup* parent,
    /* [in] */ IRemoteViewsOnClickHandler* handler,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IView> v = RemoteViews::Apply(context, parent, handler);
    *view = v;
    INTERFACE_ADDREF(*view);
    return NOERROR;
}

ECode CRemoteViews::Reapply(
    /* [in] */ IContext* context,
    /* [in] */ IView* v)
{
    return RemoteViews::Reapply(context, v);
}

/** @hide */
ECode CRemoteViews::Reapply(
    /* [in] */ IContext* context,
    /* [in] */ IView* v,
    /* [in] */ IRemoteViewsOnClickHandler* handler)
{
    return RemoteViews::Reapply(context, v, handler);
}

ECode CRemoteViews::OnLoadClass(
    /* [in] */ Handle32 clazz,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    return RemoteViews::OnLoadClass(clazz, res);
}

ECode CRemoteViews::SetNotRoot()
{
    return RemoteViews::SetNotRoot();
}

ECode CRemoteViews::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return RemoteViews::ReadFromParcel(source);
}

ECode CRemoteViews::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return RemoteViews::WriteToParcel(dest);
}

ECode CRemoteViews::SetIsWidgetCollectionChild(
    /* [in] */ Boolean isWidgetCollectionChild)
{
    return RemoteViews::SetIsWidgetCollectionChild(isWidgetCollectionChild);
}

ECode CRemoteViews::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = RemoteViews::ToString();
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos

