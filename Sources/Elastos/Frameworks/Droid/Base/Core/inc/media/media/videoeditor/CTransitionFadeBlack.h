
#ifndef __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CTRANSITIONFADEBLACK_H__
#define __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CTRANSITIONFADEBLACK_H__

#include "_CTransitionFadeBlack.h"
#include <ext/frameworkext.h>
#include "media/media/videoeditor/Transition.h"


namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

/**
 * This class is used to render a fade to black and fade from black transition
 * between two media items.
 * {@hide}
 */
CarClass(CTransitionFadeBlack), public Transition
{
public:
    ITRANSITION_METHODS_DECL()

    CTransitionFadeBlack();

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
     * @param durationMs duration of the transition
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
#endif // __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CTRANSITIONFADEBLACK_H__

