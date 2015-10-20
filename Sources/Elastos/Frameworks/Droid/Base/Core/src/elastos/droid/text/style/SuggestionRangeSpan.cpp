#include "elastos/droid/text/style/SuggestionRangeSpan.h"
#include "elastos/droid/text/CTextPaint.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

SuggestionRangeSpan::SuggestionRangeSpan()
{
    Init();
}

SuggestionRangeSpan::SuggestionRangeSpan(
    /* [in] */ IParcel* src)
{
    Init(src);
}

void SuggestionRangeSpan::Init()
{
    // 0 is a fully transparent black. Has to be set using #setBackgroundColor
    mBackgroundColor = 0;
}

void SuggestionRangeSpan::Init(
    /* [in] */ IParcel* src)
{
    ReadFromParcel(src);
}

Int32 SuggestionRangeSpan::DescribeContents()
{
    return 0;
}

ECode SuggestionRangeSpan::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return source->ReadInt32(&mBackgroundColor);
}

ECode SuggestionRangeSpan::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return dest->WriteInt32(mBackgroundColor);
}

Int32 SuggestionRangeSpan::GetSpanTypeId(
            /* [in] */ Int32* id);
{
    return ITextUtils::SUGGESTION_RANGE_SPAN;
}

void SuggestionRangeSpan::SetBackgroundColor(
    /* [in] */ Int32 backgroundColor)
{
    mBackgroundColor = backgroundColor;
}

ECode SuggestionRangeSpan::UpdateDrawState(
    /* [in] */ ITextPaint* tp)
{
    VALIDATE_NOT_NULL(tp);
    tp->SetBgColor(mBackgroundColor);
    return NOERROR;
}


} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos