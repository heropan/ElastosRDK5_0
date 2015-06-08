
#ifndef __ELASTOS_NET_NETWORKUTILITIES_H__
#define __ELASTOS_NET_NETWORKUTILITIES_H__

#include "Elastos.CoreLibrary_server.h"
#include <arpa/inet.h>

ELAPI ByteArrayToSocketAddress(
    /* [in] */ const ArrayOf<Byte>& byteArray,
    /* [in]*/ Int32 port,
    /* [out] */ sockaddr_storage* ss);


ELAPI SocketAddressToByteArray(
    /* [in] */ sockaddr_storage* ss,
    /* [out] */ ArrayOf<Byte>* ipAddress);


#endif //__ELASTOS_NET_NETWORKUTILITIES_H__

