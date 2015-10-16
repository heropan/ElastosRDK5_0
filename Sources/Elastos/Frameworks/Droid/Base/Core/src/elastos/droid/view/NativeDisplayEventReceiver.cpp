
#include "elastos/droid/view/NativeDisplayEventReceiver.h"
#include "elastos/droid/view/DisplayEventReceiver.h"
#include <utils/Log.h>

#undef ALOGV
#define ALOGV(...) ((void)0)

namespace Elastos {
namespace Droid {
namespace View {

static const size_t EVENT_BUFFER_SIZE = 100;

NativeDisplayEventReceiver::NativeDisplayEventReceiver(
    DisplayEventReceiver* receiverObj,
    MessageQueue* messageQueue)
    : mReceiverObjGlobal(receiverObj)
    , mMessageQueue(messageQueue)
    , mWaitingForVsync(false)
{
    ALOGV("receiver %p ~ Initializing input event receiver.", this);
}

NativeDisplayEventReceiver::~NativeDisplayEventReceiver()
{
    mReceiverObjGlobal = NULL;
}

android::status_t NativeDisplayEventReceiver::initialize()
{
    android::status_t result = mReceiver.initCheck();
    if (result) {
        ALOGW("Failed to initialize display event receiver, status=%d", result);
        return result;
    }

    int rc = mMessageQueue->GetLooper()->addFd(mReceiver.getFd(), 0, ALOOPER_EVENT_INPUT,
            this, NULL);
    if (rc < 0) {
        return android::UNKNOWN_ERROR;
    }
    return android::OK;
}

void NativeDisplayEventReceiver::dispose()
{
    ALOGV("receiver %p ~ Disposing display event receiver.", this);

    if (!mReceiver.initCheck()) {
        mMessageQueue->GetLooper()->removeFd(mReceiver.getFd());
    }
}

android::status_t NativeDisplayEventReceiver::scheduleVsync()
{
    if (!mWaitingForVsync) {
        ALOGV("receiver %p ~ Scheduling vsync.", this);

        // Drain all pending events.
        nsecs_t vsyncTimestamp;
        int32_t vsyncDisplayId;
        uint32_t vsyncCount;
        readLastVsyncMessage(&vsyncTimestamp, &vsyncDisplayId, &vsyncCount);

        android::status_t status = mReceiver.requestNextVsync();
        if (status) {
            ALOGW("Failed to request next vsync, status=%d", status);
            return status;
        }

        mWaitingForVsync = true;
    }
    return android::OK;
}

int NativeDisplayEventReceiver::handleEvent(int receiveFd, int events, void* data)
{
    if (events & (ALOOPER_EVENT_ERROR | ALOOPER_EVENT_HANGUP)) {
        ALOGE("Display event receiver pipe was closed or an error occurred.  "
                "events=0x%x", events);
        return 0; // remove the callback
    }

    if (!(events & ALOOPER_EVENT_INPUT)) {
        ALOGW("Received spurious callback for unhandled poll event.  "
                "events=0x%x", events);
        return 1; // keep the callback
    }

    // Drain all pending events, keep the last vsync.
    nsecs_t vsyncTimestamp;
    int32_t vsyncDisplayId;
    uint32_t vsyncCount;
    if (!readLastVsyncMessage(&vsyncTimestamp, &vsyncDisplayId, &vsyncCount)) {
        ALOGV("receiver %p ~ Woke up but there was no vsync pulse!", this);
        return 1; // keep the callback, did not obtain a vsync pulse
    }

    ALOGV("receiver %p ~ Vsync pulse: timestamp=%lld, id=%d, count=%d",
            this, vsyncTimestamp, vsyncDisplayId, vsyncCount);
    mWaitingForVsync = false;

    dispatchVsync(vsyncTimestamp, vsyncDisplayId, vsyncCount);
    return 1; // keep the callback
}

bool NativeDisplayEventReceiver::readLastVsyncMessage(
        nsecs_t* outTimestamp, int32_t* outId, uint32_t* outCount)
{
    android::DisplayEventReceiver::Event buf[EVENT_BUFFER_SIZE];
    ssize_t n;
    while ((n = mReceiver.getEvents(buf, EVENT_BUFFER_SIZE)) > 0) {
        ALOGV("receiver %p ~ Read %d events.", this, int(n));
        while (n-- > 0) {
            const android::DisplayEventReceiver::Event& ev = buf[n];
            if (ev.header.type == android::DisplayEventReceiver::DISPLAY_EVENT_VSYNC) {
                *outTimestamp = ev.header.timestamp;
                *outId = ev.header.id;
                *outCount = ev.vsync.count;
                return true; // stop at last vsync in the buffer
            }

            if (ev.header.type == android::DisplayEventReceiver::DISPLAY_EVENT_HOTPLUG) {
                dispatchHotplug(ev.header.timestamp, ev.header.id, ev.hotplug.connected);
            }
        }
    }
    if (n < 0) {
        ALOGW("Failed to get events from display event receiver, status=%d", android::status_t(n));
    }
    return false;
}

void NativeDisplayEventReceiver::dispatchVsync(
    nsecs_t timestamp, int32_t id, uint32_t count)
{
    ALOGV("receiver %p ~ Invoking vsync handler.", this);

    mReceiverObjGlobal->DispatchVsync(timestamp, id, count);

    ALOGV("receiver %p ~ Returned from vsync handler.", this);
}

void NativeDisplayEventReceiver::dispatchHotplug(
    nsecs_t timestamp, int32_t id, bool connected)
{
    ALOGV("receiver %p ~ Invoking hotplug handler.", this);

    mReceiverObjGlobal->DispatchHotplug(timestamp, id, connected);

    ALOGV("receiver %p ~ Returned from hotplug handler.", this);
}

} // namespace View
} // namespace Droid
} // namespace Elastos
