#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/text/style/ScaleXSpan.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

: public MetricAffectingSpan
, public IScaleXSpan
, public IParcelableSpan
, public IParcelable

CAR_INTERFACE_IMPL_3(ScaleXSpan, MetricAffectingSpan, IScaleXSpan, IParcelableSpan, IParcelable)

ScaleXSpan::ScaleXSpan()
    : mProportion(0)
{}

ECode ScaleXSpan::constructor(
    /* [in] */ Float proportion)
{
    mProportion = proportion;
    return NOERROR;
}

ECode ScaleXSpan::GetSpanTypeId(
    /* [in] */ Int32* id);
{
    VALIDATE_NOT_NULL(id)
    *id = ITextUtils::SCALE_X_SPAN;
    return NOERROR;
}

ECode ScaleXSpan::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return source->ReadFloat(&mProportion);
}

ECode ScaleXSpan::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return dest->WriteFloat(mProportion);
}

Float ScaleXSpan::GetScaleX()
{
    return mProportion;
}

ECode ScaleXSpan::UpdateDrawState(
    /* [in] */ ITextPaint* ds)
{
    VALIDATE_NOT_NULL(ds);
    Float textScaleX;
    ds->GetTextScaleX(&textScaleX);
    ds->SetTextScaleX(textScaleX * mProportion);
    return NOERROR;
}

ECode ScaleXSpan::UpdateMeasureState(
    /* [in] */ ITextPaint* ds)
{
    VALIDATE_NOT_NULL(ds);
    Float textScaleX;
    ds->GetTextScaleX(&textScaleX);
    ds->SetTextScaleX(textScaleX * mProportion);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos