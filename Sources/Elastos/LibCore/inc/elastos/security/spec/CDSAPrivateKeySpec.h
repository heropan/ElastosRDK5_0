
#ifndef __CDSAPRIVATEKEYSPEC_H__
#define __CDSAPRIVATEKEYSPEC_H__

#include "_CDSAPrivateKeySpec.h"

using Elastos::Math::IBigInteger;

namespace Elastos {
namespace Security {
namespace Spec {

CarClass(CDSAPrivateKeySpec)
{
public:
    CARAPI GetG(
        /* [out] */ IBigInteger **g);

    CARAPI GetP(
        /* [out] */ IBigInteger **p);

    CARAPI GetQ(
        /* [out] */ IBigInteger **q);

    CARAPI GetX(
        /* [out] */ IBigInteger **x);

    CARAPI constructor(
        /* [in] */ IBigInteger *x,
        /* [in] */ IBigInteger *p,
        /* [in] */ IBigInteger *q,
        /* [in] */ IBigInteger *g);

private:
    // Private key
    AutoPtr<IBigInteger> mX;
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

#endif // __CDSAPRIVATEKEYSPEC_H__
