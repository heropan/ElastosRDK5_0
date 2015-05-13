#include "ext/frameworkext.h"
#include "widget/CNumberPickerHelper.h"

namespace Elastos{
namespace Droid{
namespace Widget{

ECode CNumberPickerHelper::GetTwoDigitFormatter(
    /* [out] */ INumberPickerFormatter** format)
{
    VALIDATE_NOT_NULL(format);
    AutoPtr<INumberPickerFormatter> formatter = NumberPicker::GetTwoDigitFormatter();
    *format = formatter;
    INTERFACE_ADDREF(*format);
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos