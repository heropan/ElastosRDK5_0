#include "elastos/droid/text/style/SuperscriptSpan.h"
#include "elastos/droid/text/CTextPaint.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

SuperscriptSpan::SuperscriptSpan()
{
    Init();
}

SuperscriptSpan::SuperscriptSpan(
    /* [in] */ IParcel* src)
{
    Init(src);
}

void SuperscriptSpan::Init()
{
}

void SuperscriptSpan::Init(
    /* [in] */ IParcel* src)
{
}

Int32 SuperscriptSpan::GetSpanTypeId(
            /* [in] */ Int32* id);
{
    return ITextUtils::SUPERSCRIPT_SPAN;
}

Int32 SuperscriptSpan::DescribeContents()
{
    return 0;
}

ECode SuperscriptSpan::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return NOERROR;
}

ECode SuperscriptSpan::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return NOERROR;
}

ECode SuperscriptSpan::UpdateDrawState(
    /* [in] */ ITextPaint* tp)
{
    VALIDATE_NOT_NULL(tp);
    Float fAscent;
    Int32 baselineShift;
    tp->SetBaselineShift((tp->GetBaselineShift(&baselineShift), baselineShift)+(Int32)((tp->Ascent(&fAscent), fAscent) / 2));
    return NOERROR;
}

ECode SuperscriptSpan::UpdateMeasureState(
    /* [in] */ ITextPaint* tp)
{
    Float fAscent;
    Int32 baselineShift;
    tp->SetBaselineShift((tp->GetBaselineShift(&baselineShift), baselineShift)+(Int32)((tp->Ascent(&fAscent), fAscent) / 2));
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos