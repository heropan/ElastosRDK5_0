
#ifndef __CSOCKETHELPER_H__
#define __CSOCKETHELPER_H__

#include "_CSocketHelper.h"

namespace Elastos {
namespace Net {

CarClass(CSocketHelper)
{
public:
    CARAPI SetSocketImplFactory(
        /* [in] */ ISocketImplFactory* fac);
};

} // namespace Net
} // namespace Elastos

#endif //__CSOCKETHELPER_H__
