#ifndef __CPROXYHELPER_H__
#define __CPROXYHELPER_H__

#include <_CProxyHelper.h>

namespace Elastos {
namespace Net {

CarClass(CProxyHelper)
{
public:
    GetNO_PROXY(
        /* [out] */ IProxy** proxy);
};

} // namespace Net
} // namespace Elastos

#endif // __CPROXYHELPER_H__
