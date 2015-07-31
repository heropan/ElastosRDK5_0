
#ifndef __CONTENTVIDEOVIEW_H__
#define __CONTENTVIDEOVIEW_H__

// import android.app.Activity;
// import android.app.AlertDialog;
// import android.content.Context;
// import android.content.ContextWrapper;
// import android.content.DialogInterface;
// import android.util.Log;
// import android.view.Gravity;
// import android.view.KeyEvent;
// import android.view.Surface;
// import android.view.SurfaceHolder;
// import android.view.SurfaceView;
// import android.view.View;
// import android.view.ViewGroup;
// import android.widget.FrameLayout;
// import android.widget.LinearLayout;
// import android.widget.ProgressBar;
// import android.widget.TextView;

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;
// import org.chromium.base.ThreadUtils;
// import org.chromium.ui.base.ViewAndroid;
// import org.chromium.ui.base.ViewAndroidDelegate;
// import org.chromium.ui.base.WindowAndroid;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

/**
 * This class implements accelerated fullscreen video playback using surface view.
 */
//@JNINamespace("content")
class ContentVideoView
    : public FrameLayout
    , public ISurfaceHolderCallback
    , public ViewAndroidDelegate
{
private:
    class VideoSurfaceView
        : public ISurfaceView
    {
    public:
        VideoSurfaceView(
            /* [in] */ ContentVideoView* owner,
            /* [in] */ IContext* context);

    protected:
        //@Override
        CARAPI OnMeasure(
            /* [in] */ Int32 widthMeasureSpec,
            /* [in] */ Int32 heightMeasureSpec);

    private:
        ContentVideoView* mOwner;
    };

    class ProgressView
        : public ILinearLayout
    {
    public:
        ProgressView(
            /* [in] */ IContext* context,
            /* [in] */ String videoLoadingText);

    private:
        const AutoPtr<IProgressBar> mProgressBar;
        const AutoPtr<ITextView> mTextView;
    };

    class InnerRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerRunnable(
            /* [in] */ ContentVideoView* owner);

        CARAPI Run();

    private:
        ContentVideoView* mOwner;
    };

public:
    //@CalledByNative
    CARAPI_(void) OnMediaPlayerError(
        /* [in] */ Int32 errorType);

    //@Override
    CARAPI SurfaceChanged(
        /* [in] */ ISurfaceHolder* holder,
        /* [in] */ Int32 format,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    //@Override
    CARAPI SurfaceCreated(
        /* [in] */ ISurfaceHolder* holder);

    //@Override
    CARAPI SurfaceDestroyed(
        /* [in] */ ISurfaceHolder* holder);

    CARAPI_(Boolean) IsPlaying();

    CARAPI_(void) RemoveSurfaceView();

    CARAPI_(void) ExitFullscreen(
        /* [in] */ Boolean relaseMediaPlayer);

    static CARAPI_(AutoPtr<ContentVideoView>) GetContentVideoView();

    //@Override
    CARAPI OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    //@Override
    CARAPI AcquireAnchorView(
        /* [out] */ IView** view);

    //@Override
    CARAPI SetAnchorViewPosition(
        /* [in] */ IView* view,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Float width,
        /* [in] */ Float height);

    //@Override
    CARAPI ReleaseAnchorView(
        /* [in] */ IView* anchorView);

protected:
    ContentVideoView(
        /* [in] */ IContext* context,
        /* [in] */ Int64 nativeContentVideoView,
        /* [in] */ ContentVideoViewClient* client);

    CARAPI_(AutoPtr<ContentVideoViewClient>) GetContentVideoViewClient();

    CARAPI_(void) ShowContentVideoView();

    CARAPI_(AutoPtr<ISurfaceView>) GetSurfaceView();

    //@CalledByNative
    CARAPI_(void) OnBufferingUpdate(
        /* [in] */ Int32 percent);

    //@CalledByNative
    CARAPI_(void) OnUpdateMediaMetadata(
        /* [in] */ Int32 videoWidth,
        /* [in] */ Int32 videoHeight,
        /* [in] */ Int32 duration,
        /* [in] */ Boolean canPause,
        /* [in] */ Boolean canSeekBack,
        /* [in] */ Boolean canSeekForward);


    //@CalledByNative
    CARAPI_(void) OpenVideo();

    CARAPI_(void) OnCompletion();

    CARAPI_(Boolean) IsInPlaybackState();

    CARAPI_(void) Start();

    CARAPI_(void) Pause();

    // cache duration as mDuration for faster access
    CARAPI_(Int32) GetDuration();

    CARAPI_(Int32) GetCurrentPosition();

    CARAPI_(void) SeekTo(
        /* [in] */ Int32 msec);

    /**
     * This method shall only be called by native and exitFullscreen,
     * To exit fullscreen, use exitFullscreen in Java.
     */
    //@CalledByNative
    CARAPI_(void) DestroyContentVideoView(
        /* [in] */ Boolean nativeViewDestroyed);

private:
    CARAPI_(void) InitResources(
        /* [in] */ IContext* context);

    //@CalledByNative
    CARAPI_(void) OnVideoSizeChanged(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    //@CalledByNative
    CARAPI_(void) OnPlaybackComplete();

    //@CalledByNative
    static CARAPI_(AutoPtr<ContentVideoView>) CreateContentVideoView(
        /* [in] */ IContext* context,
        /* [in] */ Int64 nativeContentVideoView,
        /* [in] */ ContentVideoViewClient* client,
        /* [in] */ Boolean legacy);

    static CARAPI_(Boolean) IsActivityContext(
        /* [in] */ IContext* context);

    //@CalledByNative
    CARAPI_(void) OnExitFullscreen();

    //@CalledByNative
    CARAPI_(Int64) GetNativeViewAndroid();

    static CARAPI_(AutoPtr<ContentVideoView>) NativeGetSingletonJavaContentVideoView();

    CARAPI_(void) NativeExitFullscreen(
        /* [in] */ Int64 nativeContentVideoView,
        /* [in] */ Boolean relaseMediaPlayer);

    CARAPI_(Int32) NativeGetCurrentPosition(
        /* [in] */ Int64 nativeContentVideoView);

    CARAPI_(Int32) NativeGetDurationInMilliSeconds(
        /* [in] */ Int64 nativeContentVideoView);

    CARAPI_(void) NativeRequestMediaMetadata(
        /* [in] */ Int64 nativeContentVideoView);

    CARAPI_(Int32) NativeGetVideoWidth(
        /* [in] */ Int64 nativeContentVideoView);

    CARAPI_(Int32) NativeGetVideoHeight(
        /* [in] */ Int64 nativeContentVideoView);

    CARAPI_(Boolean) NativeIsPlaying(
        /* [in] */ Int64 nativeContentVideoView);

    CARAPI_(void) NativePause(
        /* [in] */ Int64 nativeContentVideoView);

    CARAPI_(void) NativePlay(
        /* [in] */ Int64 nativeContentVideoView);

    CARAPI_(void) NativeSeekTo(
        /* [in] */ Int64 nativeContentVideoView,
        /* [in] */ Int32 msec);

    CARAPI_(void) NativeSetSurface(
        /* [in] */ Int64 nativeContentVideoView,
        /* [in] */ ISurface* surface);

private:
    static const String TAG;

    /* Do not change these values without updating their counterparts
     * in include/media/mediaplayer.h!
     */
    static const Int32 MEDIA_NOP = 0; // interface test message
    static const Int32 MEDIA_PREPARED = 1;
    static const Int32 MEDIA_PLAYBACK_COMPLETE = 2;
    static const Int32 MEDIA_BUFFERING_UPDATE = 3;
    static const Int32 MEDIA_SEEK_COMPLETE = 4;
    static const Int32 MEDIA_SET_VIDEO_SIZE = 5;
    static const Int32 MEDIA_ERROR = 100;
    static const Int32 MEDIA_INFO = 200;

    /**
     * Keep these error codes in sync with the code we defined in
     * MediaPlayerListener.java.
     */
    static const Int32 MEDIA_ERROR_NOT_VALID_FOR_PROGRESSIVE_PLAYBACK = 2;
    static const Int32 MEDIA_ERROR_INVALID_CODE = 3;

    // all possible internal states
    static const Int32 STATE_ERROR              = -1;
    static const Int32 STATE_IDLE               = 0;
    static const Int32 STATE_PLAYING            = 1;
    static const Int32 STATE_PAUSED             = 2;
    static const Int32 STATE_PLAYBACK_COMPLETED = 3;

    AutoPtr<ISurfaceHolder> mSurfaceHolder;
    Int32 mVideoWidth;
    Int32 mVideoHeight;
    Int32 mDuration;

    // Native pointer to C++ ContentVideoView object.
    Int64 mNativeContentVideoView;

    // webkit should have prepared the media
    Int32 mCurrentState;

    // Strings for displaying media player errors
    String mPlaybackErrorText;
    String mUnknownErrorText;
    String mErrorButton;
    String mErrorTitle;
    String mVideoLoadingText;

    // This view will contain the video.
    AutoPtr<VideoSurfaceView> mVideoSurfaceView;

    // Progress view when the video is loading.
    AutoPtr<IView> mProgressView;

    // The ViewAndroid is used to keep screen on during video playback.
    AutoPtr<ViewAndroid> mViewAndroid;

    const AutoPtr<ContentVideoViewClient> mClient;

    const AutoPtr<IRunnable> mExitFullscreenRunnable;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__CONTENTVIDEOVIEW_H__
