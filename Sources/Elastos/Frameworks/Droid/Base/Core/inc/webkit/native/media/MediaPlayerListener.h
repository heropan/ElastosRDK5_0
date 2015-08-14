
#ifndef __ELASTOS_DROID_WEBKIT_MEDIA_MEDIAPLAYERLISTENER_H__
#define __ELASTOS_DROID_WEBKIT_MEDIA_MEDIAPLAYERLISTENER_H__

// import android.content.Context;
// import android.media.AudioManager;
// import android.media.MediaPlayer;

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Media {

// This class implements all the listener interface for android mediaplayer.
// Callbacks will be sent to the native class for processing.
//@JNINamespace("media")
class MediaPlayerListener
    : public Object
    , public IMediaPlayerOnPreparedListener
    , public IMediaPlayerOnCompletionListener
    , public IMediaPlayerOnBufferingUpdateListener
    , public IMediaPlayerOnSeekCompleteListener
    , public IMediaPlayerOnVideoSizeChangedListener
    , public IMediaPlayerOnErrorListener
    , public IAudioManagerOnAudioFocusChangeListener
{
public:
    //@Override
    CARAPI_(Boolean) OnError(
        /* [in] */ IMediaPlayer* mp,
        /* [in] */ Int32 what,
        /* [in] */ Int32 extra);

    //@Override
    CARAPI_(void) OnVideoSizeChanged(
        /* [in] */ IMediaPlayer* mp,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    //@Override
    CARAPI_(void) OnSeekComplete(
        /* [in] */ IMediaPlayer* mp);

    //@Override
    CARAPI_(void) OnBufferingUpdate(
        /* [in] */ IMediaPlayer* mp,
        /* [in] */ Int32 percent);

    //@Override
    CARAPI_(void) OnCompletion(
        /* [in] */ IMediaPlayer* mp);

    //@Override
    CARAPI_(void) OnPrepared(
        /* [in] */ IMediaPlayer* mp);

    //@Override
    CARAPI_(void) OnAudioFocusChange(
        /* [in] */ Int32 focusChange);

    //@CalledByNative
    CARAPI_(void) ReleaseResources();

private:
    MediaPlayerListener(
        /* [in] */ Int64 nativeMediaPlayerListener,
        /* [in] */ IContext* context);

    //@CalledByNative
    static CARAPI_(AutoPtr<IMediaPlayerListener> Create(
        /* [in] */ Int64 nativeMediaPlayerListener,
        /* [in] */ IContext* context,
        /* [in] */ MediaPlayerBridge* mediaPlayerBridge);

    /**
     * See media/base/android/media_player_listener.cc for all the following functions.
     */
    CARAPI_(void) NativeOnMediaError(
        /* [in] */ Int64 nativeMediaPlayerListener,
        /* [in] */ Int32 errorType);

    CARAPI_(void) NativeOnVideoSizeChanged(
        /* [in] */ Int64 nativeMediaPlayerListener,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI_(void) NativeOnBufferingUpdate(
        /* [in] */ Int64 nativeMediaPlayerListener,
        /* [in] */ Int32 percent);

    CARAPI_(void) NativeOnMediaPrepared(
        /* [in] */ Int64 nativeMediaPlayerListener);

    CARAPI_(void) NativeOnPlaybackComplete(
        /* [in] */ Int64 nativeMediaPlayerListener);

    CARAPI_(void) NativeOnSeekComplete(
        /* [in] */ Int64 nativeMediaPlayerListener);

    CARAPI_(void) NativeOnMediaInterrupted(
        /* [in] */ Int64 nativeMediaPlayerListener);

public:
    // These values are copied from android media player.
    static const Int32 MEDIA_ERROR_MALFORMED = -1007;
    static const Int32 MEDIA_ERROR_TIMED_OUT = -110;

private:
    // These values are mirrored as enums in media/base/android/media_player_bridge.h.
    // Please ensure they stay in sync.
    static const Int32 MEDIA_ERROR_FORMAT = 0;
    static const Int32 MEDIA_ERROR_DECODE = 1;
    static const Int32 MEDIA_ERROR_NOT_VALID_FOR_PROGRESSIVE_PLAYBACK = 2;
    static const Int32 MEDIA_ERROR_INVALID_CODE = 3;

    // Used to determine the class instance to dispatch the native call to.
    Int64 mNativeMediaPlayerListener;
    const AutoPtr<IContext> mContext;
};

} // namespace Media
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_MEDIA_MEDIAPLAYERLISTENER_H__
