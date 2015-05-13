#include "CSelectorProviderHelper.h"
#include "SelectorProvider.h"

namespace Elastos {
namespace IO {
namespace Channels {
namespace Spi {

ECode CSelectorProviderHelper::GetProvider(
        /* [out] */ ISelectorProvider** provider)
{
    return SelectorProvider::Provider(provider);
}

} // namespace Spi
} // namespace Channels
} // namespace IO
} // namespace Elastos
