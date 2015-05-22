
#ifndef __RAWSOCKET_H__
#define __RAWSOCKET_H__

#include "Elastos.CoreLibrary_server.h"

using Elastos::Core::ICloseGuard;
using Elastos::IO::IFileDescriptor;

namespace Elastos {
namespace Net {

class RawSocket
{
public:
    ~RawSocket();

    CARAPI Init(
        /* [in] */ const String& interfaceName,
        /* [in] */ Int16 protocolType);


    CARAPI Read(
        /* [in] */ ArrayOf<Byte>* packet,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 byteCount,
        /* [in] */ Int32 destPort,
        /* [in] */ Int32 timeoutMillis,
        /* [out] */ Int32* number);

    CARAPI Write(
        /* [in] */ ArrayOf<Byte>* destMac,
        /* [in] */ ArrayOf<Byte>* packet,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 byteCount,
        /* [out] */ Int32* number);

    CARAPI Close();

private:
    static CARAPI_(void) Create(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int16 protocolType,
        /* [in] */ const String& interfaceName);

    static CARAPI_(Int32) SendPacket(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ const String& interfaceName,
        /* [in] */ Int16 protocolType,
        /* [in] */ ArrayOf<Byte>* destMac,
        /* [in] */ ArrayOf<Byte>* packet,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 byteCount);

    static CARAPI_(Int32) RecvPacket(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ ArrayOf<Byte>* packet,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 byteCount,
        /* [in] */ Int32 destPort,
        /* [in] */ Int32 timeoutMillis);

private:
    AutoPtr<IFileDescriptor> mFd;
    String mMInterfaceName;
    Int16 mMProtocolType;
    AutoPtr<ICloseGuard> mGuard;
};

} // namespace Net
} // namespace Elastos

#endif //__RAWSOCKET_H__
