#ifndef __ELASTOS_DROID_WIDGET_NUMBERPICKERCUSTOMEDITTEXT_H__
#define __ELASTOS_DROID_WIDGET_NUMBERPICKERCUSTOMEDITTEXT_H__

#include "elastos/droid/widget/EditText.h"

namespace Elastos {
namespace Droid {
namespace Widget {

class NumberPickerCustomEditText : public EditText
{
public:

    NumberPickerCustomEditText(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI OnEditorAction(
        /* [in] */ Int32 actionCode);

protected:
    NumberPickerCustomEditText();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_NUMBERPICKERCUSTOMEDITTEXT_H__
