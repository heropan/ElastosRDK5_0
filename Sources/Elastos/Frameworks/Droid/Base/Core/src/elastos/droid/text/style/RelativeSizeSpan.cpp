#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/text/style/RelativeSizeSpan.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CAR_INTERFACE_IMPL_3(RelativeSizeSpan, MetricAffectingSpan, IRelativeSizeSpan, IParcelableSpan, IParcelable)

RelativeSizeSpan::RelativeSizeSpan()
    : mProportion(0)
{}

RelativeSizeSpan::~RelativeSizeSpan()
{}

ECode RelativeSizeSpan::constructor(
    /* [in] */ Float proportion)
{
    mProportion = proportion;
    return NOERROR;
}

ECode RelativeSizeSpan::GetSpanTypeId(
    /* [in] */ Int32* id);
{
    VALIDATE_NOT_NULL(id)
    *id = ITextUtils::RELATIVE_SIZE_SPAN;
    return NOERROR;
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

ECode RelativeSizeSpan::GetSizeChange(
    /* [out] */ Float* change)
{
    VALIDATE_NOT_NULL(change)
    *change = mProportion;
    return NOERROR;
}

ECode RelativeSizeSpan::UpdateDrawState(
    /* [in] */ ITextPaint* ds)
{
    VALIDATE_NOT_NULL(ds);
    Float textSize;
    ds->GetTextSize(&textSize);
    ds->SetTextSize(textSize * mProportion);
    return NOERROR;
}

ECode RelativeSizeSpan::UpdateMeasureState(
    /* [in] */ ITextPaint* ds)
{
    Float textSize;
    ds->GetTextSize(&textSize);
    ds->SetTextSize(textSize * mProportion);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastoss