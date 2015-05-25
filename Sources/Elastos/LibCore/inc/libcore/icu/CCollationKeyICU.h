
#ifndef __ICU_CCOLLATIONKEYICU_H__
#define __ICU_CCOLLATIONKEYICU_H__

#include "_CCollationKeyICU.h"
#include "CollationKeyICU.h"

namespace Libcore {
namespace ICU {

CarClass(CCollationKeyICU) , public CollationKeyICU
{
public:
    CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

    CARAPI Equals(
        /* [in] */ IInterface * object,
        /* [out] */ Boolean * value);

    CARAPI GetHashCode(
        /* [out] */ Int32 * value);

    CARAPI CompareTo(
        /* [in] */ IInterface* value,
        /* [out] */ Int32* result);

    CARAPI GetSourceString(
        /* [out] */ String* sourceString);

    CARAPI ToByteArray(
        /* [out, callee] */ ArrayOf<Byte>** array);

    CARAPI constructor(
        /* [in] */ const String& source,
        /* [in] */ const ArrayOf<Byte>& bytes);
};

} // namespace ICU
} // namespace Libcore

#endif // __ICU_CCOLLATIONKEYICU_H__
