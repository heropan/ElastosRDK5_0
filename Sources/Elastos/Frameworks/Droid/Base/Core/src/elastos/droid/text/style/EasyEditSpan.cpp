#include "elastos/droid/text/style/EasyEditSpan.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

EasyEditSpan::EasyEditSpan()
{
    // Empty
}

Int32 EasyEditSpan::DescribeContents()
{
    return 0;
}

ECode EasyEditSpan::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return NOERROR;
}

ECode EasyEditSpan::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return NOERROR;
}

Int32 EasyEditSpan::GetSpanTypeId()
{
    return ITextUtils::EASY_EDIT_SPAN;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos