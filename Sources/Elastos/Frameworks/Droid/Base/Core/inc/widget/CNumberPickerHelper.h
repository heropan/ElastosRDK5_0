#ifndef __CNUMBERPICKERHELPER_H_
#define __CNUMBERPICKERHELPER_H_

#include "_CNumberPickerHelper.h"
#include "widget/NumberPicker.h"

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

#endif //__CNUMBERPICKERHELPER_H_