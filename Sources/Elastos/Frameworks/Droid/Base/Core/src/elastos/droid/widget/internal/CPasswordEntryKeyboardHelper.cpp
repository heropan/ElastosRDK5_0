#include "elastos/droid/widget/internal/CPasswordEntryKeyboardHelper.h"

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

CPasswordEntryKeyboardHelper::CPasswordEntryKeyboardHelper()
{}

ECode CPasswordEntryKeyboardHelper::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IKeyboardView* keyboardView,
    /* [in] */ IView* targetView)
{
    return PasswordEntryKeyboardHelper::Init(context, keyboardView, targetView);
}

ECode CPasswordEntryKeyboardHelper::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IKeyboardView* keyboardView,
    /* [in] */ IView* targetView,
    /* [in] */ Boolean useFullScreenWidth)
{
    return PasswordEntryKeyboardHelper::Init(context, keyboardView, targetView, useFullScreenWidth);
}

ECode CPasswordEntryKeyboardHelper::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IKeyboardView* keyboardView,
    /* [in] */ IView* targetView,
    /* [in] */ Boolean useFullScreenWidth,
    /* [in] */ ArrayOf<Int32>* layouts)
{
    return PasswordEntryKeyboardHelper::Init(context, keyboardView, targetView, useFullScreenWidth, layouts);
}

PInterface CPasswordEntryKeyboardHelper::Probe(
    /* [in] */ REIID riid)
{
    return _CPasswordEntryKeyboardHelper::Probe(riid);
}

ECode CPasswordEntryKeyboardHelper::CreateKeyboards()
{
    return PasswordEntryKeyboardHelper::CreateKeyboards();
}

ECode CPasswordEntryKeyboardHelper::SetEnableHaptics(
    /* [in] */ Boolean enabled)
{
    return PasswordEntryKeyboardHelper::SetEnableHaptics(enabled);
}

ECode CPasswordEntryKeyboardHelper::IsAlpha(
    /* [out] */ Boolean* alpha)
{
    *alpha = PasswordEntryKeyboardHelper::IsAlpha();
    return NOERROR;
}

ECode CPasswordEntryKeyboardHelper::SetKeyboardMode(
    /* [in] */ Int32 mode)
{
    return PasswordEntryKeyboardHelper::SetKeyboardMode(mode);
}

ECode CPasswordEntryKeyboardHelper::SendDownUpKeyEvents(
    /* [in] */ Int32 keyEventCode)
{
    return PasswordEntryKeyboardHelper::SendDownUpKeyEvents(keyEventCode);
}

ECode CPasswordEntryKeyboardHelper::OnKey(
    /* [in] */ Int32 primaryCode,
    /* [in] */ ArrayOf<Int32>* keyCodes)
{
    return PasswordEntryKeyboardHelper::OnKey(primaryCode, keyCodes);
}

ECode CPasswordEntryKeyboardHelper::SetVibratePattern(
    /* [in] */ Int32 id)
{
    return PasswordEntryKeyboardHelper::SetVibratePattern(id);
}

ECode CPasswordEntryKeyboardHelper::HandleBackspace()
{
    return PasswordEntryKeyboardHelper::HandleBackspace();
}

ECode CPasswordEntryKeyboardHelper::OnPress(
    /* [in] */ Int32 primaryCode)
{
    return PasswordEntryKeyboardHelper::OnPress(primaryCode);
}

ECode CPasswordEntryKeyboardHelper::OnRelease(
    /* [in] */ Int32 primaryCode)
{
    return PasswordEntryKeyboardHelper::OnRelease(primaryCode);
}

ECode CPasswordEntryKeyboardHelper::OnText(
    /* [in] */ ICharSequence* text)
{
    return PasswordEntryKeyboardHelper::OnText(text);
}

ECode CPasswordEntryKeyboardHelper::SwipeDown()
{
    return PasswordEntryKeyboardHelper::SwipeDown();
}

ECode CPasswordEntryKeyboardHelper::SwipeLeft()
{
    return PasswordEntryKeyboardHelper::SwipeLeft();
}

ECode CPasswordEntryKeyboardHelper::SwipeRight()
{
    return PasswordEntryKeyboardHelper::SwipeRight();
}

ECode CPasswordEntryKeyboardHelper::SwipeUp()
{
    return PasswordEntryKeyboardHelper::SwipeUp();
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos