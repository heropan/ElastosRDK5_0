
#include "CSpiAndProvider.h"
#include <cmdef.h>

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Fortress {


ECode CSpiAndProvider::SetSpi(
    /* [in] */ IInterface* spi)
{
    mSpi = spi;
    return NOERROR;
}

ECode CSpiAndProvider::GetSpi(
    /* [out] */ IInterface** spi)
{
    VALIDATE_NOT_NULL(spi)
    *spi = mSpi;
    REFCOUNT_ADD(*spi)
    return NOERROR;
}

ECode CSpiAndProvider::SetProvider(
    /* [in] */ IProvider* provider)
{
    mProvider = provider;
    return NOERROR;
}

ECode CSpiAndProvider::GetProvider(
    /* [out] */ IProvider** provider)
{
    VALIDATE_NOT_NULL(provider)
    *provider = mProvider;
    REFCOUNT_ADD(*provider)
    return NOERROR;
}

CSpiAndProvider::CSpiAndProvider(
    /* [in] */ IInterface* spi,
    /* [in] */ IProvider* provider)
{
    mSpi = spi;
    mProvider = provider;
}

} // namespace Fortress
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

