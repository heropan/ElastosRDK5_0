
#include "cmdef.h"
#include "CCloseGuard.h"
#include <elastos.h>

namespace Elastos {
namespace Core {

static AutoPtr<ICloseGuard> initNOOP()
{
    AutoPtr<CCloseGuard> cg;
    CCloseGuard::NewByFriend((CCloseGuard**)&cg);
    return cg;
}

static AutoPtr<ICloseGuardReporter> initREPORTER()
{
    AutoPtr<ICloseGuardReporter> report;
    report = new CCloseGuard::DefaultReporter();
    return report;
}

Boolean CCloseGuard::ENABLED = TRUE;
const AutoPtr<ICloseGuard> CCloseGuard::NOOP = initNOOP();
AutoPtr<ICloseGuardReporter> CCloseGuard::REPORTER = initREPORTER();

CAR_INTERFACE_IMPL(CCloseGuard::DefaultReporter, ICloseGuardReporter);

ECode CCloseGuard::DefaultReporter::Report (
    /* [in] */ const String& message/*,
    [in] Throwable allocationSite*/)
{
    //TODO: System.logW(message, allocationSite);
    return NOERROR;
}

AutoPtr<ICloseGuard> CCloseGuard::Get()
{
    if (!ENABLED) {
        return NOOP;
    }
    AutoPtr<CCloseGuard> res;
    CCloseGuard::NewByFriend((CCloseGuard**)&res);
    return (ICloseGuard*)res.Get();
}

ECode CCloseGuard::SetEnabled(
    /* [in] */ Boolean enabled)
{
    ENABLED = enabled;
    return NOERROR;
}

ECode CCloseGuard::SetReporter(
    /* [in] */ ICloseGuardReporter* reporter)
{
    if (reporter == NULL) {
        //throw new NullPointerException("reporter == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    REPORTER = reporter;
    return NOERROR;
}

AutoPtr<ICloseGuardReporter> CCloseGuard::GetReporter()
{
    return REPORTER;
}

ECode CCloseGuard::Open(
    /* [in] */ const String& closer)
{
    // always perform the check for valid API usage...
    if (closer.IsNull()) {
        //throw new NullPointerException("closer == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    // ...but avoid allocating an allocationSite if disabled
    if (this == NOOP || !ENABLED) {
        return NOERROR;
    }
    String message = String("Explicit termination method '") + closer + String("' not called");
    //TODO: allocationSite = new Throwable(message);
    return NOERROR;
}

ECode CCloseGuard::Close()
{
    //TODO: allocationSite = null;
    return NOERROR;
}

ECode CCloseGuard::WarnIfOpen()
{
    if (/*TODO: allocationSite == null ||*/ !ENABLED) {
        return NOERROR;
    }

    String message =
            String("A resource was acquired at attached stack trace but never released. ")
             + String("See java.io.Closeable for information on avoiding resource leaks.");

    REPORTER->Report(message/*TODO: , allocationSite*/);
    return NOERROR;
}

} // namespace Core
} // namespace Elastos