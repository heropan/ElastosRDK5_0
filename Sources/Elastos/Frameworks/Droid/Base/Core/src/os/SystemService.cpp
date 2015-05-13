
#include "os/CSystemService.h"
#include "os/SystemService.h"

namespace Elastos {
namespace Droid {
namespace Os {

HashMap<String, SystemServiceState> SystemService::sStates;
Object SystemService::sPropertyLock;
Boolean SystemService::sInitialized = SystemService::Init();

ECode SystemService::Start(
    /* [in] */ const String& name)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode SystemService::Stop(
    /* [in] */ const String& name)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode SystemService::Restart(
    /* [in] */ const String& name)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

SystemServiceState SystemService::GetState(
    /* [in] */ const String& service)
{
    assert(0);
    return SystemServiceState_RUNNING;
}

Boolean SystemService::IsStopped(
    /* [in] */ const String& service)
{
    assert(0);
    return FALSE;
}

Boolean SystemService::IsRunning(
    /* [in] */ const String& service)
{
    // TODO:
    return FALSE;
}

ECode SystemService::WaitForState(
    /* [in] */ const String& service,
    /* [in] */ SystemServiceState state,
    /* [in] */ Int64 timeoutMillis)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode SystemService::WaitForAnyStopped(
    /* [in] */ ArrayOf<String>* services)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

Boolean SystemService::Init()
{
    // TODO:
    return FALSE;
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
