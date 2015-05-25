
#ifndef __CASN1SEQUENCE_H__
#define __CASN1SEQUENCE_H__

#include "_CASN1Sequence.h"
#include <ASN1Sequence.h>
#include <Asn1TypeMacro.h>

using Elastos::IO::IInputStream;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

CarClass(CASN1Sequence), public ASN1Sequence
{
public:
    ASN1TYPE_METHODS_DECL()

    CARAPI SetType(
        /* [in] */ ArrayOf<IASN1Type *>* type);

    CARAPI GetType(
        /* [out, callee] */ ArrayOf<IASN1Type *>** type);

    CARAPI SetOPTIONAL(
        /* [in] */ ArrayOf<Boolean>* optional);

    CARAPI GetOPTIONAL(
        /* [out, callee] */ ArrayOf<Boolean>** optional);

    CARAPI SetDEFAULT(
        /* [in] */ ArrayOf<IInterface *>* def);

    CARAPI GetDEFAULT(
        /* [out, callee] */ ArrayOf<IInterface *>** def);

    CARAPI GetValues(
        /* [in] */ IInterface* object,
        /* [in] */ ArrayOf<IInterface*>* values);

    CARAPI constructor(
        /* [in] */ ArrayOf<IASN1Type *>* type);

private:
    // TODO: Add your private member variables here.
};

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __CASN1SEQUENCE_H__
