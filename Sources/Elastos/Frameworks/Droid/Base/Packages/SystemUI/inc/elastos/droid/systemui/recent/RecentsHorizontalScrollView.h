#ifndef __ELASTOS_DROID_SYSTEMUI_RECENT_RECENTSHORIZONTALSCROLLVIEW_H__
#define __ELASTOS_DROID_SYSTEMUI_RECENT_RECENTSHORIZONTALSCROLLVIEW_H__

#include "_SystemUI.h"
#include "Elastos.Droid.Animation.h"
#include "Elastos.Droid.Content.h"
#include "Elastos.Droid.Database.h"
#include "Elastos.Droid.Graphics.h"
#include "Elastos.Droid.Utility.h"
#include "Elastos.Droid.View.h"
#include "Elastos.Droid.Widget.h"
#include "Elastos.CoreLibrary.Core.h"
#include "Elastos.CoreLibrary.Utility.h"
#include "elastos/droid/systemui/recent/FadedEdgeDrawHelper.h"
#include "elastos/droid/widget/FrameLayout.h"
// #include "elastos/droid/widget/HorizontalScrollView.h"

using Elastos::Droid::Animation::ILayoutTransition;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Database::IDataSetObserver;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::SystemUI::ISwipeHelper;
using Elastos::Droid::SystemUI::ISwipeHelperCallback;
using Elastos::Droid::SystemUI::Recent::IRecentsCallback;
using Elastos::Droid::SystemUI::Recent::IRecentsPanelViewRecentsScrollView;
using Elastos::Droid::SystemUI::Recent::ITaskDescriptionAdapter;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IOnGlobalLayoutListener;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::View::IViewOnLongClickListener;
using Elastos::Droid::View::IViewOnTouchListener;
using Elastos::Droid::Widget::FrameLayout;
// using Elastos::Droid::Widget::HorizontalScrollView;
using Elastos::Droid::Widget::IHorizontalScrollView;
using Elastos::Droid::Widget::ILinearLayout;
using Elastos::Core::IRunnable;
using Elastos::Utility::IHashSet;

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace Recent {

class RecentsHorizontalScrollView
    // : public HorizontalScrollView
    : public FrameLayout
    , public IHorizontalScrollView
    , public ISwipeHelperCallback
    , public IRecentsPanelViewRecentsScrollView
{
private:
    class NoOpListener
        : public Object
        , public IViewOnTouchListener
    {
    public:
        NoOpListener(
           /* [in] */  RecentsHorizontalScrollView* host)
            : mHost(host)
        {}

        CAR_INTERFACE_DECL()

        CARAPI OnTouch(
            /* [in] */ IView* v,
            /* [in] */ IMotionEvent* event,
            /* [out] */ Boolean* result);

    private:
        RecentsHorizontalScrollView* mHost;
    };

    class ClickListener
        : public Object
        , public IViewOnClickListener
    {
    public:
        ClickListener(
           /* [in] */  RecentsHorizontalScrollView* host)
            : mHost(host)
        {}

        CAR_INTERFACE_DECL()

        CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        RecentsHorizontalScrollView* mHost;
    };

    class LaunchAppListener
        : public Object
        , public IViewOnClickListener
    {
    public:
        LaunchAppListener(
            /* [in] */  IView* view,
            /* [in] */  RecentsHorizontalScrollView* host)
            : mView(view)
            , mHost(host)
        {}

        CAR_INTERFACE_DECL()

        CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        IView* mView;
        RecentsHorizontalScrollView* mHost;
    };

    class LongClickListener
        : public Object
        , public IViewOnLongClickListener
    {
    public:
        LongClickListener(
            /* [in] */  IView* view,
            /* [in] */  IView* thumbnailView,
            /* [in] */  RecentsHorizontalScrollView* host)
            : mView(view)
            , mThumbnailView(thumbnailView)
            , mHost(host)
        {}

        CAR_INTERFACE_DECL()

        CARAPI OnLongClick(
            /* [in] */ IView* v,
            /* [out] */ Boolean* result);

    private:
        IView* mView;
        IView* mThumbnailView;
        RecentsHorizontalScrollView* mHost;
    };

    class UpdateScroll
        : public Object
        , public IOnGlobalLayoutListener
    {
    public:
        UpdateScroll(
           /* [in] */  RecentsHorizontalScrollView* host) : mHost(host)
        {}

        CAR_INTERFACE_DECL()

        CARAPI OnGlobalLayout();

    private:
        RecentsHorizontalScrollView* mHost;
    };

    class SizeChangedRunnable
        : public Object
        , public IRunnable
    {
    public:
        SizeChangedRunnable(
           /* [in] */  RecentsHorizontalScrollView* host) : mHost(host)
        {}

        CAR_INTERFACE_DECL()

        CARAPI Run();

    private:
        RecentsHorizontalScrollView* mHost;
    };

    class AdapterDataSetObserver
        : public Object
        , public IDataSetObserver
    {
    public:
        AdapterDataSetObserver(
           /* [in] */  RecentsHorizontalScrollView* host) : mHost(host)
        {}

        CAR_INTERFACE_DECL()

        CARAPI OnChanged();

        CARAPI OnInvalidated();

    private:
        RecentsHorizontalScrollView* mHost;
    };

public:
    CAR_INTERFACE_DECL()

    RecentsHorizontalScrollView(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    CARAPI SetMinSwipeAlpha(
        /* [in] */ Float minAlpha);

    CARAPI FindViewForTask(
        /* [in] */ Int32 persistentTaskId,
        /* [out] */ IView** view);

    CARAPI RemoveViewInLayout(
        /* [in] */ IView* view);

    CARAPI_(Boolean) OnInterceptTouchEvent(
        /* [in] */ IMotionEvent* ev);

    CARAPI OnTouchEvent(
        /* [in] */ IMotionEvent* ev,
        /* [out] */ Boolean* result);

    CARAPI_(Boolean) CanChildBeDismissed(
        /* [in] */ IView* v);

    // @Override
    CARAPI IsAntiFalsingNeeded(
        /* [out] */ Boolean* result);

    // @Override
    CARAPI GetFalsingThresholdFactor(
        /* [out] */ Float* result);

    CARAPI DismissChild(
        /* [in] */ IView* v);

    CARAPI OnChildDismissed(
        /* [in] */ IView* v);

    CARAPI OnBeginDrag(
        /* [in] */ IView* v);

    CARAPI OnDragCancelled(
        /* [in] */ IView* v);

    // @Override
    CARAPI OnChildSnappedBack(
        /* [in] */ IView* animView);

    // @Override
    CARAPI UpdateSwipeProgress(
        /* [in] */ IView* animView,
        /* [in] */ Boolean dismissable,
        /* [in] */ Float swipeProgress,
        /* [out] */ Boolean* result);

    CARAPI GetChildAtPosition(
        /* [in] */ IMotionEvent* ev,
        /* [out] */ IView** view);

    CARAPI GetChildContentView(
        /* [in] */ IView* v,
        /* [out] */ IView** view);

    // @Override
    CARAPI DrawFadedEdges(
        /* [in] */ ICanvas* canvas,
        /* [in] */ Int32 left,
        /* [in] */ Int32 right,
        /* [in] */ Int32 top,
        /* [in] */ Int32 bottom);

protected:
    // @Override
    CARAPI_(void) OnScrollChanged(
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 oldl,
        /* [in] */ Int32 oldt);

public:
    CARAPI SetOnScrollListener(
        /* [in] */ IRunnable* listener);

    CARAPI_(Int32) GetVerticalFadingEdgeLength();

    CARAPI_(Int32) GetHorizontalFadingEdgeLength();

    CARAPI OnAttachedToWindow();

    CARAPI SetAdapter(
        /* [in] */ ITaskDescriptionAdapter* adapter);

    CARAPI_(Int32) NumItemsInOneScreenful();

    CARAPI SetLayoutTransition(
        /* [in] */ ILayoutTransition* transition);

    CARAPI SetCallback(
        /* [in] */ IRecentsCallback* callback);

protected:
    CARAPI OnFinishInflate();

    CARAPI_(void) OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 oldw,
        /* [in] */ Int32 oldh);

private:
    CARAPI_(Int32) ScrollPositionOfMostRecent();

    CARAPI_(void) AddToRecycledViews(
        /* [in] */ IView* v);

    CARAPI_(void) Update();

    CARAPI_(void) SetOverScrollEffectPadding(
        /* [in] */ Int32 leftPadding,
        /* [in] */ Int32 i);

protected:
    Int32 mLastScrollPosition;

private:
    static const String TAG;
    static const Boolean DEBUG;
    AutoPtr<ILinearLayout> mLinearLayout;
    AutoPtr<ITaskDescriptionAdapter> mAdapter;
    // IRecentsCallback is it's parent's parent
    IRecentsCallback* mCallback;
    AutoPtr<ISwipeHelper> mSwipeHelper;
    AutoPtr<FadedEdgeDrawHelper> mFadedEdgeDrawHelper;
    AutoPtr<IHashSet> mRecycledViews;
    Int32 mNumItemsInOneScreenful;
    AutoPtr<IRunnable> mOnScrollListener;
};

}// namespace Recent
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_SYSTEMUI_RECENT_RECENTSHORIZONTALSCROLLVIEW_H__
