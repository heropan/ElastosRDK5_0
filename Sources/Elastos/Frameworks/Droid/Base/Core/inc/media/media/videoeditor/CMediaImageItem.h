
#ifndef __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CMEDIAIMAGEITEM_H__
#define __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CMEDIAIMAGEITEM_H__

#include "_Elastos_Droid_Media_Videoeditor_CMediaImageItem.h"
#include <ext/frameworkext.h>
#include "media/media/videoeditor/MediaItem.h"


namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {
/**
 * This is the super class for all effects. An effect can only be applied to a
 * single media item.
 * {@hide}
 */
CarClass(CMediaImageItem), public MediaItem
{
public:
    IMEDIAITEM_METHODS_DECL()

    CMediaImageItem();

    CMediaImageItem(
        /* [in] */ IVideoEditor* editor,
        /* [in] */ const String& mediaItemId,
        /* [in] */ const String& filename,
        /* [in] */ Int64 durationMs,
        /* [in] */ Int32 renderingMode);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    /**
     * Constructor
     *
     * @param editor The video editor reference
     * @param mediaItemId The media item id
     * @param filename The image file name
     * @param durationMs The duration of the image on the storyboard
     * @param renderingMode The rendering mode
     *
     * @throws IOException
     */
    CARAPI constructor(
        /* [in] */ IVideoEditor* editor,
        /* [in] */ const String& mediaItemId,
        /* [in] */ const String& filename,
        /* [in] */ Int64 durationMs,
        /* [in] */ Int32 renderingMode);

    CARAPI GetFileType(
        /* [out] */ Int32* result);

    CARAPI GetWidth(
        /* [out] */ Int32* result);

    CARAPI GetHeight(
        /* [out] */ Int32* result);

    CARAPI GetScaledWidth(
        /* [out] */ Int32* result);

    CARAPI GetScaledHeight(
        /* [out] */ Int32* result);

    CARAPI GetAspectRatio(
        /* [out] */ Int32* result);

    /**
     * This method will adjust the duration of bounding transitions, effects
     * and overlays if the current duration of the transactions become greater
     * than the maximum allowable duration.
     *
     * @param durationMs The duration of the image in the storyboard timeline
     */
    CARAPI SetDuration(
        /* [in] */ Int64 durationMs);

    CARAPI GetDuration(
        /* [out] */ Int64* result);

    CARAPI GetTimelineDuration(
        /* [out] */ Int64* result);

    CARAPI GetThumbnail(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int64 timeMs,
        /* [out] */ IBitmap** result);

    CARAPI GetThumbnailList(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int64 startMs,
        /* [in] */ Int64 endMs,
        /* [in] */ Int32 thumbnailCount,
        /* [in] */ ArrayOf<Int32>* indices,
        /* [in] */ IMediaItemGetThumbnailListCallback* callback);

    static CARAPI NextPowerOf2(
        /* [in] */ Int32 n,
        /* [out] */ Int32* result);

    /**
     * @return The scaled image file name
     */
    CARAPI GetScaledImageFileName(
        /* [out] */ String* result);

    /**
     * @return The generated Kenburns clip height.
     */
    CARAPI GetGeneratedClipHeight(
        /* [out] */ Int32* result);

    /**
     * @return The generated Kenburns clip width.
     */
    CARAPI GetGeneratedClipWidth(
        /* [out] */ Int32* result);

    /**
     * @return The file name of image which is decoded and stored
     * in RGB format
     */
    CARAPI GetDecodedImageFileName(
        /* [out] */ String* result);

    /**
     * This function sets the Ken Burn effect generated clip
     * name.
     *
     * @param generatedFilePath The name of the generated clip
     */
    //@Override
    CARAPI SetGeneratedImageClip(
        /* [in] */ const String& generatedFilePath);

    /**
     * @return The name of the image clip
     * generated with ken burns effect.
     */
    //@Override
    CARAPI GetGeneratedImageClip(
        /* [out] */ String* result);

    CARAPI InvalidateTransitions(
        /* [in] */ Int64 startTimeMs,
        /* [in] */ Int64 durationMs);

    CARAPI InvalidateTransitions(
        /* [in] */ Int64 oldStartTimeMs,
        /* [in] */ Int64 oldDurationMs,
        /* [in] */ Int64 newStartTimeMs,
        /* [in] */ Int64 newDurationMs);

    /**
     * This function invalidates the rgb image clip,ken burns effect clip,
     * and scaled image clip
     */
    CARAPI Invalidate();

    /**
     * @param KenBurnEffect object.
     * @return an Object of {@link ClipSettings} with Ken Burns
     * generated clip name
     */
    CARAPI GenerateKenburnsClip(
        /* [in] */ IEffectKenBurns* effectKB,
        /* [out] */ MediaArtistNativeHelper::ClipSettings** settings);

    /**
     * @return an Object of {@link ClipSettings} with Image Clip
     * properties data populated.If the image has Ken Burns effect applied,
     * then file path contains generated image clip name with Ken Burns effect
     */
    CARAPI GetImageClipProperties(
        /* [out] */ MediaArtistNativeHelper::ClipSettings** settings);

    CARAPI AddEffect(
        /* [in] */ IEffect* effect);

private:

    /**
     * Invalidate the begin transition if any effects and overlays overlap
     * with the begin transition.
     *
     * @param effects List of effects to check for transition overlap
     * @param overlays List of overlays to check for transition overlap
     */
    CARAPI_(void) InvalidateBeginTransition(
        /* [in] */ ArrayOf<IEffect*>* effects,
        /* [in] */ ArrayOf<IOverlay*>* overlays);

    /**
     * Invalidate the end transition if any effects and overlays overlap
     * with the end transition.
     */
    CARAPI_(void) InvalidateEndTransition();

    /**
     * Adjust the start time and/or duration of effects.
     *
     * @return The list of effects which were adjusted
     */
    CARAPI_(AutoPtr<ArrayOf<IEffect*> >) AdjustEffects();

    /**
     * Adjust the start time and/or duration of overlays.
     *
     * @return The list of overlays which were adjusted
     */
    CARAPI_(AutoPtr<ArrayOf<IOverlay*> >) AdjustOverlays();

    /**
     * This function get the proper width by given aspect ratio
     * and height.
     *
     * @param aspectRatio  Given aspect ratio
     * @param height  Given height
     */
    CARAPI_(Int32) GetWidthByAspectRatioAndHeight(
        /* [in] */ Int32 aspectRatio,
        /* [in] */ Int32 height);

    /**
     * @param KenBurnEffect object.
     * @return an Object of {@link ClipSettings} with Ken Burn settings
     * needed to generate the clip
     */
    CARAPI_(AutoPtr<MediaArtistNativeHelper::ClipSettings>) GetKenBurns(
        /* [in] */ IEffectKenBurns* effectKB);

    /**
     * Resize a bitmap to the specified width and height
     *
     * @param filename The filename
     * @param width The thumbnail width
     * @param height The thumbnail height
     *
     * @return The resized bitmap
     */
    CARAPI_(AutoPtr<IBitmap>) ScaleImage(
        /* [in] */ const String& filename,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

private:
    static const String TAG;

    static AutoPtr<IPaint> sResizePaint;

    Int32 mWidth;
    Int32 mHeight;
    Int32 mAspectRatio;
    Int64 mDurationMs;
    Int32 mScaledWidth, mScaledHeight;
    String mScaledFilename;
    AutoPtr<IVideoEditorImpl> mVideoEditor;
    String mDecodedFilename;
    Int32 mGeneratedClipHeight;
    Int32 mGeneratedClipWidth;
    String mFileName;

    AutoPtr<MediaArtistNativeHelper> mMANativeHelper;

};

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CMEDIAIMAGEITEM_H__

