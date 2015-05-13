
#ifndef __CRAWSOCKET_H__
#define __CRAWSOCKET_H__

#include "_CRawSocket.h"
#include "RawSocket.h"

namespace Elastos {
namespace Net {

CarClass(CRawSocket), public RawSocket
{
public:
    CARAPI constructor(
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
};

} // namespace Net
} // namespace Elastos

#endif //__CRAWSOCKET_H__
