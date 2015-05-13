
#ifndef __APPERRORRESULT_H__
#define __APPERRORRESULT_H__

#include "ext/frameworkext.h"
#include <elastos/Object.h>
#include <elastos/Mutex.h>

using namespace Elastos;
using namespace Elastos::Core;
using namespace Elastos::Core::Threading;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

extern "C" const InterfaceID EIID_AppErrorResult;

class AppErrorResult
    : public ElRefBase
    , public IInterface
    , public Object
{
public:
    CAR_INTERFACE_DECL()

    AppErrorResult();

    CARAPI_(void) SetResult(
        /* [in] */ Int32 res);

    /*
     *  Return true if package has been added false if not
     */
    CARAPI_(Int32) GetResult();

public:
    Boolean mHasResult;
    Int32 mResult;
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__APPERRORRESULT_H__
