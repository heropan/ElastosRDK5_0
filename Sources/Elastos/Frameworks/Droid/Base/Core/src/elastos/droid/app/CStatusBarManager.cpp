
#include "CStatusBarManager.h"
#include "os/ServiceManager.h"
#include "os/CBinder.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::Os::CBinder;

namespace Elastos {
namespace Droid {
namespace App {

CStatusBarManager::CStatusBarManager()
{
    ASSERT_SUCCEEDED(CBinder::New((IBinder**)&mToken));
}

ECode CStatusBarManager::constructor(
    /* [in] */ IContext *context)
{
    mContext = context;
    return NOERROR;
}

AutoPtr<IIStatusBarService> CStatusBarManager::GetService()
{
    AutoLock lock(_m_syncLock);
    if (mService == NULL) {
        AutoPtr<IInterface> tmpObj = ServiceManager::GetService(IContext::STATUS_BAR_SERVICE);
        mService = IIStatusBarService::Probe(tmpObj.Get());
        if (mService == NULL) {
            Slogger::W("StatusBarManager", "warning: no STATUS_BAR_SERVICE");
        }
    }
    return mService;
}

ECode CStatusBarManager::Disable(
    /* [in] */ Int32 what)
{
    AutoPtr<IIStatusBarService> svc = GetService();
    if (svc != NULL) {
        String name;
        mContext->GetPackageName(&name);
        ECode ec = svc->Disable(what, mToken, name);
        if (FAILED(ec)) {
            return E_RUNTIME_EXCEPTION;
        }
    }
    return NOERROR;
}

ECode CStatusBarManager::ExpandNotificationsPanel()
{
    AutoPtr<IIStatusBarService> svc = GetService();
    if (svc != NULL) {
        ECode ec = svc->ExpandNotificationsPanel();
        if (FAILED(ec)) {
            return E_RUNTIME_EXCEPTION;
        }
    }
    return NOERROR;
}

ECode CStatusBarManager::CollapsePanels()
{
    AutoPtr<IIStatusBarService> svc = GetService();
    if (svc != NULL) {
        ECode ec = svc->CollapsePanels();
        if (FAILED(ec)) {
            return E_RUNTIME_EXCEPTION;
        }
    }
    return NOERROR;
}

ECode CStatusBarManager::ExpandSettingsPanel()
{
    AutoPtr<IIStatusBarService> svc = GetService();
    if (svc != NULL) {
        ECode ec = svc->ExpandSettingsPanel();
        if (FAILED(ec)) {
            return E_RUNTIME_EXCEPTION;
        }
    }
    return NOERROR;
}

ECode CStatusBarManager::SetIcon(
    /* [in] */ const String& slot,
    /* [in] */ Int32 iconId,
    /* [in] */ Int32 iconLevel,
    /* [in] */ const String& contentDescription)
{
    AutoPtr<IIStatusBarService> svc = GetService();
    if (svc != NULL) {
        String name;
        mContext->GetPackageName(&name);
        ECode ec = svc->SetIcon(slot, name, iconId, iconLevel, contentDescription);
        if (FAILED(ec)) {
            return E_RUNTIME_EXCEPTION;
        }
    }
    return NOERROR;
}

ECode CStatusBarManager::RemoveIcon(
    /* [in] */ const String& slot)
{
    AutoPtr<IIStatusBarService> svc = GetService();
    if (svc != NULL) {
        ECode ec = svc->RemoveIcon(slot);
        if (FAILED(ec)) {
            return E_RUNTIME_EXCEPTION;
        }
    }
    return NOERROR;
}

ECode CStatusBarManager::SetIconVisibility(
    /* [in] */ const String& slot,
    /* [in] */ Boolean visible)
{
    AutoPtr<IIStatusBarService> svc = GetService();
    if (svc != NULL) {
        ECode ec = svc->SetIconVisibility(slot, visible);
        if (FAILED(ec)) {
            return E_RUNTIME_EXCEPTION;
        }
    }
    return NOERROR;
}

}
}
}
