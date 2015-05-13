
#include "ext/frameworkdef.h"
#include "text/style/CAlignmentSpanStandard.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

ECode CAlignmentSpanStandard::DescribeContents(
    /* [out] */ Int32* ret)
{
    *ret = 0;
    return NOERROR;
}

ECode CAlignmentSpanStandard::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return source->ReadInt32(&mAlignment);
}

ECode CAlignmentSpanStandard::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return dest->WriteInt32(mAlignment);;
}

ECode CAlignmentSpanStandard::GetSpanTypeId(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = ITextUtils::ALIGNMENT_SPAN;
    return NOERROR;
}

ECode CAlignmentSpanStandard::GetAlignment(
    /* [out] */ LayoutAlignment* alignment)
{
    VALIDATE_NOT_NULL(alignment);
    *alignment = mAlignment;
    return NOERROR;
}

ECode CAlignmentSpanStandard::constructor()
{
    return NOERROR;
}

ECode CAlignmentSpanStandard::constructor(
    /* [in] */ LayoutAlignment align)
{
    mAlignment = align;
    return NOERROR;
}

ECode CAlignmentSpanStandard::constructor(
    /* [in] */ IParcel* src)
{
    return ReadFromParcel(src);
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos
