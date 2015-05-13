#ifndef __COLLATIONKEYICU_H__
#define __COLLATIONKEYICU_H__

#include "Elastos.Core_server.h"
#include "CollationKey.h"

using Elastos::Text::CollationKey;
using Elastos::Text::ICollationKey;

extern "C" const InterfaceID EIID_CollationKeyICU;

namespace Libcore {
namespace ICU {

class CollationKeyICU : public CollationKey
{
public:
    CARAPI Init(
        /* [in] */ const String& source,
        /* [in] */ const ArrayOf<Byte>& bytes);

    ~CollationKeyICU();

    virtual CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid) = 0;

    CARAPI CompareTo(
        /* [in] */ IInterface* value,
        /* [out] */ Int32* result);

    CARAPI GetSourceString(
        /* [out] */ String* sourceString);

    CARAPI Equals(
        /* [in] */ IInterface * object,
        /* [out] */ Boolean * value);

    CARAPI GetHashCode(
        /* [out] */ Int32 * value);

    CARAPI ToByteArray(
        /* [out, callee] */ ArrayOf<Byte>** array);

private:
    /**
     * The key.
     */
    AutoPtr<ArrayOf<Byte> > mBytes;

    Int32 mHashCode;
};

} // namespace ICU
} // namespace Libcore

#endif //__COLLATIONKEYICU_H__
