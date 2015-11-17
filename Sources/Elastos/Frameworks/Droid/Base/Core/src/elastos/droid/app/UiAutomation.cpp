
#include "elastos/droid/app/UiAutomation.h"
#include "elastos/droid/os/SystemClock.h"
#include "elastos/droid/graphics/CPoint";
#include "elastos/droid/graphics/CBitmap";
#include "elastos/droid/view/accessibility/CAccessibilityEvent.h"
#include "elastos/droid/hardware/display/DisplayManagerGlobal.h"

// using Elastos::Droid::Accessibilityservice::AccessibilityServiceInfo;
// using Elastos::Droid::Accessibilityservice::IIAccessibilityServiceClient;
// using Elastos::Droid::Accessibilityservice::IIAccessibilityServiceConnection;

// using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::IParcelFileDescriptor;
using Elastos::Droid::Os::SystemClock;
// using Elastos::Droid::View::IInputEvent;
// using Elastos::Droid::View::ISurface;
// using Elastos::Droid::View::IWindowAnimationFrameStats;
// using Elastos::Droid::View::IWindowContentFrameStats;
// using Elastos::Droid::View::Accessibility::IAccessibilityInteractionClient;
// using Elastos::Droid::View::Accessibility::IAccessibilityNodeInfo;
// using Elastos::Droid::View::Accessibility::IAccessibilityWindowInfo;
// using Elastos::Droid::View::Accessibility::IIAccessibilityInteractionConnection;


using Elastos::Droid::Hardware::Display::DisplayManagerGlobal;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::CBitmap;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::CCanvas;
using Elastos::Droid::Graphics::IPoint;
using Elastos::Droid::Graphics::CPoint;
using Elastos::Droid::Graphics::BitmapConfig_ARGB_8888;
using Elastos::Droid::View::Accessibility::CAccessibilityEvent;
using Elastos::Droid::View::IDisplay;

using Elastos::IO::IFileDescriptor;
using Libcore::IO::IIoUtils;
using Libcore::IO::CIoUtils;

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

ECode UiAutomation::InnerAccessibilityServiceCallbacks::OnServiceConnected()
{
    /* do nothing */
    return NOERROR;
}

ECode UiAutomation::InnerAccessibilityServiceCallbacks::OnInterrupt()
{
    /* do nothing */
    return NOERROR;
}

ECode UiAutomation::InnerAccessibilityServiceCallbacks::OnGesture(
    /* [in] */ Int32 gestureId,
    /* [out] */ Boolean* result)
{
    /* do nothing */
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    return NOERROR;
}

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

UiAutomation::~UiAutomation()
{}

ECode UiAutomation::constructor(
    /* [in] */ ILooper* looper,
    /* [in] */ IIUiAutomationConnection* connection);
{
    if (looper == NULL) {
        Logger::E(LOG_TAG, "IllegalArgumentException: Looper cannot be NULL!");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (connection == NULL) {
        Logger::E(LOG_TAG, "IllegalArgumentException: Connection cannot be NULL!");
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
        Logger::E(LOG_TAG, "Error while connecting UiAutomation");
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
                    Logger::E(LOG_TAG, "Error while connecting UiAutomation");
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
            Logger::E(LOG_TAG, "Cannot call disconnect() while connecting!");
            return E_ILLEGAL_STATE_EXCEPTION:
        }
        FAIL_RETURN(ThrowIfNotConnectedLocked())
        mConnectionId = CONNECTION_ID_UNDEFINED;
    }
    // try {
        // Calling out without a lock held.
        ECode ec = mUiAutomationConnection->Disconnect();
    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        Logger::E(LOG_TAG, "Error while disconnecting UiAutomation");
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
        // if (ec == (ECode)E_REMOTE_EXCEPTION) {
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
        // if (ec == (ECode)E_REMOTE_EXCEPTION) {
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
        // if (ec == (ECode)E_REMOTE_EXCEPTION) {
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
    // if (ec == (ECode)E_REMOTE_EXCEPTION) {
    //     Logger::E(LOG_TAG, "Error while injecting input event!", re);
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
            // if (ec == (ECode)E_REMOTE_EXCEPTION) {
            //     Logger::E(LOG_TAG, "Error while setting rotation!", re);
            // }
            return NOERROR;
        }
        default: {
            Logger::E(LOG_TAG, "IllegalArgumentException: Invalid rotation.");
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
                    Logger::E(LOG_TAG, "Expected event not received within: %lld ms.", timeoutMillis);
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
                Logger::E(LOG_TAG, "No idle state with idle timeout: %lld within global timeout: %lld",
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
    /* [out] */ IBitmap** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;

    synchronized(mLock) {
        FAIL_RETURN(ThrowIfNotConnectedLocked())
    }
    AutoPtr<IDisplay> display;
    DisplayManagerGlobal::GetInstance()->GetRealDisplay(IDisplay::DEFAULT_DISPLAY, (IDisplay**)&display);
    AutoPtr<IPoint> displaySize;
    CPoint::New(((IPoint**)&displaySize);
    display->GetRealSize(displaySize);
    Int32 displayWidth, v;
    displaySize->Get(&x, &y);

    Float screenshotWidth;
    Float screenshotHeight;

    Int32 rotation;
    display->GetRotation(&rotation);
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
            Logger::E(LOG_TAG, "IllegalArgumentException: Invalid rotation: %d", rotation);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }

    // Take the screenshot
    AutoPtr<IBitmap> screenShot;
    try {
        // Calling out without a lock held.
        mUiAutomationConnection->TakeScreenshot((Int32) screenshotWidth,
                (Int32) screenshotHeight, &screenShot);
        if (screenShot == NULL) {
            return NOERROR;
        }
    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        Logger::E(LOG_TAG, "Error while taking screnshot!", re);
        return NOERROR;
    }

    // Rotate the screenshot to the current orientation
    if (rotation != IUiAutomation::ROTATION_FREEZE_0) {
        AutoPtr<IBitmap> unrotatedScreenShot;
        CBitmap::CreateBitmap(displayWidth, displayHeight,
            BitmapConfig_ARGB_8888, (IBitmap**)&unrotatedScreenShot);
        AutoPtr<ICanvas> canvas;
        CCanvas::New(unrotatedScreenShot, (ICanvas**)&canvas);
        Int32 w, h;
        unrotatedScreenShot->GetWidth(&w);
        unrotatedScreenShot->GetHeight(&h);
        canvas->Translate(w / 2, h / 2);
        canvas->Rotate(GetDegreesForRotation(rotation));
        canvas->Translate(- screenshotWidth / 2, - screenshotHeight / 2);
        canvas->DrawBitmap(screenShot, 0, 0, NULL);
        canvas->SetBitmap(NULL);
        screenShot->Recycle();
        screenShot = unrotatedScreenShot;
    }

    // Optimization
    screenShot->SetHasAlpha(FALSE);

    *result = screenShot;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode UiAutomation::SetRunAsMonkey(
    /* [in] */ Boolean enable)
{
    synchronized(mLock) {
        FAIL_RETURN(ThrowIfNotConnectedLocked())
    }
    // try {
    ECode ec = ActivityManagerNative::GetDefault()->SetUserIsMonkey(enable);
    // if (ec == (ECode)E_REMOTE_EXCEPTION) {
    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        Logger::E(LOG_TAG, "Error while setting run as monkey!");
        return NOERROR;
    }
    // }
    return ec;
}

ECode UiAutomation::ClearWindowContentFrameStats(
    /* [in] */ Int32 windowId,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    synchronized(mLock) {
        FAIL_RETURN(ThrowIfNotConnectedLocked())
    }
    // try {
        if (DEBUG) {
            Logger::I(LOG_TAG, "Clearing content frame stats for window: %d", windowId);
        }
        // Calling out without a lock held.
    ECode ec = mUiAutomationConnection->ClearWindowContentFrameStats(windowId, result);
    // if (ec == (ECode)E_REMOTE_EXCEPTION) {
    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        Logger::E(LOG_TAG, "Error clearing window content frame stats!", re);
        return NOERROR;
    }
    // }
    return ec;
}

ECode UiAutomation::GetWindowContentFrameStats(
    /* [in] */ Int32 windowId,
    /* [out] */ IWindowContentFrameStats** stats)
{
    VALIDATE_NOT_NULL(stats)
    *stats = NULL;

    synchronized(mLock) {
        FAIL_RETURN(ThrowIfNotConnectedLocked())
    }
    // try {
    if (DEBUG) {
        Logger::I(LOG_TAG, "Getting content frame stats for window: %d", windowId);
    }
        // Calling out without a lock held.
    ECode ec = mUiAutomationConnection->GetWindowContentFrameStats(windowId, stats);
    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        Logger::E(LOG_TAG, "Error getting window content frame stats!", re);
    }
    return NOERROR;
}

ECode UiAutomation::ClearWindowAnimationFrameStats()
{
    synchronized(mLock) {
        FAIL_RETURN(ThrowIfNotConnectedLocked())
    }
    //try {
    if (DEBUG) {
        Logger::I(LOG_TAG, "Clearing window animation frame stats");
    }
    // Calling out without a lock held.
    ECode ec = mUiAutomationConnection->ClearWindowAnimationFrameStats();
    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        Logger::E(LOG_TAG, "Error clearing window animation frame stats!", re);
        return NOERROR;
    }
    return ec;
}

ECode UiAutomation::GetWindowAnimationFrameStats(
    /* [out] */ IWindowAnimationFrameStats** stats)
{
    VALIDATE_NOT_NULL(stats)
    *stats = NULL;

    synchronized(mLock) {
        FAIL_RETURN(ThrowIfNotConnectedLocked())
    }
    try {
        if (DEBUG) {
            Logger::I(LOG_TAG, "Getting window animation frame stats");
        }
        // Calling out without a lock held.
    ECode ec = mUiAutomationConnection->GetWindowAnimationFrameStats(stats);
    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        Logger::E(LOG_TAG, "Error getting window animation frame stats!");
        return NOERROR;
    }
    return ec;
}

ECode UiAutomation::ExecuteShellCommand(
    /* [in] */ const String& command,
    /* [out] */ IParcelFileDescriptor** pfd)
{
    VALIDATE_NOT_NULL(pfd)
    *pfd = NULL;

    synchronized(mLock) {
        FAIL_RETURN(ThrowIfNotConnectedLocked())
    }

    AutoPtr<IParcelFileDescriptor> source, sink;

    // try {
    AutoPtr<ArrayOf<IParcelFileDescriptor*> > pipe;
    ParcelFileDescriptor::CreatePipe((ArrayOf<IParcelFileDescriptor*>**)&pipe);
    source = (*pipe)[0];
    sink = (*pipe)[1];

    // Calling out without a lock held.
    ECode ec = mUiAutomationConnection->ExecuteShellCommand(command, sink);
    if (ec == (ECode)E_IO_EXCEPTION) {
        Logger::E(LOG_TAG, "E_IO_EXCEPTION: Error executing shell command!");
        ec = NOERROR;
    }
    else if (ec == (ECode)E_REMOTE_EXCEPTION) {
        Logger::E(LOG_TAG, "E_REMOTE_EXCEPTION: Error executing shell command!");
        ec = NOERROR;
    }
    else {
        *pfd = source;
        REFCOUNT_ADD(*pfd)
    }
    // } finally {
    AutoPtr<IIoUtils> utils;
    CIoUtils::AcquireSingleton((IIoUtils**)&utils)
    utils->CloseQuietly(IFileDescriptor::Probe(sink));
    // }

    return ec;
}

Float UiAutomation::GetDegreesForRotation(
    /* [in] */ Int32 value)
{
    switch (value) {
        case ISurface::ROTATION_90: {
            return 360f - 90f;
        }
        case ISurface::ROTATION_180: {
            return 360f - 180f;
        }
        case ISurface::ROTATION_270: {
            return 360f - 270f;
        } default: {
            return 0;
        }
    }
    return 0;
}

Boolean UiAutomation::IsConnectedLocked()
{
    return mConnectionId != IUiAutomation::CONNECTION_ID_UNDEFINED;
}

ECode UiAutomation::ThrowIfConnectedLocked()
{
    if (mConnectionId != CONNECTION_ID_UNDEFINED) {
        LoggerE(LOG_TAG, "IllegalStateException : UiAutomation connected!");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

ECode UiAutomation::ThrowIfNotConnectedLocked()
{
    if (!IsConnectedLocked()) {
        LoggerE(LOG_TAG, "IllegalStateException : UiAutomation not connected!");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
