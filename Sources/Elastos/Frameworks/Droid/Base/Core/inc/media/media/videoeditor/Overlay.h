
#ifndef __OVERLAY_H__
#define __OVERLAY_H__

#include <ext/frameworkext.h>

using Elastos::Utility::IObjectStringMap;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

extern "C" const InterfaceID EIID_Overlay;

class Overlay
{
public:
    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    /**
     * Default constructor
     */
    // @SuppressWarnings("unused")
    Overlay();

    /**
     * Constructor
     *
     * @param mediaItem The media item owner
     * @param overlayId The overlay id
     * @param startTimeMs The start time relative to the media item start time
     * @param durationMs The duration
     *
     * @throws IllegalArgumentException if the file type is not PNG or the
     *      startTimeMs and durationMs are incorrect.
     */
    Overlay(
        /* [in] */ IMediaItem* mediaItem,
        /* [in] */ const String& overlayId,
        /* [in] */ Int64 startTimeMs,
        /* [in] */ Int64 durationMs);

    /**
     * Get the overlay ID.
     *
     * @return The of the overlay
     */
    CARAPI GetId(
        /* [out] */ String* result);

    /**
     * Get the duration of overlay.
     *
     * @return The duration of the overlay effect
     */
    CARAPI GetDuration(
        /* [out] */ Int64* result);

    /**
     * If a preview or export is in progress, then this change is effective for
     * next preview or export session.
     *
     * @param durationMs The duration in milliseconds
     */
    CARAPI SetDuration(
        /* [in] */ Int64 durationMs);

    /**
     * Get the start time of overlay.
     *
     * @return the start time of the overlay
     */
    CARAPI GetStartTime(
        /* [out] */ Int64* result);

    /**
     * Set the start time for the overlay. If a preview or export is in
     * progress, then this change is effective for next preview or export
     * session.
     *
     * @param startTimeMs start time in milliseconds
     */
    CARAPI SetStartTime(
        /* [in] */ Int64 startTimeMs);

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
     * @return The media item owner.
     */
    CARAPI GetMediaItem(
        /* [out] */ IMediaItem** result);

    /**
     * Set a user attribute
     *
     * @param name The attribute name
     * @param value The attribute value
     */
    CARAPI SetUserAttribute(
        /* [in] */ const String& name,
        /* [in] */ const String& value);

    /**
     * Get the current user attributes set.
     *
     * @return The user attributes
     */
    CARAPI GetUserAttributes(
        /* [out] */ IObjectStringMap** result);

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI GetHashCode(
        /* [out] */ Int32* result);

protected:
    CARAPI Init(
        /* [in] */ IMediaItem* mediaItem,
        /* [in] */ const String& overlayId,
        /* [in] */ Int64 startTimeMs,
        /* [in] */ Int64 durationMs);

private:
    /**
     *  Instance variables
     */
    String mUniqueId;
    /**
     *  The overlay owner
     */
    AutoPtr<IMediaItem> mMediaItem;
    /**
     *  user attributes
     */
    AutoPtr<IObjectStringMap> mUserAttributes;

protected:
    Int64 mStartTimeMs;
    Int64 mDurationMs;
};

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __OVERLAY_H__

