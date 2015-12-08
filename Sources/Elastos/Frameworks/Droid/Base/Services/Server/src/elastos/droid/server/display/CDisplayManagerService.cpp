
#include "elastos/droid/server/display/CDisplayManagerService.h"
#include "elastos/droid/server/display/HeadlessDisplayAdapter.h"
#include "elastos/droid/server/display/LocalDisplayAdapter.h"
#include "elastos/droid/server/display/WifiDisplayAdapter.h"
#include "elastos/droid/server/display/OverlayDisplayAdapter.h"
#include "elastos/droid/os/SystemClock.h"
#include "elastos/droid/os/Binder.h"
#include "elastos/droid/R.h"
#include "elastos/droid/Manifest.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/Thread.h>

using Elastos::Droid::Os::Binder;
using Elastos::Droid::Os::ISystemProperties;
using Elastos::Droid::Os::CSystemProperties;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::View::IDisplay;
using Elastos::Droid::View::CDisplayInfo;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Hardware::Display::IDisplayManagerGlobal;
using Elastos::Droid::Hardware::Display::CWifiDisplayStatus;
using Elastos::Core::Thread;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Display {

const String CDisplayManagerService::TAG("CDisplayManagerService");
const Boolean CDisplayManagerService::DEBUG = FALSE;
const String CDisplayManagerService::FORCE_WIFI_DISPLAY_ENABLE("persist.debug.wfd.enable");
const String CDisplayManagerService::SYSTEM_HEADLESS("persist.debug.wfd.enable");
const Int64 CDisplayManagerService::WAIT_FOR_DEFAULT_DISPLAY_TIMEOUT;
const Int32 CDisplayManagerService::MSG_REGISTER_DEFAULT_DISPLAY_ADAPTER;
const Int32 CDisplayManagerService::MSG_REGISTER_ADDITIONAL_DISPLAY_ADAPTERS;
const Int32 CDisplayManagerService::MSG_DELIVER_DISPLAY_EVENT;
const Int32 CDisplayManagerService::MSG_REQUEST_TRAVERSAL;
const Int32 CDisplayManagerService::MSG_UPDATE_VIEWPORT;
const Int32 CDisplayManagerService::DISPLAY_BLANK_STATE_UNKNOWN;
const Int32 CDisplayManagerService::DISPLAY_BLANK_STATE_BLANKED;
const Int32 CDisplayManagerService::DISPLAY_BLANK_STATE_UNBLANKED;

//==============================================================================
// CDisplayManagerService::DisplayManagerHandler
//==============================================================================

CDisplayManagerService::DisplayManagerHandler::DisplayManagerHandler(
    /* [in] */ ILooper* looper,
    /* [in] */ CDisplayManagerService* host)
    : Handler(looper, NULL, TRUE /*async*/)
    , mHost(host)
{
}

String CDisplayManagerService::DisplayManagerHandler::CodeToString(
    /* [in] */ Int32 what)
{
    switch (what) {
        case CDisplayManagerService::MSG_REGISTER_DEFAULT_DISPLAY_ADAPTER :
            return String("MSG_REGISTER_DEFAULT_DISPLAY_ADAPTER");
        case CDisplayManagerService::MSG_REGISTER_ADDITIONAL_DISPLAY_ADAPTERS :
            return String("MSG_REGISTER_ADDITIONAL_DISPLAY_ADAPTERS");
        case CDisplayManagerService::MSG_DELIVER_DISPLAY_EVENT :
            return String("MSG_DELIVER_DISPLAY_EVENT");
        case CDisplayManagerService::MSG_REQUEST_TRAVERSAL :
            return String("MSG_REQUEST_TRAVERSAL");
        case CDisplayManagerService::MSG_UPDATE_VIEWPORT :
            return String("MSG_UPDATE_VIEWPORT");
    }
    return String("Unknown message");
}

ECode CDisplayManagerService::DisplayManagerHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    VALIDATE_NOT_NULL(msg);

    Int32 what;
    msg->GetWhat(&what);

    if (CDisplayManagerService::DEBUG) {
        Slogger::D(CDisplayManagerService::TAG, " >>>> HandleMessage %s", CodeToString(what).string());
    }

    switch (what) {
        case CDisplayManagerService::MSG_REGISTER_DEFAULT_DISPLAY_ADAPTER:
            mHost->RegisterDefaultDisplayAdapter();
            break;

        case CDisplayManagerService::MSG_REGISTER_ADDITIONAL_DISPLAY_ADAPTERS:
            mHost->RegisterAdditionalDisplayAdapters();
            break;

        case CDisplayManagerService::MSG_DELIVER_DISPLAY_EVENT: {
            Int32 arg1, arg2;
            msg->GetArg1(&arg1);
            msg->GetArg2(&arg2);
            mHost->DeliverDisplayEvent(arg1, arg2);
            break;
        }

        case CDisplayManagerService::MSG_REQUEST_TRAVERSAL:
            if (mHost->mWindowManagerFuncs)
                mHost->mWindowManagerFuncs->RequestTraversal();
            break;

        case MSG_UPDATE_VIEWPORT:
        {
            {
                AutoLock lock(mHost->mSyncRoot);
                mHost->mTempDefaultViewport->CopyFrom(mHost->mDefaultViewport);
                mHost->mTempExternalTouchViewport->CopyFrom(mHost->mExternalTouchViewport);
            }
            if (mHost->mInputManagerFuncs) {
                mHost->mInputManagerFuncs->SetDisplayViewports(
                    (IDisplayViewport*)mHost->mTempDefaultViewport->Probe(EIID_IDisplayViewport),
                    (IDisplayViewport*)mHost->mTempExternalTouchViewport->Probe(EIID_IDisplayViewport));
            }
            break;
        }
    }

    if (CDisplayManagerService::DEBUG) {
        Slogger::D(CDisplayManagerService::TAG, " <<<< HandleMessage %s done", CodeToString(what).string());
    }
    return NOERROR;
}

//==============================================================================
// CDisplayManagerService::DisplayAdapterListener
//==============================================================================
CAR_INTERFACE_IMPL(CDisplayManagerService::DisplayAdapterListener, Object, IDisplayAdapterListener)

CDisplayManagerService::DisplayAdapterListener::DisplayAdapterListener(
    /* [in] */ CDisplayManagerService* owner)
    : mHost(owner)
{
}

ECode CDisplayManagerService::DisplayAdapterListener::OnDisplayDeviceEvent(
    /* [in] */ Handle32 deviceHandle,
    /* [in] */ Int32 event)
{
    AutoPtr<DisplayDevice> device = (DisplayDevice*)deviceHandle;
    switch (event) {
        case DisplayAdapter::DISPLAY_DEVICE_EVENT_ADDED:
            mHost->HandleDisplayDeviceAdded(device);
            break;

        case DisplayAdapter::DISPLAY_DEVICE_EVENT_CHANGED:
            mHost->HandleDisplayDeviceChanged(device);
            break;

        case DisplayAdapter::DISPLAY_DEVICE_EVENT_REMOVED:
            mHost->HandleDisplayDeviceRemoved(device);
            break;
    }

    return NOERROR;
}

ECode CDisplayManagerService::DisplayAdapterListener::OnTraversalRequested()
{
    AutoLock lock(mHost->mSyncRoot);
    mHost->ScheduleTraversalLocked(FALSE);
    return NOERROR;
}

//=====================================================================================
// CDisplayManagerService::CallbackRecord
//=====================================================================================
CAR_INTERFACE_IMPL(CDisplayManagerService::CallbackRecord, IProxyDeathRecipient);

CDisplayManagerService::CallbackRecord::CallbackRecord(
    /* [in] */ CDisplayManagerService* owner,
    /* [in] */ Int32 pid,
    /* [in] */ IDisplayManagerCallback* callback)
    : mHost(owner)
    , mPid(pid)
    , mCallback(callback)
{
}

CDisplayManagerService::CallbackRecord::~CallbackRecord()
{
}

//@Override
ECode CDisplayManagerService::CallbackRecord::ProxyDied()
{
    if (DEBUG) {
        Slogger::D(TAG, "Display listener for pid %d died.", mPid);
    }
    mHost->OnCallbackDied(mPid);

    return NOERROR;
}

void CDisplayManagerService::CallbackRecord::NotifyDisplayEventAsync(
    /* [in] */ Int32 displayId,
    /* [in] */ Int32 event)
{
    if (FAILED(mCallback->OnDisplayEvent(displayId, event))) {
        Slogger::W(TAG, "Failed to notify process %d that"
            "displays changed, assuming it died.", mPid);
        ProxyDied();
    }
}

//=====================================================================================
// CDisplayManagerService
//=====================================================================================
CAR_INTERFACE_IMPL(CDisplayManagerService, Object, IIDisplayManager)

CAR_OBJECT_IMPL(CDisplayManagerService)

CDisplayManagerService::CDisplayManagerService()
    : mSafeMode(FALSE)
    , mOnlyCore(FALSE)
    , mHeadless(FALSE)
    , mSingleDisplayDemoMode(FALSE)
    , mNextNonDefaultDisplayId(IDisplay::DEFAULT_DISPLAY + 1)
    , mAllDisplayBlankStateFromPowerManager(0)
    , mPendingTraversal(FALSE)
{
    mDefaultViewport = new CDisplayViewport();
    mExternalTouchViewport = new CDisplayViewport();
    mPersistentDataStore = new PersistentDataStore();
    CDisplayInfo::New((IDisplayInfo**)&mTempDisplayInfo);
    mTempDefaultViewport = new CDisplayViewport();
    mTempExternalTouchViewport = new CDisplayViewport();
}

ECode CDisplayManagerService::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IHandler* mainHandler,
    /* [in] */ IHandler* uiHandler)
{
    mContext = context;
    AutoPtr<ISystemProperties> sysProp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
    String value;
    sysProp->Get(SYSTEM_HEADLESS, &value);
    mHeadless = value.Equals("1");

    AutoPtr<ILooper> looper;
    mainHandler->GetLooper((ILooper**)&looper);
    mHandler = new DisplayManagerHandler(looper, this);
    mUiHandler = uiHandler;
    mDisplayAdapterListener = new DisplayAdapterListener(this);
    sysProp->GetBoolean(String("persist.demo.singledisplay"), FALSE, &mSingleDisplayDemoMode);

    Boolean result;
    mHandler->SendEmptyMessage(MSG_REGISTER_DEFAULT_DISPLAY_ADAPTER, &result);
    return NOERROR;
}

/**
 * Pauses the boot process to wait for the first display to be initialized.
 */
ECode CDisplayManagerService::WaitForDefaultDisplay(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);

    AutoLock lock(mSyncRoot);
    Int64 timeout = SystemClock::GetUptimeMillis() + WAIT_FOR_DEFAULT_DISPLAY_TIMEOUT;
    HashMap<Int32, AutoPtr<LogicalDisplay> >::Iterator find;
    while (mLogicalDisplays.Find(IDisplay::DEFAULT_DISPLAY) == mLogicalDisplays.End()) {
        Int64 delay = timeout - SystemClock::GetUptimeMillis();
        if (delay <= 0) {
            *res = FALSE;
            return NOERROR;
        }
        if (DEBUG) {
            Slogger::D(TAG, "waitForDefaultDisplay: waiting, timeout=%d", delay);
        }
        //try {
        mSyncRoot.Wait(delay);
        // } catch (InterruptedException ex) {
        // }
    }

    *res = TRUE;
    return NOERROR;
}

/**
 * Called during initialization to associate the display manager with the
 * window manager.
 */
ECode CDisplayManagerService::SetWindowManager(
    /* [in] */ IDisplayManagerServiceWindowManagerFuncs* windowManagerFuncs)
{
    AutoLock lock(mSyncRoot);
    mWindowManagerFuncs = windowManagerFuncs;
    ScheduleTraversalLocked(FALSE);

    return NOERROR;
}

/**
 * Called during initialization to associate the display manager with the
 * input manager.
 */
ECode CDisplayManagerService::SetInputManager(
    /* [in] */ IDisplayManagerServiceInputManagerFuncs* inputManagerFuncs)
{
    AutoLock lock(mSyncRoot);
    mInputManagerFuncs = inputManagerFuncs;
    ScheduleTraversalLocked(FALSE);

    return NOERROR;
}

/**
 * Called when the system is ready to go.
 */
ECode CDisplayManagerService::SystemReady(
    /* [in] */ Boolean safeMode,
    /* [in] */ Boolean onlyCore)
{
    AutoLock lock(mSyncRoot);
    mSafeMode = safeMode;
    mOnlyCore = onlyCore;

    Boolean result;
    mHandler->SendEmptyMessage(MSG_REGISTER_ADDITIONAL_DISPLAY_ADAPTERS, &result);

    return NOERROR;
}

/**
 * Returns TRUE if the device is headless.
 *
 * @return True if the device is headless.
 */
ECode CDisplayManagerService::IsHeadless(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = mHeadless;
    return NOERROR;
}

ECode CDisplayManagerService::RegisterDisplayTransactionListener(
    /* [in] */ IDisplayTransactionListener* listener)
{
    VALIDATE_NOT_NULL(listener);

    AutoLock lock(mDisplayTransactionListenersLock);
    // List is self-synchronized copy-on-write.
    mDisplayTransactionListeners.PushBack(listener);
    return NOERROR;
}

ECode CDisplayManagerService::UnregisterDisplayTransactionListener(
    /* [in] */ IDisplayTransactionListener* listener)
{
    VALIDATE_NOT_NULL(listener);

    AutoLock lock(mDisplayTransactionListenersLock);
    // List is self-synchronized copy-on-write.
    mDisplayTransactionListeners.Remove(listener);
    return NOERROR;
}


/**
 * Overrides the display information of a particular logical display.
 * This is used by the window manager to control the size and characteristics
 * of the default display.  It is expected to apply the requested change
 * to the display information synchronously so that applications will immediately
 * observe the new state.
 *
 * @param displayId The logical display id.
 * @param info The new data to be stored.
 */
ECode CDisplayManagerService::SetDisplayInfoOverrideFromWindowManager(
    /* [in] */ Int32 displayId,
    /* [in] */ IDisplayInfo* info)
{
    AutoLock lock(mSyncRoot);

    HashMap<Int32, AutoPtr<LogicalDisplay> >::Iterator find = mLogicalDisplays.Find(displayId);
    if (find != mLogicalDisplays.End()) {
        AutoPtr<LogicalDisplay> display = find->mSecond;
        mTempDisplayInfo->CopyFrom(display->GetDisplayInfoLocked());
        display->SetDisplayInfoOverrideFromWindowManagerLocked(info);
        Boolean res;
        mTempDisplayInfo->Equals(display->GetDisplayInfoLocked(), &res);
        if (!res) {
            SendDisplayEventLocked(displayId, IDisplayManagerGlobal::EVENT_DISPLAY_CHANGED);
            ScheduleTraversalLocked(FALSE);
        }
    }

    return NOERROR;
}

/**
 * Called by the window manager to perform traversals while holding a
 * surface flinger transaction.
 */
ECode CDisplayManagerService::PerformTraversalInTransactionFromWindowManager()
{
    AutoLock lock(mSyncRoot);
    if (!mPendingTraversal) {
        return NOERROR;
    }
    mPendingTraversal = FALSE;

    PerformTraversalInTransactionLocked();

    {
        // List is self-synchronized copy-on-write.
        AutoLock lock(mDisplayTransactionListenersLock);

        List< AutoPtr<IDisplayTransactionListener> >::Iterator it;
        for (it = mDisplayTransactionListeners.Begin(); it != mDisplayTransactionListeners.End(); ++it) {
            (*it)->OnDisplayTransaction();
        }
    }

    return NOERROR;
}

/**
 * Called by the power manager to blank all displays.
 */
ECode CDisplayManagerService::BlankAllDisplaysFromPowerManager()
{
    AutoLock lock(mSyncRoot);
    if (mAllDisplayBlankStateFromPowerManager != DISPLAY_BLANK_STATE_BLANKED) {
        mAllDisplayBlankStateFromPowerManager = DISPLAY_BLANK_STATE_BLANKED;

        List<AutoPtr<DisplayDevice> >::Iterator iter = mDisplayDevices.Begin();
        for (; iter != mDisplayDevices.End(); ++iter) {
            (*iter)->BlankLocked();
        }

        ScheduleTraversalLocked(FALSE);
    }

    return NOERROR;
}

/**
 * Called by the power manager to unblank all displays.
 */
ECode CDisplayManagerService::UnblankAllDisplaysFromPowerManager()
{
    AutoLock lock(mSyncRoot);
    if (mAllDisplayBlankStateFromPowerManager != DISPLAY_BLANK_STATE_UNBLANKED) {
        mAllDisplayBlankStateFromPowerManager = DISPLAY_BLANK_STATE_UNBLANKED;

        List<AutoPtr<DisplayDevice> >::Iterator iter = mDisplayDevices.Begin();
        for (; iter != mDisplayDevices.End(); ++iter) {
            (*iter)->UnblankLocked();
        }

        ScheduleTraversalLocked(FALSE);
    }

    return NOERROR;
}

/**
 * Returns information about the specified logical display.
 *
 * @param displayId The logical display id.
 * @return The logical display info, or NULL if the display does not exist.  The
 * returned object must be treated as immutable.
 */
//@Override // Binder call
ECode CDisplayManagerService::GetDisplayInfo(
    /* [in] */ Int32 displayId,
    /* [out] */ IDisplayInfo** displayInfo)
{
    VALIDATE_NOT_NULL(displayInfo);
    *displayInfo = NULL;

    AutoLock lock(mSyncRoot);

    HashMap<Int32, AutoPtr<LogicalDisplay> >::Iterator find = mLogicalDisplays.Find(displayId);
    if (find != mLogicalDisplays.End()) {
        AutoPtr<LogicalDisplay> display = find->mSecond;
        AutoPtr<IDisplayInfo> temInfo = display->GetDisplayInfoLocked();
        *displayInfo = temInfo;
        REFCOUNT_ADD(*displayInfo);
    }

    return NOERROR;
}

/**
 * Returns the list of all display ids.
 */
//@Override // Binder call
ECode CDisplayManagerService::GetDisplayIds(
    /* [out, callee] */ ArrayOf<Int32>** displayIds)
{
    VALIDATE_NOT_NULL(displayIds);

    AutoLock lock(mSyncRoot);
    Int32 count = mLogicalDisplays.GetSize();
    *displayIds = ArrayOf<Int32>::Alloc(count);
    if (*displayIds == NULL)
        return E_OUT_OF_MEMORY_ERROR;

    REFCOUNT_ADD(*displayIds);
    HashMap<Int32, AutoPtr<LogicalDisplay> >::Iterator iter
        = mLogicalDisplays.Begin();
    for (Int32 i = 0; i < count; ++iter, i++) {
        (**displayIds)[i] = iter->mFirst;
    }

    return NOERROR;
}

//@Override // Binder call
ECode CDisplayManagerService::RegisterCallback(
    /* [in] */ IDisplayManagerCallback* callback)
{
    if (callback == NULL) {
        Slogger::E(TAG, "listener must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoLock lock(mSyncRoot);

    Int32 callingPid = Binder::GetCallingPid();
    HashMap<Int32, AutoPtr<CallbackRecord> >::Iterator find
        = mCallbacks.Find(callingPid);
    if (find != mCallbacks.End()) {
        Slogger::E(TAG, "The calling process has already "
            "registered an IDisplayManagerCallback.");
        return E_SECURITY_EXCEPTION;
    }

    AutoPtr<CallbackRecord> record = new CallbackRecord(this, callingPid, callback);

    AutoPtr<IProxy> proxy = (IProxy*)callback->Probe(EIID_IProxy);
    if (proxy != NULL && FAILED(proxy->LinkToDeath(record, 0))) {
        return E_RUNTIME_EXCEPTION;
    }

    mCallbacks[callingPid] = record;

    return NOERROR;
}

void CDisplayManagerService::OnCallbackDied(
    /* [in] */ Int32 pid)
{
    AutoLock lock(mSyncRoot);
    mCallbacks.Erase(pid);
}

ECode CDisplayManagerService::ScanWifiDisplays()
{
    if (DEBUG) Slogger::D(TAG, "ScanWifiDisplays");

    Int64 token = Binder::ClearCallingIdentity();
    {
        AutoLock lock(mSyncRoot);
        if (mWifiDisplayAdapter != NULL) {
            mWifiDisplayAdapter->RequestScanLocked();
        }
    }
    Binder::RestoreCallingIdentity(token);

    return NOERROR;
}

//@Override // Binder call
ECode CDisplayManagerService::ConnectWifiDisplay(
    /* [in] */ const String& address)
{
    if (DEBUG) Slogger::D(TAG, "ConnectWifiDisplay address: [%s]", address.string());

    if (address.IsNull()) {
        Slogger::E(TAG, "address must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Boolean trusted = CanCallerConfigureWifiDisplay();
    Int64 token = Binder::ClearCallingIdentity();
    {
        AutoLock lock(mSyncRoot);
        if (mWifiDisplayAdapter != NULL) {
            mWifiDisplayAdapter->RequestConnectLocked(address, trusted);
        }
    }
    Binder::RestoreCallingIdentity(token);

    return NOERROR;
}

//@Override // Binder call
ECode CDisplayManagerService::DisconnectWifiDisplay()
{
    if (DEBUG) Slogger::D(TAG, "DisconnectWifiDisplay");

    Int64 token = Binder::ClearCallingIdentity();
    {
        AutoLock lock(mSyncRoot);
        if (mWifiDisplayAdapter != NULL) {
            mWifiDisplayAdapter->RequestDisconnectLocked();
        }
    }
    Binder::RestoreCallingIdentity(token);

    return NOERROR;
}

//@Override // Binder call
ECode CDisplayManagerService::RenameWifiDisplay(
    /* [in] */ const String& address,
    /* [in] */ const String& alias)
{
    if (DEBUG) Slogger::D(TAG, "RenameWifiDisplay address: [%s], alias: [%s]", address.string(), alias.string());

    if (address.IsNull()) {
        Slogger::E(TAG, "address must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (!CanCallerConfigureWifiDisplay()) {
        Slogger::E(TAG, "Requires CONFIGURE_WIFI_DISPLAY "
            "permission to rename a wifi display.");
        return E_SECURITY_EXCEPTION;
    }

    Int64 token = Binder::ClearCallingIdentity();
    {
        AutoLock lock(mSyncRoot);
        if (mWifiDisplayAdapter != NULL) {
            mWifiDisplayAdapter->RequestRenameLocked(address, alias);
        }
    }
    Binder::RestoreCallingIdentity(token);

    return NOERROR;
}

//@Override // Binder call
ECode CDisplayManagerService::ForgetWifiDisplay(
    /* [in] */ const String& address)
{
    if (DEBUG) Slogger::D(TAG, "ForgetWifiDisplay address: [%s]", address.string());

    if (address.IsNull()) {
        Slogger::E(TAG, "address must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (!CanCallerConfigureWifiDisplay()) {
        Slogger::E(TAG, "Requires CONFIGURE_WIFI_DISPLAY "
            "permission to forget a wifi display.");
        return E_SECURITY_EXCEPTION;
    }

    Int64 token = Binder::ClearCallingIdentity();
    {
        AutoLock lock(mSyncRoot);
        if (mWifiDisplayAdapter != NULL) {
            mWifiDisplayAdapter->RequestForgetLocked(address);
        }
    }
    Binder::RestoreCallingIdentity(token);

    return NOERROR;
}

//@Override // Binder call
ECode CDisplayManagerService::GetWifiDisplayStatus(
    /* [out] */ IWifiDisplayStatus** status)
{
    VALIDATE_NOT_NULL(status);

    if (DEBUG) Slogger::D(TAG, "GetWifiDisplayStatus");

    Int64 token = Binder::ClearCallingIdentity();
    {
        AutoLock lock(mSyncRoot);
        if (mWifiDisplayAdapter != NULL) {
            AutoPtr<IWifiDisplayStatus> s = mWifiDisplayAdapter->GetWifiDisplayStatusLocked();
            *status = s;
            REFCOUNT_ADD(*status);
        }
        else {
            CWifiDisplayStatus::New(status);
        }
    }
    Binder::RestoreCallingIdentity(token);

    return NOERROR;
}

Boolean CDisplayManagerService::CanCallerConfigureWifiDisplay()
{
    Int32 res;
    FAIL_RETURN(mContext->CheckCallingPermission(
        Elastos::Droid::Manifest::permission::CONFIGURE_WIFI_DISPLAY, &res));
    Boolean result = res == IPackageManager::PERMISSION_GRANTED;
    if (DEBUG) Slogger::D(TAG, "CanCallerConfigureWifiDisplay %s", result ? "TRUE" : "FALSE");
    return result;
}

void CDisplayManagerService::RegisterDefaultDisplayAdapter()
{
    if (DEBUG) Slogger::D(TAG, "RegisterDefaultDisplayAdapter mHeadless: %d", mHeadless);
    // Register default display adapter.
    AutoLock lock(mSyncRoot);

    if (mHeadless) {
        AutoPtr<HeadlessDisplayAdapter> adapter = new HeadlessDisplayAdapter(
            &mSyncRoot, mContext, mHandler, mDisplayAdapterListener);
        RegisterDisplayAdapterLocked(adapter);
    }
    else {
        AutoPtr<LocalDisplayAdapter> adapter = new LocalDisplayAdapter(
            &mSyncRoot, mContext, mHandler, mDisplayAdapterListener);
        RegisterDisplayAdapterLocked(adapter);
    }
}

void CDisplayManagerService::RegisterAdditionalDisplayAdapters()
{
    AutoLock lock(mSyncRoot);
    Boolean result = ShouldRegisterNonEssentialDisplayAdaptersLocked();
    if (result) {
        RegisterOverlayDisplayAdapterLocked();
        RegisterWifiDisplayAdapterLocked();
    }
}

void CDisplayManagerService::RegisterOverlayDisplayAdapterLocked()
{
    AutoPtr<OverlayDisplayAdapter> adapter = new OverlayDisplayAdapter(
        &mSyncRoot, mContext, mHandler, mDisplayAdapterListener, mUiHandler);
    RegisterDisplayAdapterLocked(adapter);
}

void CDisplayManagerService::RegisterWifiDisplayAdapterLocked()
{
    AutoPtr<IResources> resources;
    mContext->GetResources((IResources**)&resources);
    Boolean value;
    resources->GetBoolean(R::bool_::config_enableWifiDisplay, &value);
    AutoPtr<ISystemProperties> sysProp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
    Int32 enable;
    Boolean needRegister = value || (sysProp->GetInt32(FORCE_WIFI_DISPLAY_ENABLE, -1, &enable), enable == 1);
    if (DEBUG) {
        Slogger::D(TAG, "RegisterWifiDisplayAdapterLocked needRegister: %d, value: %d, enable: %d TODO",
            needRegister, value, enable);
    }
    needRegister = TRUE; // TODO
    if (needRegister) {
        assert(mWifiDisplayAdapter == NULL);
        mWifiDisplayAdapter = new WifiDisplayAdapter(
            &mSyncRoot, mContext, mHandler, mDisplayAdapterListener,
            mPersistentDataStore);
        RegisterDisplayAdapterLocked(mWifiDisplayAdapter);
    }
}

Boolean CDisplayManagerService::ShouldRegisterNonEssentialDisplayAdaptersLocked()
{
    // In safe mode, we disable non-essential display adapters to give the user
    // an opportunity to fix broken settings or other problems that might affect
    // system stability.
    // In only-core mode, we disable non-essential display adapters to minimize
    // the number of dependencies that are started while in this mode and to
    // prevent problems that might occur due to the device being encrypted.
    return !mSafeMode && !mOnlyCore;
}

void CDisplayManagerService::RegisterDisplayAdapterLocked(
    /* [in] */ DisplayAdapter* adapter)
{
    mDisplayAdapters.PushBack(adapter);
    adapter->RegisterLocked();
}

void CDisplayManagerService::HandleDisplayDeviceAdded(
   /* [in] */ DisplayDevice* device)
{
    AutoLock lock(mSyncRoot);

    if (Find(mDisplayDevices.Begin(), mDisplayDevices.End(),
        AutoPtr<DisplayDevice>(device)) != mDisplayDevices.End()) {
        Slogger::W(TAG, "Attempted to add already added display device: %s",
            device->GetDisplayDeviceInfoLocked()->ToString().string());
        return;
    }

    Slogger::I(TAG, "Display device added: %s",
        device->GetDisplayDeviceInfoLocked()->ToString().string());

    mDisplayDevices.PushBack(device);
    AddLogicalDisplayLocked(device);
    ScheduleTraversalLocked(FALSE);

    // Blank or unblank the display immediately to match the state requested
    // by the power manager (if known).
    switch (mAllDisplayBlankStateFromPowerManager) {
        case DISPLAY_BLANK_STATE_BLANKED:
            device->BlankLocked();
            break;
        case DISPLAY_BLANK_STATE_UNBLANKED:
            device->UnblankLocked();
            break;
    }
}

void CDisplayManagerService::HandleDisplayDeviceChanged(
    /* [in] */ DisplayDevice* device)
{
    AutoLock lock(mSyncRoot);
    if (Find(mDisplayDevices.Begin(), mDisplayDevices.End(),
        AutoPtr<DisplayDevice>(device)) == mDisplayDevices.End()) {
        Slogger::W(TAG, "Attempted to change non-existent display device: %s",
            device->GetDisplayDeviceInfoLocked()->ToString().string());
        return;
    }

    Slogger::I(TAG, "Display device changed: %s",
        device->GetDisplayDeviceInfoLocked()->ToString().string());

    device->ApplyPendingDisplayDeviceInfoChangesLocked();
    if (UpdateLogicalDisplaysLocked()) {
        ScheduleTraversalLocked(FALSE);
    }
}

void CDisplayManagerService::HandleDisplayDeviceRemoved(
    /* [in] */ DisplayDevice* device)
{
    AutoLock lock(mSyncRoot);

    List<AutoPtr<DisplayDevice> >::Iterator find = Find(
        mDisplayDevices.Begin(), mDisplayDevices.End(), AutoPtr<DisplayDevice>(device));
    if (find == mDisplayDevices.End()) {
        Slogger::W(TAG, "Attempted to remove non-existent display device: %s",
            device->GetDisplayDeviceInfoLocked()->ToString().string());
        return;
    }

    mDisplayDevices.Erase(find);

    Slogger::I(TAG, "Display device removed: %s",
        device->GetDisplayDeviceInfoLocked()->ToString().string());

    mRemovedDisplayDevices.PushBack(device);
    UpdateLogicalDisplaysLocked();
    ScheduleTraversalLocked(FALSE);
}

// Adds a new logical display based on the given display device.
// Sends notifications if needed.
void CDisplayManagerService::AddLogicalDisplayLocked(
    /* [in] */ DisplayDevice* device)
{
    AutoPtr<DisplayDeviceInfo> deviceInfo = device->GetDisplayDeviceInfoLocked();
    Boolean isDefault = (deviceInfo->mFlags
        & DisplayDeviceInfo::FLAG_DEFAULT_DISPLAY) != 0;
    if (isDefault && mLogicalDisplays.Find(IDisplay::DEFAULT_DISPLAY) != mLogicalDisplays.End()) {
        Slogger::W(TAG, "Ignoring attempt to add a second default display: %s"
            , deviceInfo->ToString().string());
        isDefault = FALSE;
    }

    if (!isDefault && mSingleDisplayDemoMode) {
        Slogger::I(TAG, "Not creating a logical display for a secondary display "
            " because single display demo mode is enabled: %s", deviceInfo->ToString().string());
        return;
    }

    Int32 displayId = AssignDisplayIdLocked(isDefault);
    Int32 layerStack = AssignLayerStackLocked(displayId);

    AutoPtr<LogicalDisplay> display = new LogicalDisplay(displayId, layerStack, device);
    display->UpdateLocked(mDisplayDevices);
    if (!display->IsValidLocked()) {
        // This should never happen currently.
        Slogger::W(TAG, "Ignoring display device because the logical display "
            "created from it was not considered valid: %s", deviceInfo->ToString().string());
        return;
    }

    mLogicalDisplays[displayId] = display;

    // Wake up waitForDefaultDisplay.
    if (isDefault) {
        mSyncRoot.NotifyAll();
    }

    SendDisplayEventLocked(displayId, IDisplayManagerGlobal::EVENT_DISPLAY_ADDED);
}

Int32 CDisplayManagerService::AssignDisplayIdLocked(
    /* [in] */ Boolean isDefault)
{
    return isDefault ? IDisplay::DEFAULT_DISPLAY : mNextNonDefaultDisplayId++;
}

Int32 CDisplayManagerService::AssignLayerStackLocked(
    /* [in] */ Int32 displayId)
{
    // Currently layer stacks and display ids are the same.
    // This need not be the case.
    return displayId;
}

// Updates all existing logical displays given the current set of display devices.
// Removes invalid logical displays.
// Sends notifications if needed.
Boolean CDisplayManagerService::UpdateLogicalDisplaysLocked()
{
    Boolean changed = FALSE;
    // TODO: from last index to 0
    //
    HashMap<Int32, AutoPtr<LogicalDisplay> >::Iterator iter = mLogicalDisplays.Begin();
    for (; iter != mLogicalDisplays.End();) {
        Int32 displayId = iter->mFirst;
        AutoPtr<LogicalDisplay> display = iter->mSecond;

        mTempDisplayInfo->CopyFrom(display->GetDisplayInfoLocked());
        display->UpdateLocked(mDisplayDevices);
        Boolean isEqual;
        if (!display->IsValidLocked()) {
            mLogicalDisplays.Erase(iter++);
            SendDisplayEventLocked(displayId, IDisplayManagerGlobal::EVENT_DISPLAY_REMOVED);
            changed = TRUE;
        }
        else if (!(mTempDisplayInfo->Equals(display->GetDisplayInfoLocked(), &isEqual), isEqual)) {
            ++iter;
            SendDisplayEventLocked(displayId, IDisplayManagerGlobal::EVENT_DISPLAY_CHANGED);
            changed = TRUE;
        }
        else {
            ++iter;
        }
    }
    return changed;
}

void CDisplayManagerService::PerformTraversalInTransactionLocked()
{
    // Perform one last traversal for each removed display device.
    List<AutoPtr<DisplayDevice> >::Iterator iter = mRemovedDisplayDevices.Begin();
    for (; iter != mRemovedDisplayDevices.End(); ++iter) {
        (*iter)->PerformTraversalInTransactionLocked();
    }
    mRemovedDisplayDevices.Clear();

    // Clear all viewports before configuring displays so that we can keep
    // track of which ones we have configured.
    ClearViewportsLocked();

    // Configure each display device.
    iter = mDisplayDevices.Begin();
    for (; iter != mDisplayDevices.End(); ++iter) {
        ConfigureDisplayInTransactionLocked(*iter);
        (*iter)->PerformTraversalInTransactionLocked();
    }

    // Tell the input system about these new viewports.
    if (mInputManagerFuncs != NULL) {
        Boolean result;
        mHandler->SendEmptyMessage(MSG_UPDATE_VIEWPORT, &result);
    }
}

/**
 * Tells the display manager whether there is interesting unique content on the
 * specified logical display.  This is used to control automatic mirroring.
 * <p>
 * If the display has unique content, then the display manager arranges for it
 * to be presented on a physical display if appropriate.  Otherwise, the display manager
 * may choose to make the physical display mirror some other logical display.
 * </p>
 *
 * @param displayId The logical display id to update.
 * @param hasContent True if the logical display has content.
 * @param inTraversal True if called from WindowManagerService during a window traversal prior
 * to call to performTraversalInTransactionFromWindowManager.
 */
ECode CDisplayManagerService::SetDisplayHasContent(
    /* [in] */ Int32 displayId,
    /* [in] */ Boolean hasContent,
    /* [in] */ Boolean inTraversal)
{
    AutoLock lock(mSyncRoot);
    AutoPtr<LogicalDisplay> display;
    HashMap<Int32, AutoPtr<LogicalDisplay> >::Iterator find = mLogicalDisplays.Find(displayId);
    if (find != mLogicalDisplays.End())
        display = find->mSecond;

    if (display != NULL && display->HasContentLocked() != hasContent) {
        display->SetHasContentLocked(hasContent);
        ScheduleTraversalLocked(inTraversal);
    }
    return NOERROR;
}

void CDisplayManagerService::ClearViewportsLocked()
{
    mDefaultViewport->mValid = FALSE;
    mExternalTouchViewport->mValid = FALSE;
}

void CDisplayManagerService::ConfigureDisplayInTransactionLocked(
    /* [in] */ DisplayDevice* device)
{
    // Find the logical display that the display device is showing.
    AutoPtr<LogicalDisplay> display = FindLogicalDisplayForDeviceLocked(device);
    if (display != NULL && !display->HasContentLocked()) {
        display = NULL;
    }
    if (display == NULL) {
        HashMap<Int32, AutoPtr<LogicalDisplay> >::Iterator find =
            mLogicalDisplays.Find(IDisplay::DEFAULT_DISPLAY);
        if (find != mLogicalDisplays.End())
            display = find->mSecond;
    }

    // Apply the logical display configuration to the display device.
    if (display == NULL) {
        // TODO: no logical display for the device, blank it
        Slogger::W(TAG, "Missing logical display to use for physical display device: %s"
            , device->GetDisplayDeviceInfoLocked()->ToString().string());
        return;
    }
    else {
        Boolean isBlanked = (mAllDisplayBlankStateFromPowerManager
            == DISPLAY_BLANK_STATE_BLANKED);
        display->ConfigureDisplayInTransactionLocked(device, isBlanked);
    }

    // Update the viewports if needed.
    AutoPtr<DisplayDeviceInfo> info = device->GetDisplayDeviceInfoLocked();
    if (!mDefaultViewport->mValid
        && (info->mFlags & DisplayDeviceInfo::FLAG_DEFAULT_DISPLAY) != 0) {
        SetViewportLocked(mDefaultViewport, display, device);
    }
    if (!mExternalTouchViewport->mValid
        && info->mTouch == DisplayDeviceInfo::TOUCH_EXTERNAL) {
        SetViewportLocked(mExternalTouchViewport, display, device);
    }
}

void CDisplayManagerService::SetViewportLocked(
    /* [in] */ IDisplayViewport* viewport,
    /* [in] */ LogicalDisplay* display,
    /* [in] */ DisplayDevice* device)
{
    viewport->mValid = TRUE;
    viewport->mDisplayId = display->GetDisplayIdLocked();
    device->PopulateViewportLocked(viewport);
}

AutoPtr<LogicalDisplay> CDisplayManagerService::FindLogicalDisplayForDeviceLocked(
    /* [in] */ DisplayDevice* device)
{
    HashMap<Int32, AutoPtr<LogicalDisplay> >::Iterator iter;
    for (iter = mLogicalDisplays.Begin(); iter != mLogicalDisplays.End(); ++iter) {
        AutoPtr<LogicalDisplay> display = iter->mSecond;
        if (display->GetPrimaryDisplayDeviceLocked().Get() == device) {
            return display;
        }
    }
    return NULL;
}

void CDisplayManagerService::SendDisplayEventLocked(
    /* [in] */ Int32 displayId,
    /* [in] */ Int32 event)
{
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(MSG_DELIVER_DISPLAY_EVENT, displayId, event, (IMessage**)&msg);
    Boolean result;
    mHandler->SendMessage(msg, &result);
}

// Requests that performTraversalsInTransactionFromWindowManager be called at a
// later time to apply changes to surfaces and displays.
void CDisplayManagerService::ScheduleTraversalLocked(
    /* [in] */ Boolean inTraversal)
{
    if (DEBUG) {
        Slogger::D(TAG, "ScheduleTraversalLocked inTraversal:%d", inTraversal);
    }

    if (!mPendingTraversal && mWindowManagerFuncs != NULL) {
        mPendingTraversal = TRUE;
        if (!inTraversal) {
            Boolean result;
            mHandler->SendEmptyMessage(MSG_REQUEST_TRAVERSAL, &result);
        }
    }
}

// Runs on Handler thread.
// Delivers display event notifications to callbacks.
void CDisplayManagerService::DeliverDisplayEvent(
    /* [in] */ Int32 displayId,
    /* [in] */ Int32 event)
{
    if (DEBUG) {
        Slogger::D(TAG, "Delivering display event: displayId=%d, event=%d",
            displayId, event);
    }

    // Grab the lock and copy the callbacks.
    {
        AutoLock lock(mSyncRoot);
        mTempCallbacks.Clear();
        HashMap<Int32, AutoPtr<CallbackRecord> >::Iterator iter;
        for (iter = mCallbacks.Begin(); iter != mCallbacks.End(); ++iter) {
            mTempCallbacks.PushBack(iter->mSecond);
        }
    }

    // After releasing the lock, send the notifications out.
    List<AutoPtr<CallbackRecord> >::Iterator iter = mTempCallbacks.Begin();
    for (; iter != mTempCallbacks.End(); ++iter) {
        (*iter)->NotifyDisplayEventAsync(displayId, event);
    }
    mTempCallbacks.Clear();
}

//@Override // Binder call
ECode CDisplayManagerService::Dump(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* pw,
    /* [in] */ ArrayOf<String>* args)
{
    // Int32 res;
    // if (mContext == NULL
    //     || (mContext->CheckCallingOrSelfPermission(Manifest::permission::DUMP, &res)
    //         , res != PackageManager.PERMISSION_GRANTED)) {
    //     pw->PrintStringln(String("Permission Denial: can't dump DisplayManager from from pid=")
    //         + Binder::GetCallingPid() + ", uid=" + Binder::GetCallingUid());
    //     return;
    // }

    // pw->PrintStringln(String("DISPLAY MANAGER (dumpsys display)"));

    // AutoLock lock(mSyncRoot);

    // pw->PrintStringln(String("  mHeadless=") + mHeadless);
    // pw->PrintStringln(String("  mOnlyCode=") + mOnlyCore);
    // pw->PrintStringln(String("  mSafeMode=") + mSafeMode);
    // pw->PrintStringln(String("  mPendingTraversal=") + mPendingTraversal);
    // pw->PrintStringln(String("  mAllDisplayBlankStateFromPowerManager=")
    //         + mAllDisplayBlankStateFromPowerManager);
    // pw->PrintStringln(String("  mNextNonDefaultDisplayId=") + mNextNonDefaultDisplayId);
    // pw->PrintStringln(String("  mDefaultViewport=") + mDefaultViewport);
    // pw->PrintStringln(String("  mExternalTouchViewport=") + mExternalTouchViewport);
    // pw->PrintStringln(String("  mSingleDisplayDemoMode=") + mSingleDisplayDemoMode);

    // IndentingPrintWriter ipw = new IndentingPrintWriter(pw, "    ");
    // ipw->IncreaseIndent();

    // pw->PrintStringln();
    // pw->PrintStringln(String("Display Adapters: size=") + mDisplayAdapters.size());
    // for (DisplayAdapter adapter : mDisplayAdapters) {
    //     pw->PrintStringln(String("  " + adapter.getName());
    //     adapter.dumpLocked(ipw);
    // }

    // pw->PrintStringln();
    // pw->PrintStringln(String("Display Devices: size=") + mDisplayDevices.size());
    // for (DisplayDevice device : mDisplayDevices) {
    //     pw->PrintStringln(String("  " + device->GetDisplayDeviceInfoLocked()->ToString());
    //     device.dumpLocked(ipw);
    // }

    // Int32 logicalDisplayCount = mLogicalDisplays->Size();
    // pw->PrintStringln();
    // pw->PrintStringln(String("Logical Displays: size=") + logicalDisplayCount);
    // for (Int32 i = 0; i < logicalDisplayCount; i++) {
    //     Int32 displayId = mLogicalDisplays->KeyAt(i);
    //     LogicalDisplay display = mLogicalDisplays->ValueAt(i);
    //     pw->PrintStringln(String("  Display ") + displayId + ":");
    //     display.dumpLocked(ipw);
    // }
    return NOERROR;
}

} // namespace Display
} // namespace Server
} // namepsace Droid
} // namespace Elastos
