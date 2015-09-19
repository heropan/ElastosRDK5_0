
#ifndef __ELASTOS_DROID_WIDGET_FASTSCROLLER_H__
#define __ELASTOS_DROID_WIDGET_FASTSCROLLER_H__

#include "ext/frameworkext.h"
#include "os/Runnable.h"

using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::IPaint;
using Elastos::Droid::Graphics::IRectF;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::View::IMotionEvent;

namespace Elastos {
namespace Droid {
namespace Widget {

class AbsListView;
/**
 * Helper class for AbsListView to draw and control the Fast Scroll thumb
 */
class FastScroller : public ElRefBase
{
    friend class AbsListView;

public:
    class DeferStartDragRunnable : public Runnable
    {
    public:
        DeferStartDragRunnable(
            /* [in] */ FastScroller* host);

        CARAPI Run();

    private:
        FastScroller* mHost;
    };

    class ScrollFade : public Runnable
    {
        friend class FastScroller;
    protected:
        static const Int32 ALPHA_MAX = 208;
        static const Int64 FADE_DURATION = 200;

    public:
        ScrollFade(
            /* [in] */ FastScroller* host);

        CARAPI Run();

    protected:
        CARAPI_(void) StartFade();

        CARAPI_(Int32) GetAlpha();

    protected:
        Int64 mStartTime;
        Int64 mFadeDuration;

    private:
        FastScroller* mHost;
    };

public:
    FastScroller(
        /* [in] */ IContext* context,
        /* [in] */ AbsListView* listView);

    CARAPI_(void) SetAlwaysShow(
        /* [in] */ Boolean alwaysShow);

    CARAPI_(Boolean) IsAlwaysShowEnabled();

    CARAPI_(void) SetScrollbarPosition(
        /* [in] */ Int32 position);

    CARAPI_(Int32) GetWidth();

    CARAPI_(void) SetState(
        /* [in] */ Int32 state);

    CARAPI_(Int32) GetState();

    CARAPI_(void) Draw(
        /* [in] */ ICanvas* canvas);

    CARAPI_(void) OnSectionsChanged();

protected:
    CARAPI_(void) Stop();

    CARAPI_(Boolean) IsVisible();

    CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 oldw,
        /* [in] */ Int32 oldh);

    CARAPI_(void) OnScroll(
        /* [in] */ IAbsListView* view,
        /* [in] */ Int32 firstVisibleItem,
        /* [in] */ Int32 visibleItemCount,
        /* [in] */ Int32 totalItemCount);

    CARAPI_(AutoPtr<ISectionIndexer>) GetSectionIndexer();

    CARAPI_(AutoPtr<ArrayOf<IInterface*> >) GetSections();

    CARAPI_(Boolean) OnInterceptTouchEvent(
        /* [in] */ IMotionEvent* ev);

    CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* me);

    CARAPI_(Boolean) IsPointInside(
        /* [in] */ Float x,
        /* [in] */ Float y);

private:
    CARAPI_(void) RefreshDrawableState();

    CARAPI_(void) ResetThumbPos();

    CARAPI_(void) UseThumbDrawable(
        /* [in] */ IContext* context,
        /* [in] */ IDrawable* drawable);

    CARAPI_(void) Init(
        /* [in] */ IContext* context);

    CARAPI_(void) GetSectionsFromIndexer();

    CARAPI_(void) ScrollTo(
        /* [in] */ Float position);

    CARAPI_(void) CancelFling();

    CARAPI_(Boolean) PostDelayed(
        /* [in] */ Int64 delayMillis);

    CARAPI RemoveCallbacks();

    CARAPI_(void) OnItemCountChanged(
        /* [in] */ Int32 oldCount,
        /* [in] */ Int32 newCount);

    CARAPI_(Int32) GetThumbPositionForListPosition(
        /* [in] */ Int32 firstVisibleItem,
        /* [in] */ Int32 visibleItemCount,
        /* [in] */ Int32 totalItemCount);

    CARAPI_(void) CancelPendingDrag();

    CARAPI_(void) StartPendingDrag();

    CARAPI_(void) BeginDrag();

private:
    static const String TAG;// = "FastScroller";

    // Minimum number of pages to justify showing a fast scroll thumb
    static const Int32 MIN_PAGES = 4;
    // Scroll thumb not showing
    static const Int32 STATE_NONE = 0;
    // Not implemented yet - fade-in transition
    static const Int32 STATE_ENTER = 1;
    // Scroll thumb visible and moving along with the scrollbar
    static const Int32 STATE_VISIBLE = 2;
    // Scroll thumb being dragged by user
    static const Int32 STATE_DRAGGING = 3;
    // Scroll thumb fading out due to inactivity timeout
    static const Int32 STATE_EXIT = 4;

    static AutoPtr<ArrayOf<Int32> > PRESSED_STATES; //new Int32[] { android.R.attr.state_pressed };

    static AutoPtr<ArrayOf<Int32> > DEFAULT_STATES;// = new Int32[0];

    static AutoPtr<ArrayOf<Int32> > ATTRS;

    static const Int32 TEXT_COLOR = 0;
    static const Int32 THUMB_DRAWABLE = 1;
    static const Int32 TRACK_DRAWABLE = 2;
    static const Int32 PREVIEW_BACKGROUND_LEFT = 3;
    static const Int32 PREVIEW_BACKGROUND_RIGHT = 4;
    static const Int32 OVERLAY_POSITION = 5;

    static const Int32 OVERLAY_FLOATING = 0;
    static const Int32 OVERLAY_AT_THUMB = 1;

    static const Int32 FADE_TIMEOUT = 1500;
    static const Int32 PENDING_DRAG_DELAY = 180;

    AbsListView* mList;

    AutoPtr<IDrawable> mThumbDrawable;
    AutoPtr<IDrawable> mOverlayDrawable;
    AutoPtr<IDrawable> mTrackDrawable;

    AutoPtr<IDrawable> mOverlayDrawableLeft;
    AutoPtr<IDrawable> mOverlayDrawableRight;

    Int32 mThumbH;
    Int32 mThumbW;
    Int32 mThumbY;

    AutoPtr<IRectF> mOverlayPos;
    Int32 mOverlaySize;

    Boolean mScrollCompleted;
    Int32 mVisibleItem;
    AutoPtr<IPaint> mPaint;
    Int32 mListOffset;
    Int32 mItemCount;
    Boolean mLongList;

    AutoPtr<ArrayOf<IInterface*> > mSections;
    String mSectionText;
    Boolean mDrawOverlay;
    AutoPtr<ScrollFade> mScrollFade;
    AutoPtr<DeferStartDragRunnable> mDeferStartDrag;

    Int32 mState;

    AutoPtr<IHandler> mHandler;

    AutoPtr<IBaseAdapter> mListAdapter;

    AutoPtr<ISectionIndexer> mSectionIndexer;

    Boolean mChangedBounds;

    Int32 mPosition;

    Boolean mAlwaysShow;

    Int32 mOverlayPosition;

    Boolean mMatchDragPosition;

    Float mInitialTouchY;
    Boolean mPendingDrag;
    Int32 mScaledTouchSlop;

    AutoPtr<IRect> mTmpRect;
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_FASTSCROLLER_H__
