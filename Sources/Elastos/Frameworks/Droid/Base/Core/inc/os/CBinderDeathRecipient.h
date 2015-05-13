#ifndef __CBINDERDEATHRECIPIENT_H__
#define __CBINDERDEATHRECIPIENT_H__

#include "_CBinderDeathRecipient.h"

using Elastos::IO::IFileDescriptor;

namespace Elastos {
namespace Droid {
namespace Os {

CarClass(CBinderDeathRecipient)
{
    CARAPI ProxyDied();
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__CBINDERDEATHRECIPIENT_H__
