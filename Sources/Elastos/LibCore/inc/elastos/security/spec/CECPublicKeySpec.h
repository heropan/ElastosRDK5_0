
#ifndef __ELASTOS_SECURITY_SPEC_CECPUBLICKEYSPEC_H__
#define __ELASTOS_SECURITY_SPEC_CECPUBLICKEYSPEC_H__

#include "_CECPublicKeySpec.h"

namespace Elastos {
namespace Security {
namespace Spec {

CarClass(CECPublicKeySpec)
{
public:
    CARAPI GetParams(
        /* [out] */ IECParameterSpec **params);

    CARAPI GetW(
        /* [out] */ IECPoint **w);

    CARAPI constructor(
        /* [in] */ IECPoint *w,
        /* [in] */ IECParameterSpec *params);

private:
    // The public point
    const AutoPtr<IECPoint> mW;
    // The associated elliptic curve domain parameters
    const AutoPtr<IECParameterSpec> mParams;
};

}
}
}

#endif // __ELASTOS_SECURITY_SPEC_CECPUBLICKEYSPEC_H__
