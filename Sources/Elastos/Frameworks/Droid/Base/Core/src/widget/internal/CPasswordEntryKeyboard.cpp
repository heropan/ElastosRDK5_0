#include "widget/internal/CPasswordEntryKeyboard.h"

using Elastos::Droid::InputMethodService::IKeyboard;
using Elastos::Droid::InputMethodService::EIID_IKeyboard;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

CPasswordEntryKeyboard::CPasswordEntryKeyboard()
{}

PInterface CPasswordEntryKeyboard::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IKeyboard) {
        return (PasswordEntryKeyboard*)this;
    } else if (riid == EIID_IInterface) {
        return (PasswordEntryKeyboard*)this;
    }
    return _CPasswordEntryKeyboard::Probe(riid);
}

UInt32 CPasswordEntryKeyboard::AddRef()
{
    return PasswordEntryKeyboard::AddRef();
}

UInt32 CPasswordEntryKeyboard::Release()
{
    return PasswordEntryKeyboard::Release();
}

ECode CPasswordEntryKeyboard::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pObject == (IInterface*)(PasswordEntryKeyboard*)this)
    {
        *pIID = EIID_IKeyboard;
        return NOERROR;
    }
    if (pObject == (IInterface*)(IPasswordEntryKeyboard*)this) {
        *pIID = EIID_IPasswordEntryKeyboard;
        return NOERROR;
    }
    return E_INVALID_ARGUMENT;
}

ECode CPasswordEntryKeyboard::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 xmlLayoutResId)
{
    return PasswordEntryKeyboard::Init(context, xmlLayoutResId);
}

ECode CPasswordEntryKeyboard::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 xmlLayoutResId,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return PasswordEntryKeyboard::Init(context, xmlLayoutResId, width, height);
}

ECode CPasswordEntryKeyboard::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 xmlLayoutResId,
    /* [in] */ Int32 modeId,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return PasswordEntryKeyboard::Init(context, xmlLayoutResId, modeId, width, height);
}

ECode CPasswordEntryKeyboard::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 xmlLayoutResId,
    /* [in] */ Int32 modeId)
{
    return PasswordEntryKeyboard::Init(context, xmlLayoutResId, modeId);
}

ECode CPasswordEntryKeyboard::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 layoutTemplateResId,
    /* [in] */ ICharSequence* characters,
    /* [in] */ Int32 columns,
    /* [in] */ Int32 horizontalPadding)
{
    return PasswordEntryKeyboard::Init(context, layoutTemplateResId, characters, columns, horizontalPadding);
}

ECode CPasswordEntryKeyboard::GetKeys(
    /* [out] */ IObjectContainer** keys)
{
    return PasswordEntryKeyboard::GetKeys(keys);
}

ECode CPasswordEntryKeyboard::GetModifierKeys(
    /* [out] */ IObjectContainer** keys)
{
    return PasswordEntryKeyboard::GetModifierKeys(keys);
}

ECode CPasswordEntryKeyboard::GetHorizontalGap(
    /* [out] */ Int32* hGap)
{
    return PasswordEntryKeyboard::GetHorizontalGap(hGap);
}

ECode CPasswordEntryKeyboard::SetHorizontalGap(
    /* [in] */ Int32 gap)
{
    return PasswordEntryKeyboard::SetHorizontalGap(gap);
}

ECode CPasswordEntryKeyboard::GetVerticalGap(
    /* [out] */ Int32* vGap)
{
    return PasswordEntryKeyboard::GetVerticalGap(vGap);
}

ECode CPasswordEntryKeyboard::SetVerticalGap(
    /* [in] */ Int32 gap)
{
    return PasswordEntryKeyboard::SetVerticalGap(gap);
}

ECode CPasswordEntryKeyboard::GetKeyHeight(
    /* [out] */ Int32* h)
{
    return PasswordEntryKeyboard::GetKeyHeight(h);
}

ECode CPasswordEntryKeyboard::SetKeyHeight(
    /* [in] */ Int32 height)
{
    return PasswordEntryKeyboard::SetKeyHeight(height);
}

ECode CPasswordEntryKeyboard::GetKeyWidth(
    /* [out] */ Int32* w)
{
    return PasswordEntryKeyboard::GetKeyWidth(w);
}

ECode CPasswordEntryKeyboard::SetKeyWidth(
    /* [in] */ Int32 width)
{
    return PasswordEntryKeyboard::SetKeyWidth(width);
}

ECode CPasswordEntryKeyboard::GetHeight(
    /* [out] */ Int32* h)
{
    return PasswordEntryKeyboard::GetHeight(h);
}

ECode CPasswordEntryKeyboard::GetMinWidth(
    /* [out] */ Int32* minW)
{
    return PasswordEntryKeyboard::GetMinWidth(minW);
}

ECode CPasswordEntryKeyboard::SetShifted(
    /* [in] */ Boolean shiftState,
    /* [out] */ Boolean* res)
{
    return PasswordEntryKeyboard::SetShifted(shiftState, res);
}

ECode CPasswordEntryKeyboard::IsShifted(
    /* [out] */ Boolean* res)
{
    return PasswordEntryKeyboard::IsShifted(res);
}

ECode CPasswordEntryKeyboard::GetShiftKeyIndex(
    /* [out] */ Int32* index)
{
    return PasswordEntryKeyboard::GetShiftKeyIndex(index);
}

ECode CPasswordEntryKeyboard::GetShiftKeyIndices(
    /* [out, callee] */ ArrayOf<Int32>** keyIndices)
{
    return PasswordEntryKeyboard::GetShiftKeyIndices(keyIndices);
}

ECode CPasswordEntryKeyboard::GetNearestKeys(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [out, callee] */ ArrayOf<Int32>** nearestKeys)
{
    return PasswordEntryKeyboard::GetNearestKeys(x, y, nearestKeys);
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos