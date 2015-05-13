
#ifndef __CPAIR_H__
#define __CPAIR_H__

#include "_CPair.h"

namespace Elastos {
namespace Droid {
namespace Utility {

CarClass(CPair)
{
public:
    CARAPI GetFirst(
        /* [out] */ IInterface **first);

    CARAPI GetSecond(
        /* [out] */ IInterface **second);

    CARAPI Equals(
        /* [in] */ IInterface *o,
        /* [out] */ Boolean *isEqual);

    CARAPI GetHashCode(
        /* [out] */ Int32 *hashCode);

    CARAPI constructor(
        /* [in] */ IInterface *first,
        /* [in] */ IInterface *second);

    static CARAPI Create(
        /* [in] */ IInterface *A,
        /* [in] */ IInterface *B,
        /* [out] */ IPair **pair);

public:
    AutoPtr<IInterface> mFirst;
    AutoPtr<IInterface> mSecond;
};

} // namespace Utility
} // namespace Droid
} // namespace Elastos

#endif // __CPAIR_H__
