
#ifndef __ICU_CNATIVENORMALIZERHELPER_H__
#define __ICU_CNATIVENORMALIZERHELPER_H__

#include "_CNativeNormalizerHelper.h"

using Elastos::Core::ICharSequence;

namespace Libcore {
namespace ICU {

CarClass(CNativeNormalizerHelper)
{
public:
    CARAPI IsNormalized(
        /* [in] */ ICharSequence * src,
        /* [in] */ Form form,
        /* [out] */ Boolean * value);

    CARAPI Normalize(
        /* [in] */ ICharSequence * src,
        /* [in] */ Form form,
        /* [out] */ String * str);
};

} // namespace ICU
} // namespace Libcore

#endif //__ICU_CNATIVENORMALIZERHELPER_H__
