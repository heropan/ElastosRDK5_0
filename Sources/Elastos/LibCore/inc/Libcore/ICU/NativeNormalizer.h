
#ifndef __NATIVENORMALIZER_H__
#define __NATIVENORMALIZER_H__

#include <elastos.h>
#include <cmdef.h>
#include "Elastos.Core_server.h"

using Elastos::Core::ICharSequence;

namespace Libcore {
namespace ICU {

class NativeNormalizer
{
public:
    static CARAPI IsNormalized(
        /* [in] */ ICharSequence* src,
        /* [in] */ Form form,
        /* [out] */ Boolean* isNormalized);

    static CARAPI Normalize(
        /* [in] */ ICharSequence* src,
        /* [in] */ Form form,
        /* [out] */ String* result);

private:
    NativeNormalizer() {}

    static CARAPI_(Int32) ToUNormalizationMode(
        /* [in] */ Form form);

    static CARAPI NormalizeImpl(
        /* [in] */ const String& src,
        /* [in] */ Int32 form,
        /* [out] */ String* result);

    static CARAPI IsNormalizedImpl(
        /* [in] */ const String& src,
        /* [in] */ Int32 form,
        /* [out] */ Boolean* isNormalized);
};

} // namespace ICU
} // namespace Libcore

#endif //__NATIVENORMALIZER_H__
