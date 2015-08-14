
#ifndef __ELASTOS_DROID_MEDIA_VIDEOEDITOR_TRANSITION_H__
#define __ELASTOS_DROID_MEDIA_VIDEOEDITOR_TRANSITION_H__

#include <ext/frameworkext.h>
#include "MediaArtistNativeHelper.h"
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

extern "C" const InterfaceID EIID_Transition;

/**
 * This class is super class for all transitions. Transitions (with the
 * exception of TransitionAtStart and TransitioAtEnd) can only be inserted
 * between media items.
 *
 * Adding a transition between MediaItems makes the
 * duration of the storyboard shorter by the duration of the Transition itself.
 * As a result, if the duration of the transition is larger than the smaller
 * duration of the two MediaItems associated with the Transition, an exception
 * will be thrown.
 *
 * During a transition, the audio track are cross-fading
 * automatically. {@hide}
 */
class Transition
{
public:
    /**
     * An object of this type cannot be instantiated by using the default
     * constructor
     */
    // @SuppressWarnings("unused")
    Transition();

    /**
     * Constructor
     *
     * @param transitionId The transition id
     * @param afterMediaItem The transition is applied to the end of this
     *      media item
     * @param beforeMediaItem The transition is applied to the beginning of
     *      this media item
     * @param durationMs The duration of the transition in milliseconds
     * @param behavior The transition behavior
     */
    Transition(
        /* [in] */ const String& transitionId,
        /* [in] */ AutoPtr<IMediaItem> afterMediaItem,
        /* [in] */ AutoPtr<IMediaItem> beforeMediaItem,
        /* [in] */ Int64 durationMs,
        /* [in] */ Int32 behavior);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    /**
     * Get the ID of the transition.
     *
     * @return The ID of the transition
     */
    CARAPI GetId(
        /* [out] */ String* result);

    /**
     * Get the media item at the end of which the transition is applied.
     *
     * @return The media item at the end of which the transition is applied
     */
    CARAPI GetAfterMediaItem(
        /* [out] */ IMediaItem** result);

    /**
     * Get the media item at the beginning of which the transition is applied.
     *
     * @return The media item at the beginning of which the transition is
     *      applied
     */
    CARAPI GetBeforeMediaItem(
        /* [out] */ IMediaItem** result);

    /**
     * Set the duration of the transition.
     *
     * @param durationMs the duration of the transition in milliseconds
     */
    CARAPI SetDuration(
        /* [in] */ Int64 durationMs);

    /**
     * Get the duration of the transition.
     *
     * @return the duration of the transition in milliseconds
     */
    CARAPI GetDuration(
        /* [out] */ Int64* result);

    /**
     * The duration of a transition cannot be greater than half of the minimum
     * duration of the bounding media items.
     *
     * @return The maximum duration of this transition
     */
    CARAPI GetMaximumDuration(
        /* [out] */ Int64* result);

    /**
     * Get the behavior of the transition.
     *
     * @return The behavior
     */
    CARAPI GetBehavior(
        /* [out] */ Int32* result);

    CARAPI Equals(
        /* [in]*/ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI GetHashCode(
        /* [out] */ Int32* result);

    /**
     * Get the transition data.
     *
     * @return The transition data in TransitionSettings object
     * {@link android.media.videoeditor.MediaArtistNativeHelper.TransitionSettings}
     */
    CARAPI GetTransitionSettings(
        /* [out] */ MediaArtistNativeHelper::TransitionSettings** settings);

    /**
     * Checks if the effect and overlay applied on a media item
     * overlaps with the transition on media item.
     *
     * @param m The media item
     * @param clipSettings The ClipSettings object
     * @param clipNo The clip no.(out of the two media items
     * associated with current transition)for which the effect
     * clip should be generated
     * @return List of effects that overlap with the transition
     */

    CARAPI_(AutoPtr<List<AutoPtr<MediaArtistNativeHelper::EffectSettings> > >) IsEffectandOverlayOverlapping(
        /* [in] */ IMediaItem* m,
        /* [in] */ MediaArtistNativeHelper::ClipSettings* clipSettings,
        /* [in] */ Int32 clipNo);

    /**
     * Generate the video clip for the specified transition. This method may
     * block for a significant amount of time. Before the method completes
     * execution it sets the mFilename to the name of the newly generated
     * transition video clip file.
     */
    CARAPI Generate();

    /**
     * Set the transition filename.
     */
    CARAPI SetFilename(
        /* [in] */ const String& filename);

    /**
     * Get the transition filename.
     */
    CARAPI GetFilename(
        /* [out] */ String* result);

    /**
     * Remove any resources associated with this transition
     */
    CARAPI Invalidate();

    /**
     * Check if the transition is generated.
     *
     * @return true if the transition is generated
     */
    CARAPI IsGenerated(
        /* [out] */ Boolean* result);

protected:
    Init(
        /* [in] */ const String& transitionId,
        /* [in] */ AutoPtr<IMediaItem> afterMediaItem,
        /* [in] */ AutoPtr<IMediaItem> beforeMediaItem,
        /* [in] */ Int64 durationMs,
        /* [in] */ Int32 behavior);

private:
    /**
     *  The transition behavior
     */
    static const Int32 BEHAVIOR_MIN_VALUE;


    static const Int32 BEHAVIOR_MAX_VALUE;

    /**
     *  The unique id of the transition
     */
    String mUniqueId;

    /**
     *  The transition is applied at the end of this media item
     */
    AutoPtr<IMediaItem> mAfterMediaItem;
    /**
     *  The transition is applied at the beginning of this media item
     */
    AutoPtr<IMediaItem> mBeforeMediaItem;

protected:
    /**
     *  The transition behavior
     */
    Int32 mBehavior;

    /**
     *  The transition duration
     */
    Int64 mDurationMs;

    /**
     *  The transition filename
     */
    String mFilename;

    AutoPtr<MediaArtistNativeHelper> mNativeHelper;

};

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
#endif // __ELASTOS_DROID_MEDIA_VIDEOEDITOR_TRANSITION_H__

