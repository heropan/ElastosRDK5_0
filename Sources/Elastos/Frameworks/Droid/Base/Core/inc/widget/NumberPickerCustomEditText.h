#ifndef __NUMBERPICKERCUSTOMEDITTEXT__H_
#define __NUMBERPICKERCUSTOMEDITTEXT__H_

#include "widget/EditText.h"

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

#endif //__NUMBERPICKERCUSTOMEDITTEXT__H_