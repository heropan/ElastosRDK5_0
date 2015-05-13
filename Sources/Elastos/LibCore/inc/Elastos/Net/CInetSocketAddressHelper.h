
#ifndef __CINETSOCKETADDRESSHELPER_H__
#define __CINETSOCKETADDRESSHELPER_H__

#include "_CInetSocketAddressHelper.h"

namespace Elastos {
namespace Net {

CarClass(CInetSocketAddressHelper)
{
public:
    CARAPI CreateUnresolved(
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [out] */ IInetSocketAddress** addr);

};

} // namespace Net
} // namespace Elastos

#endif //__CINETSOCKETADDRESSHELPER_H__
