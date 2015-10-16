
#ifndef __ELASTOS_DROID_WIDGET_VideoView_H__
#define __ELASTOS_DROID_WIDGET_VideoView_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/view/SurfaceView.h"
#include "elastos/droid/media/CMediaPlayer.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IDialogInterface;
using Elastos::Droid::Content::IDialogInterfaceOnClickListener;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Media::IMediaPlayer;
using Elastos::Droid::Media::IMediaPlayerOnPreparedListener;
using Elastos::Droid::Media::IMediaPlayerOnCompletionListener;
using Elastos::Droid::Media::IMediaPlayerOnErrorListener;
using Elastos::Droid::Media::IMediaPlayerOnInfoListener;
using Elastos::Droid::Media::IMediaPlayerOnVideoSizeChangedListener;
using Elastos::Droid::Media::IMediaPlayerOnBufferingUpdateListener;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::SurfaceView;
using Elastos::Droid::View::ISurfaceHolderCallback;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::View::Accessibility::IAccessibilityNodeInfo;


namespace Elastos {
namespace Droid {
namespace Widget {

class VideoView : public SurfaceView
{
private:
    class VVOnVideoSizeChangedListener
        : public ElRefBase
        , public IMediaPlayerOnVideoSizeChangedListener
    {
    public:
        CAR_INTERFACE_DECL()

        VVOnVideoSizeChangedListener(
            /* [in] */ VideoView* host)
            : mHost(host)
        {}

        CARAPI OnVideoSizeChanged(
            /* [in] */ IMediaPlayer* mp,
            /* [in] */ Int32 width,
            /* [in] */ Int32 height);

    private:
        VideoView* mHost;
    };

    class VVOnPreparedListener
      : public ElRefBase
      , public IMediaPlayerOnPreparedListener
    {
    public:
        CAR_INTERFACE_DECL()

        VVOnPreparedListener(
            /* [in] */ VideoView* host)
            : mHost(host)
        {}

        CARAPI OnPrepared(
            /* [in] */ IMediaPlayer* mp);

    private:
        VideoView* mHost;
    };

    class VVOnCompletionListener
        : public ElRefBase
        , public IMediaPlayerOnCompletionListener
    {
    public:
        CAR_INTERFACE_DECL()

        VVOnCompletionListener(
            /* [in] */ VideoView* host)
            : mHost(host)
        {}

        CARAPI OnCompletion(
            /* [in] */ IMediaPlayer* mp);

    private:
        VideoView* mHost;
    };

    class VVOnErrorListener
        : public ElRefBase
        , public IMediaPlayerOnErrorListener
    {
    public:
        CAR_INTERFACE_DECL()

        VVOnErrorListener(
            /* [in] */ VideoView* host)
            : mHost(host)
        {}

        CARAPI OnError(
            /* [in] */ IMediaPlayer* mp,
            /* [in] */ Int32 what,
            /* [in] */ Int32 extra,
            /* [out] */ Boolean* res);

    private:
        VideoView* mHost;
    };

    class VVDialogOnClickListener
        : public ElRefBase
        , public IDialogInterfaceOnClickListener
    {
    public:
        CAR_INTERFACE_DECL()

        VVDialogOnClickListener(
            /* [in] */ VideoView* host)
            : mHost(host)
        {}

        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which);

    private:
        VideoView* mHost;
    };

    class VVOnBufferingUpdateListener
        : public ElRefBase
        , public IMediaPlayerOnBufferingUpdateListener
    {
    public:
        CAR_INTERFACE_DECL()

        VVOnBufferingUpdateListener(
            /* [in] */ VideoView* host)
            : mHost(host)
        {}

        CARAPI OnBufferingUpdate(
            /* [in] */ IMediaPlayer* mp,
            /* [in] */ Int32 percent);

    private:
        VideoView* mHost;
    };

    class VVSurfaceHodlerCallback
        : public ElRefBase
        , public ISurfaceHolderCallback
    {
    public:
        CAR_INTERFACE_DECL()

        VVSurfaceHodlerCallback(
            /* [in] */ VideoView* host)
            : mHost(host)
        {}

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
        VideoView* mHost;
    };

public:
    VideoView();

    virtual ~VideoView();

    virtual CARAPI_(Int32) ResolveAdjustedSize(
        /* [in] */ Int32 desiredSize,
        /* [in] */ Int32 measureSpec);

    virtual CARAPI SetVideoPath(
        /* [in] */ const String& path);

    virtual CARAPI SetVideoURI(
        /* [in] */ IUri* uri);

    /**
     * @hide
     */
    virtual CARAPI SetVideoURI(
        /* [in] */ IUri* uri,
        /* [in] */ HashMap<String, String>* headers);

    virtual CARAPI StopPlayback();

    virtual CARAPI SetMediaController(
        /* [in] */ IMediaController* controller);

    virtual CARAPI SetOnPreparedListener(
        /* [in] */ IMediaPlayerOnPreparedListener* l);

    virtual CARAPI SetOnCompletionListener(
        /* [in] */ IMediaPlayerOnCompletionListener* l);

    virtual CARAPI SetOnErrorListener(
        /* [in] */ IMediaPlayerOnErrorListener* l);

    /**
     * Register a callback to be invoked when an informational event
     * occurs during playback or setup.
     *
     * @param l The callback that will be run
     */
    virtual CARAPI SetOnInfoListener(
        /* [in] */ IMediaPlayerOnInfoListener* l);

    //@Override
    CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

    //@Override
    CARAPI_(Boolean) OnTrackballEvent(
        /* [in] */ IMotionEvent* event);

    //@Override
    CARAPI_(Boolean) OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI Start();

    virtual CARAPI Pause();

    virtual CARAPI Suspend();

    virtual CARAPI Resume();

    virtual CARAPI_(Int32) GetDuration();

    virtual CARAPI_(Int32) GetCurrentPosition();

    virtual CARAPI SeekTo(
        /* [in] */ Int32 msec);

    virtual CARAPI_(Boolean) IsPlaying();

    virtual CARAPI_(Int32) GetBufferPercentage();

    virtual CARAPI_(Boolean) CanPause();

    virtual CARAPI_(Boolean) CanSeekBackward();

    virtual CARAPI_(Boolean) CanSeekForward();

    virtual CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);


protected:
    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle = 0);

    //@Override
    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

private:
    CARAPI_(void) InitVideoView();

    CARAPI_(void) OpenVideo();

    CARAPI_(void) AttachMediaController();

    CARAPI_(void) Release(
        /* [in] */ Boolean cleartargetstate);

    CARAPI_(void) ToggleMediaControlsVisiblity();

    CARAPI_(Boolean) IsInPlaybackState();

private:
    static const String TAG;
    // settable by the client
    AutoPtr<IUri> mUri;
    HashMap<String, String> mHeaders;

    // all possible internal states
    static const Int32 STATE_ERROR              = -1;
    static const Int32 STATE_IDLE               = 0;
    static const Int32 STATE_PREPARING          = 1;
    static const Int32 STATE_PREPARED           = 2;
    static const Int32 STATE_PLAYING            = 3;
    static const Int32 STATE_PAUSED             = 4;
    static const Int32 STATE_PLAYBACK_COMPLETED = 5;


    // mCurrentState is a VideoView object's current state.
    // mTargetState is the state that a method caller intends to reach.
    // For instance, regardless the VideoView object's current state,
    // calling pause() intends to bring the object to a target state
    // of STATE_PAUSED.
    Int32 mCurrentState;
    Int32 mTargetState;

    // All the stuff we need for playing and showing a video
    AutoPtr<ISurfaceHolder> mSurfaceHolder;
    AutoPtr<IMediaPlayer> mMediaPlayer;
    Int32 mVideoWidth;
    Int32 mVideoHeight;
    Int32 mSurfaceWidth;
    Int32 mSurfaceHeight;
    AutoPtr<IMediaController> mMediaController;
    AutoPtr<IMediaPlayerOnCompletionListener> mOnCompletionListener;
    AutoPtr<IMediaPlayerOnPreparedListener> mOnPreparedListener;
    Int32 mCurrentBufferPercentage;
    AutoPtr<IMediaPlayerOnErrorListener> mOnErrorListener;
    AutoPtr<IMediaPlayerOnInfoListener>  mOnInfoListener;
    Int32 mSeekWhenPrepared; // recording the seek position while preparing
    Boolean mCanPause;
    Boolean mCanSeekBack;
    Boolean mCanSeekForward;

    AutoPtr<IMediaPlayerOnVideoSizeChangedListener> mSizeChangedListener;
    AutoPtr<IMediaPlayerOnPreparedListener> mPreparedListener;
    AutoPtr<IMediaPlayerOnCompletionListener> mCompletionListener;
    AutoPtr<IMediaPlayerOnErrorListener> mErrorListener;
    AutoPtr<IMediaPlayerOnBufferingUpdateListener> mBufferingUpdateListener;
    AutoPtr<ISurfaceHolderCallback> mSHCallback;
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_VideoView_H__
