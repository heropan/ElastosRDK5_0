
#ifndef __CONTENTVIDEOVIEWLEGACY_H__
#define __CONTENTVIDEOVIEWLEGACY_H__

// import android.content.Context;
// import android.graphics.Color;
// import android.view.KeyEvent;
// import android.view.MotionEvent;
// import android.view.SurfaceHolder;
// import android.view.SurfaceView;
// import android.view.View;
// import android.widget.MediaController;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

/**
 * Adds Android media controller to ContentVideoView.
 * The sole purpose of this class is to maintain legacy behavior while we test
 * the blink-based media controller.
 * https://code.google.com/p/chromium/issues/detail?id=331966
 */
class ContentVideoViewLegacy
    : public ContentVideoView
{
public:
    /**
     * A listener for changes in the MediaController visibility.
     */
    class MediaControlsVisibilityListener
    {
    public:
        /**
         * Callback for when the visibility of the media controls changes.
         *
         * @param shown true if the media controls are shown to the user, false otherwise
         */
        virtual CARAPI_(void) OnMediaControlsVisibilityChanged(
            /* [in] */ Boolean shown) = 0;
    };

private:
    class FullScreenMediaController
        : public Object
        , public IMediaController
    {
    public:
        /**
         * @param context The context.
         * @param video The full screen video container view.
         * @param listener A listener that listens to the visibility of media controllers.
         */
        FullScreenMediaController(
            /* [in] */ IContext* context,
            /* [in] */ IView* video,
            /* [in] */ MediaControlsVisibilityListener* listener);

        //@Override
        CARAPI Show();

        //@Override
        CARAPI Hide();

    private:
        const AutoPtr<IView> mVideoView;
        const AutoPtr<MediaControlsVisibilityListener> mListener;
    };

    class InnerViewOnKeyListener
        : public Object
        , public IViewOnKeyListener
    {
    public:
        InnerViewOnKeyListener(
            /* [in] */ ContentVideoViewLegacy* owner);

        CARAPI OnKey(
            /* [in] */ IView* v,
            /* [in] */ Int32 keyCode,
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* result);

    private:
        ContentVideoViewLegacy* mOwner;
    };

    class InnerViewOnTouchListener
        : public Object
        , public IViewOnTouchListener
    {
    public:
        InnerViewOnTouchListener(
            /* [in] */ ContentVideoViewLegacy* owner);

        CARAPI OnTouch(
            /* [in] */ IView* v,
            /* [in] */ IMotionEvent* event,
            /* [out] */ Boolean* result);

    private:
        ContentVideoViewLegacy* mOwner;
    };

    class InnerMediaPlayerControl
        : public Object
        , public IMediaPlayerControl
    {
    public:
        InnerMediaPlayerControl(
            /* [in] */ ContentVideoViewLegacy* owner);

        CARAPI Start();

        CARAPI Pause();

        CARAPI GetDuration(
            /* [out] */ Int32* duration);

        CARAPI GetCurrentPosition(
            /* [out] */ Int32* position);

        CARAPI SeekTo(
            /* [in] */ Int32 position);

        CARAPI IsPlaying(
            /* [out] */ Boolean* isPlaying);

        CARAPI GetBufferPercentage(
            /* [out] */ Int32* percentage);

        CARAPI CanPause(
            /* [out] */ Boolean* canPause);

        CARAPI CanSeekBackward(
            /* [out] */ Boolean* canSeekBackward);

        CARAPI CanSeekForward(
            /* [out] */ Boolean* canSeekForward);

        CARAPI GetAudioSessionId(
            /* [out] */ Int32* id);

    private:
        ContentVideoViewLegacy* mOwner;
    };

public:
    ContentVideoViewLegacy(
        /* [in] */ IContext* context,
        /* [in] */ Int64 nativeContentVideoView,
        /* [in] */ ContentVideoViewClient* client);

    //@Override
    CARAPI_(void) OnMediaPlayerError(
        /* [in] */ Int32 errorType);

    //@Override
    CARAPI_(void) SurfaceChanged(
        /* [in] */ ISurfaceHolder* holder,
        /* [in] */ Int32 format,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    //@Override
    CARAPI_(Boolean) OnTrackballEvent(
        /* [in] */ IMotionEvent* ev);

    //@Override
    CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* ev);

    /**
     * Sets the MediaControlsVisibilityListener that wants to listen to visibility change events.
     *
     * @param listener the listener to send the events to.
     */
    CARAPI_(void) SetListener(
        /* [in] */ MediaControlsVisibilityListener* listener);

protected:
    //@Override
    CARAPI_(void) ShowContentVideoView();

    //@Override
    CARAPI_(void) OnBufferingUpdate(
        /* [in] */ Int32 percent);

    //@Override
    CARAPI_(void) OnUpdateMediaMetadata(
        /* [in] */ Int32 videoWidth,
        /* [in] */ Int32 videoHeight,
        /* [in] */ Int32 duration,
        /* [in] */ Boolean canPause,
        /* [in] */ Boolean canSeekBack,
        /* [in] */ Boolean canSeekForward);

    //@Override
    CARAPI_(void) OpenVideo();

    //@Override
    CARAPI_(void) OnCompletion();

    //@Override
    CARAPI_(void) DestroyContentVideoView(
        /* [in] */ Boolean nativeViewDestroyed);

private:
    CARAPI_(void) ToggleMediaControlsVisiblity();

private:
    AutoPtr<FullScreenMediaController> mMediaController;
    Boolean mCanPause;
    Boolean mCanSeekBackward;
    Boolean mCanSeekForward;
    Int32 mCurrentBufferPercentage;
    AutoPtr<MediaControlsVisibilityListener> mListener;
};

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__CONTENTVIDEOVIEWLEGACY_H__
