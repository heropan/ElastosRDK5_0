
#include "elastos/droid/animation/Animator.h"
#include <elastos/utility/etl/Algorithm.h>

using Elastos::Core::EIID_ICloneable;
using Elastos::Utility::CArrayList;

namespace Elastos {
namespace Droid {
namespace Animation {

// {be5c79d5-127b-4b30-87de-a89ca967d239}
extern "C" const InterfaceID EIID_Animator =
        { 0xbe5c79d5, 0x127b, 0x4b30, { 0x87, 0xde, 0xa8, 0x9c, 0xa9, 0x67, 0xd2, 0x39 } };

UInt32 Animator::AddRef()
{
    return Object::AddRef();
}

UInt32 Animator::Release()
{
    return Object::Release();
}

ECode Animator::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    VALIDATE_NOT_NULL(iid);
    if (object == reinterpret_cast<PInterface>((Animator *)this)) {
        *iid = EIID_Animator;
        return NOERROR;
    }
    else if (object == (IInterface*)(ICloneable *)this) {
        *iid = EIID_ICloneable;
        return NOERROR;
    }

    return Object::GetInterfaceID(object, iid);
}

PInterface Animator::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Animator) {
        return reinterpret_cast<PInterface>(this);
    }
    else if (riid == EIID_ICloneable) {
        return (IInterface*)(ICloneable*)this;
    }

    return Object::Probe(riid);
}

Animator::Animator()
    : mPaused(FALSE)
{
}

Animator::~Animator()
{
    mListeners.Clear();
}

ECode Animator::Start()
{
    return NOERROR;
}

ECode Animator::Cancel()
{
    return NOERROR;
}

ECode Animator::End()
{
    return NOERROR;
}

ECode Animator::Pause()
{
    Boolean started = FALSE;
    IsStarted(&started);
    if (started && !mPaused) {
        mPaused = TRUE;
        if (mPauseListeners != NULL) {
            AutoPtr<IArrayList> tmpListeners;
            ICloneable::Probe(mPauseListeners)->Clone((IInterface**)&tmpListeners);
            Int32 numListeners = 0;
            tmpListeners->GetSize(&numListeners);
            for (Int32 i = 0; i < numListeners; ++i) {
                AutoPtr<IInterface> listener;
                tmpListeners->Get(i, (IInterface**)&listener);
                IAnimatorPauseListener::Probe(listener)->OnAnimationPause(THIS_PROBE(IAnimator));
            }
        }
    }
    return NOERROR;
}

ECode Animator::Resume()
{
    if (mPaused) {
        mPaused = FALSE;
        if (mPauseListeners != NULL) {
            AutoPtr<IArrayList> tmpListeners;
            ICloneable::Probe(mPauseListeners)->Clone((IInterface**)&tmpListeners);
            Int32 numListeners = 0;
            tmpListeners->GetSize(&numListeners);
            for (Int32 i = 0; i < numListeners; ++i) {
                AutoPtr<IInterface> listener;
                tmpListeners->Get(i, (IInterface**)&listener);
                IAnimatorPauseListener::Probe(listener)->OnAnimationResume(THIS_PROBE(IAnimator));
            }
        }
    }
    return NOERROR;
}

ECode Animator::IsPaused(
    /* [out] */ Boolean* paused)
{
    VALIDATE_NOT_NULL(paused);
    *paused = mPaused;
    return NOERROR;
}

ECode Animator::GetInterpolator(
    /* [out] */ ITimeInterpolator** interpolator)
{
    VALIDATE_NOT_NULL(interpolator);
    *interpolator = NULL;
    return NOERROR;
}

ECode Animator::IsStarted(
    /* [out] */ Boolean* started)
{
    VALIDATE_NOT_NULL(started);

    return IsRunning(started);
}

ECode Animator::AddListener(
    /* [in] */ IAnimatorListener* listener)
{
    mListeners.PushBack(listener);
    return NOERROR;
}

ECode Animator::RemoveListener(
    /* [in] */ IAnimatorListener* listener)
{
    if (mListeners.IsEmpty()) {
        return NOERROR;
    }

    mListeners.Remove(listener);
    return NOERROR;
}

ECode Animator::GetListeners(
    /* [out] */ ArrayOf<IAnimatorListener*>** listeners)
{
    VALIDATE_NOT_NULL(listeners);
    *listeners = NULL;

    Int32 size = mListeners.GetSize();
    if (size > 0) {
        AutoPtr<ArrayOf<IAnimatorListener*> > tmp = ArrayOf<IAnimatorListener*>::Alloc(size);
        List<AutoPtr<IAnimatorListener> >::Iterator it = mListeners.Begin();
        for (Int32 i = 0; it != mListeners.End(); it++, i++) {
            tmp->Set(i, *it);
        }
        *listeners = tmp;
        REFCOUNT_ADD(*listeners);
        return NOERROR;
    }

    return NOERROR;
}

ECode Animator::AddPauseListener(
    /* [in] */ IAnimatorPauseListener* listener)
{
    if (mPauseListeners == NULL) {
        CArrayList::New((IArrayList**)&mPauseListeners);
    }
    return mPauseListeners->Add(listener);
}

ECode Animator::RemovePauseListener(
    /* [in] */ IAnimatorPauseListener* listener)
{
    if (mPauseListeners == NULL) {
        return NOERROR;
    }
    mPauseListeners->Remove(listener);
    Int32 size = 0;
    mPauseListeners->GetSize(&size);
    if (size == 0) {
        mPauseListeners = NULL;
    }
    return NOERROR;
}

ECode Animator::RemoveAllListeners()
{
    mListeners.Clear();

    if (mPauseListeners != NULL) {
        mPauseListeners->Clear();
        mPauseListeners = NULL;
    }

    return NOERROR;
}

ECode Animator::CloneSuperData(
    /* [in] */ Animator* anim)
{
    VALIDATE_NOT_NULL(anim);
    if (mListeners.IsEmpty() == FALSE) {
        anim->mListeners.Clear();
        Copy(mListeners.Begin(), mListeners.End(), anim->mListeners.Begin());
    }

    if (mPauseListeners != NULL) {
        AutoPtr<IArrayList> oldListeners = mPauseListeners;
        anim->mPauseListeners = NULL;
        CArrayList::New((IArrayList**)&anim->mPauseListeners);
        Int32 numListeners = 0;
        oldListeners->GetSize(&numListeners);
        for (Int32 i = 0; i < numListeners; ++i) {
            AutoPtr<IInterface> listener;
            oldListeners->Get(i, (IInterface**)&listener);
            anim->mPauseListeners->Add(listener);
        }
    }

    return NOERROR;
}

ECode Animator::SetupStartValues()
{
    return NOERROR;
}

ECode Animator::SetupEndValues()
{
    return NOERROR;
}

ECode Animator::SetTarget(
    /* [in] */ IInterface* target)
{
    return NOERROR;
}

ECode Animator::CanReverse(
    /* [out] */ Boolean* can)
{
    VALIDATE_NOT_NULL(can);
    *can = FALSE;
    return NOERROR;
}

ECode Animator::Reverse()
{
    // throw new IllegalStateException("Reverse is not supported");
    return E_ILLEGAL_STATE_EXCEPTION;
}

ECode Animator::SetAllowRunningAsynchronously(
    /* [in] */ Boolean mayRunAsync)
{
    // It is up to subclasses to support this, if they can.
    return NOERROR;
}

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
