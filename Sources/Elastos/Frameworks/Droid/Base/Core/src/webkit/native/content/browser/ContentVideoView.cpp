
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//===============================================================
//               ContentVideoView::VideoSurfaceView
//===============================================================

ContentVideoView::VideoSurfaceView::VideoSurfaceView(
    /* [in] */ ContentVideoView* owner,
    /* [in] */ IContext* context)
    : mOwner(owner)
{
    super(context);
}

//@Override
ECode ContentVideoView::VideoSurfaceView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    // set the default surface view size to (1, 1) so that it won't block
    // the infobar. (0, 0) is not a valid size for surface view.
    Int32 width = 1;
    Int32 height = 1;
    if (mVideoWidth > 0 && mVideoHeight > 0) {
        width = GetDefaultSize(mVideoWidth, widthMeasureSpec);
        height = GetDefaultSize(mVideoHeight, heightMeasureSpec);
        if (mVideoWidth * height  > width * mVideoHeight) {
            height = width * mVideoHeight / mVideoWidth;
        }
        else if (mVideoWidth * height  < width * mVideoHeight) {
            width = height * mVideoWidth / mVideoHeight;
        }
    }

    SetMeasuredDimension(width, height);

    return NOERROR;
}

//===============================================================
//                ContentVideoView::ProgressView
//===============================================================

ContentVideoView::ProgressView::ProgressView(
    /* [in] */ IContext* context,
    /* [in] */ String videoLoadingText)
{
    super(context);
    SetOrientation(LinearLayout.VERTICAL);
    AutoPtr<ILinearLayoutLayoutParams> params;
    CLinearLayoutLayoutParams::New(
            ILinearLayoutLayoutParams::WRAP_CONTENT,
            ILinearLayoutLayoutParams::WRAP_CONTENT,
            (ILinearLayoutLayoutParams**)&params);
    SetLayoutParams(params);
    CProgressBar::New(context, NULL, android::R::attr::progressBarStyleLarge,
        (IProgressBar**)&mProgressBar);
    CTextView::New(context, (ITextView**)&mTextView);
    mTextView->SetText(videoLoadingText);
    AddView(mProgressBar);
    AddView(mTextView);
}

//===============================================================
//               ContentVideoView::InnerRunnable
//===============================================================

ContentVideoView::InnerRunnable::InnerRunnable(
    /* [in] */ ContentVideoView* owner)
    : mOwner(owner)
{
}

ECode ContentVideoView::InnerRunnable::Run()
{
    mOwner->ExitFullscreen(TRUE);
    return NOERROR;
}

//===============================================================
//                     ContentVideoView
//===============================================================

const String ContentVideoView::TAG("ContentVideoView");

/* Do not change these values without updating their counterparts
 * in include/media/mediaplayer.h!
 */
const Int32 ContentVideoView::MEDIA_NOP;
const Int32 ContentVideoView::MEDIA_PREPARED;
const Int32 ContentVideoView::MEDIA_PLAYBACK_COMPLETE;
const Int32 ContentVideoView::MEDIA_BUFFERING_UPDATE;
const Int32 ContentVideoView::MEDIA_SEEK_COMPLETE;
const Int32 ContentVideoView::MEDIA_SET_VIDEO_SIZE;
const Int32 ContentVideoView::MEDIA_ERROR;
const Int32 ContentVideoView::MEDIA_INFO;

/**
 * Keep these error codes in sync with the code we defined in
 * MediaPlayerListener.java.
 */
const Int32 ContentVideoView::MEDIA_ERROR_NOT_VALID_FOR_PROGRESSIVE_PLAYBACK;
const Int32 ContentVideoView::MEDIA_ERROR_INVALID_CODE;

// all possible internal states
const Int32 ContentVideoView::STATE_ERROR;
const Int32 ContentVideoView::STATE_IDLE;
const Int32 ContentVideoView::STATE_PLAYING;
const Int32 ContentVideoView::STATE_PAUSED;
const Int32 ContentVideoView::STATE_PLAYBACK_COMPLETED;

ContentVideoView::ContentVideoView(
    /* [in] */ IContext* context,
    /* [in] */ Int64 nativeContentVideoView,
    /* [in] */ ContentVideoViewClient* client)
    : mVideoWidth(0)
    , mVideoHeight(0)
    , mDuration(0)
    , mCurrentState(0)
{
    super(context);
    mExitFullscreenRunnable = new InnerRunnable(this);
    mNativeContentVideoView = nativeContentVideoView;
    mViewAndroid = new ViewAndroid(new WindowAndroid(context.getApplicationContext()), this);
    mClient = client;
    initResources(context);
    mVideoSurfaceView = new VideoSurfaceView(context);
    showContentVideoView();
    setVisibility(View.VISIBLE);
}

AutoPtr<ContentVideoViewClient> ContentVideoView::GetContentVideoViewClient()
{
    return mClient;
}

void ContentVideoView::InitResources(
    /* [in] */ IContext* context)
{
    if (mPlaybackErrorText != null) return;
    mPlaybackErrorText = context.getString(
            org.chromium.content.R.string.media_player_error_text_invalid_progressive_playback);
    mUnknownErrorText = context.getString(
            org.chromium.content.R.string.media_player_error_text_unknown);
    mErrorButton = context.getString(
            org.chromium.content.R.string.media_player_error_button);
    mErrorTitle = context.getString(
            org.chromium.content.R.string.media_player_error_title);
    mVideoLoadingText = context.getString(
            org.chromium.content.R.string.media_player_loading_video);
}

void ContentVideoView::ShowContentVideoView()
{
    mVideoSurfaceView.getHolder().addCallback(this);
    this.addView(mVideoSurfaceView, new FrameLayout.LayoutParams(
            ViewGroup.LayoutParams.WRAP_CONTENT,
            ViewGroup.LayoutParams.WRAP_CONTENT,
            Gravity.CENTER));

    mProgressView = mClient.getVideoLoadingProgressView();
    if (mProgressView == null) {
        mProgressView = new ProgressView(getContext(), mVideoLoadingText);
    }
    this.addView(mProgressView, new FrameLayout.LayoutParams(
            ViewGroup.LayoutParams.WRAP_CONTENT,
            ViewGroup.LayoutParams.WRAP_CONTENT,
            Gravity.CENTER));
}

AutoPtr<ISurfaceView> ContentVideoView::GetSurfaceView()
{
    return mVideoSurfaceView;
}

//@CalledByNative
void ContentVideoView::OnMediaPlayerError(
    /* [in] */ Int32 errorType)
{
    Log.d(TAG, "OnMediaPlayerError: " + errorType);
    if (mCurrentState == STATE_ERROR || mCurrentState == STATE_PLAYBACK_COMPLETED) {
        return;
    }

    // Ignore some invalid error codes.
    if (errorType == MEDIA_ERROR_INVALID_CODE) {
        return;
    }

    mCurrentState = STATE_ERROR;

    /* Pop up an error dialog so the user knows that
     * something bad has happened. Only try and pop up the dialog
     * if we're attached to a window. When we're going away and no
     * longer have a window, don't bother showing the user an error.
     *
     * TODO(qinmin): We need to review whether this Dialog is OK with
     * the rest of the browser UI elements.
     */
    if (getWindowToken() != null) {
        String message;

        if (errorType == MEDIA_ERROR_NOT_VALID_FOR_PROGRESSIVE_PLAYBACK) {
            message = mPlaybackErrorText;
        } else {
            message = mUnknownErrorText;
        }

        try {
            new AlertDialog.Builder(getContext())
                .setTitle(mErrorTitle)
                .setMessage(message)
                .setPositiveButton(mErrorButton,
                        new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int whichButton) {
                        /* Inform that the video is over.
                         */
                        onCompletion();
                    }
                })
                .setCancelable(false)
                .show();
        } catch (RuntimeException e) {
            Log.e(TAG, "Cannot show the alert dialog, error message: " + message, e);
        }
    }
}

//@CalledByNative
void ContentVideoView::OnVideoSizeChanged(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    mVideoWidth = width;
    mVideoHeight = height;
    // This will trigger the SurfaceView.onMeasure() call.
    mVideoSurfaceView.getHolder().setFixedSize(mVideoWidth, mVideoHeight);
}

//@CalledByNative
void ContentVideoView::OnBufferingUpdate(
    /* [in] */ Int32 percent)
{
}

//@CalledByNative
void ContentVideoView::OnPlaybackComplete()
{
    OnCompletion();
}

//@CalledByNative
void ContentVideoView::OnUpdateMediaMetadata(
    /* [in] */ Int32 videoWidth,
    /* [in] */ Int32 videoHeight,
    /* [in] */ Int32 duration,
    /* [in] */ Boolean canPause,
    /* [in] */ Boolean canSeekBack,
    /* [in] */ Boolean canSeekForward)
{
    mDuration = duration;
    mProgressView.setVisibility(View.GONE);
    mCurrentState = isPlaying() ? STATE_PLAYING : STATE_PAUSED;
    onVideoSizeChanged(videoWidth, videoHeight);
}

//@Override
void ContentVideoView::SurfaceChanged(
    /* [in] */ ISurfaceHolder* holder,
    /* [in] */ Int32 format,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
}

//@Override
void ContentVideoView::SurfaceCreated(
    /* [in] */ ISurfaceHolder* holder)
{
    mSurfaceHolder = holder;
    openVideo();
}

//@Override
void ContentVideoView::SurfaceDestroyed(
    /* [in] */ ISurfaceHolder* holder)
{
    if (mNativeContentVideoView != 0) {
        nativeSetSurface(mNativeContentVideoView, null);
    }
    mSurfaceHolder = null;
    post(mExitFullscreenRunnable);
}

//@CalledByNative
void ContentVideoView::OpenVideo()
{
    if (mSurfaceHolder != null) {
        mCurrentState = STATE_IDLE;
        if (mNativeContentVideoView != 0) {
            nativeRequestMediaMetadata(mNativeContentVideoView);
            nativeSetSurface(mNativeContentVideoView,
                    mSurfaceHolder.getSurface());
        }
    }
}

void ContentVideoView::OnCompletion()
{
    mCurrentState = STATE_PLAYBACK_COMPLETED;
}

Boolean ContentVideoView::IsInPlaybackState()
{
    return (mCurrentState != STATE_ERROR && mCurrentState != STATE_IDLE);
}

void ContentVideoView::Start()
{
    if (isInPlaybackState()) {
        if (mNativeContentVideoView != 0) {
            nativePlay(mNativeContentVideoView);
        }
        mCurrentState = STATE_PLAYING;
    }
}

void ContentVideoView::Pause()
{
    if (isInPlaybackState()) {
        if (isPlaying()) {
            if (mNativeContentVideoView != 0) {
                nativePause(mNativeContentVideoView);
            }
            mCurrentState = STATE_PAUSED;
        }
    }
}

// cache duration as mDuration for faster access
Int32 ContentVideoView::GetDuration()
{
    if (isInPlaybackState()) {
        if (mDuration > 0) {
            return mDuration;
        }
        if (mNativeContentVideoView != 0) {
            mDuration = nativeGetDurationInMilliSeconds(mNativeContentVideoView);
        } else {
            mDuration = 0;
        }
        return mDuration;
    }
    mDuration = -1;
    return mDuration;
}

Int32 ContentVideoView::GetCurrentPosition()
{
    if (isInPlaybackState() && mNativeContentVideoView != 0) {
        return nativeGetCurrentPosition(mNativeContentVideoView);
    }
    return 0;
}

void ContentVideoView::SeekTo(
    /* [in] */ Int32 msec)
{
    if (mNativeContentVideoView != 0) {
        nativeSeekTo(mNativeContentVideoView, msec);
    }
}

Boolean ContentVideoView::IsPlaying()
{
    return mNativeContentVideoView != 0 && nativeIsPlaying(mNativeContentVideoView);
}

//@CalledByNative
AutoPtr<ContentVideoView> ContentVideoView::CreateContentVideoView(
    /* [in] */ IContext* context,
    /* [in] */ Int64 nativeContentVideoView,
    /* [in] */ ContentVideoViewClient* client,
    /* [in] */ Boolean legacy)
{
    ThreadUtils.assertOnUiThread();
    // The context needs be Activity to create the ContentVideoView correctly.
    if (!isActivityContext(context)) {
        Log.e(TAG, "Wrong type of context, can't create fullscreen video");
        return null;
    }
    ContentVideoView videoView = null;
    if (legacy) {
        videoView = new ContentVideoViewLegacy(context, nativeContentVideoView, client);
    } else {
        videoView = new ContentVideoView(context, nativeContentVideoView, client);
    }

    if (videoView.getContentVideoViewClient().onShowCustomView(videoView)) {
        return videoView;
    }
    return null;
}

Boolean ContentVideoView::IsActivityContext(
    /* [in] */ IContext* context)
{
    // Only retrieve the base context if the supplied context is a ContextWrapper but not
    // an Activity, given that Activity is already a subclass of ContextWrapper.
    if (context instanceof ContextWrapper && !(context instanceof Activity)) {
        context = ((ContextWrapper) context).getBaseContext();
    }
    return context instanceof Activity;
}

void ContentVideoView::RemoveSurfaceView()
{
    removeView(mVideoSurfaceView);
    removeView(mProgressView);
    mVideoSurfaceView = null;
    mProgressView = null;
}

void ContentVideoView::ExitFullscreen(
    /* [in] */ Boolean relaseMediaPlayer)
{
    destroyContentVideoView(false);
    if (mNativeContentVideoView != 0) {
        nativeExitFullscreen(mNativeContentVideoView, relaseMediaPlayer);
        mNativeContentVideoView = 0;
    }
}

//@CalledByNative
void ContentVideoView::OnExitFullscreen()
{
    exitFullscreen(false);
}

/**
 * This method shall only be called by native and exitFullscreen,
 * To exit fullscreen, use exitFullscreen in Java.
 */
//@CalledByNative
void ContentVideoView::DestroyContentVideoView(
    /* [in] */ Boolean nativeViewDestroyed)
{
    if (mVideoSurfaceView != null) {
        removeSurfaceView();
        setVisibility(View.GONE);

        // To prevent re-entrance, call this after removeSurfaceView.
        mClient.onDestroyContentVideoView();
    }
    if (nativeViewDestroyed) {
        mNativeContentVideoView = 0;
    }
}

AutoPtr<ContentVideoView> ContentVideoView::GetContentVideoView()
{
    return nativeGetSingletonJavaContentVideoView();
}

//@Override
Boolean ContentVideoView::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (keyCode == KeyEvent.KEYCODE_BACK) {
        exitFullscreen(false);
        return true;
    }
    return super.onKeyUp(keyCode, event);
}

//@Override
AutoPtr<IView> ContentVideoView::AcquireAnchorView()
{
    View anchorView = new View(getContext());
    addView(anchorView);
    return anchorView;
}

//@Override
void ContentVideoView::SetAnchorViewPosition(
    /* [in] */ IView* view,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float width,
    /* [in] */ Float height)
{
    Log.e(TAG, "setAnchorViewPosition isn't implemented");
}

//@Override
void ContentVideoView::ReleaseAnchorView(
    /* [in] */ IView* anchorView)
{
    removeView(anchorView);
}

//@CalledByNative
Int64 ContentVideoView::GetNativeViewAndroid()
{
    return mViewAndroid.getNativePointer();
}

AutoPtr<ContentVideoView> ContentVideoView::NativeGetSingletonJavaContentVideoView()
{
}

void ContentVideoView::NativeExitFullscreen(
    /* [in] */ Int64 nativeContentVideoView,
    /* [in] */ Boolean relaseMediaPlayer)
{
}

Int32 ContentVideoView::NativeGetCurrentPosition(
    /* [in] */ Int64 nativeContentVideoView)
{
}

Int32 ContentVideoView::NativeGetDurationInMilliSeconds(
    /* [in] */ Int64 nativeContentVideoView)
{
}

void ContentVideoView::NativeRequestMediaMetadata(
    /* [in] */ Int64 nativeContentVideoView)
{
}

Int32 ContentVideoView::NativeGetVideoWidth(
    /* [in] */ Int64 nativeContentVideoView)
{
}

Int32 ContentVideoView::NativeGetVideoHeight(
    /* [in] */ Int64 nativeContentVideoView)
{
}

Boolean ContentVideoView::NativeIsPlaying(
    /* [in] */ Int64 nativeContentVideoView)
{
}

void ContentVideoView::NativePause(
    /* [in] */ Int64 nativeContentVideoView)
{
}

void ContentVideoView::NativePlay(
    /* [in] */ Int64 nativeContentVideoView)
{
}

void ContentVideoView::NativeSeekTo(
    /* [in] */ Int64 nativeContentVideoView,
    /* [in] */ Int32 msec)
{
}

void ContentVideoView::NativeSetSurface(
    /* [in] */ Int64 nativeContentVideoView,
    /* [in] */ ISurface* surface)
{
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
