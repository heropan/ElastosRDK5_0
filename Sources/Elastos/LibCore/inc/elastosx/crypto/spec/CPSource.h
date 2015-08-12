
#ifndef __CPSOURCE_H__
#define __CPSOURCE_H__

#include "_Elastosx_Crypto_Spec_CPSource.h"
#include "PSource.h"

namespace Elastosx {
namespace Crypto {
namespace Spec {

CarClass(CPSource)
    , public PSource
{
public:
    CAR_OBJECT_DECL()

};

}
}
}

#endif // __CPSOURCE_H__
