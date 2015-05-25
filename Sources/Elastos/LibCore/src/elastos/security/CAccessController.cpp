
#include "CAccessController.h"
#include "cmdef.h"
#include "CAccessControlContext.h"

namespace Elastos {
namespace Security {

ECode CAccessController::DoPrivileged(
    /* [in] */ IPrivilegedAction *action,
    /* [out] */ IInterface **result)
{
    return action->Run(result);
}

ECode CAccessController::DoPrivilegedEx(
    /* [in] */ IPrivilegedAction *action,
    /* [in] */ IAccessControlContext *context,
    /* [out] */ IInterface **result)
{
    return action->Run(result);
}

ECode CAccessController::DoPrivilegedEx2(
    /* [in] */ IPrivilegedExceptionAction *action,
    /* [out] */ IInterface **result)
{
    ECode ec = action->Run(result);
    if (FAILED(ec) && ec != E_RUNTIME_EXCEPTION) {
        ec = E_PRIVILEGED_ACTION_EXCEPTION;
    }
    return ec;
}

ECode CAccessController::DoPrivilegedEx3(
    /* [in] */ IPrivilegedExceptionAction *action,
    /* [in] */ IAccessControlContext *context,
    /* [out] */ IInterface **result)
{
    return DoPrivilegedEx2(action, result);
}

ECode CAccessController::DoPrivilegedWithCombiner(
    /* [in] */ IPrivilegedAction *action,
    /* [out] */ IInterface **result)
{
    return action->Run(result);
}

ECode CAccessController::DoPrivilegedWithCombinerEx(
    /* [in] */ IPrivilegedExceptionAction *action,
    /* [out] */ IInterface **result)
{
    return DoPrivilegedEx2(action, result);
}

ECode CAccessController::CheckPermission(
    /* [in] */ IPermission *permission)
{
    return NOERROR;
}

ECode CAccessController::GetContext(
    /* [out] */ IAccessControlContext **context)
{
    return CAccessControlContext::New(NULL, context);
}

}
}

