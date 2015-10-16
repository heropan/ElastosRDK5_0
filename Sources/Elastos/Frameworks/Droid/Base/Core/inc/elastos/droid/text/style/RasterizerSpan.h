#ifndef __ELASTOS_DROID_TEXT_STYLE_RasterizerSpan_H__
#define __ELASTOS_DROID_TEXT_STYLE_RasterizerSpan_H__

#include "elastos/droid/text/style/CharacterStyle.h"

using Elastos::Droid::Graphics::IRasterizer;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

class RasterizerSpan
    : public CharacterStyle
    , public IRasterizerSpan
    , public IUpdateAppearance
{
public:
    RasterizerSpan(
        /* [in] */ IRasterizer* r);

    CARAPI_(void) Init(
        /* [in] */ IRasterizer* r);

    CARAPI_(AutoPtr<IRasterizer>) GetRasterizer();

    //@Override
    CARAPI UpdateDrawState(
        /* [in] */ ITextPaint* ds);

protected:
    RasterizerSpan();

private:
    AutoPtr<IRasterizer> mRasterizer;
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_RasterizerSpan_H__
