#include "CTransitionSliding.h"

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

ITRANSITION_METHODS_IMPL(CTransitionSliding, Transition);

PInterface CTransitionSliding::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Transition) {
        return reinterpret_cast<PInterface>((Transition*)this);
    }
    return _CTransitionSliding::Probe(riid);
}

CTransitionSliding::CTransitionSliding()
    : mSlidingDirection(0)
{
}

ECode CTransitionSliding::constructor(
    /* [in] */ const String& transitionId,
    /* [in] */ IMediaItem* afterMediaItem,
    /* [in] */ IMediaItem* beforeMediaItem,
    /* [in] */ Int64 durationMs,
    /* [in] */ Int32 behavior,
    /* [in] */ Int32 direction)
{
    Init(transitionId, afterMediaItem, beforeMediaItem, durationMs, behavior);

    switch (direction) {
        case ITransitionSliding::DIRECTION_RIGHT_OUT_LEFT_IN:
        case ITransitionSliding::DIRECTION_LEFT_OUT_RIGHT_IN:
        case ITransitionSliding::DIRECTION_TOP_OUT_BOTTOM_IN:
        case ITransitionSliding::DIRECTION_BOTTOM_OUT_TOP_IN:
            break;

        default:
            //throw new IllegalArgumentException("Invalid direction");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mSlidingDirection = direction;
    return NOERROR;
}

ECode CTransitionSliding::GetDirection(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mSlidingDirection;
    return NOERROR;
}

ECode CTransitionSliding::Generate()
{
    return Transition::Generate();
}

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
