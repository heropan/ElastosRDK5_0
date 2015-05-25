
#ifndef __CASN1SETOF_H__
#define __CASN1SETOF_H__

#include "_CASN1SetOf.h"
#include <ASN1SetOf.h>
#include <Asn1TypeMacro.h>

using Elastos::Utility::ICollection;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

CarClass(CASN1SetOf), public ASN1SetOf
{
public:
    ASN1TYPE_METHODS_DECL()

    CARAPI SetType(
        /* [in] */ IASN1Type* type);

    CARAPI GetType(
        /* [out] */ IASN1Type** type);

    CARAPI GetValues(
        /* [in] */ IInterface* object,
        /* [out] */ ICollection** values);

    CARAPI constructor(
        /* [in] */ IASN1Type* type);

private:
    // TODO: Add your private member variables here.
};

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __CASN1SETOF_H__
