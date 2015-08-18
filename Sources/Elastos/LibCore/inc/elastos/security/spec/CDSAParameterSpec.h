
#ifndef __ELASTOS_SECURITY_SPEC_CDSAPARAMETERSPEC_H__
#define __ELASTOS_SECURITY_SPEC_CDSAPARAMETERSPEC_H__

#include "_Elastos_Security_Spec_CDSAParameterSpec.h"

using Elastos::Math::IBigInteger;

namespace Elastos {
namespace Security {
namespace Spec {

CarClass(CDSAParameterSpec)
{
public:
    CARAPI GetG(
        /* [out] */ IBigInteger **base);

    CARAPI GetP(
        /* [out] */ IBigInteger **prime);

    CARAPI GetQ(
        /* [out] */ IBigInteger **subprime);

    CARAPI constructor(
        /* [in] */ IBigInteger *p,
        /* [in] */ IBigInteger *q,
        /* [in] */ IBigInteger *g);

private:
    // Prime
    AutoPtr<IBigInteger> mP;
    // Sub-prime
    AutoPtr<IBigInteger> mQ;
    // Base
    AutoPtr<IBigInteger> mG;
};

}
}
}

#endif // __ELASTOS_SECURITY_SPEC_CDSAPARAMETERSPEC_H__
