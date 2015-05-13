
#ifndef __LAYOUTANIMATIONCONTROLLER_H__
#define __LAYOUTANIMATIONCONTROLLER_H__

#include "Elastos.Droid.Core_server.h"
#include "ext/frameworkext.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

class LayoutAnimationController
{
public:
    class AnimationParameters
    {
    public:

        AnimationParameters();

        CARAPI GetCount(
            /* [out] */ Int32* count);

        CARAPI SetCount(
            /* [in] */ Int32 count);

        CARAPI GetIndex(
            /* [out] */ Int32* index);

        CARAPI SetIndex(
            /* [in] */ Int32 index);

    public:
        /**
         * The number of children in the view group containing the view to which
         * these parameters are attached.
         */
        Int32 mCount;

        /**
         * The index of the view to which these parameters are attached in its
         * containing view group.
         */
        Int32 mIndex;
    };

public:
    /**
     * The set of parameters that has to be attached to each view contained in
     * the view group animated by the layout animation controller. These
     * parameters are used to compute the start time of each individual view's
     * animation.
     */
    class LayoutAnimationParameters
            : public ILayoutAnimationParameters
            , public ElRefBase
            , public AnimationParameters
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI GetCount(
            /* [out] */ Int32* count);

        CARAPI SetCount(
            /* [in] */ Int32 count);

        CARAPI GetIndex(
            /* [out] */ Int32* index);

        CARAPI SetIndex(
            /* [in] */ Int32 index);

        friend class LayoutAnimationController;
    };

public:
    LayoutAnimationController(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    LayoutAnimationController(
        /* [in] */ IAnimation* animation,
        /* [in] */ Float delay = 0.5f);

    virtual CARAPI_(Int32) GetOrder();

    virtual CARAPI SetOrder(
        /* [in] */ Int32 order);

    virtual CARAPI SetAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resourceID);

    virtual CARAPI SetAnimation(
        /* [in] */ IAnimation* animation);

    virtual CARAPI_(AutoPtr<IAnimation>) GetAnimation();

    virtual CARAPI SetInterpolator(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resourceID);

    virtual CARAPI SetInterpolator(
        /* [in] */ IInterpolator* interpolator);

    virtual CARAPI_(AutoPtr<IInterpolator>) GetInterpolator();

    virtual CARAPI_(Float) GetDelay();

    virtual CARAPI SetDelay(
        /* [in] */ Float delay);

    virtual CARAPI_(Boolean) WillOverlap();

    virtual CARAPI Start();

    CARAPI_(AutoPtr<IAnimation>) GetAnimationForView(
        /* [in] */ IView* view);

    virtual CARAPI_(Boolean) IsDone();

protected:
    LayoutAnimationController();

    virtual CARAPI_(Int64) GetDelayForView(
        /* [in] */ IView* view);

    virtual CARAPI_(Int32) GetTransformedIndex(
        /* [in] */ LayoutAnimationParameters* params);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IAnimation* animation,
        /* [in] */ Float delay = 0.5f);

    CARAPI_(Float) GetRandomFloat();
protected:
    /**
     * The animation applied on each child of the view group on which this
     * layout animation controller is set.
     */
    AutoPtr<IAnimation> mAnimation;

    /**
     * The randomizer used when the order is set to random. Subclasses should
     * use this object to avoid creating their own.
     */
//    Random mRandomizer;

    /**
     * The interpolator used to interpolate the delays.
     */
    AutoPtr<IInterpolator> mInterpolator;

private:
    Float mDelay;
    Int32 mOrder;

    Int64 mDuration;
    Int64 mMaxDelay;
};

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos

#endif //__LAYOUTANIMATIONCONTROLLER_H__
