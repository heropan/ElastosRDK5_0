#include "text/style/SubscriptSpan.h"
#include "text/CTextPaint.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

SubscriptSpan::SubscriptSpan()
{

}

SubscriptSpan::SubscriptSpan(
    /* [in] */ IParcel* src)
{

}

void SubscriptSpan::Init()
{

}

void SubscriptSpan::Init(
    /* [in] */ IParcel* src)
{

}

Int32 SubscriptSpan::GetSpanTypeId()
{
    return ITextUtils::SUBSCRIPT_SPAN;
}

Int32 SubscriptSpan::DescribeContents()
{
    return 0;
}

ECode SubscriptSpan::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return NOERROR;
}

ECode SubscriptSpan::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return NOERROR;
}

ECode SubscriptSpan::UpdateDrawState(
    /* [in] */ ITextPaint* tp)
{
    VALIDATE_NOT_NULL(tp);
    Float fAscent;
    Int32 baselineShift;
    tp->SetBaselineShift((tp->GetBaselineShift(&baselineShift), baselineShift)-(Int32) ((tp->Ascent(&fAscent), fAscent) / 2));
    return NOERROR;
}

ECode SubscriptSpan::UpdateMeasureState(
    /* [in] */ ITextPaint* tp)
{
    Float fAscent;
    Int32 baselineShift;
    tp->SetBaselineShift((tp->GetBaselineShift(&baselineShift), baselineShift)-(Int32) ((tp->Ascent(&fAscent), fAscent) / 2));
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos