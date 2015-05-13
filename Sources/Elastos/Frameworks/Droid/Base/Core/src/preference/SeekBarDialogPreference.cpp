
#include "SeekBarDialogPreference.h"
#include "R.h"

using Elastos::Droid::Widget::IImageView;

namespace Elastos {
namespace Droid {
namespace Preference {

const String SeekBarDialogPreference::TAG("SeekBarDialogPreference");

SeekBarDialogPreference::SeekBarDialogPreference(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : DialogPreference(context, attrs)
{
    SetDialogLayoutResource(R::layout::seekbar_dialog);
    CreateActionButtons();

    // Steal the XML dialogIcon attribute's value
    GetDialogIcon((IDrawable**)&mMyIcon);
    SetDialogIcon(NULL);
}

PInterface SeekBarDialogPreference::Probe(
    /* [in] */REIID riid)
{
    if (riid == EIID_IDialogPreference) {
        return (IDialogPreference*)this;
    }

    return DialogPreference::Probe(riid);
}

UInt32 SeekBarDialogPreference::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 SeekBarDialogPreference::Release()
{
    return ElRefBase::Release();
}

ECode SeekBarDialogPreference::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface *)(IDialogPreference*)this) {
        *pIID = EIID_IDialogPreference;
        return NOERROR;
    }
    return DialogPreference::GetInterfaceID(pObject, pIID);
}

void SeekBarDialogPreference::CreateActionButtons()
{
    SetPositiveButtonTextEx(R::string::ok);
    SetNegativeButtonTextEx(R::string::cancel);
}

ECode SeekBarDialogPreference::OnBindDialogView(
    /* [in] */ IView* view)
{
    DialogPreference::OnBindDialogView(view);

    AutoPtr<IImageView> iconView;
    view->FindViewById(R::id::icon, (IView**)&iconView);
    if (mMyIcon != NULL) {
        iconView->SetImageDrawable(mMyIcon);
    }
    else {
        iconView->SetVisibility(IView::GONE);
    }

    return NOERROR;
}

AutoPtr<ISeekBar> SeekBarDialogPreference::GetSeekBar(
    /* [in] */ IView* dialogView)
{
    AutoPtr<IView> v;
    dialogView->FindViewById(R::id::seekbar, (IView**)&v);
    AutoPtr<ISeekBar> seekBar = ISeekBar::Probe(v);
    return seekBar;
}

ECode SeekBarDialogPreference::SetDialogTitle(
    /* [in] */ ICharSequence* dialogTitle)
{
    return DialogPreference::SetDialogTitle(dialogTitle);
}

ECode SeekBarDialogPreference::SetDialogTitleEx(
    /* [in] */ Int32 dialogTitleResId)
{
    return DialogPreference::SetDialogTitleEx(dialogTitleResId);
}

ECode SeekBarDialogPreference::GetDialogTitle(
    /* [out] */ ICharSequence** title)
{
    return DialogPreference::GetDialogTitle(title);
}

ECode SeekBarDialogPreference::SetDialogMessage(
    /* [in] */ ICharSequence* dialogMessage)
{
    return DialogPreference::SetDialogMessage(dialogMessage);
}

ECode SeekBarDialogPreference::SetDialogMessageEx(
    /* [in] */ Int32 dialogMessageResId)
{
    return DialogPreference::SetDialogMessageEx(dialogMessageResId);
}

ECode SeekBarDialogPreference::GetDialogMessage(
    /* [out] */ ICharSequence** message)
{
    return DialogPreference::GetDialogMessage(message);
}

ECode SeekBarDialogPreference::SetDialogIcon(
    /* [in] */ IDrawable* dialogIcon)
{
    return DialogPreference::SetDialogIcon(dialogIcon);
}

ECode SeekBarDialogPreference::SetDialogIconEx(
    /* [in] */ Int32 dialogIconRes)
{
    return DialogPreference::SetDialogIconEx(dialogIconRes);
}

ECode SeekBarDialogPreference::GetDialogIcon(
    /* [out] */ IDrawable** icon)
{
    return DialogPreference::GetDialogIcon(icon);
}

ECode SeekBarDialogPreference::SetPositiveButtonText(
    /* [in] */ ICharSequence* positiveButtonText)
{
    return DialogPreference::SetPositiveButtonText(positiveButtonText);
}

ECode SeekBarDialogPreference::SetPositiveButtonTextEx(
    /* [in] */ Int32 positiveButtonTextResId)
{
    return DialogPreference::SetPositiveButtonTextEx(positiveButtonTextResId);
}

ECode SeekBarDialogPreference::GetPositiveButtonText(
    /* [out] */ ICharSequence** text)
{
    return DialogPreference::GetPositiveButtonText(text);
}

ECode SeekBarDialogPreference::SetNegativeButtonText(
    /* [in] */ ICharSequence* negativeButtonText)
{
    return DialogPreference::SetNegativeButtonText(negativeButtonText);
}

ECode SeekBarDialogPreference::SetNegativeButtonTextEx(
    /* [in] */ Int32 negativeButtonTextResId)
{
    return DialogPreference::SetNegativeButtonTextEx(negativeButtonTextResId);
}

ECode SeekBarDialogPreference::GetNegativeButtonText(
    /* [out] */ ICharSequence** text)
{
    return DialogPreference::GetNegativeButtonText(text);
}

ECode SeekBarDialogPreference::SetDialogLayoutResource(
    /* [in] */ Int32 dialogLayoutResId)
{
    return DialogPreference::SetDialogLayoutResource(dialogLayoutResId);
}

ECode SeekBarDialogPreference::GetDialogLayoutResource(
    /* [out] */ Int32* layoutResId)
{
    return DialogPreference::GetDialogLayoutResource(layoutResId);
}

ECode SeekBarDialogPreference::OnPrepareDialogBuilder(
    /* [in] */ IAlertDialogBuilder* builder)
{
    return DialogPreference::OnPrepareDialogBuilder(builder);
}

ECode SeekBarDialogPreference::ShowDialog(
    /* [in] */ IBundle* state)
{
    return DialogPreference::ShowDialog(state);
}

ECode SeekBarDialogPreference::NeedInputMethod(
    /* [out] */ Boolean* isNeed)
{
    return DialogPreference::NeedInputMethod(isNeed);
}

ECode SeekBarDialogPreference::OnCreateDialogView(
    /* [out] */ IView** view)
{
    return DialogPreference::OnCreateDialogView(view);
}

ECode SeekBarDialogPreference::OnDialogClosed(
    /* [in] */ Boolean positiveResult)
{
    return DialogPreference::OnDialogClosed(positiveResult);
}

ECode SeekBarDialogPreference::GetDialog(
    /* [out] */ IDialog** dialog)
{
    return DialogPreference::GetDialog(dialog);
}

}
}
}

