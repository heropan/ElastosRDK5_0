
#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/text/style/CLeadingMarginSpanStandard.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

ECode CLeadingMarginSpanStandard::DescribeContents(
    /* [out] */ Int32* ret)
{
    *ret = 0;
    return NOERROR;
}

ECode CLeadingMarginSpanStandard::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    FAIL_RETURN(source->ReadInt32(&mFirst));
    FAIL_RETURN(source->ReadInt32(&mRest));
    return NOERROR;
}

ECode CLeadingMarginSpanStandard::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    FAIL_RETURN(dest->WriteInt32(mFirst));
    FAIL_RETURN(dest->WriteInt32(mRest));
    return NOERROR;
}

ECode CLeadingMarginSpanStandard::GetSpanTypeId(
    /* [out] */ Int32* ret)
{
    *ret = ITextUtils::LEADING_MARGIN_SPAN;
    return NOERROR;
}

ECode CLeadingMarginSpanStandard::GetLeadingMargin(
    /* [in] */ Boolean first,
    /* [out] */ Int32* offset)
{
    *offset = first ? mFirst : mRest;
    return NOERROR;
}

ECode CLeadingMarginSpanStandard::DrawLeadingMargin(
    /* [in] */ ICanvas* c,
    /* [in] */ IPaint* p,
    /* [in] */ Int32 x,
    /* [in] */ Int32 dir,
    /* [in] */ Int32 top,
    /* [in] */ Int32 baseline,
    /* [in] */ Int32 bottom,
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Boolean first,
    /* [in] */ ILayout* layout)
{
    return NOERROR;
}

ECode CLeadingMarginSpanStandard::constructor()
{
    return NOERROR;
}

ECode CLeadingMarginSpanStandard::constructor(
    /* [in] */ Int32 first,
    /* [in] */ Int32 rest)
{
    mFirst = first;
    mRest = rest;
    return NOERROR;
}

ECode CLeadingMarginSpanStandard::constructor(
    /* [in] */ Int32 every)
{
    mFirst = every;
    mRest = every;
    return NOERROR;
}

ECode CLeadingMarginSpanStandard::constructor(
    /* [in] */ IParcel* src)
{
    return ReadFromParcel(src);
}


} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos
