
#include "elastos/droid/inputmethodservice/CSoftInputWindow.h"
// #include "elastos/droid/view/CWindowManagerLayoutParams.h"

using Elastos::Core::CString;
using Elastos::Core::ICharSequence;
// using Elastos::Droid::View::CWindowManagerLayoutParams;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::IInputEvent;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IWindow;
using Elastos::Droid::View::IViewGroupLayoutParams;

namespace Elastos {
namespace Droid {
namespace InputMethodService {

CAR_OBJECT_IMPL(CSoftInputWindow);
CAR_INTERFACE_IMPL(CSoftInputWindow, /*Dialog*/Object, ISoftInputWindow);
CSoftInputWindow::CSoftInputWindow()
    : mWindowType(0)
    , mGravity(0)
    , mTakesFocus(FALSE)
{
    // CRect::New((IRect**)&mBounds);
    assert(0 && "TODO");
}

ECode CSoftInputWindow::SetToken(
    /* [in] */ IBinder* token)
{
    assert(0 && "TODO");
    AutoPtr<IWindow> window/* = GetWindow()*/;
    AutoPtr<IWindowManagerLayoutParams> lp;
    window->GetAttributes((IWindowManagerLayoutParams**)&lp);
    lp->SetToken(token);
    return window->SetAttributes(lp);
}

ECode CSoftInputWindow::OnWindowFocusChanged(
    /* [in] */ Boolean hasFocus)
{
    // FAIL_RETURN(Dialog::OnWindowFocusChanged(hasFocus));
    assert(0 && "TODO");
    return mDispatcherState->Reset();
}

ECode CSoftInputWindow::DispatchTouchEvent(
    /* [in] */ IMotionEvent* ev,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IView> docorView;
    assert(0 && "TODO");
    // GetWindow()->GetDecorView((IView**)&docorView);
    docorView->GetHitRect(mBounds);

    Boolean withIn = FALSE;

    Int32 left = 0, top = 0, right = 0, bottom;
    mBounds->GetLeft(&left);
    mBounds->GetTop(&top);
    mBounds->GetRight(&right);
    mBounds->GetBottom(&bottom);
    if (ev->IsWithinBoundsNoHistory(left, top,
            right - 1, bottom - 1, &withIn), withIn) {
        assert(0 && "TODO");
        // return Dialog::DispatchTouchEvent(ev, result);
    } else {
        AutoPtr<IMotionEvent> temp;
        ev->ClampNoHistory(left, top,
                right - 1, bottom - 1, (IMotionEvent**)&temp);

        assert(0 && "TODO");
        // Dialog::DispatchTouchEvent(temp, result);
        IInputEvent::Probe(temp)->Recycle();
    }

    return NOERROR;
}

ECode CSoftInputWindow::SetGravity(
    /* [in] */ Int32 gravity)
{
    assert(0 && "TODO");
    AutoPtr<IWindow> window/* = GetWindow()*/;
    AutoPtr<IWindowManagerLayoutParams> lp;
    window->GetAttributes((IWindowManagerLayoutParams**)&lp);
    lp->SetGravity(gravity);
    UpdateWidthHeight(lp);
    return window->SetAttributes(lp);
}

ECode CSoftInputWindow::GetGravity(
    /* [out] */ Int32* gravity)
{
    VALIDATE_NOT_NULL(gravity);
    assert(0 && "TODO");
    AutoPtr<IWindow> window/* = GetWindow()*/;
    AutoPtr<IWindowManagerLayoutParams> lp;
    window->GetAttributes((IWindowManagerLayoutParams**)&lp);
    return lp->GetGravity(gravity);
}

void CSoftInputWindow::UpdateWidthHeight(
    /* [in] */ IWindowManagerLayoutParams* lp)
{
    Int32 gravity = 0;
    lp->GetGravity(&gravity);
    if (gravity == IGravity::TOP || gravity == IGravity::BOTTOM) {
        IViewGroupLayoutParams::Probe(lp)->SetWidth(IViewGroupLayoutParams::MATCH_PARENT);
        IViewGroupLayoutParams::Probe(lp)->SetHeight(IViewGroupLayoutParams::WRAP_CONTENT);
    } else {
        IViewGroupLayoutParams::Probe(lp)->SetWidth(IViewGroupLayoutParams::WRAP_CONTENT);
        IViewGroupLayoutParams::Probe(lp)->SetHeight(IViewGroupLayoutParams::MATCH_PARENT);
    }
}

ECode CSoftInputWindow::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Boolean tmp = FALSE;
    if (mKeyEventCallback != NULL && (mKeyEventCallback->OnKeyDown(keyCode, event, &tmp), tmp)) {
        *result = TRUE;
        return NOERROR;
    }
    assert(0 && "TODO");
    // return Dialog::OnKeyDown(keyCode, event, result);
    return NOERROR;
}

ECode CSoftInputWindow::OnKeyLongPress(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Boolean tmp = FALSE;
    if (mKeyEventCallback != NULL && (mKeyEventCallback->OnKeyLongPress(keyCode, event, &tmp), tmp)) {
        *result = TRUE;
        return NOERROR;
    }

    assert(0 && "TODO");
    // return Dialog::OnKeyLongPress(keyCode, event, result);
    return NOERROR;
}

ECode CSoftInputWindow::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Boolean tmp = FALSE;
    if (mKeyEventCallback != NULL && (mKeyEventCallback->OnKeyUp(keyCode, event, &tmp), tmp)) {
        *result = TRUE;
        return NOERROR;
    }

    assert(0 && "TODO");
    // return Dialog::OnKeyUp(keyCode, event, result);
    return NOERROR;
}

ECode CSoftInputWindow::OnKeyMultiple(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 count,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Boolean tmp = FALSE;
    if (mKeyEventCallback != NULL && (mKeyEventCallback->OnKeyMultiple(keyCode, count, event, &tmp), tmp)) {
        *result = TRUE;
        return NOERROR;
    }

    assert(0 && "TODO");
    // return Dialog::OnKeyMultiple(keyCode, count, event, result);
    return NOERROR;
}

ECode CSoftInputWindow::OnBackPressed()
{
    if (mCallback != NULL) {
        mCallback->OnBackPressed();
    } else {
        assert(0 && "TODO");
        // Dialog::OnBackPressed();
    }

    return NOERROR;
}

void CSoftInputWindow::InitDockWindow()
{
    assert(0 && "TODO");
    AutoPtr<IWindow> window/* = GetWindow()*/;
    AutoPtr<IWindowManagerLayoutParams> lp;
    window->GetAttributes((IWindowManagerLayoutParams**)&lp);
    lp->SetType(mWindowType);

    AutoPtr<ICharSequence> name;
    CString::New(mName, (ICharSequence**)&name);
    lp->SetTitle(name);

    lp->SetGravity(mGravity);
    UpdateWidthHeight(lp);

    window->SetAttributes(lp);

    Int32 windowSetFlags = IWindowManagerLayoutParams::FLAG_LAYOUT_IN_SCREEN;
    Int32 windowModFlags = IWindowManagerLayoutParams::FLAG_LAYOUT_IN_SCREEN |
            IWindowManagerLayoutParams::FLAG_NOT_FOCUSABLE |
            IWindowManagerLayoutParams::FLAG_DIM_BEHIND;

    if (!mTakesFocus) {
        windowSetFlags |= IWindowManagerLayoutParams::FLAG_NOT_FOCUSABLE;
    } else {
        windowSetFlags |= IWindowManagerLayoutParams::FLAG_NOT_TOUCH_MODAL;
        windowModFlags |= IWindowManagerLayoutParams::FLAG_NOT_TOUCH_MODAL;
    }

    window->SetFlags(windowSetFlags, windowModFlags);
}

ECode CSoftInputWindow::constructor(
    /* [in] */ IContext* context,
    /* [in] */ const String& name,
    /* [in] */ Int32 theme,
    /* [in] */ ISoftInputWindowCallback* callback,
    /* [in] */ IKeyEventCallback* keyEventCallback,
    /* [in] */ IDispatcherState* dispatcherState,
    /* [in] */ Int32 windowType,
    /* [in] */ Int32 gravity,
    /* [in] */ Boolean takesFocus)
{
    assert(0 && "TODO");
    // FAIL_RETURN(Dialog::constructor(context, theme));
    mName = name;
    mCallback = callback;
    mKeyEventCallback = keyEventCallback;
    mDispatcherState = dispatcherState;
    mWindowType = windowType;
    mGravity = gravity;
    mTakesFocus = takesFocus;
    InitDockWindow();
    return NOERROR;
}

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos
