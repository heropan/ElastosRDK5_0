#ifndef __TEXTNORMALIZER_H__
#define __TEXTNORMALIZER_H__

#include "Elastos.CoreLibrary_server.h"
#include <elastos.h>

using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Text {

class TextNormalizer {
public:
    static CARAPI_(Boolean) IsNormalized(
        /* [in] */ ICharSequence* src,
        /* [in] */ NormalizerForm form);

    static CARAPI_(String) Normalize(
        /* [in] */ ICharSequence* src,
        /* [in] */ NormalizerForm form);

private:
    TextNormalizer();
};

} // namespace Text
} // namespace Elastos

#endif //__TEXTNORMALIZER_H__