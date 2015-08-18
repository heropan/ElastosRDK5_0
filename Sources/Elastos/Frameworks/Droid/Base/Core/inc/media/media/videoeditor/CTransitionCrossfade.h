
#ifndef __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CTRANSITIONCROSSFADE_H__
#define __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CTRANSITIONCROSSFADE_H__

#include "_Elastos_Droid_Media_Videoeditor_CTransitionCrossfade.h"
#include <ext/frameworkext.h>
#include "media/media/videoeditor/Transition.h"


namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

/**
 * This class allows to render a crossfade (dissolve) effect transition between
 * two videos
 * {@hide}
 */
CarClass(CTransitionCrossfade), public Transition
{
public:
    ITRANSITION_METHODS_DECL()

    CTransitionCrossfade();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    /**
     * Constructor
     *
     * @param transitionId The transition id
     * @param afterMediaItem The transition is applied to the end of this
     *      media item
     * @param beforeMediaItem The transition is applied to the beginning of
     *      this media item
     * @param durationMs duration of the transition in milliseconds
     * @param behavior behavior is one of the behavior defined in Transition
     *      class
     *
     * @throws IllegalArgumentException if behavior is not supported.
     */
    CARAPI constructor(
        /* [in] */ const String& transitionId,
        /* [in] */ IMediaItem* afterMediaItem,
        /* [in] */ IMediaItem* beforeMediaItem,
        /* [in] */ Int64 durationMs,
        /* [in] */ Int32 behavior);

    CARAPI Generate();

};

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CTRANSITIONCROSSFADE_H__

