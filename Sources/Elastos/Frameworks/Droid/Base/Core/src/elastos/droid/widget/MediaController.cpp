
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/MediaController.h"
#include "elastos/droid/widget/CFrameLayoutLayoutParams.h"
#include "elastos/droid/view/CWindowManagerLayoutParams.h"
#include "elastos/droid/impl/CPolicyManager.h"
#include "elastos/droid/R.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringBuilder.h>

using Elastos::Core::CStringWrapper;
using Elastos::Core::StringBuilder;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Internal::Policy::IPolicyManager;
using Elastos::Droid::Internal::Policy::CPolicyManager;
using Elastos::Droid::Media::IAudioManager;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::EIID_IViewOnTouchListener;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::CWindowManagerLayoutParams;

namespace Elastos {
namespace Droid {
namespace Widget {

const Int32 MediaController::sDefaultTimeout;
const Int32 MediaController::FADE_OUT;
const Int32 MediaController::SHOW_PROGRESS;
const String MediaController::MEDIACONTROLLER_NAME = String("MediaController");

CAR_INTERFACE_IMPL(MediaController::MediaControllerOnTouchListener, IViewOnTouchListener)
CAR_INTERFACE_IMPL(MediaController::MediaControllerOnLayoutChangeListener, IViewOnTouchListener)
CAR_INTERFACE_IMPL(MediaController::PauseOnClickListener, IViewOnTouchListener)
CAR_INTERFACE_IMPL(MediaController::FfwdOnClickListener, IViewOnTouchListener)
CAR_INTERFACE_IMPL(MediaController::RewOnClickListener, IViewOnTouchListener)
CAR_INTERFACE_IMPL(MediaController::MediaControllerOnSeekBarChangeListener, IViewOnTouchListener)

ECode MediaController::MyHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);

    switch(what) {
        case MediaController::FADE_OUT:
            mHost->Hide();
            break;
        case MediaController::SHOW_PROGRESS: {
            mHost->HandleShowProgress();
            break;
        }
    }

    return NOERROR;
}

MediaController::MediaController()
    : mShowing(FALSE)
    , mDragging(FALSE)
    , mUseFastForward(FALSE)
    , mFromXml(FALSE)
    , mListenersSet(FALSE)
{
    mLayoutChangeListener = new MediaControllerOnLayoutChangeListener(this);
    mTouchListener = new MediaControllerOnTouchListener(this);
    mHandler = new MyHandler(this);
    mPauseListener = new PauseOnClickListener(this);
    mSeekListener = new MediaControllerOnSeekBarChangeListener(this);
    mRewListener = new RewOnClickListener(this);
    mFfwdListener = new FfwdOnClickListener(this);
}

MediaController::MediaController(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs)
    : mShowing(FALSE)
    , mDragging(FALSE)
    , mUseFastForward(FALSE)
    , mFromXml(FALSE)
    , mListenersSet(FALSE)
{
    mLayoutChangeListener = new MediaControllerOnLayoutChangeListener(this);
    mTouchListener = new MediaControllerOnTouchListener(this);
    mHandler = new MyHandler(this);
    mPauseListener = new PauseOnClickListener(this);
    mSeekListener = new MediaControllerOnSeekBarChangeListener(this);
    mRewListener = new RewOnClickListener(this);
    mFfwdListener = new FfwdOnClickListener(this);
    Init(context, attrs);
}

MediaController::MediaController(
        /* [in] */ IContext* context,
        /* [in] */ Boolean useFastForWard)
    : mShowing(FALSE)
    , mDragging(FALSE)
    , mUseFastForward(FALSE)
    , mFromXml(FALSE)
    , mListenersSet(FALSE)
{
    mLayoutChangeListener = new MediaControllerOnLayoutChangeListener(this);
    mTouchListener = new MediaControllerOnTouchListener(this);
    mHandler = new MyHandler(this);
    mPauseListener = new PauseOnClickListener(this);
    mSeekListener = new MediaControllerOnSeekBarChangeListener(this);
    mRewListener = new RewOnClickListener(this);
    mFfwdListener = new FfwdOnClickListener(this);
    Init(context, useFastForWard);
}

MediaController::~MediaController()
{
}

ECode MediaController::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    FrameLayout::Init(context, attrs);
    mRoot = (IView*)this->Probe(EIID_IView);
    mContext = context;
    mUseFastForward = TRUE;
    mFromXml = TRUE;
    return NOERROR;
}

ECode MediaController::Init(
    /* [in] */ IContext* context,
    /* [in] */ Boolean useFastForWard)
{
    FrameLayout::Init(context);
    mContext = context;
    mUseFastForward = useFastForWard;
    InitFloatingWindowLayout();
    InitFloatingWindow();
    return NOERROR;
}

ECode MediaController::OnFinishInflate()
{
    if (mRoot) {
        InitControllerView(mRoot);
    }
    return NOERROR;
}

ECode MediaController::SetMediaPlayer(
        /* [in] */ IMediaPlayerControl* player)
{
    mPlayer = player;
    UpdatePausePlay();
    return NOERROR;
}

ECode MediaController::SetAnchorView(
        /* [in] */ IView* view)
{
    AutoPtr<MediaControllerOnLayoutChangeListener> mLayoutChangeListener = new MediaControllerOnLayoutChangeListener(this);
    if(mAnchor) {
        mAnchor->RemoveOnLayoutChangeListener(mLayoutChangeListener);
    }
    mAnchor = view;
    if(mAnchor) {
        mAnchor->AddOnLayoutChangeListener(mLayoutChangeListener);
    }
    AutoPtr<IFrameLayoutLayoutParams> frameParams;
    FAIL_RETURN(CFrameLayoutLayoutParams::New(IViewGroupLayoutParams::MATCH_PARENT,
         IViewGroupLayoutParams::MATCH_PARENT, (IFrameLayoutLayoutParams**)&frameParams));
    RemoveAllViews();
    AutoPtr<IView> v = MakeControllerView();
    AddView(v, frameParams);
    return NOERROR;
}

ECode MediaController::Show()
{
    return Show(sDefaultTimeout);
}

ECode MediaController::Show(
        /* [in] */ Int32 timeout)
{
    if(!mShowing && mAnchor) {
        SetProgress();
        if(mPauseButton) {
            Boolean res = FALSE;
            mPauseButton->RequestFocus(&res);
        }

        DisableUnsupportedButtons();
        UpdateFloatingWindowLayout();
        mWindowManager->AddView(mDecor, mDecorLayoutParams);
        mShowing = TRUE;
    }
    UpdatePausePlay();

    // cause the progress bar to be updated even if mShowing
    // was already true.  This happens, for example, if we're
    // paused with the progress bar showing the user hits play.
    Boolean result;
    mHandler->SendEmptyMessage(SHOW_PROGRESS, &result);

    if (timeout != 0) {
        mHandler->RemoveMessages(FADE_OUT);
        mHandler->SendEmptyMessageDelayed(FADE_OUT, timeout, &result);
    }

    return NOERROR;
}

Boolean MediaController::IsShowing()
{
    return mShowing;
}

ECode MediaController::Hide()
{

    if (mAnchor == NULL)
        return NOERROR;

    if (mShowing) {
        //try {
        mHandler->RemoveMessages(SHOW_PROGRESS);
        mWindowManager->RemoveView(mDecor);
        //} catch (IllegalArgumentException ex) {
        //    Log.w("MediaController", "already removed");
        //}
        mShowing = FALSE;
    }

    return NOERROR;
}

Boolean MediaController::OnTouchEvent(
        /* [in] */ IMotionEvent* event)
{
    Show(sDefaultTimeout);
    return TRUE;
}

Boolean MediaController::OnTrackballEvent(
        /* [in] */ IMotionEvent* event)
{
    Show(sDefaultTimeout);
    return FALSE;
}

Boolean MediaController::DispatchKeyEvent(
        /* [in] */ IKeyEvent* event)
{
    Int32 keyCode = 0;
    event->GetKeyCode(&keyCode);
    Int32 repeatCount = 0;
    event->GetRepeatCount(&repeatCount);
    Int32 action = 0;
    event->GetAction(&action);
    Boolean uniqueDown = repeatCount == 0 && action == IKeyEvent::ACTION_DOWN;

    Boolean isPlaying = FALSE;
    mPlayer->IsPlaying(&isPlaying);

    if(keyCode == IKeyEvent::KEYCODE_HEADSETHOOK
        || keyCode == IKeyEvent::KEYCODE_MEDIA_PLAY_PAUSE
        || keyCode == IKeyEvent::KEYCODE_SPACE) {
        if(uniqueDown) {
            DoPauseResume();
            Show(sDefaultTimeout);
            if(mPauseButton) {
                Boolean res = FALSE;
                mPauseButton->RequestFocus(&res);
            }
        }
        return TRUE;
    } else if (keyCode == IKeyEvent::KEYCODE_MEDIA_PLAY_PAUSE) {
        if(uniqueDown && !isPlaying) {
            mPlayer->Start();
            UpdatePausePlay();
        }
        return TRUE;
    } else if (keyCode == IKeyEvent::KEYCODE_MEDIA_STOP
        || keyCode == IKeyEvent::KEYCODE_MEDIA_PAUSE) {
        if(uniqueDown && !isPlaying) {
            mPlayer->Pause();
            UpdatePausePlay();
            Show(sDefaultTimeout);
        }
        return TRUE;
    } else if (keyCode == IKeyEvent::KEYCODE_VOLUME_DOWN
        || keyCode == IKeyEvent::KEYCODE_VOLUME_UP
        || keyCode == IKeyEvent::KEYCODE_VOLUME_MUTE
        || keyCode == IKeyEvent::KEYCODE_CAMERA) {
        return FrameLayout::DispatchKeyEvent(event);
    } else if (keyCode == IKeyEvent::KEYCODE_BACK
        || keyCode == IKeyEvent::KEYCODE_MENU) {
        if(uniqueDown) {
            Hide();
        }
        return TRUE;
    }
    Show(sDefaultTimeout);
    return FrameLayout::DispatchKeyEvent(event);
}

ECode MediaController::SetEnabled(
        /* [in] */ Boolean enabled)
{
    if (mPauseButton) {
        mPauseButton->SetEnabled(enabled);
    }
    if (mFfwdButton) {
        mFfwdButton->SetEnabled(enabled);
    }
    if (mRewButton) {
        mRewButton->SetEnabled(enabled);
    }
    if (mNextButton) {
        mNextButton->SetEnabled(enabled && mNextListener != NULL);
    }
    if (mPrevButton) {
        mPrevButton->SetEnabled(enabled && mPrevListener != NULL);
    }
    if (mProgress) {
        mProgress->SetEnabled(enabled);
    }
    DisableUnsupportedButtons();
    return FrameLayout::SetEnabled(enabled);
}

ECode MediaController::OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event)
{
    FrameLayout::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(MEDIACONTROLLER_NAME, (ICharSequence**)&seq));
    event->SetClassName(seq);
    return NOERROR;
}

ECode MediaController::OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info)
{
    FrameLayout::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(MEDIACONTROLLER_NAME, (ICharSequence**)&seq));
    info->SetClassName(seq);
    return NOERROR;
}


AutoPtr<IView> MediaController::MakeControllerView()
{
    mRoot = NULL;
    AutoPtr<ILayoutInflater> inflate;
    mContext->GetSystemService(
            IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&inflate);
    inflate->Inflate(R::layout::media_controller, NULL, (IView**)&mRoot);

    InitControllerView(mRoot);

    return mRoot;
}

ECode MediaController::InitFloatingWindow()
{
    FAIL_RETURN(mContext->GetSystemService(IContext::WINDOW_SERVICE, (IInterface**)&mWindowManager));
    AutoPtr<IPolicyManager> policy;
    FAIL_RETURN(CPolicyManager::AcquireSingleton((IPolicyManager**)&policy));
    FAIL_RETURN(policy->MakeNewWindow(mContext, (IWindow**)&mWindow));
    mWindow->SetWindowManager(mWindowManager, NULL, String(NULL));
    Boolean result = FALSE;
    mWindow->RequestFeature(IWindow::FEATURE_NO_TITLE, &result);
    FAIL_RETURN(mWindow->GetDecorView((IView**)&mDecor));
    AutoPtr<MediaControllerOnTouchListener> mTouchListener = new MediaControllerOnTouchListener(this);
    mDecor->SetOnTouchListener(mTouchListener);
    mWindow->SetContentView((IView*)this->Probe(EIID_IView));
    FAIL_RETURN(mWindow->SetBackgroundDrawableResource(R::color::transparent));

    mWindow->SetVolumeControlStream(IAudioManager::STREAM_MUSIC);

    SetFocusable(TRUE);
    SetFocusableInTouchMode(TRUE);
    SetDescendantFocusability(IViewGroup::FOCUS_AFTER_DESCENDANTS);
    View::RequestFocus();

    return NOERROR;
}

ECode MediaController::InitFloatingWindowLayout()
{
    FAIL_RETURN(CWindowManagerLayoutParams::New((
        IWindowManagerLayoutParams**)&mDecorLayoutParams));
    CWindowManagerLayoutParams* cp = (CWindowManagerLayoutParams*)mDecorLayoutParams.Get();
    cp->mGravity = IGravity::TOP;
    cp->mHeight = IViewGroupLayoutParams::WRAP_CONTENT;
    cp->SetX(0);
    cp->SetFormat(IPixelFormat::TRANSLUCENT);
    cp->mType = IWindowManagerLayoutParams::TYPE_APPLICATION_PANEL;
    cp->mFlags |= IWindowManagerLayoutParams::FLAG_ALT_FOCUSABLE_IM
        | IWindowManagerLayoutParams::FLAG_NOT_TOUCH_MODAL
        | IWindowManagerLayoutParams::FLAG_SPLIT_TOUCH;
    cp->mToken = NULL;
    cp->SetWindowAnimations(0);

    return NOERROR;
}

ECode MediaController::UpdateFloatingWindowLayout()
{
    Int32 anchorPos[2];
    mAnchor->GetLocationOnScreen(anchorPos, anchorPos + 1);
    CWindowManagerLayoutParams* cp = (CWindowManagerLayoutParams*)mDecorLayoutParams.Get();
    mAnchor->GetWidth(&(cp->mWidth));
    Int32 height = 0;
    mAnchor->GetHeight(&height);
    cp->SetY(anchorPos[1] + height);

    return NOERROR;
}

ECode MediaController::InitControllerView(
    /* [in] */ IView* view)
{
    mPauseButton = NULL;
    FAIL_RETURN(view->FindViewById(R::id::pause, (IView**)&mPauseButton));
    if (mPauseButton != NULL) {
        Boolean result = FALSE;
        mPauseButton->RequestFocus(&result);
        AutoPtr<PauseOnClickListener> mPauseListener = new PauseOnClickListener(this);
        mPauseButton->SetOnClickListener(mPauseListener);
    }

    mFfwdButton = NULL;
    FAIL_RETURN(view->FindViewById(R::id::ffwd, (IView**)&mFfwdButton));
    if (mFfwdButton != NULL) {
        AutoPtr<FfwdOnClickListener> mFfwdListener = new FfwdOnClickListener(this);
        mFfwdButton->SetOnClickListener(mFfwdListener);
        if (!mFromXml) {
            mFfwdButton->SetVisibility(mUseFastForward ? IView::VISIBLE : IView::GONE);
        }
    }

    mRewButton = NULL;
    FAIL_RETURN(view->FindViewById(R::id::rew, (IView**)&mRewButton));
    if (mRewButton != NULL) {
        AutoPtr<RewOnClickListener> mRewListener = new RewOnClickListener(this);
        mRewButton->SetOnClickListener(mRewListener);
        if (!mFromXml) {
            mRewButton->SetVisibility(mUseFastForward ? IView::VISIBLE : IView::GONE);
        }
    }

    mNextButton = NULL;
    FAIL_RETURN(view->FindViewById(R::id::next, (IView**)&mNextButton));
    if (mNextButton != NULL && !mFromXml && !mListenersSet) {
        mNextButton->SetVisibility(IView::GONE);
    }

    mPrevButton = NULL;
    FAIL_RETURN(view->FindViewById(R::id::prev, (IView**)&mPrevButton));
    if (mPrevButton != NULL && !mFromXml && !mListenersSet) {
        mPrevButton->SetVisibility(IView::GONE);
    }

    mProgress = NULL;
    FAIL_RETURN(view->FindViewById(R::id::mediacontroller_progress, (IView**)&mProgress));
    if (mProgress != NULL) {
        ISeekBar* pSeekBar = (ISeekBar*)mProgress->Probe(EIID_ISeekBar);
        if (pSeekBar) {
            AutoPtr<MediaControllerOnSeekBarChangeListener> mSeekListener = new MediaControllerOnSeekBarChangeListener(this);
            pSeekBar->SetOnSeekBarChangeListener(mSeekListener);
        }
        mProgress->SetMax(1000);
    }

    mEndTime = NULL;
    mCurrentTime = NULL;
    FAIL_RETURN(view->FindViewById(R::id::time, (IView**)&mEndTime));
    FAIL_RETURN(view->FindViewById(R::id::time_current, (IView**)&mCurrentTime));
    mFormatBuilder = new StringBuilder();
    //assert(0 && "TODO");
    //mFormatter = new Formatter(mFormatBuilder, Local.getDefault());

    InstallPrevNextListeners();

    return NOERROR;
}

ECode MediaController::DisableUnsupportedButtons()
{
    //try{
        Boolean canPause = FALSE;
        mPlayer->CanPause(&canPause);
        if(mPauseButton && !canPause) {
            mPauseButton->SetEnabled(FALSE);
        }
        Boolean canSeekBackward = FALSE;
        mPlayer->CanSeekBackward(&canSeekBackward);
        if(mRewButton && !canSeekBackward) {
            mRewButton->SetEnabled(FALSE);
        }
        Boolean canSeekForward = FALSE;
        mPlayer->CanSeekForward(&canSeekForward);
        if(mFfwdButton && !canSeekForward) {
            mFfwdButton->SetEnabled(FALSE);
        }
    //} catch {

    //}
    return NOERROR;
}

String MediaController::StringForTime(
        /* [in] */ Int32 timeMs)
{
    Int32 totalSeconds = timeMs / 1000;
    Int32 seconds = totalSeconds % 60;
    Int32 minutes = (totalSeconds / 60) % 60;
    Int32 hours = totalSeconds / 3600;

    String result("");
    if (hours > 0) {
        result.AppendFormat("%d", hours);
        result += ":";
        result.AppendFormat("%2d", minutes);
        result += ":";
        result.AppendFormat("%2d", seconds);
    }
    else {
        result.AppendFormat("%2d", minutes);
        result += ":";
        result.AppendFormat("%2d", seconds);
    }

    return result;
}

Int32 MediaController::SetProgress()
{
    if (mPlayer == NULL || mDragging) {
        return 0;
    }
    Int32 position, duration;
    mPlayer->GetCurrentPosition(&position);
    mPlayer->GetDuration(&duration);
    if (mProgress != NULL) {
        if (duration > 0) {
            Int32 pos = 1000L * position / duration;
            mProgress->SetProgress(pos);
        }
        Int32 percent;
        mPlayer->GetBufferPercentage(&percent);
        mProgress->SetSecondaryProgress(percent * 10);
    }

    if (mEndTime != NULL) {
        AutoPtr<ICharSequence> charS;
        ASSERT_SUCCEEDED(CStringWrapper::New(
                StringForTime(duration), (ICharSequence**)&charS));
        mEndTime->SetText(charS);
    }
    if (mCurrentTime != NULL) {
        AutoPtr<ICharSequence> charS;
        ASSERT_SUCCEEDED(CStringWrapper::New(
                StringForTime(position), (ICharSequence**)&charS));
        mCurrentTime->SetText(charS);
    }

    return 0;
}

ECode MediaController::UpdatePausePlay()
{
    if(!mRoot || !mPauseButton) {
        return NOERROR;
    }
    Boolean isPlaying = FALSE;
    mPlayer->IsPlaying(&isPlaying);
    if(isPlaying) {
        mPauseButton->SetImageResource(R::drawable::ic_media_pause);
    } else {
        mPauseButton->SetImageResource(R::drawable::ic_media_play);
    }
    return NOERROR;
}

ECode MediaController::DoPauseResume()
{
    Boolean isPlaying = FALSE;
    mPlayer->IsPlaying(&isPlaying);
    if (isPlaying) {
        mPlayer->Pause();
    }
    else {
        mPlayer->Start();
    }
    UpdatePausePlay();

    return NOERROR;
}

ECode MediaController::InstallPrevNextListeners()
{
    if (mNextButton != NULL) {
        mNextButton->SetOnClickListener(mNextListener);
        mNextButton->SetEnabled(mNextListener != NULL);
    }

    if (mPrevButton != NULL) {
        mPrevButton->SetOnClickListener(mPrevListener);
        mPrevButton->SetEnabled(mPrevListener != NULL);
    }
    return NOERROR;
}

ECode MediaController::SetPrevNextListeners(
        /* [in] */ IViewOnClickListener* next,
        /* [in] */ IViewOnClickListener* prev)
{
    mNextListener = next;
    mPrevListener = prev;
    mListenersSet = TRUE;

    if (mRoot != NULL) {
        InstallPrevNextListeners();

        if (mNextButton != NULL && !mFromXml) {
            mNextButton->SetVisibility(IView::VISIBLE);
        }
        if (mPrevButton != NULL && !mFromXml) {
            mPrevButton->SetVisibility(IView::VISIBLE);
        }
    }

    return NOERROR;
}

ECode MediaController::HandleShowProgress()
{
    Int32 pos = SetProgress();
    Boolean isPlaying = FALSE;
    if (!mDragging && mShowing && (mPlayer->IsPlaying(&isPlaying), isPlaying)) {
        Boolean result;
        mHandler->SendEmptyMessageDelayed(SHOW_PROGRESS, 1000 - (pos % 1000), &result);
    }
    return NOERROR;
}

//==============================================================================================
//          MediaController::MediaControllerOnTouchListener
//==============================================================================================

MediaController::MediaControllerOnTouchListener::MediaControllerOnTouchListener(
    /* [in] */ MediaController* host) :
    host(host)
{
}

ECode MediaController::MediaControllerOnTouchListener::OnTouch(
    /* [in] */ IView* v,
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* res)
{
    Int32 action = 0;
    event->GetAction(&action);
    if(action == IMotionEvent::ACTION_DOWN) {
        if(host->mShowing) {
            host->Hide();
        }
    }
    *res = FALSE;
    return NOERROR;
}

//==============================================================================================
//          MediaController::MediaControllerOnLayoutChangeListener
//==============================================================================================

MediaController::MediaControllerOnLayoutChangeListener::MediaControllerOnLayoutChangeListener(
    /* [in] */ MediaController* host) :
    host(host)
{

}

ECode MediaController::MediaControllerOnLayoutChangeListener::OnLayoutChange(
    /* [in] */ IView* v,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [in] */ Int32 oldLeft,
    /* [in] */ Int32 oldTop,
    /* [in] */ Int32 oldRight,
    /* [in] */ Int32 oldBottom)
{
    host->UpdateFloatingWindowLayout();
    if(host->mShowing) {
        host->mWindowManager->UpdateViewLayout(host->mDecor, host->mDecorLayoutParams);
    }
    return NOERROR;
}

//==============================================================================================
//          MediaController::PauseOnClickListener
//==============================================================================================

MediaController::PauseOnClickListener::PauseOnClickListener(
    /* [in] */ MediaController* host) :
    host(host)
{

}

ECode MediaController::PauseOnClickListener::OnClick(
    /* [in] */ IView* v)
{
    host->DoPauseResume();
    host->Show(host->sDefaultTimeout);
    return NOERROR;
}

//==============================================================================================
//          MediaController::FfwdOnClickListener
//==============================================================================================

MediaController::FfwdOnClickListener::FfwdOnClickListener(
    /* [in] */ MediaController* host) :
    host(host)
{

}

ECode MediaController::FfwdOnClickListener::OnClick(
    /* [in] */ IView* v)
{
    Int32 pos = 0;
    host->mPlayer->GetCurrentPosition(&pos);
    pos += 15000;
    host->mPlayer->SeekTo(pos);
    host->SetProgress();

    host->Show(host->sDefaultTimeout);
    return NOERROR;
}

//==============================================================================================
//          MediaController::RewOnClickListener
//==============================================================================================

MediaController::RewOnClickListener::RewOnClickListener(
    /* [in] */ MediaController* host) :
    host(host)
{

}

ECode MediaController::RewOnClickListener::OnClick(
    /* [in] */ IView* v)
{
    Int32 pos = 0;
    host->mPlayer->GetCurrentPosition(&pos);
    pos -= 5000;
    host->mPlayer->SeekTo(pos);
    host->SetProgress();

    host->Show(host->sDefaultTimeout);
    return NOERROR;
}

//==============================================================================================
//          MediaController::MediaControllerOnSeekBarChangeListener
//==============================================================================================

MediaController::MediaControllerOnSeekBarChangeListener::MediaControllerOnSeekBarChangeListener(
    /* [in] */ MediaController* host) :
    host(host)
{
}

ECode MediaController::MediaControllerOnSeekBarChangeListener::OnStartTrackingTouch(
    /* [in] */ ISeekBar* seekBar)
{
    host->Show(3600000);
    host->mDragging = TRUE;
    host->mHandler->RemoveMessages(MediaController::SHOW_PROGRESS);
    return NOERROR;
}

ECode MediaController::MediaControllerOnSeekBarChangeListener::OnProgressChanged(
    /* [in] */ ISeekBar* seekBar,
    /* [in] */ Int32 progress,
    /* [in] */ Boolean fromUser)
{
    if(!fromUser) {
        return NOERROR;
    }
    Int32 duration = 0;
    host->mPlayer->GetDuration(&duration);
    Int64 newPosition = (duration * progress) / 1000L;
    host->mPlayer->SeekTo((Int32)newPosition);
    if(host->mCurrentTime) {
        AutoPtr<ICharSequence> csq;
        CStringWrapper::New(host->StringForTime(Int32(newPosition)), (ICharSequence**)&csq);
        host->mCurrentTime->SetText(csq);
    }
    return NOERROR;
}

ECode MediaController::MediaControllerOnSeekBarChangeListener::OnStopTrackingTouch(
    /* [in] */ ISeekBar* seekBar)
{
    host->mDragging = FALSE;
    host->SetProgress();
    host->UpdatePausePlay();
    host->Show(sDefaultTimeout);

    Boolean result;
    host->mHandler->SendEmptyMessage(MediaController::SHOW_PROGRESS, &result);
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
