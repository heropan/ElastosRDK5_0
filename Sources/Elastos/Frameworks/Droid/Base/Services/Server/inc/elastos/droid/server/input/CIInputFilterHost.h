#ifndef __ELASTOS_DROID_SERVER_INPUT_CIINPUTFILTERHOST_H__
#define __ELASTOS_DROID_SERVER_INPUT_CIINPUTFILTERHOST_H__

#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/server/input/InputManagerService.h"
#include "_Elastos_Droid_Server_Input_CIInputFilterHost.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Input {

CarClass(CIInputFilterHost)
    , public InputManagerService::InputFilterHost
{
public:
    CAR_OBJECT_DECL();

    CIInputFilterHost();

    virtual ~CIInputFilterHost();
};

} // Input
} // Server
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_SERVER_INPUT_CIINPUTFILTERHOST_H__
