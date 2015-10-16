
#include "elastos/droid/widget/VideoView.h"
#include <elastos/core/Math.h>
#include <Elastos.Droid.Core_server.h>
#include "elastos/droid/net/Uri.h"
#include "elastos/droid/content/CIntent.h"
#include "elastos/droid/app/CAlertDialogBuilder.h"
#include "elastos/droid/R.h"

using Elastos::Utility::Etl::HashMap;
using Elastos::Core::CStringWrapper;
using Elastos::Droid::R;
using Elastos::Droid::Media::IMetadata;
using Elastos::Droid::Media::CMediaPlayer;
using Elastos::Droid::Media::IAudioManager;
using Elastos::Droid::Media::EIID_IMediaPlayerOnPreparedListener;
using Elastos::Droid::Media::EIID_IMediaPlayerOnCompletionListener;
using Elastos::Droid::Media::EIID_IMediaPlayerOnErrorListener;
using Elastos::Droid::Media::EIID_IMediaPlayerOnInfoListener;
using Elastos::Droid::Media::EIID_IMediaPlayerOnVideoSizeChangedListener;
using Elastos::Droid::Media::EIID_IMediaPlayerOnBufferingUpdateListener;
using Elastos::Droid::Net::Uri;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::EIID_IDialogInterfaceOnClickListener;
using Elastos::Droid::App::IAlertDialog;
using Elastos::Droid::App::IAlertDialogBuilder;
using Elastos::Droid::App::CAlertDialogBuilder;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::EIID_ISurfaceHolderCallback;
using Elastos::Droid::View::IViewParent;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::IViewOnLayoutChangeListener;
using Elastos::Droid::View::EIID_IViewOnLayoutChangeListener;

namespace Elastos {
namespace Droid {
namespace Widget {


//==============================================================================
//          VideoView::VVOnVideoSizeChangedListener
//==============================================================================

CAR_INTERFACE_IMPL(VideoView::VVOnVideoSizeChangedListener,
        IMediaPlayerOnVideoSizeChangedListener)

ECode VideoView::VVOnVideoSizeChangedListener::OnVideoSizeChanged(
    /* [in] */ IMediaPlayer* mp,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    mp->GetVideoWidth(&mHost->mVideoWidth);
    mp->GetVideoHeight(&mHost->mVideoHeight);
    if (mHost->mVideoWidth != 0 &&mHost-> mVideoHeight != 0) {
        mHost->GetHolder()->SetFixedSize(
                    mHost->mVideoWidth, mHost->mVideoHeight);

        mHost->RequestLayout();
    }

    return NOERROR;
}

//==============================================================================
//          VideoView::VVOnPreparedListener
//==============================================================================

CAR_INTERFACE_IMPL(VideoView::VVOnPreparedListener,
        IMediaPlayerOnPreparedListener)

ECode VideoView::VVOnPreparedListener::OnPrepared(
    /* [in] */ IMediaPlayer* mp)
{
    mHost->mCurrentState = STATE_PREPARED;

    // Get the capabilities of the player for this stream
    AutoPtr<IMetadata> data;
    mp->GetMetadata(IMediaPlayer::METADATA_ALL,
                    IMediaPlayer::BYPASS_METADATA_FILTER, (IMetadata**)&data);

    if (data != NULL) {
        Boolean hasRes, boolRes;
        mHost->mCanPause = (data->Has(IMetadata::PAUSE_AVAILABLE, &hasRes), !hasRes)
                            || (data->GetBoolean(IMetadata::PAUSE_AVAILABLE, &boolRes), boolRes);
        mHost->mCanSeekBack = (data->Has(IMetadata::SEEK_BACKWARD_AVAILABLE, &hasRes), !hasRes)
                            || (data->GetBoolean(IMetadata::SEEK_BACKWARD_AVAILABLE, &boolRes), boolRes);
        mHost->mCanSeekForward = (data->Has(IMetadata::SEEK_FORWARD_AVAILABLE, &hasRes), !hasRes)
                            || (data->GetBoolean(IMetadata::SEEK_FORWARD_AVAILABLE, &boolRes), boolRes);
    }
    else {
        mHost->mCanPause = mHost->mCanSeekBack = mHost->mCanSeekForward = TRUE;
    }

    if (mHost->mOnPreparedListener != NULL) {
        mHost->mOnPreparedListener->OnPrepared(mHost->mMediaPlayer);
    }
    if (mHost->mMediaController != NULL) {
        mHost->mMediaController->SetEnabled(TRUE);
    }
    mp->GetVideoWidth(&mHost->mVideoWidth);
    mp->GetVideoHeight(&mHost->mVideoHeight);

    Int32 seekToPosition = mHost->mSeekWhenPrepared;  // mSeekWhenPrepared may be changed after seekTo() call
    if (seekToPosition != 0) {
        mHost->SeekTo(seekToPosition);
    }
    if (mHost->mVideoWidth != 0 && mHost->mVideoHeight != 0) {
        //Log.i("@@@@", "video size: " + mVideoWidth +"/"+ mVideoHeight);
        mHost->GetHolder()->SetFixedSize(mHost->mVideoWidth, mHost->mVideoHeight);
        if (mHost->mSurfaceWidth == mHost->mVideoWidth &&
            mHost->mSurfaceHeight == mHost->mVideoHeight) {
            // We didn't actually change the size (it was already at the size
            // we need), so we won't get a "surface changed" callback, so
            // start the video here instead of in the callback.
            if (mHost->mTargetState == STATE_PLAYING) {
                mHost->Start();
                if (mHost->mMediaController != NULL) {
                    mHost->mMediaController->Show();
                }
            }
            else if (!mHost->IsPlaying() &&
                    (seekToPosition != 0 || mHost->GetCurrentPosition() > 0)) {
                if (mHost->mMediaController != NULL) {
                    // Show the media controls when we're paused into a video and make 'em stick.
                    mHost->mMediaController->Show(0);
                }
            }
        }
    }
    else {
        // We don't know the video size yet, but should start anyway.
        // The video size might be reported to us later.
        if (mHost->mTargetState == STATE_PLAYING) {
            mHost->Start();
        }
    }

    return NOERROR;
}

//==============================================================================
//          VideoView::VVOnCompletionListener
//==============================================================================

CAR_INTERFACE_IMPL(VideoView::VVOnCompletionListener,
        IMediaPlayerOnCompletionListener)

ECode VideoView::VVOnCompletionListener::OnCompletion(
    /* [in] */ IMediaPlayer* mp)
{
    mHost->mCurrentState = STATE_PLAYBACK_COMPLETED;
    mHost->mTargetState = STATE_PLAYBACK_COMPLETED;
    if (mHost->mMediaController != NULL) {
        mHost->mMediaController->Hide();
    }
    if (mHost->mOnCompletionListener != NULL) {
        mHost->mOnCompletionListener->OnCompletion(mHost->mMediaPlayer);
    }

    return NOERROR;
}

//==============================================================================
//          VideoView::VVOnErrorListener
//==============================================================================

CAR_INTERFACE_IMPL(VideoView::VVOnErrorListener,
        IMediaPlayerOnErrorListener)

ECode VideoView::VVOnErrorListener::OnError(
    /* [in] */ IMediaPlayer* mp,
    /* [in] */ Int32 what,
    /* [in] */ Int32 extra,
    /* [out] */ Boolean* res)
{
    // Log.d(TAG, "Error: " + framework_err + "," + impl_err);
    mHost->mCurrentState = STATE_ERROR;
    mHost->mTargetState = STATE_ERROR;
    if (mHost->mMediaController != NULL) {
        mHost->mMediaController->Hide();
    }

    /* If an error handler has been supplied, use it and finish. */
    if (mHost->mOnErrorListener != NULL) {
        Boolean result;
        mHost->mOnErrorListener->OnError(mHost->mMediaPlayer, what, extra, &result);
        if (result) {
            *res = TRUE;
            return NOERROR;
        }
    }

    /* Otherwise, pop up an error dialog so the user knows that
     * something bad has happened. Only try and pop up the dialog
     * if we're attached to a window. When we're going away and no
     * longer have a window, don't bother showing the user an error.
     */
    if (mHost->GetWindowToken() != NULL) {
        AutoPtr<IResources> r;
        FAIL_RETURN(mHost->mContext->GetResources((IResources**)&r));
        Int32 messageId;

        if (what == IMediaPlayer::MEDIA_ERROR_NOT_VALID_FOR_PROGRESSIVE_PLAYBACK) {
            messageId = R::string::VideoView_error_text_invalid_progressive_playback;
        }
        else {
            messageId = R::string::VideoView_error_text_unknown;
        }


        AutoPtr<IAlertDialogBuilder> builder;
        FAIL_RETURN(CAlertDialogBuilder::New(
                mHost->mContext, (IAlertDialogBuilder**)&builder));
        //builder->SetTitle(R::string::VideoView_error_title);
        builder->SetMessage(messageId);
        builder->SetCancelable(FALSE);

        AutoPtr<VVDialogOnClickListener> listener = new VVDialogOnClickListener(mHost);
        builder->SetPositiveButton(R::string::VideoView_error_button, listener);

        AutoPtr<IAlertDialog> dialog;
        builder->Show((IAlertDialog**)&dialog);
    }

    *res = TRUE;
    return NOERROR;
}

//==============================================================================
//          VideoView::VVDialogOnClickListener
//==============================================================================

CAR_INTERFACE_IMPL(VideoView::VVDialogOnClickListener,
        IDialogInterfaceOnClickListener)

ECode VideoView::VVDialogOnClickListener::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    /* If we get here, there is no onError listener, so
     * at least inform them that the video is over.
     */
    if (mHost->mOnCompletionListener != NULL) {
        mHost->mOnCompletionListener->OnCompletion(mHost->mMediaPlayer);
    }

    return NOERROR;
}

//==============================================================================
//          VideoView::VVOnBufferingUpdateListener
//==============================================================================

CAR_INTERFACE_IMPL(VideoView::VVOnBufferingUpdateListener,
        IMediaPlayerOnBufferingUpdateListener)

ECode VideoView::VVOnBufferingUpdateListener::OnBufferingUpdate(
    /* [in] */ IMediaPlayer* mp,
    /* [in] */ Int32 percent)
{
    mHost->mCurrentBufferPercentage = percent;

    return NOERROR;
}

//==============================================================================
//          VideoView::VVSurfaceHodlerCallback
//==============================================================================

CAR_INTERFACE_IMPL(VideoView::VVSurfaceHodlerCallback,
        ISurfaceHolderCallback)

ECode VideoView::VVSurfaceHodlerCallback::SurfaceChanged(
    /* [in] */ ISurfaceHolder* holder,
    /* [in] */ Int32 format,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    mHost->mSurfaceWidth = width;
    mHost->mSurfaceHeight = height;
    Boolean isValidState =  (mHost->mTargetState == STATE_PLAYING);
    Boolean hasValidSize = (mHost->mVideoWidth == width &&
                            mHost->mVideoHeight == height);
    if (mHost->mMediaPlayer != NULL && isValidState && hasValidSize) {
        if (mHost->mSeekWhenPrepared != 0) {
            mHost->SeekTo(mHost->mSeekWhenPrepared);
        }

        mHost->Start();
    }

    return NOERROR;
}

ECode VideoView::VVSurfaceHodlerCallback::SurfaceCreated(
    /* [in] */ ISurfaceHolder* holder)
{
    mHost->mSurfaceHolder = holder;
    //resume() was called before surfaceCreated()

    mHost->OpenVideo();

    return NOERROR;
}

ECode VideoView::VVSurfaceHodlerCallback::SurfaceDestroyed(
    /* [in] */ ISurfaceHolder* holder)
{
    // after we return from this we can't use the surface any more
    mHost->mSurfaceHolder = NULL;
    if (mHost->mMediaController != NULL) mHost->mMediaController->Hide();
        mHost->Release(TRUE);

    return NOERROR;
}

//==============================================================================
//          VideoView
//==============================================================================

const String VideoView::TAG("VideoView");
const Int32 VideoView::STATE_ERROR;
const Int32 VideoView::STATE_IDLE;
const Int32 VideoView::STATE_PREPARING;
const Int32 VideoView::STATE_PREPARED;
const Int32 VideoView::STATE_PLAYING;
const Int32 VideoView::STATE_PAUSED;
const Int32 VideoView::STATE_PLAYBACK_COMPLETED;


VideoView::VideoView()
    : mCurrentState(STATE_IDLE)
    , mTargetState(STATE_IDLE)
    , mVideoWidth(0)
    , mVideoHeight(0)
    , mSurfaceWidth(0)
    , mSurfaceHeight(0)
    , mCurrentBufferPercentage(0)
    , mSeekWhenPrepared(0)
    , mCanPause(FALSE)
    , mCanSeekBack(FALSE)
    , mCanSeekForward(FALSE)
{
    mSizeChangedListener = new VVOnVideoSizeChangedListener(this);
    mPreparedListener = new VVOnPreparedListener(this);
    mCompletionListener = new VVOnCompletionListener(this);
    mErrorListener = new VVOnErrorListener(this);
    mBufferingUpdateListener = new VVOnBufferingUpdateListener(this);
    mSHCallback = new VVSurfaceHodlerCallback(this);
}

VideoView::~VideoView()
{
    printf("FILE=[%s]\n FUNC=[%s], line=[%d] ++ \n", __FILE__, __FUNCTION__, __LINE__);
}

ECode VideoView::Init(
    /* [in] */ IContext* context)
{
    FAIL_RETURN(SurfaceView::Init(context));
    InitVideoView();

    return NOERROR;
}

ECode VideoView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    FAIL_RETURN(SurfaceView::Init(context, attrs, defStyle));
    InitVideoView();

    return NOERROR;
}

void VideoView::InitVideoView()
{
    AutoPtr<ISurfaceHolder> holder = GetHolder();
    holder->AddCallback(mSHCallback);
    holder->SetType(ISurfaceHolder::SURFACE_TYPE_PUSH_BUFFERS);
    SetFocusable(TRUE);
    SetFocusableInTouchMode(TRUE);
    RequestFocus();
}

void VideoView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    //Log.i("@@@@", "onMeasure");
    Int32 width = GetDefaultSize(mVideoWidth, widthMeasureSpec);
    Int32 height = GetDefaultSize(mVideoHeight, heightMeasureSpec);

    if (mVideoWidth > 0 && mVideoHeight > 0) {
        if ( mVideoWidth * height  > width * mVideoHeight ) {
            //Log.i("@@@", "image too tall, correcting");
            height = width * mVideoHeight / mVideoWidth;
        }
        else if ( mVideoWidth * height  < width * mVideoHeight ) {
            //Log.i("@@@", "image too wide, correcting");
            width = height * mVideoWidth / mVideoHeight;
        }
        else {
            //Log.i("@@@", "aspect ratio is correct: " +
                    //width+"/"+height+"="+
                    //mVideoWidth+"/"+mVideoHeight);
        }
    }
    //Log.i("@@@@@@@@@@", "setting size: " + width + 'x' + height);
    SetMeasuredDimension(width, height);
}

ECode VideoView::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    SurfaceView::OnInitializeAccessibilityEvent(event);

    AutoPtr<ICharSequence> clsName;
    CStringWrapper::New(String("CVideoView"), (ICharSequence**)&clsName);
    event->SetClassName(clsName);
    return NOERROR;
}

ECode VideoView::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    SurfaceView::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> clsName;
    CStringWrapper::New(String("CVideoView"), (ICharSequence**)&clsName);
    info->SetClassName(clsName);
    return NOERROR;
}

Int32 VideoView::ResolveAdjustedSize(
    /* [in] */ Int32 desiredSize,
    /* [in] */ Int32 measureSpec)
{
    Int32 result = desiredSize;
    Int32 specMode = MeasureSpec::GetMode(measureSpec);
    Int32 specSize =  MeasureSpec::GetSize(measureSpec);

    switch (specMode) {
        case MeasureSpec::UNSPECIFIED:
            /* Parent says we can be as big as we want. Just don't be larger
             * than max size imposed on ourselves.
             */
            result = desiredSize;
            break;

        case MeasureSpec::AT_MOST:
            /* Parent says we can be as big as we want, up to specSize.
             * Don't be larger than specSize, and don't be larger than
             * the max size imposed on ourselves.
             */
            result = Elastos::Core::Math::Min(desiredSize, specSize);
            break;

        case MeasureSpec::EXACTLY:
            // No choice. Do what we are told.
            result = specSize;
            break;
    }
    return result;
}

ECode VideoView::SetVideoPath(
    /* [in] */ const String& path)
{
    AutoPtr<IUri> uri;
    FAIL_RETURN(Uri::Parse(path, (IUri**)&uri));
    return SetVideoURI(uri);
}

ECode VideoView::SetVideoURI(
    /* [in] */ IUri* uri)
{
    return SetVideoURI(uri, NULL);
}

ECode VideoView::SetVideoURI(
    /* [in] */ IUri* uri,
    /* [in] */  HashMap<String, String>* headers)
{
    mUri = uri;
    if (headers != NULL) {
        mHeaders = *headers;
    }
    mSeekWhenPrepared = 0;
    OpenVideo();
    FAIL_RETURN(RequestLayout());
    return Invalidate();
}

ECode VideoView::StopPlayback()
{
    if (mMediaPlayer != NULL) {
        mMediaPlayer->Stop();
        mMediaPlayer->ReleaseResources();
        mMediaPlayer = NULL;
        mCurrentState = STATE_IDLE;
        mTargetState  = STATE_IDLE;
    }

    return NOERROR;
}

void VideoView::OpenVideo()
{
    if (mUri == NULL || mSurfaceHolder == NULL) {
        // not ready for playback just yet, will try again later
        return;
    }
    // Tell the music playback service to pause
    // TODO: these constants need to be published somewhere in the framework.
    AutoPtr<IIntent> i;
    ASSERT_SUCCEEDED(CIntent::New(
        String("com.android.music.musicservicecommand"), (IIntent**)&i));
    i->PutExtra(String("command"), String("pause"));
    mContext->SendBroadcast(i);

    // we shouldn't clear the target state, because somebody might have
    // called start() previously
    Release(FALSE);
    //try {
    ECode ec = CMediaPlayer::New((IMediaPlayer**)&mMediaPlayer);
    if (FAILED(ec)) goto exit;
    ASSERT_SUCCEEDED(mMediaPlayer->SetOnPreparedListener(mPreparedListener));
    ASSERT_SUCCEEDED(mMediaPlayer->SetOnVideoSizeChangedListener(mSizeChangedListener));
    ASSERT_SUCCEEDED(mMediaPlayer->SetOnCompletionListener(mCompletionListener));
    ASSERT_SUCCEEDED(mMediaPlayer->SetOnErrorListener(mErrorListener));
    mMediaPlayer->SetOnInfoListener(mOnInfoListener);
    ASSERT_SUCCEEDED(mMediaPlayer->SetOnBufferingUpdateListener(mBufferingUpdateListener));
    mCurrentBufferPercentage = 0;
    ec = mMediaPlayer->SetDataSource(mContext, mUri, NULL/*mHeaders*/);
    if (FAILED(ec)) goto exit;
    ASSERT_SUCCEEDED(mMediaPlayer->SetDisplay(mSurfaceHolder));
    ASSERT_SUCCEEDED(mMediaPlayer->SetAudioStreamType(IAudioManager::STREAM_MUSIC));
    ASSERT_SUCCEEDED(mMediaPlayer->SetScreenOnWhilePlaying(TRUE));
    ec = mMediaPlayer->PrepareAsync();
    if (FAILED(ec)) goto exit;
    // we don't set the target state here either, but preserve the
    // target state that was there before.
    mCurrentState = STATE_PREPARING;
    AttachMediaController();
exit:
    if (ec == (ECode)E_IO_EXCEPTION) {
        mCurrentState = STATE_ERROR;
        mTargetState = STATE_ERROR;
        Boolean result;
        mErrorListener->OnError(mMediaPlayer, IMediaPlayer::MEDIA_ERROR_UNKNOWN, 0, &result);
        return;
    }
    else if (ec == (ECode)E_ILLEGAL_ARGUMENT_EXCEPTION) {
        mCurrentState = STATE_ERROR;
        mTargetState = STATE_ERROR;
        Boolean result;
        mErrorListener->OnError(mMediaPlayer, IMediaPlayer::MEDIA_ERROR_UNKNOWN, 0, &result);
        return;
    }
    // } catch (IOException ex) {
    //     Log.w(TAG, "Unable to open content: " + mUri, ex);
    //     mCurrentState = STATE_ERROR;
    //     mTargetState = STATE_ERROR;
    //     mErrorListener.onError(mMediaPlayer, MediaPlayer.MEDIA_ERROR_UNKNOWN, 0);
    //     return;
    // } catch (IllegalArgumentException ex) {
    //     Log.w(TAG, "Unable to open content: " + mUri, ex);
    //     mCurrentState = STATE_ERROR;
    //     mTargetState = STATE_ERROR;
    //     mErrorListener.onError(mMediaPlayer, MediaPlayer.MEDIA_ERROR_UNKNOWN, 0);
    //     return;
    // }
}

ECode VideoView::SetMediaController(
    /* [in] */ IMediaController* controller)
{
    if (mMediaController != NULL) {
        mMediaController->Hide();
    }
    mMediaController = controller;
    AttachMediaController();

    return NOERROR;
}

void VideoView::AttachMediaController()
{
    if (mMediaPlayer != NULL && mMediaController != NULL) {
        mMediaController->SetMediaPlayer((IMediaPlayerControl*)this->Probe(EIID_IMediaPlayerControl));

        AutoPtr<IViewParent> parent = GetParent();
        IView* anchorView = IView::Probe(parent) != NULL ?
                IView::Probe(parent) : (IView*)this->Probe(EIID_IView);
        mMediaController->SetAnchorView(anchorView);
        mMediaController->SetEnabled(IsInPlaybackState());
    }
}

ECode VideoView::SetOnPreparedListener(
    /* [in] */ IMediaPlayerOnPreparedListener* l)
{
    mOnPreparedListener = l;
    return NOERROR;
}

ECode VideoView::SetOnCompletionListener(
    /* [in] */ IMediaPlayerOnCompletionListener* l)
{
    mOnCompletionListener = l;
    return NOERROR;
}

ECode VideoView::SetOnErrorListener(
    /* [in] */ IMediaPlayerOnErrorListener* l)
{
    mOnErrorListener = l;
    return NOERROR;
}

/**
 * Register a callback to be invoked when an informational event
 * occurs during playback or setup.
 *
 * @param l The callback that will be run
 */
ECode VideoView::SetOnInfoListener(
    /* [in] */ IMediaPlayerOnInfoListener* l)
{
    mOnInfoListener = l;
    return NOERROR;
}

void VideoView::Release(
    /* [in] */ Boolean cleartargetstate)
{
    if (mMediaPlayer != NULL) {
        mMediaPlayer->Reset();
        mMediaPlayer->ReleaseResources();
        mMediaPlayer = NULL;
        mCurrentState = STATE_IDLE;
        if (cleartargetstate) {
            mTargetState  = STATE_IDLE;
        }
    }
}

Boolean VideoView::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    if (IsInPlaybackState() && mMediaController != NULL) {
        ToggleMediaControlsVisiblity();
    }
    return FALSE;
}

Boolean VideoView::OnTrackballEvent(
    /* [in] */ IMotionEvent* event)
{
    if (IsInPlaybackState() && mMediaController != NULL) {
        ToggleMediaControlsVisiblity();
    }
    return FALSE;
}

Boolean VideoView::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    Boolean isKeyCodeSupported = keyCode != IKeyEvent::KEYCODE_BACK &&
                                 keyCode != IKeyEvent::KEYCODE_VOLUME_UP &&
                                 keyCode != IKeyEvent::KEYCODE_VOLUME_DOWN &&
                                 keyCode != IKeyEvent::KEYCODE_VOLUME_MUTE &&
                                 keyCode != IKeyEvent::KEYCODE_MENU &&
                                 keyCode != IKeyEvent::KEYCODE_CALL &&
                                 keyCode != IKeyEvent::KEYCODE_ENDCALL;
    if (IsInPlaybackState() && isKeyCodeSupported && mMediaController != NULL) {
        Boolean isPlaying;
        if (keyCode == IKeyEvent::KEYCODE_HEADSETHOOK ||
            keyCode == IKeyEvent::KEYCODE_MEDIA_PLAY_PAUSE) {
            if (mMediaPlayer->IsPlaying(&isPlaying), isPlaying) {
                Pause();
                mMediaController->Show();
            }
            else {
                Start();
                mMediaController->Hide();
            }
            return TRUE;
        }
        else if (keyCode == IKeyEvent::KEYCODE_MEDIA_PLAY) {
            if (mMediaPlayer->IsPlaying(&isPlaying), !isPlaying) {
                Start();
                mMediaController->Hide();
            }
            return true;
        }
        else if (keyCode == IKeyEvent::KEYCODE_MEDIA_STOP|| keyCode == IKeyEvent::KEYCODE_MEDIA_PAUSE) {
            if (mMediaPlayer->IsPlaying(&isPlaying), isPlaying) {
                Pause();
                mMediaController->Show();
            }

            return true;
        }
        else {
            ToggleMediaControlsVisiblity();
        }
    }

    return SurfaceView::OnKeyDown(keyCode, event);
}

void VideoView::ToggleMediaControlsVisiblity()
{
    Boolean isShowing;
    mMediaController->IsShowing(&isShowing);
    if (isShowing) {
        mMediaController->Hide();
    }
    else {
        mMediaController->Show();
    }
}

ECode VideoView::Start()
{
    if (IsInPlaybackState()) {
        mMediaPlayer->Start();
        mCurrentState = STATE_PLAYING;
    }
    mTargetState = STATE_PLAYING;

    return NOERROR;
}

ECode VideoView::Pause()
{
    if (IsInPlaybackState()) {
        Boolean isPlaying;
        if (mMediaPlayer->IsPlaying(&isPlaying), isPlaying) {
            mMediaPlayer->Pause();
            mCurrentState = STATE_PAUSED;
        }
    }
    mTargetState = STATE_PAUSED;

    return NOERROR;
}


ECode VideoView::Suspend()
{
    Release(FALSE);

    return NOERROR;
}

ECode VideoView::Resume()
{
    OpenVideo();

    return NOERROR;
}

Int32 VideoView::GetDuration()
{
    if (IsInPlaybackState()) {
        Int32 rst;
        mMediaPlayer->GetDuration(&rst);
        return rst;
    }

    return -1;
}

Int32 VideoView::GetCurrentPosition()
{
    if (IsInPlaybackState()) {
        Int32 position;
        mMediaPlayer->GetCurrentPosition(&position);
        return position;
    }
    return 0;
}

ECode VideoView::SeekTo(
    /* [in] */ Int32 msec)
{
    if (IsInPlaybackState()) {
        mMediaPlayer->SeekTo(msec);
        mSeekWhenPrepared = 0;
    }
    else {
        mSeekWhenPrepared = msec;
    }

    return NOERROR;
}

Boolean VideoView::IsPlaying()
{
    Boolean isPlaying;
    mMediaPlayer->IsPlaying(&isPlaying);
    return IsInPlaybackState() && isPlaying;
}

Int32 VideoView::GetBufferPercentage()
{
    if (mMediaPlayer != NULL) {
        return mCurrentBufferPercentage;
    }
    return 0;
}

Boolean VideoView::IsInPlaybackState()
{
    return (mMediaPlayer != NULL &&
                mCurrentState != STATE_ERROR &&
                mCurrentState != STATE_IDLE &&
                mCurrentState != STATE_PREPARING);
}

Boolean VideoView::CanPause()
{
    return mCanPause;
}

Boolean VideoView::CanSeekBackward()
{
    return mCanSeekBack;
}

Boolean VideoView::CanSeekForward()
{
    return mCanSeekForward;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos

