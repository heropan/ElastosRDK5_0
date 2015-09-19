#include "widget/internal/PasswordEntryKeyboardView.h"

using Elastos::Droid::InputMethodService::IKeyboard;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

PasswordEntryKeyboardView::PasswordEntryKeyboardView()
{}

PasswordEntryKeyboardView::PasswordEntryKeyboardView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : KeyboardView(context, attrs)
{}

PasswordEntryKeyboardView::PasswordEntryKeyboardView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : KeyboardView(context, attrs, defStyle)
{}

ECode PasswordEntryKeyboardView::SetShifted(
    /* [in] */ Boolean shifted,
    /* [out] */ Boolean* res)
{
    ECode ec = KeyboardView::SetShifted(shifted, res);
    // invalidate both shift keys
    AutoPtr<IKeyboard> kb;
    GetKeyboard((IKeyboard**)&kb);
    AutoPtr<ArrayOf<Int32> > indices;
    kb->GetShiftKeyIndices((ArrayOf<Int32>**)&indices);
    for (Int32 i = 0; i < indices->GetLength(); i++) {
        InvalidateKey((*indices)[i]);
    }
    return ec;
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos