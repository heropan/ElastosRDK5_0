#include "ext/frameworkext.h"
#include "text/style/RasterizerSpan.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

RasterizerSpan::RasterizerSpan()
{}

RasterizerSpan::RasterizerSpan(
    /* [in] */ IRasterizer* r)
{
    Init(r);
}

void RasterizerSpan::Init(
    /* [in] */ IRasterizer* r)
{
    mRasterizer = r;
}

AutoPtr<IRasterizer> RasterizerSpan::GetRasterizer()
{
    return mRasterizer;
}

ECode RasterizerSpan::UpdateDrawState(
    /* [in] */ ITextPaint* ds)
{
    VALIDATE_NOT_NULL(ds);
    ds->SetRasterizer(mRasterizer);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos