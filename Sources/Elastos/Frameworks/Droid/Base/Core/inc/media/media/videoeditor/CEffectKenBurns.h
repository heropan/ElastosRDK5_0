
#ifndef __CEFFECTKENBURNS_H__
#define __CEFFECTKENBURNS_H__

#include "_CEffectKenBurns.h"
#include <ext/frameworkext.h>
#include "media/media/videoeditor/Effect.h"
#include "media/media/videoeditor/EffectMacro.h"

using Elastos::Droid::Graphics::IRect;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

/**
 * This class represents a Ken Burns effect.
 * {@hide}
 */
CarClass(CEffectKenBurns), public Effect
{
public:
    IEFFECT_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    /**
     * Constructor
     *
     * @param mediaItem The media item owner
     * @param effectId The effect id
     * @param startRect The start rectangle
     * @param endRect The end rectangle
     * @param startTimeMs The start time
     * @param durationMs The duration of the Ken Burns effect in milliseconds
     */
    constructor(
        /* [in] */ IMediaItem* mediaItem,
        /* [in] */ const String& effectId,
        /* [in] */ IRect* startRect,
        /* [in] */ IRect* endRect,
        /* [in] */ Int64 startTimeMs,
        /* [in] */ Int64 durationMs);

    /**
     * Get the start rectangle.
     *
     * @return The start rectangle
     */
    CARAPI GetStartRect(
        /* [out] */ IRect** result);

    /**
     * Get the end rectangle.
     *
     * @return The end rectangle
     */
    CARAPI GetEndRect(
        /* [out] */ IRect** result);

    /**
     * Get the KenBurn effect start and end rectangle coordinates
     * @param start The rect object to be populated with start
     * rectangle coordinates
     *
     * @param end The rect object to be populated with end
     * rectangle coordinates
     */
    CARAPI GetKenBurnsSettings(
        /* [in] */ IRect* start,
        /* [in] */ IRect* end);

private:
    /**
     *  Instance variables
     */
    AutoPtr<IRect> mStartRect;
    AutoPtr<IRect> mEndRect;

};

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __CEFFECTKENBURNS_H__

