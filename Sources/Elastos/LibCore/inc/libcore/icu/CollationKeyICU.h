#ifndef __COLLATIONKEYICU_H__
#define __COLLATIONKEYICU_H__

#include "Elastos.CoreLibrary_server.h"
#if 0 // for compiling
#include "CollationKey.h"

using Elastos::Text::CollationKey;
using Elastos::Text::ICollationKey;
#else
#include <Object.h>
using Elastos::Core::Object;
#endif // #if 0

extern "C" const InterfaceID EIID_CollationKeyICU;

namespace Libcore {
namespace ICU {

class CollationKeyICU 
#if 0 // for compiling
    : public CollationKey
#else
    : public Object
#endif // #if 0
    , public ICollationKeyICU
{
public:
    CAR_INTERFACE_DECL()

    CARAPI constructor(
        /* [in] */ const String& source,
        /* [in] */ ArrayOf<Byte>* bytes);

    CARAPI CompareTo(
        /* [in] */ IInterface* value,
        /* [out] */ Int32* result);

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
