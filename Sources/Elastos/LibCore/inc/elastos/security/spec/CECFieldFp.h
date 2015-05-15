
#ifndef __CECFIELDFP_H__
#define __CECFIELDFP_H__

#include "_CECFieldFp.h"

using Elastos::Math::IBigInteger;

namespace Elastos {
namespace Security {
namespace Spec {

CarClass(CECFieldFp)
{
public:
    CARAPI GetFieldSize(
        /* [out] */ Int32 *size);

    CARAPI GetP(
        /* [out] */ IBigInteger **p);

    CARAPI Equals(
        /* [in] */ IInterface *obj,
        /* [out] */ Boolean *result);

    CARAPI GetHashCode(
        /* [out] */ Int32 *hashCode);

    CARAPI constructor(
        /* [in] */ IBigInteger *p);

private:
    // Prime
    AutoPtr<IBigInteger> mP;
};

}
}
}

#endif // __CECFIELDFP_H__
