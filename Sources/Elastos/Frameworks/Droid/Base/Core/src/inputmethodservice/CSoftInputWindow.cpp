
#include "inputmethodservice/CSoftInputWindow.h"
#include "view/CWindowManagerLayoutParams.h"

using Elastos::Core::CStringWrapper;
using Elastos::Droid::View::CWindowManagerLayoutParams;
using Elastos::Droid::View::IGravity;

namespace Elastos {
namespace Droid {
namespace InputMethodService {

IDIALOG_METHODS_IMPL(CSoftInputWindow, SoftInputWindow);
IWINDOWCALLBACK_METHODS_IMPL(CSoftInputWindow, SoftInputWindow);
IKEYEVENTCALLBACK_METHODS_IMPL(CSoftInputWindow, SoftInputWindow);

SoftInputWindow::SoftInputWindow()
{
    CRect::New((IRect**)&mBounds);
}

ECode SoftInputWindow::SetToken(
    /* [in] */ IBinder* token)
{
    AutoPtr<IWindow> window = GetWindow();
    AutoPtr<IWindowManagerLayoutParams> lp;
    window->GetAttributes((IWindowManagerLayoutParams**)&lp);
    lp->SetToken(token);
    return window->SetAttributes(lp);
}

ECode SoftInputWindow::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 theme,
    /* [in] */ IDispatcherState* dispatcherState)
{
    FAIL_RETURN(Dialog::Init(context, theme));
    mDispatcherState = dispatcherState;
    InitDockWindow();
    return NOERROR;
}

ECode SoftInputWindow::OnWindowFocusChanged(
    /* [in] */ Boolean hasFocus)
{
    FAIL_RETURN(Dialog::OnWindowFocusChanged(hasFocus));
    return mDispatcherState->Reset();
}

Boolean SoftInputWindow::DispatchTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    AutoPtr<IView> docorView;
    GetWindow()->GetDecorView((IView**)&docorView);
    docorView->GetHitRect(mBounds);

    Boolean withIn = FALSE;

    Int32 left = 0, top = 0, right = 0, bottom;
    mBounds->GetLeft(&left);
    mBounds->GetTop(&top);
    mBounds->GetRight(&right);
    mBounds->GetBottom(&bottom);
    if (ev->IsWithinBoundsNoHistory(left, top,
            right - 1, bottom - 1, &withIn), withIn) {
        return Dialog::DispatchTouchEvent(ev);
    } else {
        AutoPtr<IMotionEvent> temp;
        ev->ClampNoHistory(left, top,
                right - 1, bottom - 1, (IMotionEvent**)&temp);

        Boolean handled = Dialog::DispatchTouchEvent(temp);
        temp->Recycle();
        return handled;
    }
}

ECode SoftInputWindow::GetSize(
    /* [out] */ Int32* size)
{
    assert(size != NULL);
    AutoPtr<IWindow> window = GetWindow();
    AutoPtr<IWindowManagerLayoutParams> lp;
    window->GetAttributes((IWindowManagerLayoutParams**)&lp);
    CWindowManagerLayoutParams* lpObj = (CWindowManagerLayoutParams*)lp.Get();
    if (lpObj->mGravity == IGravity::TOP || lpObj->mGravity == IGravity::BOTTOM) {
        *size = lpObj->mHeight;
    }
    else {
        *size = lpObj->mWidth;
    }
    return NOERROR;
}

ECode SoftInputWindow::SetSize(
    /* [in] */ Int32 size)
{
    AutoPtr<IWindow> window = GetWindow();
    AutoPtr<IWindowManagerLayoutParams> lp;
    window->GetAttributes((IWindowManagerLayoutParams**)&lp);
    CWindowManagerLayoutParams* lpObj = (CWindowManagerLayoutParams*)lp.Get();
    if (lpObj->mGravity == IGravity::TOP || lpObj->mGravity == IGravity::BOTTOM) {
        lpObj->mWidth = -1;
        lpObj->mHeight = size;
    }
    else {
        lpObj->mWidth = size;
        lpObj->mHeight = -1;
    }
    return window->SetAttributes(lp);
}

ECode SoftInputWindow::SetGravity(
    /* [in] */ Int32 gravity)
{
    AutoPtr<IWindow> window = GetWindow();
    AutoPtr<IWindowManagerLayoutParams> lp;
    window->GetAttributes((IWindowManagerLayoutParams**)&lp);
    CWindowManagerLayoutParams* lpObj = (CWindowManagerLayoutParams*)lp.Get();

    Boolean oldIsVertical = (lpObj->mGravity == IGravity::TOP || lpObj->mGravity == IGravity::BOTTOM);

    lpObj->mGravity = gravity;

    Boolean newIsVertical = (lpObj->mGravity == IGravity::TOP || lpObj->mGravity == IGravity::BOTTOM);

    if (oldIsVertical != newIsVertical) {
        Int32 tmp = lpObj->mWidth;
        lpObj->mWidth = lpObj->mHeight;
        lpObj->mHeight = tmp;
        return window->SetAttributes(lp);
    }
    return NOERROR;
}

void SoftInputWindow::InitDockWindow()
{
    AutoPtr<IWindow> window = GetWindow();
    AutoPtr<IWindowManagerLayoutParams> lp;
    window->GetAttributes((IWindowManagerLayoutParams**)&lp);
    CWindowManagerLayoutParams* lpObj = (CWindowManagerLayoutParams*)lp.Get();

    lpObj->mType = IWindowManagerLayoutParams::TYPE_INPUT_METHOD;
    AutoPtr<ICharSequence> text;
    CStringWrapper::New(String("InputMethod"), (ICharSequence**)&text);
    lp->SetTitle(text);

    lpObj->mGravity = IGravity::BOTTOM;
    lpObj->mWidth = -1;
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
    return SoftInputWindow::Init(context, theme, dispatcherState);
}

PInterface CSoftInputWindow::Probe(
    /* [in] */ REIID riid)
{
    return _CSoftInputWindow::Probe(riid);
}

ECode CSoftInputWindow::SetToken(
    /* [in] */ IBinder* token)
{
    return SoftInputWindow::SetToken(token);
}

ECode CSoftInputWindow::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    return SoftInputWindow::GetSize(size);
}

ECode CSoftInputWindow::SetSize(
    /* [in] */ Int32 size)
{
    return SoftInputWindow::SetSize(size);
}

ECode CSoftInputWindow::SetGravity(
    /* [in] */ Int32 gravity)
{
    return SoftInputWindow::SetGravity(gravity);
}

ECode CSoftInputWindow::OnCreateContextMenu(
    /* [in] */ IContextMenu* menu,
    /* [in] */ IView* v,
    /* [in] */ IContextMenuInfo* menuInfo)
{
    return SoftInputWindow::OnCreateContextMenu(menu, v, menuInfo);
}

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos
