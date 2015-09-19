
#include "webkit/HTML5VideoFullScreen.h"
#include "webkit/HTML5VideoViewProxy.h"
#include "webkit/WebChromeClient.h"
#include "webkit/CWebViewClassic.h"
#include "widget/CFrameLayout.h"
#include "widget/CFrameLayoutLayoutParams.h"

#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;
using Elastos::Droid::Media::IMetadata;
using Elastos::Droid::Media::EIID_IMediaPlayerOnPreparedListener;
using Elastos::Droid::Media::EIID_IMediaPlayerOnBufferingUpdateListener;
using Elastos::Droid::Media::EIID_IMediaPlayerOnVideoSizeChangedListener;
using Elastos::Droid::Widget::EIID_IMediaController;
using Elastos::Droid::Widget::EIID_IMediaPlayerControl;
using Elastos::Droid::Widget::CFrameLayoutLayoutParams;
using Elastos::Droid::Widget::IFrameLayoutLayoutParams;
using Elastos::Droid::Widget::CFrameLayout;
using Elastos::Droid::View::EIID_ISurfaceView;
using Elastos::Droid::View::EIID_ISurfaceHolderCallback;
using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::EIID_IViewOnTouchListener;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::IViewGroupLayoutParams;

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//         HTML5VideoFullScreen::FullScreenMediaController
//===============================================================
HTML5VideoFullScreen::FullScreenMediaController::FullScreenMediaController(
    /* [in] */ IContext* context,
    /* [in] */ IView* video)
    : MediaController(context)
    , mVideoView(video)
{
}

CAR_INTERFACE_IMPL(HTML5VideoFullScreen::FullScreenMediaController, IMediaController);

IVIEW_METHODS_IMPL(HTML5VideoFullScreen::FullScreenMediaController, View);

IVIEWGROUP_METHODS_IMPL(HTML5VideoFullScreen::FullScreenMediaController, ViewGroup);

IFRAMELAYOUT_METHODS_IMPL(HTML5VideoFullScreen::FullScreenMediaController, FrameLayout);

ECode HTML5VideoFullScreen::FullScreenMediaController::SetMediaPlayer(
    /* [in] */ IMediaPlayerControl* player)
{
    MediaController::SetMediaPlayer(player);
    return NOERROR;
}

ECode HTML5VideoFullScreen::FullScreenMediaController::SetAnchorView(
    /* [in] */ IView* view)
{
    MediaController::SetAnchorView(view);
    return NOERROR;
}

ECode HTML5VideoFullScreen::FullScreenMediaController::Show()
{
    MediaController::Show();
    if (mVideoView != NULL) {
        mVideoView->SetSystemUiVisibility(IView::SYSTEM_UI_FLAG_VISIBLE);
    }

    return NOERROR;
}

ECode HTML5VideoFullScreen::FullScreenMediaController::Show(
    /* [in] */ Int32 timeout)
{
    MediaController::Show(timeout);
    return NOERROR;
}

ECode HTML5VideoFullScreen::FullScreenMediaController::IsShowing(
    /* [out] */ Boolean* isShowing)
{
    VALIDATE_NOT_NULL(isShowing);
    *isShowing = MediaController::IsShowing();
    return NOERROR;
}

ECode HTML5VideoFullScreen::FullScreenMediaController::Hide()
{
    if (mVideoView != NULL) {
        mVideoView->SetSystemUiVisibility(IView::SYSTEM_UI_FLAG_LOW_PROFILE
                | IView::SYSTEM_UI_FLAG_HIDE_NAVIGATION);
    }
    MediaController::Hide();
    return NOERROR;
}

ECode HTML5VideoFullScreen::FullScreenMediaController::SetPrevNextListeners(
    /* [in] */ IViewOnClickListener* next,
    /* [in] */ IViewOnClickListener* prev)
{
    MediaController::SetPrevNextListeners(next, prev);
    return NOERROR;
}

//===============================================================
//            HTML5VideoFullScreen::VideoSurfaceView
//===============================================================

HTML5VideoFullScreen::VideoSurfaceView::VideoSurfaceView(
    /* [in] */ HTML5VideoFullScreen* owner,
    /* [in] */ IContext* context)
    : mOwner(owner)
{
    SurfaceView::Init(context);
}

PInterface HTML5VideoFullScreen::VideoSurfaceView::Probe(
    /* [in] */ REIID riid)
{
    if ( riid == EIID_IInterface) {
        return (IInterface*)(ISurfaceView*)this;
    }
    else if ( riid == EIID_View) {
        return (PInterface)(View*)this;
    }
    else if ( riid == EIID_IView) {
        return (IView*)this;
    }
    else if ( riid == EIID_ISurfaceView) {
        return (ISurfaceView*)this;
    }
    return NULL;
}

UInt32 HTML5VideoFullScreen::VideoSurfaceView::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 HTML5VideoFullScreen::VideoSurfaceView::Release()
{
    return ElRefBase::Release();
}

ECode HTML5VideoFullScreen::VideoSurfaceView::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    VALIDATE_NOT_NULL(iid);
    if (object == (IInterface*)(View*)this) {
        *iid = EIID_View;
    }
    if (object == (IInterface*)(IView*)this) {
        *iid = EIID_IView;
    }
    else if (object == (IInterface*)(ISurfaceView*)this) {
        *iid = EIID_ISurfaceView;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

IVIEW_METHODS_IMPL(HTML5VideoFullScreen::VideoSurfaceView, View);

ISURFACEVIEW_METHODS_IMPL(HTML5VideoFullScreen::VideoSurfaceView, SurfaceView);

void HTML5VideoFullScreen::VideoSurfaceView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    Int32 width = GetDefaultSize(mOwner->mVideoWidth, widthMeasureSpec);
    Int32 height = GetDefaultSize(mOwner->mVideoHeight, heightMeasureSpec);
    if (mOwner->mVideoWidth > 0 && mOwner->mVideoHeight > 0) {
        if ( mOwner->mVideoWidth * height  > width * mOwner->mVideoHeight ) {
            height = width * mOwner->mVideoHeight / mOwner->mVideoWidth;
        }
        else if ( mOwner->mVideoWidth * height  < width * mOwner->mVideoHeight ) {
            width = height * mOwner->mVideoWidth / mOwner->mVideoHeight;
        }
    }

    SetMeasuredDimension(width, height);
}

//===============================================================
//       HTML5VideoFullScreen::InnerSurfaceHolderCallback
//===============================================================

HTML5VideoFullScreen::InnerSurfaceHolderCallback::InnerSurfaceHolderCallback(
    /* [in] */ HTML5VideoFullScreen* owner)
    : mOwner(owner)
{
}

CAR_INTERFACE_IMPL_LIGHT(HTML5VideoFullScreen::InnerSurfaceHolderCallback, ISurfaceHolderCallback);

ECode HTML5VideoFullScreen::InnerSurfaceHolderCallback::SurfaceCreated(
    /* [in] */ ISurfaceHolder* holder)
{
    mOwner->mSurfaceHolder = holder;
    mOwner->mFullScreenMode = HTML5VideoFullScreen::FULLSCREEN_SURFACECREATED;

    mOwner->PrepareForFullScreen();

    return NOERROR;
}

ECode HTML5VideoFullScreen::InnerSurfaceHolderCallback::SurfaceChanged(
    /* [in] */ ISurfaceHolder* holder,
    /* [in] */ Int32 format,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    if (mOwner->mPlayer != NULL && mOwner->mMediaController != NULL
            && mOwner->mCurrentState == STATE_PREPARED) {
        Boolean isShowing = FALSE;
        mOwner->mMediaController->IsShowing(&isShowing);
        if (isShowing) {
            // ensure the controller will get repositioned later
            mOwner->mMediaController->Hide();
        }
        mOwner->mMediaController->Show();
    }

    return NOERROR;
}

ECode HTML5VideoFullScreen::InnerSurfaceHolderCallback::SurfaceDestroyed(
    /* [in] */ ISurfaceHolder* holder)
{
    mOwner->mPlayer->IsPlaying(&mOwner->mPlayingWhenDestroyed); //fixed by Bevis, do not restart play in inline mode.
    mOwner->PauseAndDispatch(mOwner->mProxy);
    // We need to set the display to null before switching into inline
    // mode to avoid error.
    mOwner->Pause();
    mPlayer->SetDisplay(NULL);
    mOwner->Reset();    //add by Bevis, reset media player when fullscreen video exited.
    mOwner->mSurfaceHolder = NULL;
    if (mOwner->mMediaController != NULL) {
        mOwner->mMediaController->Hide();
    }

    return NOERROR;
}

//==========================================================================
//     HTML5VideoFullScreen::InnerMediaPlayerOnVideoSizeChangedListener
//==========================================================================

HTML5VideoFullScreen::InnerMediaPlayerOnVideoSizeChangedListener::InnerMediaPlayerOnVideoSizeChangedListener(
    /* [in] */ HTML5VideoFullScreen* owner)
    : mOwner(owner)
{
}

CAR_INTERFACE_IMPL_LIGHT(HTML5VideoFullScreen::InnerMediaPlayerOnVideoSizeChangedListener, IMediaPlayerOnVideoSizeChangedListener);

ECode HTML5VideoFullScreen::InnerMediaPlayerOnVideoSizeChangedListener::OnVideoSizeChanged(
    /* [in] */ IMediaPlayer* mp,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    VALIDATE_NOT_NULL(mp);

    mp->GetVideoWidth(&mOwner->mVideoWidth);
    mp->GetVideoHeight(&mOwner->mVideoHeight);
    if (mOwner->mVideoWidth != 0 && mOwner->mVideoHeight != 0) {
        AutoPtr<ISurfaceHolder> holder;
        mOwner->mVideoSurfaceView->GetHolder((ISurfaceHolder**)&holder);
        holder->SetFixedSize(mOwner->mVideoWidth, mOwner->mVideoHeight);
    }

    return NOERROR;
}

//==========================================================================
//     HTML5VideoFullScreen::InnerMediaPlayerOnBufferingUpdateListener
//==========================================================================

HTML5VideoFullScreen::InnerMediaPlayerOnBufferingUpdateListener::InnerMediaPlayerOnBufferingUpdateListener(
    /* [in] */ HTML5VideoFullScreen* owner)
    : mOwner(owner)
{
}

CAR_INTERFACE_IMPL_LIGHT(HTML5VideoFullScreen::InnerMediaPlayerOnBufferingUpdateListener, IMediaPlayerOnBufferingUpdateListener);

ECode HTML5VideoFullScreen::InnerMediaPlayerOnBufferingUpdateListener::OnBufferingUpdate(
    /* [in] */ IMediaPlayer* mp,
    /* [in] */ Int32 percent)
{
    mOwner->mCurrentBufferPercentage = percent;
    return NOERROR;
}

//==========================================================================
//      HTML5VideoFullScreen::InnerWebChromeClientCustomViewCallback
//==========================================================================

HTML5VideoFullScreen::InnerWebChromeClientCustomViewCallback::InnerWebChromeClientCustomViewCallback(
    /* [in] */ HTML5VideoFullScreen* owner)
    : mOwner(owner)
{
}

CAR_INTERFACE_IMPL_LIGHT(HTML5VideoFullScreen::InnerWebChromeClientCustomViewCallback, IWebChromeClientCustomViewCallback);

ECode HTML5VideoFullScreen::InnerWebChromeClientCustomViewCallback::OnCustomViewHidden()
{
    assert(0);
    // which happens when the video view is detached from its parent
    // view. This happens in the WebChromeClient before this method
    // is invoked.
//    mOwner->mLayout->RemoveView(mOwner->GetSurfaceView());

    if (mProgressView != NULL) {
//        mOwner->mLayout->RemoveView(HTML5VideoFullScreen::mProgressView);
        HTML5VideoFullScreen::mProgressView = NULL;
    }
    mOwner->mLayout = NULL;
    // Re enable plugin views.
    mOwner->mProxy->GetWebView()->GetViewManager()->ShowAll();
    // Don't show the controller after exiting the full screen.
    mOwner->mMediaController = NULL;
    // Continue the inline mode playing if necessary.
    mOwner->mProxy->DispatchOnStopFullScreen(mOwner->mPlayingWhenDestroyed);
    mOwner->mProxy = NULL;

    return NOERROR;
}

//===============================================================
//                     HTML5VideoFullScreen
//===============================================================

const Int32 HTML5VideoFullScreen::FULLSCREEN_OFF;
const Int32 HTML5VideoFullScreen::FULLSCREEN_SURFACECREATING;
const Int32 HTML5VideoFullScreen::FULLSCREEN_SURFACECREATED;

AutoPtr<IView> HTML5VideoFullScreen::mProgressView;
AutoPtr<IFrameLayout> HTML5VideoFullScreen::mLayout;

HTML5VideoFullScreen::HTML5VideoFullScreen(
    /* [in] */ IContext* context,
    /* [in] */ Int32 videoLayerId,
    /* [in] */ Int32 position,
    /* [in] */ Boolean skipPrepare)
    : mFullScreenMode(FULLSCREEN_OFF)
    , mCanSeekBack(FALSE)
    , mCanSeekForward(FALSE)
    , mCanPause(FALSE)
    , mCurrentBufferPercentage(0)
    , mVideoWidth(0)
    , mVideoHeight(0)
    , mPlayingWhenDestroyed(FALSE)
{
    mVideoSurfaceView = new VideoSurfaceView(this, context);
    Init(videoLayerId, position, skipPrepare);
    mSHCallback = new InnerSurfaceHolderCallback(this);
    mSizeChangedListener =  new InnerMediaPlayerOnVideoSizeChangedListener(this);
    mBufferingUpdateListener = new InnerMediaPlayerOnBufferingUpdateListener(this);
    mCallback = new InnerWebChromeClientCustomViewCallback(this);
}

UInt32 HTML5VideoFullScreen::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 HTML5VideoFullScreen::Release()
{
    return ElRefBase::Release();
}

ECode HTML5VideoFullScreen::GetInterfaceID(
    /* [in] */ IInterface *Object,
    /* [out] */ InterfaceID *IID)
{
    VALIDATE_NOT_NULL(IID);

    if (Object == (IInterface*)(IMediaPlayerControl*)this) {
        *IID = EIID_IMediaPlayerControl;
    }
    else if (Object == (IInterface*)(IMediaPlayerOnPreparedListener*)this) {
        *IID = EIID_IMediaPlayerOnPreparedListener;
    }
    else if (Object == (IInterface*)(IViewOnTouchListener*)this) {
        *IID = EIID_IViewOnTouchListener;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

PInterface HTML5VideoFullScreen::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IMediaPlayerControl) {
        return (IMediaPlayerControl*)this;
    }
    else if (riid == EIID_IMediaPlayerOnPreparedListener) {
        return (IMediaPlayerOnPreparedListener*)this;
    }
    else if (riid == EIID_IViewOnTouchListener) {
        return (IViewOnTouchListener*)this;
    }

    return NULL;
}

ECode HTML5VideoFullScreen::Start()
{
    return HTML5VideoView::Start();
}

ECode HTML5VideoFullScreen::Pause()
{
    return HTML5VideoView::Pause();
}

ECode HTML5VideoFullScreen::GetDuration(
    /* [out] */ Int32* duration)
{
    VALIDATE_NOT_NULL(duration);
    *duration = HTML5VideoView::GetDuration();
    return NOERROR;
}

ECode HTML5VideoFullScreen::GetCurrentPosition(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    *position = HTML5VideoView::GetCurrentPosition();
    return NOERROR;
}

ECode HTML5VideoFullScreen::SeekTo(
    /* [in] */ Int32 position)
{
    return HTML5VideoView::SeekTo(position);
}

ECode HTML5VideoFullScreen::IsPlaying(
    /* [out] */ Boolean* isPlaying)
{
    VALIDATE_NOT_NULL(isPlaying);
    *isPlaying = HTML5VideoView::IsPlaying();
    return NOERROR;
}

void HTML5VideoFullScreen::DecideDisplayMode()
{
    mPlayer->SetDisplay(mSurfaceHolder);
}

ECode HTML5VideoFullScreen::OnPrepared(
    /* [in] */ IMediaPlayer* mp)
{
    HTML5VideoView::OnPrepared(mp);

    mVideoSurfaceView->SetOnTouchListener(this);
    // Get the capabilities of the player for this stream
    AutoPtr<IMetadata> data;
    mp->GetMetadata(IMediaPlayer::METADATA_ALL,
            IMediaPlayer::BYPASS_METADATA_FILTER,
            (IMetadata**)&data);

    if (data != NULL) {
        Boolean bFlag1, bFlag2;
        data->Has(IMetadata::PAUSE_AVAILABLE, &bFlag1);
        data->GetBoolean(IMetadata::PAUSE_AVAILABLE, &bFlag2);
        mCanPause = !bFlag1 || bFlag2;

        data->Has(IMetadata::SEEK_BACKWARD_AVAILABLE, &bFlag1);
        data->GetBoolean(IMetadata::SEEK_BACKWARD_AVAILABLE, &bFlag2);
        mCanSeekBack = !bFlag1 || bFlag2;

        data->Has(IMetadata::SEEK_FORWARD_AVAILABLE, &bFlag1);
        data->GetBoolean(IMetadata::SEEK_FORWARD_AVAILABLE, &bFlag2);
        mCanSeekForward = !bFlag1 || bFlag2;
    }
    else {
        mCanPause = mCanSeekBack = mCanSeekForward = TRUE;
    }

    if (GetStartWhenPrepared()) {
        mPlayer->Start();
        // Clear the flag.
        SetStartWhenPrepared(FALSE);
    }

    // mMediaController status depends on the Metadata result, so put it
    // after reading the MetaData.
    // And make sure mPlayer state is updated before showing the controller.
    if (mMediaController != NULL) {
        mMediaController->SetEnabled(TRUE);
        mMediaController->Show();
    }

    if (mProgressView != NULL) {
        mProgressView->SetVisibility(IView::GONE);
    }

    mp->GetVideoWidth(&mVideoWidth);
    mp->GetVideoHeight(&mVideoHeight);
    // This will trigger the onMeasure to get the display size right.
    AutoPtr<ISurfaceHolder> holder;
    mVideoSurfaceView->GetHolder((ISurfaceHolder**)&holder);
    holder->SetFixedSize(mVideoWidth, mVideoHeight);

    return NOERROR;
}

ECode HTML5VideoFullScreen::FullScreenExited(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = (mLayout == NULL);
    return NOERROR;
}

void HTML5VideoFullScreen::EnterFullScreenVideoState(
    /* [in] */ Int32 layerId,
    /* [in] */ HTML5VideoViewProxy* proxy,
    /* [in] */ CWebViewClassic* webView)
{
    mFullScreenMode = FULLSCREEN_SURFACECREATING;
    mCurrentBufferPercentage = 0;
    mPlayer->SetOnBufferingUpdateListener(mBufferingUpdateListener);
    mProxy = proxy;

    AutoPtr<ISurfaceHolder> surfaceHolder;
    mVideoSurfaceView->GetHolder((ISurfaceHolder**)&surfaceHolder);
    surfaceHolder->AddCallback(mSHCallback);
    surfaceHolder->SetType(ISurfaceHolder::SURFACE_TYPE_PUSH_BUFFERS);
    mVideoSurfaceView->SetFocusable(TRUE);
    mVideoSurfaceView->SetFocusableInTouchMode(TRUE);
    mVideoSurfaceView->RequestFocus(NULL);
    mVideoSurfaceView->SetOnKeyListener(mProxy);
    // Create a FrameLayout that will contain the VideoView and the
    // progress view (if any).
    mLayout = NULL;
    CFrameLayout::New(mProxy->GetContext(), (IFrameLayout**)&mLayout);
    AutoPtr<IFrameLayoutLayoutParams> layoutParams;
    CFrameLayoutLayoutParams::New(
                        IViewGroupLayoutParams::WRAP_CONTENT,
                        IViewGroupLayoutParams::WRAP_CONTENT,
                        IGravity::CENTER,
                        (IFrameLayoutLayoutParams**)&layoutParams);

   mLayout->AddView(GetSurfaceView(), layoutParams);

    mLayout->SetVisibility(IView::VISIBLE);
    AutoPtr<IWebChromeClient> client;
    webView->GetWebChromeClient((IWebChromeClient**)&client);
    if (client != NULL) {
        client->OnShowCustomView(mLayout, mCallback);
        // Plugins like Flash will draw over the video so hide
        // them while we're playing.
        if (webView->GetViewManager() != NULL) {
            webView->GetViewManager()->HideAll();
        }

        mProgressView = NULL;
        client->GetVideoLoadingProgressView((IView**)&mProgressView);
        if (mProgressView != NULL) {
            mLayout->AddView(mProgressView, layoutParams);
            mProgressView->SetVisibility(IView::VISIBLE);
        }
    }
}

/**
 * @return true when we are in full screen mode, even the surface not fully
 * created.
 */
ECode HTML5VideoFullScreen::IsFullScreenMode(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TRUE;
    return NOERROR;
}

// MediaController FUNCTIONS:
ECode HTML5VideoFullScreen::CanPause(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mCanPause;
    return NOERROR;
}

ECode HTML5VideoFullScreen::CanSeekBackward(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mCanSeekBack;
    return NOERROR;
}

ECode HTML5VideoFullScreen::CanSeekForward(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mCanSeekForward;
    return NOERROR;
}

ECode HTML5VideoFullScreen::GetBufferPercentage(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    if (mPlayer != NULL) {
        *result = mCurrentBufferPercentage;
        return NOERROR;
    }

    *result = 0;
    return NOERROR;
}

void HTML5VideoFullScreen::ShowControllerInFullScreen()
{
    if (mMediaController != NULL) {
        mMediaController->Show(0);
    }
}

ECode HTML5VideoFullScreen::OnTouch(
    /* [in] */ IView* v,
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    if (mFullScreenMode >= FULLSCREEN_SURFACECREATED
            && mMediaController != NULL) {
        ToggleMediaControlsVisiblity();
    }

    if (result) *result = FALSE;

    return NOERROR;
}

void HTML5VideoFullScreen::SwitchProgressView(
    /* [in] */ Boolean playerBuffering)
{
    if (mProgressView != NULL) {
        if (playerBuffering) {
            mProgressView->SetVisibility(IView::VISIBLE);
        }
        else {
            mProgressView->SetVisibility(IView::GONE);
        }
    }
}

AutoPtr<ISurfaceView> HTML5VideoFullScreen::GetSurfaceView()
{
    return mVideoSurfaceView;
}

void HTML5VideoFullScreen::SetMediaController(
    /* [in] */ IMediaController* m)
{
    mMediaController  = m;
    AttachMediaController();
}

void HTML5VideoFullScreen::AttachMediaController()
{
    if (mPlayer != NULL && mMediaController != NULL) {
        mMediaController->SetMediaPlayer(this);
        mMediaController->SetAnchorView(mVideoSurfaceView);
        //Will be enabled when prepared
        mMediaController->SetEnabled(FALSE);
    }
}

void HTML5VideoFullScreen::PrepareForFullScreen()
{
    AutoPtr<IMediaController> mc = new FullScreenMediaController(mProxy->GetContext(), mLayout);
    Int32 visibility;
    mLayout->GetSystemUiVisibility(&visibility);
    mc->SetSystemUiVisibility(visibility);
    SetMediaController(mc);
    mPlayer->SetScreenOnWhilePlaying(TRUE);
    mPlayer->SetOnVideoSizeChangedListener(mSizeChangedListener);
    PrepareDataAndDisplayMode(mProxy);
}

void HTML5VideoFullScreen::ToggleMediaControlsVisiblity()
{
    Boolean bFlag = FALSE;
    mMediaController->IsShowing(&bFlag);
    if (bFlag) {
        mMediaController->Hide();
    }
    else {
        mMediaController->Show();
    }
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
