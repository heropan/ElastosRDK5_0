
#ifndef __CSERVERSOCKETHELPER_H__
#define __CSERVERSOCKETHELPER_H__

#include "_CServerSocketHelper.h"

namespace Elastos {
namespace Net {

CarClass(CServerSocketHelper)
{
public:
    CARAPI SetSocketFactory(
        /* [in] */ ISocketImplFactory* aFactory);
};

} // namespace Net
} // namespace Elastos

#endif //__CSERVERSOCKETHELPER_H__
