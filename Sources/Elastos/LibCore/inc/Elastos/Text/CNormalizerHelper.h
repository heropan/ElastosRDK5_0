
#ifndef __CNORMALIZERHELPER_H__
#define __CNORMALIZERHELPER_H__

#include "_CNormalizerHelper.h"

using Libcore::ICU::ILocale;
using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Text {

CarClass(CNormalizerHelper)
{
public:
    CARAPI IsNormalized(
        /* [in] */ ICharSequence* src,
        /* [in] */ NormalizerForm form,
        /* [out] */ Boolean* isNormalized);

    CARAPI Normalize(
        /* [in] */ ICharSequence* src,
        /* [in] */ NormalizerForm form,
        /* [out] */ String* normalizeString);

};

} // namespace Text
} // namespace Elastos

#endif // __CNORMALIZERHELPER_H__
