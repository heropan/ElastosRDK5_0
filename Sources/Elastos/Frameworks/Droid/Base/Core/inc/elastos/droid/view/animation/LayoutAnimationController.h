
#ifndef __ELASTOS_DROID_VIEW_ANIMATION_LAYOUTANIMATIONCONTROLLER_H__
#define __ELASTOS_DROID_VIEW_ANIMATION_LAYOUTANIMATIONCONTROLLER_H__

#include "Elastos.Droid.Core_server.h"
#include "ext/frameworkext.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

class LayoutAnimationController
    : public Object
    , public ILayoutAnimationController
{
public:
    class AnimationParameters
        : public Object
        , public IAnimationParameters
    {
    public:
        CAR_INTERFACE_DECL();

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
    CAR_INTERFACE_DECL();

    LayoutAnimationController(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    LayoutAnimationController(
        /* [in] */ IAnimation* animation,
        /* [in] */ Float delay = 0.5f);

    virtual CARAPI GetOrder(
        /* [out] */ Int32* order);

    virtual CARAPI SetOrder(
        /* [in] */ Int32 order);

    virtual CARAPI SetAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resourceID);

    virtual CARAPI SetAnimation(
        /* [in] */ IAnimation* animation);

    virtual CARAPI GetAnimation(
        /* [out] */ IAnimation** animation);

    virtual CARAPI SetInterpolator(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resourceID);

    virtual CARAPI SetInterpolator(
        /* [in] */ IInterpolator* interpolator);

    virtual CARAPI GetInterpolator(
        /* [out] */ IInterpolator** interpolator);

    virtual CARAPI GetDelay(
        /* [out] */ Float* delay);

    virtual CARAPI SetDelay(
        /* [in] */ Float delay);

    virtual CARAPI WillOverlap(
        /* [out] */ Boolean* willOverlap);

    virtual CARAPI Start();

    CARAPI GetAnimationForView(
        /* [in] */ IView* view,
        /* [out] */ IAnimation** animation);

    virtual CARAPI IsDone(
        /* [out] */ Boolean* done);

protected:
    LayoutAnimationController();

    virtual CARAPI_(Int64) GetDelayForView(
        /* [in] */ IView* view);

    virtual CARAPI_(Int32) GetTransformedIndex(
        /* [in] */ AnimationParameters* params);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
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

#endif //__ELASTOS_DROID_VIEW_ANIMATION_LAYOUTANIMATIONCONTROLLER_H__
