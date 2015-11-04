
#include "elastos/droid/app/UiAutomation.h"

namespace Elastos {
namespace Droid {
namespace App {

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
    if (looper == null) {
        throw new IllegalArgumentException("Looper cannot be null!");
    }
    if (connection == null) {
        throw new IllegalArgumentException("Connection cannot be null!");
    }
    mUiAutomationConnection = connection;
    mClient = new IAccessibilityServiceClientImpl(looper);
}

ECode UiAutomation::Connect()
{
    synchronized (mLock) {
        throwIfConnectedLocked();
        if (mIsConnecting) {
            return;
        }
        mIsConnecting = true;
    }

    try {
        // Calling out without a lock held.
        mUiAutomationConnection.connect(mClient);
    } catch (RemoteException re) {
        throw new RuntimeException("Error while connecting UiAutomation", re);
    }

    synchronized (mLock) {
        final Int64 startTimeMillis = SystemClock.uptimeMillis();
        try {
            while (true) {
                if (isConnectedLocked()) {
                    break;
                }
                final Int64 elapsedTimeMillis = SystemClock.uptimeMillis() - startTimeMillis;
                final Int64 remainingTimeMillis = CONNECT_TIMEOUT_MILLIS - elapsedTimeMillis;
                if (remainingTimeMillis <= 0) {
                    throw new RuntimeException("Error while connecting UiAutomation");
                }
                try {
                    mLock.wait(remainingTimeMillis);
                } catch (InterruptedException ie) {
                    /* ignore */
                }
            }
        } finally {
            mIsConnecting = false;
        }
    }
}

ECode UiAutomation::Disconnect()
{
    synchronized (mLock) {
        if (mIsConnecting) {
            throw new IllegalStateException(
                    "Cannot call disconnect() while connecting!");
        }
        throwIfNotConnectedLocked();
        mConnectionId = CONNECTION_ID_UNDEFINED;
    }
    try {
        // Calling out without a lock held.
        mUiAutomationConnection.disconnect();
    } catch (RemoteException re) {
        throw new RuntimeException("Error while disconnecting UiAutomation", re);
    }
}

ECode UiAutomation::GetConnectionId(
    /* [out] */ Int32* id)
{
    synchronized (mLock) {
        throwIfNotConnectedLocked();
        return mConnectionId;
    }
}

ECode UiAutomation::SetOnAccessibilityEventListener(
    /* [in] */ IOnAccessibilityEventListener* listener)
{
    synchronized (mLock) {
        mOnAccessibilityEventListener = listener;
    }
}

ECode UiAutomation::PerformGlobalAction(
    /* [in] */ Int32 action,
    /* [out] */ Boolean* action)
{
    final IAccessibilityServiceConnection connection;
    synchronized (mLock) {
        throwIfNotConnectedLocked();
        connection = AccessibilityInteractionClient.getInstance()
                .getConnection(mConnectionId);
    }
    // Calling out without a lock held.
    if (connection != null) {
        try {
            return connection.performGlobalAction(action);
        } catch (RemoteException re) {
            Log.w(LOG_TAG, "Error while calling performGlobalAction", re);
        }
    }
    return false;
}

ECode UiAutomation::FindFocus(
    /* [in] */ Int32 focus,
    /* [out] */ IAccessibilityNodeInfo** info)
{
    return AccessibilityInteractionClient.getInstance().findFocus(mConnectionId,
            AccessibilityNodeInfo.ANY_WINDOW_ID, AccessibilityNodeInfo.ROOT_NODE_ID, focus);
}

ECode UiAutomation::GetServiceInfo(
    /* [out] */ IAccessibilityServiceInfo** info)
{
    final IAccessibilityServiceConnection connection;
    synchronized (mLock) {
        throwIfNotConnectedLocked();
        connection = AccessibilityInteractionClient.getInstance()
                .getConnection(mConnectionId);
    }
    // Calling out without a lock held.
    if (connection != null) {
        try {
            return connection.getServiceInfo();
        } catch (RemoteException re) {
            Log.w(LOG_TAG, "Error while getting AccessibilityServiceInfo", re);
        }
    }
    return null;
}

ECode UiAutomation::SetServiceInfo(
    /* [in] */ IAccessibilityServiceInfo* info)
{
    final IAccessibilityServiceConnection connection;
    synchronized (mLock) {
        throwIfNotConnectedLocked();
        AccessibilityInteractionClient.getInstance().clearCache();
        connection = AccessibilityInteractionClient.getInstance()
                .getConnection(mConnectionId);
    }
    // Calling out without a lock held.
    if (connection != null) {
        try {
            connection.setServiceInfo(info);
        } catch (RemoteException re) {
            Log.w(LOG_TAG, "Error while setting AccessibilityServiceInfo", re);
        }
    }
}

ECode UiAutomation::GetWindows(
    /* [out] */ IList** windows) //List<AccessibilityWindowInfo>
{
    final Int32 connectionId;
    synchronized (mLock) {
        throwIfNotConnectedLocked();
        connectionId = mConnectionId;
    }
    // Calling out without a lock held.
    return AccessibilityInteractionClient.getInstance()
            .getWindows(connectionId);
}

ECode UiAutomation::GetRootInActiveWindow(
    /* [out] */ IAccessibilityNodeInfo** info)
{
    final Int32 connectionId;
    synchronized (mLock) {
        throwIfNotConnectedLocked();
        connectionId = mConnectionId;
    }
    // Calling out without a lock held.
    return AccessibilityInteractionClient.getInstance()
            .getRootInActiveWindow(connectionId);
}

ECode UiAutomation::InjectInputEvent(
    /* [in] */ IInputEvent* event,
    /* [in] */ Boolean sync,
    /* [out] */ Boolean* result)
{
    synchronized (mLock) {
        throwIfNotConnectedLocked();
    }
    try {
        if (DEBUG) {
            Log.i(LOG_TAG, "Injecting: " + event + " sync: " + sync);
        }
        // Calling out without a lock held.
        return mUiAutomationConnection.injectInputEvent(event, sync);
    } catch (RemoteException re) {
        Log.e(LOG_TAG, "Error while injecting input event!", re);
    }
    return false;
}

ECode UiAutomation::SetRotation(
    /* [in] */ Int32 rotation,
    /* [out] */ Boolean* result)
{
    synchronized (mLock) {
        throwIfNotConnectedLocked();
    }
    switch (rotation) {
        case ROTATION_FREEZE_0:
        case ROTATION_FREEZE_90:
        case ROTATION_FREEZE_180:
        case ROTATION_FREEZE_270:
        case ROTATION_UNFREEZE:
        case ROTATION_FREEZE_CURRENT: {
            try {
                // Calling out without a lock held.
                mUiAutomationConnection.setRotation(rotation);
                return true;
            } catch (RemoteException re) {
                Log.e(LOG_TAG, "Error while setting rotation!", re);
            }
        } return false;
        default: {
            throw new IllegalArgumentException("Invalid rotation.");
        }
    }
}

ECode UiAutomation::ExecuteAndWaitForEvent(
    /* [in] */ IRunnable* command,
    /* [in] */ IAccessibilityEventFilter* filter,
    /* [in] */ Int64 timeoutMillis,
    /* [out] */ IAccessibilityEvent** event)
{
    // Acquire the lock and prepare for receiving events.
    synchronized (mLock) {
        throwIfNotConnectedLocked();
        mEventQueue.clear();
        // Prepare to wait for an event.
        mWaitingForEventDelivery = true;
    }

    // Note: We have to release the lock since calling out with this lock held
    // can bite. We will correctly filter out events from other interactions,
    // so starting to collect events before running the action is just fine.

    // We will ignore events from previous interactions.
    final Int64 executionStartTimeMillis = SystemClock.uptimeMillis();
    // Execute the command *without* the lock being held.
    command.run();

    // Acquire the lock and wait for the event.
    synchronized (mLock) {
        try {
            // Wait for the event.
            final Int64 startTimeMillis = SystemClock.uptimeMillis();
            while (true) {
                // Drain the event queue
                while (!mEventQueue.isEmpty()) {
                    AccessibilityEvent event = mEventQueue.remove(0);
                    // Ignore events from previous interactions.
                    if (event.getEventTime() < executionStartTimeMillis) {
                        continue;
                    }
                    if (filter.accept(event)) {
                        return event;
                    }
                    event.recycle();
                }
                // Check if timed out and if not wait.
                final Int64 elapsedTimeMillis = SystemClock.uptimeMillis() - startTimeMillis;
                final Int64 remainingTimeMillis = timeoutMillis - elapsedTimeMillis;
                if (remainingTimeMillis <= 0) {
                    throw new TimeoutException("Expected event not received within: "
                            + timeoutMillis + " ms.");
                }
                try {
                    mLock.wait(remainingTimeMillis);
                } catch (InterruptedException ie) {
                    /* ignore */
                }
            }
        } finally {
            mWaitingForEventDelivery = false;
            mEventQueue.clear();
            mLock.notifyAll();
        }
    }
}

ECode UiAutomation::WaitForIdle(
    /* [in] */ Int64 idleTimeoutMillis,
    /* [in] */ Int64 globalTimeoutMillis)
{
    synchronized (mLock) {
        throwIfNotConnectedLocked();

        final Int64 startTimeMillis = SystemClock.uptimeMillis();
        if (mLastEventTimeMillis <= 0) {
            mLastEventTimeMillis = startTimeMillis;
        }

        while (true) {
            final Int64 currentTimeMillis = SystemClock.uptimeMillis();
            // Did we get idle state within the global timeout?
            final Int64 elapsedGlobalTimeMillis = currentTimeMillis - startTimeMillis;
            final Int64 remainingGlobalTimeMillis =
                    globalTimeoutMillis - elapsedGlobalTimeMillis;
            if (remainingGlobalTimeMillis <= 0) {
                throw new TimeoutException("No idle state with idle timeout: "
                        + idleTimeoutMillis + " within global timeout: "
                        + globalTimeoutMillis);
            }
            // Did we get an idle state within the idle timeout?
            final Int64 elapsedIdleTimeMillis = currentTimeMillis - mLastEventTimeMillis;
            final Int64 remainingIdleTimeMillis = idleTimeoutMillis - elapsedIdleTimeMillis;
            if (remainingIdleTimeMillis <= 0) {
                return;
            }
            try {
                 mLock.wait(remainingIdleTimeMillis);
            } catch (InterruptedException ie) {
                 /* ignore */
            }
        }
    }
}

ECode UiAutomation::TakeScreenshot(
    /* [out] */ IBitmap** bmp)
{
    synchronized (mLock) {
        throwIfNotConnectedLocked();
    }
    Display display = DisplayManagerGlobal.getInstance()
            .getRealDisplay(Display.DEFAULT_DISPLAY);
    Point displaySize = new Point();
    display.getRealSize(displaySize);
    final Int32 displayWidth = displaySize.x;
    final Int32 displayHeight = displaySize.y;

    final float screenshotWidth;
    final float screenshotHeight;

    final Int32 rotation = display.getRotation();
    switch (rotation) {
        case ROTATION_FREEZE_0: {
            screenshotWidth = displayWidth;
            screenshotHeight = displayHeight;
        } break;
        case ROTATION_FREEZE_90: {
            screenshotWidth = displayHeight;
            screenshotHeight = displayWidth;
        } break;
        case ROTATION_FREEZE_180: {
            screenshotWidth = displayWidth;
            screenshotHeight = displayHeight;
        } break;
        case ROTATION_FREEZE_270: {
            screenshotWidth = displayHeight;
            screenshotHeight = displayWidth;
        } break;
        default: {
            throw new IllegalArgumentException("Invalid rotation: "
                    + rotation);
        }
    }

    // Take the screenshot
    Bitmap screenShot = null;
    try {
        // Calling out without a lock held.
        screenShot = mUiAutomationConnection.takeScreenshot((Int32) screenshotWidth,
                (Int32) screenshotHeight);
        if (screenShot == null) {
            return null;
        }
    } catch (RemoteException re) {
        Log.e(LOG_TAG, "Error while taking screnshot!", re);
        return null;
    }

    // Rotate the screenshot to the current orientation
    if (rotation != ROTATION_FREEZE_0) {
        Bitmap unrotatedScreenShot = Bitmap.createBitmap(displayWidth, displayHeight,
                Bitmap.Config.ARGB_8888);
        Canvas canvas = new Canvas(unrotatedScreenShot);
        canvas.translate(unrotatedScreenShot.getWidth() / 2,
                unrotatedScreenShot.getHeight() / 2);
        canvas.rotate(getDegreesForRotation(rotation));
        canvas.translate(- screenshotWidth / 2, - screenshotHeight / 2);
        canvas.drawBitmap(screenShot, 0, 0, null);
        canvas.setBitmap(null);
        screenShot.recycle();
        screenShot = unrotatedScreenShot;
    }

    // Optimization
    screenShot.setHasAlpha(false);

    return screenShot;
}

ECode UiAutomation::SetRunAsMonkey(
    /* [in] */ Boolean enable)
{
    synchronized (mLock) {
        throwIfNotConnectedLocked();
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
    synchronized (mLock) {
        throwIfNotConnectedLocked();
    }
    try {
        if (DEBUG) {
            Log.i(LOG_TAG, "Clearing content frame stats for window: " + windowId);
        }
        // Calling out without a lock held.
        return mUiAutomationConnection.clearWindowContentFrameStats(windowId);
    } catch (RemoteException re) {
        Log.e(LOG_TAG, "Error clearing window content frame stats!", re);
    }
    return false;
}

ECode UiAutomation::GetWindowContentFrameStats(
    /* [in] */ Int32 windowId,
    /* [out] */ IWindowContentFrameStats** stats)
{
    synchronized (mLock) {
        throwIfNotConnectedLocked();
    }
    try {
        if (DEBUG) {
            Log.i(LOG_TAG, "Getting content frame stats for window: " + windowId);
        }
        // Calling out without a lock held.
        return mUiAutomationConnection.getWindowContentFrameStats(windowId);
    } catch (RemoteException re) {
        Log.e(LOG_TAG, "Error getting window content frame stats!", re);
    }
    return null;
}

ECode UiAutomation::ClearWindowAnimationFrameStats()
{
    synchronized (mLock) {
        throwIfNotConnectedLocked();
    }
    try {
        if (DEBUG) {
            Log.i(LOG_TAG, "Clearing window animation frame stats");
        }
        // Calling out without a lock held.
        mUiAutomationConnection.clearWindowAnimationFrameStats();
    } catch (RemoteException re) {
        Log.e(LOG_TAG, "Error clearing window animation frame stats!", re);
    }
}

ECode UiAutomation::GetWindowAnimationFrameStats(
    /* [out] */ IWindowAnimationFrameStats** stats)
{
    synchronized (mLock) {
        throwIfNotConnectedLocked();
    }
    try {
        if (DEBUG) {
            Log.i(LOG_TAG, "Getting window animation frame stats");
        }
        // Calling out without a lock held.
        return mUiAutomationConnection.getWindowAnimationFrameStats();
    } catch (RemoteException re) {
        Log.e(LOG_TAG, "Error getting window animation frame stats!", re);
    }
    return null;
}

ECode UiAutomation::ExecuteShellCommand(
    /* [in] */ const String& command,
    /* [out] */ IParcelFileDescriptor** pfd)
{
    synchronized (mLock) {
        throwIfNotConnectedLocked();
    }

    ParcelFileDescriptor source = null;
    ParcelFileDescriptor sink = null;

    try {
        ParcelFileDescriptor[] pipe = ParcelFileDescriptor.createPipe();
        source = pipe[0];
        sink = pipe[1];

        // Calling out without a lock held.
        mUiAutomationConnection.executeShellCommand(command, sink);
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

} // namespace App
} // namespace Droid
} // namespace Elastos
