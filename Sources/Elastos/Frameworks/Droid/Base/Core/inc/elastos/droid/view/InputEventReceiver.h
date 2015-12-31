
#ifndef __ELASTOS_DROID_VIEW_INPUTEVENTRECEIVER_H__
#define __ELASTOS_DROID_VIEW_INPUTEVENTRECEIVER_H__

#include "Elastos.Droid.View.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/os/NativeMessageQueue.h"
#include <elastos/utility/etl/HashMap.h>
#include <input/InputTransport.h>
#include <utils/Vector.h>

using Elastos::Droid::Os::MessageQueue;
using Elastos::Droid::Os::NativeMessageQueue;
using Elastos::Droid::Os::ILooper;
using Elastos::Utility::Etl::HashMap;
using android::Vector;

namespace Elastos {
namespace Droid {
namespace View {

/**
 * Provides a low-level mechanism for an application to receive input events.
 * @hide
 */
class InputEventReceiver
    : public Object
    , public IInputEventReceiver
{
private:
    class NativeInputEventReceiver : public android::LooperCallback
    {
    public:
        NativeInputEventReceiver(
            IWeakReference* inputEventReceiver,
            const android::sp<android::InputChannel>& inputChannel,
            MessageQueue* messageQueue);

        android::status_t initialize();
        void dispose();
        android::status_t finishInputEvent(uint32_t seq, bool handled);
        android::status_t consumeEvents(bool consumeBatches, nsecs_t frameTime, Boolean* outConsumedBatch);

    protected:
        virtual ~NativeInputEventReceiver();

    private:
        struct Finish
        {
            uint32_t seq;
            bool handled;
        };
        AutoPtr<IWeakReference> mInputEventReceiver;
        android::InputConsumer mInputConsumer;
        AutoPtr<MessageQueue> mMessageQueue;
        android::PreallocatedInputEventFactory mInputEventFactory;
        Boolean mBatchedInputEventPending;
        int mFdEvents;
        Vector<Finish> mFinishQueue;

        void setFdEvents(int events);

        const char* getInputChannelName() {
            return mInputConsumer.getChannel()->getName().string();
        }

        virtual int handleEvent(int receiveFd, int events, void* data);
    };

public:
    CAR_INTERFACE_DECL();

    InputEventReceiver();

    InputEventReceiver(
        /* [in] */ IInputChannel* inputChannel,
        /* [in] */ ILooper* looper);

    CARAPI constructor(
        /* [in] */ IInputChannel* inputChannel,
        /* [in] */ ILooper* looper);

    virtual ~InputEventReceiver();

    virtual CARAPI Dispose();

    virtual CARAPI OnInputEvent(
        /* [in] */ IInputEvent* event);

    virtual CARAPI OnBatchedInputEventPending();

    CARAPI FinishInputEvent(
        /* [in] */ IInputEvent* event,
        /* [in] */ Boolean handled);

    CARAPI ConsumeBatchedInputEvents(
        /* [in] */ Int64 frameTimeNanos,
        /* [out] */ Boolean* result);

private:
    CARAPI DispatchInputEvent(
        /* [in] */ Int32 seq,
        /* [in] */ IInputEvent* event);

    CARAPI DispatchBatchedInputEventPending();

    // public static interface Factory {
    //     public InputEventReceiver createInputEventReceiver(
    //             InputChannel inputChannel, Looper looper);
    // }

private:
    CARAPI NativeInit();

    CARAPI NativeDispose();

    CARAPI NativeFinishInputEvent(
        /* [in] */ Int32 seq,
        /* [in] */ Boolean handled);

    CARAPI NativeConsumeBatchedInputEvents(
        /* [in] */ Int64 frameTimeNanos,
        /* [out] */ Boolean* result);

private:
     static const char* TAG;

    //final CloseGuard mCloseGuard = CloseGuard.get();

    android::sp<NativeInputEventReceiver> mNativeReceiver;

    // We keep references to the input channel and message queue objects here so that
    // they are not GC'd while the native peer of the receiver is using them.
    AutoPtr<IInputChannel> mInputChannel;
    NativeMessageQueue* mMessageQueue;

    // Map from InputEvent sequence numbers to dispatcher sequence numbers.
    HashMap<Int32, Int32> mSeqMap;
};

} // namespace View
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_VIEW_INPUTEVENTRECEIVER_H__
