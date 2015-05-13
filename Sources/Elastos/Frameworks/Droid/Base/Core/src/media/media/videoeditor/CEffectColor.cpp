
#include "CEffectColor.h"

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

IEFFECT_METHODS_IMPL(CEffectColor, Effect)

CEffectColor::CEffectColor()
    : mType(0)
    , mColor(0)
{
}

PInterface CEffectColor::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Effect) {
        return reinterpret_cast<PInterface>((Effect*)this);
    }
    return _CEffectColor::Probe(riid);
}

ECode CEffectColor::constructor(
    /* [in] */ IMediaItem* mediaItem,
    /* [in] */ const String& effectId,
    /* [in] */ Int64 startTimeMs,
    /* [in] */ Int64 durationMs,
    /* [in] */ Int32 type,
    /* [in] */ Int32 color)
{
    Init(mediaItem, effectId, startTimeMs, durationMs);
    switch (type) {
        case TYPE_COLOR:
        case TYPE_GRADIENT: {
            switch (color) {
                case GREEN:
                case PINK:
                case GRAY:
                    mColor = color;
                    break;

                default: {
                //    throw new IllegalArgumentException("Invalid Color: " + color);
                }
            }
            break;
        }
        case TYPE_SEPIA:
        case TYPE_NEGATIVE:
        case TYPE_FIFTIES: {
            mColor = -1;
            break;
        }

        default: {
        //    throw new IllegalArgumentException("Invalid type: " + type);
        }
    }
    mType = type;

    return NOERROR;
}

ECode CEffectColor::GetType(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mType;
    return NOERROR;
}

ECode CEffectColor::GetColor(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mColor;
    return NOERROR;
}

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
