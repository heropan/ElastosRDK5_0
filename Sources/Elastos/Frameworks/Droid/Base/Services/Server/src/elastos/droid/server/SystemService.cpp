
#include "elastos/droid/server/SystemService.h"

using Elastos::Droid::Os::IServiceManager;
using Elastos::Droid::Os::CServiceManager;

namespace Elastos {
namespace Droid {
namespace Server {

CAR_INTERFACE_IMPL(SystemService, Object, ISystemService)

SystemService::SystemService()
{}

SystemService::~SystemService()
{}

ECode SystemService::constructor(
    /* [in] */ IContext* context)
{
    mContext = context;
    return NOERROR;
}

ECode SystemService::GetContext(
    /* [out] */ IContext** ctx)
{
    VALIDATE_NOT_NULL(ctx)
    *ctx = mContext;
    REFCOUNT_ADD(*ctx)
    return NOERROR;
}

ECode SystemService::IsSafeMode(
    /* [out] */  Boolean* safeMode)
{
    VALIDATE_NOT_NULL(safeMode)
    AutoPtr<ISystemServiceManager> ssMgr = GetManager();
    return ssMgr->IsSafeMode(safeMode);
}

ECode SystemService::OnBootPhase(
    /* [in] */ Int32 phase)
{
    return NOERROR;
}

ECode SystemService::OnStartUser(
    /* [in] */ Int32 userHandle)
{
    return NOERROR;
}

ECode SystemService::OnSwitchUser(
    /* [in] */ Int32 userHandle)
{
    return NOERROR;
}

ECode SystemService::OnStopUser(
    /* [in] */ Int32 userHandle)
{
    return NOERROR;
}

ECode SystemService::OnCleanupUser(
    /* [in] */ Int32 userHandle)
{
    return NOERROR;
}

void SystemService::PublishBinderService(
    /* [in] */ const String& name,
    /* [in] */ IBinder* service)
{
    PublishBinderService(name, service, FALSE);
}

void SystemService::PublishBinderService(
    /* [in] */ const String& name,
    /* [in] */ IBinder* service,
    /* [in] */ Boolean allowIsolated)
{
    assert(0 && "TODO");
    AutoPtr<IServiceManager> smgr;
    // CServiceManager::AcquireSingleton((IServiceManager**)&smgr);
    smgr->AddService(name, service, allowIsolated);
}

AutoPtr<IBinder> SystemService::GetBinderService(
    /* [in] */ const String& name)
{
    assert(0 && "TODO");
    AutoPtr<IServiceManager> smgr;
    // CServiceManager::AcquireSingleton((IServiceManager**)&smgr);
    // return ServiceManager.getService(name);
    return NULL;
}

void SystemService::PublishLocalService(
    /* [in] */ const InterfaceID& type,
    /* [in] */ IInterface* service)
{
    assert(0 && "TODO");
    // LocalServices.addService(type, service);
}

AutoPtr<IInterface> SystemService::GetLocalService(
    /* [in] */const InterfaceID& type)
{
    assert(0 && "TODO");
    // return LocalServices.getService(type);
    return NULL;
}


AutoPtr<ISystemServiceManager> SystemService::GetManager()
{
    assert(0 && "TODO");
    // return LocalServices.getService(SystemServiceManager.class);
    return NULL;
}


} // namespace Server
} // namepsace Droid
} // namespace Elastos
