
#ifndef __CTRANSITIONSLIDING_H__
#define __CTRANSITIONSLIDING_H__

#include "_CTransitionSliding.h"
#include <ext/frameworkext.h>
#include "media/media/videoeditor/Transition.h"
#include "media/media/videoeditor/TransitionMacro.h"

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

/**
 * This class allows to create sliding transitions
 * {@hide}
 */
CarClass(CTransitionSliding), public Transition
{
public:
    ITRANSITION_METHODS_DECL();

    CTransitionSliding();

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
     *            class
     * @param direction direction shall be one of the supported directions like
     *            RIGHT_OUT_LEFT_IN
     *
     * @throws IllegalArgumentException if behavior is not supported.
     */
    CARAPI constructor(
        /* [in] */ const String& transitionId,
        /* [in] */ IMediaItem* afterMediaItem,
        /* [in] */ IMediaItem* beforeMediaItem,
        /* [in] */ Int64 durationMs,
        /* [in] */ Int32 behavior,
        /* [in] */ Int32 direction);

    /**
     * Get the sliding direction.
     *
     * @return The sliding direction
     */
    CARAPI GetDirection(
        /* [out] */ Int32* result);

    CARAPI Generate();

private:
    // The sliding transitions
    Int32 mSlidingDirection;

};

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
#endif // __CTRANSITIONSLIDING_H__

