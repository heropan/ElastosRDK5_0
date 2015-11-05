
#ifndef __ELASTOS_DROID_NET_CLOCALSOCKETADDRESSNAMESPACE_H__
#define __ELASTOS_DROID_NET_CLOCALSOCKETADDRESSNAMESPACE_H__

#include "_Elastos_Droid_Net_CLocalSocketAddressNamespace.h"
#include "elastos/droid/net/LocalSocketAddress.h"

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CLocalSocketAddressNamespace)
    , public LocalSocketAddressNamespace
{
public:
    CAR_OBJECT_DECL()
};

} // namespace Net
} // namespace Droid
} // namespace Elastos
#endif // __ELASTOS_DROID_NET_CLOCALSOCKETADDRESSNAMESPACE_H__
