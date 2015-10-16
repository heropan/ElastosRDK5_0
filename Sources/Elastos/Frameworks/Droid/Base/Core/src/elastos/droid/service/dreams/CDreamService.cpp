
#include "elastos/droid/media/CDreamService.h"
#include <elastos/utility/logging/Slogger.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;
using namespace Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Service {
namespace Dreams {

const String CDreamService::TAG  = DreamService.class.getSimpleName() + "[" + getClass().getSimpleName() + "]";

CDreamService::CDreamService()
    : mHandler(NULL)
    , mWindowToken(NULL)
    , mWindow(NULL)
    , mWindowManager(NULL)
    , mSandman(NULL)
    , mInteractive(FALSE)
    , mLowProfile(TRUE)
    , mFullscreen(FALSE)
    , mScreenBright(TRUE)
    , mFinished(FALSE)
    , mDebug(FALSE)
{}

ECode CDreamService::SetDebug(
    /* [in] */ Boolean dbg)
{
    mDebug = dbg;
    return NOERROR;
}

ECode CDreamService::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // TODO: create more flexible version of mInteractive that allows use of KEYCODE_BACK
    if (!mInteractive) {
        if (mDebug)
        {
            Slogger::V(TAG, "Finishing on keyEvent");
        }
        SafelyFinish();
        *result = TRUE;
        return NOERROR;
    }
    else if (event->GetKeyCode() == KeyEvent::KEYCODE_BACK) {
        if (mDebug)
        {
            Slogger::V(TAG, "Finishing on back key");
        }
        SafelyFinish();
        *result = TRUE;
        return NOERROR;
    }
    *result = mWindow->SuperDispatchKeyEvent(event);
    return NOERROR;
}

ECode CDreamService::DispatchKeyShortcutEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (!mInteractive) {
        if (mDebug)
        {
            Slogger::V(TAG, "Finishing on keyShortcutEvent");
        }
        SafelyFinish();
        *result = TRUE;
        return NOERROR;
    }
    *result = mWindow->SuperDispatchKeyShortcutEvent(event);
    return NOERROR;
}

ECode CDreamService::DispatchTouchEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // TODO: create more flexible version of mInteractive that allows clicks
    // but finish()es on any other kind of activity
    if (!mInteractive) {
        if (mDebug)
        {
            Slogger::V(TAG, "Finishing on touchEvent");
        }
        SafelyFinish();
        *result = TRUE;
        return NOERROR;
    }
    *result = mWindow->SuperDispatchTouchEvent(event);
    return NOERROR;
}

ECode CDreamService::DispatchTrackballEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (!mInteractive) {
        if (mDebug)
        {
            Slogger::V(TAG, "Finishing on trackballEvent");
        }
        SafelyFinish();
        *result = TRUE;
        return NOERROR;
    }
    *result = mWindow->SuperDispatchTrackballEvent(event);
    return NOERROR;
}

ECode CDreamService::DispatchGenericMotionEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (!mInteractive) {
        if (mDebug)
        {
            Slogger::V(TAG, "Finishing on genericMotionEvent");
        }
        SafelyFinish();
        *result = TRUE;
        return NOERROR;
    }
    *result = mWindow->SuperDispatchGenericMotionEvent(event);
    return NOERROR;
}

ECode CDreamService::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode CDreamService::OnCreatePanelView(
    /* [in] */ Int32 featureId,
    /* [out] */ IView** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;
    return NOERROR;
}

ECode CDreamService::OnCreatePanelMenu(
    /* [in] */ Int32 featureId,
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode CDreamService::OnPreparePanel(
    /* [in] */ Int32 featureId,
    /* [in] */ IView* view,
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode CDreamService::OnMenuOpened(
    /* [in] */ Int32 featureId,
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode CDreamService::OnMenuItemSelected(
    /* [in] */ Int32 featureId,
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode CDreamService::OnWindowAttributesChanged(
    /* [in] */ ILayoutParams* attrs)
{
    return NOERROR;
}

ECode CDreamService::OnContentChanged()
{
    return NOERROR;
}

ECode CDreamService::OnWindowFocusChanged(
    /* [in] */ Boolean hasFocus)
{
    return NOERROR;
}

ECode CDreamService::OnAttachedToWindow()
{
    return NOERROR;
}

ECode CDreamService::OnDetachedFromWindow()
{
    return NOERROR;
}

ECode CDreamService::OnPanelClosed(
    /* [in] */ Int32 featureId,
    /* [in] */ IMenu* menu)
{
    return NOERROR;
}

ECode CDreamService::OnSearchRequested(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode CDreamService::OnWindowStartingActionMode(
    /* [in] */ ActionMode.Callback callback,
    /* [out] */ IActionMode** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;
    return NOERROR;
}

ECode CDreamService::OnActionModeStarted(
    /* [in] */ IActionMode* mode)
{
    return NOERROR;
}

ECode CDreamService::OnActionModeFinished(
    /* [in] */ IActionMode* mode)
{
    return NOERROR;
}

ECode CDreamService::GetWindowManager(
    /* [out] */ IWindowManager** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mWindowManager;
    return NOERROR;
}

ECode CDreamService::GetWindow(
    /* [out] */ IWindow** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mWindow;
    return NOERROR;
}

ECode CDreamService::SetContentView(
    /* [in] */ Int32 layoutResID)
{
    GetWindow()->SetContentView(layoutResID);
    return NOERROR;
}

ECode CDreamService::SetContentView(
    /* [in] */ IView* view)
{
    GetWindow()->SetContentView(view);
    return NOERROR;
}

ECode CDreamService::SetContentView(
    /* [in] */ IView* view,
    /* [in] */ IViewGroup.LayoutParams* params)
{
    GetWindow()->SetContentView(view, params);
    return NOERROR;
}

ECode CDreamService::AddContentView(
    /* [in] */ IView* view,
    /* [in] */ IViewGroup.LayoutParams* params)
{
    GetWindow()->AddContentView(view, params);
    return NOERROR;
}

ECode CDreamService::FindViewById(
    /* [in] */ Int32 id,
    /* [out] */ IView** result)
{
    VALIDATE_NOT_NULL(result);
    *result = GetWindow()->FindViewById(id);
    return NOERROR;
}

ECode CDreamService::SetInteractive(
    /* [in] */ Boolean interactive)
{
    mInteractive = interactive;
    return NOERROR;
}

ECode CDreamService::IsInteractive(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mInteractive;
    return NOERROR;
}

ECode CDreamService::SetLowProfile(
    /* [in] */ Boolean lowProfile)
{
    mLowProfile = lowProfile;
    Int32 flag = View::SYSTEM_UI_FLAG_LOW_PROFILE;
    ApplySystemUiVisibilityFlags(mLowProfile ? flag : 0, flag);
    return NOERROR;
}

ECode CDreamService::IsLowProfile(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = GetSystemUiVisibilityFlagValue(View::SYSTEM_UI_FLAG_LOW_PROFILE, mLowProfile);
    return NOERROR;
}

ECode CDreamService::SetFullscreen(
    /* [in] */ Boolean fullscreen)
{
    mFullscreen = fullscreen;
    Int32 flag = WindowManager::LayoutParams::FLAG_FULLSCREEN;
    ApplyWindowFlags(mFullscreen ? flag : 0, flag);
    return NOERROR;
}

ECode CDreamService::IsFullscreen(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mFullscreen;
    return NOERROR;
}

ECode CDreamService::SetScreenBright(
    /* [in] */ Boolean screenBright)
{
    mScreenBright = screenBright;
    Int flag = WindowManager::LayoutParams::FLAG_KEEP_SCREEN_ON;
    ApplyWindowFlags(mScreenBright ? flag : 0, flag);
    return NOERROR;
}

ECode CDreamService::IsScreenBright(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = GetWindowFlagValue(WindowManager::LayoutParams::FLAG_KEEP_SCREEN_ON, mScreenBright);
    return NOERROR;
}

ECode CDreamService::OnCreate()
{
    if (mDebug){
        Slogger::V(TAG, "onCreate() on thread " + Thread.currentThread().getId());
    }
    // super.onCreate();
    return NOERROR;
}

ECode CDreamService::OnDreamingStarted()
{
    if (mDebug){
     Slogger::V(TAG, "onDreamingStarted()");
    }
    // hook for subclasses
    return NOERROR;
}

ECode CDreamService::OnDreamingStopped()
{
    if (mDebug){
     Slogger::V(TAG, "onDreamingStopped()");
    }
    // hook for subclasses
    return NOERROR;
}

ECode CDreamService::OnBind(
    /* [in] */ Intent intent,
    /* [out] */ IIBinder** result)
{
    VALIDATE_NOT_NULL(result);
    if (mDebug){
        Slogger::V(TAG, "onBind() intent = " + intent);
    }
    *result = new DreamServiceWrapper();
    return NOERROR;
}

ECode CDreamService::Finish()
{
    if (mDebug){
     Slogger::V(TAG, "finish()");
    }
    FinishInternal();
    return NOERROR;
}

ECode CDreamService::OnDestroy()
{
    if (mDebug){
     Slogger::V(TAG, "onDestroy()");
    }
    // hook for subclasses

    // Just in case destroy came in before detach, let's take care of that now
    Detach();

    super->OnDestroy();
    return NOERROR;
}

void CDreamService::LoadSandman()
{
    mSandman = IDreamManager::Stub->AsInterface(ServiceManager->GetService(DREAM_SERVICE));
}

void CDreamService::Detach()
{
    if (mWindow == NULL) {
        // already detached!
        return;
    }

    // try {
        OnDreamingStopped();
    // } catch (Throwable t) {
        // Slog.w(TAG, "Crashed in onDreamingStopped()", t);
        // we were going to stop anyway
    // }

    if (mDebug)
    {
        Slogger::V(TAG, "detach(): Removing window from window manager");
    }
    // try {
        // force our window to be removed synchronously
        mWindowManager->RemoveViewImmediate(mWindow->GetDecorView());
        // the following will print a log message if it finds any other leaked windows
        WindowManagerGlobal->GetInstance()->CloseAll(mWindowToken, this->GetClass()->GetName(), "Dream");
    // } catch (Throwable t) {
        // Slog.w(TAG, "Crashed removing window view", t);
    // }

    mWindow = NULL;
    mWindowToken = NULL;
}

void CDreamService::Attach(
    /* [in] */ IIBinder* windowToken)
{
    if (mWindowToken != NULL) {
        Slogger::E(TAG, "attach() called when already attached with token=" + mWindowToken);
        return;
    }

    if (mDebug){
        Slogger::V(TAG, "Attached on thread " + Thread.currentThread().getId());
    }

    if (mSandman == NULL) {
        LoadSandman();
    }
    mWindowToken = windowToken;
    mWindow = PolicyManager->MakeNewWindow(this);
    mWindow->SetCallback(this);
    mWindow->RequestFeature(Window::FEATURE_NO_TITLE);
    AutoPtr<ColorDrawable> cd = new ColorDrawable(0xFF000000);
    mWindow->SetBackgroundDrawable(cd);
    mWindow->SetFormat(PixelFormat::OPAQUE);

    if (mDebug){
        Slogger::V(TAG, String.format("Attaching window token: %s to window of type %s",windowToken, WindowManager.LayoutParams.TYPE_DREAM));
    }
    WindowManager::LayoutParams lp = mWindow->GetAttributes();
    lp::type = WindowManager::LayoutParams::TYPE_DREAM;
    lp::token = windowToken;
    lp::windowAnimations = com.android.internal.R.style.Animation_Dream;
    lp::flags |= ( WindowManager::LayoutParams::FLAG_LAYOUT_IN_SCREEN
                | WindowManager::LayoutParams::FLAG_LAYOUT_INSET_DECOR
                | WindowManager::LayoutParams::FLAG_SHOW_WHEN_LOCKED
                | WindowManager::LayoutParams::FLAG_DISMISS_KEYGUARD
                | WindowManager::LayoutParams::FLAG_ALLOW_LOCK_WHILE_SCREEN_ON
                | (mFullscreen ? WindowManager::LayoutParams::FLAG_FULLSCREEN : 0)
                | (mScreenBright ? WindowManager::LayoutParams::FLAG_KEEP_SCREEN_ON : 0)
                );
    mWindow->SetAttributes(lp);

    if (mDebug){
        Slogger::V(TAG, "Created and attached window: " + mWindow);
    }

    mWindow->SetWindowManager(NULL, windowToken, "dream", TRUE);
    mWindowManager = mWindow->GetWindowManager();

    if (mDebug){
        Slogger::V(TAG, "Window added on thread " + Thread->CurrentThread()->GetId());
    }
    // try {
        ApplySystemUiVisibilityFlags(
                (mLowProfile ? View.SYSTEM_UI_FLAG_LOW_PROFILE : 0),
                View.SYSTEM_UI_FLAG_LOW_PROFILE);
        GetWindowManager()->AddView(mWindow->GetDecorView(), mWindow->GetAttributes());
    // } catch (Throwable t) {
        Slogger::W(TAG, "Crashed adding window view", t);
        SafelyFinish();
        return;
    // }

    // start it up
    AutoPtr<IRunnable> runnable = new Runnable() {
        @Override
        public void run() {
            // try {
                OnDreamingStarted();
            // } catch (Throwable t) {
                // Slog.w(TAG, "Crashed in onDreamingStarted()", t);
                // SafelyFinish();
            // }
        }
    };
    mHandler->Post(runnable);
}

void CDreamService::SafelyFinish()
{
    if (mDebug)
    {
     Slogger::V(TAG, "safelyFinish()");
    }
    // try {
        Finish();
    // } catch (Throwable t) {
        // Slog.w(TAG, "Crashed in safelyFinish()", t);
        // FinishInternal();
        return;
    // }

    if (!mFinished) {
        Slogger::W(TAG, "Bad dream, did not call super.finish()");
        FinishInternal();
    }
}

void CDreamService::FinishInternal()
{
    if (mDebug)
        {
            Slogger::V(TAG, "finishInternal() mFinished = " + mFinished);
        }
    if (mFinished) return;
    // try {
        mFinished = TRUE;

        if (mSandman != NULL) {
            mSandman->FinishSelf(mWindowToken);
        }
        else {
            Slogger::W(TAG, "No dream manager found");
        }
        StopSelf(); // if launched via any other means

    // } catch (Throwable t) {
        // Slog.w(TAG, "Crashed in finishInternal()", t);
    // }
}

Boolean CDreamService::GetWindowFlagValue(
    /* [in] */ Int32 flag,
    /* [in] */ Boolean defaultValue)
{
    return mWindow == NULL ? defaultValue : (mWindow->GetAttributes().flags & flag) != 0;
}

void CDreamService::ApplyWindowFlags(
    /* [in] */ Int32 flags,
    /* [in] */ Int32 mask)
{
    if (mWindow != NULL) {
        WindowManager::LayoutParams lp = mWindow->GetAttributes();
        lp::flags = ApplyFlags(lp::flags, flags, mask);
        mWindow->SetAttributes(lp);
        mWindowManager->UpdateViewLayout(mWindow->GetDecorView(), lp);
    }
}

Boolean CDreamService::GetSystemUiVisibilityFlagValue(
    /* [in] */ Int32 flag,
    /* [in] */ Boolean defaultValue)
{
    AutoPtr<IView> v = mWindow == NULL ? NULL : mWindow->GetDecorView();
    return v == NULL ? defaultValue : (v->GetSystemUiVisibility() & flag) != 0;
}

void CDreamService::ApplySystemUiVisibilityFlags(
    /* [in] */ Int32 flags,
    /* [in] */ Int32 mask)
{
    AutoPtr<IView> v = mWindow == NULL ? NULL : mWindow->GetDecorView();
    if (v != NULL) {
        v->SetSystemUiVisibility(ApplyFlags(v->GetSystemUiVisibility(), flags, mask));
    }
}

Int32 CDreamService::ApplyFlags(
    /* [in] */ Int32 oldFlags,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 mask)
{
    return (oldFlags&~mask) | (flags&mask);
}

void CDreamService::Dump(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* pw,
    /* [in] */ ArrayOf<String> args)
{
    super->Dump(fd, pw, args);

    pw->Print(TAG + ": ");
    if (mWindowToken == null) {
        pw->Println("stopped");
    } else {
        pw->Println("running (token=" + mWindowToken + ")");
    }
    pw->Println("  window: " + mWindow);
    pw->Print("  flags:");
    if (IsInteractive()) pw->Print(" interactive");
    if (IsLowProfile()) pw->Print(" lowprofile");
    if (IsFullscreen()) pw->Print(" fullscreen");
    if (IsScreenBright()) pw->Print(" bright");
    pw->Println();
}

ECode CDreamService::DreamServiceWrapper::Attach(
    /* [in] */ const IIBinder* windowToken)
{
    /*
    mHandler.post(new Runnable() {
        @Override
        public void run() {
            DreamService.this.attach(windowToken);
        }
    });
    */
}

ECode CDreamService::DreamServiceWrapper::Detach()
{
    /*
    mHandler.post(new Runnable() {
        @Override
        public void run() {
            DreamService.this.detach();
        }
    });
    */
}

} // namespace Dreams
} // namespace Service
} // namepsace Droid
} // namespace Elastos
