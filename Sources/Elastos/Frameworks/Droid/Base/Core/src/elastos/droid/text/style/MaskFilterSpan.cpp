#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/text/style/MaskFilterSpan.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

MaskFilterSpan::MaskFilterSpan()
{}

MaskFilterSpan::MaskFilterSpan(
    /* [in] */ IMaskFilter* filter)
{
    Init(filter);
}

void MaskFilterSpan::Init(
    /* [in] */ IMaskFilter* filter)
{
    mFilter = filter;
}

AutoPtr<IMaskFilter> MaskFilterSpan::GetMaskFilter()
{
    return mFilter;
}

ECode MaskFilterSpan::UpdateDrawState(
    /* [in] */ ITextPaint* ds)
{
    VALIDATE_NOT_NULL(ds);
    ds->SetMaskFilter( mFilter );
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos