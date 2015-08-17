
#ifndef __ELASTOS_DROID_VIEW_ANIMATION_ANIMATION_H__
#define __ELASTOS_DROID_VIEW_ANIMATION_ANIMATION_H__

#include "ext/frameworkext.h"
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Logger.h>
#include "os/Runnable.h"
#include "R.h"

using Elastos::Core::Math;
using Elastos::Droid::R;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Utility::ITypedValue;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Graphics::IMatrix;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::IRectF;
using Elastos::Droid::Os::IHandler;

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

extern "C" const InterfaceID EIID_Animation;

class Animation
    : public Object
    , public IAnimation
    , public ICloneable
{
    friend class AnimationSet;

public:

    class SetListenerHandlerRunable : public Runnable
    {
    public:
        SetListenerHandlerRunable(
            /* [in] */ Animation* host,
            /* [in] */ Int32 type);
        CARAPI Run();

    private:
        Animation* mHost;
        const Int32 mType;

    public:
        static const Int32 ONSTART_TYPE = 0;
        static const Int32 ONREPEAT_TYPE = 1;
        static const Int32 ONEND_TYPE = 2;
    };

    /**
     * Utility class to parse a string description of a size.
     */
    class Description : public Object
    {
    public:
        Description();

        static CARAPI_(AutoPtr<Description>) ParseValue(
            /* [in] */ ITypedValue* value);

    public:
        /**
        * One of Animation.ABSOLUTE, Animation.RELATIVE_TO_SELF, or
        * Animation.RELATIVE_TO_PARENT.
        */
        Int32 mType;

        /**
        * The absolute or relative dimension for this Description.
        */
        Float mValue;
    };

public:
    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    Animation();

    Animation(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    virtual ~Animation() = 0;

    virtual CARAPI Reset();

    virtual CARAPI Cancel();

    virtual CARAPI Detach();

    virtual CARAPI IsInitialized(
        /* [out] */ Boolean* init);

    virtual CARAPI Initialize(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 parentWidth,
        /* [in] */ Int32 parentHeight);

    /**
     * Sets the handler used to invoke listeners.
     *
     * @hide
     */
    virtual CARAPI SetListenerHandler(
        /* [in] */ IHandler* handler);

    virtual CARAPI SetInterpolator(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resID);

    virtual CARAPI SetInterpolator(
        /* [in] */ IInterpolator* i);

    virtual CARAPI SetStartOffset(
        /* [in] */ Int64 startOffset);

    virtual CARAPI SetDuration(
        /* [in] */ Int64 durationMillis);

    virtual CARAPI RestrictDuration(
        /* [in] */ Int64 durationMillis);

    virtual CARAPI ScaleCurrentDuration(
        /* [in] */ Float scale);

    virtual CARAPI SetStartTime(
        /* [in] */ Int64 startTimeMillis);

    virtual CARAPI Start();

    virtual CARAPI StartNow();

    virtual CARAPI SetRepeatMode(
        /* [in] */ Int32 repeatMode);

    virtual CARAPI SetRepeatCount(
        /* [in] */ Int32 repeatCount);

    virtual CARAPI IsFillEnabled(
        /* [out] */ Boolean* enabled);

    virtual CARAPI SetFillEnabled(
        /* [in] */ Boolean fillEnabled);

    virtual CARAPI SetFillBefore(
        /* [in] */ Boolean fillBefore);

    virtual CARAPI SetFillAfter(
        /* [in] */ Boolean fillAfter);

    virtual CARAPI SetZAdjustment(
        /* [in] */ Int32 zAdjustment);

    /**
     * Set background behind animation.
     *
     * @param bg The background color.  If 0, no background.  Currently must
     * be black, with any desired alpha level.
     */
    virtual CARAPI SetBackgroundColor(
        /* [in] */ Int32 bg);

    virtual CARAPI GetBackgroundColor(
        /* [out] */ Int32* bg);

    virtual CARAPI SetDetachWallpaper(
        /* [in] */ Boolean detachWallpaper);

    virtual CARAPI GetInterpolator(
        /* [out] */ IInterpolator** interpolator);

    virtual CARAPI GetStartTime(
        /* [out] */ Int64* time);

    virtual CARAPI GetDuration(
        /* [out] */ Int64* duration);

    virtual CARAPI GetStartOffset(
        /* [out] */ Int64* startOffset);

    virtual CARAPI GetRepeatMode(
        /* [out] */ Int32* mode);

    virtual CARAPI GetRepeatCount(
        /* [out] */ Int32* count);

    virtual CARAPI GetFillBefore(
        /* [out] */ Boolean* result);

    virtual CARAPI GetFillAfter(
        /* [out] */ Boolean* result);

    virtual CARAPI GetZAdjustment(
        /* [out] */ Int32* zAdjustment);

    virtual CARAPI GetDetachWallpaper(
        /* [out] */ Boolean* result);

    virtual CARAPI WillChangeTransformationMatrix(
        /* [out] */ Boolean* result);

    virtual CARAPI WillChangeBounds(
        /* [out] */ Boolean* result);

    virtual CARAPI SetAnimationListener(
        /* [in] */ IAnimationListener* listener);

    virtual CARAPI ComputeDurationHint(
        /* [out] */ Int64* hint);

    virtual CARAPI GetTransformation(
        /* [in] */ Int64 currentTime,
        /* [in] */ ITransformation* outTransformation,
        /* [out] */ Boolean* result);

    virtual CARAPI GetTransformation(
        /* [in] */ Int64 currentTime,
        /* [in, out] */ ITransformation* outTransformation,
        /* [in] */ Float scale,
        /* [out] */ Boolean* result);

    virtual CARAPI HasStarted(
        /* [out] */ Boolean* has);

    virtual CARAPI HasEnded(
        /* [out] */ Boolean* has);

    virtual CARAPI HasAlpha(
        /* [out] */ Boolean* has);

    virtual CARAPI GetInvalidateRegion(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom,
        /* [in] */ IRectF* invalidate,
        /* [in] */ ITransformation* transformation);

    virtual CARAPI InitializeInvalidateRegion(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

protected:
    virtual CARAPI_(void) EnsureInterpolator();

    virtual CARAPI_(void) ApplyTransformation(
        /* [in] */ Float interpolatedTime,
        /* [in] */ ITransformation* t);

    virtual CARAPI_(Float) ResolveSize(
        /* [in] */ Int32 type,
        /* [in] */ Float value,
        /* [in] */ Int32 size,
        /* [in] */ Int32 parentSize);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    /**
     * The scale factor is set by the call to <code>getTransformation</code>. Overrides of
     * {@link #getTransformation(long, Transformation, float)} will get this value
     * directly. Overrides of {@link #applyTransformation(float, Transformation)} can
     * call this method to get the value.
     *
     * @return float The scale factor that should be applied to pre-scaled values in
     * an Animation such as the pivot points in {@link ScaleAnimation} and {@link RotateAnimation}.
     */
    virtual CARAPI_(Float) GetScaleFactor();

    virtual CARAPI_(void) Finalize();

    virtual CARAPI_(AutoPtr<IAnimation>) GetCloneInstance() = 0;

    virtual CARAPI Clone(
        /* [out] */ IInterface** object);

private:
    CARAPI_(void) FireAnimationStart();

    CARAPI_(void) FireAnimationRepeat();

    CARAPI_(void) FireAnimationEnd();

protected:
    /**
     * Set by {@link #getTransformation(Int64, Transformation)} when the animation ends.
     */
    Boolean mEnded;

    /**
     * Set by {@link #getTransformation(Int64, Transformation)} when the animation starts.
     */
    Boolean mStarted;

    /**
     * Set by {@link #getTransformation(Int64, Transformation)} when the animation repeats
     * in REVERSE mode.
     */
    Boolean mCycleFlip;

    /**
     * This value must be Set to TRUE by {@link #initialize(Int32, Int32, Int32, Int32)}. It
     * indicates the animation was successfully initialized and can be played.
     */
    Boolean mInitialized;

    /**
     * Indicates whether the animation transformation should be applied before the
     * animation starts. The value of this variable is only relevant if mFillEnabled is true;
     * otherwise it is assumed to be true.
     */
    Boolean mFillBefore;

    /**
     * Indicates whether the animation transformation should be applied after the
     * animation ends.
     */
    Boolean mFillAfter;

    /**
     * Indicates whether fillBefore should be taken into account.
     */
    Boolean mFillEnabled;

    /**
     * The time in milliseconds at which the animation must start;
     */
    Int64 mStartTime;

    /**
     * The delay in milliseconds after which the animation must start. When the
     * start offset is > 0, the start time of the animation is startTime + startOffset.
     */
    Int64 mStartOffset;

    /**
     * The duration of one animation cycle in milliseconds.
     */
    Int64 mDuration;

    /**
     * The number of times the animation must repeat. By default, an animation repeats
     * indefinitely.
     */
    Int32 mRepeatCount;

    /**
     * Indicates how many times the animation was repeated.
     */
    Int32 mRepeated;

    /**
     * The behavior of the animation when it repeats. The repeat mode is either
     * {@link #Animation_RESTART} or {@link #REVERSE}.
     *
     */
    Int32 mRepeatMode;

    /**
     * The interpolator used by the animation to smooth the movement.
     */
    AutoPtr<IInterpolator> mInterpolator;

    /**
     * The animation listener to be notified when the animation starts, ends or repeats.
     */
    AutoPtr<IAnimationListener> mListener;

    AutoPtr<IRectF> mPreviousRegion;
    AutoPtr<IRectF> mRegion;
    AutoPtr<ITransformation> mTransformation;
    AutoPtr<ITransformation> mPreviousTransformation;

private:
    /**
     * Desired Z order mode during animation.
     */
    Int32 mZAdjustment;

    /**
     * Desired background color behind animation.
     */
    Int32 mBackgroundColor;

    /**
     * scalefactor to apply to pivot points, etc. during animation. Subclasses retrieve the
     * value via getScaleFactor().
     */
    Float mScaleFactor;

    /**
     * Don't animate the wallpaper.
     */
    Boolean mDetachWallpaper;

    Boolean mMore;
    Boolean mOneMoreTime;
    static Boolean USE_CLOSEGUARD;

    //TODO
    //private final CloseGuard guard = CloseGuard.get();

    AutoPtr<IHandler> mListenerHandler;
    AutoPtr<IRunnable> mOnStart;
    AutoPtr<IRunnable> mOnRepeat;
    AutoPtr<IRunnable> mOnEnd;
};

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_VIEW_ANIMATION_ANIMATION_H__
