#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/text/style/StrikethroughSpan.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

StrikethroughSpan::StrikethroughSpan()
{
}

StrikethroughSpan::StrikethroughSpan(
    /* [in] */ IParcel* src)
{
}

void StrikethroughSpan::Init()
{
}

void StrikethroughSpan::Init(
    /* [in] */ IParcel* src)
{
}

Int32 StrikethroughSpan::GetSpanTypeId()
{
    return ITextUtils::STRIKETHROUGH_SPAN;
}

Int32 StrikethroughSpan::DescribeContents()
{
    return 0;
}

ECode StrikethroughSpan::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return NOERROR;
}

ECode StrikethroughSpan::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return NOERROR;
}

ECode StrikethroughSpan::UpdateDrawState(
    /* [in] */ ITextPaint* ds)
{
    VALIDATE_NOT_NULL(ds);
    ds->SetStrikeThruText(TRUE);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos