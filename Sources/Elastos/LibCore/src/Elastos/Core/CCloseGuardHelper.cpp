
#include "cmdef.h"
#include "CCloseGuardHelper.h"
#include "CCloseGuard.h"
#include <elastos.h>

namespace Elastos {
namespace Core {

ECode CCloseGuardHelper::Get(
    /* [out] */ ICloseGuard** guard)
{
    VALIDATE_NOT_NULL(guard);

    AutoPtr<ICloseGuard> res = CCloseGuard::Get();
    *guard = res;
    INTERFACE_ADDREF(*guard)
    return NOERROR;
}

ECode CCloseGuardHelper::SetEnabled(
    /* [in] */ Boolean enabled)
{
    return CCloseGuard::SetEnabled(enabled);
}

ECode CCloseGuardHelper::SetReporter(
    /* [in] */ ICloseGuardReporter* reporter)
{
    return CCloseGuard::SetReporter(reporter);
}

ECode CCloseGuardHelper::GetReporter(
    /* [out] */ ICloseGuardReporter** report)
{
    VALIDATE_NOT_NULL(report);

    AutoPtr<ICloseGuardReporter> res = CCloseGuard::GetReporter();
    *report = res;
    INTERFACE_ADDREF(*report)
    return NOERROR;
}

} // namespace Core
} // namespace Elastos