#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/text/style/RelativeSizeSpan.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

RelativeSizeSpan::RelativeSizeSpan()
{}

RelativeSizeSpan::RelativeSizeSpan(
    /* [in] */ Float proportion)
{
    Init(proportion);
}

RelativeSizeSpan::RelativeSizeSpan(
    /* [in] */ IParcel* src)
{
    Init(src);
}

void RelativeSizeSpan::Init(
    /* [in] */ Float proportion)
{
    mProportion = proportion;
}

void RelativeSizeSpan::Init(
    /* [in] */ IParcel* src)
{
    ReadFromParcel(src);
}

Int32 RelativeSizeSpan::GetSpanTypeId()
{
    return ITextUtils::RELATIVE_SIZE_SPAN;
}

Int32 RelativeSizeSpan::DescribeContents()
{
    return 0;
}

ECode RelativeSizeSpan::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return source->ReadFloat(&mProportion);
}

ECode RelativeSizeSpan::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return dest->WriteFloat(mProportion);
}

Float RelativeSizeSpan::GetSizeChange()
{
    return mProportion;
}

ECode RelativeSizeSpan::UpdateDrawState(
    /* [in] */ ITextPaint* ds)
{
    VALIDATE_NOT_NULL(ds);
    Float textSize;
    ds->SetTextSize((ds->GetTextSize(&textSize), textSize) * mProportion);
    return NOERROR;
}

ECode RelativeSizeSpan::UpdateMeasureState(
    /* [in] */ ITextPaint* ds)
{
    Float textSize;
    ds->SetTextSize((ds->GetTextSize(&textSize), textSize) * mProportion);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastoss