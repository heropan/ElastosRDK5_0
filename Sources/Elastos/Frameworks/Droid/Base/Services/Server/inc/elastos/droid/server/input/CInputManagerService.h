#ifndef __ELASTOS_DROID_SERVER_INPUT_CINPUTMANAGERSERVICE_H__
#define __ELASTOS_DROID_SERVER_INPUT_CINPUTMANAGERSERVICE_H__

#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/server/input/InputManagerService.h"
#include "_Elastos_Droid_Server_Input_CInputManagerService.h"

using Elastos::Droid::Server::Input::IInputManagerService;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Input {

CarClass(CInputManagerService)
    , public InputManagerService
{
public:
    CAR_OBJECT_DECL();

    CInputManagerService();

    virtual ~CInputManagerService();
};

} // Input
} // Server
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_SERVER_INPUT_CINPUTMANAGERSERVICE_H__
