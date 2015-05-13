
#include "animation/TimeAnimator.h"
#include "animation/CTimeAnimator.h"

namespace Elastos {
namespace Droid {
namespace Animation {


TimeAnimator::TimeAnimator()
    : mPreviousTime(-1)
{
}

ECode TimeAnimator::Start()
{
    mPreviousTime = -1;
    return ValueAnimator::Start();
}

Boolean TimeAnimator::AnimationFrame(
    /* [in] */ Int64 currentTime)
{
    if (mListener != NULL) {
        Int64 totalTime = currentTime - mStartTime;
        Int64 deltaTime = (mPreviousTime < 0) ? 0 : (currentTime - mPreviousTime);
        mPreviousTime = currentTime;
        mListener->OnTimeUpdate((ITimeAnimator*)(this->Probe(EIID_ITimeAnimator)), totalTime, deltaTime);
    }
    return FALSE;
}

ECode TimeAnimator::SetTimeListener(
    /* [in] */ ITimeListener* listener)
{
    mListener = listener;
    return NOERROR;
}

ECode TimeAnimator::AnimateValue(
    /* [in] */ Float fraction)
{
    // Noop
    return NOERROR;
}

ECode TimeAnimator::InitAnimation()
{
    // noop
    return NOERROR;
}

ECode TimeAnimator::Clone(
    /* [out] */ IAnimator** object)
{
    AutoPtr<CTimeAnimator> newObject;
    CTimeAnimator::NewByFriend((CTimeAnimator**)&newObject);
    CloneSuperData(newObject.Get());
    CloneInternal(newObject);
    TimeAnimator* temp = newObject;
    temp->mListener = mListener;
    temp->mPreviousTime = mPreviousTime;
    *object = (IAnimator*)newObject->Probe(EIID_IAnimator);
    INTERFACE_ADDREF(*object);
    return NOERROR;
}

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
