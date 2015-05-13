
#include "animation/CTimeAnimator.h"

namespace Elastos {
namespace Droid {
namespace Animation {

IANIMATOR_METHOD_IMPL(CTimeAnimator, TimeAnimator)
IVALUEANIMATOR_METHOD_IMPL(CTimeAnimator, TimeAnimator)

PInterface CTimeAnimator::Probe(
    /* [in] */ REIID riid)
{
    if(riid == EIID_Animator)
    {
        return reinterpret_cast<PInterface>((Animator*)this);
    }
    if(riid == EIID_ValueAnimator)
    {
        return reinterpret_cast<PInterface>((ValueAnimator*)this);
    }
    return _CTimeAnimator::Probe(riid);
}

ECode CTimeAnimator::SetTimeListener(
    /* [in] */ ITimeListener* listener)
{
    TimeAnimator::SetTimeListener(listener);
    return NOERROR;
}

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
