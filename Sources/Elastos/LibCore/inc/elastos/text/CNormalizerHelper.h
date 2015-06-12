
#ifndef __ELASTOS_TEXT_CNORMALIZERHELPER_H__
#define __ELASTOS_TEXT_CNORMALIZERHELPER_H__

#include "_Elastos_Text_CNormalizerHelper.h"

using Elastos::Utility::ILocale;
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

#endif // __ELASTOS_TEXT_CNORMALIZERHELPER_H__
