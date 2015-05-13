
#include "ext/frameworkdef.h"
#include "inputmethodservice/CLatinKeyboardView.h"

using Elastos::Droid::InputMethodService::ISoftKeyboard;
using Elastos::Droid::View::EIID_View;

namespace Elastos {
namespace Droid {
namespace InputMethodService {

IVIEW_METHODS_IMPL(CLatinKeyboardView, KeyboardView);
IDRAWABLECALLBACK_METHODS_IMPL(CLatinKeyboardView, KeyboardView);
IKEYEVENTCALLBACK_METHODS_IMPL(CLatinKeyboardView, KeyboardView);
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CLatinKeyboardView, KeyboardView);


ECode CLatinKeyboardView::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return KeyboardView::Init(ctx, attrs);
}

ECode CLatinKeyboardView::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return KeyboardView::Init(ctx, attrs, defStyle);
}

PInterface CLatinKeyboardView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CLatinKeyboardView::Probe(riid);
}

ECode CLatinKeyboardView::OnLongPress(
    /* [in] */ IKeyboardKey* popupKey,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);

    AutoPtr< ArrayOf<Int32> > codes;
    popupKey->GetCodes((ArrayOf<Int32>**)&codes);
    assert(codes != NULL);
    if ((*codes)[0] == IKeyboard::KEYCODE_CANCEL) {
        AutoPtr<IOnKeyboardActionListener> listener;
        KeyboardView::GetOnKeyboardActionListener((IOnKeyboardActionListener**)&listener);
        listener->OnKey(ISoftKeyboard::KEYCODE_OPTIONS, NULL);
        *res = TRUE;
        return NOERROR;
    }
    else {
        return KeyboardView::OnLongPress(popupKey, res);
    }
}

ECode CLatinKeyboardView::SetOnKeyboardActionListener(
    /* [in] */ IOnKeyboardActionListener* listener)
{
    return KeyboardView::SetOnKeyboardActionListener(listener);
}

ECode CLatinKeyboardView::GetOnKeyboardActionListener(
    /* [out] */ IOnKeyboardActionListener** listener)
{
    VALIDATE_NOT_NULL(listener);
    return KeyboardView::GetOnKeyboardActionListener(listener);
}

ECode CLatinKeyboardView::SetKeyboard(
    /* [in] */ IKeyboard* keyboard)
{
    return KeyboardView::SetKeyboard(keyboard);
}

ECode CLatinKeyboardView::GetKeyboard(
    /* [out] */ IKeyboard** keyboard)
{
    VALIDATE_NOT_NULL(keyboard);
    return KeyboardView::GetKeyboard(keyboard);
}

ECode CLatinKeyboardView::SetShifted(
    /* [in] */ Boolean shifted,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    return KeyboardView::SetShifted(shifted, res);
}

ECode CLatinKeyboardView::IsShifted(
    /* [out] */ Boolean* shifted)
{
    VALIDATE_NOT_NULL(shifted);
    return KeyboardView::IsShifted(shifted);
}

ECode CLatinKeyboardView::SetPreviewEnabled(
    /* [in] */ Boolean previewEnabled)
{
    return KeyboardView::SetPreviewEnabled(previewEnabled);
}

ECode CLatinKeyboardView::IsPreviewEnabled(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    return KeyboardView::IsPreviewEnabled(res);
}

ECode CLatinKeyboardView::SetVerticalCorrection(
    /* [in] */ Int32 verticalOffset)
{
    return KeyboardView::SetVerticalCorrection(verticalOffset);
}

ECode CLatinKeyboardView::SetPopupParent(
    /* [in] */ IView* v)
{
    return KeyboardView::SetPopupParent(v);
}

ECode CLatinKeyboardView::SetPopupOffset(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    return KeyboardView::SetPopupOffset(x, y);
}

ECode CLatinKeyboardView::SetProximityCorrectionEnabled(
    /* [in] */ Boolean enabled)
{
    return KeyboardView::SetProximityCorrectionEnabled(enabled);
}

ECode CLatinKeyboardView::IsProximityCorrectionEnabled(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    return KeyboardView::IsProximityCorrectionEnabled(res);
}

ECode CLatinKeyboardView::OnClick(
    /* [in] */ IView* v)
{
    return KeyboardView::OnClick(v);
}

ECode CLatinKeyboardView::Closing()
{
    return KeyboardView::Closing();
}

ECode CLatinKeyboardView::HandleBack(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    return KeyboardView::HandleBack(res);
}

ECode CLatinKeyboardView::InvalidateAllKeys()
{
    return KeyboardView::InvalidateAllKeys();
}

ECode CLatinKeyboardView::InvalidateKey(
    /* [in] */ Int32 keyIndex)
{
    return KeyboardView::InvalidateKey(keyIndex);
}

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos
