
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

//==============================================================================
// CDisplayManagerService::BinderService
//==============================================================================

CAR_INTERFACE_IMPL_2(BinderService, Object, IIDisplayManager, IBinder)

CARAPI constructor();

/**
 * Returns information about the specified logical display.
 *
 * @param displayId The logical display id.
 * @return The logical display info, or null if the display does not exist.  The
 * returned object must be treated as immutable.
 */
// @Override // Binder call
CARAPI GetDisplayInfo(
    /* [in] */ Int32 displayId,
    /* [out] */ IDisplayInfo** info)
{
    final int callingUid = Binder.getCallingUid();
    final long token = Binder.ClearCallingIdentity();
    try {
        return getDisplayInfoInternal(displayId, callingUid);
    } finally {
        Binder.RestoreCallingIdentity(token);
    }
}

/**
 * Returns the list of all display ids.
 */
// @Override // Binder call
CARAPI GetDisplayIds(
    /* [out, callee] */ ArrayOf<Int32>** ids)
{
    final int callingUid = Binder.getCallingUid();
    final long token = Binder.ClearCallingIdentity();
    try {
        return getDisplayIdsInternal(callingUid);
    } finally {
        Binder.RestoreCallingIdentity(token);
    }
}

//@Override // Binder call
CARAPI RegisterCallback(
    /* [in] */ IIDisplayManagerCallback* callback)
{
    if (callback == null) {
        throw new IllegalArgumentException("listener must not be null");
    }

    final int callingPid = Binder.getCallingPid();
    final long token = Binder.ClearCallingIdentity();
    try {
        registerCallbackInternal(callback, callingPid);
    } finally {
        Binder.RestoreCallingIdentity(token);
    }
}

//@Override // Binder call
CARAPI StartWifiDisplayScan()
{
    mContext.EnforceCallingOrSelfPermission(Manifest.permission.CONFIGURE_WIFI_DISPLAY,
            "Permission required to start wifi display scans");

    final int callingPid = Binder.getCallingPid();
    final long token = Binder.ClearCallingIdentity();
    try {
        startWifiDisplayScanInternal(callingPid);
    } finally {
        Binder.RestoreCallingIdentity(token);
    }
}

//@Override // Binder call
CARAPI StopWifiDisplayScan()
{
    mContext.EnforceCallingOrSelfPermission(Manifest.permission.CONFIGURE_WIFI_DISPLAY,
            "Permission required to stop wifi display scans");

    final int callingPid = Binder.getCallingPid();
    final long token = Binder.ClearCallingIdentity();
    try {
        stopWifiDisplayScanInternal(callingPid);
    } finally {
        Binder.RestoreCallingIdentity(token);
    }
}

//@Override // Binder call
CARAPI ConnectWifiDisplay(
    /* [in] */ const String& address)
{
    if (address == null) {
        throw new IllegalArgumentException("address must not be null");
    }
    mContext.EnforceCallingOrSelfPermission(Manifest.permission.CONFIGURE_WIFI_DISPLAY,
            "Permission required to connect to a wifi display");

    final long token = Binder.ClearCallingIdentity();
    try {
        connectWifiDisplayInternal(address);
    } finally {
        Binder.RestoreCallingIdentity(token);
    }
}

//@Override // Binder call
CARAPI DisconnectWifiDisplay()
{
    // This request does not require special permissions.
    // Any app can request disconnection from the currently active wifi display.
    // This exception should no longer be needed once wifi display control moves
    // to the media router service.

    final long token = Binder.ClearCallingIdentity();
    try {
        DisconnectWifiDisplayInternal();
    } finally {
        Binder.RestoreCallingIdentity(token);
    }
}

//@Override // Binder call
CARAPI RenameWifiDisplay(
    /* [in] */ const String& address,
    /* [in] */ const String& alias)
{
    if (address == null) {
        throw new IllegalArgumentException("address must not be null");
    }
    mContext.EnforceCallingOrSelfPermission(Manifest.permission.CONFIGURE_WIFI_DISPLAY,
            "Permission required to rename to a wifi display");

    final long token = Binder.ClearCallingIdentity();
    try {
        RenameWifiDisplayInternal(address, alias);
    } finally {
        Binder.RestoreCallingIdentity(token);
    }
}

//@Override // Binder call
CARAPI ForgetWifiDisplay(
    /* [in] */ const String& address)
{
    if (address == null) {
        throw new IllegalArgumentException("address must not be null");
    }
    mContext.EnforceCallingOrSelfPermission(Manifest.permission.CONFIGURE_WIFI_DISPLAY,
            "Permission required to forget to a wifi display");

    final long token = Binder.ClearCallingIdentity();
    try {
        ForgetWifiDisplayInternal(address);
    } finally {
        Binder.RestoreCallingIdentity(token);
    }
}

//@Override // Binder call
CARAPI PauseWifiDisplay()
{
    mContext.EnforceCallingOrSelfPermission(Manifest.permission.CONFIGURE_WIFI_DISPLAY,
            "Permission required to pause a wifi display session");

    final long token = Binder.ClearCallingIdentity();
    try {
        PauseWifiDisplayInternal();
    } finally {
        Binder.RestoreCallingIdentity(token);
    }
}

//@Override // Binder call
CARAPI ResumeWifiDisplay()
{
    mContext.EnforceCallingOrSelfPermission(Manifest.permission.CONFIGURE_WIFI_DISPLAY,
            "Permission required to resume a wifi display session");

    final long token = Binder.ClearCallingIdentity();
    try {
        ResumeWifiDisplayInternal();
    } finally {
        Binder.RestoreCallingIdentity(token);
    }
}

//@Override // Binder call
CARAPI GetWifiDisplayStatus(
    /* [out] */ IWifiDisplayStatus** status)
{
    // This request does not require special permissions.
    // Any app can get information about available wifi displays.

    final long token = Binder.ClearCallingIdentity();
    try {
        return GetWifiDisplayStatusInternal();
    } finally {
        Binder.RestoreCallingIdentity(token);
    }
}

//@Override // Binder call
CARAPI CreateVirtualDisplay(
    /* [in] */ IIVirtualDisplayCallback* callback,
    /* [in] */ IIMediaProjection* projection,
    /* [in] */ const String& packageName,
    /* [in] */ const String& name,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 densityDpi,
    /* [in] */ ISurface* surface,
    /* [in] */ Int32 flags,
    /* [out] */ Int32* result)
{
    final int callingUid = Binder.getCallingUid();
    if (!validatePackageName(callingUid, packageName)) {
        throw new SecurityException("packageName must match the calling uid");
    }
    if (callback == null) {
        throw new IllegalArgumentException("appToken must not be null");
    }
    if (TextUtils.isEmpty(name)) {
        throw new IllegalArgumentException("name must be non-null and non-empty");
    }
    if (width <= 0 || height <= 0 || densityDpi <= 0) {
        throw new IllegalArgumentException("width, height, and densityDpi must be "
                + "greater than 0");
    }

    if ((flags & DisplayManager.VIRTUAL_DISPLAY_FLAG_PUBLIC) != 0) {
        flags |= DisplayManager.VIRTUAL_DISPLAY_FLAG_AUTO_MIRROR;
    }
    if ((flags & DisplayManager.VIRTUAL_DISPLAY_FLAG_OWN_CONTENT_ONLY) != 0) {
        flags &= ~DisplayManager.VIRTUAL_DISPLAY_FLAG_AUTO_MIRROR;
    }

    if (projection != null) {
        try {
            if (!getProjectionService().isValidMediaProjection(projection)) {
                throw new SecurityException("Invalid media projection");
            }
            flags = projection.applyVirtualDisplayFlags(flags);
        } catch (RemoteException e) {
            throw new SecurityException("unable to validate media projection or flags");
        }
    }

    if (callingUid != Process.SYSTEM_UID &&
            (flags & DisplayManager.VIRTUAL_DISPLAY_FLAG_AUTO_MIRROR) != 0) {
        if (!canProjectVideo(projection)) {
            throw new SecurityException("Requires CAPTURE_VIDEO_OUTPUT or "
                    + "CAPTURE_SECURE_VIDEO_OUTPUT permission, or an appropriate "
                    + "MediaProjection token in order to create a screen sharing virtual "
                    + "display.");
        }
    }
    if ((flags & DisplayManager.VIRTUAL_DISPLAY_FLAG_SECURE) != 0) {
        if (!canProjectSecureVideo(projection)) {
            throw new SecurityException("Requires CAPTURE_SECURE_VIDEO_OUTPUT "
                    + "or an appropriate MediaProjection token to create a "
                    + "secure virtual display.");
        }
    }

    final long token = Binder.ClearCallingIdentity();
    try {
        return createVirtualDisplayInternal(callback, projection, callingUid,
                packageName, name, width, height, densityDpi, surface, flags);
    } finally {
        Binder.RestoreCallingIdentity(token);
    }
}

//@Override // Binder call
CARAPI ResizeVirtualDisplay(
    /* [in] */ IIVirtualDisplayCallback* callback,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 densityDpi)
{
    final long token = Binder.ClearCallingIdentity();
    try {
        resizeVirtualDisplayInternal(callback.asBinder(), width, height, densityDpi);
    } finally {
        Binder.RestoreCallingIdentity(token);
    }
}

//@Override // Binder call
CARAPI SetVirtualDisplaySurface(
    /* [in] */ IIVirtualDisplayCallback* callback,
    /* [in] */ ISurface* surface)
{
    final long token = Binder.ClearCallingIdentity();
    try {
        setVirtualDisplaySurfaceInternal(callback.asBinder(), surface);
    } finally {
        Binder.RestoreCallingIdentity(token);
    }
}

//@Override // Binder call
CARAPI ReleaseVirtualDisplay(
    /* [in] */ IIVirtualDisplayCallback* callback)
{
    final long token = Binder.ClearCallingIdentity();
    try {
        releaseVirtualDisplayInternal(callback.asBinder());
    } finally {
        Binder.RestoreCallingIdentity(token);
    }
}

//@Override // Binder call
CARAPI Dump(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* pw,
    /* [in] */ ArrayOf<String>* args)
{
    if (mContext == null
            || mContext.checkCallingOrSelfPermission(Manifest.permission.DUMP)
                    != PackageManager.PERMISSION_GRANTED) {
        pw.println("Permission Denial: can't dump DisplayManager from from pid="
                + Binder.getCallingPid() + ", uid=" + Binder.getCallingUid());
        return;
    }

    final long token = Binder.ClearCallingIdentity();
    try {
        dumpInternal(pw);
    } finally {
        Binder.RestoreCallingIdentity(token);
    }
}

Boolean ValidatePackageName(
    /* [in] */ Int32 uid,
    /* [in] */ const String& packageName)
{
    if (packageName != null) {
        String[] packageNames = mContext.getPackageManager().getPackagesForUid(uid);
        if (packageNames != null) {
            for (String n : packageNames) {
                if (n.equals(packageName)) {
                    return true;
                }
            }
        }
    }
    return false;
}

Boolean CanProjectVideo(
    /* [in] */ IIMediaProjection* projection)
{
    if (projection != null) {
        try {
            if (projection.canProjectVideo()) {
                return true;
            }
        } catch (RemoteException e) {
            Slog.e(TAG, "Unable to query projection service for permissions", e);
        }
    }
    if (mContext.checkCallingPermission(
            android.Manifest.permission.CAPTURE_VIDEO_OUTPUT)
            == PackageManager.PERMISSION_GRANTED) {
        return true;
    }
    return canProjectSecureVideo(projection);
}

Boolean CanProjectSecureVideo(
    /* [in] */ IIMediaProjection* projection)
{
    if (projection != null) {
        try {
            if (projection.canProjectSecureVideo()){
                return true;
            }
        } catch (RemoteException e) {
            Slog.e(TAG, "Unable to query projection service for permissions", e);
        }
    }
    return mContext.checkCallingPermission(
            android.Manifest.permission.CAPTURE_SECURE_VIDEO_OUTPUT)
            == PackageManager.PERMISSION_GRANTED;
}

//==============================================================================
// CDisplayManagerService::LocalService
//==============================================================================

CAR_INTERFACE_IMPL(LocalService, Object, IDisplayManagerInternal)

CARAPI constructor();

// @Override
CARAPI InitPowerManagement(
    /* [in] */ IDisplayPowerCallbacks* callbacks,
    /* [in] */ IHandler* handler,
    /* [in] */ ISensorManager* sensorManager)
{
    synchronized (mSyncRoot) {
        DisplayBlanker blanker = new DisplayBlanker() {
            @Override
            CARAPI requestDisplayState(int state) {
                // The order of operations is important for legacy reasons.
                if (state == Display.STATE_OFF) {
                    requestGlobalDisplayStateInternal(state);
                }

                callbacks.onDisplayStateChange(state);

                if (state != Display.STATE_OFF) {
                    requestGlobalDisplayStateInternal(state);
                }
            }
        };
        mDisplayPowerController = new DisplayPowerController(
                mContext, callbacks, handler, sensorManager, blanker);
    }
}

// @Override
CARAPI RequestPowerState(
    /* [in] */ IDisplayPowerRequest* request,
    /* [in] */ Boolean waitForNegativeProximity,
    /* [out] */ Boolean* result)
{
    return mDisplayPowerController.requestPowerState(request,
            waitForNegativeProximity);
}

// @Override
CARAPI IsProximitySensorAvailable(
    /* [out] */ Boolean* result)
{
    return mDisplayPowerController.isProximitySensorAvailable();
}

// @Override
CARAPI GetDisplayInfo(
    /* [in] */ Int32 displayId,
    /* [out] */ IDisplayInfo** result)
{
    return GetDisplayInfoInternal(displayId, Process.myUid());
}

// @Override
CARAPI RegisterDisplayTransactionListener(
    /* [in] */ IDisplayTransactionListener* listener)
{
    if (listener == null) {
        throw new IllegalArgumentException("listener must not be null");
    }

    RegisterDisplayTransactionListenerInternal(listener);
}

// @Override
CARAPI UnregisterDisplayTransactionListener(
    /* [in] */ IDisplayTransactionListener* listener)
{
    if (listener == null) {
        throw new IllegalArgumentException("listener must not be null");
    }

    UnregisterDisplayTransactionListenerInternal(listener);
}

// @Override
CARAPI SetDisplayInfoOverrideFromWindowManager(
    /* [in] */ Int32 displayId,
    /* [in] */ IDisplayInfo* info)
{
    SetDisplayInfoOverrideFromWindowManagerInternal(displayId, info);
}

// @Override
CARAPI PerformTraversalInTransactionFromWindowManager()
{
    PerformTraversalInTransactionFromWindowManagerInternal();
}

// @Override
CARAPI SetDisplayProperties(
    /* [in] */ Int32 displayId,
    /* [in] */ Boolean hasContent,
    /* [in] */ Float requestedRefreshRate,
    /* [in] */ Boolean inTraversal)
{
    SetDisplayPropertiesInternal(displayId, hasContent, requestedRefreshRate, inTraversal);
}

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
            if (mHost->mWindowManagerInternal)
                mHost->mWindowManagerInternal->RequestTraversalFromDisplayManager();
            break;

        case MSG_UPDATE_VIEWPORT:
        {
            {
                AutoLock lock(mHost->mSyncRoot);
                mHost->mTempDefaultViewport->CopyFrom(mHost->mDefaultViewport);
                mHost->mTempExternalTouchViewport->CopyFrom(mHost->mExternalTouchViewport);
            }
            if (mHost->mInputManagerInternal) {
                mHost->mInputManagerInternal->SetDisplayViewports(
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
    : mPid(pid)
    , mWifiDisplayScanRequested(FALSE)
    , mHost(owner)
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
    mHost->OnCallbackDied(this);

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

CAR_OBJECT_IMPL(CDisplayManagerService)

CDisplayManagerService::CDisplayManagerService()
    : mSafeMode(FALSE)
    , mOnlyCore(FALSE)
    , mSingleDisplayDemoMode(FALSE)
    , mNextNonDefaultDisplayId(IDisplay::DEFAULT_DISPLAY + 1)
    , mAllDisplayBlankStateFromPowerManager(0)
    , mGlobalDisplayState(IDisplay::STATE_UNKNOWN)
    , mPendingTraversal(FALSE)
    , mWifiDisplayScanRequestCount(0)
{
    mDefaultViewport = new CDisplayViewport();
    mExternalTouchViewport = new CDisplayViewport();
    mPersistentDataStore = new PersistentDataStore();
    CDisplayInfo::New((IDisplayInfo**)&mTempDisplayInfo);
    mTempDefaultViewport = new CDisplayViewport();
    mTempExternalTouchViewport = new CDisplayViewport();
}

ECode CDisplayManagerService::constructor(
    /* [in] */ IContext* context)
{
    SystemService::constructor(context);
    mContext = context;

    AutoPtr<ILooper> looper;
    assert(0 && "TODO");
    // looper = DisplayThread.get().getLooper();

    mHandler = new DisplayManagerHandler(looper);
    // mUiHandler = UiThread::GetHandler();
    mDisplayAdapterListener = new DisplayAdapterListener(this);
    mSingleDisplayDemoMode = SystemProperties.getBoolean("persist.demo.singledisplay", false);

    AutoPtr<ISystemProperties> sysProp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp)
    sysProp->GetBoolean(String("persist.demo.singledisplay"), FALSE, &mSingleDisplayDemoMode);

    return NOERROR;
}

ECode CDisplayManagerService::OnStart()
{
    Boolean bval;
    mHandler->SendEmptyMessage(MSG_REGISTER_DEFAULT_DISPLAY_ADAPTER, &bval);

    PublishBinderService(Context.DISPLAY_SERVICE, new BinderService(),
            true /*allowIsolated*/);
    PublishLocalService(DisplayManagerInternal.class, new LocalService());
    return NOERROR;
}

ECode CDisplayManagerService::OnBootPhase(
    /* [in] */ Int32 phase)
{
    if (phase == PHASE_WAIT_FOR_DEFAULT_DISPLAY) {
        synchronized (mSyncRoot) {
            long timeout = SystemClock.uptimeMillis() + WAIT_FOR_DEFAULT_DISPLAY_TIMEOUT;
            while (mLogicalDisplays.get(Display.DEFAULT_DISPLAY) == null) {
                long delay = timeout - SystemClock.uptimeMillis();
                if (delay <= 0) {
                    throw new RuntimeException("Timeout waiting for default display "
                            + "to be initialized.");
                }
                if (DEBUG) {
                    Slog.d(TAG, "waitForDefaultDisplay: waiting, timeout=" + delay);
                }
                try {
                    mSyncRoot.wait(delay);
                } catch (InterruptedException ex) {
                }
            }
        }
    }
}

ECode CDisplayManagerService::WindowManagerAndInputReady() {
    synchronized (mSyncRoot) {
        mWindowManagerInternal = LocalServices.getService(WindowManagerInternal.class);
        mInputManagerInternal = LocalServices.getService(InputManagerInternal.class);
        scheduleTraversalLocked(false);
    }
}

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

ECode CDisplayManagerService::RegisterDisplayTransactionListenerInternal(
    /* [in] */ IDisplayTransactionListener* listener)
{
    // List is self-synchronized copy-on-write.
    mDisplayTransactionListeners->Add(listener);
    return NOERROR;
}

ECode CDisplayManagerService::UnregisterDisplayTransactionListenerInternal(
    /* [in] */ IDisplayTransactionListener* listener)
{
    // List is self-synchronized copy-on-write.
    mDisplayTransactionListeners->Remove(listener);
    return NOERROR;
}

ECode CDisplayManagerService::SetDisplayInfoOverrideFromWindowManagerInternal(
    /* [in] */ Int32 displayId,
    /* [in] */ IDisplayInfo* info)
{
    AutoLock lock(mSyncRoot);

    HashMap<Int32, AutoPtr<LogicalDisplay> >::Iterator find = mLogicalDisplays.Find(displayId);
    if (find != mLogicalDisplays.End()) {
        AutoPtr<LogicalDisplay> display = find->mSecond;
        if (display->SetDisplayInfoOverrideFromWindowManagerLocked(info)) {
            SendDisplayEventLocked(displayId, IDisplayManagerGlobal::EVENT_DISPLAY_CHANGED);
            ScheduleTraversalLocked(FALSE);
        }
    }

    return NOERROR;
}

ECode CDisplayManagerService::PerformTraversalInTransactionFromWindowManagerInternal()
{
    AutoLock lock(mSyncRoot);
    if (!mPendingTraversal) {
        return NOERROR;
    }
    mPendingTraversal = FALSE;

    PerformTraversalInTransactionLocked();

    {
        // List is self-synchronized copy-on-write.
        List< AutoPtr<IDisplayTransactionListener> >::Iterator it;
        for (it = mDisplayTransactionListeners.Begin(); it != mDisplayTransactionListeners.End(); ++it) {
            (*it)->OnDisplayTransaction();
        }
    }

    return NOERROR;
}

ECode CDisplayManagerService::RequestGlobalDisplayStateInternal(
        /* [in] */ Int32 state)
{
    synchronized (mTempDisplayStateWorkQueue) {
        try {
            // Update the display state within the lock.
            synchronized (mSyncRoot) {
                if (mGlobalDisplayState != state) {
                    mGlobalDisplayState = state;
                    updateGlobalDisplayStateLocked(mTempDisplayStateWorkQueue);
                    scheduleTraversalLocked(false);
                }
            }

            // Setting the display power state can take hundreds of milliseconds
            // to complete so we defer the most expensive part of the work until
            // after we have exited the critical section to avoid blocking other
            // threads for a long time.
            for (int i = 0; i < mTempDisplayStateWorkQueue.size(); i++) {
                mTempDisplayStateWorkQueue.get(i).run();
            }
        } finally {
            mTempDisplayStateWorkQueue.clear();
        }
    }
}

ECode CDisplayManagerService::GetDisplayInfoInternal(
    /* [in] */ Int32 displayId,
    /* [in] */ Int32 callingUid,
    /* [out] */ IDisplayInfo** displayInfo)
{
    synchronized (mSyncRoot) {
        LogicalDisplay display = mLogicalDisplays.get(displayId);
        if (display != null) {
            DisplayInfo info = display.getDisplayInfoLocked();
            if (info.hasAccess(callingUid)) {
                return info;
            }
        }
        return null;
    }
}

ECode CDisplayManagerService::GetDisplayIdsInternal(
    /* [in] */ Int32 callingUid,
    /* [out, callee] */ ArrayOf<Int32>** displayIds)
{
    VALIDATE_NOT_NULL(displayIds);

    synchronized (mSyncRoot) {
        final int count = mLogicalDisplays.size();
        int[] displayIds = new int[count];
        int n = 0;
        for (int i = 0; i < count; i++) {
            LogicalDisplay display = mLogicalDisplays.valueAt(i);
            DisplayInfo info = display.getDisplayInfoLocked();
            if (info.hasAccess(callingUid)) {
                displayIds[n++] = mLogicalDisplays.keyAt(i);
            }
        }
        if (n != count) {
            displayIds = Arrays.copyOfRange(displayIds, 0, n);
        }
        return displayIds;
    }

    // AutoLock lock(mSyncRoot);
    // Int32 count = mLogicalDisplays.GetSize();
    // *displayIds = ArrayOf<Int32>::Alloc(count);
    // if (*displayIds == NULL)
    //     return E_OUT_OF_MEMORY_ERROR;

    // REFCOUNT_ADD(*displayIds);
    // HashMap<Int32, AutoPtr<LogicalDisplay> >::Iterator iter
    //     = mLogicalDisplays.Begin();
    // for (Int32 i = 0; i < count; ++iter, i++) {
    //     (**displayIds)[i] = iter->mFirst;
    // }

    return NOERROR;
}

ECode CDisplayManagerService::RegisterCallbackInternal(
    /* [in] */ IDisplayManagerCallback* callback,
    /* [in] */ Int32 callingUid)
{
    // if (callback == NULL) {
    //     Slogger::E(TAG, "listener must not be NULL");
    //     return E_ILLEGAL_ARGUMENT_EXCEPTION;
    // }

    // AutoLock lock(mSyncRoot);

    // Int32 callingPid = Binder::GetCallingPid();
    // HashMap<Int32, AutoPtr<CallbackRecord> >::Iterator find
    //     = mCallbacks.Find(callingPid);
    // if (find != mCallbacks.End()) {
    //     Slogger::E(TAG, "The calling process has already "
    //         "registered an IDisplayManagerCallback.");
    //     return E_SECURITY_EXCEPTION;
    // }

    // AutoPtr<CallbackRecord> record = new CallbackRecord(this, callingPid, callback);

    // AutoPtr<IProxy> proxy = (IProxy*)callback->Probe(EIID_IProxy);
    // if (proxy != NULL && FAILED(proxy->LinkToDeath(record, 0))) {
    //     return E_RUNTIME_EXCEPTION;
    // }

    // mCallbacks[callingPid] = record;

    synchronized (mSyncRoot) {
        if (mCallbacks.get(callingPid) != null) {
            throw new SecurityException("The calling process has already "
                    + "registered an IDisplayManagerCallback.");
        }

        CallbackRecord record = new CallbackRecord(callingPid, callback);
        try {
            IBinder binder = callback.asBinder();
            binder.linkToDeath(record, 0);
        } catch (RemoteException ex) {
            // give up
            throw new RuntimeException(ex);
        }

        mCallbacks.put(callingPid, record);
    }

    return NOERROR;
}

void CDisplayManagerService::OnCallbackDied(
    /* [in] */ CallbackRecord* record)
{
    synchronized (mSyncRoot) {
        mCallbacks.remove(record.mPid);
        stopWifiDisplayScanLocked(record);
    }
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


ECode CDisplayManagerService::StartWifiDisplayScanInternal(
    /* [in] */ Int32 callingPid)
{
    synchronized (mSyncRoot) {
        CallbackRecord record = mCallbacks.get(callingPid);
        if (record == null) {
            throw new IllegalStateException("The calling process has not "
                    + "registered an IDisplayManagerCallback.");
        }
        startWifiDisplayScanLocked(record);
    }
}

ECode CDisplayManagerService::StartWifiDisplayScanLocked(
    /* [in] */ CallbackRecord* record)
{
    if (!record.mWifiDisplayScanRequested) {
        record.mWifiDisplayScanRequested = true;
        if (mWifiDisplayScanRequestCount++ == 0) {
            if (mWifiDisplayAdapter != null) {
                mWifiDisplayAdapter.requestStartScanLocked();
            }
        }
    }
}

ECode CDisplayManagerService::StopWifiDisplayScanInternal(
    /* [in] */ Int32 callingPid)
{
    synchronized (mSyncRoot) {
        CallbackRecord record = mCallbacks.get(callingPid);
        if (record == null) {
            throw new IllegalStateException("The calling process has not "
                    + "registered an IDisplayManagerCallback.");
        }
        stopWifiDisplayScanLocked(record);
    }
}

ECode CDisplayManagerService::StopWifiDisplayScanLocked(
    /* [in] */ CallbackRecord* record)
{
    if (record.mWifiDisplayScanRequested) {
        record.mWifiDisplayScanRequested = false;
        if (--mWifiDisplayScanRequestCount == 0) {
            if (mWifiDisplayAdapter != null) {
                mWifiDisplayAdapter.requestStopScanLocked();
            }
        } else if (mWifiDisplayScanRequestCount < 0) {
            Slog.wtf(TAG, "mWifiDisplayScanRequestCount became negative: "
                    + mWifiDisplayScanRequestCount);
            mWifiDisplayScanRequestCount = 0;
        }
    }
}

ECode CDisplayManagerService::ConnectWifiDisplayInternal(
    /* [in] */ const String& address)
{
    synchronized (mSyncRoot) {
        if (mWifiDisplayAdapter != null) {
            mWifiDisplayAdapter.requestConnectLocked(address);
        }
    }
}

ECode CDisplayManagerService::PauseWifiDisplayInternal()
{
    synchronized (mSyncRoot) {
        if (mWifiDisplayAdapter != null) {
            mWifiDisplayAdapter.requestPauseLocked();
        }
    }
}

ECode CDisplayManagerService::ResumeWifiDisplayInternal()
{
    synchronized (mSyncRoot) {
        if (mWifiDisplayAdapter != null) {
            mWifiDisplayAdapter.requestResumeLocked();
        }
    }
}

ECode CDisplayManagerService::DisconnectWifiDisplayInternal()
{
    synchronized (mSyncRoot) {
        if (mWifiDisplayAdapter != null) {
            mWifiDisplayAdapter.requestDisconnectLocked();
        }
    }
}

ECode CDisplayManagerService::RenameWifiDisplayInternal(
    /* [in] */ const String& address,
    /* [in] */ const String& alias)
{
    synchronized (mSyncRoot) {
        if (mWifiDisplayAdapter != null) {
            mWifiDisplayAdapter.requestRenameLocked(address, alias);
        }
    }
}

ECode CDisplayManagerService::ForgetWifiDisplayInternal(
    /* [in] */ const String& address)
{
    synchronized (mSyncRoot) {
        if (mWifiDisplayAdapter != null) {
            mWifiDisplayAdapter.requestForgetLocked(address);
        }
    }
}

ECode CDisplayManagerService::GetWifiDisplayStatusInternal(
    /* [out] */ IWifiDisplayStatus** status)
{
    VALIDATE_NOT_NULL(status);

    synchronized (mSyncRoot) {
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

    return NOERROR;
}

Int32 CDisplayManagerService::CreateVirtualDisplayInternal(
    /* [in] */ IIVirtualDisplayCallback* callback,
    /* [in] */ IIMediaProjection* projection,
    /* [in] */ Int32 callingUid,
    /* [in] */ const String& packageName,
    /* [in] */ const String& name,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 densityDpi,
    /* [in] */ ISurface* surface,
    /* [in] */ Int32 flags)
{
    synchronized (mSyncRoot) {
        if (mVirtualDisplayAdapter == null) {
            Slog.w(TAG, "Rejecting request to create private virtual display "
                    + "because the virtual display adapter is not available.");
            return -1;
        }

        DisplayDevice device = mVirtualDisplayAdapter.createVirtualDisplayLocked(
                callback, projection, callingUid, packageName,
                name, width, height, densityDpi, surface, flags);
        if (device == null) {
            return -1;
        }

        handleDisplayDeviceAddedLocked(device);
        LogicalDisplay display = findLogicalDisplayForDeviceLocked(device);
        if (display != null) {
            return display.getDisplayIdLocked();
        }

        // Something weird happened and the logical display was not created.
        Slog.w(TAG, "Rejecting request to create virtual display "
                + "because the logical display was not created.");
        mVirtualDisplayAdapter.releaseVirtualDisplayLocked(callback.asBinder());
        handleDisplayDeviceRemovedLocked(device);
    }
    return -1;
}

ECode CDisplayManagerService::ResizeVirtualDisplayInternal(
    /* [in] */ IBinder* appToken,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 densityDpi)
{
    synchronized (mSyncRoot) {
        if (mVirtualDisplayAdapter == null) {
            return;
        }

        mVirtualDisplayAdapter.resizeVirtualDisplayLocked(appToken, width, height, densityDpi);
    }
}

ECode CDisplayManagerService::SetVirtualDisplaySurfaceInternal(
    /* [in] */ IBinder* appToken,
    /* [in] */ ISurface* surface)
{
    synchronized (mSyncRoot) {
        if (mVirtualDisplayAdapter == null) {
            return;
        }

        mVirtualDisplayAdapter.setVirtualDisplaySurfaceLocked(appToken, surface);
    }
}

ECode CDisplayManagerService::ReleaseVirtualDisplayInternal(
    /* [in] */ IBinder* appToken)
{
    synchronized (mSyncRoot) {
        if (mVirtualDisplayAdapter == null) {
            return;
        }

        DisplayDevice device =
                mVirtualDisplayAdapter.releaseVirtualDisplayLocked(appToken);
        if (device != null) {
            handleDisplayDeviceRemovedLocked(device);
        }
    }
}

ECode CDisplayManagerService::RegisterDefaultDisplayAdapter()
{
    if (DEBUG) Slogger::D(TAG, "RegisterDefaultDisplayAdapter mHeadless: %d", mHeadless);
    // Register default display adapter.
    AutoLock lock(mSyncRoot);

    AutoPtr<LocalDisplayAdapter> adapter = new LocalDisplayAdapter(
        &mSyncRoot, mContext, mHandler, mDisplayAdapterListener);
    RegisterDisplayAdapterLocked(adapter);
}

ECode CDisplayManagerService::RegisterAdditionalDisplayAdapters()
{
    AutoLock lock(mSyncRoot);
    Boolean result = ShouldRegisterNonEssentialDisplayAdaptersLocked();
    if (result) {
        RegisterOverlayDisplayAdapterLocked();
        RegisterWifiDisplayAdapterLocked();
    }
}

ECode CDisplayManagerService::RegisterOverlayDisplayAdapterLocked()
{
    AutoPtr<OverlayDisplayAdapter> adapter = new OverlayDisplayAdapter(
        &mSyncRoot, mContext, mHandler, mDisplayAdapterListener, mUiHandler);
    RegisterDisplayAdapterLocked(adapter);
}

ECode CDisplayManagerService::RegisterWifiDisplayAdapterLocked()
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

ECode CDisplayManagerService::RegisterWifiDisplayAdapterLocked()
{
    mVirtualDisplayAdapter = new VirtualDisplayAdapter(
            mSyncRoot, mContext, mHandler, mDisplayAdapterListener);
    registerDisplayAdapterLocked(mVirtualDisplayAdapter);
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
    synchronized (mSyncRoot) {
        handleDisplayDeviceAddedLocked(device);
    }
}

void CDisplayManagerService::HandleDisplayDeviceAddedLocked(
   /* [in] */ DisplayDevice* device)
{
    if (mDisplayDevices.contains(device)) {
        Slog.w(TAG, "Attempted to add already added display device: "
                + device.getDisplayDeviceInfoLocked());
        return;
    }

    Slog.i(TAG, "Display device added: " + device.getDisplayDeviceInfoLocked());

    mDisplayDevices.add(device);
    addLogicalDisplayLocked(device);
    Runnable work = updateDisplayStateLocked(device);
    if (work != null) {
        work.run();
    }
    scheduleTraversalLocked(false);
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
    synchronized (mSyncRoot) {
        handleDisplayDeviceRemovedLocked(device);
    }
}

void CDisplayManagerService::handleDisplayDeviceRemovedLocked(
    /* [in] */ DisplayDevice* device)
{
    // AutoLock lock(mSyncRoot);

    // List<AutoPtr<DisplayDevice> >::Iterator find = Find(
    //     mDisplayDevices.Begin(), mDisplayDevices.End(), AutoPtr<DisplayDevice>(device));
    // if (find == mDisplayDevices.End()) {
    //     Slogger::W(TAG, "Attempted to remove non-existent display device: %s",
    //         device->GetDisplayDeviceInfoLocked()->ToString().string());
    //     return;
    // }

    // mDisplayDevices.Erase(find);

    // Slogger::I(TAG, "Display device removed: %s",
    //     device->GetDisplayDeviceInfoLocked()->ToString().string());

    // UpdateLogicalDisplaysLocked();
    // ScheduleTraversalLocked(FALSE);

    if (!mDisplayDevices.remove(device)) {
        Slog.w(TAG, "Attempted to remove non-existent display device: "
                + device.getDisplayDeviceInfoLocked());
        return;
    }

    Slog.i(TAG, "Display device removed: " + device.getDisplayDeviceInfoLocked());

    updateLogicalDisplaysLocked();
    scheduleTraversalLocked(false);
}

void CDisplayManagerService::UpdateGlobalDisplayStateLocked(
    /* [in] */ IList* workQueue)
{
    final int count = mDisplayDevices.size();
    for (int i = 0; i < count; i++) {
        DisplayDevice device = mDisplayDevices.get(i);
        Runnable runnable = updateDisplayStateLocked(device);
        if (runnable != null) {
            workQueue.add(runnable);
        }
    }
}

AutoPtr<IRunnable> CDisplayManagerService::UpdateDisplayStateLocked(
    /* [in] */ DisplayDevice* device)
{
    // Blank or unblank the display immediately to match the state requested
    // by the display power controller (if known).
    DisplayDeviceInfo info = device.getDisplayDeviceInfoLocked();
    if ((info.flags & DisplayDeviceInfo.FLAG_NEVER_BLANK) == 0) {
        return device.requestDisplayStateLocked(mGlobalDisplayState);
    }
    return null;
}

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
    if (mInputManagerInternal != NULL) {
        Boolean result;
        mHandler->SendEmptyMessage(MSG_UPDATE_VIEWPORT, &result);
    }
}

void CDisplayManagerService::SetDisplayPropertiesInternal(
    /* [in] */ Int32 displayId,
    /* [in] */ Boolean hasContent,
    /* [in] */ Float requestedRefreshRate,
    /* [in] */ Boolean inTraversal)
{
    synchronized (mSyncRoot) {
        LogicalDisplay display = mLogicalDisplays.get(displayId);
        if (display == null) {
            return;
        }
        if (display.hasContentLocked() != hasContent) {
            if (DEBUG) {
                Slog.d(TAG, "Display " + displayId + " hasContent flag changed: "
                        + "hasContent=" + hasContent + ", inTraversal=" + inTraversal);
            }

            display.setHasContentLocked(hasContent);
            scheduleTraversalLocked(inTraversal);
        }
        if (display.getRequestedRefreshRateLocked() != requestedRefreshRate) {
            if (DEBUG) {
                Slog.d(TAG, "Display " + displayId + " has requested a new refresh rate: "
                        + requestedRefreshRate + "fps");
            }
            display.setRequestedRefreshRateLocked(requestedRefreshRate);
            scheduleTraversalLocked(inTraversal);
        }
    }
}

void CDisplayManagerService::ClearViewportsLocked()
{
    mDefaultViewport->mValid = FALSE;
    mExternalTouchViewport->mValid = FALSE;
}

void CDisplayManagerService::ConfigureDisplayInTransactionLocked(
    /* [in] */ DisplayDevice* device)
{
    // // Find the logical display that the display device is showing.
    // AutoPtr<LogicalDisplay> display = FindLogicalDisplayForDeviceLocked(device);
    // if (display != NULL && !display->HasContentLocked()) {
    //     display = NULL;
    // }
    // if (display == NULL) {
    //     HashMap<Int32, AutoPtr<LogicalDisplay> >::Iterator find =
    //         mLogicalDisplays.Find(IDisplay::DEFAULT_DISPLAY);
    //     if (find != mLogicalDisplays.End())
    //         display = find->mSecond;
    // }

    // // Apply the logical display configuration to the display device.
    // if (display == NULL) {
    //     // TODO: no logical display for the device, blank it
    //     Slogger::W(TAG, "Missing logical display to use for physical display device: %s"
    //         , device->GetDisplayDeviceInfoLocked()->ToString().string());
    //     return;
    // }
    // else {
    //     Boolean isBlanked = (mAllDisplayBlankStateFromPowerManager
    //         == DISPLAY_BLANK_STATE_BLANKED);
    //     display->ConfigureDisplayInTransactionLocked(device, isBlanked);
    // }

    // // Update the viewports if needed.
    // AutoPtr<DisplayDeviceInfo> info = device->GetDisplayDeviceInfoLocked();
    // if (!mDefaultViewport->mValid
    //     && (info->mFlags & DisplayDeviceInfo::FLAG_DEFAULT_DISPLAY) != 0) {
    //     SetViewportLocked(mDefaultViewport, display, device);
    // }
    // if (!mExternalTouchViewport->mValid
    //     && info->mTouch == DisplayDeviceInfo::TOUCH_EXTERNAL) {
    //     SetViewportLocked(mExternalTouchViewport, display, device);
    // }

    final DisplayDeviceInfo info = device.getDisplayDeviceInfoLocked();
    final boolean ownContent = (info.flags & DisplayDeviceInfo.FLAG_OWN_CONTENT_ONLY) != 0;

    // Find the logical display that the display device is showing.
    // Certain displays only ever show their own content.
    LogicalDisplay display = findLogicalDisplayForDeviceLocked(device);
    if (!ownContent) {
        if (display != null && !display.hasContentLocked()) {
            // If the display does not have any content of its own, then
            // automatically mirror the default logical display contents.
            display = null;
        }
        if (display == null) {
            display = mLogicalDisplays.get(Display.DEFAULT_DISPLAY);
        }
    }

    // Apply the logical display configuration to the display device.
    if (display == null) {
        // TODO: no logical display for the device, blank it
        Slog.w(TAG, "Missing logical display to use for physical display device: "
                + device.getDisplayDeviceInfoLocked());
        return;
    }
    display.configureDisplayInTransactionLocked(device, info.state == Display.STATE_OFF);

    // Update the viewports if needed.
    if (!mDefaultViewport.valid
            && (info.flags & DisplayDeviceInfo.FLAG_DEFAULT_DISPLAY) != 0) {
        setViewportLocked(mDefaultViewport, display, device);
    }
    if (!mExternalTouchViewport.valid
            && info.touch == DisplayDeviceInfo.TOUCH_EXTERNAL) {
        setViewportLocked(mExternalTouchViewport, display, device);
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

    if (!mPendingTraversal && mWindowManagerInternal != NULL) {
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

AutoPtr<IIMediaProjectionManager> CDisplayManagerService::GetProjectionService()
{
    if (mProjectionService == null) {
        IBinder b = ServiceManager.getService(Context.MEDIA_PROJECTION_SERVICE);
        mProjectionService = IMediaProjectionManager.Stub.asInterface(b);
    }
    return mProjectionService;
}

ECode CDisplayManagerService::DumpInternal(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* pw,
    /* [in] */ ArrayOf<String>* args)
{
    // pw.println("DISPLAY MANAGER (dumpsys display)");

    // synchronized (mSyncRoot) {
    //     pw.println("  mOnlyCode=" + mOnlyCore);
    //     pw.println("  mSafeMode=" + mSafeMode);
    //     pw.println("  mPendingTraversal=" + mPendingTraversal);
    //     pw.println("  mGlobalDisplayState=" + Display.stateToString(mGlobalDisplayState));
    //     pw.println("  mNextNonDefaultDisplayId=" + mNextNonDefaultDisplayId);
    //     pw.println("  mDefaultViewport=" + mDefaultViewport);
    //     pw.println("  mExternalTouchViewport=" + mExternalTouchViewport);
    //     pw.println("  mSingleDisplayDemoMode=" + mSingleDisplayDemoMode);
    //     pw.println("  mWifiDisplayScanRequestCount=" + mWifiDisplayScanRequestCount);

    //     IndentingPrintWriter ipw = new IndentingPrintWriter(pw, "    ");
    //     ipw.increaseIndent();

    //     pw.println();
    //     pw.println("Display Adapters: size=" + mDisplayAdapters.size());
    //     for (DisplayAdapter adapter : mDisplayAdapters) {
    //         pw.println("  " + adapter.getName());
    //         adapter.dumpLocked(ipw);
    //     }

    //     pw.println();
    //     pw.println("Display Devices: size=" + mDisplayDevices.size());
    //     for (DisplayDevice device : mDisplayDevices) {
    //         pw.println("  " + device.getDisplayDeviceInfoLocked());
    //         device.dumpLocked(ipw);
    //     }

    //     final int logicalDisplayCount = mLogicalDisplays.size();
    //     pw.println();
    //     pw.println("Logical Displays: size=" + logicalDisplayCount);
    //     for (int i = 0; i < logicalDisplayCount; i++) {
    //         int displayId = mLogicalDisplays.keyAt(i);
    //         LogicalDisplay display = mLogicalDisplays.valueAt(i);
    //         pw.println("  Display " + displayId + ":");
    //         display.dumpLocked(ipw);
    //     }

    //     final int callbackCount = mCallbacks.size();
    //     pw.println();
    //     pw.println("Callbacks: size=" + callbackCount);
    //     for (int i = 0; i < callbackCount; i++) {
    //         CallbackRecord callback = mCallbacks.valueAt(i);
    //         pw.println("  " + i + ": mPid=" + callback.mPid
    //                 + ", mWifiDisplayScanRequested=" + callback.mWifiDisplayScanRequested);
    //     }

    //     if (mDisplayPowerController != null) {
    //         mDisplayPowerController.dump(pw);
    //     }
    // }
    return NOERROR;
}

} // namespace Display
} // namespace Server
} // namepsace Droid
} // namespace Elastos
