
#ifndef __ELASTOS_DROID_NET_CLOCALSOCKET_H__
#define __ELASTOS_DROID_NET_CLOCALSOCKET_H__

#include "_CLocalSocket.h"
#include "net/LocalSocketImpl.h"
#include <elastos/Mutex.h>

using Elastos::Core::Threading::Mutex;
using Elastos::IO::IFileDescriptor;

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CLocalSocket)
{
public:
    CLocalSocket();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IFileDescriptor* fd);

    CARAPI constructor(
        /* [in] */ Handle32 impl);

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI Connect(
        /* [in] */ ILocalSocketAddress* endpoint);

    CARAPI Bind(
        /* [in] */ ILocalSocketAddress* bindpoint);

    CARAPI GetLocalSocketAddress(
        /* [out] */ ILocalSocketAddress** address);

    CARAPI GetInputStream(
        /* [out] */ IInputStream** is);

    CARAPI GetOutputStream(
        /* [out] */ IOutputStream** os);

    CARAPI Close();

    CARAPI ShutdownInput();

    CARAPI ShutdownOutput();

    CARAPI SetReceiveBufferSize(
        /* [in] */ Int32 size);

    CARAPI GetReceiveBufferSize(
        /* [out] */ Int32* result);

    CARAPI SetSoTimeout(
        /* [in] */ Int32 n);

    CARAPI GetSoTimeout(
      /* [out] */ Int32* result);

    CARAPI SetSendBufferSize(
        /* [in] */ Int32 n);

    CARAPI GetSendBufferSize(
      /* [out] */ Int32* result);

    CARAPI GetRemoteSocketAddress(
      /* [out] */ ILocalSocketAddress** result);

    CARAPI IsConnected(
      /* [out] */ Boolean* result);

    CARAPI IsClosed(
      /* [out] */ Boolean* result);

    CARAPI IsBound(
      /* [out] */ Boolean* result);

    CARAPI IsOutputShutdown(
      /* [out] */ Boolean* result);

    CARAPI IsInputShutdown(
      /* [out] */ Boolean* result);

    CARAPI ConnectEx(
      /* [in] */ ILocalSocketAddress* endpoint,
      /* [in] */ Int32 timeout);

    CARAPI SetFileDescriptorsForSend(
      /* [in] */ ArrayOf<IFileDescriptor*>* fds);

    CARAPI GetAncillaryFileDescriptors(
        /* [out, callee] */ ArrayOf<IFileDescriptor*>** result);

    CARAPI GetPeerCredentials(
        /* [out] */ ICredentials** result);

    CARAPI GetFileDescriptor(
        /* [out] */ IFileDescriptor** result);

private:
    CARAPI_(void) ImplCreateIfNeeded();

private:
    AutoPtr<LocalSocketImpl> mImpl;
    Boolean mImplCreated;
    AutoPtr<ILocalSocketAddress> mLocalAddress;
    Boolean mIsBound;
    Boolean mIsConnected;
    Mutex mLock;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_NET_CLOCALSOCKET_H__
