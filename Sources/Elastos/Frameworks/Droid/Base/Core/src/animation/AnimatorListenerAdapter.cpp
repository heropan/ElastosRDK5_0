
#include "animation/AnimatorListenerAdapter.h"

namespace Elastos {
namespace Droid {
namespace Animation {


UInt32 AnimatorListenerAdapter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AnimatorListenerAdapter::Release()
{
    return ElRefBase::Release();
}

PInterface AnimatorListenerAdapter::Probe(
    /* [in] */ REIID riid)
{
    if(riid == EIID_IInterface) {
        return (IInterface*)(IAnimatorListener*)this;
    } else if (riid == EIID_IAnimatorListener) {
        return (IAnimatorListener*)this;
    }
    return NULL;
}

ECode AnimatorListenerAdapter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    assert(pIID != NULL);
    if (pObject == (IInterface*)(IAnimatorListener*)this) {
        *pIID = EIID_IAnimatorListener;
    } else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode AnimatorListenerAdapter::OnAnimationStart(
    /* [in] */ IAnimator* animation)
{
    return NOERROR;
}

ECode AnimatorListenerAdapter::OnAnimationEnd(
    /* [in] */ IAnimator* animation)
{
    return NOERROR;
}

ECode AnimatorListenerAdapter::OnAnimationCancel(
    /* [in] */ IAnimator* animation)
{
    return NOERROR;
}

ECode AnimatorListenerAdapter::OnAnimationRepeat(
    /* [in] */ IAnimator* animation)
{
    return NOERROR;
}


}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
