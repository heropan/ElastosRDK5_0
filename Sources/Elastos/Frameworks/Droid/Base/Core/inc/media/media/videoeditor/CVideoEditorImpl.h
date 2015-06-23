
#ifndef __CVIDEOEDITORIMPL_H__
#define __CVIDEOEDITORIMPL_H__

#include "_CVideoEditorImpl.h"
#include <ext/frameworkext.h>
#include <elastos/utility/etl/List.h>
#include "MediaArtistNativeHelper.h"

using Elastos::Utility::Etl::List;
using Org::Xmlpull::V1::IXmlPullParser;
using Elastos::Droid::View::ISurfaceHolder;
using Elastos::Utility::Concurrent::ISemaphore;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

/**
 * The VideoEditorFactory class must be used to instantiate VideoEditor objects
 * by creating a new project {@link #create(String)} or by loading an
 * existing project {@link #load(String)}.
 * {@hide}
 */
CarClass(CVideoEditorImpl)
{
public:
    CVideoEditorImpl();

    /**
     * Constructor
     *
     * @param projectPath - The path where the VideoEditor stores all files
     *        related to the project
     */
    CARAPI constructor(
        /* [in] */ const String& projectPath);

    /*
     * {@inheritDoc}
     */
    //synchronized
    CARAPI AddAudioTrack(
        /* [in] */ IAudioTrack2* audioTrack);

    /*
     * {@inheritDoc}
     */
    //synchronized
    CARAPI AddMediaItem(
        /* [in] */ IMediaItem* mediaItem);

    /*
     * {@inheritDoc}
     */
    //synchronized
    CARAPI AddTransition(
        /* [in] */ ITransition* transition);

    /*
     * {@inheritDoc}
     */
    CARAPI CancelExport(
        /* [in] */ const String& filename);

    /*
     * {@inheritDoc}
     */
    CARAPI Export(
        /* [in] */ const String& filename,
        /* [in] */ Int32 height,
        /* [in] */ Int32 bitrate,
        /* [in] */ Int32 audioCodec,
        /* [in] */ Int32 videoCodec,
        /* [in] */ IVideoEditorExportProgressListener* listener);

    /*
     * {@inheritDoc}
     */
    CARAPI Export(
        /* [in] */ const String& filename,
        /* [in] */ Int32 height,
        /* [in] */ Int32 bitrate,
        /* [in] */ IVideoEditorExportProgressListener* listener);

    /*
     * {@inheritDoc}
     */
    CARAPI GeneratePreview(
        /* [in] */ IVideoEditorMediaProcessingProgressListener* listener);

    /*
     * {@inheritDoc}
     */
    CARAPI GetAllAudioTracks(
        /* [out] */ IObjectContainer** result);

    /*
     * {@inheritDoc}
     */
    CARAPI GetAllMediaItems(
        /* [out] */ IObjectContainer** result);
    /*
     * {@inheritDoc}
     */
    CARAPI GetAllTransitions(
        /* [out] */ IObjectContainer** result);

    /*
     * {@inheritDoc}
     */
    CARAPI GetAspectRatio(
        /* [out] */ Int32* result);

    /*
     * {@inheritDoc}
     */
    CARAPI GetAudioTrack(
        /* [in] */ const String& audioTrackId,
        /* [out] */ IAudioTrack2** result);

    /*
     * {@inheritDoc}
     */
    CARAPI GetDuration(
        /* [out] */ Int64* result);

    /*
     * {@inheritDoc}
     */
    //synchronized
    CARAPI GetMediaItem(
        /* [in] */ const String& mediaItemId,
        /* [out] */ IMediaItem** result);

    /*
     * {@inheritDoc}
     */
    CARAPI GetPath(
        /* [out]*/ String* result);

    /*
     * {@inheritDoc}
     */
    CARAPI GetTransition(
        /* [in] */ const String& transitionId,
        /* [out] */ ITransition** result);

    /*
     * {@inheritDoc}
     */
    //synchronized
    CARAPI InsertAudioTrack(
        /* [in] */ IAudioTrack2* audioTrack,
        /* [in] */ const String& afterAudioTrackId);

    /*
     * {@inheritDoc}
     */
    //synchronized
    CARAPI InsertMediaItem(
        /* [in] */ IMediaItem* mediaItem,
        /* [in] */ const String& afterMediaItemId);

    /*
     * {@inheritDoc}
     */
    //synchronized
    CARAPI MoveAudioTrack(
        /* [in] */ const String& audioTrackId,
        /* [in] */ const String& afterAudioTrackId);

    /*
     * {@inheritDoc}
     */
    //synchronized
    CARAPI MoveMediaItem(
        /* [in] */ const String& mediaItemId,
        /* [in] */ const String& afterMediaItemId);

    /*
     * {@inheritDoc}
     */
    CARAPI ReleaseResources();

    /*
     * {@inheritDoc}
     */
    //synchronized
    CARAPI RemoveAllMediaItems();

    /*
     * {@inheritDoc}
     */
    //synchronized
    CARAPI RemoveAudioTrack(
        /* [in] */ const String& audioTrackId,
        /* [out] */ IAudioTrack2** result);

    /*
     * {@inheritDoc}
     */
    //synchronized
    CARAPI RemoveMediaItem(
        /* [in] */ const String& mediaItemId,
        /* [out] */ IMediaItem** result);

    /*
     * {@inheritDoc}
     */
    //synchronized
    CARAPI RemoveTransition(
        /* [in] */ const String& transitionId,
        /* [out] */ ITransition** result);

    /*
     * {@inheritDoc}
     */
    CARAPI RenderPreviewFrame(
        /* [in] */ ISurfaceHolder* surfaceHolder,
        /* [in] */ Int64 timeMs,
        /* [in] */ IVideoEditorOverlayData* overlayData,
        /* [out] */ Int64* result);

    /*
     * {@inheritDoc}
     */
    CARAPI Save();

    /*
     * {@inheritDoc}
     */
    CARAPI SetAspectRatio(
        /* [in] */ Int32 aspectRatio);

    /*
     * {@inheritDoc}
     */
    CARAPI StartPreview(
        /* [in] */ ISurfaceHolder* surfaceHolder,
        /* [in] */ Int64 fromMs,
        /* [in] */ Int64 toMs,
        /* [in] */ Boolean loop,
        /* [in] */ Int32 callbackAfterFrameCount,
        /* [in] */ IVideoEditorPreviewProgressListener* listener);

    /*
     * {@inheritDoc}
     */
    CARAPI StopPreview(
        /* [out] */ Int64* result);

    /**
     * Clears the preview surface
     *
     * @param surfaceHolder SurfaceHolder where the preview is rendered
     * and needs to be cleared.
     */
    CARAPI ClearSurface(
        /* [in] */ ISurfaceHolder* surfaceHolder);

public:
    /*
     * @return The MediaArtistNativeHelper object
     */
    CARAPI GetNativeContext(
        /* [out] */ MediaArtistNativeHelper** result);

    /*
     * Force updates the timeline duration
     */
    CARAPI UpdateTimelineDuration();

private:
    //synchronized
    CARAPI_(AutoPtr<IMediaItem>) RemoveMediaItem(
        /* [in] */ const String& mediaItemId,
        /* [in] */ Boolean flag);

    /**
     *  the project form XML
     */
    CARAPI_(void) Load();

    /**
     * Parse the media item
     *
     * @param parser The parser
     * @return The media item
     */
    CARAPI_(AutoPtr<IMediaItem>) ParseMediaItem(
        /* [in] */ IXmlPullParser* parser);

    /**
     * Parse the transition
     *
     * @param parser The parser
     * @param ignoredMediaItems The list of ignored media items
     *
     * @return The transition
     */
    CARAPI_(AutoPtr<ITransition>) ParseTransition(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ List<String>* ignoredMediaItems);

    /**
     * Parse the overlay
     *
     * @param parser The parser
     * @param mediaItem The media item owner
     *
     * @return The overlay
     */
    CARAPI_(AutoPtr<IOverlay>) ParseOverlay(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IMediaItem* mediaItem);

    /**
     * Parse the effect
     *
     * @param parser The parser
     * @param mediaItem The media item owner
     *
     * @return The effect
     */
    CARAPI_(AutoPtr<IEffect>) ParseEffect(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IMediaItem* mediaItem);

    /**
     * Parse the audio track
     *
     * @param parser The parser
     *
     * @return The audio track
     */
    CARAPI_(AutoPtr<IAudioTrack2>) ParseAudioTrack(
        /* [in] */ IXmlPullParser* parser);

    /*
     * Remove transitions associated with the specified media item
     *
     * @param mediaItem The media item
     */
    CARAPI_(void) RemoveAdjacentTransitions(
        /* [in] */ IMediaItem* mediaItem);

    /**
     * Remove the transition before this media item
     *
     * @param index The media item index
     */
    CARAPI_(void) RemoveTransitionBefore(
        /* [in] */ Int32 index);

    /**
     * Remove the transition after this media item
     *
     * @param mediaItem The media item
     */
    CARAPI_(void) RemoveTransitionAfter(
        /* [in] */ Int32 index);

    /**
     * Compute the duration
     */
    CARAPI_(void) ComputeTimelineDuration();

    /*
     * Generate the project thumbnail
     */
    CARAPI_(void) GenerateProjectThumbnail();

    /**
     * Grab the semaphore which arbitrates access to the editor
     *
     * @throws InterruptedException
     */
    CARAPI_(void) Lock();

    /**
     * Tries to grab the semaphore with a specified time out which arbitrates access to the editor
     *
     * @param timeoutMs time out in ms.
     *
     * @return true if the semaphore is acquired, false otherwise
     * @throws InterruptedException
     */
    CARAPI_(Boolean) Lock(
        /* [in] */ Int64 timeoutMs);

    /**
     * Release the semaphore which arbitrates access to the editor
     */
    CARAPI_(void) Unlock();

    /**
     * Dumps the heap memory usage information to file
     */
    static CARAPI_(void) DumpHeap(
        /* [in] */ const String& filename);

private:
    /*
     *  Logging
     */
    static const String TAG;

    /*
     *  The project filename
     */
    static const String PROJECT_FILENAME;

    /*
     *  XML tags
     */
    static const String TAG_PROJECT;
    static const String TAG_MEDIA_ITEMS;
    static const String TAG_MEDIA_ITEM;
    static const String TAG_TRANSITIONS;
    static const String TAG_TRANSITION;
    static const String TAG_OVERLAYS;
    static const String TAG_OVERLAY;
    static const String TAG_OVERLAY_USER_ATTRIBUTES;
    static const String TAG_EFFECTS;
    static const String TAG_EFFECT;
    static const String TAG_AUDIO_TRACKS;
    static const String TAG_AUDIO_TRACK;

    static const String ATTR_ID;
    static const String ATTR_FILENAME;
    static const String ATTR_AUDIO_WAVEFORM_FILENAME;
    static const String ATTR_RENDERING_MODE;
    static const String ATTR_ASPECT_RATIO;
    static const String ATTR_REGENERATE_PCM;
    static const String ATTR_TYPE ;
    static const String ATTR_DURATION;
    static const String ATTR_START_TIME;
    static const String ATTR_BEGIN_TIME;
    static const String ATTR_END_TIME;
    static const String ATTR_VOLUME;
    static const String ATTR_BEHAVIOR;
    static const String ATTR_DIRECTION;
    static const String ATTR_BLENDING;
    static const String ATTR_INVERT;
    static const String ATTR_MASK;
    static const String ATTR_BEFORE_MEDIA_ITEM_ID;
    static const String ATTR_AFTER_MEDIA_ITEM_ID;
    static const String ATTR_COLOR_EFFECT_TYPE;
    static const String ATTR_COLOR_EFFECT_VALUE;
    static const String ATTR_START_RECT_LEFT;
    static const String ATTR_START_RECT_TOP;
    static const String ATTR_START_RECT_RIGHT;
    static const String ATTR_START_RECT_BOTTOM;
    static const String ATTR_END_RECT_LEFT;
    static const String ATTR_END_RECT_TOP;
    static const String ATTR_END_RECT_RIGHT;
    static const String ATTR_END_RECT_BOTTOM;
    static const String ATTR_LOOP;
    static const String ATTR_MUTED ;
    static const String ATTR_DUCK_ENABLED;
    static const String ATTR_DUCK_THRESHOLD;
    static const String ATTR_DUCKED_TRACK_VOLUME ;
    static const String ATTR_GENERATED_IMAGE_CLIP ;
    static const String ATTR_IS_IMAGE_CLIP_GENERATED;
    static const String ATTR_GENERATED_TRANSITION_CLIP;
    static const String ATTR_IS_TRANSITION_GENERATED;
    static const String ATTR_OVERLAY_RGB_FILENAME;
    static const String ATTR_OVERLAY_FRAME_WIDTH;
    static const String ATTR_OVERLAY_FRAME_HEIGHT;
    static const String ATTR_OVERLAY_RESIZED_RGB_FRAME_WIDTH ;
    static const String ATTR_OVERLAY_RESIZED_RGB_FRAME_HEIGHT ;
    static const Int32 ENGINE_ACCESS_MAX_TIMEOUT_MS;
    /*
     *  Instance variables
     */
    AutoPtr<ISemaphore> mLock;
    String mProjectPath;
    List<AutoPtr<IMediaItem> > mMediaItems;
    List<AutoPtr<IAudioTrack2> > mAudioTracks;
    List<AutoPtr<ITransition> > mTransitions;
    Int64 mDurationMs;
    Int32 mAspectRatio;

    /*
     * Private Object for calling native Methods via MediaArtistNativeHelper
     */
    AutoPtr<MediaArtistNativeHelper> mMANativeHelper;
    Boolean mPreviewInProgress;
    Boolean mMallocDebug;

};

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
#endif // __CVIDEOEDITORIMPL_H__

