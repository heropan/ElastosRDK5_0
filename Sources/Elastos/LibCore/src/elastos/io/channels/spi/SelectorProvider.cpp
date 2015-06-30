#include "SelectorProvider.h"
#include "CSelectorProviderImpl.h"

namespace Elastos {
namespace IO {
namespace Channels {
namespace Spi {

CAR_INTERFACE_IMPL(SelectorProvider, Object, ISelectorProvider)

AutoPtr<ISelectorProvider> SelectorProvider::sProvider;

SelectorProvider::SelectorProvider()
{
}

SelectorProvider::~SelectorProvider()
{
}

ECode SelectorProvider::GetProvider(
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
    // for (SelectorProvider provider : ServiceLoader.load(SelectorProvider.class)) {
    //     return provider;
    // }
    // return null;
    return E_NOT_IMPLEMENTED;
}

ECode SelectorProvider::InheriteChannel(
    /* [out] */ IChannel** cannel)
{
    // // Android never has stdin/stdout connected to a socket.
    // return null;
    return E_NOT_IMPLEMENTED;
}

} // namespace Spi
} // namespace Channels
} // namespace IO
} // namespace Elastos
