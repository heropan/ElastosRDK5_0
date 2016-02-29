
#include "elastos/droid/systemui/statusbar/policy/LocationControllerImpl.h"
#include "../../R.h"
#include "Elastos.Droid.Location.h"
#include "Elastos.Droid.Os.h"
#include "Elastos.Droid.Provider.h"
#include <elastos/droid/provider/Settings.h>

using Elastos::Droid::App::CActivityManagerHelper;
using Elastos::Droid::App::IActivityManagerHelper;
using Elastos::Droid::App::IAppOpsManagerOpEntry;
using Elastos::Droid::App::IAppOpsManagerPackageOps;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Location::ILocationManager;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Os::CUserHandle;
using Elastos::Droid::Os::CUserHandleHelper;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::IUserHandleHelper;
using Elastos::Droid::Os::IUserManager;
using Elastos::Droid::Provider::ISettingsSecure;
using Elastos::Utility::CArrayList;
using Elastos::Utility::IIterator;
using Elastos::Utility::IList;

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {
namespace Policy {

const String LocationControllerImpl::LOCATION_STATUS_ICON_PLACEHOLDER("location");
const Int32 LocationControllerImpl::LOCATION_STATUS_ICON_ID = R::drawable::stat_sys_location;
AutoPtr<ArrayOf<Int32> > LocationControllerImpl::mHighPowerRequestAppOpArray = InitStatic();

LocationControllerImpl::_BroadcastReceiver::_BroadcastReceiver(
    /* [in] */ LocationControllerImpl* host)
    : mHost(host)
{}

ECode LocationControllerImpl::_BroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);
    if (ILocationManager::MODE_CHANGED_ACTION.Equals(action)) {
        mHost->LocationSettingsChanged();
    }
    return NOERROR;
}


CAR_INTERFACE_IMPL(LocationControllerImpl, BroadcastReceiver, ILocationController);
LocationControllerImpl::LocationControllerImpl(
    /* [in] */ IContext* context)
    : mAreActiveLocationRequests(FALSE)
{
    CArrayList::New((IArrayList**)&mSettingsChangeCallbacks);
    mContext = context;

    AutoPtr<IIntentFilter> filter;
    CIntentFilter::New((IIntentFilter**)&filter);
    filter->AddAction(ILocationManager::HIGH_POWER_REQUEST_CHANGE_ACTION);

    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    AutoPtr<IUserHandle> ALL;
    helper->GetALL((IUserHandle**)&ALL);
    AutoPtr<IIntent> i;
    context->RegisterReceiverAsUser(this, ALL, filter, String(NULL), NULL, (IIntent**)&i);

    AutoPtr<IInterface> obj;
    context->GetSystemService(IContext::APP_OPS_SERVICE, (IInterface**)&obj);
    mAppOpsManager = IAppOpsManager::Probe(obj);

    obj = NULL;
    context->GetSystemService(IContext::STATUS_BAR_SERVICE, (IInterface**)&obj);
    mStatusBarManager = IStatusBarManager::Probe(obj);

    // Register to listen for changes in location settings.
    i = NULL;
    AutoPtr<IIntentFilter> intentFilter;
    CIntentFilter::New((IIntentFilter**)&intentFilter);
    intentFilter->AddAction(ILocationManager::MODE_CHANGED_ACTION);
    AutoPtr<IHandler> handler;
    CHandler::New((IHandler**)&handler);
    AutoPtr<_BroadcastReceiver> b = new _BroadcastReceiver(this);
    context->RegisterReceiverAsUser(b, ALL, intentFilter, String(NULL), handler, (IIntent**)&i);

    // Examine the current location state and initialize the status view.
    UpdateActiveLocationRequests();
    RefreshViews();
}

AutoPtr<ArrayOf<Int32> > LocationControllerImpl::InitStatic()
{
    AutoPtr<ArrayOf<Int32> > a = ArrayOf<Int32>::Alloc(1);
    (*a)[0] = IAppOpsManager::OP_MONITOR_HIGH_POWER_LOCATION;
    return a;
}

ECode LocationControllerImpl::AddSettingsChangedCallback(
    /* [in] */ ILocationSettingsChangeCallback* cb)
{
    mSettingsChangeCallbacks->Add(cb);
    LocationSettingsChanged(cb);
    return NOERROR;
}

ECode LocationControllerImpl::RemoveSettingsChangedCallback(
    /* [in] */ ILocationSettingsChangeCallback* cb)
{
    mSettingsChangeCallbacks->Remove(cb);
    return NOERROR;
}

ECode LocationControllerImpl::SetLocationEnabled(
    /* [in] */ Boolean enabled,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 currentUserId = 0;
    AutoPtr<IActivityManagerHelper> amHelper;
    CActivityManagerHelper::AcquireSingleton((IActivityManagerHelper**)&amHelper);
    amHelper->GetCurrentUser(&currentUserId);
    if (IsUserLocationRestricted(currentUserId)) {
        *result = FALSE;
        return NOERROR;
    }
    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver**)&cr);
    // When enabling location, a user consent dialog will pop up, and the
    // setting won't be fully enabled until the user accepts the agreement.
    Int32 mode = enabled
            ? ISettingsSecure::LOCATION_MODE_HIGH_ACCURACY : ISettingsSecure::LOCATION_MODE_OFF;
    // QuickSettings always runs as the owner, so specifically set the settings
    // for the current foreground user.
    Boolean tmp = FALSE;
    return Elastos::Droid::Provider::Settings::Secure::PutInt32ForUser(cr,
            ISettingsSecure::LOCATION_MODE, mode, currentUserId, &tmp);
}

ECode LocationControllerImpl::IsLocationEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IContentResolver> resolver;
    mContext->GetContentResolver((IContentResolver**)&resolver);
    // QuickSettings always runs as the owner, so specifically retrieve the settings
    // for the current foreground user.
    Int32 currentUserId = 0;
    AutoPtr<IActivityManagerHelper> amHelper;
    CActivityManagerHelper::AcquireSingleton((IActivityManagerHelper**)&amHelper);
    amHelper->GetCurrentUser(&currentUserId);
    Int32 mode = 0;
    Elastos::Droid::Provider::Settings::Secure::GetInt32ForUser(resolver, ISettingsSecure::LOCATION_MODE,
            ISettingsSecure::LOCATION_MODE_OFF, currentUserId, &mode);
    *result = mode != ISettingsSecure::LOCATION_MODE_OFF;
    return NOERROR;
}

Boolean LocationControllerImpl::IsUserLocationRestricted(
    /* [in] */ Int32 userId)
{
    AutoPtr<IInterface> obj;
    mContext->GetSystemService(IContext::USER_SERVICE, (IInterface**)&obj);
    AutoPtr<IUserManager> um = IUserManager::Probe(obj);
    AutoPtr<IUserHandle> uh;
    CUserHandle::New(userId, (IUserHandle**)&uh);
    Boolean result = FALSE;
    um->HasUserRestriction(IUserManager::DISALLOW_SHARE_LOCATION, uh, &result);
    return result;
}

Boolean LocationControllerImpl::AreActiveHighPowerLocationRequests()
{
    AutoPtr<IList/*<AppOpsManager.PackageOps*/> packages;
    mAppOpsManager->GetPackagesForOps(mHighPowerRequestAppOpArray, (IList**)&packages);
    // AppOpsManager can return NULL when there is no requested data.
    if (packages != NULL) {
        Int32 numPackages = 0;
        packages->GetSize(&numPackages);
        for (Int32 packageInd = 0; packageInd < numPackages; packageInd ++) {
            AutoPtr<IInterface> obj;
            packages->Get(packageInd, (IInterface**)&obj);
            AutoPtr<IAppOpsManagerPackageOps> packageOp = IAppOpsManagerPackageOps::Probe(obj);
            AutoPtr<IList/*<AppOpsManager.OpEntry*/> opEntries;
            packageOp->GetOps((IList**)&opEntries);
            if (opEntries != NULL) {
                Int32 numOps = 0;
                opEntries->GetSize(&numOps);
                for (Int32 opInd = 0; opInd < numOps; opInd ++) {
                    AutoPtr<IInterface> e;
                    opEntries->Get(opInd, (IInterface**)&e);
                    AutoPtr<IAppOpsManagerOpEntry> opEntry = IAppOpsManagerOpEntry::Probe(e);
                    // AppOpsManager should only return OP_MONITOR_HIGH_POWER_LOCATION because
                    // of the mHighPowerRequestAppOpArray filter, but checking defensively.
                    Int32 op = 0;
                    opEntry->GetOp(&op);
                    if (op == IAppOpsManager::OP_MONITOR_HIGH_POWER_LOCATION) {
                        Boolean r = FALSE;
                        if (opEntry->IsRunning(&r), r) {
                            return TRUE;
                        }
                    }
                }
            }
        }
    }

    return FALSE;
}

void LocationControllerImpl::RefreshViews()
{
    if (mAreActiveLocationRequests) {
        String value;
        mContext->GetString(R::string::accessibility_location_active, &value);
        mStatusBarManager->SetIcon(LOCATION_STATUS_ICON_PLACEHOLDER, LOCATION_STATUS_ICON_ID, 0, value);
    }
    else {
        mStatusBarManager->RemoveIcon(LOCATION_STATUS_ICON_PLACEHOLDER);
    }
}

void LocationControllerImpl::UpdateActiveLocationRequests()
{
    Boolean hadActiveLocationRequests = mAreActiveLocationRequests;
    mAreActiveLocationRequests = AreActiveHighPowerLocationRequests();
    if (mAreActiveLocationRequests != hadActiveLocationRequests) {
        RefreshViews();
    }
}

void LocationControllerImpl::LocationSettingsChanged()
{
    Boolean isEnabled = FALSE;
    IsLocationEnabled(&isEnabled);

    AutoPtr<IIterator> ator;
    mSettingsChangeCallbacks->GetIterator((IIterator**)&ator);
    Boolean hasNext = FALSE;
    while (ator->HasNext(&hasNext), hasNext) {
        AutoPtr<IInterface> cb;
        ator->GetNext((IInterface**)&cb);
        ILocationSettingsChangeCallback::Probe(cb)->OnLocationSettingsChanged(isEnabled);
    }
}

void LocationControllerImpl::LocationSettingsChanged(
    /* [in] */ ILocationSettingsChangeCallback* cb)
{
    Boolean tmp = FALSE;
    IsLocationEnabled(&tmp);
    cb->OnLocationSettingsChanged(tmp);
}

ECode LocationControllerImpl::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);
    if (ILocationManager::HIGH_POWER_REQUEST_CHANGE_ACTION.Equals(action)) {
        UpdateActiveLocationRequests();
    }
    return NOERROR;
}

} // namespace Policy
} // namespace StatusBar
} // namespace SystemUI
} // namespace Droid
} // namespace Elastos
