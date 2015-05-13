
#ifndef __COBJECTIDENTIFIERHELPER_H__
#define __COBJECTIDENTIFIERHELPER_H__

#include "_CObjectIdentifierHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

CarClass(CObjectIdentifierHelper)
{
public:
    CARAPI Validate(
        /* [in] */ ArrayOf<Int32>* oid);

    CARAPI ToString(
        /* [in] */ ArrayOf<Int32>* oid,
        /* [out] */ String* str);

    CARAPI ToIntArray(
        /* [in] */ const String& str,
        /* [out, callee] */ ArrayOf<Int32>** intArray);

    CARAPI IsOID(
        /* [in] */ const String& str,
        /* [out] */ Boolean* isOID);

private:
    // TODO: Add your private member variables here.
};

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __COBJECTIDENTIFIERHELPER_H__
