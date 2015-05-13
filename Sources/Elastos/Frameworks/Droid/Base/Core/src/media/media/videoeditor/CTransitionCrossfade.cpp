#include "CTransitionCrossfade.h"

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

ITRANSITION_METHODS_IMPL(CTransitionCrossfade, Transition)

PInterface CTransitionCrossfade::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Transition) {
        return reinterpret_cast<PInterface>((Transition*)this);
    }
    return _CTransitionCrossfade::Probe(riid);
}

CTransitionCrossfade::CTransitionCrossfade()
{
}

ECode CTransitionCrossfade::constructor(
    /* [in] */ const String& transitionId,
    /* [in] */ IMediaItem* afterMediaItem,
    /* [in] */ IMediaItem* beforeMediaItem,
    /* [in] */ Int64 durationMs,
    /* [in] */ Int32 behavior)
{
    return Init(transitionId, afterMediaItem, beforeMediaItem, durationMs, behavior);
}

ECode CTransitionCrossfade::Generate()
{
    return Transition::Generate();
}

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
