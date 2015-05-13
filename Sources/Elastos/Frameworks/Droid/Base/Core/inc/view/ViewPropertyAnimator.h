#ifndef __VIEWPROPERTYANIMATOR_H__
#define __VIEWPROPERTYANIMATOR_H__

#include "ext/frameworkext.h"
#include "os/Runnable.h"
#include <elastos/List.h>
#include <elastos/HashMap.h>

using Elastos::Utility::List;
using Elastos::Utility::HashMap;
using Elastos::Core::IRunnable;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Animation::IAnimator;
using Elastos::Droid::Animation::IValueAnimator;
using Elastos::Droid::Animation::IAnimatorListener;
using Elastos::Droid::Animation::ITimeInterpolator;
using Elastos::Droid::Animation::IAnimatorUpdateListener;

namespace Elastos {
namespace Droid {
namespace View {

class ViewPropertyAnimator
{
private:
    class AnimatorEventListener
        : public IAnimatorListener
        , public IAnimatorUpdateListener
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()

        AnimatorEventListener(
            /* [in] */ ViewPropertyAnimator* host);

        CARAPI OnAnimationStart(
            /* [in] */ IAnimator* animation);

        CARAPI OnAnimationEnd(
            /* [in] */ IAnimator* animation);

        CARAPI OnAnimationCancel(
            /* [in] */ IAnimator* animation);

        CARAPI OnAnimationRepeat(
            /* [in] */ IAnimator* animation);

        CARAPI OnAnimationUpdate(
            /* [in] */ IValueAnimator* animation);
    private:
        ViewPropertyAnimator* mHost;
    };
    class NameValuesHolder;
    class PropertyBundle : public ElRefBase
    {
    public:
        PropertyBundle(
            /* [in] */ Int32 propertyMask,
            /* [in] */ List<AutoPtr<NameValuesHolder> >* nameValuesHolder);

        ~PropertyBundle();

        CARAPI_(Boolean) Cancel(
            /* [in] */ Int32 propertyConstant);

        Int32 mPropertyMask;
        const AutoPtr< List<AutoPtr<NameValuesHolder> > > mNameValuesHolder;
    };

    class NameValuesHolder : public ElRefBase
    {
    public:
        NameValuesHolder(
            /* [in] */ Int32 nameConstant,
            /* [in] */ Float fromeValue,
            /* [in] */ Float deltaValue);

        Int32 mNameConstant;
        Float mFromValue;
        Float mDeltaValue;
    };

    class InnerRunnable : public Runnable
    {
    public:
        InnerRunnable(
            /* [in] */ ViewPropertyAnimator* host);

        CARAPI Run();
    private:
        ViewPropertyAnimator* mHost;
    };

    class InnerRunnableEx : public Runnable
    {
    public:
        InnerRunnableEx(
            /* [in] */ ViewPropertyAnimator* host,
            /* [in] */ Int32 type);

        CARAPI Run();
    private:
        ViewPropertyAnimator* mHost;
        Int32 mType;
    };

public:
    ViewPropertyAnimator(
        /* [in] */ IView* view);

    ~ViewPropertyAnimator();

    CARAPI SetDuration(
        /* [in] */ Int64 duration);

    CARAPI_(Int64) GetDuration();

    CARAPI_(Int64) GetStartDelay();

    CARAPI SetStartDelay(
        /* [in] */ Int64 delay);

    CARAPI SetInterpolator(
        /* [in] */ ITimeInterpolator* interpolator);

    CARAPI SetListener(
        /* [in] */ IAnimatorListener* listener);

    CARAPI Start();

    CARAPI Cancel();

    CARAPI X(
        /* [in] */ Float value);

    CARAPI XBy(
        /* [in] */ Float value);

    CARAPI Y(
        /* [in] */ Float value);

    CARAPI YBy(
        /* [in] */ Float value);

    CARAPI Rotation(
        /* [in] */ Float value);

    CARAPI RotationBy(
        /* [in] */ Float value);

    CARAPI RotationX(
        /* [in] */ Float value);

    CARAPI RotationXBy(
        /* [in] */ Float value);

    CARAPI RotationY(
        /* [in] */ Float value);

    CARAPI RotationYBy(
        /* [in] */ Float value);

    CARAPI TranslationX(
        /* [in] */ Float value);

    CARAPI TranslationXBy(
        /* [in] */ Float value);

    CARAPI TranslationY(
        /* [in] */ Float value);

    CARAPI TranslationYBy(
        /* [in] */ Float value);

    CARAPI ScaleX(
        /* [in] */ Float value);

    CARAPI ScaleXBy(
        /* [in] */ Float value);

    CARAPI ScaleY(
        /* [in] */ Float value);

    CARAPI ScaleYBy(
        /* [in] */ Float value);

    CARAPI Alpha(
        /* [in] */ Float value);

    CARAPI AlphaBy(
        /* [in] */ Float value);

    CARAPI WithLayer();

    CARAPI WithStartAction(
        /* [in] */ IRunnable* runnable);

    CARAPI WithEndAction(
        /* [in] */ IRunnable* runnable);

protected:
    ViewPropertyAnimator();

    CARAPI Init(
        /* [in] */ IView* v);

private:
    CARAPI StartAnimation();

    CARAPI AnimateProperty(
        /* [in] */ Int32 constantName,
        /* [in] */ Float toValue);

    CARAPI AnimatePropertyBy(
        /* [in] */ Int32 constantName,
        /* [in] */ Float byValue);

    CARAPI AnimatePropertyBy(
        /* [in] */ Int32 constantName,
        /* [in] */ Float startValue,
        /* [in] */ Float byValue);

    CARAPI SetValue(
        /* [in] */ Int32 propertyConstant,
        /* [in] */ Float value);

    CARAPI_(Float) GetValue(
        /* [in] */ Int32 propertyConstant);

private:
    AutoPtr<IView> mView;
    Int64 mDuration;
    Boolean mDurationSet;
    Int64 mStartDelay;
    Boolean mStartDelaySet;
    AutoPtr<ITimeInterpolator> mInterpolator;
    Boolean mInterpolatorSet;
    AutoPtr<IAnimatorListener> mListener;
    AutoPtr<AnimatorEventListener> mAnimatorEventListener;
    AutoPtr<List<AutoPtr<NameValuesHolder> > > mPendingAnimations;
    AutoPtr<IRunnable> mPendingSetupAction;
    AutoPtr<IRunnable> mPendingCleanupAction;
    AutoPtr<IRunnable> mPendingOnStartAction;
    AutoPtr<IRunnable> mPendingOnEndAction;

    const static Int32 NONE           = 0x0000;
    const static Int32 TRANSLATION_X  = 0x0001;
    const static Int32 TRANSLATION_Y  = 0x0002;
    const static Int32 SCALE_X        = 0x0004;
    const static Int32 SCALE_Y        = 0x0008;
    const static Int32 ROTATION       = 0x0010;
    const static Int32 ROTATION_X     = 0x0020;
    const static Int32 ROTATION_Y     = 0x0040;
    const static Int32 _X              = 0x0080;
    const static Int32 _Y              = 0x0100;
    const static Int32 ALPHA          = 0x0200;

    const static Int32 TRANSFORM_MASK = TRANSLATION_X | TRANSLATION_Y | SCALE_X | SCALE_Y |
            ROTATION | ROTATION_X | ROTATION_Y | _X | _Y;
    AutoPtr<IRunnable> mAnimationStarter;

    typedef HashMap<AutoPtr<IAnimator>, AutoPtr<PropertyBundle> >::Iterator PIterator;
    typedef HashMap<AutoPtr<IAnimator>, AutoPtr<IRunnable> > AnimatorRunnableMap;
    typedef AnimatorRunnableMap::Iterator RNIterator;

    AutoPtr< HashMap<AutoPtr<IAnimator>, AutoPtr<PropertyBundle> > > mAnimatorMap;
    AutoPtr<AnimatorRunnableMap> mAnimatorSetupMap;
    AutoPtr<AnimatorRunnableMap> mAnimatorCleanupMap;
    AutoPtr<AnimatorRunnableMap> mAnimatorOnStartMap;
    AutoPtr<AnimatorRunnableMap> mAnimatorOnEndMap;

};

}// namespace View
}// namespace Droid
}// namespace Elastos
#endif
