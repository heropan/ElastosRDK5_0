
#ifndef __CRUNTIMEINIT_H__
#define __CRUNTIMEINIT_H__

#include "_CRuntimeInit.h"
#include "RuntimeInit.h"

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Os {

CarClass(CRuntimeInit), public RuntimeInit
{
public:
    CARAPI Main(
        /* [in] */ const ArrayOf<String>& args);
};

} // namespace Os
} // namespace Droid
} // namespace Internal
} // namespace Elastos

#endif // __CRUNTIMEINIT_H__
