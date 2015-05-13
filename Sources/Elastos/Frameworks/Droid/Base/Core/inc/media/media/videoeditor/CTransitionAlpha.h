
#ifndef __CTRANSITIONALPHA_H__
#define __CTRANSITIONALPHA_H__

#include "_CTransitionAlpha.h"
#include <ext/frameworkext.h>
#include "media/media/videoeditor/Transition.h"
#include "media/media/videoeditor/TransitionMacro.h"

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

/**
 * This class allows to render an "alpha blending" transition according to a
 * bitmap mask. The mask shows the shape of the transition all along the
 * duration of the transition: just before the transition, video 1 is fully
 * displayed. When the transition starts, as the time goes on, pixels of video 2
 * replace pixels of video 1 according to the gray scale pixel value of the
 * mask.
 * {@hide}
 */
CarClass(CTransitionAlpha), public Transition
{
public:
    ITRANSITION_METHODS_DECL()

    CTransitionAlpha();

    CTransitionAlpha(
        /* [in] */ const String& transitionId,
        /* [in] */ IMediaItem* afterMediaItem,
        /* [in] */ IMediaItem* beforeMediaItem,
        /* [in] */ Int64 durationMs,
        /* [in] */ Int32 behavior,
        /* [in] */ const String& maskFilename,
        /* [in] */ Int32 blendingPercent,
        /* [in] */ Boolean invert);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    /**
     * Constructor
     *
     * @param transitionId The transition id
     * @param afterMediaItem The transition is applied to the end of this media
     *            item
     * @param beforeMediaItem The transition is applied to the beginning of this
     *            media item
     * @param durationMs duration of the transition in milliseconds
     * @param behavior behavior is one of the behavior defined in Transition
     *            class
     * @param maskFilename JPEG file name. The dimension of the image
     *           corresponds to 720p (16:9 aspect ratio). Mask files are
     *           shared between video editors and can be created in the
     *           projects folder (the parent folder for all projects).
     * @param blendingPercent The blending percent applied
     * @param invert true to invert the direction of the alpha blending
     * @throws IllegalArgumentException if behavior is not supported, or if
     *             direction are not supported.
     */
    CARAPI constructor(
        /* [in] */ const String& transitionId,
        /* [in] */ IMediaItem* afterMediaItem,
        /* [in] */ IMediaItem* beforeMediaItem,
        /* [in] */ Int64 durationMs,
        /* [in] */ Int32 behavior,
        /* [in] */ const String& maskFilename,
        /* [in] */ Int32 blendingPercent,
        /* [in] */ Boolean invert);

    CARAPI GetRGBFileWidth(
        /* [out] */ Int32* result);

    CARAPI GetRGBFileHeight(
        /* [out] */ Int32* result);

    CARAPI GetPNGMaskFilename(
        /* [out] */ String* result);

    /**
     * Get the blending percentage
     *
     * @return The blending percentage
     */
    CARAPI GetBlendingPercent(
        /* [out] */ Int32* result);

    /**
     * Get the filename of the mask.
     *
     * @return The mask filename
     */
    CARAPI GetMaskFilename(
        /* [out] */ String* result);

    /**
     * Check if the alpha blending direction is inverted.
     *
     * @return true if the direction of the alpha blending is inverted
     */
    CARAPI IsInvert(
        /* [out] */ Boolean* result);

    CARAPI Generate();

private:
    /** This is the input JPEG file for the mask */
    String mMaskFilename;

    /**
     * This is percentage (between 0 and 100) of blending between video 1 and
     * video 2 if this value equals 0, then the mask is strictly applied if this
     * value equals 100, then the mask is not at all applied (no transition
     * effect)
     */
    Int32 mBlendingPercent;

    /**
     * If true, this value inverts the direction of the mask: white pixels of
     * the mask show video 2 pixels first black pixels of the mask show video 2
     * pixels last.
     */
    Boolean mIsInvert;
    Int32 mWidth;
    Int32 mHeight;
    String mRGBMaskFile;

};

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
#endif // __CTRANSITIONALPHA_H__

