#include "elastos/droid/text/style/BackgroundColorSpan.h"
#include "elastos/droid/text/CTextPaint.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

BackgroundColorSpan::BackgroundColorSpan()
{}

BackgroundColorSpan::BackgroundColorSpan(
    /* [in] */ Int32 color)
{
    Init(color);
}

BackgroundColorSpan::BackgroundColorSpan(
    /* [in] */ IParcel* src)
{
    Init(src);
}

void BackgroundColorSpan::Init(
    /* [in] */ Int32 color)
{
    mColor = color;
}

void BackgroundColorSpan::Init(
    /* [in] */ IParcel* src)
{
    ReadFromParcel(src);
}

Int32 BackgroundColorSpan::GetSpanTypeId()
{
    return ITextUtils::BACKGROUND_COLOR_SPAN;
}

Int32 BackgroundColorSpan::DescribeContents()
{
    return 0;
}


ECode BackgroundColorSpan::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return source->ReadInt32(&mColor);
}

ECode BackgroundColorSpan::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return dest->WriteInt32(mColor);
}

Int32 BackgroundColorSpan::GetBackgroundColor()
{
    return mColor;
}

ECode BackgroundColorSpan::UpdateDrawState(
    /* [in] */ ITextPaint* ds)
{
    VALIDATE_NOT_NULL(ds);
    ds->SetBgColor(mColor);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos