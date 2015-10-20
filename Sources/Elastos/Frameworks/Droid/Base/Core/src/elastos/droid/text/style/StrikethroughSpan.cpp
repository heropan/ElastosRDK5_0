#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/text/style/StrikethroughSpan.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

StrikethroughSpan::StrikethroughSpan()
{
}

StrikethroughSpan::~StrikethroughSpan()
{
}

ECode StrikethroughSpan::constructor()
{
    return NOERROR;
}


ECode StrikethroughSpan::GetSpanTypeId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id)
    *id = ITextUtils::STRIKETHROUGH_SPAN;
    return NOERROR;
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