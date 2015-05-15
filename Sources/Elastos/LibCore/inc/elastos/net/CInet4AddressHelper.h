#ifndef __CINET4ADDRESSHELPER_H__
#define __CINET4ADDRESSHELPER_H__

#include "_CInet4AddressHelper.h"
#include "InetAddress.h"

namespace Elastos {
namespace Net {

CarClass(CInet4AddressHelper)
{
public:
    CARAPI GetANY(
        /* [out] */ IInetAddress** result);

    CARAPI GetLOOPBACK(
        /* [out] */ IInetAddress** result);

    CARAPI GetALL(
        /* [out] */ IInetAddress** result);
};

} // namespace Net
} // namespace Elastos

#endif //__CINET4ADDRESSHELPER_H__
