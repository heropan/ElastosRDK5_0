#ifndef __ELASTOS_DROID_APP_FRAGMENT_BREADCRUMBS_H__
#define __ELASTOS_DROID_APP_FRAGMENT_BREADCRUMBS_H__

#include "elastos/droid/view/ViewGroup.h"

using Elastos::Droid::Animation::ILayoutTransition;
using Elastos::Droid::App::IFragmentManagerBackStackEntry;
using Elastos::Droid::App::IFragmentManagerOnBackStackChangedListener;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Util::IAttributeSet;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::Widget::ILinearLayout;
using Elastos::Droid::Widget::ITextView;

namespace Elastos {
namespace Droid {
namespace App {

/**
 * Helper class for showing "bread crumbs" representing the fragment
 * stack in an activity.  This is intended to be used with
 * {@link ActionBar#setCustomView(View)
 * ActionBar.setCustomView(View)} to place the bread crumbs in
 * the action bar.
 *
 * <p>The default style for this view is
 * {@link android.R.style#Widget_FragmentBreadCrumbs}.
 *
 * @deprecated This widget is no longer supported.
 */
class FragmentBreadCrumbs
    : public ViewGroup
    , public IFragmentBreadCrumbs
    , public IFragmentManagerOnBackStackChangedListener
{
private:
    class MyOnClickListener
        : public Object
        , public IViewOnClickListener()
    {
    public:
        CAR_INTERFACE_DECL()

        MyOnClickListener(
            /* [in] */ FragmentBreadCrumbs* host);

        CARAPI onClick(View v) {
            if (v.getTag() instanceof BackStackEntry) {
                BackStackEntry bse = (BackStackEntry) v.getTag();
                if (bse == mParentEntry) {
                    if (mParentClickListener != null) {
                        mParentClickListener.onClick(v);
                    }
                } else {
                    if (mOnBreadCrumbClickListener != null) {
                        if (mOnBreadCrumbClickListener.onBreadCrumbClick(
                                bse == mTopEntry ? null : bse, 0)) {
                            return;
                        }
                    }
                    if (bse == mTopEntry) {
                        // Pop everything off the back stack.
                        mActivity.getFragmentManager().popBackStack();
                    } else {
                        mActivity.getFragmentManager().popBackStack(bse.getId(), 0);
                    }
                }
            }
        }
    private:
        FragmentBreadCrumbs* mHost;
    };

public:
    CAR_INTERFACE_DECL()

    FragmentBreadCrumbs();


    CARAPI constructor(
        /*[in] */ IContext* ctx)
    {
        this(ctx, null);
    }

    CARAPI constructor(
        /*[in] */ IContext* ctx,
        /*[in] */ IAttributeSet* attrs)
    {
        this(ctx, attrs, com.android.internal.R.attr.fragmentBreadCrumbsStyle);
    }

    CARAPI constructor(
        /*[in] */ IContext* ctx,
        /*[in] */ IAttributeSet* attrs,
         /*[in] */ Int32 defStyleAttr)
    {
        this(ctx, attrs, defStyleAttr, 0);
    }

    /**
     * @hide
     */
    CARAPI constructor(
            /*[in] */ IContext* ctx,
            /*[in] */ IAttributeSet* attrs,
            /*[in] */ Int32 defStyleAttr,
            /*[in] */ Int32 defStyleRes)
    {
        super(context, attrs, defStyleAttr, defStyleRes);

        final TypedArray a = context.obtainStyledAttributes(attrs,
                com.android.internal.R.styleable.FragmentBreadCrumbs, defStyleAttr, defStyleRes);

        mGravity = a.getInt(com.android.internal.R.styleable.FragmentBreadCrumbs_gravity,
                DEFAULT_GRAVITY);
        mLayoutResId = a.getResourceId(
                com.android.internal.R.styleable.FragmentBreadCrumbs_itemLayout,
                com.android.internal.R.layout.fragment_bread_crumb_item);
        mTextColor = a.getColor(
                com.android.internal.R.styleable.FragmentBreadCrumbs_itemColor,
                0);

        a.recycle();
    }

    /**
     * Attach the bread crumbs to their activity.  This must be called once
     * when creating the bread crumbs.
     */
    CARAPI SetActivity(
        /* [in] */ IActivity* a)
    {
        mActivity = a;
        mInflater = (LayoutInflater)a.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        mContainer = (LinearLayout)mInflater.inflate(
                com.android.internal.R.layout.fragment_bread_crumbs,
                this, false);
        addView(mContainer);
        a.getFragmentManager().addOnBackStackChangedListener(this);
        updateCrumbs();
        setLayoutTransition(new LayoutTransition());
    }

    /**
     * The maximum number of breadcrumbs to show. Older fragment headers will be hidden from view.
     * @param visibleCrumbs the number of visible breadcrumbs. This should be greater than zero.
     */
    CARAPI SetMaxVisible(
        /* [in] */ Int32 visibleCrumbs)
    {
        if (visibleCrumbs < 1) {
            throw new IllegalArgumentException("visibleCrumbs must be greater than zero");
        }
        mMaxVisible = visibleCrumbs;
    }

    /**
     * Inserts an optional parent entry at the first position in the breadcrumbs. Selecting this
     * entry will result in a call to the specified listener's
     * {@link android.view.View.OnClickListener#onClick(View)}
     * method.
     *
     * @param title the title for the parent entry
     * @param shortTitle the short title for the parent entry
     * @param listener the {@link android.view.View.OnClickListener} to be called when clicked.
     * A null will result in no action being taken when the parent entry is clicked.
     */
    CARAPI SetParentTitle(
        /* [in] */ ICharSequence* title,
        /* [in] */ ICharSequence* shortTitle,
        /* [in] */ IViewOnClickListener* listener)
    {
        mParentEntry = createBackStackEntry(title, shortTitle);
        mParentClickListener = listener;
        updateCrumbs();
    }

    /**
     * Sets a listener for clicks on the bread crumbs.  This will be called before
     * the default click action is performed.
     *
     * @param listener The new listener to set.  Replaces any existing listener.
     */
    CARAPI SetOnBreadCrumbClickListener(
        /* [in] */ IOnBreadCrumbClickListener* listener)
    {
        mOnBreadCrumbClickListener = listener;
    }

    private BackStackRecord createBackStackEntry(CharSequence title, CharSequence shortTitle) {
        if (title == null) return null;

        final BackStackRecord entry = new BackStackRecord(
                (FragmentManagerImpl) mActivity.getFragmentManager());
        entry.setBreadCrumbTitle(title);
        entry.setBreadCrumbShortTitle(shortTitle);
        return entry;
    }

    /**
     * Set a custom title for the bread crumbs.  This will be the first entry
     * shown at the left, representing the root of the bread crumbs.  If the
     * title is null, it will not be shown.
     */
    CARAPI setTitle(CharSequence title, CharSequence shortTitle) {
        mTopEntry = createBackStackEntry(title, shortTitle);
        updateCrumbs();
    }

    @Override
    protected void onLayout(boolean changed, Int32 l, Int32 t, Int32 r, Int32 b) {
        // Eventually we should implement our own layout of the views, rather than relying on
        // a single linear layout.
        final Int32 childCount = getChildCount();
        if (childCount == 0) {
            return;
        }

        final View child = getChildAt(0);

        final Int32 childTop = mPaddingTop;
        final Int32 childBottom = mPaddingTop + child.getMeasuredHeight() - mPaddingBottom;

        Int32 childLeft;
        Int32 childRight;

        final Int32 layoutDirection = getLayoutDirection();
        final Int32 horizontalGravity = mGravity & Gravity.RELATIVE_HORIZONTAL_GRAVITY_MASK;
        switch (Gravity.getAbsoluteGravity(horizontalGravity, layoutDirection)) {
            case Gravity.RIGHT:
                childRight = mRight - mLeft - mPaddingRight;
                childLeft = childRight - child.getMeasuredWidth();
                break;

            case Gravity.CENTER_HORIZONTAL:
                childLeft = mPaddingLeft + (mRight - mLeft - child.getMeasuredWidth()) / 2;
                childRight = childLeft + child.getMeasuredWidth();
                break;

            case Gravity.LEFT:
            default:
                childLeft = mPaddingLeft;
                childRight = childLeft + child.getMeasuredWidth();
                break;
        }

        if (childLeft < mPaddingLeft) {
            childLeft = mPaddingLeft;
        }

        if (childRight > mRight - mLeft - mPaddingRight) {
            childRight = mRight - mLeft - mPaddingRight;
        }

        child.layout(childLeft, childTop, childRight, childBottom);
    }

    @Override
    protected void onMeasure(Int32 widthMeasureSpec, Int32 heightMeasureSpec) {
        final Int32 count = getChildCount();

        Int32 maxHeight = 0;
        Int32 maxWidth = 0;
        Int32 measuredChildState = 0;

        // Find rightmost and bottom-most child
        for (Int32 i = 0; i < count; i++) {
            final View child = getChildAt(i);
            if (child.getVisibility() != GONE) {
                measureChild(child, widthMeasureSpec, heightMeasureSpec);
                maxWidth = Math.max(maxWidth, child.getMeasuredWidth());
                maxHeight = Math.max(maxHeight, child.getMeasuredHeight());
                measuredChildState = combineMeasuredStates(measuredChildState,
                        child.getMeasuredState());
            }
        }

        // Account for padding too
        maxWidth += mPaddingLeft + mPaddingRight;
        maxHeight += mPaddingTop + mPaddingBottom;

        // Check against our minimum height and width
        maxHeight = Math.max(maxHeight, getSuggestedMinimumHeight());
        maxWidth = Math.max(maxWidth, getSuggestedMinimumWidth());

        setMeasuredDimension(resolveSizeAndState(maxWidth, widthMeasureSpec, measuredChildState),
                resolveSizeAndState(maxHeight, heightMeasureSpec,
                        measuredChildState<<MEASURED_HEIGHT_STATE_SHIFT));
    }

    @Override
    CARAPI onBackStackChanged() {
        updateCrumbs();
    }

    /**
     * Returns the number of entries before the backstack, including the title of the current
     * fragment and any custom parent title that was set.
     */
    private Int32 getPreEntryCount() {
        return (mTopEntry != null ? 1 : 0) + (mParentEntry != null ? 1 : 0);
    }

    /**
     * Returns the pre-entry corresponding to the index. If there is a parent and a top entry
     * set, parent has an index of zero and top entry has an index of 1. Returns null if the
     * specified index doesn't exist or is null.
     * @param index should not be more than {@link #getPreEntryCount()} - 1
     */
    private BackStackEntry getPreEntry(Int32 index) {
        // If there's a parent entry, then return that for zero'th item, else top entry.
        if (mParentEntry != null) {
            return index == 0 ? mParentEntry : mTopEntry;
        } else {
            return mTopEntry;
        }
    }

    void updateCrumbs() {
        FragmentManager fm = mActivity.getFragmentManager();
        Int32 numEntries = fm.getBackStackEntryCount();
        Int32 numPreEntries = getPreEntryCount();
        Int32 numViews = mContainer.getChildCount();
        for (Int32 i = 0; i < numEntries + numPreEntries; i++) {
            BackStackEntry bse = i < numPreEntries
                    ? getPreEntry(i)
                    : fm.getBackStackEntryAt(i - numPreEntries);
            if (i < numViews) {
                View v = mContainer.getChildAt(i);
                Object tag = v.getTag();
                if (tag != bse) {
                    for (Int32 j = i; j < numViews; j++) {
                        mContainer.removeViewAt(i);
                    }
                    numViews = i;
                }
            }
            if (i >= numViews) {
                final View item = mInflater.inflate(mLayoutResId, this, false);
                final TextView text = (TextView) item.findViewById(com.android.internal.R.id.title);
                text.setText(bse.getBreadCrumbTitle());
                text.setTag(bse);
                text.setTextColor(mTextColor);
                if (i == 0) {
                    item.findViewById(com.android.internal.R.id.left_icon).setVisibility(View.GONE);
                }
                mContainer.addView(item);
                text.setOnClickListener(mOnClickListener);
            }
        }
        Int32 viewI = numEntries + numPreEntries;
        numViews = mContainer.getChildCount();
        while (numViews > viewI) {
            mContainer.removeViewAt(numViews - 1);
            numViews--;
        }
        // Adjust the visibility and availability of the bread crumbs and divider
        for (Int32 i = 0; i < numViews; i++) {
            final View child = mContainer.getChildAt(i);
            // Disable the last one
            child.findViewById(com.android.internal.R.id.title).setEnabled(i < numViews - 1);
            if (mMaxVisible > 0) {
                // Make only the last mMaxVisible crumbs visible
                child.setVisibility(i < numViews - mMaxVisible ? View.GONE : View.VISIBLE);
                final View leftIcon = child.findViewById(com.android.internal.R.id.left_icon);
                // Remove the divider for all but the last mMaxVisible - 1
                leftIcon.setVisibility(i > numViews - mMaxVisible && i != 0 ? View.VISIBLE
                        : View.GONE);
            }
        }
    }

private:

    AutoPtr<IViewOnClickListener> mOnClickListener;

    AutoPtr<IActivity> mActivity;
    AutoPtr<ILayoutInflater> mInflater;
    AutoPtr<ILinearLayout> mContainer;
    Int32 mMaxVisible = -1;

    // Hahah
    AutoPtr<IBackStackRecord> mTopEntry;
    AutoPtr<IBackStackRecord> mParentEntry;

    /** Listener to inform when a parent entry is clicked */
    AutoPtr<IViewOnClickListener> mParentClickListener;

    AutoPtr<IOnBreadCrumbClickListener> mOnBreadCrumbClickListener;

    Int32 mGravity;
    Int32 mLayoutResId;
    Int32 mTextColor;

    static const Int32 DEFAULT_GRAVITY;// = Gravity.START | Gravity.CENTER_VERTICAL;

};

} // namespace App
} // namespace Droid
} // namespace Elastos


#endif //__ELASTOS_DROID_APP_FRAGMENT_BREADCRUMBS_H__