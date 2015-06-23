
#ifndef __SEEKBARPREFERENCE_H__
#define __SEEKBARPREFERENCE_H__

#include "DialogPreference.h"

using Elastos::Droid::Widget::ISeekBar;

namespace Elastos {
namespace Droid {
namespace Preference {

/**
 * @hide
 */
class SeekBarDialogPreference
    : public DialogPreference
    , public IDialogPreference
{
public:
    SeekBarDialogPreference(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CAR_INTERFACE_DECL()

    // Allow subclasses to override the action buttons
    virtual CARAPI_(void) CreateActionButtons();

    virtual CARAPI SetDialogTitle(
        /* [in] */ ICharSequence* dialogTitle);

    virtual CARAPI SetDialogTitle(
        /* [in] */ Int32 dialogTitleResId);

    virtual CARAPI GetDialogTitle(
        /* [out] */ ICharSequence** title);

    virtual CARAPI SetDialogMessage(
        /* [in] */ ICharSequence* dialogMessage);

    virtual CARAPI SetDialogMessage(
        /* [in] */ Int32 dialogMessageResId);

    virtual CARAPI GetDialogMessage(
        /* [out] */ ICharSequence** message);

    virtual CARAPI SetDialogIcon(
        /* [in] */ IDrawable* dialogIcon);

    virtual CARAPI SetDialogIcon(
        /* [in] */ Int32 dialogIconRes);

    virtual CARAPI GetDialogIcon(
        /* [out] */ IDrawable** icon);

    virtual CARAPI SetPositiveButtonText(
        /* [in] */ ICharSequence* positiveButtonText);

    virtual CARAPI SetPositiveButtonText(
        /* [in] */ Int32 positiveButtonTextResId);

    virtual CARAPI GetPositiveButtonText(
        /* [out] */ ICharSequence** text);

    virtual CARAPI SetNegativeButtonText(
        /* [in] */ ICharSequence* negativeButtonText);

    virtual CARAPI SetNegativeButtonText(
        /* [in] */ Int32 negativeButtonTextResId);

    virtual CARAPI GetNegativeButtonText(
        /* [out] */ ICharSequence** text);

    virtual CARAPI SetDialogLayoutResource(
        /* [in] */ Int32 dialogLayoutResId);

    virtual CARAPI GetDialogLayoutResource(
        /* [out] */ Int32* layoutResId);

    virtual CARAPI OnPrepareDialogBuilder(
        /* [in] */ IAlertDialogBuilder* builder);

    virtual CARAPI ShowDialog(
        /* [in] */ IBundle* state);

    virtual CARAPI NeedInputMethod(
        /* [out] */ Boolean* isNeed);

    virtual CARAPI OnCreateDialogView(
        /* [out] */ IView** view);

    virtual CARAPI OnDialogClosed(
        /* [in] */ Boolean positiveResult);

    virtual CARAPI GetDialog(
        /* [out] */ IDialog** dialog);

protected:
    virtual CARAPI OnBindDialogView(
        /* [in] */ IView* view);

    static CARAPI_(AutoPtr<ISeekBar>) GetSeekBar(
        /* [in] */ IView* dialogView);

private:
    static const String TAG;

    AutoPtr<IDrawable> mMyIcon;
};

}
}
}

#endif // __SEEKBARPREFERENCE_H__
