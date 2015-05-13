
#ifndef __MEDIAITEM_H__
#define __MEDIAITEM_H__

#include <ext/frameworkext.h>
#include <elastos/List.h>
#include "MediaArtistNativeHelper.h"

using Elastos::Utility::List;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Media::Videoeditor::MediaArtistNativeHelper;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

extern "C" const InterfaceID EIID_MediaItem;

/**
 * This is the super class for all effects. An effect can only be applied to a
 * single media item.
 * {@hide}
 */
class MediaItem
{
private:
    class GetThumbnailListCallback
        : public ElRefBase
        , public IMediaItemGetThumbnailListCallback
    {
    public:
        GetThumbnailListCallback(
            /* [in] */ MediaItem* host,
            /* [in] */ ArrayOf<IBitmap*>* bitmaps)
             : mHost(host)
             , mBitmaps(bitmaps)
        {}

        CAR_INTERFACE_DECL()

        CARAPI OnThumbnail(
            /* [in] */ IBitmap* bitmap,
            /* [in] */ Int32 index);

    private:
        MediaItem* mHost;
        ArrayOf<IBitmap*>* mBitmaps;
    };

public:
    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    /**
     * @return The id of the media item
     */
    CARAPI GetId(
        /* [out] */ String* result);

    /**
     * @return The media source file name
     */
    CARAPI GetFilename(
        /* [out] */ String* result);

    /**
     * If aspect ratio of the MediaItem is different from the aspect ratio of
     * the editor then this API controls the rendering mode.
     *
     * @param renderingMode rendering mode. It is one of:
     *            {@link #RENDERING_MODE_BLACK_BORDER},
     *            {@link #RENDERING_MODE_STRETCH}
     */
    CARAPI SetRenderingMode(
        /* [in] */ Int32 renderingMode);

    /**
     * @return The rendering mode
     */
    CARAPI GetRenderingMode(
        /* [out] */ Int32* result);

    /**
     * @return The begin transition
     */
    CARAPI GetBeginTransition(
        /* [out] */ ITransition** result);

    /**
     * @return The end transition
     */
    CARAPI GetEndTransition(
        /* [out] */ ITransition** result);

    /**
     * @return The timeline duration. This is the actual duration in the
     *         timeline (trimmed duration)
     */
    virtual CARAPI GetTimelineDuration(
        /* [out] */ Int64* result) = 0;

    /**
     * @return The is the full duration of the media item (not trimmed)
     */
    virtual CARAPI GetDuration(
        /* [out] */ Int64* result) = 0;

    /**
     * @return The source file type
     */
    virtual CARAPI GetFileType(
        /* [out] */ Int32* result) = 0;

    /**
     * @return Get the native width of the media item
     */
    virtual CARAPI GetWidth(
        /* [out] */ Int32* result) = 0;

    /**
     * @return Get the native height of the media item
     */
    virtual CARAPI GetHeight(
        /* [out] */ Int32* result) = 0;

    /**
     * Get aspect ratio of the source media item.
     *
     * @return the aspect ratio as described in MediaProperties.
     *         MediaProperties.ASPECT_RATIO_UNDEFINED if aspect ratio is not
     *         supported as in MediaProperties
     */
    virtual CARAPI GetAspectRatio(
        /* [out] */ Int32* result) = 0;

    /**
     * Add the specified effect to this media item.
     *
     * Note that certain types of effects cannot be applied to video and to
     * image media items. For example in certain implementation a Ken Burns
     * implementation cannot be applied to video media item.
     *
     * This method invalidates transition video clips if the
     * effect overlaps with the beginning and/or the end transition.
     *
     * @param effect The effect to apply
     * @throws IllegalStateException if a preview or an export is in progress
     * @throws IllegalArgumentException if the effect start and/or duration are
     *      invalid or if the effect cannot be applied to this type of media
     *      item or if the effect id is not unique across all the Effects
     *      added.
     */
    CARAPI AddEffect(
        /* [in] */ IEffect* effect);


    /**
     * Remove the effect with the specified id.
     *
     * This method invalidates a transition video clip if the effect overlaps
     * with a transition.
     *
     * @param effectId The id of the effect to be removed
     *
     * @return The effect that was removed
     * @throws IllegalStateException if a preview or an export is in progress
     */
    CARAPI RemoveEffect(
        /* [in] */ const String& effectId,
        /* [out] */ IEffect** result);

    /**
     * Find the effect with the specified id
     *
     * @param effectId The effect id
     * @return The effect with the specified id (null if it does not exist)
     */
    CARAPI GetEffect(
        /* [in] */ const String& effectId,
        /* [out] */ IEffect** result);

    /**
     * Get the list of effects.
     *
     * @return the effects list. If no effects exist an empty list will be
     *         returned.
     */
    CARAPI GetAllEffects(
        /* [out] */ ArrayOf<IEffect*>** result);

    /**
     * Add an overlay to the storyboard. This method invalidates a transition
     * video clip if the overlay overlaps with a transition.
     *
     * @param overlay The overlay to add
     * @throws IllegalStateException if a preview or an export is in progress or
     *             if the overlay id is not unique across all the overlays added
     *             or if the bitmap is not specified or if the dimensions of the
     *             bitmap do not match the dimensions of the media item
     * @throws FileNotFoundException, IOException if overlay could not be saved
     *             to project path
     */
    CARAPI AddOverlay(
        /* [in] */ IOverlay* overlay);

    /**
     * Remove the overlay with the specified id.
     *
     * This method invalidates a transition video clip if the overlay overlaps
     * with a transition.
     *
     * @param overlayId The id of the overlay to be removed
     *
     * @return The overlay that was removed
     * @throws IllegalStateException if a preview or an export is in progress
     */
    CARAPI RemoveOverlay(
        /* [in] */ const String& overlayId,
        /* [out] */ IOverlay** result);

    /**
     * Find the overlay with the specified id
     *
     * @param overlayId The overlay id
     *
     * @return The overlay with the specified id (null if it does not exist)
     */
    CARAPI GetOverlay(
        /* [in] */ const String& overlayId,
        /* [out] */ IOverlay** result);

    /**
     * Get the list of overlays associated with this media item
     *
     * Note that if any overlay source files are not accessible anymore,
     * this method will still provide the full list of overlays.
     *
     * @return The list of overlays. If no overlays exist an empty list will
     *         be returned.
     */
    CARAPI GetAllOverlays(
        /* [out] */ ArrayOf<IOverlay*>** result);

    /**
     * Create a thumbnail at specified time in a video stream in Bitmap format
     *
     * @param width width of the thumbnail in pixels
     * @param height height of the thumbnail in pixels
     * @param timeMs The time in the source video file at which the thumbnail is
     *            requested (even if trimmed).
     *
     * @return The thumbnail as a Bitmap.
     *
     * @throws IOException if a file error occurs
     * @throws IllegalArgumentException if time is out of video duration
     */
    virtual CARAPI GetThumbnail(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int64 timeMs,
        /* [out] */ IBitmap** result) = 0;

    /**
     * Get the array of Bitmap thumbnails between start and end.
     *
     * @param width width of the thumbnail in pixels
     * @param height height of the thumbnail in pixels
     * @param startMs The start of time range in milliseconds
     * @param endMs The end of the time range in milliseconds
     * @param thumbnailCount The thumbnail count
     * @param indices The indices of the thumbnails wanted
     * @param callback The callback used to pass back the bitmaps
     *
     * @throws IOException if a file error occurs
     */
    virtual CARAPI GetThumbnailList(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int64 startMs,
        /* [in] */ Int64 endMs,
        /* [in] */ Int32 thumbnailCount,
        /* [in] */ ArrayOf<Int32>* indices,
        /* [in] */ IMediaItemGetThumbnailListCallback* listcallback) = 0;

    // This is for compatibility, only used in tests.
    CARAPI GetThumbnailListEx(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int64 startMs,
        /* [in] */ Int64 endMs,
        /* [in] */ Int32 thumbnailCount,
        /* [out,callee] */ ArrayOf<IBitmap*>** result);

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI GetHashCode(
        /* [out] */ Int32* result);

    /**
     * @param transition The beginning transition
     */
    CARAPI SetBeginTransition(
        /* [in] */ ITransition* transition);

    /**
     * @param transition The end transition
     */
    CARAPI SetEndTransition(
        /* [in] */ ITransition* transition);

    /**
     * Set the filepath of the generated image clip when the effect is added.
     *
     * @param The filepath of the generated image clip.
     */
    CARAPI SetGeneratedImageClip(
        /* [in] */ const String& generatedFilePath);

    /**
     * Get the filepath of the generated image clip when the effect is added.
     *
     * @return The filepath of the generated image clip (null if it does not
     *         exist)
     */
    CARAPI GetGeneratedImageClip(
        /* [out] */ String* result);

    /**
     * @param flag The flag to indicate if regeneration of clip is true or
     *            false.
     */
    CARAPI SetRegenerateClip(
        /* [in] */ Boolean flag);

    /**
     * @return flag The flag to indicate if regeneration of clip is true or
     *         false.
     */
    CARAPI GetRegenerateClip(
        /* [out] */ Boolean* result);

    /**
     * Invalidate the start and end transitions if necessary
     *
     * @param startTimeMs The start time of the effect or overlay
     * @param durationMs The duration of the effect or overlay
     */
    virtual CARAPI InvalidateTransitions(
        /* [in ]*/ Int64 startTimeMs,
        /* [in ]*/ Int64 durationMs) = 0;

    /**
     * Invalidate the start and end transitions if necessary. This method is
     * typically called when the start time and/or duration of an overlay or
     * effect is changing.
     *
     * @param oldStartTimeMs The old start time of the effect or overlay
     * @param oldDurationMs The old duration of the effect or overlay
     * @param newStartTimeMs The new start time of the effect or overlay
     * @param newDurationMs The new duration of the effect or overlay
     */
    virtual CARAPI InvalidateTransitionsEx(
        /* [in ]*/ Int64 oldStartTimeMs,
        /* [in ]*/ Int64 oldDurationMs,
        /* [in ]*/ Int64 newStartTimeMs,
        /* [in ]*/ Int64 newDurationMs) = 0;

    /**
     * @return MediaArtistNativeHleper context
     */
    CARAPI GetNativeContext(
        /* [out] */ MediaArtistNativeHelper** helper);

    /**
     * Initialises ClipSettings fields to default value
     *
     * @param ClipSettings object
     *{@link android.media.videoeditor.MediaArtistNativeHelper.ClipSettings}
     */
    CARAPI InitClipSettings(
        /* [in] */ MediaArtistNativeHelper::ClipSettings* clipSettings);

    /**
     * @return ClipSettings object with populated data
     *{@link android.media.videoeditor.MediaArtistNativeHelper.ClipSettings}
     */
    CARAPI GetClipSettings(
        /* [out] */ MediaArtistNativeHelper::ClipSettings** clipSettings);

    /**
     * Generates a black frame to be used for generating
     * begin transition at first media item in storyboard
     * or end transition at last media item in storyboard
     *
     * @param ClipSettings object
     *{@link android.media.videoeditor.MediaArtistNativeHelper.ClipSettings}
     */
    CARAPI GenerateBlankFrame(
        /* [in] */ MediaArtistNativeHelper::ClipSettings* clipSettings);

    /**
     * Invalidates the blank frame generated
     */
    CARAPI InvalidateBlankFrame();

protected:
    MediaItem();

    /**
     * Constructor
     *
     * @param editor The video editor reference
     * @param mediaItemId The MediaItem id
     * @param filename name of the media file.
     * @param renderingMode The rendering mode
     * @throws IOException if file is not found
     * @throws IllegalArgumentException if a capability such as file format is
     *             not supported the exception object contains the unsupported
     *             capability
     */
    MediaItem(
        /* [in] */ IVideoEditor* editor,
        /* [in] */ const String& mediaItemId,
        /* [in] */ const String& filename,
        /* [in] */ Int32 renderingMode);

    Init(
        /* [in] */ IVideoEditor* editor,
        /* [in] */ const String& mediaItemId,
        /* [in] */ const String& filename,
        /* [in] */ Int32 renderingMode);

    /**
     * Check if two items overlap in time
     *
     * @param startTimeMs1 Item 1 start time
     * @param durationMs1 Item 1 duration
     * @param startTimeMs2 Item 2 start time
     * @param durationMs2 Item 2 end time
     * @return true if the two items overlap
     */
    CARAPI_(Boolean) IsOverlapping(
        /* [in]*/ Int64 startTimeMs1,
        /* [in]*/ Int64 durationMs1,
        /* [in]*/ Int64 startTimeMs2,
        /* [in]*/ Int64 durationMs2);

    /**
     * Adjust the duration transitions.
     */
    CARAPI_(void) AdjustTransitions();

private:
    /**
     *  The unique id of the MediaItem
     */
    String mUniqueId;

    /**
     *  List of effects
     */
    List<AutoPtr<IEffect> > mEffects;

    /**
     *  List of overlays
     */
    List<AutoPtr<IOverlay> > mOverlays;

    /**
     *  The rendering mode
     */
    Int32 mRenderingMode;

    AutoPtr<MediaArtistNativeHelper> mMANativeHelper;

    String mProjectPath;

    Boolean mBlankFrameGenerated;

    String mBlankFrameFilename;

protected:
    /**
     *  The name of the file associated with the MediaItem
     */
    String mFilename;

    /**
     *  Beginning and end transitions
     */
    AutoPtr<ITransition> mBeginTransition;

    AutoPtr<ITransition> mEndTransition;

    String mGeneratedImageClip;

    Boolean mRegenerateClip;

};

} // namespace videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __MEDIAITEM_H__

