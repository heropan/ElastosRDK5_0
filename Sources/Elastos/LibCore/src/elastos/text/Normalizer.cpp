
#include "Normalizer.h"
// #include "NativeNormalizer.h"

// using Libcore::ICU::NativeNormalizer;

namespace Elastos {
namespace Text {

Boolean Normalizer::IsNormalized(
    /* [in] */ ICharSequence* src,
    /* [in] */ NormalizerForm form)
{
    assert(src);

    String str("");
    src->ToString(&str);
    Boolean result;
    // NativeNormalizer::IsNormalized(str, form, &result);
    return result;
}

String Normalizer::Normalize(
    /* [in] */ ICharSequence* src,
    /* [in] */ NormalizerForm form)
{
    assert(src);

    String str("");
    src->ToString(&str);
    String result;
    // NativeNormalizer::Normalize(str, form, &result);
    return result;
}

} // namespace Text
} // namespace Elastos
