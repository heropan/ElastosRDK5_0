
#include "view/DisplayEventReceiver.h"
#include "os/Looper.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Os::Looper;
using Elastos::Droid::Os::MessageQueue;
using Elastos::Droid::Os::NativeMessageQueue;

namespace Elastos {
namespace Droid {
namespace View {

const char* DisplayEventReceiver::TAG = "DisplayEventReceiver";

CAR_INTERFACE_IMPL(DisplayEventReceiver, Object, IDisplayEventReceiver)

DisplayEventReceiver::DisplayEventReceiver(
    /* [in] */ ILooper* looper)
{
    if (looper == NULL) {
        Logger::E(TAG, "looper must not be NULL");
        assert(0);
    }

    mMessageQueue = ((Looper*)looper)->GetQueue();
    NativeInit();

    //mCloseGuard.open("dispose");
}

DisplayEventReceiver::~DisplayEventReceiver()
{
    Dispose(TRUE);
}

void DisplayEventReceiver::NativeInit()
{
    Handle32 nativeQueue;
    mMessageQueue->GetNativeMessageQueue(&nativeQueue);
    AutoPtr<MessageQueue> messageQueue = (NativeMessageQueue*)nativeQueue;
    if (messageQueue == NULL) {
        Logger::E(TAG, "MessageQueue is not initialized.");
        assert(0) ;
    }

    mNativeReceiver = new NativeDisplayEventReceiver(this, messageQueue);
    android::status_t status = mNativeReceiver->initialize();
    if (status) {
        Logger::E(TAG, "Failed to initialize display event receiver.  status=%d", status);
        assert(0) ;
    }
}

ECode DisplayEventReceiver::Dispose()
{
    Dispose(FALSE);
    return NOERROR;
}

void DisplayEventReceiver::Dispose(
    /* [in] */ Boolean finalized)
{
    // if (mCloseGuard != NULL) {
    //     if (finalized) {
    //         mCloseGuard.warnIfOpen();
    //     }
    //     mCloseGuard.close();
    // }

    if (mNativeReceiver != NULL) {
        mNativeReceiver->dispose();
        mNativeReceiver = NULL;
    }
    mMessageQueue = NULL;
}

ECode DisplayEventReceiver::OnVsync(
    /* [in] */ Int64 timestampNanos,
    /* [in] */ Int32 builtInDisplayId,
    /* [in] */ Int32 frame)
{
    return NOERROR;
}

ECode DisplayEventReceiver::OnHotplug(
    /* [in] */ Int64 timestampNanos,
    /* [in] */ Int32 builtInDisplayId,
    /* [in] */ Boolean connected)
{
    return NOERROR;
}

ECode DisplayEventReceiver::ScheduleVsync()
{
    if (mNativeReceiver == NULL) {
        Logger::W(TAG, "Attempted to schedule a vertical sync pulse but the display event "
            "receiver has already been disposed.");
    }
    else {
        android::status_t status = mNativeReceiver->scheduleVsync();
        if (status) {
            Logger::E(TAG, "Failed to schedule next vertical sync pulse.  status=%d", status);
            return E_RUNTIME_EXCEPTION;
        }
    }
    return NOERROR;
}

void DisplayEventReceiver::DispatchVsync(
    /* [in] */ Int64 timestampNanos,
    /* [in] */ Int32 builtInDisplayId,
    /* [in] */ Int32 frame)
{
    OnVsync(timestampNanos, builtInDisplayId, frame);
}

void DisplayEventReceiver::DispatchHotplug(
    /* [in] */ Int64 timestampNanos,
    /* [in] */ Int32 builtInDisplayId,
    /* [in] */ Boolean connected)
{
    OnHotplug(timestampNanos, builtInDisplayId, connected);
}

} // namespace View
} // namespace Droid
} // namespace Elastos
