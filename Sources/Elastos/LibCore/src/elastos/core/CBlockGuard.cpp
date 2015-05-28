
#include "CBlockGuard.h"

namespace Elastos {
namespace Core {

CAR_SINGLETON_METHODS_IMPL(CBlockGuard)

ECode CBlockGuard::GetThreadPolicy(
    /* [out] */ IBlockGuardPolicy** policy)
{
    VALIDATE_NOT_NULL(policy);

    return BlockGuard::GetThreadPolicy(policy);
}

ECode CBlockGuard::SetThreadPolicy(
    /* [in] */ IBlockGuardPolicy* policy)
{
    return BlockGuard::SetThreadPolicy(policy);
}

CARAPI CBlockGuard::GetLaxPolicy(
    /* [out] */ IBlockGuardPolicy** policy)
{
    VALIDATE_NOT_NULL(policy);

    return BlockGuard::GetLaxPolicy(policy);
}

} // namespace Core
} // namespace Elastos
