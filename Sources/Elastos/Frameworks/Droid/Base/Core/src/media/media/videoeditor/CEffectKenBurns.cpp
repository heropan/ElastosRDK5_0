
#include "ext/frameworkext.h"
#include "CEffectKenBurns.h"
#include "graphics/CRect.h"

using Elastos::Droid::Graphics::CRect;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

IEFFECT_METHODS_IMPL(CEffectKenBurns, Effect)

PInterface CEffectKenBurns::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Effect) {
        return reinterpret_cast<PInterface>((Effect*)this);
    }
    return _CEffectKenBurns::Probe(riid);
}

ECode CEffectKenBurns::constructor(
    /* [in] */ IMediaItem* mediaItem,
    /* [in] */ const String& effectId,
    /* [in] */ IRect* startRect,
    /* [in] */ IRect* endRect,
    /* [in] */ Int64 startTimeMs,
    /* [in] */ Int64 durationMs)
{
    Init(mediaItem, effectId, startTimeMs, durationMs);

    Int32 sw, sh, ew, eh;
    startRect->GetWidth(&sw);
    startRect->GetHeight(&sh);
    endRect->GetWidth(&ew);
    endRect->GetHeight(&eh);
    if ( (sw <= 0) || (sh <= 0) ) {
    //    throw new IllegalArgumentException("Invalid Start rectangle");
    }
    if ( (ew <= 0) || (eh <= 0) ) {
    //    throw new IllegalArgumentException("Invalid End rectangle");
    }

    mStartRect = startRect;
    mEndRect = endRect;
}

ECode CEffectKenBurns::GetStartRect(
    /* [out] */ IRect** result)
{
    VALIDATE_NOT_NULL(result);

    *result = mStartRect;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CEffectKenBurns::GetEndRect(
    /* [out] */ IRect** result)
{
    VALIDATE_NOT_NULL(result);

    *result = mEndRect;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CEffectKenBurns::GetKenBurnsSettings(
    /* [in] */ IRect* start,
    /* [in] */ IRect* end)
{
    AutoPtr<IRect> startRect;
    AutoPtr<IRect> endRect;
    GetStartRect((IRect**)&startRect);
    GetEndRect((IRect**)&endRect);

    ((CRect*)start)->mLeft = ((CRect*)startRect.Get())->mLeft;
    ((CRect*)start)->mTop = ((CRect*)startRect.Get())->mTop;
    ((CRect*)start)->mRight = ((CRect*)startRect.Get())->mRight;
    ((CRect*)start)->mBottom = ((CRect*)startRect.Get())->mBottom;
    ((CRect*)end)->mLeft = ((CRect*)endRect.Get())->mLeft;
    ((CRect*)end)->mTop = ((CRect*)endRect.Get())->mTop;
    ((CRect*)end)->mRight = ((CRect*)endRect.Get())->mRight;
    ((CRect*)end)->mBottom = ((CRect*)endRect.Get())->mBottom;

    return NOERROR;
}

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
