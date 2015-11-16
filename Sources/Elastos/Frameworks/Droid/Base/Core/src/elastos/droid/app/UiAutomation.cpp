
#include "elastos/droid/app/UiAutomation.h"
#include "elastos/droid/view/accessibility/CAccessibilityEvent.h"
#include "elastos/droid/hardware/display/DisplayManagerGlobal.h"

// using Elastos::Droid::Accessibilityservice::AccessibilityServiceInfo;
// using Elastos::Droid::Accessibilityservice::IIAccessibilityServiceClient;
// using Elastos::Droid::Accessibilityservice::IIAccessibilityServiceConnection;

// using Elastos::Droid::Os::ILooper;
// using Elastos::Droid::Os::IParcelFileDescriptor;
// using Elastos::Droid::Os::IRemoteException;
// using Elastos::Droid::Os::ISystemClock;
// using Elastos::Droid::View::IInputEvent;
// using Elastos::Droid::View::ISurface;
// using Elastos::Droid::View::IWindowAnimationFrameStats;
// using Elastos::Droid::View::IWindowContentFrameStats;
// using Elastos::Droid::View::Accessibility::IAccessibilityInteractionClient;
// using Elastos::Droid::View::Accessibility::IAccessibilityNodeInfo;
// using Elastos::Droid::View::Accessibility::IAccessibilityWindowInfo;
// using Elastos::Droid::View::Accessibility::IIAccessibilityInteractionConnection;
// import libcore.io.IoUtils;


using Elastos::Droid::Hardware::Display::DisplayManagerGlobal;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::IPoint;
using Elastos::Droid::View::Accessibility::CAccessibilityEvent;
using Elastos::Droid::View::IDisplay;

namespace Elastos {
namespace Droid {
namespace App {

//==================================================================
// UiAutomation::InnerAccessibilityServiceCallbacks
//==================================================================

CAR_INTERFACE_IMPL(UiAutomation::InnerAccessibilityServiceCallbacks, Object, IAccessibilityServiceCallbacks)

ECode UiAutomation::InnerAccessibilityServiceCallbacks::InnerAccessibilityServiceCallbacks(
    /* [in] */ UiAutomation* host)
    : mHost(host)
{}

ECode UiAutomation::InnerAccessibilityServiceCallbacks::OnSetConnectionId(
    /* [in] */ Int32 connectionId)
{
    synchronized (mHost->mLock) {
        mHost->mConnectionId = connectionId;
        mHost->mLock.NotifyAll();
    }
}

// @Override
ECode UiAutomation::InnerAccessibilityServiceCallbacks::OnServiceConnected()
{
    /* do nothing */
    return NOERROR;
}

// @Override
ECode UiAutomation::InnerAccessibilityServiceCallbacks::OnInterrupt()
{
    /* do nothing */
    return NOERROR;
}

// @Override
ECode UiAutomation::InnerAccessibilityServiceCallbacks::OnGesture(
    /* [in] */ Int32 gestureId,
    /* [out] */ Boolean* result)
{
    /* do nothing */
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    return NOERROR;
}

// @Override
ECode UiAutomation::InnerAccessibilityServiceCallbacks::OnAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    synchronized (mHost->mLock) {
        event->GetEventTime(&mHost->mLastEventTimeMillis);
        if (mHost->mWaitingForEventDelivery) {
            AutoPtr<IAccessibilityEvent> ae;
            CAccessibilityEvent::Obtain(event, (IAccessibilityEvent**)&ae);
            mHost->mEventQueue->Add(TO_IINTERFACE(ae));
        }
        mLock.NotifyAll();
    }
    // Calling out only without a lock held.
    AutoPtr<IOnAccessibilityEventListener> listener = mOnAccessibilityEventListener;
    if (listener != NULL) {
        AutoPtr<IAccessibilityEvent> ae;
        CAccessibilityEvent::Obtain(event, (IAccessibilityEvent**)&ae);
        listener->OnAccessibilityEvent(ae);
    }
    return NOERROR;
}

// @Override
ECode UiAutomation::InnerAccessibilityServiceCallbacks::OnKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    return NOERROR;
}

//==================================================================
// UiAutomation::IAccessibilityServiceClientImpl
//==================================================================

ECode UiAutomation::IAccessibilityServiceClientImpl::constructor(
    /* [in] */ ILooper* looper,
    /* [in] */ IUiAutomation* host)
{
    UiAutomation* ua = (UiAutomation*)host;
    AutoPtr<IAccessibilityServiceCallbacks> cbs = new InnerAccessibilityServiceCallbacks(ua);
    return IAccessibilityServiceClientWrapper::constructor(NULL, looper, cbs);
}

//==================================================================
// UiAutomation
//==================================================================

const String UiAutomation::LOG_TAG("UiAutomation");
const Boolean UiAutomation::DEBUG = FALSE;

const Int32 UiAutomation::CONNECTION_ID_UNDEFINED; = -1;
const Int64 UiAutomation::CONNECT_TIMEOUT_MILLIS = 5000;

CAR_INTERFACE_IMPL(UiAutomation, Object, IUiAutomation)

UiAutomation::UiAutomation()
    : mConnectionId(IUiAutomation::CONNECTION_ID_UNDEFINED)
    , mWaitingForEventDelivery(FALSE)
    , mLastEventTimeMillis(0)
    , mIsConnecting(FALSE)
{}

UiAutomation::~UiAutomation();

ECode UiAutomation::constructor(
    /* [in] */ ILooper* looper,
    /* [in] */ IIUiAutomationConnection* connection);
{
    if (looper == NULL) {
        Logger::E(TAG, "IllegalArgumentException: Looper cannot be NULL!");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (connection == NULL) {
        Logger::E(TAG, "IllegalArgumentException: Connection cannot be NULL!");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mUiAutomationConnection = connection;
    CAccessibilityServiceClientImpl::New(looper, this, (IIAccessibilityServiceClient**)&mClient);
}

ECode UiAutomation::Connect()
{
    synchronized(mLock) {
        throwIfConnectedLocked();
        if (mIsConnecting) {
            return NOERROR;
        }
        mIsConnecting = TRUE;
    }

    // try {
    // Calling out without a lock held.
    ECode ec = mUiAutomationConnection->Connect(mClient);
    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        Logger::E(TAG, "Error while connecting UiAutomation");
        return E_RUNTIME_EXCEPTION:
    }

    synchronized(mLock) {
        Int64 startTimeMillis = SystemClock::GetUptimeMillis();
        // try {
            while (TRUE) {
                if (IsConnectedLocked()) {
                    break;
                }
                Int64 elapsedTimeMillis = SystemClock::GetUptimeMillis() - startTimeMillis;
                Int64 remainingTimeMillis = CONNECT_TIMEOUT_MILLIS - elapsedTimeMillis;
                if (remainingTimeMillis <= 0) {
                    Logger::E(TAG, "Error while connecting UiAutomation");
                    return E_RUNTIME_EXCEPTION:
                }
                // try {
                    mLock.Wait(remainingTimeMillis);
                // } catch (InterruptedException ie) {
                //     /* ignore */
                // }
            }
        // } finally {
            mIsConnecting = FALSE;
        // }
    }
    return NOERROR;
}

ECode UiAutomation::Disconnect()
{
    synchronized(mLock) {
        if (mIsConnecting) {
            Logger::E(TAG, "Cannot call disconnect() while connecting!");
            return E_ILLEGAL_STATE_EXCEPTION:
        }
        FAIL_RETURN(ThrowIfNotConnectedLocked())
        mConnectionId = CONNECTION_ID_UNDEFINED;
    }
    // try {
        // Calling out without a lock held.
        ECode ec = mUiAutomationConnection->Disconnect();
    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        Logger::E(TAG, "Error while disconnecting UiAutomation");
        return E_RUNTIME_EXCEPTION:
    }
    return ec;
}

ECode UiAutomation::GetConnectionId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id)
    synchronized(mLock) {
        FAIL_RETURN(ThrowIfNotConnectedLocked())
        *id = mConnectionId;
    }
    return NOERROR;
}

ECode UiAutomation::SetOnAccessibilityEventListener(
    /* [in] */ IOnAccessibilityEventListener* listener)
{
    synchronized(mLock) {
        mOnAccessibilityEventListener = listener;
    }
    return NOERROR;
}

ECode UiAutomation::PerformGlobalAction(
    /* [in] */ Int32 action,
    /* [out] */ Boolean* action)
{
    VALIDATE_NOT_NULL(action)
    *action = FALSE;

    AutoPtr<IIAccessibilityServiceConnection> connection;
    synchronized(mLock) {
        FAIL_RETURN(ThrowIfNotConnectedLocked())
        CAccessibilityInteractionClient::GetInstance()->GetConnection(mConnectionId,
            (IIAccessibilityServiceConnection**)&connection);
    }
    // Calling out without a lock held.
    if (connection != NULL) {
        // try {
            return connection->PerformGlobalAction(action, action);
        // } catch (RemoteException re) {
        //     Log.w(LOG_TAG, "Error while calling performGlobalAction", re);
        // }
    }
    return NOERROR;
}

ECode UiAutomation::FindFocus(
    /* [in] */ Int32 focus,
    /* [out] */ IAccessibilityNodeInfo** info)
{
    VALIDATE_NOT_NULL(info)
    return CAccessibilityInteractionClient::GetInstance()->FindFocus(
        mConnectionId,
        IAccessibilityNodeInfo::ANY_WINDOW_ID, IAccessibilityNodeInfo::ROOT_NODE_ID,
        focus, info);
}

ECode UiAutomation::GetServiceInfo(
    /* [out] */ IAccessibilityServiceInfo** info)
{
    VALIDATE_NOT_NULL(info)
    *info = NULL;

    AutoPtr<IIAccessibilityServiceConnection> connection;
    synchronized(mLock) {
        FAIL_RETURN(ThrowIfNotConnectedLocked())
        CAccessibilityInteractionClient::GetInstance()->GetConnection(mConnectionId,
            (IIAccessibilityServiceConnection**)&connection);
    }
    // Calling out without a lock held.
    if (connection != NULL) {
        // try {
        return connection->GetServiceInfo(info);
        // } catch (RemoteException re) {
        //     Log.w(LOG_TAG, "Error while getting AccessibilityServiceInfo", re);
        // }
    }
    return NOERROR;
}

ECode UiAutomation::SetServiceInfo(
    /* [in] */ IAccessibilityServiceInfo* info)
{
    VALIDATE_NOT_NULL(info)
    *info = NULL;

    AutoPtr<IIAccessibilityServiceConnection> connection;
    synchronized(mLock) {
        FAIL_RETURN(ThrowIfNotConnectedLocked())
        CAccessibilityInteractionClient::GetInstance()->ClearCache();
        CAccessibilityInteractionClient::GetInstance()->GetConnection(mConnectionId,
            (IIAccessibilityServiceConnection**)&connection);
    }
    // Calling out without a lock held.
    if (connection != NULL) {
        // try {
            connection->SetServiceInfo(info);
        // } catch (RemoteException re) {
        //     Log.w(LOG_TAG, "Error while setting AccessibilityServiceInfo", re);
        // }
    }
    return NOERROR;
}

ECode UiAutomation::GetWindows(
    /* [out] */ IList** windows) //List<AccessibilityWindowInfo>
{
    VALIDATE_NOT_NULL(windows)
    *windows = NULL;

    Int32 connectionId;
    synchronized(mLock) {
        FAIL_RETURN(ThrowIfNotConnectedLocked())
        connectionId = mConnectionId;
    }
    // Calling out without a lock held.
    return CAccessibilityInteractionClient::GetInstance()->GetWindows(connectionId, windows);
}

ECode UiAutomation::GetRootInActiveWindow(
    /* [out] */ IAccessibilityNodeInfo** info)
{
    VALIDATE_NOT_NULL(info)
    *info = NULL;

    Int32 connectionId;
    synchronized(mLock) {
        FAIL_RETURN(ThrowIfNotConnectedLocked())
        connectionId = mConnectionId;
    }
    // Calling out without a lock held.
    return CAccessibilityInteractionClient::GetInstance()->GetRootInActiveWindow(connectionId, info);
}

ECode UiAutomation::InjectInputEvent(
    /* [in] */ IInputEvent* event,
    /* [in] */ Boolean sync,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    result = FALSE;

    synchronized(mLock) {
        FAIL_RETURN(ThrowIfNotConnectedLocked())
    }
    // try {
        if (DEBUG) {
            Logger::I(LOG_TAG, "Injecting: %s sync: %d", Object::ToString(event).string(), sync);
        }
        // Calling out without a lock held.
        return mUiAutomationConnection->injectInputEvent(event, sync);
    // } catch (RemoteException re) {
    //     Log.e(LOG_TAG, "Error while injecting input event!", re);
    // }
}

ECode UiAutomation::SetRotation(
    /* [in] */ Int32 rotation,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    synchronized(mLock) {
        FAIL_RETURN(ThrowIfNotConnectedLocked())
    }
    switch (rotation) {
        case IUiAutomation::ROTATION_FREEZE_0:
        case IUiAutomation::ROTATION_FREEZE_90:
        case IUiAutomation::ROTATION_FREEZE_180:
        case IUiAutomation::ROTATION_FREEZE_270:
        case IUiAutomation::ROTATION_UNFREEZE:
        case IUiAutomation::ROTATION_FREEZE_CURRENT: {
            // try {
                // Calling out without a lock held.
                mUiAutomationConnection->SetRotation(rotation, result);
                *result = TRUE;
            // } catch (RemoteException re) {
            //     Log.e(LOG_TAG, "Error while setting rotation!", re);
            // }
            return NOERROR;
        }
        default: {
            Logger::E(TAG, "IllegalArgumentException: Invalid rotation.");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode UiAutomation::ExecuteAndWaitForEvent(
    /* [in] */ IRunnable* command,
    /* [in] */ IAccessibilityEventFilter* filter,
    /* [in] */ Int64 timeoutMillis,
    /* [out] */ IAccessibilityEvent** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;

    // Acquire the lock and prepare for receiving events.
    synchronized(mLock) {
        FAIL_RETURN(ThrowIfNotConnectedLocked())
        mEventQueue->Clear();
        // Prepare to wait for an event.
        mWaitingForEventDelivery = TRUE;
    }

    // Note: We have to release the lock since calling out with this lock held
    // can bite. We will correctly filter out events from other interactions,
    // so starting to collect events before running the action is just fine.

    // We will ignore events from previous interactions.
    Int64 executionStartTimeMillis = SystemClock::GetUptimeMillis();
    // Execute the command *without* the lock being held.
    command.run();

    // Acquire the lock and wait for the event.
    synchronized(mLock) {
        // try {
            // Wait for the event.
            Int64 startTimeMillis = SystemClock::GetUptimeMillis();
            Int64 eventTime;
            Boolean empty, accept;
            while (TRUE) {
                // Drain the event queue
                while (mEventQueue->IsEmpty(&empty), !empty) {
                    AutoPtr<IInterface> obj;
                    mEventQueue->Remove(0, (IInterface**)&obj);
                    IAccessibilityEvent* event = IAccessibilityEvent::Probe(obj;
                    // Ignore events from previous interactions.
                    event->GetEventTime(&eventTime)
                    if (eventTime < executionStartTimeMillis) {
                        continue;
                    }
                    if (filter->Accept(event, &accept), accept) {
                        *result = event;
                        REFCOUNT_ADD(*result)
                        return NOERROR;
                    }
                    event->Recycle();
                }
                // Check if timed out and if not wait.
                Int64 elapsedTimeMillis = SystemClock::GetUptimeMillis() - startTimeMillis;
                Int64 remainingTimeMillis = timeoutMillis - elapsedTimeMillis;
                if (remainingTimeMillis <= 0) {
                    Logger::E(TAG, "Expected event not received within: %lld ms.", timeoutMillis);
                    return E_TIMEOUT_EXCEPTION;
                }
                // try {
                    mLock.Wait(remainingTimeMillis);
                // } catch (InterruptedException ie) {
                //     /* ignore */
                // }
            }
        // } finally {
            mWaitingForEventDelivery = FALSE;
            mEventQueue->Clear();
            mLock.NotifyAll();
        // }
    }
    return NOERROR;
}

ECode UiAutomation::WaitForIdle(
    /* [in] */ Int64 idleTimeoutMillis,
    /* [in] */ Int64 globalTimeoutMillis)
{
    synchronized(mLock) {
        FAIL_RETURN(ThrowIfNotConnectedLocked())

        Int64 startTimeMillis = SystemClock::GetUptimeMillis();
        if (mLastEventTimeMillis <= 0) {
            mLastEventTimeMillis = startTimeMillis;
        }

        while (TRUE) {
            Int64 currentTimeMillis = SystemClock::GetUptimeMillis();
            // Did we get idle state within the global timeout?
            Int64 elapsedGlobalTimeMillis = currentTimeMillis - startTimeMillis;
            Int64 remainingGlobalTimeMillis =
                    globalTimeoutMillis - elapsedGlobalTimeMillis;
            if (remainingGlobalTimeMillis <= 0) {
                Logger::E(TAG, "No idle state with idle timeout: %lld within global timeout: %lld",
                    idleTimeoutMillis, globalTimeoutMillis);
                    return E_TIMEOUT_EXCEPTION;
            }
            // Did we get an idle state within the idle timeout?
            Int64 elapsedIdleTimeMillis = currentTimeMillis - mLastEventTimeMillis;
            Int64 remainingIdleTimeMillis = idleTimeoutMillis - elapsedIdleTimeMillis;
            if (remainingIdleTimeMillis <= 0) {
                return NOERROR;
            }
            // try {
                 mLock.Wait(remainingIdleTimeMillis);
            // } catch (InterruptedException ie) {
            //      /* ignore */
            // }
        }
    }
    return NOERROR;
}

ECode UiAutomation::TakeScreenshot(
    /* [out] */ IBitmap** bmp)
{
    VALIDATE_NOT_NULL(bmp)
    synchronized(mLock) {
        FAIL_RETURN(ThrowIfNotConnectedLocked())
    }
    AutoPtr<IDisplay> display;
    DisplayManagerGlobal::GetInstance()->GetRealDisplay(IDisplay::DEFAULT_DISPLAY, (IDisplay**)&display);
    Point displaySize = new Point();
    display.getRealSize(displaySize);
    Int32 displayWidth = displaySize.x;
    Int32 displayHeight = displaySize.y;

    float screenshotWidth;
    float screenshotHeight;

    Int32 rotation = display.getRotation();
    switch (rotation) {
        case IUiAutomation::ROTATION_FREEZE_0: {
            screenshotWidth = displayWidth;
            screenshotHeight = displayHeight;
        } break;
        case IUiAutomation::ROTATION_FREEZE_90: {
            screenshotWidth = displayHeight;
            screenshotHeight = displayWidth;
        } break;
        case IUiAutomation::ROTATION_FREEZE_180: {
            screenshotWidth = displayWidth;
            screenshotHeight = displayHeight;
        } break;
        case IUiAutomation::ROTATION_FREEZE_270: {
            screenshotWidth = displayHeight;
            screenshotHeight = displayWidth;
        } break;
        default: {
            Logger::E(TAG, "IllegalArgumentException: Invalid rotation: "
                    + rotation);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }

    // Take the screenshot
    Bitmap screenShot = NULL;
    try {
        // Calling out without a lock held.
        screenShot = mUiAutomationConnection->takeScreenshot((Int32) screenshotWidth,
                (Int32) screenshotHeight);
        if (screenShot == NULL) {
            return NULL;
        }
    } catch (RemoteException re) {
        Log.e(LOG_TAG, "Error while taking screnshot!", re);
        return NULL;
    }

    // Rotate the screenshot to the current orientation
    if (rotation != IUiAutomation::ROTATION_FREEZE_0) {
        Bitmap unrotatedScreenShot = Bitmap.createBitmap(displayWidth, displayHeight,
                Bitmap.Config.ARGB_8888);
        Canvas canvas = new Canvas(unrotatedScreenShot);
        canvas.translate(unrotatedScreenShot.getWidth() / 2,
                unrotatedScreenShot.getHeight() / 2);
        canvas.rotate(getDegreesForRotation(rotation));
        canvas.translate(- screenshotWidth / 2, - screenshotHeight / 2);
        canvas.drawBitmap(screenShot, 0, 0, NULL);
        canvas.setBitmap(NULL);
        screenShot.recycle();
        screenShot = unrotatedScreenShot;
    }

    // Optimization
    screenShot.setHasAlpha(FALSE);

    return screenShot;
}

ECode UiAutomation::SetRunAsMonkey(
    /* [in] */ Boolean enable)
{
    synchronized(mLock) {
        FAIL_RETURN(ThrowIfNotConnectedLocked())
    }
    try {
        ActivityManagerNative.getDefault().setUserIsMonkey(enable);
    } catch (RemoteException re) {
        Log.e(LOG_TAG, "Error while setting run as monkey!", re);
    }
}

ECode UiAutomation::ClearWindowContentFrameStats(
    /* [in] */ Int32 windowId,
    /* [out] */ Boolean* result)
{
    synchronized(mLock) {
        FAIL_RETURN(ThrowIfNotConnectedLocked())
    }
    try {
        if (DEBUG) {
            Log.i(LOG_TAG, "Clearing content frame stats for window: " + windowId);
        }
        // Calling out without a lock held.
        return mUiAutomationConnection->clearWindowContentFrameStats(windowId);
    } catch (RemoteException re) {
        Log.e(LOG_TAG, "Error clearing window content frame stats!", re);
    }
    return FALSE;
}

ECode UiAutomation::GetWindowContentFrameStats(
    /* [in] */ Int32 windowId,
    /* [out] */ IWindowContentFrameStats** stats)
{
    synchronized(mLock) {
        FAIL_RETURN(ThrowIfNotConnectedLocked())
    }
    try {
        if (DEBUG) {
            Log.i(LOG_TAG, "Getting content frame stats for window: " + windowId);
        }
        // Calling out without a lock held.
        return mUiAutomationConnection->getWindowContentFrameStats(windowId);
    } catch (RemoteException re) {
        Log.e(LOG_TAG, "Error getting window content frame stats!", re);
    }
    return NULL;
}

ECode UiAutomation::ClearWindowAnimationFrameStats()
{
    synchronized(mLock) {
        FAIL_RETURN(ThrowIfNotConnectedLocked())
    }
    try {
        if (DEBUG) {
            Log.i(LOG_TAG, "Clearing window animation frame stats");
        }
        // Calling out without a lock held.
        mUiAutomationConnection->clearWindowAnimationFrameStats();
    } catch (RemoteException re) {
        Log.e(LOG_TAG, "Error clearing window animation frame stats!", re);
    }
}

ECode UiAutomation::GetWindowAnimationFrameStats(
    /* [out] */ IWindowAnimationFrameStats** stats)
{
    synchronized(mLock) {
        FAIL_RETURN(ThrowIfNotConnectedLocked())
    }
    try {
        if (DEBUG) {
            Log.i(LOG_TAG, "Getting window animation frame stats");
        }
        // Calling out without a lock held.
        return mUiAutomationConnection->getWindowAnimationFrameStats();
    } catch (RemoteException re) {
        Log.e(LOG_TAG, "Error getting window animation frame stats!", re);
    }
    return NULL;
}

ECode UiAutomation::ExecuteShellCommand(
    /* [in] */ const String& command,
    /* [out] */ IParcelFileDescriptor** pfd)
{
    synchronized(mLock) {
        FAIL_RETURN(ThrowIfNotConnectedLocked())
    }

    ParcelFileDescriptor source = NULL;
    ParcelFileDescriptor sink = NULL;

    try {
        ParcelFileDescriptor[] pipe = ParcelFileDescriptor.createPipe();
        source = pipe[0];
        sink = pipe[1];

        // Calling out without a lock held.
        mUiAutomationConnection->executeShellCommand(command, sink);
    } catch (IOException ioe) {
        Log.e(LOG_TAG, "Error executing shell command!", ioe);
    } catch (RemoteException re) {
        Log.e(LOG_TAG, "Error executing shell command!", re);
    } finally {
        IoUtils.closeQuietly(sink);
    }

    return source;
}

Float UiAutomation::GetDegreesForRotation(
    /* [in] */ Int32 value)
{
    switch (value) {
        case Surface.ROTATION_90: {
            return 360f - 90f;
        }
        case Surface.ROTATION_180: {
            return 360f - 180f;
        }
        case Surface.ROTATION_270: {
            return 360f - 270f;
        } default: {
            return 0;
        }
    }
}

Boolean UiAutomation::IsConnectedLocked()
{
    return mConnectionId != IUiAutomation::CONNECTION_ID_UNDEFINED;
}

CARAPI ThrowIfConnectedLocked()
{
    if (mConnectionId != CONNECTION_ID_UNDEFINED) {
        throw new IllegalStateException("UiAutomation not connected!");
    }
}

CARAPI ThrowIfNotConnectedLocked()
{
    if (!IsConnectedLocked()) {
        throw new IllegalStateException("UiAutomation not connected!");
    }
}

} // namespace App
} // namespace Droid
} // namespace Elastos
