#include "widget/CSpinner.h"
#include "view/ViewGroup.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEW_METHODS_IMPL(CSpinner, Spinner)
IVIEWGROUP_METHODS_IMPL(CSpinner, Spinner)
IADAPTERVIEW_METHODS_IMPL(CSpinner, Spinner)

ECode CSpinner::constructor(
    /* [in] */ IContext* ctx)
{
    return Spinner::Init(ctx);
}

ECode CSpinner::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 mode)
{
    return Spinner::Init(context, NULL, R::attr::spinnerStyle, mode);
}

ECode CSpinner::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return Spinner::Init(ctx, attrs);
}

ECode CSpinner::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return Spinner::Init(ctx, attrs, defStyle);
}

PInterface CSpinner::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    } else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    } else if (riid == EIID_Spinner) {
        return reinterpret_cast<PInterface>((Spinner*)this);
    }
    return _CSpinner::Probe(riid);
}

ECode CSpinner::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    Spinner::OnClick(dialog, which);
    return NOERROR;
}

ECode CSpinner::SetSelectionEx(
    /* [in] */ Int32 position,
    /* [in] */ Boolean animate)
{
    return Spinner::SetSelection(position, animate);
}

ECode CSpinner::PointToPosition(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [out] */ Int32* pos)
{
    VALIDATE_NOT_NULL(pos);
    *pos = Spinner::PointToPosition(x, y);
    return NOERROR;
}

ECode CSpinner::SetPopupBackgroundDrawable(
    /* [in] */ IDrawable* background)
{
    Spinner::SetPopupBackgroundDrawable(background);
    return NOERROR;
}

ECode CSpinner::SetPopupBackgroundResource(
    /* [in] */ Int32 resId)
{
    Spinner::SetPopupBackgroundResource(resId);
    return NOERROR;
}

ECode CSpinner::GetPopupBackground(
    /* [out] */ IDrawable** d)
{
    VALIDATE_NOT_NULL(d);
    AutoPtr<IDrawable> dTemp = Spinner::GetPopupBackground();
    *d = dTemp;
    INTERFACE_ADDREF(*d);
    return NOERROR;
}

ECode CSpinner::SetDropDownVerticalOffset(
    /* [in] */ Int32 pixels)
{
    Spinner::SetDropDownVerticalOffset(pixels);
    return NOERROR;
}

ECode CSpinner::GetDropDownVerticalOffset(
    /* [out] */ Int32* pixels)
{
    VALIDATE_NOT_NULL(pixels);
    *pixels = Spinner::GetDropDownVerticalOffset();
    return NOERROR;
}

ECode CSpinner::SetDropDownHorizontalOffset(
    /* [in] */ Int32 pixels)
{
    Spinner::SetDropDownHorizontalOffset(pixels);
    return NOERROR;
}

ECode CSpinner::GetDropDownHorizontalOffset(
    /* [out] */ Int32* pixels)
{
    VALIDATE_NOT_NULL(pixels);
    *pixels = Spinner::GetDropDownHorizontalOffset();
    return NOERROR;
}

ECode CSpinner::SetDropDownWidth(
    /* [in] */ Int32 pixels)
{
    Spinner::SetDropDownWidth(pixels);
    return NOERROR;
}

ECode CSpinner::GetDropDownWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = Spinner::GetDropDownWidth();
    return NOERROR;
}

ECode CSpinner::SetGravity(
    /* [in] */ Int32 gravity)
{
    Spinner::SetGravity(gravity);
    return NOERROR;
}

ECode CSpinner::GetGravity(
    /* [out] */ Int32* gravity)
{
    VALIDATE_NOT_NULL(gravity);
    *gravity = Spinner::GetGravity();
    return NOERROR;
}

ECode CSpinner::SetPrompt(
    /* [in] */ ICharSequence* prompt)
{
    Spinner::SetPrompt(prompt);
    return NOERROR;
}

ECode CSpinner::SetPromptId(
    /* [in] */ Int32 promptId)
{
    Spinner::SetPromptId(promptId);
    return NOERROR;
}

ECode CSpinner::GetPrompt(
    /* [out] */ ICharSequence** prompt)
{
    VALIDATE_NOT_NULL(prompt);
    AutoPtr<ICharSequence> cTemp = Spinner::GetPrompt();
    *prompt = cTemp;
    INTERFACE_ADDREF(*prompt);
    return NOERROR;
}


}// namespace Widget
}// namespace Droid
}// namespace Elastos
