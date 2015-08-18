
#ifndef __ELASTOS_SECURITY_SPEC_CECPARAMETERSPEC_H__
#define __ELASTOS_SECURITY_SPEC_CECPARAMETERSPEC_H__

#include "_Elastos_Security_Spec_CECParameterSpec.h"

using Elastos::Math::IBigInteger;

namespace Elastos {
namespace Security {
namespace Spec {

CarClass(CECParameterSpec)
{
public:
    CARAPI GetCofactor(
        /* [out] */ Int32 *cofactor);

    CARAPI GetCurve(
        /* [out] */ IEllipticCurve **curve);

    CARAPI GetGenerator(
        /* [out] */ IECPoint **generator);

    CARAPI GetOrder(
        /* [out] */ IBigInteger **order);

    CARAPI constructor(
        /* [in] */ IEllipticCurve *curve,
        /* [in] */ IECPoint *generator,
        /* [in] */ IBigInteger *order,
        /* [in] */ Int32 cofactor);

private:
    // Elliptic curve for which this is parameter
    AutoPtr<IEllipticCurve> mCurve;
    // Distinguished point on the elliptic curve called generator or base point
    AutoPtr<IECPoint> mGenerator;
    // Order of the generator
    AutoPtr<IBigInteger> mOrder;
    // Cofactor
    Int32 mCofactor;
};

}
}
}

#endif // __ELASTOS_SECURITY_SPEC_CECPARAMETERSPEC_H__
