#include "ext/frameworkext.h"
#include "text/style/UnderlineSpan.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

UnderlineSpan::UnderlineSpan()
{
    Init();
}

UnderlineSpan::UnderlineSpan(
    /* [in] */ IParcel* src)
{
    Init(src);
}

void UnderlineSpan::Init()
{
    return;
}

void UnderlineSpan::Init(
    /* [in] */ IParcel* src)
{
    return;
}

Int32 UnderlineSpan::GetSpanTypeId()
{
    return ITextUtils::UNDERLINE_SPAN;
}

Int32 UnderlineSpan::DescribeContents()
{
    return 0;
}

ECode UnderlineSpan::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return NOERROR;
}

ECode UnderlineSpan::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return NOERROR;
}

ECode UnderlineSpan::UpdateDrawState(
    /* [in] */ ITextPaint* ds)
{
    VALIDATE_NOT_NULL(ds);
    ds->SetUnderlineText(TRUE);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos