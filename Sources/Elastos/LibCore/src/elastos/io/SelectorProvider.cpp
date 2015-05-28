#include "SelectorProvider.h"
#include "CSelectorProviderImpl.h"
#include "coredef.h"

namespace Elastos {
namespace IO {
namespace Channels {
namespace Spi {

AutoPtr<ISelectorProvider> SelectorProvider::sProvider;

SelectorProvider::SelectorProvider()
{
}

SelectorProvider::~SelectorProvider()
{
}

ECode SelectorProvider::Provider(
    /* [out] */ ISelectorProvider** provider)
{
    if (NULL == sProvider) {
        AutoPtr<CSelectorProviderImpl> impl;
        ASSERT_SUCCEEDED(CSelectorProviderImpl::NewByFriend((CSelectorProviderImpl**)&impl));
        sProvider = (ISelectorProvider*)impl.Get();
    }

    *provider = sProvider;
    REFCOUNT_ADD(*provider);
    return NOERROR;
}

ECode SelectorProvider::LoadProviderByJar(
    /* [out] */ ISelectorProvider** provider)
{
    printf("ERROR: Need ServiceLoader\n");
    return E_NOT_IMPLEMENTED;
}

ECode SelectorProvider::InheriteChannel(
    /* [out] */ IChannel** cannel)
{
    // Android never has stdin/stdout connected to a socket.
    return E_NOT_IMPLEMENTED;
}

} // namespace Spi
} // namespace Channels
} // namespace IO
} // namespace Elastos
