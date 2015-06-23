
#ifndef __HTML5VIDEOFULLSCREEN_H__
#define __HTML5VIDEOFULLSCREEN_H__

#include "ext/frameworkext.h"
#include "webkit/HTML5VideoView.h"

#include "widget/MediaController.h"
#include "view/SurfaceView.h"

#include "view/View.h"


using Elastos::Droid::Content::IContext;
using Elastos::Droid::Widget::IFrameLayout;
using Elastos::Droid::Widget::IMediaPlayerControl;
using Elastos::Droid::Widget::IMediaController;
using Elastos::Droid::Widget::MediaController;
using Elastos::Droid::Media::IMediaPlayerOnPreparedListener;
using Elastos::Droid::Media::IMediaPlayerOnBufferingUpdateListener;
using Elastos::Droid::Media::IMediaPlayerOnVideoSizeChangedListener;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::ISurfaceHolder;
using Elastos::Droid::View::ISurfaceView;
using Elastos::Droid::View::SurfaceView;
using Elastos::Droid::View::View;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewOnTouchListener;
using Elastos::Droid::View::ISurfaceHolderCallback;

namespace Elastos {
namespace Droid {
namespace Webkit {

class CWebViewClassic;
class HTML5VideoViewProxy;

/**
 * @hide This is only used by the browser
 */
class HTML5VideoFullScreen
    : public HTML5VideoView
    , public IMediaPlayerControl
    , public IViewOnTouchListener
{
public:
    class FullScreenMediaController
        : public ElRefBase
        , public MediaController
        , public IMediaController
    {
    public:
        FullScreenMediaController(
            /* [in] */ IContext* context,
            /* [in] */ IView* video);

        CAR_INTERFACE_DECL();

        IVIEW_METHODS_DECL();

        IVIEWGROUP_METHODS_DECL();

        IFRAMELAYOUT_METHODS_DECL();

        CARAPI SetMediaPlayer(
            /* [in] */ IMediaPlayerControl* player);

        CARAPI SetAnchorView(
            /* [in] */ IView* view);

        CARAPI Show();

        CARAPI Show(
            /* [in] */ Int32 timeout);

        CARAPI IsShowing(
            /* [out] */ Boolean* isShowing);

        CARAPI Hide();

        CARAPI SetPrevNextListeners(
            /* [in] */ IViewOnClickListener* next,
            /* [in] */ IViewOnClickListener* prev);

    public:
        AutoPtr<IView> mVideoView;
    };

private:
    // Add this sub-class to handle the resizing when rotating screen.
    class VideoSurfaceView
        : public ElRefBase
        , public SurfaceView
        , public ISurfaceView
    {
    public:
        VideoSurfaceView(
            /* [in] */ HTML5VideoFullScreen* owner,
            /* [in] */ IContext* context);

        CAR_INTERFACE_DECL();

        IVIEW_METHODS_DECL();

        ISURFACEVIEW_METHODS_DECL();

    protected:
        CARAPI_(void) OnMeasure(
            /* [in] */ Int32 widthMeasureSpec,
            /* [in] */ Int32 heightMeasureSpec);

    private:
        HTML5VideoFullScreen* mOwner;
    };

    class InnerSurfaceHolderCallback
        : public ElLightRefBase
        , public ISurfaceHolderCallback
    {
    public:
        InnerSurfaceHolderCallback(
            /* [in] */ HTML5VideoFullScreen* owner);

        CAR_INTERFACE_DECL();

        CARAPI SurfaceCreated(
            /* [in] */ ISurfaceHolder* holder);

        CARAPI SurfaceChanged(
            /* [in] */ ISurfaceHolder* holder,
            /* [in] */ Int32 format,
            /* [in] */ Int32 width,
            /* [in] */ Int32 height);

        CARAPI SurfaceDestroyed(
            /* [in] */ ISurfaceHolder* holder);

    private:
        HTML5VideoFullScreen* mOwner;
    };

    class InnerMediaPlayerOnVideoSizeChangedListener
        : public ElLightRefBase
        , public IMediaPlayerOnVideoSizeChangedListener
    {
    public:
        InnerMediaPlayerOnVideoSizeChangedListener(
            /* [in] */ HTML5VideoFullScreen* owner);

        CAR_INTERFACE_DECL();

        CARAPI OnVideoSizeChanged(
            /* [in] */ IMediaPlayer* mp,
            /* [in] */ Int32 width,
            /* [in] */ Int32 height);

    private:
        HTML5VideoFullScreen* mOwner;
    };

    class InnerMediaPlayerOnBufferingUpdateListener
        : public ElLightRefBase
        , public IMediaPlayerOnBufferingUpdateListener
    {
    public:
        InnerMediaPlayerOnBufferingUpdateListener(
            /* [in] */ HTML5VideoFullScreen* owner);

        CAR_INTERFACE_DECL();

        CARAPI OnBufferingUpdate(
            /* [in] */ IMediaPlayer* mp,
            /* [in] */ Int32 percent);

    private:
        HTML5VideoFullScreen* mOwner;
    };

    class InnerWebChromeClientCustomViewCallback
        : public ElLightRefBase
        , public IWebChromeClientCustomViewCallback
    {
    public:
        InnerWebChromeClientCustomViewCallback(
            /* [in] */ HTML5VideoFullScreen* owner);

        CAR_INTERFACE_DECL();

        CARAPI OnCustomViewHidden();

    private:
        HTML5VideoFullScreen* mOwner;
    };

public:
    // We need the full screen state to decide which surface to render to and
    // when to create the MediaPlayer accordingly.
    static const Int32 FULLSCREEN_OFF               = 0;
    static const Int32 FULLSCREEN_SURFACECREATING   = 1;
    static const Int32 FULLSCREEN_SURFACECREATED    = 2;

    AutoPtr<ISurfaceHolderCallback> mSHCallback;
    AutoPtr<IMediaPlayerOnVideoSizeChangedListener> mSizeChangedListener;

public:
    HTML5VideoFullScreen(
        /* [in] */ IContext* context,
        /* [in] */ Int32 videoLayerId,
        /* [in] */ Int32 position,
        /* [in] */ Boolean skipPrepare);

    CAR_INTERFACE_DECL();

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

    CARAPI_(void) DecideDisplayMode();

    CARAPI OnPrepared(
        /* [in] */ IMediaPlayer* mp);

    CARAPI FullScreenExited(
        /* [out] */ Boolean* result);

    CARAPI_(void) EnterFullScreenVideoState(
        /* [in] */ Int32 layerId,
        /* [in] */ HTML5VideoViewProxy* proxy,
        /* [in] */ CWebViewClassic* webView);

    /**
     * @return true when we are in full screen mode, even the surface not fully
     * created.
     */
    CARAPI IsFullScreenMode(
        /* [out] */ Boolean* result);

    // MediaController FUNCTIONS:
    CARAPI CanPause(
        /* [out] */ Boolean* result);

    CARAPI CanSeekBackward(
        /* [out] */ Boolean* result);

    CARAPI CanSeekForward(
        /* [out] */ Boolean* result);

    CARAPI GetBufferPercentage(
        /* [out] */ Int32* result);

    CARAPI_(void) ShowControllerInFullScreen();

    CARAPI OnTouch(
        /* [in] */ IView* v,
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* result);

protected:
    CARAPI_(void) SwitchProgressView(
        /* [in] */ Boolean playerBuffering);

private:
    CARAPI_(AutoPtr<ISurfaceView>) GetSurfaceView();

    CARAPI_(void) SetMediaController(
        /* [in] */ IMediaController* m);

    CARAPI_(void) AttachMediaController();

    CARAPI_(void) PrepareForFullScreen();

    CARAPI_(void) ToggleMediaControlsVisiblity();

private:
    // This view will contain the video.
    AutoPtr<VideoSurfaceView> mVideoSurfaceView;

    Int32 mFullScreenMode;
    // The Media Controller only used for full screen mode
    AutoPtr<IMediaController> mMediaController;

    // SurfaceHolder for full screen
    AutoPtr<ISurfaceHolder> mSurfaceHolder;

    // Data only for MediaController
    Boolean mCanSeekBack;
    Boolean mCanSeekForward;
    Boolean mCanPause;
    Int32 mCurrentBufferPercentage;

    // The progress view.
    static AutoPtr<IView> mProgressView;
    // The container for the progress view and video view
    static AutoPtr<IFrameLayout> mLayout;

    // The video size will be ready when prepared. Used to make sure the aspect
    // ratio is correct.
    Int32 mVideoWidth;
    Int32 mVideoHeight;
    Boolean mPlayingWhenDestroyed;

    // Other listeners functions:
    AutoPtr<IMediaPlayerOnBufferingUpdateListener> mBufferingUpdateListener;
    AutoPtr<IWebChromeClientCustomViewCallback> mCallback;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__HTML5VIDEOFULLSCREEN_H__
