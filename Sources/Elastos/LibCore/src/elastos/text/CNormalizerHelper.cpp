
#include "CNormalizerHelper.h"
// #include "CNativeNormalizerHelper.h"

using Libcore::ICU::INativeNormalizerHelper;
// using Libcore::ICU::CNativeNormalizerHelper;

namespace Elastos {
namespace Text {

CAR_INTERFACE_IMPL(CNormalizerHelper, Singleton, INormalizer)

CAR_SINGLETON_IMPL(CNormalizerHelper)

ECode CNormalizerHelper::IsNormalized(
    /* [in] */ ICharSequence* src,
    /* [in] */ NormalizerForm form,
    /* [out] */ Boolean* isNormalized)
{
    VALIDATE_NOT_NULL(src)
    VALIDATE_NOT_NULL(isNormalized)

    AutoPtr<INativeNormalizerHelper> normhelper;
    assert(0 && "TODO");
    // FAIL_RETURN(CNativeNormalizerHelper::AcquireSingleton((INativeNormalizerHelper **)&normhelper));
    return normhelper->IsNormalized(src,form,isNormalized);
}

ECode CNormalizerHelper::Normalize(
    /* [in] */ ICharSequence* src,
    /* [in] */ NormalizerForm form,
    /* [out] */ String* normalizeString)
{
    VALIDATE_NOT_NULL(normalizeString)
    *normalizeString = String(NULL);
    VALIDATE_NOT_NULL(src)

    AutoPtr<INativeNormalizerHelper> normhelper;
    assert(0 && "TODO");
    // FAIL_RETURN(CNativeNormalizerHelper::AcquireSingleton((INativeNormalizerHelper **)&normhelper));
    return normhelper->Normalize(src,form,normalizeString);
}

} // namespace Text
} // namespace Elastos

