
#ifndef __ELASTOS_DROID_VIEW_NATIVEDISPLAYEVENTRECEIVER_H__
#define __ELASTOS_DROID_VIEW_NATIVEDISPLAYEVENTRECEIVER_H__

#ifdef DROID_CORE
#include "Elastos.Droid.Core_server.h"
#else
#include "Elastos.Droid.Core.h"
#endif
#include "os/NativeMessageQueue.h"
#include <utils/Looper.h>
#include <gui/DisplayEventReceiver.h>

using Elastos::Droid::Os::MessageQueue;

namespace Elastos {
namespace Droid {
namespace View {

class DisplayEventReceiver;

class NativeDisplayEventReceiver : public android::LooperCallback
{
public:
    NativeDisplayEventReceiver(
        DisplayEventReceiver* receiverObj,
        MessageQueue* messageQueue);

    android::status_t initialize();
    void dispose();
    android::status_t scheduleVsync();

protected:
    virtual ~NativeDisplayEventReceiver();

private:
    AutoPtr<DisplayEventReceiver> mReceiverObjGlobal;
    AutoPtr<MessageQueue> mMessageQueue;
    android::DisplayEventReceiver mReceiver;
    bool mWaitingForVsync;

    virtual int handleEvent(int receiveFd, int events, void* data);
    bool readLastVsyncMessage(nsecs_t* outTimestamp, int32_t* id, uint32_t* outCount);
    void dispatchVsync(nsecs_t timestamp, int32_t id, uint32_t count);
    void dispatchHotplug(nsecs_t timestamp, int32_t id, bool connected);
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_VIEW_NATIVEDISPLAYEVENTRECEIVER_H__
