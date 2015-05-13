
#ifndef __CECPRIVATEKEYSPEC_H__
#define __CECPRIVATEKEYSPEC_H__

#include "_CECPrivateKeySpec.h"

using Elastos::Math::IBigInteger;

namespace Elastos {
namespace Security {
namespace Spec {

CarClass(CECPrivateKeySpec)
{
public:
    CARAPI GetParams(
        /* [out] */ IECParameterSpec **params);

    CARAPI GetS(
        /* [out] */ IBigInteger **s);

    CARAPI constructor(
        /* [in] */ IBigInteger *s,
        /* [in] */ IECParameterSpec *params);

private:
    // Private value associated with this key
    const AutoPtr<IBigInteger> mS;
    // Elliptic Curve domain parameters associated with this key
    const AutoPtr<IECParameterSpec> mParams;
};

}
}
}

#endif // __CECPRIVATEKEYSPEC_H__
