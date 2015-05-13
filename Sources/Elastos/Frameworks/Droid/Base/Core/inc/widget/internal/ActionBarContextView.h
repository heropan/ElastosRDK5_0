
#ifndef __ACTIONBARCONTEXTVIEW_H__
#define __ACTIONBARCONTEXTVIEW_H__

#include "widget/internal/AbsActionBarView.h"

using Elastos::Droid::View::EIID_IViewOnClickListener;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

class ActionBarContextView : public AbsActionBarView
{
private:

    class CloseButtonListener
        : public IViewOnClickListener
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()

        CloseButtonListener(
            /* [in] */ IActionMode* mode);

        CARAPI OnClick(
            /* [in] */ IView* v);
    private:
        AutoPtr<IActionMode> mMode;
    };
public:
    ActionBarContextView();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = R::attr::actionModeStyle);

    virtual CARAPI OnDetachedFromWindow();

    virtual CARAPI SetSplitActionBar(
        /* [in] */ Boolean split);

    virtual CARAPI SetContentHeight(
        /* [in] */ Int32 height);

    virtual CARAPI SetCustomView(
        /* [in] */ IView* view);

    virtual CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    virtual CARAPI SetSubtitle(
        /* [in] */ ICharSequence* subtitle);

    virtual CARAPI_(AutoPtr<ICharSequence>) GetTitle();

    virtual CARAPI_(AutoPtr<ICharSequence>) GetSubtitle();

    virtual CARAPI InitForMode(
        /* [in] */ IActionMode* mode);

    virtual CARAPI CloseMode();

    virtual CARAPI KillMode();

    virtual CARAPI_(Boolean) ShowOverflowMenu();

    virtual CARAPI_(Boolean) HideOverflowMenu();

    virtual CARAPI_(Boolean) IsOverflowMenuShowing();

    virtual CARAPI OnAnimationStart(
        /* [in] */ IAnimator* animation);

    virtual CARAPI OnAnimationEnd(
        /* [in] */ IAnimator* animation);

    virtual CARAPI OnAnimationCancel(
        /* [in] */ IAnimator* animation);

    virtual CARAPI OnAnimationRepeat(
        /* [in] */ IAnimator* animation);

    virtual CARAPI_(Boolean) ShouldDelayChildPressedState();

    virtual CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI SetTitleOptional(
        /* [in] */ Boolean titleOptional);

    virtual CARAPI_(Boolean) IsTitleOptional();

    virtual CARAPI GenerateLayoutParams(
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ IViewGroupLayoutParams** params);

protected:
    CARAPI_(AutoPtr<IViewGroupLayoutParams>) GenerateDefaultLayoutParams();

    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    CARAPI_(void) OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

private:
    CARAPI_(void) InitTitle();

    CARAPI_(void) FinishAnimation();

    CARAPI_(AutoPtr<IAnimator>) MakeInAnimation();

    CARAPI_(AutoPtr<IAnimator>) MakeOutAnimation();

private:
    static String TAG;

    AutoPtr<ICharSequence> mTitle;
    AutoPtr<ICharSequence> mSubtitle;

    AutoPtr<IView> mClose;
    AutoPtr<IView> mCustomView;
    AutoPtr<ILinearLayout> mTitleLayout;
    AutoPtr<ITextView> mTitleView;
    AutoPtr<ITextView> mSubtitleView;
    Int32 mTitleStyleRes;
    Int32 mSubtitleStyleRes;
    AutoPtr<IDrawable> mSplitBackground;
    Boolean mTitleOptional;

    AutoPtr<IAnimator> mCurrentAnimation;
    Boolean mAnimateInOnLayout;
    Int32 mAnimationMode;

    static const Int32 ANIMATE_IDLE = 0;
    static const Int32 ANIMATE_IN = 1;
    static const Int32 ANIMATE_OUT = 2;
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif
