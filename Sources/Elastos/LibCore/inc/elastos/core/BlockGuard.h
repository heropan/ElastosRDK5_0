#ifndef __BLOCKGUARD_H__
#define __BLOCKGUARD_H__

#include "Elastos.CoreLibrary_server.h"
#include <elastos/Object.h>

using Elastos::Core::Object;

namespace Elastos {
namespace Core {

class BlockGuard
{
public:
    /**
     * The default, permissive policy that doesn't prevent any operations.
     */
    class LAXPOLICY
        : public ElRefBase
        , public IBlockGuardPolicy
    {
    public:
        CAR_INTERFACE_DECL();

        ~LAXPOLICY();

        CARAPI OnWriteToDisk();

        CARAPI OnReadFromDisk();

        CARAPI OnNetwork();

        CARAPI GetPolicyMask(
            /* [out] */ Int32* mask);
    };

public:
    virtual ~BlockGuard();

    /**
     * Get the current thread's policy.
     *
     * @return the current thread's policy.  Never returns null.
     *     Will return the LAX_POLICY instance if nothing else is set.
     */
    static CARAPI GetThreadPolicy(
        /* [out] */ IBlockGuardPolicy** policy);

    static CARAPI_(AutoPtr<IBlockGuardPolicy>) GetThreadPolicy();

    /**
     * Sets the current thread's block guard policy.
     *
     * @param policy policy to set.  May not be null.  Use the public LAX_POLICY
     *   if you want to unset the active policy.
     */
    static CARAPI SetThreadPolicy(
        /* [in] */ IBlockGuardPolicy* policy);

    static CARAPI GetLaxPolicy(
        /* [out] */ IBlockGuardPolicy** policy);

public:
    /**
     * The default, permissive policy that doesn't prevent any operations.
     */
    static AutoPtr<IBlockGuardPolicy> sPolicy;
    static Object sLock;
    static pthread_once_t sKeyOnce;
    static pthread_key_t sTlsKey;
};

} // namespace Core
} // namespace Elastos

#endif //__BLOCKGUARD_H__
