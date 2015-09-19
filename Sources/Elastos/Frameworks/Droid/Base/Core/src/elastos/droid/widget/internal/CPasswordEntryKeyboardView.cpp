#include "widget/internal/CPasswordEntryKeyboardView.h"

using Elastos::Droid::View::EIID_View;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

IVIEW_METHODS_IMPL(CPasswordEntryKeyboardView, PasswordEntryKeyboardView)
IDRAWABLECALLBACK_METHODS_IMPL(CPasswordEntryKeyboardView, PasswordEntryKeyboardView)
IKEYEVENTCALLBACK_METHODS_IMPL(CPasswordEntryKeyboardView, PasswordEntryKeyboardView)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CPasswordEntryKeyboardView, PasswordEntryKeyboardView)

PInterface CPasswordEntryKeyboardView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CPasswordEntryKeyboardView::Probe(riid);
}

CPasswordEntryKeyboardView::CPasswordEntryKeyboardView()
{}

ECode CPasswordEntryKeyboardView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return PasswordEntryKeyboardView::Init(context, attrs);
}

ECode CPasswordEntryKeyboardView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return PasswordEntryKeyboardView::Init(context, attrs, defStyle);
}

ECode CPasswordEntryKeyboardView::OnClick(
    /* [in] */ IView* v)
{
    return PasswordEntryKeyboardView::OnClick(v);
}

ECode CPasswordEntryKeyboardView::SetOnKeyboardActionListener(
    /* [in] */ IOnKeyboardActionListener* listener)
{
    return PasswordEntryKeyboardView::SetOnKeyboardActionListener(listener);
}

ECode CPasswordEntryKeyboardView::SetKeyboard(
    /* [in] */ IKeyboard* keyboard)
{
    return PasswordEntryKeyboardView::SetKeyboard(keyboard);
}

ECode CPasswordEntryKeyboardView::GetKeyboard(
    /* [out] */ IKeyboard** keyboard)
{
    VALIDATE_NOT_NULL(keyboard);
    return PasswordEntryKeyboardView::GetKeyboard(keyboard);
}

ECode CPasswordEntryKeyboardView::SetShifted(
    /* [in] */ Boolean shifted,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    return PasswordEntryKeyboardView::SetShifted(shifted, res);
}

ECode CPasswordEntryKeyboardView::IsShifted(
    /* [out] */ Boolean* shifted)
{
    VALIDATE_NOT_NULL(shifted);
    return PasswordEntryKeyboardView::IsShifted(shifted);
}

ECode CPasswordEntryKeyboardView::SetPreviewEnabled(
    /* [in] */ Boolean previewEnabled)
{
    return PasswordEntryKeyboardView::SetPreviewEnabled(previewEnabled);
}

ECode CPasswordEntryKeyboardView::IsPreviewEnabled(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    return PasswordEntryKeyboardView::IsPreviewEnabled(res);
}

ECode CPasswordEntryKeyboardView::SetVerticalCorrection(
    /* [in] */ Int32 verticalOffset)
{
    return PasswordEntryKeyboardView::SetVerticalCorrection(verticalOffset);
}

ECode CPasswordEntryKeyboardView::SetPopupParent(
    /* [in] */ IView* v)
{
    return PasswordEntryKeyboardView::SetPopupParent(v);
}

ECode CPasswordEntryKeyboardView::SetPopupOffset(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    return PasswordEntryKeyboardView::SetPopupOffset(x, y);
}

ECode CPasswordEntryKeyboardView::SetProximityCorrectionEnabled(
    /* [in] */ Boolean enabled)
{
    return PasswordEntryKeyboardView::SetProximityCorrectionEnabled(enabled);
}

ECode CPasswordEntryKeyboardView::IsProximityCorrectionEnabled(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    return PasswordEntryKeyboardView::IsProximityCorrectionEnabled(res);
}

ECode CPasswordEntryKeyboardView::InvalidateAllKeys()
{
    return PasswordEntryKeyboardView::InvalidateAllKeys();
}

ECode CPasswordEntryKeyboardView::InvalidateKey(
    /* [in] */ Int32 keyIndex)
{
    return PasswordEntryKeyboardView::InvalidateKey(keyIndex);
}

ECode CPasswordEntryKeyboardView::Closing()
{
    return PasswordEntryKeyboardView::Closing();
}

ECode CPasswordEntryKeyboardView::HandleBack(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    return PasswordEntryKeyboardView::HandleBack(res);
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos