
#ifndef __ELASTOS_CORE_CBLOCKGUARD_H__
#define __ELASTOS_CORE_CBLOCKGUARD_H__

#include "_Elastos_Core_CBlockGuard.h"
#include "BlockGuard.h"

namespace Elastos {
namespace Core {

CarClass(CBlockGuard)
    , public BlockGuard
{
public:
    CAR_SINGLETON_METHODS_DECL()

    /**
     * Get the current thread's policy.
     *
     * @return the current thread's policy.  Never returns null.
     *     Will return the LAX_POLICY instance if nothing else is set.
     */
    CARAPI GetThreadPolicy(
        /* [out] */ IBlockGuardPolicy** policy);


    /**
     * Sets the current thread's block guard policy.
     *
     * @param policy policy to set.  May not be null.  Use the public LAX_POLICY
     *   if you want to unset the active policy.
     */
    CARAPI SetThreadPolicy(
        /* [in] */ IBlockGuardPolicy* policy);

    CARAPI GetLaxPolicy(
        /* [out] */ IBlockGuardPolicy** policy);
};

} // namespace Core
} // namespace Elastos

#endif //__ELASTOS_CORE_CBLOCKGUARD_H__
