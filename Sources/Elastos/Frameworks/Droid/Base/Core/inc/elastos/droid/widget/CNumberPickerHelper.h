#ifndef __ELASTOS_DROID_WIDGET_CNUMBERPICKERHELPER_H__
#define __ELASTOS_DROID_WIDGET_CNUMBERPICKERHELPER_H__

#include "_Elastos_Droid_Widget_CNumberPickerHelper.h"
#include "elastos/droid/widget/NumberPicker.h"

namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CNumberPickerHelper)
{
    CARAPI GetTwoDigitFormatter(
        /* [out] */ INumberPickerFormatter** format);
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CNUMBERPICKERHELPER_H__
