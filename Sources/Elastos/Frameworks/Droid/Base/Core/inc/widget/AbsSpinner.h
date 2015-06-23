
#ifndef __ABSSPANNER_H__
#define __ABSSPANNER_H__

/**
 * An abstract base class for spinner widgets. SDK users will probably not
 * need to use this class.
 *
 * @attr ref android.R.styleable#AbsSpinner_entries
 */

#include "widget/AdapterView.h"
#include <elastos/utility/etl/HashMap.h>
#include "R.h"

using Elastos::Droid::View::IView;
using Elastos::Core::CStringWrapper;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::EIID_IViewGroup;
using Elastos::Droid::R;

namespace Elastos {
namespace Droid {
namespace Widget {


class AbsSpinner : public AdapterView
{
public:
    class RecycleBin : public ElRefBase
    {
    public:
        RecycleBin(
            /* [in] */ AbsSpinner* mHost);

        CARAPI_(void) Put(
            /* [in] */ Int32 position,
            /* [in] */ IView* v);

        CARAPI_(AutoPtr<IView>) Get(
            /* [in] */ Int32 position);

        CARAPI_(void) Clear();

    private:
        HashMap<Int32, AutoPtr<IView> > mScrapHeap;
        AbsSpinner* mHost;
    };

public:
    AbsSpinner(
        /* [in] */ IContext* context);

    AbsSpinner(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle = 0);

    virtual ~AbsSpinner();

    /**
     * The Adapter is used to provide the data which backs this Spinner.
     * It also provides methods to transform spinner items based on their position
     * relative to the selected item.
     * @param adapter The SpinnerAdapter to use for this Spinner
     */
    //@Override
    CARAPI SetAdapter(
        /* [in] */ IAdapter* adapter);

    /**
     * Clear out all children from the list
     */
    virtual CARAPI_(void) ResetList();

    virtual CARAPI_(Int32) GetChildHeight(
        /* [in] */ IView* child);

    virtual CARAPI_(Int32) GetChildWidth(
        /* [in] */ IView* child);

    virtual CARAPI_(void) RecycleAllViews();

    /**
     * Jump directly to a specific item in the adapter data.
     */
    virtual CARAPI SetSelection(
        /* [in] */ Int32 position,
        /* [in] */ Boolean animate);

    //@Override
    virtual CARAPI SetSelection(
        /* [in] */ Int32 position);

    /**
     * Makes the item at the supplied position selected.
     *
     * @param position Position to select
     * @param animate Should the transition be animated
     *
     */
    CARAPI_(void) SetSelectionInt(
        /* [in] */ Int32 position,
        /* [in] */ Boolean animate);

    virtual CARAPI_(void) Layout(
        /* [in] */ Int32 delta,
        /* [in] */ Boolean animate) = 0;

    //@Override
    CARAPI_(AutoPtr<IView>) GetSelectedView();

    /**
     * Override to prevent spamming ourselves with layout requests
     * as we place views
     *
     * @see android.view.View#requestLayout()
     */
    //@Override
    CARAPI RequestLayout();

    //@Override
    CARAPI_(AutoPtr<IAdapter>) GetAdapter();

    //@Override
    CARAPI_(Int32) GetCount();

    /**
     * Maps a point to a position in the list.
     *
     * @param x X in local coordinate
     * @param y Y in local coordinate
     * @return The position of the item which contains the specified point, or
     *         {@link #INVALID_POSITION} if the point does not intersect an item.
     */
    virtual CARAPI_(Int32) PointToPosition(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    //@Override
    CARAPI_(AutoPtr<IParcelable>) OnSaveInstanceState();

    //@Override
    CARAPI_(void) OnRestoreInstanceState(
        /* [in] */ IParcelable* state);

    virtual CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

protected:
    AbsSpinner();

    /**
     * @see android.view.View#measure(int, int)
     *
     * Figure out the dimensions of this Spinner. The width comes from
     * the widthMeasureSpec as Spinnners can't have their width set to
     * UNSPECIFIED. The height is based on the height of the selected item
     * plus padding.
     */
    //@Override
    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    //@Override
    CARAPI GenerateDefaultLayoutParams(
        /* [out] */ IViewGroupLayoutParams** params);

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle = 0);

private:
    /**
     * Common code for different constructor flavors
     */
    CARAPI_(void) InitAbsSpinner();

    CARAPI InitFromAttributes(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

public:
    AutoPtr<ISpinnerAdapter> mAdapter;

    Int32 mHeightMeasureSpec;
    Int32 mWidthMeasureSpec;

    Int32 mSelectionLeftPadding;
    Int32 mSelectionTopPadding;
    Int32 mSelectionRightPadding;
    Int32 mSelectionBottomPadding;
    AutoPtr<CRect> mSpinnerPadding;

    AutoPtr<RecycleBin> mRecycler;

private:
    AutoPtr<IDataSetObserver> mDataSetObserver;

    /** Temporary frame to hold a child View's frame rectangle */
    AutoPtr<IRect> mTouchFrame;

};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ABSSPANNER_H__

