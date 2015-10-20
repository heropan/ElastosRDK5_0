
#include "elastos/droid/preference/SeekBarDialogPreference.h"
#include "elastos/droid/R.h"

using Elastos::Droid::Widget::IImageView;
using Elastos::Droid::R;

namespace Elastos {
namespace Droid {
namespace Preference {

const String SeekBarDialogPreference::TAG("SeekBarDialogPreference");

CAR_INTERFACE_IMPL(SeekBarDialogPreference, DialogPreference, ISeekBarDialogPreference)

ECode SeekBarDialogPreference::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes)
{
    DialogPreference::constructor(context, attrs, defStyleAttr, defStyleRes);
    SetDialogLayoutResource(R::layout::seekbar_dialog);
    CreateActionButtons();

    // Steal the XML dialogIcon attribute's value
    GetDialogIcon((IDrawable**)&mMyIcon);
    SetDialogIcon(NULL);
    return NOERROR;
}

ECode SeekBarDialogPreference::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr)
{
    return constructor(context, attrs, defStyleAttr, 0);
}

ECode SeekBarDialogPreference::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(context, attrs, R::attr::dialogPreferenceStyle);
}

ECode SeekBarDialogPreference::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, NULL);
}

ECode SeekBarDialogPreference::CreateActionButtons()
{
    SetPositiveButtonText(R::string::ok);
    SetNegativeButtonText(R::string::cancel);
    return NOERROR;
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
        AutoPtr<IView> v = IView::Probe(iconView);
        v->SetVisibility(IView::GONE);
    }
    return NOERROR;
}

}
}
}

