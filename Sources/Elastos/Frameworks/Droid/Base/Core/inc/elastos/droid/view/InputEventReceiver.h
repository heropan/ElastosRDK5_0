
#ifndef __ELASTOS_DROID_VIEW_INPUTEVENTRECEIVER_H__
#define __ELASTOS_DROID_VIEW_INPUTEVENTRECEIVER_H__

#include "ext/frameworkext.h"
#include <utils/Looper.h>
#include <androidfw/InputTransport.h>
#include "os/NativeMessageQueue.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;
using Elastos::Droid::Os::NativeMessageQueue;
using Elastos::Droid::Os::ILooper;

namespace Elastos {
namespace Droid {
namespace View {

/**
 * Provides a low-level mechanism for an application to receive input events.
 * @hide
 */
class InputEventReceiver :
    public ElRefBase,
    public IInputEventReceiver,
    public IWeakReferenceSource
{
private:
    class NativeInputEventReceiver : public android::LooperCallback
    {
    public:
        NativeInputEventReceiver(
            IWeakReference* inputEventReceiver,
            const android::sp<android::InputChannel>& inputChannel,
            const android::sp<android::Looper>& looper);

        android::status_t initialize();
        void dispose();
        android::status_t finishInputEvent(uint32_t seq, bool handled);
        android::status_t consumeEvents(bool consumeBatches, nsecs_t frameTime);

    protected:
        virtual ~NativeInputEventReceiver();

    private:
        AutoPtr<IWeakReference> mInputEventReceiver;
        android::InputConsumer mInputConsumer;
        android::sp<android::Looper> mLooper;
        android::PreallocatedInputEventFactory mInputEventFactory;
        bool mBatchedInputEventPending;

        const char* getInputChannelName() {
            return mInputConsumer.getChannel()->getName().string();
        }

        virtual int handleEvent(int receiveFd, int events, void* data);
    };

public:
    CAR_INTERFACE_DECL()

    CARAPI GetWeakReference(
        /* [out] */ IWeakReference** weakReference);

    InputEventReceiver(
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
        /* [in] */ Int64 frameTimeNanos);

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
    CARAPI_(void) NativeInit();

    CARAPI_(void) NativeDispose();

    CARAPI_(void) NativeFinishInputEvent(
        /* [in] */ Int32 seq,
        /* [in] */ Boolean handled);

    CARAPI_(void) NativeConsumeBatchedInputEvents(
        /* [in] */ Int64 frameTimeNanos);

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
