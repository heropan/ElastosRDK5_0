#ifndef __ELASTOS_DROID_WIDGET_CREMOTEVIEWS_H__
#define __ELASTOS_DROID_WIDGET_CREMOTEVIEWS_H__

#include "_CRemoteViews.h"
#include "widget/RemoteViews.h"

namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CRemoteViews), public RemoteViews
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 layoutId);

    CARAPI constructor(
        /* [in] */ IRemoteViews* landscape,
        /* [in] */ IRemoteViews* portrait);
public:

    /**
     * Merges the passed RemoteViews actions with this RemoteViews actions according to
     * action-specific merge rules.
     *
     * @param newRv
     *
     * @hide
     */
    CARAPI MergeRemoteViews(
        /* [in] */ IRemoteViews* newRv);

    CARAPI SetUser(
        /* [in] */ IUserHandle* user);

    CARAPI Clone(
        /* [out] */ IRemoteViews** views);

    CARAPI GetPackage(
        /* [out] */ String* package);

    /**
     * Reutrns the layout id of the root layout associated with this RemoteViews. In the case
     * that the RemoteViews has both a landscape and portrait root, this will return the layout
     * id associated with the portrait layout.
     *
     * @return the layout id.
     */
    CARAPI GetLayoutId(
        /* [out] */ Int32* id);

    /**
     * Returns an estimate of the bitmap heap memory usage for this RemoteViews.
     */
    /** @hide */
    CARAPI EstimateMemoryUsage(
        /* [out] */ Int32* usage);

    /**
     * Equivalent to calling {@link ViewGroup#addView(View)} after inflating the
     * given {@link RemoteViews}. This allows users to build "nested"
     * {@link RemoteViews}. In cases where consumers of {@link RemoteViews} may
     * recycle layouts, use {@link #removeAllViews(Int32)} to clear any existing
     * children.
     *
     * @param viewId The id of the parent {@link ViewGroup} to add child into.
     * @param nestedView {@link RemoteViews} that describes the child.
     */
    CARAPI AddView(
        /* [in] */ Int32 viewId,
        /* [in] */ IRemoteViews* nestedView);

    /**
     * Equivalent to calling {@link ViewGroup#removeAllViews()}.
     *
     * @param viewId The id of the parent {@link ViewGroup} to remove all
     *            children from.
     */
    CARAPI RemoveAllViews(
        /* [in] */ Int32 viewId);

    /**
     * Equivalent to calling {@link AdapterViewAnimator#showNext()}
     *
     * @param viewId The id of the view on which to call {@link AdapterViewAnimator#showNext()}
     */
    CARAPI ShowNext(
        /* [in] */ Int32 viewId);

    /**
     * Equivalent to calling {@link AdapterViewAnimator#showPrevious()}
     *
     * @param viewId The id of the view on which to call {@link AdapterViewAnimator#showPrevious()}
     */
    CARAPI ShowPrevious(
        /* [in] */ Int32 viewId);

    /**
     * Equivalent to calling {@link AdapterViewAnimator#setDisplayedChild(Int32)}
     *
     * @param viewId The id of the view on which to call
     *               {@link AdapterViewAnimator#setDisplayedChild(Int32)}
     */
    CARAPI SetDisplayedChild(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 childIndex);

    /**
     * Equivalent to calling View.setVisibility
     *
     * @param viewId The id of the view whose visibility should change
     * @param visibility The new visibility for the view
     */
    CARAPI SetViewVisibility(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 visibility);

    /**
     * Equivalent to calling TextView.setText
     *
     * @param viewId The id of the view whose text should change
     * @param text The new text for the view
     */
    CARAPI SetTextViewText(
        /* [in] */ Int32 viewId,
        /* [in] */ ICharSequence* text);

    /**
     * Equivalent to calling {@link TextView#setTextSize(Int32, float)}
     *
     * @param viewId The id of the view whose text size should change
     * @param units The units of size (e.g. COMPLEX_UNIT_SP)
     * @param size The size of the text
     */
    CARAPI SetTextViewTextSize(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 units,
        /* [in] */ Float size);

    /**
     * Equivalent to calling
     * {@link TextView#setCompoundDrawablesWithIntrinsicBounds(Int32, Int32, Int32, Int32)}.
     *
     * @param viewId The id of the view whose text should change
     * @param left The id of a drawable to place to the left of the text, or 0
     * @param top The id of a drawable to place above the text, or 0
     * @param right The id of a drawable to place to the right of the text, or 0
     * @param bottom The id of a drawable to place below the text, or 0
     */
    CARAPI SetTextViewCompoundDrawables(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    /**
     * Equivalent to calling {@link
     * TextView#setCompoundDrawablesRelativeWithIntrinsicBounds(Int32, Int32, Int32, Int32)}.
     *
     * @param viewId The id of the view whose text should change
     * @param start The id of a drawable to place before the text (relative to the
     * layout direction), or 0
     * @param top The id of a drawable to place above the text, or 0
     * @param end The id of a drawable to place after the text, or 0
     * @param bottom The id of a drawable to place below the text, or 0
     */
    CARAPI SetTextViewCompoundDrawablesRelative(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 start,
        /* [in] */ Int32 top,
        /* [in] */ Int32 end,
        /* [in] */ Int32 bottom);

    /**
     * Equivalent to calling ImageView.setImageResource
     *
     * @param viewId The id of the view whose drawable should change
     * @param srcId The new resource id for the drawable
     */
    CARAPI SetImageViewResource(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 srcId);

    /**
     * Equivalent to calling ImageView.setImageURI
     *
     * @param viewId The id of the view whose drawable should change
     * @param uri The Uri for the image
     */
    CARAPI SetImageViewUri(
        /* [in] */ Int32 viewId,
        /* [in] */ IUri* uri);

    /**
     * Equivalent to calling ImageView.setImageBitmap
     *
     * @param viewId The id of the view whose bitmap should change
     * @param bitmap The new Bitmap for the drawable
     */
    CARAPI SetImageViewBitmap(
        /* [in] */ Int32 viewId,
        /* [in] */ IBitmap* bitmap);

    /**
     * Equivalent to calling AdapterView.setEmptyView
     *
     * @param viewId The id of the view on which to set the empty view
     * @param emptyViewId The view id of the empty view
     */
    CARAPI SetEmptyView(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 emptyViewId);

    /**
     * Equivalent to calling {@link Chronometer#setBase Chronometer.setBase},
     * {@link Chronometer#setFormat Chronometer.setFormat},
     * and {@link Chronometer#start Chronometer.start()} or
     * {@link Chronometer#stop Chronometer.stop()}.
     *
     * @param viewId The id of the {@link Chronometer} to change
     * @param base The time at which the timer would have read 0:00.  This
     *             time should be based off of
     *             {@link android.os.SystemClock#elapsedRealtime SystemClock.elapsedRealtime()}.
     * @param format The Chronometer format string, or null to
     *               simply display the timer value.
     * @param started True if you want the clock to be started, false if not.
     */
    CARAPI SetChronometer(
        /* [in] */ Int32 viewId,
        /* [in] */ Int64 base,
        /* [in] */ const String& format,
        /* [in] */ Boolean started);

    /**
     * Equivalent to calling {@link ProgressBar#setMax ProgressBar.setMax},
     * {@link ProgressBar#setProgress ProgressBar.setProgress}, and
     * {@link ProgressBar#setIndeterminate ProgressBar.setIndeterminate}
     *
     * If indeterminate is true, then the values for max and progress are ignored.
     *
     * @param viewId The id of the {@link ProgressBar} to change
     * @param max The 100% value for the progress bar
     * @param progress The current value of the progress bar.
     * @param indeterminate True if the progress bar is indeterminate,
     *                false if not.
     */
    CARAPI SetProgressBar(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 max,
        /* [in] */ Int32 progress,
        /* [in] */ Boolean indeterminate);

    /**
     * Equivalent to calling
     * {@link android.view.View#setOnClickListener(android.view.View.OnClickListener)}
     * to launch the provided {@link PendingIntent}.
     *
     * When setting the on-click action of items within collections (eg. {@link ListView},
     * {@link StackView} etc.), this method will not work. Instead, use {@link
     * RemoteViews#setPendingIntentTemplate(Int32, PendingIntent) in conjunction with
     * RemoteViews#setOnClickFillInIntent(Int32, Intent).
     *
     * @param viewId The id of the view that will trigger the {@link PendingIntent} when clicked
     * @param pendingIntent The {@link PendingIntent} to send when user clicks
     */
    CARAPI SetOnClickPendingIntent(
        /* [in] */ Int32 viewId,
        /* [in] */ IPendingIntent* pendingIntent);

    /**
     * When using collections (eg. {@link ListView}, {@link StackView} etc.) in widgets, it is very
     * costly to set PendingIntents on the individual items, and is hence not permitted. Instead
     * this method should be used to set a single PendingIntent template on the collection, and
     * individual items can differentiate their on-click behavior using
     * {@link RemoteViews#setOnClickFillInIntent(Int32, Intent)}.
     *
     * @param viewId The id of the collection who's children will use this PendingIntent template
     *          when clicked
     * @param pendingIntentTemplate The {@link PendingIntent} to be combined with extras specified
     *          by a child of viewId and executed when that child is clicked
     */
    CARAPI SetPendingIntentTemplate(
        /* [in] */ Int32 viewId,
        /* [in] */ IPendingIntent* pendingIntentTemplate);

    /**
     * When using collections (eg. {@link ListView}, {@link StackView} etc.) in widgets, it is very
     * costly to set PendingIntents on the individual items, and is hence not permitted. Instead
     * a single PendingIntent template can be set on the collection, see {@link
     * RemoteViews#setPendingIntentTemplate(Int32, PendingIntent)}, and the individual on-click
     * action of a given item can be distinguished by setting a fillInIntent on that item. The
     * fillInIntent is then combined with the PendingIntent template in order to determine the final
     * intent which will be executed when the item is clicked. This works as follows: any fields
     * which are left blank in the PendingIntent template, but are provided by the fillInIntent
     * will be overwritten, and the resulting PendingIntent will be used.
     *
     *
     * of the PendingIntent template will then be filled in with the associated fields that are
     * set in fillInIntent. See {@link Intent#fillIn(Intent, Int32)} for more details.
     *
     * @param viewId The id of the view on which to set the fillInIntent
     * @param fillInIntent The intent which will be combined with the parent's PendingIntent
     *        in order to determine the on-click behavior of the view specified by viewId
     */
    CARAPI SetOnClickFillInIntent(
        /* [in] */ Int32 viewId,
        /* [in] */ IIntent* fillInIntent);

    /**
     * @hide
     * Equivalent to calling a combination of {@link Drawable#setAlpha(Int32)},
     * {@link Drawable#setColorFilter(Int32, android.graphics.PorterDuff.Mode)},
     * and/or {@link Drawable#setLevel(Int32)} on the {@link Drawable} of a given
     * view.
     * <p>
     * You can omit specific calls by marking their values with null or -1.
     *
     * @param viewId The id of the view that contains the target
     *            {@link Drawable}
     * @param targetBackground If true, apply these parameters to the
     *            {@link Drawable} returned by
     *            {@link android.view.View#getBackground()}. Otherwise, assume
     *            the target view is an {@link ImageView} and apply them to
     *            {@link ImageView#getDrawable()}.
     * @param alpha Specify an alpha value for the drawable, or -1 to leave
     *            unchanged.
     * @param colorFilter Specify a color for a
     *            {@link android.graphics.ColorFilter} for this drawable, or -1
     *            to leave unchanged.
     * @param mode Specify a PorterDuff mode for this drawable, or null to leave
     *            unchanged.
     * @param level Specify the level for the drawable, or -1 to leave
     *            unchanged.
     */
    CARAPI SetDrawableParameters(
        /* [in] */ Int32 viewId,
        /* [in] */ Boolean targetBackground,
        /* [in] */ Int32 alpha,
        /* [in] */ Int32 colorFilter,
        /* [in] */ PorterDuffMode mode,
        /* [in] */ Int32 level);

    /**
     * Equivalent to calling {@link android.widget.TextView#setTextColor(Int32)}.
     *
     * @param viewId The id of the view whose text color should change
     * @param color Sets the text color for all the states (normal, selected,
     *            focused) to be this color.
     */
    CARAPI SetTextColor(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 color);

    /**
     * Equivalent to calling {@link android.widget.AbsListView#setRemoteViewsAdapter(Intent)}.
     *
     * @param appWidgetId The id of the app widget which contains the specified view. (This
     *      parameter is ignored in this deprecated method)
     * @param viewId The id of the {@link AbsListView}
     * @param intent The intent of the service which will be
     *            providing data to the RemoteViewsAdapter
     * @deprecated This method has been deprecated. See
     *      {@link android.widget.RemoteViews#setRemoteAdapter(Int32, Intent)}
     */
    CARAPI SetRemoteAdapter(
        /* [in] */ Int32 viewId,
        /* [in] */ IIntent* intent);

    //    @Deprecated
    CARAPI SetRemoteAdapter(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ Int32 viewId,
        /* [in] */ IIntent* intent);

    /**
     * Equivalent to calling {@link android.widget.AbsListView#smoothScrollToPosition(Int32, Int32)}.
     *
     * @param viewId The id of the view to change
     * @param position Scroll to this adapter position
     */
    CARAPI SetScrollPosition(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 position);

    /**
     * Equivalent to calling {@link android.widget.AbsListView#smoothScrollToPosition(Int32, Int32)}.
     *
     * @param viewId The id of the view to change
     * @param offset Scroll by this adapter position offset
     */
    CARAPI SetRelativeScrollPosition(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 offset);

    /**
     * Equivalent to calling {@link android.view.View#setPadding(Int32, Int32, Int32, Int32)}.
     *
     * @param viewId The id of the view to change
     * @param left the left padding in pixels
     * @param top the top padding in pixels
     * @param right the right padding in pixels
     * @param bottom the bottom padding in pixels
     */
    CARAPI SetViewPadding(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    /**
     * Call a method taking one Boolean on a view in the layout for this RemoteViews.
     *
     * @param viewId The id of the view on which to call the method.
     * @param methodName The name of the method to call.
     * @param value The value to pass to the method.
     */
    CARAPI SetBoolean(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ Boolean value);

    /**
     * Call a method taking one byte on a view in the layout for this RemoteViews.
     *
     * @param viewId The id of the view on which to call the method.
     * @param methodName The name of the method to call.
     * @param value The value to pass to the method.
     */
    SetByte(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ Byte value);

    /**
     * Call a method taking one short on a view in the layout for this RemoteViews.
     *
     * @param viewId The id of the view on which to call the method.
     * @param methodName The name of the method to call.
     * @param value The value to pass to the method.
     */
    CARAPI SetInt16(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ Int16 value);

    /**
     * Call a method taking one Int32 on a view in the layout for this RemoteViews.
     *
     * @param viewId The id of the view on which to call the method.
     * @param methodName The name of the method to call.
     * @param value The value to pass to the method.
     */
    CARAPI SetInt32(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ Int32 value);

    /**
     * Call a method taking one long on a view in the layout for this RemoteViews.
     *
     * @param viewId The id of the view on which to call the method.
     * @param methodName The name of the method to call.
     * @param value The value to pass to the method.
     */
    CARAPI SetInt64(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ Int64 value);

    /**
     * Call a method taking one float on a view in the layout for this RemoteViews.
     *
     * @param viewId The id of the view on which to call the method.
     * @param methodName The name of the method to call.
     * @param value The value to pass to the method.
     */
    CARAPI SetFloat(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ Float value);

    /**
     * Call a method taking one double on a view in the layout for this RemoteViews.
     *
     * @param viewId The id of the view on which to call the method.
     * @param methodName The name of the method to call.
     * @param value The value to pass to the method.
     */
    CARAPI SetDouble(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ Double value);

    /**
     * Call a method taking one char on a view in the layout for this RemoteViews.
     *
     * @param viewId The id of the view on which to call the method.
     * @param methodName The name of the method to call.
     * @param value The value to pass to the method.
     */
    CARAPI SetChar(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ Char32 value);

    /**
     * Call a method taking one String on a view in the layout for this RemoteViews.
     *
     * @param viewId The id of the view on which to call the method.
     * @param methodName The name of the method to call.
     * @param value The value to pass to the method.
     */
    CARAPI SetString(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ const String& value);

    /**
     * Call a method taking one CharSequence on a view in the layout for this RemoteViews.
     *
     * @param viewId The id of the view on which to call the method.
     * @param methodName The name of the method to call.
     * @param value The value to pass to the method.
     */
    CARAPI SetCharSequence(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ ICharSequence* value);

    /**
     * Call a method taking one Uri on a view in the layout for this RemoteViews.
     *
     * @param viewId The id of the view on which to call the method.
     * @param methodName The name of the method to call.
     * @param value The value to pass to the method.
     */
    CARAPI SetUri(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ IUri* value);

    /**
     * Call a method taking one Bitmap on a view in the layout for this RemoteViews.
     * @more
     * <p class="note">The bitmap will be flattened into the parcel if this object is
     * sent across processes, so it may end up using a lot of memory, and may be fairly slow.</p>
     *
     * @param viewId The id of the view on which to call the method.
     * @param methodName The name of the method to call.
     * @param value The value to pass to the method.
     */
    CARAPI SetBitmap(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ IBitmap* value);

    /**
     * Call a method taking one Bundle on a view in the layout for this RemoteViews.
     *
     * @param viewId The id of the view on which to call the method.
     * @param methodName The name of the method to call.
     * @param value The value to pass to the method.
     */
    CARAPI SetBundle(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ IBundle* value);

    /**
     * Call a method taking one Intent on a view in the layout for this RemoteViews.
     *
     * @param viewId The id of the view on which to call the method.
     * @param methodName The name of the method to call.
     * @param value The {@link android.content.Intent} to pass the method.
     */
    CARAPI SetIntent(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ IIntent* value);

    /**
     * Equivalent to calling View.setContentDescription(CharSequence).
     *
     * @param viewId The id of the view whose content description should change.
     * @param contentDescription The new content description for the view.
     */
    CARAPI SetContentDescription(
        /* [in] */ Int32 viewId,
        /* [in] */ ICharSequence* contentDescription);

    /**
     * Equivalent to calling View.setLabelFor(Int32).
     *
     * @param viewId The id of the view whose property to set.
     * @param labeledId The id of a view for which this view serves as a label.
     */
    CARAPI SetLabelFor(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 labeledId);

    /**
     * Inflates the view hierarchy represented by this object and applies
     * all of the actions.
     *
     * <p><strong>Caller beware: this may throw</strong>
     *
     * @param context Default context to use
     * @param parent Parent that the resulting view hierarchy will be attached to. This method
     * does <strong>not</strong> attach the hierarchy. The caller should do so when appropriate.
     * @return The inflated view hierarchy
     */
    CARAPI Apply(
        /* [in] */ IContext* context,
        /* [in] */ IViewGroup* parent,
        /* [out] */ IView** view);

    /** @hide */
    CARAPI Apply(
        /* [in] */ IContext* context,
        /* [in] */ IViewGroup* parent,
        /* [in] */ IRemoteViewsOnClickHandler* handler,
        /* [out] */ IView** view);

    /**
     * Applies all of the actions to the provided view.
     *
     * <p><strong>Caller beware: this may throw</strong>
     *
     * @param v The view to apply the actions to.  This should be the result of
     * the {@link #apply(Context,ViewGroup)} call.
     */
    CARAPI Reapply(
        /* [in] */ IContext* context,
        /* [in] */ IView* v);

    /** @hide */
    CARAPI Reapply(
        /* [in] */ IContext* context,
        /* [in] */ IView* v,
        /* [in] */ IRemoteViewsOnClickHandler* handler);

    /* (non-Javadoc)
     * Used to restrict the views which can be inflated
     *
     * @see android.view.LayoutInflater.Filter#onLoadClass(java.lang.Class)
     */
    CARAPI OnLoadClass(
        /* [in] */ Handle32 clazz,
        /* [out] */ Boolean* res);

    CARAPI SetNotRoot();

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI SetIsWidgetCollectionChild(
        /* [in] */ Boolean isWidgetCollectionChild);

    CARAPI ToString(
        /* [out] */ String* info);
};


}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CREMOTEVIEWS_H__
