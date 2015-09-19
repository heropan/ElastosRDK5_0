
#ifndef __ELASTOS_DROID_WIDGET_INTERNAL_PASSWORDENTRYKEYBOARDVIEW_H__
#define __ELASTOS_DROID_WIDGET_INTERNAL_PASSWORDENTRYKEYBOARDVIEW_H__

#include "inputmethodservice/KeyboardView.h"

using Elastos::Droid::InputMethodService::KeyboardView;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

class PasswordEntryKeyboardView : public KeyboardView
{
public:
    PasswordEntryKeyboardView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    PasswordEntryKeyboardView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    virtual CARAPI SetShifted(
        /* [in] */ Boolean shifted,
        /* [out] */ Boolean* res);
protected:
    PasswordEntryKeyboardView();

    static const Int32 KEYCODE_OPTIONS = -100;
    static const Int32 KEYCODE_SHIFT_LONGPRESS = -101;
    static const Int32 KEYCODE_VOICE = -102;
    static const Int32 KEYCODE_F1 = -103;
    static const Int32 KEYCODE_NEXT_LANGUAGE = -104;

};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif
