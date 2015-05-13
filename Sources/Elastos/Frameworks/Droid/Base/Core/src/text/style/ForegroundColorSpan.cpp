#include "ext/frameworkext.h"
#include "text/style/ForegroundColorSpan.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

ForegroundColorSpan::ForegroundColorSpan()
{}

ForegroundColorSpan::ForegroundColorSpan(
    /* [in] */ Int32 color)
{
    Init(color);
}

ForegroundColorSpan::ForegroundColorSpan(
    /* [in] */ IParcel* src)
{
    Init(src);
}

void ForegroundColorSpan::Init(
    /* [in] */ Int32 color)
{
    mColor = color;
}

void ForegroundColorSpan::Init(
    /* [in] */ IParcel* src)
{
    ReadFromParcel(src);
}

Int32 ForegroundColorSpan::GetSpanTypeId()
{
    return ITextUtils::FOREGROUND_COLOR_SPAN;
}

Int32 ForegroundColorSpan::DescribeContents()
{
    return 0;
}

ECode ForegroundColorSpan::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return source->ReadInt32(&mColor);
}

ECode ForegroundColorSpan::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return dest->WriteInt32(mColor);;
}

Int32 ForegroundColorSpan::GetForegroundColor()
{
    return mColor;
}

ECode ForegroundColorSpan::UpdateDrawState(
    /* [in] */ ITextPaint* ds)
{
    VALIDATE_NOT_NULL(ds);
    ds->SetColor(mColor);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos