#ifndef __ELASTOS_DROID_INTERNAL_UTILITY_CFASTMATH_H__
#define __ELASTOS_DROID_INTERNAL_UTILITY_CFASTMATH_H__

#include "_Elastos_Droid_Internal_Utility_CFastMath.h"
#include <elastos/core/Singleton.h>

using Elastos::Core::Singleton;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Utility {

/**
 * Fast and loose math routines.
 */
CarClass(CFastMath)
    , public Singleton
    , public IFastMath
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    /**
     * Fast round from float to int. This is faster than Math.round()
     * thought it may return slightly different results. It does not try to
     * handle (in any meaningful way) NaN or infinities.
     */
    static Int32 Round(
        /* [in] */ Float value);

    CARAPI Round(
        /* [in] */ Float value,
        /* [out] */ Int32* result);
};

} // namespace Utility
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_INTERNAL_UTILITY_CFASTMATH_H__
