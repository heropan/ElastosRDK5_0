
#ifndef __ELASTOS_DROID_INTERNAL_WIDGET_ACTIONBARCONTEXTVIEW_H__
#define __ELASTOS_DROID_INTERNAL_WIDGET_ACTIONBARCONTEXTVIEW_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/internal/widget/AbsActionBarView.h"
#include "Elastos.Droid.Os.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::IActionMode;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::Animation::IAnimator;
using Elastos::Droid::Animation::IAnimatorListener;
using Elastos::Droid::Widget::ILinearLayout;
using Elastos::Droid::Widget::ITextView;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::Internal::Widget::IActionBarContextView;
using Elastos::Droid::Internal::Widget::AbsActionBarView;
using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Widget {

/**
  * @hide
  */
class ActionBarContextView
    : public AbsActionBarView
    , public IAnimatorListener
    , public IActionBarContextView
{
private:
    class InnerCloseButtonListener
        : public Object
        , public IViewOnClickListener
    {
    public:
        CAR_INTERFACE_DECL()

        InnerCloseButtonListener(
            /* [in] */ IActionMode* mode);

        CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        AutoPtr<IActionMode> mMode;
    };

public:
    CAR_INTERFACE_DECL()

    ActionBarContextView();

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyleAttr);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyleAttr,
        /* [in] */ Int32 defStyleRes);

    // @Override
    CARAPI OnDetachedFromWindow();

    // @Override
    CARAPI SetSplitToolbar(
        /* [in] */ Boolean split);

    virtual CARAPI SetContentHeight(
        /* [in] */ Int32 height);

    virtual CARAPI SetCustomView(
        /* [in] */ IView* view);

    virtual CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    virtual CARAPI SetSubtitle(
        /* [in] */ ICharSequence* subtitle);

    virtual CARAPI GetTitle(
        /* [out] */ ICharSequence** result);

    virtual CARAPI GetSubtitle(
        /* [out] */ ICharSequence** result);

    virtual CARAPI InitForMode(
        /* [in] */ IActionMode* mode);

    virtual CARAPI CloseMode();

    virtual CARAPI KillMode();

    // @Override
    CARAPI ShowOverflowMenu(
        /* [out] */ Boolean* result);

    // @Override
    CARAPI HideOverflowMenu(
        /* [out] */ Boolean* result);

    // @Override
    CARAPI IsOverflowMenuShowing(
        /* [out] */ Boolean* result);

    // @Override
    CARAPI GenerateLayoutParams(
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ IViewGroupLayoutParams** result);

    // @Override
    CARAPI OnAnimationStart(
        /* [in] */ IAnimator* animation);

    // @Override
    CARAPI OnAnimationEnd(
        /* [in] */ IAnimator* animation);

    // @Override
    CARAPI OnAnimationCancel(
        /* [in] */ IAnimator* animation);

    // @Override
    CARAPI OnAnimationRepeat(
        /* [in] */ IAnimator* animation);

    // @Override
    CARAPI ShouldDelayChildPressedState(
        /* [out] */ Boolean* result);

    // @Override
    CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI SetTitleOptional(
        /* [in] */ Boolean titleOptional);

    virtual CARAPI IsTitleOptional(
        /* [out] */ Boolean* result);

protected:
    // @Override
    CARAPI GenerateDefaultLayoutParams(
        /* [out] */ IViewGroupLayoutParams** result);

    // @Override
    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    // @Override
    CARAPI OnLayout(
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
    static const String TAG;
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
    Int32 mCloseItemLayout;
    AutoPtr<IAnimator> mCurrentAnimation;
    Boolean mAnimateInOnLayout;
    Int32 mAnimationMode;
    static const Int32 ANIMATE_IDLE = 0;
    static const Int32 ANIMATE_IN = 1;
    static const Int32 ANIMATE_OUT = 2;
};

} // namespace Widget
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_INTERNAL_WIDGET_ACTIONBARCONTEXTVIEW_H__

