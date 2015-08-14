
#ifndef __ELASTOS_DROID_WIDGET_INTERNAL_ACTIONBARCONTAINER_H__
#define __ELASTOS_DROID_WIDGET_INTERNAL_ACTIONBARCONTAINER_H__

#include "widget/FrameLayout.h"

using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IActionMode;
using Elastos::Droid::View::IActionModeCallback;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {
/**
 * This class acts as a container for the action bar view and action mode context views.
 * It applies special styles as needed to help handle animated transitions between them.
 * @hide
 */
class ActionBarContainer : public FrameLayout
{

public:
    ActionBarContainer(
        /* [in] */ IContext* context);

    ActionBarContainer(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    virtual CARAPI OnFinishInflate();

    virtual CARAPI SetPrimaryBackground(
        /* [in] */ IDrawable* bg);

    virtual CARAPI SetStackedBackground(
        /* [in] */ IDrawable* bg);

    virtual CARAPI SetSplitBackground(
        /* [in] */ IDrawable* bg);

    virtual CARAPI SetVisibility(
        /* [in] */ Int32 visibility);

    virtual CARAPI JumpDrawablesToCurrentState();

    /**
     * @hide
     */
    virtual CARAPI OnResolveDrawables(
        /* [in] */ Int32 layoutDirection);

    /**
     * Set the action bar into a "transitioning" state. While transitioning
     * the bar will block focus and touch from all of its descendants. This
     * prevents the user from interacting with the bar while it is animating
     * in or out.
     *
     * @param isTransitioning true if the bar is currently transitioning, false otherwise.
     */
    virtual CARAPI SetTransitioning(
        /* [in] */ Boolean isTransitioning);

    virtual CARAPI_(Boolean) OnInterceptTouchEvent(
        /* [in] */ IMotionEvent* ev);

    virtual CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* ev);

    virtual CARAPI_(Boolean) OnHoverEvent(
        /* [in] */ IMotionEvent* ev);

    virtual CARAPI SetTabContainer(
        /* [in] */ IScrollingTabContainerView* tabView);

    virtual CARAPI_(AutoPtr<IView>) GetTabContainer();

    virtual CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

    virtual CARAPI_(AutoPtr<IActionMode>) StartActionModeForChild(
        /* [in] */ IView* child,
        /* [in] */ IActionModeCallback* callback);

    virtual CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    virtual CARAPI_(void) OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

protected:
    ActionBarContainer();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL);

    virtual CARAPI_(Boolean) VerifyDrawable(
        /* [in] */ IDrawable* who);

    virtual CARAPI DrawableStateChanged();

private:
    Boolean mIsTransitioning;
    AutoPtr<IView> mTabContainer;
    AutoPtr<IActionBarView> mActionBarView;

    AutoPtr<IDrawable> mBackground;
    AutoPtr<IDrawable> mStackedBackground;
    AutoPtr<IDrawable> mSplitBackground;
    Boolean mIsSplit;
    Boolean mIsStacked;
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_INTERNAL_ACTIONBARCONTAINER_H__
