
#ifndef __CSPIANDPROVIDER_H__
#define __CSPIANDPROVIDER_H__

#include "_CSpiAndProvider.h"

using Elastos::Security::IProvider;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Fortress {

CarClass(CSpiAndProvider)
{
public:
    CARAPI SetSpi(
        /* [in] */ IInterface* spi);

    CARAPI GetSpi(
        /* [out] */ IInterface** spi);

    CARAPI SetProvider(
        /* [in] */ IProvider* provider);

    CARAPI GetProvider(
        /* [out] */ IProvider** provider);

    CSpiAndProvider(
        /* [in] */ IInterface* spi,
        /* [in] */ IProvider* provider);

public:
    AutoPtr<IInterface> mSpi;
    AutoPtr<IProvider> mProvider;
};

} // namespace Fortress
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __CSPIANDPROVIDER_H__
