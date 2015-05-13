
#include "animation/Animator.h"
#include <elastos/Algorithm.h>

namespace Elastos {
namespace Droid {
namespace Animation {

// {be5c79d5-127b-4b30-87de-a89ca967d239}
extern "C" const InterfaceID EIID_Animator =
        { 0xbe5c79d5, 0x127b, 0x4b30, { 0x87, 0xde, 0xa8, 0x9c, 0xa9, 0x67, 0xd2, 0x39 } };

Animator::Animator()
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

Boolean Animator::IsStarted()
{
    return IsRunning();
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
        INTERFACE_ADDREF(*listeners);
        return NOERROR;
    }

    return NOERROR;
}

ECode Animator::RemoveAllListeners()
{
    mListeners.Clear();
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


}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos


