
#ifndef __ELASTOS_DROID_SERVER_AM_APPERRORRESULT_H__
#define __ELASTOS_DROID_SERVER_AM_APPERRORRESULT_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/Core/Object.h>

using namespace Elastos;
using namespace Elastos::Core;

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

#endif //__ELASTOS_DROID_SERVER_AM_APPERRORRESULT_H__
