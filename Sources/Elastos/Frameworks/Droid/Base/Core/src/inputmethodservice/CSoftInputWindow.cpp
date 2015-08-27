
#include "inputmethodservice/CSoftInputWindow.h"
// #include "view/CWindowManagerLayoutParams.h"

using Elastos::Core::CString;
using Elastos::Core::ICharSequence;
// using Elastos::Droid::View::CWindowManagerLayoutParams;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::IInputEvent;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IWindow;
using Elastos::Droid::View::IWindowManagerLayoutParams;

namespace Elastos {
namespace Droid {
namespace InputMethodService {

CAR_OBJECT_IMPL(CSoftInputWindow);
CAR_INTERFACE_IMPL(CSoftInputWindow, /*Dialog*/Object, ISoftInputWindow);
CSoftInputWindow::CSoftInputWindow()
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

ECode CSoftInputWindow::GetSize(
    /* [out] */ Int32* size)
{
    assert(size != NULL);
    assert(0 && "TODO");
    AutoPtr<IWindow> window/* = GetWindow()*/;
    AutoPtr<IWindowManagerLayoutParams> lp;
    window->GetAttributes((IWindowManagerLayoutParams**)&lp);
    // CWindowManagerLayoutParams* lpObj = (CWindowManagerLayoutParams*)lp.Get();
    // if (lpObj->mGravity == IGravity::TOP || lpObj->mGravity == IGravity::BOTTOM) {
    //     *size = lpObj->mHeight;
    // }
    // else {
    //     *size = lpObj->mWidth;
    // }
    return NOERROR;
}

ECode CSoftInputWindow::SetSize(
    /* [in] */ Int32 size)
{
    assert(0 && "TODO");
    AutoPtr<IWindow> window/* = GetWindow()*/;
    AutoPtr<IWindowManagerLayoutParams> lp;
    window->GetAttributes((IWindowManagerLayoutParams**)&lp);
    // CWindowManagerLayoutParams* lpObj = (CWindowManagerLayoutParams*)lp.Get();
    // if (lpObj->mGravity == IGravity::TOP || lpObj->mGravity == IGravity::BOTTOM) {
    //     lpObj->mWidth = -1;
    //     lpObj->mHeight = size;
    // }
    // else {
    //     lpObj->mWidth = size;
    //     lpObj->mHeight = -1;
    // }
    return window->SetAttributes(lp);
}

ECode CSoftInputWindow::SetGravity(
    /* [in] */ Int32 gravity)
{
    assert(0 && "TODO");
    AutoPtr<IWindow> window/* = GetWindow()*/;
    AutoPtr<IWindowManagerLayoutParams> lp;
    window->GetAttributes((IWindowManagerLayoutParams**)&lp);
    // CWindowManagerLayoutParams* lpObj = (CWindowManagerLayoutParams*)lp.Get();

    // Boolean oldIsVertical = (lpObj->mGravity == IGravity::TOP || lpObj->mGravity == IGravity::BOTTOM);

    // lpObj->mGravity = gravity;

    // Boolean newIsVertical = (lpObj->mGravity == IGravity::TOP || lpObj->mGravity == IGravity::BOTTOM);

    // if (oldIsVertical != newIsVertical) {
    //     Int32 tmp = lpObj->mWidth;
    //     lpObj->mWidth = lpObj->mHeight;
    //     lpObj->mHeight = tmp;
    //     return window->SetAttributes(lp);
    // }
    return NOERROR;
}

void CSoftInputWindow::InitDockWindow()
{
    assert(0 && "TODO");
    AutoPtr<IWindow> window/* = GetWindow()*/;
    AutoPtr<IWindowManagerLayoutParams> lp;
    window->GetAttributes((IWindowManagerLayoutParams**)&lp);
    // CWindowManagerLayoutParams* lpObj = (CWindowManagerLayoutParams*)lp.Get();

    // lpObj->mType = IWindowManagerLayoutParams::TYPE_INPUT_METHOD;
    // AutoPtr<ICharSequence> text;
    // CString::New(String("InputMethod"), (ICharSequence**)&text);
    // lp->SetTitle(text);

    // lpObj->mGravity = IGravity::BOTTOM;
    // lpObj->mWidth = -1;
    // Let the input method window's orientation follow sensor based rotation
    // Turn this off for now, it is very problematic.
    //lp.screenOrientation = ActivityInfo.SCREEN_ORIENTATION_USER;

    window->SetAttributes(lp);
    window->SetFlags(
            IWindowManagerLayoutParams::FLAG_LAYOUT_IN_SCREEN |
            IWindowManagerLayoutParams::FLAG_NOT_FOCUSABLE,
            IWindowManagerLayoutParams::FLAG_LAYOUT_IN_SCREEN |
            IWindowManagerLayoutParams::FLAG_NOT_FOCUSABLE |
            IWindowManagerLayoutParams::FLAG_DIM_BEHIND);
}

ECode CSoftInputWindow::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 theme,
    /* [in] */ IDispatcherState* dispatcherState)
{
    assert(0 && "TODO");
    // FAIL_RETURN(Dialog::constructor(context, theme));
    mDispatcherState = dispatcherState;
    InitDockWindow();
    return NOERROR;
}

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos
