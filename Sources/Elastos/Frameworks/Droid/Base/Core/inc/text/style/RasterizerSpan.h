#ifndef __RasterizerSpan_H__
#define __RasterizerSpan_H__

#include "text/style/CharacterStyle.h"

using Elastos::Droid::Graphics::IRasterizer;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

class RasterizerSpan
    : public CharacterStyle
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

#endif // __RasterizerSpan_H__
