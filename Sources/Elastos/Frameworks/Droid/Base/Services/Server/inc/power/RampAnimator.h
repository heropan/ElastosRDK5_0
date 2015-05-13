
#ifndef __RAMPANIMATOR_H__
#define __RAMPANIMATOR_H__

#include "Elastos.Droid.Server_server.h"
#include "ext/frameworkdef.h"
#include "R.h"
#include <elastos/Math.h>

using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Core::IRunnable;
using Elastos::Core::EIID_IRunnable;
using Elastos::Droid::Animation::IValueAnimatorHelper;
using Elastos::Droid::Animation::CValueAnimatorHelper;
using Elastos::Droid::View::IChoreographer;
using Elastos::Droid::View::IChoreographerHelper;
using Elastos::Droid::View::CChoreographerHelper;
using Elastos::Droid::Utility::IInt32Property;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Power {

/**
 * A custom animator that progressively updates a property value at
 * a given variable rate until it reaches a particular target value.
 */
template <typename T>
class RampAnimator : public ElRefBase
{
private:
    class CallbackRunnable
        : public ElRefBase
        , public IRunnable
    {
    public:
        CallbackRunnable(
            /* [in] */ RampAnimator<T>* host);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* pObject,
            /* [in] */ InterfaceID* pIID);

        CARAPI Run();

    private:
        RampAnimator<T>* mHost;
    };

public:
    RampAnimator(
        /* [in] */ T& object,
        /* [in] */ IInt32Property* property);

    /**
     * Starts animating towards the specified value.
     *
     * If this is the first time the property is being set, the value jumps
     * directly to the target.
     *
     * @param target The target value.
     * @param rate The convergence rate, in units per second.
     * @return True if the target differs from the previous target.
     */
    CARAPI_(Boolean) AnimateTo(
        /* [in] */ Int32 target,
        /* [in] */ Int32 rate);

private:
    CARAPI_(void) PostCallback();

private:
    T& mObject;
    AutoPtr<IInt32Property> mProperty;
    AutoPtr<IChoreographer> mChoreographer;

    Int32 mCurrentValue;
    Int32 mTargetValue;
    Int32 mRate;

    Boolean mAnimating;
    Float mAnimatedValue; // higher precision copy of mCurrentValue
    Int64 mLastFrameTimeNanos;

    Boolean mFirstTime;
    AutoPtr<IRunnable> mCallback;

    friend class CallbackRunnable;
};

template <typename T>
RampAnimator<T>::CallbackRunnable::CallbackRunnable(
    /* [in] */ RampAnimator<T>* host)
    : mHost(host)
{}

template <typename T>
PInterface RampAnimator<T>::CallbackRunnable::Probe(
    /* [in] */ REIID riid)
{
    if ( riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if ( riid == EIID_IRunnable) {
        return (IInterface*)(IRunnable*)this;
    }
    return NULL;
}

template <typename T>
UInt32 RampAnimator<T>::CallbackRunnable::AddRef()
{
    return ElRefBase::AddRef();
}

template <typename T>
UInt32 RampAnimator<T>::CallbackRunnable::Release()
{
    return ElRefBase::Release();
}

template <typename T>
ECode RampAnimator<T>::CallbackRunnable::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [in] */ InterfaceID* pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface*)this) {
        *pIID = EIID_IInterface;
    }
    else if (pObject == (IInterface *)(IRunnable *)this) {
        *pIID = EIID_IRunnable;
        return NOERROR;
    }
    return E_INVALID_ARGUMENT;
}

template <typename T>
ECode RampAnimator<T>::CallbackRunnable::Run()
{
    Int64 frameTimeNanos;
    mHost->mChoreographer->GetFrameTimeNanos(&frameTimeNanos);
    Float timeDelta = (frameTimeNanos - mHost->mLastFrameTimeNanos)
            * 0.000000001f;
    mHost->mLastFrameTimeNanos = frameTimeNanos;

    // Advance the animated value towards the target at the specified rate
    // and clamp to the target. This gives us the new current value but
    // we keep the animated value around to allow for fractional increments
    // towards the target.
    AutoPtr<IValueAnimatorHelper> helper;
    ASSERT_SUCCEEDED(CValueAnimatorHelper::AcquireSingleton((IValueAnimatorHelper**)&helper));
    Float scale;
    helper->GetDurationScale(&scale);
    if (scale == 0) {
        // Animation off.
        mHost->mAnimatedValue = mHost->mTargetValue;
    }
    else {
        Float amount = timeDelta * mHost->mRate / scale;
        if (mHost->mTargetValue > mHost->mCurrentValue) {
            mHost->mAnimatedValue = Elastos::Core::Math::Min(mHost->mAnimatedValue + amount, (Float)mHost->mTargetValue);
        }
        else {
            mHost->mAnimatedValue = Elastos::Core::Math::Max(mHost->mAnimatedValue - amount, (Float)mHost->mTargetValue);
        }
    }
    Int32 oldCurrentValue = mHost->mCurrentValue;
    mHost->mCurrentValue = Elastos::Core::Math::Round(mHost->mAnimatedValue);

    if (oldCurrentValue != mHost->mCurrentValue) {
        mHost->mProperty->SetValue(mHost->mObject, mHost->mCurrentValue);
    }

    if (mHost->mTargetValue != mHost->mCurrentValue) {
        mHost->PostCallback();
    }
    else {
        mHost->mAnimating = FALSE;
    }

    return NOERROR;
}


template <typename T>
RampAnimator<T>::RampAnimator(
    /* [in] */ T& object,
    /* [in] */ IInt32Property* property)
    : mObject(object)
    , mCurrentValue(0)
    , mTargetValue(0)
    , mRate(0)
    , mAnimating(FALSE)
    , mAnimatedValue(0)
    , mLastFrameTimeNanos(0)
    , mFirstTime(TRUE)
    , mProperty(property)
{
    AutoPtr<IChoreographerHelper> helper;
    ASSERT_SUCCEEDED(CChoreographerHelper::AcquireSingleton((IChoreographerHelper**)&helper));
    helper->GetInstance((IChoreographer**)&mChoreographer);
    mCallback = (IRunnable*)new CallbackRunnable(this);
}

template <typename T>
Boolean RampAnimator<T>::AnimateTo(
    /* [in] */Int32 target,
    /* [in] */Int32 rate)
{
    // Immediately jump to the target the first time.
    if (mFirstTime) {
        mFirstTime = FALSE;
        mProperty->SetValue(mObject, target);
        mCurrentValue = target;
        return TRUE;
    }

    // Adjust the rate based on the closest target.
    // If a faster rate is specified, then use the new rate so that we converge
    // more rapidly based on the new request.
    // If a slower rate is specified, then use the new rate only if the current
    // value is somewhere in between the new and the old target meaning that
    // we will be ramping in a different direction to get there.
    // Otherwise, continue at the previous rate.
    if (!mAnimating
            || rate > mRate
            || (target <= mCurrentValue && mCurrentValue <= mTargetValue)
            || (mTargetValue <= mCurrentValue && mCurrentValue <= target)) {
        mRate = rate;
    }

    Boolean changed = (mTargetValue != target);
    mTargetValue = target;

    // Start animating.
    if (!mAnimating && target != mCurrentValue) {
        mAnimating = TRUE;
        mAnimatedValue = mCurrentValue;
        AutoPtr<ISystem> system;
        Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
        system->GetNanoTime(&mLastFrameTimeNanos);
        PostCallback();
    }

    return changed;
}

template <typename T>
void RampAnimator<T>::PostCallback()
{
    mChoreographer->PostCallback(IChoreographer::CALLBACK_ANIMATION, mCallback, NULL);
}

} // namespace Power
} // namespace Server
} // namepsace Droid
} // namespace Elastos

#endif //__RAMPANIMATOR_H__
