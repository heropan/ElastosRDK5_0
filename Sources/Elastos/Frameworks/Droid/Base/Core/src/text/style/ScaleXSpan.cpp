#include "ext/frameworkext.h"
#include "text/style/ScaleXSpan.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

ScaleXSpan::ScaleXSpan()
{}

ScaleXSpan::ScaleXSpan(
    /* [in] */ Float proportion)
{
    Init(proportion);
}

ScaleXSpan::ScaleXSpan(
    /* [in] */ IParcel* src)
{
    Init(src);
}

void ScaleXSpan::Init(
    /* [in] */ Float proportion)
{
    mProportion = proportion;
}

void ScaleXSpan::Init(
    /* [in] */ IParcel* src)
{
    ReadFromParcel(src);
}

Int32 ScaleXSpan::GetSpanTypeId()
{
    return ITextUtils::SCALE_X_SPAN;
}

Int32 ScaleXSpan::DescribeContents()
{
    return 0;
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
    ds->SetTextScaleX((ds->GetTextScaleX(&textScaleX), textScaleX) * mProportion);
    return NOERROR;
}

ECode ScaleXSpan::UpdateMeasureState(
    /* [in] */ ITextPaint* ds)
{
    Float textScaleX;
    ds->SetTextScaleX((ds->GetTextScaleX(&textScaleX), textScaleX) * mProportion);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos