
#ifndef __CDATAGRAMPACKET_H__
#define __CDATAGRAMPACKET_H__

#include "_CDatagramPacket.h"
#include <elastos/Mutex.h>

using Elastos::Core::Threading::Mutex;

namespace Elastos {
namespace Net {

CarClass(CDatagramPacket)
{
public:
    CDatagramPacket();

    ~CDatagramPacket();

    CARAPI constructor(
        /* [in] */ ArrayOf<Byte>* data,
        /* [in] */ Int32 length);

    CARAPI constructor(
        /* [in] */ ArrayOf<Byte>* data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length);

    CARAPI constructor(
        /* [in] */ ArrayOf<Byte>* data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ IInetAddress* host,
        /* [in] */ Int32 aPort);

    CARAPI constructor(
        /* [in] */ ArrayOf<Byte>* data,
        /* [in] */ Int32 length,
        /* [in] */ IInetAddress* host,
        /* [in] */ Int32 port);

    CARAPI constructor(
        /* [in] */ ArrayOf<Byte>* data,
        /* [in] */ Int32 length,
        /* [in] */ ISocketAddress* sockAddr);

    CARAPI constructor(
        /* [in] */ ArrayOf<Byte>* data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ ISocketAddress* sockAddr);

    CARAPI ResetLengthForReceive();

    CARAPI SetReceivedLength(
        /* [in] */ Int32 length);

    CARAPI GetAddress(
        /* [out] */ IInetAddress** address);

    CARAPI GetData(
        /* [out, callee] */ ArrayOf<Byte>** data);

    CARAPI GetLength(
        /* [out] */ Int32* length);

    CARAPI GetOffset(
        /* [out] */ Int32* offset);

    CARAPI GetPort(
        /* [out] */ Int32* port);

    CARAPI SetAddress(
        /* [in] */ IInetAddress* addr);

    CARAPI SetData(
        /* [in] */ ArrayOf<Byte>* buf);

    CARAPI SetDataEx(
        /* [in] */ ArrayOf<Byte>* buf,
        /* [in] */ Int32 anOffset,
        /* [in] */ Int32 aLength);

    CARAPI SetLength(
        /* [in] */ Int32 len);

    CARAPI SetPort(
        /* [in] */ Int32 aPort);

    CARAPI GetSocketAddress(
        /* [out] */ ISocketAddress** sockAddr);

    CARAPI SetSocketAddress(
        /* [in] */ ISocketAddress* sockAddr);

private:
    AutoPtr<ArrayOf<Byte> > mData;

    Int32 mLength;

    Int32 mUserSuppliedLength;

    AutoPtr<IInetAddress> mAddress;

    Int32 mPort; // The default port number is -1

    Int32 mOffset;
};

} // namespace Net
} // namespace Elastos

#endif //__CDATAGRAMPACKET_H__
