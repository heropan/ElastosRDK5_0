#include "elastos/droid/view/RenderNodeAnimator.h"
#include "elastos/droid/view/View.h"
#include "elastos/droid/view/ViewPropertyAnimator.h"
#include "elastos/droid/animation/ValueAnimator.h"

#include <elastos/utility/logging/Slogger.h>

#include <hwui/Animator.h>

using Elastos::Droid::Animation::ValueAnimator;
using Elastos::Droid::Animation::EIID_IAnimator;
using Elastos::Core::EIID_IRunnable;
using android::uirenderer::RenderPropertyAnimator;
using android::uirenderer::BaseRenderNodeAnimator;
using android::uirenderer::CanvasPropertyPrimitive;
using android::uirenderer::CanvasPropertyPrimitiveAnimator;
using android::uirenderer::CanvasPropertyPaint;
using android::uirenderer::CanvasPropertyPaintAnimator;
using android::uirenderer::RevealAnimator;
using android::uirenderer::Interpolator;
using android::uirenderer::AnimationListener;

namespace Elastos {
namespace Droid {
namespace View {

HashMap<Int32, Int32> RenderNodeAnimator::sViewPropertyAnimatorMap(15);
pthread_key_t RenderNodeAnimator::sAnimationHelper;

void ThreadDestructor(void* st)
{
    RenderNodeAnimator::DelayedAnimationHelper* handler = static_cast<RenderNodeAnimator::DelayedAnimationHelper*>(st);
    if (handler) {
        handler->Release();
    }
}

Boolean InitTLS()
{
    Int32 result = pthread_key_create(&RenderNodeAnimator::sAnimationHelper, ThreadDestructor);
    assert(result == 0);
    return TRUE;
}

Boolean RenderNodeAnimator::InitHashMap()
{
    // sViewPropertyAnimatorMap[ViewPropertyAnimator::TRANSLATION_X] = IRenderNodeAnimator::TRANSLATION_X;
    // sViewPropertyAnimatorMap[ViewPropertyAnimator::TRANSLATION_Y] = IRenderNodeAnimator::TRANSLATION_Y;
    // sViewPropertyAnimatorMap[ViewPropertyAnimator::TRANSLATION_Z] = IRenderNodeAnimator::TRANSLATION_Z;
    // sViewPropertyAnimatorMap[ViewPropertyAnimator::SCALE_X] = IRenderNodeAnimator::SCALE_X;
    // sViewPropertyAnimatorMap[ViewPropertyAnimator::SCALE_Y] = IRenderNodeAnimator::SCALE_Y;
    // sViewPropertyAnimatorMap[ViewPropertyAnimator::ROTATION] = IRenderNodeAnimator::ROTATION;
    // sViewPropertyAnimatorMap[ViewPropertyAnimator::ROTATION_X] = IRenderNodeAnimator::ROTATION_X;
    // sViewPropertyAnimatorMap[ViewPropertyAnimator::ROTATION_Y] = IRenderNodeAnimator::ROTATION_Y;
    // sViewPropertyAnimatorMap[ViewPropertyAnimator::_X] = IRenderNodeAnimator::X;
    // sViewPropertyAnimatorMap[ViewPropertyAnimator::_Y] = IRenderNodeAnimator::Y;
    // sViewPropertyAnimatorMap[ViewPropertyAnimator::_Z] = IRenderNodeAnimator::Z;
    // sViewPropertyAnimatorMap[ViewPropertyAnimator::ALPHA] = IRenderNodeAnimator::ALPHA;
    return TRUE; // zhangjingcheng
}

Boolean RenderNodeAnimator::sHaveKey = InitTLS();
Boolean RenderNodeAnimator::sHasInitMap = RenderNodeAnimator::InitHashMap();

CAR_INTERFACE_IMPL(RenderNodeAnimator, Animator, IRenderNodeAnimator)
CAR_INTERFACE_IMPL(RenderNodeAnimator::DelayedAnimationHelper, Object, IRunnable)

RenderNodeAnimator::DelayedAnimationHelper::DelayedAnimationHelper()
{
    // mChoreographer = Choreographer::GetInstance();
}

ECode RenderNodeAnimator::DelayedAnimationHelper::AddDelayedAnimation(
    /* [in] */ IRenderNodeAnimator* animator)
{
    mDelayedAnims.PushBack(animator);
    return ScheduleCallback();
}

ECode RenderNodeAnimator::DelayedAnimationHelper::RemoveDelayedAnimation(
    /* [in] */ IRenderNodeAnimator* animator)
{
    mDelayedAnims.Remove(animator);
    return NOERROR;
}

ECode RenderNodeAnimator::DelayedAnimationHelper::Run()
{
    Int64 frameTimeMs; // = mChoreographer.getFrameTime(); zhangjingcheng
    mCallbackScheduled = FALSE;

    List<AutoPtr<IRenderNodeAnimator> >::Iterator end = mDelayedAnims.Begin();
    List<AutoPtr<IRenderNodeAnimator> >::Iterator it = mDelayedAnims.Begin();
    for (; it != mDelayedAnims.End(); it++) {
        AutoPtr<IRenderNodeAnimator> animator = *it;
        if (!((RenderNodeAnimator*)animator.Get())->ProcessDelayed(frameTimeMs)) {
            if (end != it) {
                *end = animator;
            }
            end++;
        }
    }
    while (mDelayedAnims.End() != end) {
        List<AutoPtr<IRenderNodeAnimator> >::Iterator tmp = --mDelayedAnims.End();
        mDelayedAnims.Remove(*tmp);
    }

    if (mDelayedAnims.GetSize() > 0) {
        ScheduleCallback();
    }
    return NOERROR;
}

ECode RenderNodeAnimator::DelayedAnimationHelper::ScheduleCallback()
{
    if (!mCallbackScheduled) {
        mCallbackScheduled = TRUE;
        // mChoreographer.postCallback(Choreographer.CALLBACK_ANIMATION, this, null);
        // zhangjingcheng
    }
    return NOERROR;
}

class AnimationListenerBridge : public AnimationListener {
public:
    // This holds a strong reference to a Java WeakReference<T> object. This avoids
    // cyclic-references-of-doom. If you think "I know, just use NewWeakGlobalRef!"
    // then you end up with basically a PhantomReference, which is totally not
    // what we want.
    AnimationListenerBridge(
        /* [in] */ IRenderNodeAnimator* finishListener)
    {
        mFinishListener = finishListener;
    }

    virtual ~AnimationListenerBridge()
    {
        if (mFinishListener) {
            onAnimationFinished(NULL);
        }
    }

    virtual void onAnimationFinished(
        /* [in] */ BaseRenderNodeAnimator*)
    {
        // LOG_ALWAYS_FATAL_IF(!mFinishListener, "Finished listener twice?");
        if (!mFinishListener) {
            SLOGGERE("RenderNodeAnimator", "Finished listener twice?")
            assert(0);
        }
        RenderNodeAnimator::CallOnFinished(mFinishListener);
        release();
    }

private:
    void release() {
        mFinishListener = NULL;
    }

    AutoPtr<IRenderNodeAnimator> mFinishListener;
};

RenderNodeAnimator::RenderNodeAnimator()
    : mRenderProperty(-1)
    , mFinalValue(0.f)
    , mState(STATE_PREPARE)
    , mUnscaledDuration(300)
    , mUnscaledStartDelay(0)
    , mUiThreadHandlesDelay(FALSE)
    , mStartDelay(0)
    , mStartTime(0)
{}

ECode RenderNodeAnimator::constructor(
    /* [in] */ Int32 property,
    /* [in] */ Float finalValue)
{
    mRenderProperty = property;
    mFinalValue = finalValue;
    mUiThreadHandlesDelay = TRUE;
    Init(nCreateAnimator(property, finalValue));
    return NOERROR;
}

ECode RenderNodeAnimator::constructor(
    /* [in] */ ICanvasProperty* property,
    /* [in] */ Float finalValue)
{
    Int64 ptr;
    property->GetNativeContainer(&ptr);
    Init(nCreateCanvasPropertyFloatAnimator(ptr, finalValue));
    mUiThreadHandlesDelay = FALSE;
    return NOERROR;
}

/**
 * Creates a new render node animator for a field on a Paint property.
 *
 * @param property The paint property to target
 * @param paintField Paint field to animate, one of {@link #PAINT_ALPHA} or
 *            {@link #PAINT_STROKE_WIDTH}
 * @param finalValue The target value for the property
 */
ECode RenderNodeAnimator::constructor(
    /* [in] */ ICanvasProperty* property,
    /* [in] */ Int32 paintField,
    /* [in] */ Float finalValue)
{
    Int64 ptr;
    property->GetNativeContainer(&ptr);
    Init(nCreateCanvasPropertyPaintAnimator(
            ptr, paintField, finalValue));
    mUiThreadHandlesDelay = FALSE;
    return NOERROR;
}

ECode RenderNodeAnimator::constructor(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Float startRadius,
    /* [in] */ Float endRadius)
{
    Init(nCreateRevealAnimator(x, y, startRadius, endRadius));
    mUiThreadHandlesDelay = FALSE;
    return NOERROR;
}

ECode RenderNodeAnimator::Start()
{
    if (mTarget == NULL) {
        SLOGGERE("RenderNodeAnimator", "Missing target!")
        // throw new IllegalStateException("Missing target!");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (mState != STATE_PREPARE) {
        SLOGGERE("RenderNodeAnimator", "Already started!")
        // throw new IllegalStateException("Already started!");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    mState = STATE_DELAYED;
    ApplyInterpolator();

    if (mStartDelay <= 0 || !mUiThreadHandlesDelay) {
        nSetStartDelay(mNativePtr->get(), mStartDelay);
        DoStart();
    } else {
        GetHelper()->AddDelayedAnimation(this);
    }
    return NOERROR;
}

ECode RenderNodeAnimator::Cancel()
{
    if (mState != STATE_FINISHED) {
        if (mState == STATE_DELAYED) {
            GetHelper()->RemoveDelayedAnimation(this);
            NotifyStartListeners();
        }
        nEnd(mNativePtr->get());

        AutoPtr<ArrayOf<IAnimatorListener*> > listeners = CloneListeners();
        Int32 numListeners = listeners == NULL ? 0 : listeners->GetLength();
        for (Int32 i = 0; i < numListeners; i++) {
            (*listeners)[i]->OnAnimationCancel((IAnimator*)this->Probe(EIID_IAnimator));
        }

        if (mViewTarget != NULL) {
            // Kick off a frame to flush the state change
            ((View*)mViewTarget.Get())->InvalidateViewProperty(TRUE, FALSE);
        }
    }
    return NOERROR;
}

ECode RenderNodeAnimator::End()
{
    if (mState != STATE_FINISHED) {
        nEnd(mNativePtr->get());
    }
    return NOERROR;
}

ECode RenderNodeAnimator::Pause()
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode RenderNodeAnimator::Resume()
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode RenderNodeAnimator::SetTarget(
    /* [in] */ IView* view)
{
    mViewTarget = view;
    return SetTarget(((View*)view)->mRenderNode);
}

ECode RenderNodeAnimator::SetTarget(
    /* [in] */ ICanvas* canvas)
{
    // if (!(canvas instanceof GLES20RecordingCanvas)) {
    //     throw new IllegalArgumentException("Not a GLES20RecordingCanvas");
    // }
    // final GLES20RecordingCanvas recordingCanvas = (GLES20RecordingCanvas) canvas;
    // setTarget(recordingCanvas.mNode);
    return NOERROR; //zhangjingcheng
}

ECode RenderNodeAnimator::SetStartValue(
    /* [in] */ Float startValue)
{
    FAIL_RETURN(CheckMutable())
    nSetStartValue(mNativePtr->get(), startValue);
    return NOERROR;
}

ECode RenderNodeAnimator::SetStartDelay(
    /* [in] */ Int64 startDelay)
{
    FAIL_RETURN(CheckMutable())
    if (startDelay < 0) {
        SLOGGERE("RenderNodeAnimator", "startDelay must be positive; %lld", startDelay)
        // throw new IllegalArgumentException("startDelay must be positive; " + startDelay);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mUnscaledStartDelay = startDelay;
    mStartDelay = (Int64) (ValueAnimator::GetDurationScale() * startDelay);
    return NOERROR;
}

ECode RenderNodeAnimator::GetStartDelay(
    /* [out] */ Int64* startDelay)
{
    VALIDATE_NOT_NULL(startDelay)

    *startDelay = mUnscaledStartDelay;
    return NOERROR;
}

ECode RenderNodeAnimator::SetDuration(
    /* [in] */ Int64 duration)
{
    FAIL_RETURN(CheckMutable())
    if (duration < 0) {
        SLOGGERE("RenderNodeAnimator", "duration must be positive; %lld", duration)
        // throw new IllegalArgumentException("duration must be positive; " + duration);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mUnscaledDuration = duration;
    nSetDuration(mNativePtr->get(), (Int64) (duration * ValueAnimator::GetDurationScale()));
    return NOERROR;
}

ECode RenderNodeAnimator::GetDuration(
    /* [out] */ Int64* duration)
{
    VALIDATE_NOT_NULL(duration)

    *duration = mUnscaledDuration;
    return NOERROR;
}

ECode RenderNodeAnimator::IsRunning(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mState == STATE_DELAYED || mState == STATE_RUNNING;
    return NOERROR;
}

ECode RenderNodeAnimator::IsStarted(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mState != STATE_PREPARE;
    return NOERROR;
}

ECode RenderNodeAnimator::SetInterpolator(
    /* [in] */ ITimeInterpolator* interpolator)
{
    FAIL_RETURN(CheckMutable())
    mInterpolator = interpolator;
    return NOERROR;
}

ECode RenderNodeAnimator::GetInterpolator(
    /* [out] */ ITimeInterpolator** interpolator)
{
    VALIDATE_NOT_NULL(interpolator)

    *interpolator = mInterpolator;
    REFCOUNT_ADD(*interpolator)
    return NOERROR;
}

ECode RenderNodeAnimator::Clone(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)

    // throw new IllegalStateException("Cannot clone this animator");
    SLOGGERE("RenderNodeAnimator", "Cannot clone this animator")
    return E_ILLEGAL_STATE_EXCEPTION;
}

ECode RenderNodeAnimator::GetNativeAnimator(
    /* [out] */ Int64* ptr)
{
    *ptr = mNativePtr->get();
    return NOERROR;
}

ECode RenderNodeAnimator::SetAllowRunningAsynchronously(
    /* [in] */ Boolean mayRunAsync)
{
    FAIL_RETURN(CheckMutable())
    nSetAllowRunningAsync(mNativePtr->get(), mayRunAsync);
    return NOERROR;
}

Int32 RenderNodeAnimator::MapViewPropertyToRenderProperty(
    /* [in] */ Int32 viewProperty)
{
    if(sViewPropertyAnimatorMap.Find(viewProperty) != sViewPropertyAnimatorMap.End())
    {
        return sViewPropertyAnimatorMap[viewProperty];
    }
    return -1;
}

Boolean RenderNodeAnimator::IsNativeInterpolator(
    /* [in] */ ITimeInterpolator* interpolator)
{
    return FALSE; //zhangjingcheng
}

void RenderNodeAnimator::OnFinished()
{
    if (mState == STATE_DELAYED) {
        GetHelper()->RemoveDelayedAnimation(this);
        NotifyStartListeners();
    }
    mState = STATE_FINISHED;

    AutoPtr<ArrayOf<IAnimatorListener*> > listeners = CloneListeners();
    Int32 numListeners = listeners == NULL ? 0 : listeners->GetLength();
    for (int i = 0; i < numListeners; i++) {
        // (*listeners)[i]->OnAnimationEnd(this); zhangjingcheng
    }

    // Release the native object, as it has a global reference to us. This
    // breaks the cyclic reference chain, and allows this object to be
    // GC'd
    mNativePtr = NULL;
}

Int64 RenderNodeAnimator::GetNativeAnimator()
{
    return mNativePtr->get();
}

void RenderNodeAnimator::Init(
    /* [in] */ Int64 ptr)
{
    mNativePtr = new VirtualRefBasePtr(ptr);
}

ECode RenderNodeAnimator::CheckMutable()
{
    if (mState != STATE_PREPARE) {
        // throw new IllegalStateException("Animator has already started, cannot change it now!");
        SLOGGERE("RenderNodeAnimator", "Animator has already started, cannot change it now!")
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

void RenderNodeAnimator::ApplyInterpolator()
{
    if (mInterpolator == NULL) return;

    // Int64 ni;
    // if (isNativeInterpolator(mInterpolator)) {
    //     (INativeInterpolatorFactory::Probe(mInterpolator))->CreateNativeInterpolator(&ni);
    // } else {
    //     Int64 duration = nGetDuration(mNativePtr->get());
    //     ni = FallbackLUTInterpolator.createNativeInterpolator(mInterpolator, duration);
    // }
    // nSetInterpolator(mNativePtr->get(), ni); // zhangjingcheng
}

void RenderNodeAnimator::DoStart()
{
    mState = STATE_RUNNING;
    nStart(mNativePtr->get(), this);

    // Alpha is a special snowflake that has the canonical value stored
    // in mTransformationInfo instead of in RenderNode, so we need to update
    // it with the final value here.
    if (mRenderProperty == IRenderNodeAnimator::ALPHA) {
        // Don't need null check because ViewPropertyAnimator's
        // ctor calls ensureTransformationInfo()
        ((View*)mViewTarget.Get())->mTransformationInfo->mAlpha = mFinalValue;
    }

    NotifyStartListeners();

    if (mViewTarget != NULL) {
        // Kick off a frame to start the process
        ((View*)mViewTarget.Get())->InvalidateViewProperty(TRUE, FALSE);
    }
}

void RenderNodeAnimator::NotifyStartListeners()
{
    AutoPtr<ArrayOf<IAnimatorListener*> > listeners = CloneListeners();
    Int32 numListeners = listeners == NULL ? 0 : listeners->GetLength();
    for (int i = 0; i < numListeners; i++) {
        // (*listeners)[i]->OnAnimationStart(this); zhangjingcheng
    }
}

AutoPtr<RenderNodeAnimator::DelayedAnimationHelper> RenderNodeAnimator::GetHelper()
{
    AutoPtr<DelayedAnimationHelper> handler = (DelayedAnimationHelper*)pthread_getspecific(sAnimationHelper);
    if (handler == NULL) {
        handler = new DelayedAnimationHelper();
        pthread_setspecific(sAnimationHelper, handler.Get());
        handler->AddRef();
    }

    return handler;
}

ECode RenderNodeAnimator::SetTarget(
    /* [in] */ IRenderNode* node)
{
    if (mTarget != NULL) {
        // throw new IllegalStateException("Target already set!");
        SLOGGERE("RenderNodeAnimator", "Target already set!")
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mTarget = node;
    mTarget->AddAnimator(this);
    return NOERROR;
}

AutoPtr<ArrayOf<IAnimatorListener*> > RenderNodeAnimator::CloneListeners()
{
    AutoPtr<ArrayOf<IAnimatorListener*> > listeners;
    GetListeners((ArrayOf<IAnimatorListener*>**)&listeners);
    if (listeners != NULL) {
        AutoPtr<ArrayOf<IAnimatorListener*> > res
            = ArrayOf<IAnimatorListener*>::Alloc(listeners->GetLength());
        res->Copy(listeners);
        return res;
    }
    return NULL;
}

Boolean RenderNodeAnimator::ProcessDelayed(
    /* [in] */ Int64 frameTimeMs)
{
    if (mStartTime == 0) {
        mStartTime = frameTimeMs;
    } else if ((frameTimeMs - mStartTime) >= mStartDelay) {
        DoStart();
        return TRUE;
    }
    return FALSE;
}

void RenderNodeAnimator::CallOnFinished(
    /* [in] */ IRenderNodeAnimator* animator)
{
    ((RenderNodeAnimator*)animator)->OnFinished();
}

static inline RenderPropertyAnimator::RenderProperty toRenderProperty(
    /* [in] */ Int32 property)
{
    // LOG_ALWAYS_FATAL_IF(property < 0 || property > RenderPropertyAnimator::ALPHA,
    //         "Invalid property %d", property);
    if (property < 0 || property > RenderPropertyAnimator::ALPHA) {
        SLOGGERE("RenderNodeAnimator", "Invalid property %d", property)
        assert(0);
    }
    return static_cast<RenderPropertyAnimator::RenderProperty>(property);
}

static inline CanvasPropertyPaintAnimator::PaintField toPaintField(
    /* [in] */ Int32 field)
{
    // LOG_ALWAYS_FATAL_IF(field < 0
    //         || field > CanvasPropertyPaintAnimator::ALPHA,
    //         "Invalid paint field %d", field);
    if (field < 0 || field > RenderPropertyAnimator::ALPHA) {
        SLOGGERE("RenderNodeAnimator", "Invalid paint field %d", field)
        assert(0);
    }
    return static_cast<CanvasPropertyPaintAnimator::PaintField>(field);
}

Int64 RenderNodeAnimator::nCreateAnimator(
    /* [in] */ Int32 propertyRaw,
    /* [in] */ Float finalValue)
{
    RenderPropertyAnimator::RenderProperty property = toRenderProperty(propertyRaw);
    BaseRenderNodeAnimator* animator = new RenderPropertyAnimator(property, finalValue);
    return reinterpret_cast<Int64>( animator );
}

Int64 RenderNodeAnimator::nCreateCanvasPropertyFloatAnimator(
    /* [in] */ Int64 canvasPropertyPtr,
    /* [in] */ Float finalValue)
{
    CanvasPropertyPrimitive* canvasProperty = reinterpret_cast<CanvasPropertyPrimitive*>(canvasPropertyPtr);
    BaseRenderNodeAnimator* animator = new CanvasPropertyPrimitiveAnimator(canvasProperty, finalValue);
    return reinterpret_cast<Int64>( animator );
}

Int64 RenderNodeAnimator::nCreateCanvasPropertyPaintAnimator(
    /* [in] */ Int64 canvasPropertyPtr,
    /* [in] */ Int32 paintFieldRaw,
    /* [in] */ Float finalValue)
{
    CanvasPropertyPaint* canvasProperty = reinterpret_cast<CanvasPropertyPaint*>(canvasPropertyPtr);
    CanvasPropertyPaintAnimator::PaintField paintField = toPaintField(paintFieldRaw);
    BaseRenderNodeAnimator* animator = new CanvasPropertyPaintAnimator(
            canvasProperty, paintField, finalValue);
    return reinterpret_cast<Int64>(animator);
}

Int64 RenderNodeAnimator::nCreateRevealAnimator(
    /* [in] */ Int32 centerX,
    /* [in] */ Int32 centerY,
    /* [in] */ Float startRadius,
    /* [in] */ Float endRadius)
{
    BaseRenderNodeAnimator* animator = new RevealAnimator(centerX, centerY, startRadius, endRadius);
    return reinterpret_cast<Int64>( animator );
}

void RenderNodeAnimator::nSetStartValue(
    /* [in] */ Int64 animatorPtr,
    /* [in] */ Float startValue)
{
    BaseRenderNodeAnimator* animator = reinterpret_cast<BaseRenderNodeAnimator*>(animatorPtr);
    animator->setStartValue(startValue);
}

void RenderNodeAnimator::nSetDuration(
    /* [in] */ Int64 animatorPtr,
    /* [in] */ Int64 duration)
{
    // LOG_ALWAYS_FATAL_IF(duration < 0, "Duration cannot be negative");
    if (duration < 0) {
        SLOGGERE("RenderNodeAnimator", "Duration cannot be negative")
        assert(0);
    }
    BaseRenderNodeAnimator* animator = reinterpret_cast<BaseRenderNodeAnimator*>(animatorPtr);
    animator->setDuration(duration);
}

Int64 RenderNodeAnimator::nGetDuration(
    /* [in] */ Int64 animatorPtr)
{
    BaseRenderNodeAnimator* animator = reinterpret_cast<BaseRenderNodeAnimator*>(animatorPtr);
    return static_cast<Int64>(animator->duration());
}

void RenderNodeAnimator::nSetStartDelay(
    /* [in] */ Int64 animatorPtr,
    /* [in] */ Int64 startDelay)
{
    // LOG_ALWAYS_FATAL_IF(startDelay < 0, "Start delay cannot be negative");
    if (startDelay < 0) {
        SLOGGERE("RenderNodeAnimator", "Start delay cannot be negative")
        assert(0);
    }
    BaseRenderNodeAnimator* animator = reinterpret_cast<BaseRenderNodeAnimator*>(animatorPtr);
    animator->setStartDelay(startDelay);
}

void RenderNodeAnimator::nSetInterpolator(
    /* [in] */ Int64 animatorPtr,
    /* [in] */ Int64 interpolatorPtr)
{
    BaseRenderNodeAnimator* animator = reinterpret_cast<BaseRenderNodeAnimator*>(animatorPtr);
    Interpolator* interpolator = reinterpret_cast<Interpolator*>(interpolatorPtr);
    animator->setInterpolator(interpolator);
}

void RenderNodeAnimator::nSetAllowRunningAsync(
    /* [in] */ Int64 animatorPtr,
    /* [in] */ Boolean mayRunAsync)
{
    BaseRenderNodeAnimator* animator = reinterpret_cast<BaseRenderNodeAnimator*>(animatorPtr);
    animator->setAllowRunningAsync(mayRunAsync);
}

void RenderNodeAnimator::nStart(
    /* [in] */ Int64 animatorPtr,
    /* [in] */ IRenderNodeAnimator* finishListener)
{
    BaseRenderNodeAnimator* animator = reinterpret_cast<BaseRenderNodeAnimator*>(animatorPtr);
    if (finishListener) {
        animator->setListener(new AnimationListenerBridge(finishListener));
    }
    animator->start();
}

void RenderNodeAnimator::nEnd(
    /* [in] */ Int64 animatorPtr)
{
    BaseRenderNodeAnimator* animator = reinterpret_cast<BaseRenderNodeAnimator*>(animatorPtr);
    animator->end();
}

} // namespace View
} // namespace Droid
} // namespace Elastos