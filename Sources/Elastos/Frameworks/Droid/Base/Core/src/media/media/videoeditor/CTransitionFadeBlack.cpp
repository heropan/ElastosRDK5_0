#include "CTransitionFadeBlack.h"

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

ITRANSITION_METHODS_IMPL(CTransitionFadeBlack, Transition)

PInterface CTransitionFadeBlack::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Transition) {
        return reinterpret_cast<PInterface>((Transition*)this);
    }
    return _CTransitionFadeBlack::Probe(riid);
}

CTransitionFadeBlack::CTransitionFadeBlack()
{
}

CTransitionFadeBlack::constructor(
    /* [in] */ const String& transitionId,
    /* [in] */ IMediaItem* afterMediaItem,
    /* [in] */ IMediaItem* beforeMediaItem,
    /* [in] */ Int64 durationMs,
    /* [in] */ Int32 behavior)
{
    return Init(transitionId, afterMediaItem, beforeMediaItem, durationMs, behavior);
}

ECode CTransitionFadeBlack::Generate()
{
    return Transition::Generate();
}

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
