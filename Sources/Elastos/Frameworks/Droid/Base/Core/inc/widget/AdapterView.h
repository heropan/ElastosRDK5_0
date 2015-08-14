
#ifndef __ELASTOS_DROID_WIDGET_ADAPTERVIEW_H__
#define __ELASTOS_DROID_WIDGET_ADAPTERVIEW_H__

#ifdef DROID_CORE
#include "Elastos.Droid.Core_server.h"
#else
#include "Elastos.Droid.Core.h"
#endif

#include "os/Runnable.h"
#include "view/ViewGroup.h"
#include "R.h"

using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Database::IDataSetObserver;
using Elastos::Droid::Database::EIID_IDataSetObserver;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::ViewGroup;
using Elastos::Droid::View::IViewParent;
using Elastos::Droid::View::IContextMenuInfo;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::View::EIID_IContextMenuInfo;
using Elastos::Droid::View::IViewGroupLayoutParams;


namespace Elastos{
namespace Droid{
namespace Widget{


class AdapterView : public ViewGroup
{
public:
    class AdapterContextMenuInfo
        : public IAdapterContextMenuInfo
        , public IContextMenuInfo
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL();

        AdapterContextMenuInfo(
            /* [in] */ IView* targetView,
            /* [in] */ Int32 position,
            /* [in] */ Int64 id);

        CARAPI GetTargetView(
            /* [out] */ IView** targetView);

        CARAPI SetTargetView(
            /* [in] */ IView* targetView);

        CARAPI GetPosition(
            /* [out] */ Int32* position);

        CARAPI SetPosition(
            /* [in] */ Int32 position);

        CARAPI GetId(
            /* [out] */ Int64* id);

        CARAPI SetId(
            /* [in] */ Int64 id);

    public:
        /**
         * The child view for which the context menu is being displayed. This
         * will be one of the children of this AdapterView.
         */
        AutoPtr<IView> mTargetView;

        /**
         * The position in the adapter for which the context menu is being
         * displayed.
         */
        Int32 mPosition;

        /**
         * The row id of the item for which the context menu is being displayed.
         */
        Int64 mId;
    };

protected:
    class AdapterDataSetObserver : public ElRefBase, public IDataSetObserver
    {
    public:
        CAR_INTERFACE_DECL();

        AdapterDataSetObserver(
            /* [in] */ AdapterView* host)
            : mHost(host)
        {
            assert(host);
        }

        CARAPI OnChanged();

        CARAPI OnInvalidated();

        CARAPI ClearSavedState();

    private:
        AutoPtr<IParcelable> mInstanceState;
    protected:
        AdapterView* mHost;
    };

private:
    class SelectionNotifier : public Runnable
    {
    public:
        SelectionNotifier(
            /* [in] */ AdapterView* host);

        CARAPI Run();

    private:
        AdapterView* mHost;
    };

protected:

    AdapterView();

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

public:

    AdapterView(
        /* [in] */ IContext* context);

    AdapterView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    AdapterView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    virtual CARAPI SetOnItemClickListener(
        /* [in] */ IAdapterViewOnItemClickListener* listener);

    CARAPI_(AutoPtr<IAdapterViewOnItemClickListener>) GetOnItemClickListener();

    virtual CARAPI_(Boolean) PerformItemClick(
        /* [in] */ IView* view,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

    virtual CARAPI SetOnItemLongClickListener(
        /* [in] */ IAdapterViewOnItemLongClickListener* listener);

    CARAPI_(AutoPtr<IAdapterViewOnItemLongClickListener>) GetOnItemLongClickListener();

    virtual CARAPI SetOnItemSelectedListener(
        /* [in] */ IAdapterViewOnItemSelectedListener* listener);

    CARAPI_(AutoPtr<IAdapterViewOnItemSelectedListener>) GetOnItemSelectedListener();

    //@Override
    CARAPI AddView(
        /* [in] */ IView* child);

    //@Override
    CARAPI AddView(
        /* [in] */ IView* child,
        /* [in] */ Int32 index);

    //@Override
    CARAPI AddView(
        /* [in] */ IView* child,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    //@Override
    CARAPI AddView(
        /* [in] */ IView* child,
        /* [in] */ IViewGroupLayoutParams* params);

    //@Override
    CARAPI AddView(
        /* [in] */ IView* child,
        /* [in] */ Int32 index,
        /* [in] */ IViewGroupLayoutParams* params);

    //@Override
    CARAPI RemoveView(
        /* [in] */ IView* child);

    //@Override
    CARAPI RemoveViewAt(
        /* [in] */ Int32 index);

    //@Override
    CARAPI RemoveAllViews();

    virtual CARAPI_(Int32) GetSelectedItemPosition();

    virtual CARAPI_(Int64) GetSelectedItemId();

    virtual CARAPI_(AutoPtr<IView>) GetSelectedView() = 0;

    virtual CARAPI_(AutoPtr<IInterface>) GetSelectedItem();

    virtual CARAPI_(Int32) GetCount();

    virtual CARAPI_(Int32) GetPositionForView(
        /* [in] */ IView* view);

    virtual CARAPI_(Int32) GetFirstVisiblePosition();

    virtual CARAPI_(Int32) GetLastVisiblePosition();

    virtual CARAPI SetSelection(
        /* [in] */ Int32 position) = 0;

    virtual CARAPI SetEmptyView(
        /* [in] */ IView* emptyView);

    virtual CARAPI_(AutoPtr<IView>) GetEmptyView();

    virtual CARAPI_(Boolean) IsInFilterMode();

    //@Override
    CARAPI SetFocusable(
        /* [in] */ Boolean focusable);

    //@Override
    CARAPI SetFocusableInTouchMode(
        /* [in] */ Boolean focusable);

    virtual CARAPI CheckFocus();

    virtual CARAPI_(AutoPtr<IInterface>) GetItemAtPosition(
        /* [in] */ Int32 position);

    virtual CARAPI_(Int64) GetItemIdAtPosition(
        /* [in] */ Int32 position);

    //@Override
    CARAPI SetOnClickListener(
        /* [in] */ IViewOnClickListener* l);

    //@Override
    CARAPI_(Boolean) DispatchPopulateAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    CARAPI_(Boolean) OnRequestSendAccessibilityEvent(
        /* [in] */ IView* child,
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

    virtual CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

protected:
    //@Override
    CARAPI_(void) OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    //@Override
    CARAPI DispatchSaveInstanceState(
        /* [in] */ IObjectInt32Map* container);

    //@Override
    CARAPI DispatchRestoreInstanceState(
        /* [in] */ IObjectInt32Map* container);

    //@Override
    virtual CARAPI OnDetachedFromWindow();

    virtual CARAPI_(void) SelectionChanged();

    //@Override
    CARAPI_(Boolean) CanAnimate();

    virtual CARAPI_(void) HandleDataChanged();

    virtual CARAPI_(void) CheckSelectionChanged();

    virtual CARAPI_(Int32) FindSyncPosition();

    virtual CARAPI_(Int32) LookForSelectablePosition(
        /* [in] */ Int32 position,
        /* [in] */ Boolean lookDown);

    virtual CARAPI_(void) SetSelectedPositionInt(
        /* [in] */ Int32 position);

    virtual CARAPI_(void) SetNextSelectedPositionInt(
        /* [in] */ Int32 position);

    virtual CARAPI_(void) RememberSyncState();

private:
    CARAPI_(void) UpdateEmptyStatus(
        /* [in] */ Boolean empty);

    CARAPI_(void) FireOnSelected();

    CARAPI_(void) PerformAccessibilityActionsOnSelected();

    CARAPI_(Boolean) IsScrollableForAccessibility();

public:
    /**
     * Maximum amount of time to spend in {@link #findSyncPosition()}
     */
    static const Int32 SYNC_MAX_DURATION_MILLIS = 100;

protected:
    /**
     * Sync based on the selected child
     */
    static const Int32 SYNC_SELECTED_POSITION = 0;

    /**
     * Sync based on the first child displayed
     */
    static const Int32 SYNC_FIRST_POSITION = 1;

protected:
    /**
     * The position of the first child displayed
     */
    Int32 mFirstPosition;

    /**
     * The offset in pixels from the top of the AdapterView to the top
     * of the view to select during the next layout.
     */
    Int32 mSpecificTop;

    /**
     * Position from which to start looking for mSyncRowId
     */
    Int32 mSyncPosition;

    /**
     * Row id to look for when data has changed
     */
    Int64 mSyncRowId;

    /**
     * Height of the view when mSyncPosition and mSyncRowId where set
     */
    Int64 mSyncHeight;

    /**
     * True if we need to sync to mSyncRowId
     */
    Boolean mNeedSync;

    /**
     * Indicates whether to sync based on the selection or position. Possible
     * values are {@link #SYNC_SELECTED_POSITION} or
     * {@link #SYNC_FIRST_POSITION}.
     */
    Int32 mSyncMode;

    /**
     * Indicates that this view is currently being laid out.
     */
    Boolean mInLayout;

    /**
     * The listener that receives notifications when an item is selected.
     */
    AutoPtr<IAdapterViewOnItemSelectedListener> mOnItemSelectedListener;

    /**
     * The listener that receives notifications when an item is clicked.
     */
    AutoPtr<IAdapterViewOnItemClickListener> mOnItemClickListener;

    /**
     * The listener that receives notifications when an item is Int64 clicked.
     */
    AutoPtr<IAdapterViewOnItemLongClickListener> mOnItemLongClickListener;

    /**
     * True if the data has changed since the last layout
     */
    Boolean mDataChanged;

    /**
     * The position within the adapter's data set of the item to select
     * during the next layout.
     */
    Int32 mNextSelectedPosition;

    /**
     * The item id of the item to select during the next layout.
     */
    Int64 mNextSelectedRowId;

    /**
     * The position within the adapter's data set of the currently selected item.
     */
    Int32 mSelectedPosition;

    /**
     * The item id of the currently selected item.
     */
    Int64 mSelectedRowId;

    /**
     * The number of items in the current adapter.
     */
    Int32 mItemCount;

    /**
     * The number of items in the adapter before a data changed event occured.
     */
    Int32 mOldItemCount;

    /**
     * The last selected position we used when notifying
     */
    Int32 mOldSelectedPosition;

    /**
     * The id of the last selected position we used when notifying
     */
    Int64 mOldSelectedRowId ;

    /**
     * When set to TRUE, calls to requestLayout() will not propagate up the parent hierarchy.
     * This is used to layout the children during a layout pass.
     */
    Boolean mBlockLayoutRequests;

private:
    /**
     * Our height after the last layout
     */
    Int32 mLayoutHeight;

    /**
     * View to show if there are no items to show.
     */
    AutoPtr<IView> mEmptyView;

    /**
     * Indicates what focusable state is requested when calling setFocusable().
     * In addition to this, this view has other criteria for actually
     * determining the focusable state (such as whether its empty or the text
     * filter is shown).
     *
     * @see #setFocusable(Boolean)
     * @see #checkFocus()
     */
    Boolean mDesiredFocusableState;
    Boolean mDesiredFocusableInTouchModeState;

    AutoPtr<SelectionNotifier> mSelectionNotifier;
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif  //__ELASTOS_DROID_WIDGET_ADAPTERVIEW_H__
