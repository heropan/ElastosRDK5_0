
#include "widget/CListView.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEW_METHODS_IMPL(CListView, ListView)
IVIEWGROUP_METHODS_IMPL(CListView, ListView)
IVIEWPARENT_METHODS_IMPL(CListView, ListView)
IVIEWMANAGER_METHODS_IMPL(CListView, ListView)
IDRAWABLECALLBACK_METHODS_IMPL(CListView, ListView)
IKEYEVENTCALLBACK_METHODS_IMPL(CListView, ListView)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CListView, ListView)
IADAPTERVIEW_METHODS_IMPL(CListView, ListView)
IABSLISTVIEW_METHODS_IMPL(CListView, ListView)

/**
 * This method is called to notify you that, within <code>s</code>,
 * the <code>count</code> characters beginning at <code>start</code>
 * are about to be replaced by new text with length <code>after</code>.
 * It is an error to attempt to make changes to <code>s</code> from
 * this callback.
 */
ECode CListView::BeforeTextChanged(
    /* [in] */  ICharSequence* s,
    /* [in] */  Int32 start,
    /* [in] */  Int32 count,
    /* [in] */  Int32 after)
{
    return ListView::BeforeTextChanged(s, start, count, after);
}

/**
 * This method is called to notify you that, within <code>s</code>,
 * the <code>count</code> characters beginning at <code>start</code>
 * have just replaced old text that had length <code>before</code>.
 * It is an error to attempt to make changes to <code>s</code> from
 * this callback.
 */
ECode CListView::OnTextChanged(
    /* [in] */  ICharSequence* s,
    /* [in] */  Int32 start,
    /* [in] */  Int32 before,
    /* [in] */  Int32 count)
{
    return ListView::OnTextChanged(s, start, before, count);
}

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
ECode CListView::AfterTextChanged(
    /* [in] */  IEditable* s)
{
    return ListView::AfterTextChanged(s);
}

/**
 * Callback method to be invoked when the global layout state or the visibility of views
 * within the view tree changes
 */
ECode CListView::OnGlobalLayout()
{
    return ListView::OnGlobalLayout();
}

/**
 * Callback method to be invoked when the touch mode changes.
 *
 * @param isInTouchMode True if the view hierarchy is now in touch mode, false  otherwise.
 */
ECode CListView::OnTouchModeChanged(
    /* [in] */  Boolean isInTouchMode)
{
    return ListView::OnTouchModeChanged(isInTouchMode);
}

/**
 * <p>Notifies the end of a filtering operation.</p>
 *
 * @param count the number of values computed by the filter
 */
ECode CListView::OnFilterComplete(
    /* [in] */  Int32 count)
{
    return ListView::OnFilterComplete(count);
}

ECode CListView::OnRemoteAdapterConnected(
    /* [out] */ Boolean* rst)
{
    VALIDATE_NOT_NULL(rst);
    *rst = ListView::OnRemoteAdapterConnected();
    return NOERROR;
}

ECode CListView::OnRemoteAdapterDisconnected()
{
    return ListView::OnRemoteAdapterDisconnected();
}

/**
 * This defers a notifyDataSetChanged on the pending RemoteViewsAdapter if it has not
 * connected yet.
 */
ECode CListView::DeferNotifyDataSetChanged()
{
    return ListView::DeferNotifyDataSetChanged();
}

PInterface CListView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CListView::Probe(riid);
}

ECode CListView::constructor(
    /* [in] */ IContext* context)
{
    return ListView::Init(context);
}

ECode CListView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return ListView::Init(context, attrs);
}

ECode CListView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return ListView::Init(context, attrs, defStyle);
}

/**
 * @return The maximum amount a list view will scroll in response to
 *   an arrow event.
 */
ECode CListView::GetMaxScrollAmount(
    /* [out] */ Int32* amount)
{
    VALIDATE_NOT_NULL(amount);
    *amount = ListView::GetMaxScrollAmount();
    return NOERROR;
}

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
ECode CListView::AddHeaderView(
    /* [in] */ IView* v,
    /* [in] */ IInterface* data,
    /* [in] */ Boolean isSelectable)
{
    return ListView::AddHeaderView(v, data, isSelectable);
}

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
ECode CListView::AddHeaderView(
    /* [in] */ IView* v)
{
    return ListView::AddHeaderView(v);
}

/**
 * Removes a previously-added header view.
 *
 * @param v The view to remove
 * @return TRUE if the view was removed, FALSE if the view was not a header
 *         view
 */
ECode CListView::RemoveHeaderView(
    /* [in] */ IView* v,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = ListView::RemoveHeaderView(v);
    return NOERROR;
}

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
ECode CListView::AddFooterView(
    /* [in] */ IView* v,
    /* [in] */ IInterface* data,
    /* [in] */ Boolean isSelectable)
{
    return ListView::AddFooterView(v, data, isSelectable);
}

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
ECode CListView::AddFooterView(
    /* [in] */ IView* v)
{
    return ListView::AddFooterView(v);
}

/**
 * Removes a previously-added footer view.
 *
 * @param v The view to remove
 * @return
 * TRUE if the view was removed, FALSE if the view was not a footer view
 */
ECode CListView::RemoveFooterView(
    /* [in] */ IView* v,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = ListView::RemoveFooterView(v);
    return NOERROR;
}

/**
 * Smoothly scroll to the specified adapter position offset. The view will
 * scroll such that the indicated position is displayed.
 * @param offset The amount to offset from the adapter position to scroll to.
 */
ECode CListView::SmoothScrollByOffset(
    /* [in] */ Int32 position)
{
    return ListView::SmoothScrollByOffset(position);
}

/**
 * Sets the selected item and positions the selection y pixels from the top edge
 * of the ListView. (If in touch mode, the item will not be selected but it will
 * still be positioned appropriately.)
 *
 * @param position Index (starting at 0) of the data item to be selected.
 * @param y The distance from the top edge of the ListView (plus padding) that the
 *        item will be positioned.
 */
ECode CListView::SetSelectionFromTop(
    /* [in] */ Int32 position,
    /* [in] */ Int32 y)
{
    return ListView::SetSelectionFromTop(position, y);
}

/**
 * setSelectionAfterHeaderView set the selection to be the first list item
 * after the header views.
 */
ECode CListView::SetSelectionAfterHeaderView()
{
    return ListView::SetSelectionAfterHeaderView();
}

/**
 * Indicates that the views created by the ListAdapter can contain focusable
 * items.
 *
 * @param itemsCanFocus TRUE if items can Get focus, FALSE otherwise
 */
ECode CListView::SetItemsCanFocus(
    /* [in] */ Boolean itemsCanFocus)
{
    return ListView::SetItemsCanFocus(itemsCanFocus);
}

/**
 * @return Whether the views created by the ListAdapter can contain focusable
 * items.
 */
ECode CListView::GetItemsCanFocus(
    /* [out] */ Boolean* canFocus)
{
    VALIDATE_NOT_NULL(canFocus);
    *canFocus = ListView::GetItemsCanFocus();
    return NOERROR;
}

/**
 * Returns the drawable that will be drawn between each item in the list.
 *
 * @return the current drawable drawn between list elements
 */
ECode CListView::GetDivider(
    /* [out] */ IDrawable** divider)
{
    VALIDATE_NOT_NULL(divider);
    AutoPtr<IDrawable> temp = ListView::GetDivider();
    *divider = temp;
    INTERFACE_ADDREF(*divider)
    return NOERROR;
}

/**
 * Sets the drawable that will be drawn between each item in the list. If the drawable does
 * not have an intrinsic height, you should also call {@link #setDividerHeight(Int32)}
 *
 * @param divider The drawable to use.
 */
ECode CListView::SetDivider(
    /* [in] */ IDrawable* divider)
{
    return ListView::SetDivider(divider);
}

/**
 * @return Returns the height of the divider that will be drawn between each item in the list.
 */
ECode CListView::GetDividerHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    *height = ListView::GetDividerHeight();
    return NOERROR;
}

/**
 * Sets the height of the divider that will be drawn between each item in the list. Calling
 * this will override the intrinsic height as set by {@link #setDivider(Drawable)}
 *
 * @param height The new height of the divider in pixels.
 */
ECode CListView::SetDividerHeight(
    /* [in] */ Int32 height)
{
    return ListView::SetDividerHeight(height);
}

/**
 * Enables or disables the drawing of the divider for header views.
 *
 * @param headerDividersEnabled True to draw the headers, FALSE otherwise.
 *
 * @see #setFooterDividersEnabled(Boolean)
 * @see #addHeaderView(android.view.View)
 */
ECode CListView::SetHeaderDividersEnabled(
    /* [in] */ Boolean headerDividersEnabled)
{
    return ListView::SetHeaderDividersEnabled(headerDividersEnabled);
}

/**
 * Enables or disables the drawing of the divider for footer views.
 *
 * @param footerDividersEnabled True to draw the footers, FALSE otherwise.
 *
 * @see #setHeaderDividersEnabled(Boolean)
 * @see #addFooterView(android.view.View)
 */
ECode CListView::SetFooterDividersEnabled(
    /* [in] */ Boolean footerDividersEnabled)
{
    return ListView::SetFooterDividersEnabled(footerDividersEnabled);
}

/**
 * Sets the drawable that will be drawn above all other list content.
 * This area can become visible when the user overscrolls the list.
 *
 * @param header The drawable to use
 */
ECode CListView::SetOverscrollHeader(
    /* [in] */ IDrawable* header)
{
    return ListView::SetOverscrollHeader(header);
}

/**
 * @return The drawable that will be drawn above all other list content
 */
ECode CListView::GetOverscrollHeader(
    /* [out] */ IDrawable** overScrollHeader)
{
    VALIDATE_NOT_NULL(overScrollHeader);
    AutoPtr<IDrawable> temp = ListView::GetOverscrollHeader();
    *overScrollHeader = temp;
    INTERFACE_ADDREF(*overScrollHeader)
    return NOERROR;
}

/**
 * Sets the drawable that will be drawn below all other list content.
 * This area can become visible when the user overscrolls the list,
 * or when the list's content does not fully fill the container area.
 *
 * @param footer The drawable to use
 */
ECode CListView::SetOverscrollFooter(
    /* [in] */ IDrawable* footer)
{
    return ListView::SetOverscrollFooter(footer);
}

/**
 * @return The drawable that will be drawn below all other list content
 */
ECode CListView::GetOverscrollFooter(
    /* [out] */ IDrawable** overScrollFooter)
{
    VALIDATE_NOT_NULL(overScrollFooter);
    AutoPtr<IDrawable> temp = ListView::GetOverscrollFooter();
    *overScrollFooter = temp;
    INTERFACE_ADDREF(*overScrollFooter)
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
