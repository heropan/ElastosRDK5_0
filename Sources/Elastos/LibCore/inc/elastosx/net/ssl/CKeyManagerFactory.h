
#ifndef __ELASTOS_NET_SSL_CKEYMANAGERFACTORY_H__
#define __ELASTOS_NET_SSL_CKEYMANAGERFACTORY_H__

#include "_Elastosx_Net_Ssl_CKeyManagerFactory.h"
#include "KeyManagerFactory.h"

namespace Elastosx {
namespace Net {
namespace Ssl {

CarClass(CKeyManagerFactory)
    , public KeyManagerFactory
{
public:
    CAR_OBJECT_DECL()
};

} // namespace Ssl
} // namespace Net
} // namespace Elastosx

#endif //__ELASTOS_NET_SSL_CKEYMANAGERFACTORY_H__
