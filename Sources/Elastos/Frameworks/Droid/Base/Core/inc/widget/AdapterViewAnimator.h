
#ifndef __ADAPTERVIEWANIMATOR_H__
#define __ADAPTERVIEWANIMATOR_H__

#include <ext/frameworkext.h>
#include <elastos/utility/etl/HashMap.h>
#include <elastos/utility/etl/List.h>
#include <widget/AdapterView.h>

using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::Etl::List;
using Elastos::Droid::Animation::IObjectAnimator;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::View::IMotionEvent;

namespace Elastos {
namespace Droid {
namespace Widget {

class AdapterViewAnimator : public AdapterView
{
protected:
    class ViewAndMetaData
        : public ElRefBase
    {
    public:
        ViewAndMetaData(
            /* [in] */ IView* v,
            /* [in] */ Int32 relativeIndex,
            /* [in] */ Int32 adapterPosition,
            /* [in] */ Int64 itemId);

    public:
        AutoPtr<IView> mView;
        Int32 mRelativeIndex;
        Int32 mAdapterPosition;
        Int64 mItemId;
    };


private:
    class CheckForTap
        : public Runnable
    {
    public:
        CheckForTap(
            /* [in] */ AdapterViewAnimator* host);

        CARAPI Run();
    private:
        AdapterViewAnimator* mHost;
    };

    class ActionUpRun
        : public Runnable
    {
    public:
        ActionUpRun(
            /* [in] */ AdapterViewAnimator* host,
            /* [in] */ ViewAndMetaData* data,
            /* [in] */ IView* v);

        CARAPI Run();
    private:
        AdapterViewAnimator* mHost;
        AutoPtr<ViewAndMetaData> mData;
        AutoPtr<IView> mView;
    };

    class ActionUpInner
        : public Runnable
    {
    public:
        ActionUpInner(
            /* [in] */ AdapterViewAnimator* host,
            /* [in] */ ViewAndMetaData* data,
            /* [in] */ IView* v);

        CARAPI Run();
    private:
        AdapterViewAnimator* mHost;
        AutoPtr<ViewAndMetaData> mData;
        AutoPtr<IView> mView;
    };

    class CheckDataRun
        : public Runnable
    {
    public:
        CheckDataRun(
            /* [in] */ AdapterViewAnimator* host);

        CARAPI Run();
    private:
        AdapterViewAnimator* mHost;
    };

public:
    AdapterViewAnimator(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0);

    CARAPI SetDisplayedChild(
        /* [in] */ Int32 whichChild);

    CARAPI_(Int32) GetDisplayedChild();

    CARAPI ShowNext();

    CARAPI ShowPrevious();

    CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* ev);

    CARAPI_(AutoPtr<IParcelable>) OnSaveInstanceState();

    CARAPI_(void) OnRestoreInstanceState(
        /* [in] */ IParcelable* state);

    CARAPI_(AutoPtr<IView>) GetCurrentView();

    CARAPI_(AutoPtr<IObjectAnimator>) GetInAnimation();

    CARAPI SetInAnimation(
        /* [in] */ IObjectAnimator* inAnimation);

    CARAPI SetInAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resourceID);

    CARAPI_(AutoPtr<IObjectAnimator>) GetOutAnimation();

    CARAPI SetOutAnimation(
        /* [in] */ IObjectAnimator* outAnimation);

    CARAPI SetOutAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resourceID);

    CARAPI SetAnimateFirstView(
        /* [in] */ Boolean animate);

    CARAPI GetBaseline(
        /* [out] */ Int32* baseline);

    CARAPI_(AutoPtr<IAdapter>) GetAdapter();

    CARAPI SetAdapter(
        /* [in] */ IAdapter* adapter);

    CARAPI SetRemoteViewsAdapter(
        /* [in] */ IIntent* intent);

    CARAPI SetRemoteViewsOnClickHandler(
        /* [in] */ IRemoteViewsOnClickHandler* handler);

    CARAPI SetSelection(
        /* [in] */ Int32 position);

    CARAPI_(AutoPtr<IView>) GetSelectedView();

    CARAPI DeferNotifyDataSetChanged();

    CARAPI_(Boolean) OnRemoteAdapterConnected();

    CARAPI OnRemoteAdapterDisconnected();

    CARAPI Advance();

    CARAPI FyiWillBeAdvancedByHostKThx();

    CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

protected:
    AdapterViewAnimator();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0);

    CARAPI_(void) ConfigureViewAnimator(
        /* [in] */ Int32 numVisibleViews,
        /* [in] */ Int32 activeOffset);

    CARAPI_(void) TransformViewForTransition(
        /* [in] */ Int32 fromIndex,
        /* [in] */ Int64 toIndex,
        /* [in] */ IView* v,
        /* [in] */ Boolean animate);

    CARAPI_(AutoPtr<IObjectAnimator>) GetDefaultInAnimation();

    CARAPI_(AutoPtr<IObjectAnimator>) GetDefaultOutAnimation();

    virtual CARAPI_(void) ApplyTransformForChildAtIndex(
        /* [in] */ IView* child,
        /* [in] */ Int32 relativeIndex);

    CARAPI_(Int32) Modulo(
        /* [in] */ Int32 pos,
        /* [in] */ Int32 size);

    CARAPI_(AutoPtr<IView>) GetViewAtRelativeIndex(
        /* [in] */ Int32 relativeIndex);

    CARAPI_(Int32) GetNumActiveViews();

    CARAPI_(Int32) GetWindowSize();

    virtual CARAPI_(AutoPtr<IViewGroupLayoutParams>) CreateOrReuseLayoutParams(
        /* [in] */ IView* v);

    CARAPI_(void) RefreshChildren();

    virtual CARAPI_(AutoPtr<IFrameLayout>) GetFrameForChild();

    virtual CARAPI_(void) ShowOnly(
        /* [in] */ Int32 childIndex,
        /* [in] */ Boolean animate);

    virtual CARAPI_(void) ShowTapFeedback(
        /* [in] */ IView* child);

    virtual CARAPI_(void) HideTapFeedback(
        /* [in] */ IView* child);

    CARAPI_(void) CancelHandleClick();

    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    CARAPI_(void) CheckForAndHandleDataChanged();

    CARAPI_(void) OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

private:
    CARAPI_(void) InitViewAnimator();

    CARAPI_(void) SetDisplayedChild(
        /* [in] */ Int32 whichChild,
        /* [in] */ Boolean animate);

    CARAPI_(AutoPtr<ViewAndMetaData>) GetMetaDataForChild(
        /* [in] */ IView* child);

    CARAPI_(void) AddChild(
        /* [in] */ IView* child);

    CARAPI_(void) MeasureChildren();


protected:
    static const Int32 TOUCH_MODE_NONE = 0;
    static const Int32 TOUCH_MODE_DOWN_IN_CURRENT_VIEW = 1;
    static const Int32 TOUCH_MODE_HANDLED = 2;

    Int32 mWhichChild;
    Boolean mAnimateFirstTime;
    Int32 mActiveOffset;
    Int32 mMaxNumActiveViews;
    Int32 mCurrentWindowStart;
    Int32 mCurrentWindowEnd;
    Int32 mCurrentWindowStartUnbounded;

    AutoPtr<AdapterDataSetObserver> mDataSetObserver;
    AutoPtr<IAdapter> mAdapter;
    AutoPtr<IRemoteViewsAdapter> mRemoteViewsAdapter;

    Boolean mDeferNotifyDataSetChanged;
    Boolean mFirstTime;
    Boolean mLoopViews;

    Int32 mReferenceChildWidth;
    Int32 mReferenceChildHeight;

    AutoPtr<IObjectAnimator> mInAnimation;
    AutoPtr<IObjectAnimator> mOutAnimation;

    HashMap< Int32, AutoPtr<ViewAndMetaData> > mViewsMap;
    List<Int32> mPreviousViews;

private:
    static const String TAG;
    static const Int32 DEFAULT_ANIMATION_DURATION = 200;

    AutoPtr<IRunnable> mPendingCheckForTap;

    Int32 mRestoreWhichChild;
    Int32 mTouchMode; //= TOUCH_MODE_NONE;
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__ADAPTERVIEWANIMATOR_H__