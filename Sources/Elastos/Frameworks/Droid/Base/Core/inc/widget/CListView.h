#ifndef __ELASTOS_DROID_WIDGET_CLISTVIEW_H__
#define __ELASTOS_DROID_WIDGET_CLISTVIEW_H__

#include "_Elastos_Droid_Widget_CListView.h"

#include "widget/ListView.h"



namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CListView), public ListView
{
public:
    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    IADAPTERVIEW_METHODS_DECL()
    IABSLISTVIEW_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

public:

    /**
     * This method is called to notify you that, within <code>s</code>,
     * the <code>count</code> characters beginning at <code>start</code>
     * are about to be replaced by new text with length <code>after</code>.
     * It is an error to attempt to make changes to <code>s</code> from
     * this callback.
     */
    CARAPI BeforeTextChanged(
        /* [in] */  ICharSequence* s,
        /* [in] */  Int32 start,
        /* [in] */  Int32 count,
        /* [in] */  Int32 after);

    /**
     * This method is called to notify you that, within <code>s</code>,
     * the <code>count</code> characters beginning at <code>start</code>
     * have just replaced old text that had length <code>before</code>.
     * It is an error to attempt to make changes to <code>s</code> from
     * this callback.
     */
    CARAPI OnTextChanged(
        /* [in] */  ICharSequence* s,
        /* [in] */  Int32 start,
        /* [in] */  Int32 before,
        /* [in] */  Int32 count);

    /**
     * This method is called to notify you that, somewhere within
     * <code>s</code>, the text has been changed.
     * It is legitimate to make further changes to <code>s</code> from
     * this callback, but be careful not to get yourself into an infinite
     * loop, because any changes you make will cause this method to be
     * called again recursively.
     * (You are not told where the change took place because other
     * afterTextChanged() methods may already have made other changes
     * and invalidated the offsets.  But if you need to know here,
     * you can use {@link Spannable#setSpan} in {@link #onTextChanged}
     * to mark your place and then look up from here where the span
     * ended up.
     */
    CARAPI AfterTextChanged(
        /* [in] */  IEditable* s);

    /**
     * Callback method to be invoked when the global layout state or the visibility of views
     * within the view tree changes
     */
    CARAPI OnGlobalLayout();

    /**
     * Callback method to be invoked when the touch mode changes.
     *
     * @param isInTouchMode True if the view hierarchy is now in touch mode, false  otherwise.
     */
    CARAPI OnTouchModeChanged(
        /* [in] */  Boolean isInTouchMode);

    /**
     * <p>Notifies the end of a filtering operation.</p>
     *
     * @param count the number of values computed by the filter
     */
    CARAPI OnFilterComplete(
        /* [in] */  Int32 count);

    CARAPI OnRemoteAdapterConnected(
        /* [out] */  Boolean* rst);

    CARAPI OnRemoteAdapterDisconnected();

    /**
     * This defers a notifyDataSetChanged on the pending RemoteViewsAdapter if it has not
     * connected yet.
     */
    CARAPI DeferNotifyDataSetChanged();

    /**
     * @return The maximum amount a list view will scroll in response to
     *   an arrow event.
     */
    CARAPI GetMaxScrollAmount(
        /* [out] */ Int32* amount);

    /**
     * Add a fixed view to appear at the top of the list. If addHeaderView is
     * called more than once, the views will appear in the order they were
     * added. Views added using this call can take focus if they want.
     * <p>
     * NOTE: Call this before calling setAdapter. This is so ListView can wrap
     * the supplied cursor with one that will also account for header and footer
     * views.
     *
     * @param v The view to add.
     * @param data Data to associate with this view
     * @param isSelectable whether the item is selectable
     */
    CARAPI AddHeaderView(
        /* [in] */ IView* v,
        /* [in] */ IInterface* data,
        /* [in] */ Boolean isSelectable);

    /**
     * Add a fixed view to appear at the top of the list. If addHeaderView is
     * called more than once, the views will appear in the order they were
     * added. Views added using this call can take focus if they want.
     * <p>
     * NOTE: Call this before calling setAdapter. This is so ListView can wrap
     * the supplied cursor with one that will also account for header and footer
     * views.
     *
     * @param v The view to add.
     */
    CARAPI AddHeaderView(
        /* [in] */ IView* v);

    /**
     * Removes a previously-added header view.
     *
     * @param v The view to remove
     * @return TRUE if the view was removed, FALSE if the view was not a header
     *         view
     */
    CARAPI RemoveHeaderView(
        /* [in] */ IView* v,
        /* [out] */ Boolean* result);

    /**
     * Add a fixed view to appear at the bottom of the list. If addFooterView is
     * called more than once, the views will appear in the order they were
     * added. Views added using this call can take focus if they want.
     * <p>
     * NOTE: Call this before calling setAdapter. This is so ListView can wrap
     * the supplied cursor with one that will also account for header and footer
     * views.
     *
     * @param v The view to add.
     * @param data Data to associate with this view
     * @param isSelectable TRUE if the footer view can be selected
     */
    CARAPI AddFooterView(
        /* [in] */ IView* v,
        /* [in] */ IInterface* data,
        /* [in] */ Boolean isSelectable);

    /**
     * Add a fixed view to appear at the bottom of the list. If addFooterView is called more
     * than once, the views will appear in the order they were added. Views added using
     * this call can take focus if they want.
     * <p>NOTE: Call this before calling setAdapter. This is so ListView can wrap the supplied
     * cursor with one that will also account for header and footer views.
     *
     *
     * @param v The view to add.
     */
    CARAPI AddFooterView(
        /* [in] */ IView* v);

    /**
     * Removes a previously-added footer view.
     *
     * @param v The view to remove
     * @return
     * TRUE if the view was removed, FALSE if the view was not a footer view
     */
    CARAPI RemoveFooterView(
        /* [in] */ IView* v,
        /* [out] */ Boolean* result);

    /**
     * Smoothly scroll to the specified adapter position offset. The view will
     * scroll such that the indicated position is displayed.
     * @param offset The amount to offset from the adapter position to scroll to.
     */
    CARAPI SmoothScrollByOffset(
        /* [in] */ Int32 position);

    /**
     * Sets the selected item and positions the selection y pixels from the top edge
     * of the ListView. (If in touch mode, the item will not be selected but it will
     * still be positioned appropriately.)
     *
     * @param position Index (starting at 0) of the data item to be selected.
     * @param y The distance from the top edge of the ListView (plus padding) that the
     *        item will be positioned.
     */
    CARAPI SetSelectionFromTop(
        /* [in] */ Int32 position,
        /* [in] */ Int32 y);

    /**
     * setSelectionAfterHeaderView set the selection to be the first list item
     * after the header views.
     */
    CARAPI SetSelectionAfterHeaderView();

    /**
     * Indicates that the views created by the ListAdapter can contain focusable
     * items.
     *
     * @param itemsCanFocus TRUE if items can Get focus, FALSE otherwise
     */
    CARAPI SetItemsCanFocus(
        /* [in] */ Boolean itemsCanFocus);

    /**
     * @return Whether the views created by the ListAdapter can contain focusable
     * items.
     */
    CARAPI GetItemsCanFocus(
        /* [out] */ Boolean* canFocus);

    /**
     * Returns the drawable that will be drawn between each item in the list.
     *
     * @return the current drawable drawn between list elements
     */
    CARAPI GetDivider(
        /* [out] */ IDrawable** divider);

    /**
     * Sets the drawable that will be drawn between each item in the list. If the drawable does
     * not have an intrinsic height, you should also call {@link #setDividerHeight(Int32)}
     *
     * @param divider The drawable to use.
     */
    CARAPI SetDivider(
        /* [in] */ IDrawable* divider);

    /**
     * @return Returns the height of the divider that will be drawn between each item in the list.
     */
    CARAPI GetDividerHeight(
        /* [out] */ Int32* height);

    /**
     * Sets the height of the divider that will be drawn between each item in the list. Calling
     * this will override the intrinsic height as set by {@link #setDivider(Drawable)}
     *
     * @param height The new height of the divider in pixels.
     */
    CARAPI SetDividerHeight(
        /* [in] */ Int32 height);

    /**
     * Enables or disables the drawing of the divider for header views.
     *
     * @param headerDividersEnabled True to draw the headers, FALSE otherwise.
     *
     * @see #setFooterDividersEnabled(Boolean)
     * @see #addHeaderView(android.view.View)
     */
    CARAPI SetHeaderDividersEnabled(
        /* [in] */ Boolean headerDividersEnabled);

    /**
     * Enables or disables the drawing of the divider for footer views.
     *
     * @param footerDividersEnabled True to draw the footers, FALSE otherwise.
     *
     * @see #setHeaderDividersEnabled(Boolean)
     * @see #addFooterView(android.view.View)
     */
    CARAPI SetFooterDividersEnabled(
        /* [in] */ Boolean footerDividersEnabled);

    /**
     * Sets the drawable that will be drawn above all other list content.
     * This area can become visible when the user overscrolls the list.
     *
     * @param header The drawable to use
     */
    CARAPI SetOverscrollHeader(
        /* [in] */ IDrawable* header);

    /**
     * @return The drawable that will be drawn above all other list content
     */
    CARAPI GetOverscrollHeader(
        /* [out] */ IDrawable** overScrollHeader);

    /**
     * Sets the drawable that will be drawn below all other list content.
     * This area can become visible when the user overscrolls the list,
     * or when the list's content does not fully fill the container area.
     *
     * @param footer The drawable to use
     */
    CARAPI SetOverscrollFooter(
        /* [in] */ IDrawable* footer);

    /**
     * @return The drawable that will be drawn below all other list content
     */
    CARAPI GetOverscrollFooter(
        /* [out] */ IDrawable** overScrollFooter);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CLISTVIEW_H__
