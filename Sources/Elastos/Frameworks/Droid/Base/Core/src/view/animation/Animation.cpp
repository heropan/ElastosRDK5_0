
#include "view/animation/Animation.h"
#ifdef DROID_CORE
#include "view/animation/CAnimationUtils.h"
#include "view/animation/CTransformation.h"
#include "view/animation/CAccelerateDecelerateInterpolator.h"
#include "util/CTypedValueHelper.h"
#include "graphics/CRectF.h"
#include "os/CSystemProperties.h"
#endif
#include "os/Handler.h"
#include "R.h"
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Logger.h>

using Elastos::Droid::Graphics::CRectF;
using Elastos::Droid::Utility::ITypedValueHelper;
using Elastos::Droid::Utility::CTypedValueHelper;
using Elastos::Droid::Os::ISystemProperties;
using Elastos::Droid::Os::CSystemProperties;

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

extern "C" const InterfaceID EIID_Animation =
    {0x483e7556, 0x8aff, 0x4b89, {0x8e, 0xf8, 0x3a, 0x57, 0x39, 0x6f, 0x59, 0xe6}};

static Boolean InitUSECLOSEGUARD()
{
    AutoPtr<ISystemProperties> sysProp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
    Boolean value;
    sysProp->GetBoolean(String("ro.monkey"), FALSE, &value);
    return value;
}
Boolean Animation::USE_CLOSEGUARD = InitUSECLOSEGUARD();
const Int32 Animation::SetListenerHandlerRunable::ONSTART_TYPE;
const Int32 Animation::SetListenerHandlerRunable::ONREPEAT_TYPE;
const Int32 Animation::SetListenerHandlerRunable::ONEND_TYPE;

Animation::Description::Description()
    : mType(0)
    , mValue(0.f)
{}
/**
 * Size descriptions can appear inthree forms:
 * <ol>
 * <li>An absolute size. This is represented by a number.</li>
 * <li>A size relative to the size of the object being animated. This
 * is represented by a number followed by "%".</li> *
 * <li>A size relative to the size of the parent of object being
 * animated. This is represented by a number followed by "%p".</li>
 * </ol>
 * @param value The typed value to parse
 * @return The parsed version of the description
 */
AutoPtr<Animation::Description> Animation::Description::ParseValue(
    /* [in] */ ITypedValue* value)
{
    AutoPtr<Description> d = new Description();
    if (value == NULL) {
        d->mType = IAnimation::ABSOLUTE;
        d->mValue = 0;
    }
    else {
        Int32 type;
        value->GetType(&type);
        if (type == ITypedValue::TYPE_FRACTION) {
            Int32 data;
            value->GetData(&data);
            d->mType = ((data & ITypedValue::COMPLEX_UNIT_MASK)
                == ITypedValue::COMPLEX_UNIT_FRACTION_PARENT) ?
                IAnimation::RELATIVE_TO_PARENT : IAnimation::RELATIVE_TO_SELF;
            AutoPtr<ITypedValueHelper> helper;
            CTypedValueHelper::AcquireSingleton((ITypedValueHelper**)&helper);
            helper->ComplexToFloat(data, &d->mValue);
            return d;
        }
        else if (type ==ITypedValue::TYPE_FLOAT) {
            d->mType = IAnimation::ABSOLUTE;
            value->GetFloat(&d->mValue);
            return d;
        }
        else if (type >= ITypedValue::TYPE_FIRST_INT &&
            type <= ITypedValue::TYPE_LAST_INT) {
            d->mType = IAnimation::ABSOLUTE;
            Int32 data;
            value->GetData(&data);
            d->mValue = (Float)data;
            return d;
        }
    }

    d->mType = IAnimation::ABSOLUTE;
    d->mValue = 0.0f;

    return d;
}

Animation::SetListenerHandlerRunable::SetListenerHandlerRunable(
        /* [in] */ Animation* host,
        /* [in] */ Int32 type) : mHost(host), mType(type)
{}

ECode Animation::SetListenerHandlerRunable::Run()
{
    if(mHost->mListener)
    {
        switch(mType)
        {
        case ONSTART_TYPE:
            mHost->mListener->OnAnimationStart((IAnimation*)mHost->Probe(EIID_IAnimation));
            break;
        case ONREPEAT_TYPE:
            mHost->mListener->OnAnimationRepeat((IAnimation*)mHost->Probe(EIID_IAnimation));
            break;
        case ONEND_TYPE:
            mHost->mListener->OnAnimationEnd((IAnimation*)mHost->Probe(EIID_IAnimation));
            break;
        default:
            break;
        }
    }
    return NOERROR;
}

UInt32 Animation::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Animation::Release()
{
    return ElRefBase::Release();
}

PInterface Animation::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IAnimation) {
        return (IAnimation*)this;
    }
    else if (riid == EIID_ICloneable) {
        return (ICloneable*)this;
    }
    else if (riid == EIID_Animation) {
        return reinterpret_cast<PInterface>((Animation*)this);
    }

    return Object::Probe(riid);
}

ECode Animation::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IAnimation*)this) {
        *pIID = EIID_IAnimation;
    }
    if (pObject == (IInterface*)(ICloneable*)this) {
        *pIID = EIID_ICloneable;
    }

    return Object::GetInterfaceID(pObject, pIID);
}

CAR_INTERFACE_IMPL_2(Animation, Object, IAnimation, ICloneable);
Animation::Animation()
    : mEnded(FALSE)
    , mStarted(FALSE)
    , mCycleFlip(FALSE)
    , mInitialized(FALSE)
    , mFillBefore(TRUE)
    , mFillAfter(FALSE)
    , mFillEnabled(FALSE)
    , mStartTime(-1)
    , mStartOffset(0)
    , mDuration(0)
    , mRepeatCount(0)
    , mRepeated(0)
    , mRepeatMode(IAnimation::RESTART)
    , mZAdjustment(0)
    , mBackgroundColor(0)
    , mScaleFactor(1.0f)
    , mDetachWallpaper(FALSE)
    , mMore(TRUE)
    , mOneMoreTime(TRUE)
{
    ASSERT_SUCCEEDED(CRectF::New((IRectF**)&mPreviousRegion));
    ASSERT_SUCCEEDED(CRectF::New((IRectF**)&mRegion));
    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mTransformation));
    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mPreviousTransformation));
    Init();
}

/**
 * Creates a new animation whose parameters come from the specified context and
 * attributes Set.
 *
 * @param context the application environment
 * @param attrs the Set of attributes holding the animation parameters
 */
Animation::Animation(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : mEnded(FALSE)
    , mStarted(FALSE)
    , mCycleFlip(FALSE)
    , mInitialized(FALSE)
    , mFillBefore(TRUE)
    , mFillAfter(FALSE)
    , mFillEnabled(FALSE)
    , mStartTime(-1)
    , mRepeatCount(0)
    , mRepeated(0)
    , mRepeatMode(IAnimation::RESTART)
    , mZAdjustment(0)
    , mBackgroundColor(0)
    , mScaleFactor(1.f)
    , mDetachWallpaper(FALSE)
    , mMore(TRUE)
    , mOneMoreTime(TRUE)
{
    ASSERT_SUCCEEDED(CRectF::New((IRectF**)&mPreviousRegion));
    ASSERT_SUCCEEDED(CRectF::New((IRectF**)&mRegion));
    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mTransformation));
    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mPreviousTransformation));

    Init(context, attrs);
}

ECode Animation::Clone(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    AutoPtr<IAnimation> result = GetCloneInstance();
    if(result == NULL) {
        *object = result;
        REFCOUNT_ADD(*object);
        return NOERROR;
    }

    Animation* animation = (Animation*)result->Probe(EIID_Animation);
    animation->mEnded = mEnded;
    animation->mStarted = mStarted;
    animation->mCycleFlip = mCycleFlip;
    animation->mInitialized = mInitialized;
    animation->mFillBefore = mFillBefore;
    animation->mFillAfter = mFillAfter;
    animation->mFillEnabled = mFillEnabled;
    animation->mStartTime = mStartTime;
    animation->mRepeatCount = mRepeatCount;
    animation->mRepeated = mRepeated;
    animation->mRepeatMode = mRepeatMode;
    animation->mZAdjustment = mZAdjustment;
    animation->mBackgroundColor = mBackgroundColor;
    animation->mScaleFactor = mScaleFactor;
    animation->mDetachWallpaper = mDetachWallpaper;
    animation->mMore = mMore;
    animation->mOneMoreTime = mOneMoreTime;
    animation->mInterpolator = mInterpolator;
    animation->mListener = mListener;
    animation->mListenerHandler = mListenerHandler;
    animation->mOnStart = mOnStart;
    animation->mOnRepeat = mOnRepeat;
    animation->mOnEnd = mOnEnd;

    //Reference object has Init in Constructor
    //So just need to value ordinary object
    *object = result;
    REFCOUNT_ADD(*object);
    return NOERROR;
}

/**
 * Reset the initialization state of this animation.
 *
 * @see #initialize(Int32, Int32, Int32, Int32)
 */
ECode Animation::Reset()
{
    mPreviousRegion->SetEmpty();
    mPreviousTransformation->Clear();
    mInitialized = FALSE;
    mCycleFlip = FALSE;
    mRepeated = 0;
    mMore = TRUE;
    mOneMoreTime = TRUE;
    mListenerHandler = NULL;

    return NOERROR;
}

/**
 * Cancel the animation. Cancelling an animation invokes the animation
 * listener, if Set, to notify the end of the animation.
 *
 * If you cancel an animation manually, you must call {@link #reset()}
 * before starting the animation again.
 *
 * @see #reset()
 * @see #start()
 * @see #startNow()
 */
ECode Animation::Cancel()
{
    if (mStarted && !mEnded) {
        FireAnimationEnd();
        mEnded = TRUE;

        //TODO
        //guard.close();
    }
    // Make sure we move the animation to the end
    mStartTime = Elastos::Core::Math::INT64_MIN_VALUE;
    mMore = mOneMoreTime = FALSE;

    return NOERROR;
}

/**
 * @hide
 */
ECode Animation::Detach()
{
    if (mStarted && !mEnded) {
        mEnded = TRUE;

        //TODO
        //guard.close();

        FireAnimationEnd();
    }

    return NOERROR;
}

/**
 * Whether or not the animation has been initialized.
 *
 * @return Has this animation been initialized.
 * @see #initialize(Int32, Int32, Int32, Int32)
 */
ECode Animation::IsInitialized(
    /* [out] */ Boolean* init)
{
    VALIDATE_NOT_NULL(init);
    *init = mInitialized;
    return NOERROR;
}

/**
 * Initialize this animation with the dimensions of the object being
 * animated as well as the objects parents. (This is to support animation
 * sizes being specified relative to these dimensions.)
 *
 * <p>Objects that interpret Animations should call this method when
 * the sizes of the object being animated and its parent are known, and
 * before calling {@link #getTransformation}.
 *
 *
 * @param width Width of the object being animated
 * @param height Height of the object being animated
 * @param parentWidth Width of the animated object's parent
 * @param parentHeight Height of the animated object's parent
 */
ECode Animation::Initialize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 parentWidth,
    /* [in] */ Int32 parentHeight)
{
    Reset();
    mInitialized = TRUE;

    return NOERROR;
}

ECode Animation::SetListenerHandler(
    /* [in] */ IHandler* handler)
{
    if (mListenerHandler == NULL) {
        mOnStart = new SetListenerHandlerRunable(this, SetListenerHandlerRunable::ONSTART_TYPE);
        mOnRepeat = new SetListenerHandlerRunable(this, SetListenerHandlerRunable::ONREPEAT_TYPE);
        mOnEnd = new SetListenerHandlerRunable(this, SetListenerHandlerRunable::ONEND_TYPE);
    }

    mListenerHandler = handler;
    return NOERROR;
}

/**
 * Sets the acceleration curve for this animation. The interpolator is loaded as
 * a resource from the specified context.
 *
 * @param context The application environment
 * @param resID The resource identifier of the interpolator to load
 * @attr ref android.R.styleable#IAnimation::interpolator
 */
ECode Animation::SetInterpolator(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resID)
{
    AutoPtr<IInterpolator> interpolator;
    AutoPtr<IAnimationUtils> au;
    CAnimationUtils::AcquireSingleton((IAnimationUtils**)&au);
    au->LoadInterpolator(context, resID, (IInterpolator**)&interpolator);
    return SetInterpolator(interpolator);
}

/**
 * Sets the acceleration curve for this animation. Defaults to a linear
 * interpolation.
 *
 * @param i The interpolator which defines the acceleration curve
 * @attr ref android.R.styleable#IAnimation::interpolator
 */
ECode Animation::SetInterpolator(
    /* [in] */ IInterpolator* i)
{
    mInterpolator = i;
    return NOERROR;
}

/**
 * When this animation should start relative to the start time. This is most
 * useful when composing complex animations using an {@link AnimationSet }
 * where some of the animations components start at different times.
 *
 * @param startOffset When this Animation should start, in milliseconds from
 *                    the start time of the root AnimationSet.
 * @attr ref android.R.styleable#IAnimation::startOffset
 */
ECode Animation::SetStartOffset(
    /* [in] */ Int64 startOffset)
{
    mStartOffset = startOffset;

    return NOERROR;
}

/**
 * How Int64 this animation should last. The duration cannot be negative.
 *
 * @param durationMillis Duration in milliseconds
 *
 * @throws java.lang.IllegalArgumentException if the duration is < 0
 *
 * @attr ref android.R.styleable#IAnimation::duration
 */
ECode Animation::SetDuration(
    /* [in] */ Int64 durationMillis)
{
    if (durationMillis < 0) {
        Logger::E(String("Animation"), String("Animation duration cannot be negative"));
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mDuration = durationMillis;

    return NOERROR;
}

/**
 * Ensure that the duration that this animation will run is not longer
 * than <var>durationMillis</var>.  In addition to adjusting the duration
 * itself, this ensures that the repeat count also will not make it run
 * longer than the given time.
 *
 * @param durationMillis The maximum duration the animation is allowed
 * to run.
 */
ECode Animation::RestrictDuration(
    /* [in] */ Int64 durationMillis)
{
    // If we start after the duration, then we just won't run.
    if (mStartOffset > durationMillis) {
        mStartOffset = durationMillis;
        mDuration = 0;
        mRepeatCount = 0;
        return NOERROR;
    }

    Int64 dur = mDuration + mStartOffset;
    if (dur > durationMillis) {
        mDuration = durationMillis - mStartOffset;
        dur = durationMillis;
    }
    // If the duration is 0 or less, then we won't run.
    if (mDuration <= 0) {
        mDuration = 0;
        mRepeatCount = 0;
        return NOERROR;
    }
    // Reduce the number of repeats to keep below the maximum duration.
    // The comparison between mRepeatCount and duration is to catch
    // overflows after multiplying them.
    if (mRepeatCount < 0 || mRepeatCount > durationMillis
            || (dur*mRepeatCount) > durationMillis) {
        // Figure out how many times to do the animation.  Subtract 1 since
        // repeat count is the number of times to repeat so 0 runs once.
        mRepeatCount = (Int32)(durationMillis/dur) - 1;
        if (mRepeatCount < 0) {
            mRepeatCount = 0;
        }
    }

    return NOERROR;
}

/**
 * How much to scale the duration by.
 *
 * @param scale The amount to scale the duration.
 */
ECode Animation::ScaleCurrentDuration(
    /* [in] */ Float scale)
{
    mDuration = (Int64)(mDuration * scale);
    mStartOffset = (Int64) (mStartOffset * scale);

    return NOERROR;
}

/**
 * When this animation should start. When the start time is Set to
 * {@link #START_ON_FIRST_FRAME}, the animation will start the first time
 * {@link #getTransformation(Int64, Transformation)} is invoked. The time passed
 * to this method should be obtained by calling
 * {@link AnimationUtils#currentAnimationTimeMillis()} instead of
 * {@link System#currentTimeMillis()}.
 *
 * @param startTimeMillis the start time in milliseconds
 */
ECode Animation::SetStartTime(
    /* [in] */ Int64 startTimeMillis)
{
    mStartTime = startTimeMillis;
    mStarted = mEnded = FALSE;
    mCycleFlip = FALSE;
    mRepeated = 0;
    mMore = TRUE;

    return NOERROR;
}

/**
 * Convenience method to start the animation the first time
 * {@link #getTransformation(Int64, Transformation)} is invoked.
 */
ECode Animation::Start()
{
    return SetStartTime(-1);
}

/**
 * Convenience method to start the animation at the current time in
 * milliseconds.
 */
ECode Animation::StartNow()
{
    AutoPtr<IAnimationUtils> au;
    CAnimationUtils::AcquireSingleton((IAnimationUtils**)&au);
    Int64 time;
    au->CurrentAnimationTimeMillis(&time);
    return SetStartTime(time);
}

/**
 * Defines what this animation should do when it reaches the end. This
 * Setting is applied only when the repeat count is either greater than
 * 0 or {@link #INFINITE}. Defaults to {@link #IAnimation::RESTART}.
 *
 * @param repeatMode {@link #IAnimation::RESTART} or {@link #REVERSE}
 * @attr ref android.R.styleable#IAnimation::repeatMode
 */
ECode Animation::SetRepeatMode(
    /* [in] */ Int32 repeatMode)
{
    mRepeatMode = repeatMode;
    return NOERROR;
}

/**
 * Sets how many times the animation should be repeated. If the repeat
 * count is 0, the animation is never repeated. If the repeat count is
 * greater than 0 or {@link #INFINITE}, the repeat mode will be taken
 * into account. The repeat count is 0 by default.
 *
 * @param repeatCount the number of times the animation should be repeated
 * @attr ref android.R.styleable#IAnimation::repeatCount
 */
ECode Animation::SetRepeatCount(
    /* [in] */ Int32 repeatCount)
{
    if (repeatCount < 0) {
        repeatCount = IAnimation::ANIMATION_INFINITE;
    }
    mRepeatCount = repeatCount;

    return NOERROR;
}

/**
 * If fillEnabled is true, this animation will apply the value of fillBefore.
 *
 * @return true if the animation will take fillBefore into account
 * @attr ref android.R.styleable#IAnimation::fillEnabled
 */
ECode Animation::IsFillEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = mFillEnabled;
    return NOERROR;
}

/**
 * If fillEnabled is true, the animation will apply the value of fillBefore.
 * Otherwise, fillBefore is ignored and the animation
 * transformation is always applied until the animation ends.
 *
 * @param fillEnabled true if the animation should take the value of fillBefore into account
 * @attr ref android.R.styleable#IAnimation::fillEnabled
 *
 * @see #setFillBefore(boolean)
 * @see #setFillAfter(boolean)
 */
ECode Animation::SetFillEnabled(
    /* [in] */ Boolean fillEnabled)
{
    mFillEnabled = fillEnabled;
    return NOERROR;
}

/**
 * If fillBefore is true, this animation will apply its transformation
 * before the start time of the animation. Defaults to true if
 * {@link #setFillEnabled(boolean)} is not set to true.
 * Note that this applies when using an {@link
 * android.view.animation.AnimationSet AnimationSet} to chain
 * animations. The transformation is not applied before the AnimationSet
 * itself starts.
 *
 * @param fillBefore true if the animation should apply its transformation before it starts
 * @attr ref android.R.styleable#IAnimation::fillBefore
 *
 * @see #setFillEnabled(boolean)
 */
ECode Animation::SetFillBefore(
    /* [in] */ Boolean fillBefore)
{
    mFillBefore = fillBefore;
    return NOERROR;
}

/**
 * If fillAfter is true, the transformation that this animation performed
 * will persist when it is finished. Defaults to false if not set.
 * Note that this applies to individual animations and when using an {@link
 * android.view.animation.AnimationSet AnimationSet} to chain
 * animations.
 *
 * @param fillAfter true if the animation should apply its transformation after it ends
 * @attr ref android.R.styleable#IAnimation::fillAfter
 *
 * @see #setFillEnabled(boolean)
 */
ECode Animation::SetFillAfter(
    /* [in] */ Boolean fillAfter)
{
    mFillAfter = fillAfter;
    return NOERROR;
}

/**
 * Set the Z ordering mode to use while running the animation.
 *
 * @param zAdjustment The desired mode, one of {@link #ZORDER_NORMAL},
 * {@link #ZORDER_TOP}, or {@link #ZORDER_BOTTOM}.
 * @attr ref android.R.styleable#IAnimation::zAdjustment
 */
ECode Animation::SetZAdjustment(
    /* [in] */ Int32 zAdjustment)
{
    mZAdjustment = zAdjustment;
    return NOERROR;
}

ECode Animation::SetBackgroundColor(
    /* [in] */ Int32 bg)
{
    mBackgroundColor = bg;
    return NOERROR;
}

ECode Animation::GetBackgroundColor(
    /* [out] */ Int32* bg)
{
    assert(bg != NULL);
    *bg = mBackgroundColor;
    return NOERROR;
}

/**
 * If detachWallpaper is TRUE, and this is a window animation of a window
 * that has a wallpaper background, then the window will be detached from
 * the wallpaper while it runs.  That is, the animation will only be applied
 * to the window, and the wallpaper behind it will remain static.
 *
 * @param detachWallpaper TRUE if the wallpaper should be detached from the animation
 * @attr ref android.R.styleable#IAnimation::detachWallpaper
 */
ECode Animation::SetDetachWallpaper(
    /* [in] */ Boolean detachWallpaper)
{
    mDetachWallpaper = detachWallpaper;
    return NOERROR;
}

/**
 * Gets the acceleration curve type for this animation.
 *
 * @return the {@link Interpolator} associated to this animation
 * @attr ref android.R.styleable#IAnimation::interpolator
 */
ECode Animation::GetInterpolator(
    /* [out] */ IInterpolator** interpolator)
{
    VALIDATE_NOT_NULL(interpolator);
    *interpolator = mInterpolator;
    REFCOUNT_ADD(*interpolator);
    return NOERROR;
}

/**
 * When this animation should start. If the animation has not startet yet,
 * this method might return {@link #START_ON_FIRST_FRAME}.
 *
 * @return the time in milliseconds when the animation should start or
 *         {@link #START_ON_FIRST_FRAME}
 */
ECode Animation::GetStartTime(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);
    *time = mStartTime;
    return NOERROR;
}

/**
 * How Int64 this animation should last
 *
 * @return the duration in milliseconds of the animation
 * @attr ref android.R.styleable#IAnimation::duration
 */
ECode Animation::GetDuration(
    /* [out] */ Int64* duration)
{
    VALIDATE_NOT_NULL(duration);
    *duration = mDuration;
    return NOERROR;
}

/**
 * When this animation should start, relative to StartTime
 *
 * @return the start offset in milliseconds
 * @attr ref android.R.styleable#IAnimation::startOffset
 */
ECode Animation::GetStartOffset(
    /* [out] */ Int64* startOffset)
{
    VALIDATE_NOT_NULL(startOffset);
    *startOffset = mStartOffset;
    return NOERROR;
}

/**
 * Defines what this animation should do when it reaches the end.
 *
 * @return either one of {@link #REVERSE} or {@link #IAnimation::RESTART}
 * @attr ref android.R.styleable#IAnimation::repeatMode
 */
ECode Animation::GetRepeatMode(
    /* [out] */ Int32* mode)
{
    VALIDATE_NOT_NULL(mode);
    *mode = mRepeatMode;
    return NOERROR;
}

/**
 * Defines how many times the animation should repeat. The default value
 * is 0.
 *
 * @return the number of times the animation should repeat, or {@link #INFINITE}
 * @attr ref android.R.styleable#IAnimation::repeatCount
 */
ECode Animation::GetRepeatCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = mRepeatCount;
    return NOERROR;
}

/**
 * If fillBefore is true, this animation will apply its transformation
 * before the start time of the animation. If fillBefore is false and
 * {@link #isFillEnabled() fillEnabled} is true, the transformation will not be applied until
 * the start time of the animation.
 *
 * @return true if the animation applies its transformation before it starts
 * @attr ref android.R.styleable#IAnimation::fillBefore
 */
ECode Animation::GetFillBefore(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mFillBefore;
    return NOERROR;
}

/**
 * If fillAfter is TRUE, this animation will apply its transformation
 * after the end time of the animation.
 *
 * @return TRUE if the animation applies its transformation after it ends
 * @attr ref android.R.styleable#IAnimation::fillAfter
 */
ECode Animation::GetFillAfter(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mFillAfter;
    return NOERROR;
}

/**
 * Returns the Z ordering mode to use while running the animation as
 * previously Set by {@link #setZAdjustment}.
 *
 * @return Returns one of {@link #ZORDER_NORMAL},
 * {@link #ZORDER_TOP}, or {@link #ZORDER_BOTTOM}.
 * @attr ref android.R.styleable#IAnimation::zAdjustment
 */
ECode Animation::GetZAdjustment(
    /* [out] */ Int32* zAdjustment)
{
    VALIDATE_NOT_NULL(zAdjustment);
    *zAdjustment = mZAdjustment;
    return NOERROR;
}

/**
 * Return value of {@link #setDetachWallpaper(Boolean)}.
 * @attr ref android.R.styleable#IAnimation::detachWallpaper
 */
ECode Animation::GetDetachWallpaper(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mDetachWallpaper;
    return NOERROR;
}

/**
 * <p>Indicates whether or not this animation will affect the transformation
 * matrix. For instance, a fade animation will not affect the matrix whereas
 * a scale animation will.</p>
 *
 * @return TRUE if this animation will change the transformation matrix
 */
ECode Animation::WillChangeTransformationMatrix(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // assume we will change the matrix
    *result = TRUE;
    return NOERROR;
}

/**
 * <p>Indicates whether or not this animation will affect the bounds of the
 * animated view. For instance, a fade animation will not affect the bounds
 * whereas a 200% scale animation will.</p>
 *
 * @return TRUE if this animation will change the view's bounds
 */
ECode Animation::WillChangeBounds(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // assume we will change the bounds
    *result = TRUE;
    return NOERROR;
}

/**
 * <p>Binds an animation listener to this animation. The animation listener
 * is notified of animation events such as the end of the animation or the
 * repetition of the animation.</p>
 *
 * @param listener the animation listener to be notified
 */
ECode Animation::SetAnimationListener(
    /* [in] */ IAnimationListener* listener)
{
    mListener = listener;

    return NOERROR;
}

/**
 * Gurantees that this animation has an interpolator. Will use
 * a AccelerateDecelerateInterpolator is nothing else was specified.
 */
void Animation::EnsureInterpolator()
{
    if (mInterpolator == NULL) {
        CAccelerateDecelerateInterpolator::New(
            (IAccelerateDecelerateInterpolator**)&mInterpolator);
    }
}

/**
 * Compute a hint at how Int64 the entire animation may last, in milliseconds.
 * Animations can be written to cause themselves to run for a different
 * duration than what is computed here, but generally this should be
 * accurate.
 */
ECode Animation::ComputeDurationHint(
    /* [out] */ Int64* hint)
{
    VALIDATE_NOT_NULL(hint);
    *hint = (GetStartOffset() + GetDuration()) * (GetRepeatCount() + 1);
    return NOERROR;
}

/**
 * Gets the transformation to apply at a specified point in time. Implementations of this
 * method should always replace the specified Transformation or document they are doing
 * otherwise.
 *
 * @param currentTime Where we are in the animation. This is wall clock time.
 * @param outTransformation A tranformation object that is provided by the
 *        caller and will be filled in by the animation.
 * @return True if the animation is still running
 */
ECode Animation::GetTransformation(
    /* [in] */ Int64 currentTime,
    /* [in] */ ITransformation* outTransformation,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (mStartTime == -1) {
        mStartTime = currentTime;
    }

    Int64 startOffset = GetStartOffset();
    Int64 duration = mDuration;
    Float normalizedTime;
    if (duration != 0) {
        normalizedTime = ((Float)(currentTime - (mStartTime + startOffset))) /
                (Float)duration;
    }
    else {
        // time is a step-change with a zero duration
        normalizedTime = currentTime < mStartTime ? 0.0f : 1.0f;
    }

    Boolean expired = normalizedTime >= 1.0f;
    mMore = !expired;

    if (!mFillEnabled) {
        normalizedTime = Elastos::Core::Math::Max(Elastos::Core::Math::Min(normalizedTime, 1.0f), 0.0f);
    }

    if ((normalizedTime >= 0.0f || mFillBefore)
        && (normalizedTime <= 1.0f || mFillAfter)) {
        if (!mStarted) {
            FireAnimationStart();
            mStarted = TRUE;

//            CloseGuard is just used to remind you with Log infomations
//            if (USE_CLOSEGUARD) {
//                assert(0);
//                //TODO
//                //guard.open("cancel or detach or getTransformation");
//            }
        }

        if (mFillEnabled) {
            normalizedTime = Elastos::Core::Math::Max(Elastos::Core::Math::Min(normalizedTime, 1.0f), 0.0f);
        }

        if (mCycleFlip) {
            normalizedTime = 1.0f - normalizedTime;
        }

        Float interpolatedTime;
        mInterpolator->GetInterpolation(normalizedTime, &interpolatedTime);
        ApplyTransformation(interpolatedTime, outTransformation);
    }

    if (expired) {
        if (mRepeatCount == mRepeated) {
            if (!mEnded) {
                mEnded = TRUE;
                FireAnimationEnd();
            }
        }
        else {
            if (mRepeatCount > 0) {
                mRepeated++;
            }

            if (mRepeatMode == IAnimation::REVERSE) {
                mCycleFlip = !mCycleFlip;
            }

            mStartTime = -1;
            mMore = TRUE;

            FireAnimationRepeat();
        }
    }

    if (!mMore && mOneMoreTime) {
        mOneMoreTime = FALSE;
        *result = TRUE;
        return NOERROR;
    }

    *result = mMore;
    return NOERROR;
}

Boolean Animation::GetTransformation(
    /* [in] */ Int64 currentTime,
    /* [in, out] */ ITransformation* outTransformation,
    /* [in] */ Float scale,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    mScaleFactor = scale;
    return GetTransformation(currentTime, outTransformation, result);
}

/**
 * <p>Indicates whether this animation has started or not.</p>
 *
 * @return TRUE if the animation has started, FALSE otherwise
 */
ECode Animation::HasStarted(
    /* [out] */ Boolean* has)
{
    VALIDATE_NOT_NULL(has);
    *has = mStarted;
    return NOERROR;
}

/**
 * <p>Indicates whether this animation has ended or not.</p>
 *
 * @return TRUE if the animation has ended, FALSE otherwise
 */
ECode Animation::HasEnded(
    /* [out] */ Boolean* has)
{
    VALIDATE_NOT_NULL(has);
    *has = mEnded;
    return NOERROR;
}

ECode Animation::HasAlpha(
    /* [out] */ Boolean* has)
{
    VALIDATE_NOT_NULL(has);
    *has = FALSE;
    return NOERROR;
}

/**
 * Helper for getTransformation. Subclasses should implement this to apply
 * their transforms given an interpolation value.  Implementations of this
 * method should always replace the specified Transformation or document
 * they are doing otherwise.
 *
 * @param interpolatedTime The value of the normalized time (0.0 to 1.0)
 *        after it has been run through the interpolation function.
 * @param t The Transofrmation object to fill in with the current
 *        transforms.
 */
void Animation::ApplyTransformation(
    /* [in] */ Float interpolatedTime,
    /* [in] */ ITransformation* t)
{
}

/**
 * Convert the information in the description of a size to an actual
 * dimension
 *
 * @param type One of Animation.ABSOLUTE, Animation.RELATIVE_TO_SELF, or
 *             Animation.RELATIVE_TO_PARENT.
 * @param value The dimension associated with the type parameter
 * @param size The size of the object being animated
 * @param parentSize The size of the parent of the object being animated
 * @return The dimension to use for the animation
 */
Float Animation::ResolveSize(
    /* [in] */ Int32 type,
    /* [in] */ Float value,
    /* [in] */ Int32 size,
    /* [in] */ Int32 parentSize)
{
    switch (type) {
        case IAnimation::ABSOLUTE:
            return value;
        case IAnimation::RELATIVE_TO_SELF:
            return size * value;
        case IAnimation::RELATIVE_TO_PARENT:
            return parentSize * value;
        default:
            return value;
    }
}

/**
 * @param left
 * @param top
 * @param right
 * @param bottom
 * @param invalidate
 * @param transformation
 *
 * @hide
 */
ECode Animation::GetInvalidateRegion(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [in] */ IRectF* invalidate,
    /* [in] */ ITransformation* transformation)
{
    IRectF* tempRegion = mRegion;
    IRectF* previousRegion = mPreviousRegion;

    invalidate->Set(left, top, right, bottom);

    AutoPtr<IMatrix> matrix;
    transformation->GetMatrix((IMatrix**)&matrix);
    Boolean res;
    matrix->MapRect(invalidate, &res);

    // Enlarge the invalidate region to account for rounding errors
    invalidate->Inset(-1.0f, -1.0f);
    tempRegion->Set(invalidate);
    invalidate->Union(previousRegion);

    previousRegion->Set(tempRegion);

    mTransformation->Set(transformation);
    transformation->Set(mPreviousTransformation);
    mPreviousTransformation->Set(mTransformation);

    return NOERROR;
}

/**
 * @param left
 * @param top
 * @param right
 * @param bottom
 *
 * @hide
 */
ECode Animation::InitializeInvalidateRegion(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    mPreviousRegion->Set(left, top, right, bottom);
    // Enlarge the invalidate region to account for rounding errors
    mPreviousRegion->Inset(-1.0f, -1.0f);
    if (mFillBefore) {
        Float time;
        mInterpolator->GetInterpolation(0.0f, &time);
        ApplyTransformation(time, mPreviousTransformation);
    }
    return NOERROR;
}

ECode Animation::constructor()
{
    EnsureInterpolator();
    return NOERROR;
}

ECode Animation::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::Animation),
            ARRAY_SIZE(R::styleable::Animation));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds,
        (ITypedArray**)&a);

    Int32 data;
    a->GetInt32(R::styleable::Animation_duration, 0, &data);
    SetDuration((Int64)data);
    a->GetInt32(R::styleable::Animation_startOffset, 0, &data);
    SetStartOffset((Int64)data);

    Boolean flag;
    a->GetBoolean(R::styleable::Animation_fillEnabled, mFillEnabled, &flag);
    SetFillEnabled(flag);
    a->GetBoolean(R::styleable::Animation_fillBefore, mFillBefore, &flag);
    SetFillBefore(flag);
    a->GetBoolean(R::styleable::Animation_fillAfter, mFillAfter, &flag);
    SetFillAfter(flag);

    a->GetInt32(R::styleable::Animation_repeatCount, mRepeatCount, &data);
    SetRepeatCount(data);
    a->GetInt32(R::styleable::Animation_repeatMode, IAnimation::RESTART, &data);
    SetRepeatMode(data);
    a->GetInt32(R::styleable::Animation_zAdjustment, IAnimation::ZORDER_NORMAL, &data);
    SetZAdjustment(data);

    a->GetInt32(R::styleable::Animation_background, 0, &data);
    SetBackgroundColor(data);

    a->GetBoolean(R::styleable::Animation_detachWallpaper, FALSE, &flag);
    SetDetachWallpaper(flag);

    Int32 resID;
    a->GetResourceId(R::styleable::Animation_interpolator, 0, &resID);

    a->Recycle();

    if (resID > 0) {
        SetInterpolator(context, resID);
    }

    EnsureInterpolator();
    return NOERROR;
}

Float Animation::GetScaleFactor()
{
    return mScaleFactor;
}

void Animation::Finalize()
{
}

void Animation::FireAnimationStart()
{
    if (mListener != NULL) {
        if (mListenerHandler == NULL) {
            mListener->OnAnimationStart(THIS_PROBE(IAnimation));
        }
        else {
            Boolean result;
            mListenerHandler->PostAtFrontOfQueue(mOnStart, &result);
        }
    }
}

void Animation::FireAnimationRepeat()
{
    if (mListener != NULL) {
        if (mListenerHandler == NULL) {
            mListener->OnAnimationRepeat(THIS_PROBE(IAnimation));
        }
        else {
            Boolean result;
            mListenerHandler->PostAtFrontOfQueue(mOnRepeat, &result);
        }
    }
}

void Animation::FireAnimationEnd()
{
    if (mListener != NULL) {
        if (mListenerHandler == NULL) {
            mListener->OnAnimationEnd(THIS_PROBE(IAnimation));
        }
        else {
            Boolean result;
            mListenerHandler->PostAtFrontOfQueue(mOnEnd, &result);
        }
    }
}

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos
