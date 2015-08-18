
#ifndef __ELASTOS_DROID_NET_CLOCALSERVERSOCKET_H__
#define __ELASTOS_DROID_NET_CLOCALSERVERSOCKET_H__

#include "_Elastos_Droid_Net_CLocalServerSocket.h"
#include "net/LocalSocketImpl.h"

using Elastos::IO::IFileDescriptor;

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CLocalServerSocket)
{
public:
    CARAPI constructor(
        /* [in] */ const String& name);

    CARAPI constructor(
        /* [in] */ IFileDescriptor* fd);

    CARAPI GetLocalSocketAddress(
        /* [out] */ ILocalSocketAddress** result);

    CARAPI Accept(
        /* [out] */ ILocalSocket** result);

    CARAPI GetFileDescriptor(
        /* [out] */ IFileDescriptor** result);

    CARAPI Close();

private:
    AutoPtr<LocalSocketImpl> mImpl;
    AutoPtr<ILocalSocketAddress> mLocalAddress;

    /** 50 seems a bit much, but it's what was here */
    static const Int32 LISTEN_BACKLOG = 50;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_NET_CLOCALSERVERSOCKET_H__
