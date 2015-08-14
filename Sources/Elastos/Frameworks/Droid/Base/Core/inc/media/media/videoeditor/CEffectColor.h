
#ifndef __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CEFFECTCOLOR_H__
#define __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CEFFECTCOLOR_H__

#include "_CEffectColor.h"
#include <ext/frameworkext.h>
#include "media/media/videoeditor/Effect.h"


namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

/**
 * This is the super class for all effects. An effect can only be applied to a
 * single media item.
 * {@hide}
 */
CarClass(CEffectColor), public Effect
{
public:
    IEFFECT_METHODS_DECL()

    CEffectColor();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    /**
     * Constructor
     *
     * @param mediaItem The media item owner
     * @param effectId The effect id
     * @param startTimeMs The start time relative to the media item to which it
     *            is applied
     * @param durationMs The duration of this effect in milliseconds
     * @param type type of the effect. type is one of: TYPE_COLOR,
     *            TYPE_GRADIENT, TYPE_SEPIA, TYPE_NEGATIVE, TYPE_FIFTIES.
     * @param color If type is TYPE_COLOR, color is the RGB color as 888.
     *              If type is TYPE_GRADIENT, color is the RGB color at the
     *              top of the frame. Otherwise, color is ignored
     */
    CARAPI constructor(
        /* [in] */ IMediaItem* mediaItem,
        /* [in] */ const String& effectId,
        /* [in] */ Int64 startTimeMs,
        /* [in] */ Int64 durationMs,
        /* [in] */ Int32 type,
        /* [in] */ Int32 color);

    /**
     * Get the effect type.
     *
     * @return The effect type
     */
    CARAPI GetType(
        /* [out] */ Int32* result);

    /**
     * Get the color if effect type is TYPE_COLOR or TYPE_GRADIENT.
     *
     * @return the color as RGB 888 if type is TYPE_COLOR or TYPE_GRADIENT.
     */
    CARAPI GetColor(
        /* [out] */ Int32* result);

private:
    /**
     *  The effect type
     */
    Int32 mType;

    /**
     *  The effect color
     */
    Int32 mColor;

};

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CEFFECTCOLOR_H__

