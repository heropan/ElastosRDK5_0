
#include "ext/frameworkdef.h"
#include "inputmethodservice/CKeyboardView.h"
#include "R.h"

using Elastos::Droid::R;
using Elastos::Droid::View::EIID_View;

namespace Elastos {
namespace Droid {
namespace InputMethodService {

IVIEW_METHODS_IMPL(CKeyboardView, KeyboardView);
IDRAWABLECALLBACK_METHODS_IMPL(CKeyboardView, KeyboardView);
IKEYEVENTCALLBACK_METHODS_IMPL(CKeyboardView, KeyboardView);
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CKeyboardView, KeyboardView);

ECode CKeyboardView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return Init(context, attrs, R::attr::keyboardViewStyle);
}

ECode CKeyboardView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return Init(context, attrs, defStyle);
}

PInterface CKeyboardView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CKeyboardView::Probe(riid);
}

ECode CKeyboardView::OnClick(
    /* [in] */ IView* v)
{
    return KeyboardView::OnClick(v);
}

ECode CKeyboardView::SetOnKeyboardActionListener(
    /* [in] */ IOnKeyboardActionListener* listener)
{
    return KeyboardView::SetOnKeyboardActionListener(listener);
}

ECode CKeyboardView::GetOnKeyboardActionListener(
    /* [out] */ IOnKeyboardActionListener** listener)
{
    VALIDATE_NOT_NULL(listener);
    return KeyboardView::GetOnKeyboardActionListener(listener);
}

ECode CKeyboardView::Closing()
{
    return KeyboardView::Closing();
}

ECode CKeyboardView::HandleBack(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    return KeyboardView::HandleBack(res);
}

ECode CKeyboardView::SetKeyboard(
    /* [in] */ IKeyboard* keyboard)
{
    return KeyboardView::SetKeyboard(keyboard);
}

ECode CKeyboardView::GetKeyboard(
    /* [out] */ IKeyboard** keyboard)
{
    VALIDATE_NOT_NULL(keyboard);
    return KeyboardView::GetKeyboard(keyboard);
}

ECode CKeyboardView::SetShifted(
    /* [in] */ Boolean shifted,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    return KeyboardView::SetShifted(shifted, res);
}

ECode CKeyboardView::IsShifted(
    /* [out] */ Boolean* shifted)
{
    VALIDATE_NOT_NULL(shifted);
    return KeyboardView::IsShifted(shifted);
}

ECode CKeyboardView::SetPreviewEnabled(
    /* [in] */ Boolean previewEnabled)
{
    return KeyboardView::SetPreviewEnabled(previewEnabled);
}

ECode CKeyboardView::IsPreviewEnabled(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    return KeyboardView::IsPreviewEnabled(res);
}

ECode CKeyboardView::SetVerticalCorrection(
    /* [in] */ Int32 verticalOffset)
{
    return KeyboardView::SetVerticalCorrection(verticalOffset);
}

ECode CKeyboardView::SetPopupParent(
    /* [in] */ IView* v)
{
    return KeyboardView::SetPopupParent(v);
}

ECode CKeyboardView::SetPopupOffset(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    return KeyboardView::SetPopupOffset(x, y);
}

ECode CKeyboardView::SetProximityCorrectionEnabled(
    /* [in] */ Boolean enabled)
{
    return KeyboardView::SetProximityCorrectionEnabled(enabled);
}

ECode CKeyboardView::IsProximityCorrectionEnabled(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    return KeyboardView::IsProximityCorrectionEnabled(res);
}

ECode CKeyboardView::InvalidateAllKeys()
{
    return KeyboardView::InvalidateAllKeys();
}

ECode CKeyboardView::InvalidateKey(
    /* [in] */ Int32 keyIndex)
{
    return KeyboardView::InvalidateKey(keyIndex);
}

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos
