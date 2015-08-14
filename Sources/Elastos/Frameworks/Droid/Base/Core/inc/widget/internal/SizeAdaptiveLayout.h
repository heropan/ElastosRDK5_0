
#ifndef __ELASTOS_DROID_WIDGET_INTERNAL_SIZEADAPTIVELAYOUT_H__
#define __ELASTOS_DROID_WIDGET_INTERNAL_SIZEADAPTIVELAYOUT_H__

#include "view/ViewGroup.h"
#include "ext/frameworkext.h"

using Elastos::Droid::View::ViewGroup;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::Animation::IAnimatorListener;
using Elastos::Droid::Animation::IAnimatorSet;
using Elastos::Droid::Animation::IObjectAnimator;
using Elastos::Droid::Animation::IAnimator;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

/**
 * A layout that switches between its children based on the requested layout height.
 * Each child specifies its minimum and maximum valid height.  Results are undefined
 * if children specify overlapping ranges.  A child may specify the maximum height
 * as 'unbounded' to indicate that it is willing to be displayed arbitrarily tall.
 *
 * <p>
 * See {@link SizeAdaptiveLayout.LayoutParams} for a full description of the
 * layout parameters used by SizeAdaptiveLayout.
 */
class SizeAdaptiveLayout : public ViewGroup
{
public:
    class BringToFrontOnEnd
        : public ElRefBase
        , public IAnimatorListener
    {
    public:
        CAR_INTERFACE_DECL()

        BringToFrontOnEnd(
            /* [in] */ SizeAdaptiveLayout* host);

        virtual CARAPI OnAnimationEnd(
            /* [in] */ IAnimator* animation);

        virtual CARAPI OnAnimationCancel(
            /* [in] */ IAnimator* animation);

        virtual CARAPI OnAnimationRepeat(
            /* [in] */ IAnimator* animation);

        virtual CARAPI OnAnimationStart(
            /* [in] */ IAnimator* animation);

    private:
        SizeAdaptiveLayout* mHost;
    };

public:
    SizeAdaptiveLayout();

    SizeAdaptiveLayout(
        /* [in] */ IContext* context);

    SizeAdaptiveLayout(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    SizeAdaptiveLayout(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    /**
     * Visible for testing
     * @hide
     */
    virtual CARAPI_(AutoPtr<IAnimator>) GetTransitionAnimation();

    /**
     * Visible for testing
     * @hide
     */
    virtual CARAPI_(AutoPtr<IView>) GetModestyPanel();

    virtual CARAPI OnAttachedToWindow();

protected:
    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    virtual CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    virtual CARAPI_(void) OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    virtual CARAPI GenerateLayoutParams(
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ IViewGroupLayoutParams** params);

    virtual CARAPI_(AutoPtr<IViewGroupLayoutParams>) GenerateLayoutParams(
        /* [in] */ IViewGroupLayoutParams* p);

    virtual CARAPI_(AutoPtr<IViewGroupLayoutParams>) GenerateDefaultLayoutParams();

    virtual CARAPI_(Boolean) CheckLayoutParams(
        /* [in] */ IViewGroupLayoutParams* p);

private:
    CARAPI Initialize();

    CARAPI_(Int32) ClampSizeToBounds(
        /* [in] */ Int32 measuredHeight,
        /* [in] */ IView* child);

    //TODO extend to width and height
    CARAPI_(AutoPtr<IView>) SelectActiveChild(
        /* [in] */ Int32 heightMeasureSpec);

public:
        // TypedArray indices
    static const Int32 MIN_VALID_HEIGHT;// = R::styleable::SizeAdaptiveLayout_Layout_layout_minHeight;
    static const Int32 MAX_VALID_HEIGHT;// = R::styleable::SizeAdaptiveLayout_Layout_layout_maxHeight;

private:
    static const String TAG;// = "SizeAdaptiveLayout";
    static const Boolean DEBUG;// = FALSE;
    static const Boolean REPORT_BAD_BOUNDS;// = TRUE;
    static const Int64 CROSSFADE_TIME;// = 250;

    // view state
    AutoPtr<IView> mActiveChild;
    AutoPtr<IView> mLastActive;

    // animation state
    AutoPtr<IAnimatorSet> mTransitionAnimation;
    AutoPtr<IAnimatorListener> mAnimatorListener;
    AutoPtr<IObjectAnimator> mFadePanel;
    AutoPtr<IObjectAnimator> mFadeView;
    Int32 mCanceledAnimationCount;
    AutoPtr<IView> mEnteringView;
    AutoPtr<IView> mLeavingView;

    // View used to hide larger views under smaller ones to create a uniform crossfade
    AutoPtr<IView> mModestyPanel;
    Int32 mModestyPanelTop;
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif

