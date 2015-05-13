#include "text/style/AbsoluteSizeSpan.h"
#include "text/CTextPaint.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

AbsoluteSizeSpan::AbsoluteSizeSpan()
{}

AbsoluteSizeSpan::AbsoluteSizeSpan(
    /* [in] */ Int32 size)
{
    Init(size);
}

AbsoluteSizeSpan::AbsoluteSizeSpan(
    /* [in] */ Int32 size,
    /* [in] */ Boolean dip)
{
    Init(size, dip);
}

AbsoluteSizeSpan::AbsoluteSizeSpan(
    /* [in] */ IParcel* src)
{
    Init(src);
}

void AbsoluteSizeSpan::Init(
    /* [in] */ Int32 size)
{
    mSize = size;
}

void AbsoluteSizeSpan::Init(
    /* [in] */ Int32 size,
    /* [in] */ Boolean dip)
{
    mSize = size;
    mDip = dip;
}

void AbsoluteSizeSpan::Init(
    /* [in] */ IParcel* src)
{
    ReadFromParcel(src);
}

Int32 AbsoluteSizeSpan::GetSpanTypeId()
{
    return ITextUtils::ABSOLUTE_SIZE_SPAN;
}

Int32 AbsoluteSizeSpan::DescribeContents()
{
    return 0;
}

ECode AbsoluteSizeSpan::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    FAIL_RETURN(source->ReadInt32(&mSize));
    FAIL_RETURN(source->ReadBoolean(&mDip));
    return NOERROR;
}

ECode AbsoluteSizeSpan::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    FAIL_RETURN(dest->WriteInt32(mSize));
    FAIL_RETURN(dest->WriteBoolean(mDip));
    return NOERROR;
}

Int32 AbsoluteSizeSpan::GetSize()
{
    return mSize;
}

Boolean AbsoluteSizeSpan::GetDip()
{
    return mDip;
}

ECode AbsoluteSizeSpan::UpdateDrawState(
    /* [in] */ ITextPaint* ds)
{
    VALIDATE_NOT_NULL(ds);
    if (mDip) {
        Float density;
        ds->SetTextSize(mSize * (ds->GetDensity(&density), density));
    } else {
        ds->SetTextSize(mSize);
    }
    return NOERROR;
}

ECode AbsoluteSizeSpan::UpdateMeasureState(
    /* [in] */ ITextPaint* ds)
{
    VALIDATE_NOT_NULL(ds);
    if (mDip) {
        Float density;
        ds->SetTextSize(mSize * (ds->GetDensity(&density), density));
    } else {
        ds->SetTextSize(mSize);
    }
    return NOERROR;
}


} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos