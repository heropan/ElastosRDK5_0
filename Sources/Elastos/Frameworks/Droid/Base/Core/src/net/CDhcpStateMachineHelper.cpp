
#include "ext/frameworkdef.h"
#include "CDhcpStateMachineHelper.h"
#include "DhcpStateMachine.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL(CDhcpStateMachineHelper, Singleton, IDhcpStateMachineHelper)

CAR_SINGLETON_IMPL(CDhcpStateMachineHelper)

ECode CDhcpStateMachineHelper::MakeDhcpStateMachine(
    /* [in] */ IContext* context,
    /* [in] */ IStateMachine* controller,
    /* [in] */ const String& intf,
    /* [out] */ IDhcpStateMachine** result)
{
    VALIDATE_NOT_NULL(*result)
    *result = NULL;
    VALIDATE_NOT_NULL(context)
    VALIDATE_NOT_NULL(controller)

    *result = DhcpStateMachine::MakeDhcpStateMachine(context, controller, intf);
    if(*result != NULL)
        REFCOUNT_ADD(*result)

    return NOERROR;
}

} // namespace Net
} // namespace Droid
} // namespace Elastos
