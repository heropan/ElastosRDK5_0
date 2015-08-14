
#ifndef __ELASTOS_DROID_MEDIA_VIDEOEDITOR_EFFECT_H__
#define __ELASTOS_DROID_MEDIA_VIDEOEDITOR_EFFECT_H__

#include <ext/frameworkext.h>

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

extern "C" const InterfaceID EIID_Effect;

/**
 * This is the super class for all effects. An effect can only be applied to a
 * single media item.
 * {@hide}
 */
class Effect
{
public:
    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    /**
     * Default constructor
     */
    // @SuppressWarnings("unused")
    Effect();

    /**
     * Constructor
     *
     * @param mediaItem The media item owner
     * @param effectId The effect id
     * @param startTimeMs The start time relative to the media item to which it
     *            is applied
     * @param durationMs The effect duration in milliseconds
     */
    Effect(
        /* [in] */ IMediaItem* mediaItem,
        /* [in] */ const String& effectId,
        /* [in] */ Int64 startTimeMs,
        /* [in] */ Int64 durationMs);

    /**
     * Get the id of the effect.
     *
     * @return The id of the effect
     */
    CARAPI GetId(
        /* [out] */ String* result);

    /**
     * Set the duration of the effect. If a preview or export is in progress,
     * then this change is effective for next preview or export session.
     *
     * @param durationMs of the effect in milliseconds
     */
    CARAPI SetDuration(
        /* [in] */ Int64 durationMs);

    /**
     * Get the duration of the effect
     *
     * @return The duration of the effect in milliseconds
     */
    CARAPI GetDuration(
        /* [out] */ Int64* result);

    /**
     * Set start time of the effect. If a preview or export is in progress, then
     * this change is effective for next preview or export session.
     *
     * @param startTimeMs The start time of the effect relative to the beginning
     *            of the media item in milliseconds
     */
    CARAPI SetStartTime(
        /* [in] */ Int64 startTimeMs);

    /**
     * Get the start time of the effect
     *
     * @return The start time in milliseconds
     */
    CARAPI GetStartTime(
        /* [out] */ Int64* result);

    /**
     * Set the start time and duration
     *
     * @param startTimeMs start time in milliseconds
     * @param durationMs The duration in milliseconds
     */
    CARAPI SetStartTimeAndDuration(
        /* [in] */ Int64 startTimeMs,
        /* [in] */ Int64 durationMs);

    /**
     * Get the media item owner.
     *
     * @return The media item owner
     */
    CARAPI GetMediaItem(
        /* [out] */ IMediaItem** result);

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI GetHashCode(
        /* [out] */ Int32* result);

protected:
    CARAPI Init(
        /* [in] */ IMediaItem* mediaItem,
        /* [in] */ const String& effectId,
        /* [in] */ Int64 startTimeMs,
        /* [in] */ Int64 durationMs);

private:
    /**
     *  Instance variables
     */
    String mUniqueId;
    /**
     *  The effect owner
     */
    AutoPtr<IMediaItem> mMediaItem;

protected:
    Int64 mDurationMs;
    /**
     *  The start time of the effect relative to the beginning
     *  of the media item
     */
    Int64 mStartTimeMs;
};

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_MEDIA_VIDEOEDITOR_EFFECT_H__

