
#include "view/VelocityTracker.h"
#include "view/VelocityTrackerState.h"
#include <elastos/Math.h>
#include "util/Pools.h"
#include <elastos/StringBuffer.h>
#include <stdio.h>

using namespace Elastos::Core;
using Elastos::Droid::Utility::Pools;

namespace Elastos {
namespace Droid {
namespace View {

// {b03b50d8-cfc9-4181-8089-7c308b4adb38}
extern "C" const InterfaceID EIID_VelocityTracker =
        { 0xb03b50d8, 0xcfc9, 0x4181, { 0x80, 0x89, 0x7c, 0x30, 0x8b, 0x4a, 0xdb, 0x38 } };

const Int32 VelocityTracker::ACTIVE_POINTER_ID;
const Int32 VelocityTracker::Estimator::MAX_DEGREE;
AutoPtr<IPool> VelocityTracker::sPool = Pools::AcquireSynchronizedPool(
                Pools::AcquireFinitePool(new VelocityTrackerPoolableManager(), 2));

//========================================================================================
//              VelocityTracker::Estimator
//========================================================================================
VelocityTracker::Estimator::Estimator()
{
    mXCoeff = ArrayOf<Float>::Alloc(MAX_DEGREE + 1);
    mYCoeff = ArrayOf<Float>::Alloc(MAX_DEGREE + 1);
}

Float VelocityTracker::Estimator::EstimateX(Float time)
{
    return Estimate(time, mXCoeff);
}

Float VelocityTracker::Estimator::EstimateY(Float time)
{
    return Estimate(time, mYCoeff);
}

Float VelocityTracker::Estimator::GetXCoeff(Int32 index)
{
    return index <= mDegree ? (*mXCoeff)[index] : 0.0f;
}

Float VelocityTracker::Estimator::GetYCoeff(Int32 index)
{
    return index <= mDegree ? (*mYCoeff)[index] : 0.0f;
}

Float VelocityTracker::Estimator::Estimate(Float time, ArrayOf<Float>* c)
{
    Float a = 0;
    Float scale = 1;
    for (Int32 i = 0; i <= mDegree; i++) {
        a += (*c)[i] * scale;
        scale *= time;
    }
    return a;
}

//========================================================================================
//              VelocityTracker::VelocityTrackerPoolableManager
//========================================================================================
CAR_INTERFACE_IMPL(VelocityTracker::VelocityTrackerPoolableManager, IPoolableManager)

ECode VelocityTracker::VelocityTrackerPoolableManager::NewInstance(
    /* [out] */ IPoolable** element)
{
    VALIDATE_NOT_NULL(element);
    AutoPtr<VelocityTracker> vt = new VelocityTracker(String(NULL));
    *element = (IPoolable*)(vt->Probe(EIID_IPoolable));
    if (*element == NULL) {
        return E_OUT_OF_MEMORY_ERROR;
    }
    INTERFACE_ADDREF(*element);
    return NOERROR;
}

ECode VelocityTracker::VelocityTrackerPoolableManager::OnAcquired(
    /* [in] */ IPoolable* element)
{
    return NOERROR;
}

ECode VelocityTracker::VelocityTrackerPoolableManager::OnReleased(
    /* [in] */ IPoolable* element)
{
    if (element) {
        AutoPtr<VelocityTracker> vt = reinterpret_cast<VelocityTracker*>(element->Probe(EIID_VelocityTracker));
        if (vt) {
            vt->Clear();
        }
    }

    return NOERROR;
}

AutoPtr<VelocityTrackerState> VelocityTracker::NativeInitialize(
    /* [in] */ const String& strategy)
{
    AutoPtr<VelocityTrackerState> temp = new VelocityTrackerState(strategy.string());
    return temp;
}

void VelocityTracker::NativeClear(
    /* [in] */ VelocityTrackerState* state)
{
    if (state)
        state->Clear();
}

void VelocityTracker::NativeAddMovement(
    /* [in] */ VelocityTrackerState* state,
    /* [in] */ IMotionEvent* event)
{
    state->AddMovement(event);
}

void VelocityTracker::NativeComputeCurrentVelocity(
    /* [in] */ VelocityTrackerState* state,
    /* [in] */ Int32 units,
    /* [in] */ Float maxVelocity)
{
    state->ComputeCurrentVelocity(units, maxVelocity);
}

Float VelocityTracker::NativeGetXVelocity(
    /* [in] */ VelocityTrackerState* state,
    /* [in] */ Int32 id)
{
    Float vx;
    state->GetVelocity(id, &vx, NULL);
    return vx;
}

Float VelocityTracker::NativeGetYVelocity(
    /* [in] */ VelocityTrackerState* state,
    /* [in] */ Int32 id)
{
    Float vy;
    state->GetVelocity(id, NULL, &vy);
    return vy;
}

Boolean VelocityTracker::NativeGetEstimator(
    /* [in] */ VelocityTrackerState* state,
    /* [in] */ Int32 id,
    /* [in] */ Estimator* outEstimator)
{
    android::VelocityTracker::Estimator estimator;
    Boolean result = state->GetEstimator(id, &estimator);

    for(Int32 i=0; i < VelocityTracker::Estimator::MAX_DEGREE + 1; i++) {
        (*(outEstimator->mXCoeff))[i] = estimator.xCoeff[i];
        (*(outEstimator->mYCoeff))[i] = estimator.yCoeff[i];
    }

    outEstimator->mDegree = estimator.degree;
    outEstimator->mConfidence = estimator.confidence;

    return result;
}

PInterface VelocityTracker::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IPoolable) {
        return (IPoolable*)(IVelocityTracker*)this;
    }
    else if (riid == EIID_VelocityTracker) {
        return reinterpret_cast<PInterface>(this);
    }
    else if (riid == EIID_IVelocityTracker) {
        return (IVelocityTracker*)this;
    }

    return NULL;
}

UInt32 VelocityTracker::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 VelocityTracker::Release()
{
    return ElRefBase::Release();
}

ECode VelocityTracker::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IPoolable*)this) {
        *pIID = EIID_IPoolable;
    }
    else if (pObject == (IInterface*)(IVelocityTracker*)this) {
        *pIID = EIID_IVelocityTracker;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

AutoPtr<VelocityTracker> VelocityTracker::Obtain()
{
    AutoPtr<VelocityTracker> element;
    AutoPtr<IPoolable> temp;
    sPool->Acquire((IPoolable**)&temp);
    if(temp != NULL) {
        element = reinterpret_cast<VelocityTracker*>(temp->Probe(EIID_VelocityTracker));
    }

    return element;
}

AutoPtr<VelocityTracker> VelocityTracker::Obtain(
    /* [in] */ const String& strategy)
{
    if (strategy.IsNull()) {
        return Obtain();
    }
    AutoPtr<VelocityTracker> vt = new VelocityTracker(strategy);
    return vt;
}

ECode VelocityTracker::Recycle()
{
    if (mStrategy.IsNull()) {
        AutoPtr<IPoolable> poolable = THIS_PROBE(IPoolable);
        if (poolable) {
            sPool->ReleaseElement(poolable);
        }
    }

    return NOERROR;
}

ECode VelocityTracker::SetNextPoolable(
    /* [in] */ IPoolable* element)
{
    if (element) {
        mNext = reinterpret_cast<VelocityTracker*>(element->Probe(EIID_VelocityTracker));
    }
    else {
        mNext = NULL;
    }

    return NOERROR;
}

ECode VelocityTracker::GetNextPoolable(
    /* [out] */ IPoolable** element)
{
    VALIDATE_NOT_NULL(element);
    *element = THIS_PROBE(IPoolable);
    INTERFACE_ADDREF(*element);
    return NOERROR;
}

ECode VelocityTracker::IsPooled(
    /* [out] */ Boolean* isPooled)
{
    VALIDATE_NOT_NULL(isPooled);
    *isPooled = mIsPooled;
    return NOERROR;
}

ECode VelocityTracker::SetPooled(
    /* [in] */ Boolean isPooled)
{
    mIsPooled = isPooled;
    return NOERROR;
}

VelocityTracker::VelocityTracker(
    /* [in] */ const String& strategy)
   : mPtr(NativeInitialize(strategy))
   , mIsPooled(FALSE)
{
    mStrategy = strategy;
}

VelocityTracker::~VelocityTracker()
{
    if (mPtr != NULL) {
        mPtr = NULL;
    }
}

ECode VelocityTracker::Clear()
{
    NativeClear(mPtr);
    return NOERROR;
}

ECode VelocityTracker::AddMovement(
    /* [in] */ IMotionEvent* ev)
{
    if(ev == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    NativeAddMovement(mPtr, ev);
    return NOERROR;
}

ECode VelocityTracker::ComputeCurrentVelocity(
    /* [in] */ Int32 units)
{
    return ComputeCurrentVelocityEx(units, Elastos::Core::Math::FLOAT_MAX_VALUE);
}

ECode VelocityTracker::ComputeCurrentVelocityEx(
    /* [in] */ Int32 units,
    /* [in] */ Float maxVelocity)
{
    NativeComputeCurrentVelocity(mPtr, units, maxVelocity);
    return NOERROR;
}

ECode VelocityTracker::GetXVelocity(
    /* [out] */ Float* x)
{
    VALIDATE_NOT_NULL(x);

    *x = NativeGetXVelocity(mPtr, ACTIVE_POINTER_ID);
    return NOERROR;
}

ECode VelocityTracker::GetYVelocity(
    /* [out] */ Float* y)
{
    VALIDATE_NOT_NULL(y);

    *y = NativeGetYVelocity(mPtr, ACTIVE_POINTER_ID);
    return NOERROR;
}

ECode VelocityTracker::GetXVelocityEx(
    /* [in] */ Int32 id,
    /* [out] */ Float* x)
{
    VALIDATE_NOT_NULL(x);

    *x = NativeGetXVelocity(mPtr, id);
    return NOERROR;
}

ECode VelocityTracker::GetYVelocityEx(
    /* [in] */ Int32 id,
    /* [out] */ Float* y)
{
    VALIDATE_NOT_NULL(y);

    *y = NativeGetYVelocity(mPtr, id);
    return NOERROR;
}

ECode VelocityTracker::GetEstimator(
    /* [in] */ Int32 id,
    /* [in] */ Estimator* outEstimatorObj,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    if(outEstimatorObj == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    *result = NativeGetEstimator(mPtr, id, outEstimatorObj);
    return NOERROR;
}

} // namespace View
} // namespace Droid
} // namespace Elastos
