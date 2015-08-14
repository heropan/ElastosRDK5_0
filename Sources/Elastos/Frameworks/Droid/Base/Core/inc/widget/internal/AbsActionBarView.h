
#ifndef __ELASTOS_DROID_WIDGET_INTERNAL_ABSACTIONBARVIEW_H__
#define __ELASTOS_DROID_WIDGET_INTERNAL_ABSACTIONBARVIEW_H__

#include "ext/frameworkdef.h"
#include "view/ViewGroup.h"
#include "view/menu/ActionMenuPresenter.h"

using Elastos::Droid::Animation::IAnimator;
using Elastos::Droid::Animation::IAnimatorListener;
using Elastos::Droid::Animation::ITimeInterpolator;
using Elastos::Droid::View::ViewGroup;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::Menu::ActionMenuPresenter;
using Elastos::Droid::View::Menu::IActionMenuView;
using Elastos::Droid::View::Menu::IActionMenuPresenter;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

class AbsActionBarView : public ViewGroup
{
public:
    AbsActionBarView();

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    /**
     * Sets whether the bar should be split right now, no questions asked.
     * @param split true if the bar should split
     */
    virtual CARAPI SetSplitActionBar(
        /* [in] */ Boolean split);

    /**
     * Sets whether the bar should split if we enter a narrow screen configuration.
     * @param splitWhenNarrow true if the bar should check to split after a config change
     */
    virtual CARAPI SetSplitWhenNarrow(
        /* [in] */ Boolean splitWhenNarrow);

    virtual CARAPI SetContentHeight(
        /* [in] */ Int32 height);

    virtual CARAPI_(Int32) GetContentHeight();

    virtual CARAPI SetSplitView(
        /* [in] */ IActionBarContainer* splitView);

    /**
     * @return Current visibility or if animating, the visibility being animated to.
     */
    virtual CARAPI_(Int32) GetAnimatedVisibility();

    virtual CARAPI AnimateToVisibility(
        /* [in] */ Int32 visibility);

    virtual CARAPI SetVisibility(
        /* [in] */ Int32 visibility);

    virtual CARAPI_(Boolean) ShowOverflowMenu();

    virtual CARAPI PostShowOverflowMenu();

    virtual CARAPI_(Boolean) HideOverflowMenu();

    virtual CARAPI_(Boolean) IsOverflowMenuShowing();

    virtual CARAPI_(Boolean) IsOverflowReserved();

    virtual CARAPI DismissPopupMenus();

protected:

    virtual CARAPI_(void) OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    virtual CARAPI_(Int32) MeasureChildView(
        /* [in] */ IView* child,
        /* [in] */ Int32 availableWidth,
        /* [in] */ Int32 childSpecHeight,
        /* [in] */ Int32 spacing);

    static CARAPI_(Int32) Next(
        /* [in] */ Int32 x,
        /* [in] */ Int32 val,
        /* [in] */ Boolean isRtl);

    virtual CARAPI_(Int32) PositionChild(
        /* [in] */ IView* child,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 contentHeight,
        /* [in] */ Boolean reverse);

protected:
    class VisibilityAnimListener
        : public IAnimatorListener
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()

        VisibilityAnimListener(
            /* [in] */ AbsActionBarView* host);

        virtual CARAPI_(AutoPtr<VisibilityAnimListener>) WithFinalVisibility(
            /* [in] */ Int32 visibility);

        virtual CARAPI OnAnimationStart(
            /* [in] */ IAnimator* animation);

        virtual CARAPI OnAnimationEnd(
            /* [in] */ IAnimator* animation);

        virtual CARAPI OnAnimationCancel(
            /* [in] */ IAnimator* animation);

        virtual CARAPI OnAnimationRepeat(
            /* [in] */ IAnimator* animation);

    private:
        AbsActionBarView* mHost;
        Boolean mCanceled;// = FALSE;
        Int32 mFinalVisibility;

        friend class AbsActionBarView;
    };

private:
    class ActionBarRunnable
        : public Runnable
    {
    public:
        ActionBarRunnable(
            /* [in] */ AbsActionBarView* host);

        CARAPI Run();

    private:
        AbsActionBarView* mHost;
    };

protected:
    AutoPtr<IActionMenuView> mMenuView;
    AutoPtr<IActionMenuPresenter> mActionMenuPresenter;
    AutoPtr<IActionBarContainer> mSplitView;
    Boolean mSplitActionBar;
    Boolean mSplitWhenNarrow;
    Int32 mContentHeight;

    AutoPtr<IAnimator> mVisibilityAnim;
    AutoPtr<VisibilityAnimListener> mVisAnimListener;

private:
    static AutoPtr<ITimeInterpolator> sAlphaInterpolator;
    static const Int32 FADE_DURATION = 200;
    friend class VisibilityAnimListener;
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_INTERNAL_ABSACTIONBARVIEW_H__
