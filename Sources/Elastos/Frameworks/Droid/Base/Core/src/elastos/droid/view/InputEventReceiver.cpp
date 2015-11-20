
#include "elastos/droid/view/InputEventReceiver.h"
#include "elastos/droid/view/InputChannel.h"
#include "elastos/droid/view/NativeInputChannel.h"
#include "elastos/droid/view/ScopedLocalRef.h"
#ifdef DROID_CORE
// #include "elastos/droid/view/CKeyEvent.h"
// #include "elastos/droid/view/CMotionEvent.h"
#endif
#include <elastos/utility/logging/Logger.h>
#include <android/looper.h>
#include <input/Input.h>

using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::IMessageQueue;
using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace View {

const char* InputEventReceiver::TAG = "InputEventReceiver";

#define DEBUG_DISPATCH_CYCLE 0

static AutoPtr<IKeyEvent> CreateKeyEventFromNative(
    /* [in] */ const android::KeyEvent* event)
{
    AutoPtr<IKeyEvent> eventObj;
    ECode ec;
    // ECode ec = CKeyEvent::New(nanoseconds_to_milliseconds(event->getDownTime()),
    //         nanoseconds_to_milliseconds(event->getEventTime()),
    //         event->getAction(),
    //         event->getKeyCode(),
    //         event->getRepeatCount(),
    //         event->getMetaState(),
    //         event->getDeviceId(),
    //         event->getScanCode(),
    //         event->getFlags(),
    //         event->getSource(),
    //         (IKeyEvent**)&eventObj);
    // zhangjingcheng wait
    if (FAILED(ec)) {
//        ALOGE("An exception occurred while obtaining a key event.");
        return NULL;
    }
    return eventObj;
}

static AutoPtr<IMotionEvent> CreateMotionEventFromNative(
    /* [in] */ const android::MotionEvent* event)
{
    AutoPtr<IMotionEvent> eventObj;
    // if (FAILED(CMotionEvent::New((IMotionEvent**)&eventObj))) {
    //     return NULL;
    // }
    // zhangjingcheng wait
    Handle64 native;
    eventObj->GetNative(&native);
    android::MotionEvent* destEvent = (android::MotionEvent*)native;
    if (!destEvent) {
        destEvent = new android::MotionEvent();
        eventObj->SetNative((Handle64)destEvent);
    }

    destEvent->copyFrom(event, true);
    return eventObj;
}

CAR_INTERFACE_IMPL_2(InputEventReceiver, Object, IInputEventReceiver, IWeakReferenceSource);

InputEventReceiver::NativeInputEventReceiver::NativeInputEventReceiver(
    /* [in] */ IWeakReference* inputEventReceiver,
    /* [in] */ const android::sp<android::InputChannel>& inputChannel,
    /* [in] */ MessageQueue* messageQueue)
    : mInputEventReceiver(inputEventReceiver)
    , mInputConsumer(inputChannel)
    , mMessageQueue(messageQueue)
    , mBatchedInputEventPending(FALSE)
    , mFdEvents(0)
{
#if DEBUG_DISPATCH_CYCLE
    ALOGD("channel '%s' ~ Initializing input event receiver.", getInputChannelName());
#endif
}

InputEventReceiver::NativeInputEventReceiver::~NativeInputEventReceiver()
{
}

android::status_t InputEventReceiver::NativeInputEventReceiver::initialize()
{
    setFdEvents(ALOOPER_EVENT_INPUT);
    return android::OK;
}

void InputEventReceiver::NativeInputEventReceiver::dispose()
{
#if DEBUG_DISPATCH_CYCLE
    ALOGD("channel '%s' ~ Disposing input event receiver.", getInputChannelName());
#endif

    setFdEvents(0);
}

android::status_t InputEventReceiver::NativeInputEventReceiver::finishInputEvent(uint32_t seq, bool handled)
{
#if DEBUG_DISPATCH_CYCLE
    ALOGD("channel '%s' ~ Finished input event.", getInputChannelName());
#endif

    android::status_t status = mInputConsumer.sendFinishedSignal(seq, handled);
    if (status) {
        if (status == android::WOULD_BLOCK) {
#if DEBUG_DISPATCH_CYCLE
            ALOGD("channel '%s' ~ Could not send finished signal immediately.  "
                    "Enqueued for later.", getInputChannelName());
#endif
            Finish finish;
            finish.seq = seq;
            finish.handled = handled;
            mFinishQueue.add(finish);
            if (mFinishQueue.size() == 1) {
                setFdEvents(ALOOPER_EVENT_INPUT | ALOOPER_EVENT_OUTPUT);
            }
            return android::OK;
        }
        ALOGW("Failed to send finished signal on channel '%s'.  status=%d",
                getInputChannelName(), status);
    }
    return status;
}

void InputEventReceiver::NativeInputEventReceiver::setFdEvents(int events)
{
    if (mFdEvents != events) {
        mFdEvents = events;
        int fd = mInputConsumer.getChannel()->getFd();
        if (events) {
            mMessageQueue->GetLooper()->addFd(fd, 0, events, this, NULL);
        }
        else {
            mMessageQueue->GetLooper()->removeFd(fd);
        }
    }
}

int InputEventReceiver::NativeInputEventReceiver::handleEvent(int receiveFd, int events, void* data)
{
    if (events & (ALOOPER_EVENT_ERROR | ALOOPER_EVENT_HANGUP)) {
#if DEBUG_DISPATCH_CYCLE
        // This error typically occurs when the publisher has closed the input channel
        // as part of removing a window or finishing an IME session, in which case
        // the consumer will soon be disposed as well.
        ALOGD("channel '%s' ~ Publisher closed input channel or an error occurred.  "
                "events=0x%x", getInputChannelName(), events);
#endif
        return 0; // remove the callback
    }

    if (events & ALOOPER_EVENT_OUTPUT) {
        for (size_t i = 0; i < mFinishQueue.size(); i++) {
            const Finish& finish = mFinishQueue.itemAt(i);
            android::status_t status = mInputConsumer.sendFinishedSignal(finish.seq, finish.handled);
            if (status) {
                mFinishQueue.removeItemsAt(0, i);

                if (status == android::WOULD_BLOCK) {
#if DEBUG_DISPATCH_CYCLE
                    ALOGD("channel '%s' ~ Sent %u queued finish events; %u left.",
                            getInputChannelName(), i, mFinishQueue.size());
#endif
                    return 1; // keep the callback, try again later
                }

                ALOGW("Failed to send finished signal on channel '%s'.  status=%d",
                        getInputChannelName(), status);
                if (status != android::DEAD_OBJECT) {
                    // String8 message;
                    // message.appendFormat("Failed to finish input event.  status=%d", status);
                    ALOGE("Failed to finish input event.  status=%d", status);
                    return E_RUNTIME_EXCEPTION;
                    // jniThrowRuntimeException(env, message.string());
                    // mMessageQueue->raiseAndClearException(env, "finishInputEvent");
                }
                return 0; // remove the callback
            }
        }
#if DEBUG_DISPATCH_CYCLE
        ALOGD("channel '%s' ~ Sent %u queued finish events; none left.",
                getInputChannelName(), mFinishQueue.size());
#endif
        mFinishQueue.clear();
        setFdEvents(ALOOPER_EVENT_INPUT);
        return 1;
    }

    ALOGW("channel '%s' ~ Received spurious callback for unhandled poll event.  "
            "events=0x%x", getInputChannelName(), events);
    return 1;
}

android::status_t InputEventReceiver::NativeInputEventReceiver::consumeEvents(
    bool consumeBatches, nsecs_t frameTime, Boolean* outConsumedBatch)
{
#if DEBUG_DISPATCH_CYCLE
    ALOGD("channel '%s' ~ Consuming input events, consumeBatches=%s, frameTime=%lld.",
            getInputChannelName(), consumeBatches ? "true" : "false", frameTime);
#endif

    if (consumeBatches) {
        mBatchedInputEventPending = FALSE;
    }
    if (outConsumedBatch) {
        *outConsumedBatch = FALSE;
    }

    AutoPtr<IWeakReference> receiverObj;
    bool skipCallbacks = false;
    for (;;) {
        uint32_t seq;
        android::InputEvent* inputEvent;
        android::status_t status = mInputConsumer.consume(&mInputEventFactory,
                consumeBatches, frameTime, &seq, &inputEvent);
        if (status) {
            if (status == android::WOULD_BLOCK) {
                if (!skipCallbacks && !mBatchedInputEventPending
                        && mInputConsumer.hasPendingBatch()) {
                    // There is a pending batch.  Come back later.
                    AutoPtr<IInterface> obj;
                    if (receiverObj != NULL) {
                        receiverObj->Resolve(EIID_IInterface, (IInterface**)&obj);
                    }
                    if (obj == NULL) {
                        receiverObj = mInputEventReceiver;
                        receiverObj->Resolve(EIID_IInterface, (IInterface**)&obj);
                        if (obj) {
                            ALOGW("channel '%s' ~ Receiver object was finalized "
                                    "without being disposed.", getInputChannelName());
                            return android::DEAD_OBJECT;
                        }
                    }

                    mBatchedInputEventPending = TRUE;
#if DEBUG_DISPATCH_CYCLE
                    ALOGD("channel '%s' ~ Dispatching batched input event pending notification.",
                            getInputChannelName());
#endif
                    obj = NULL;
                    if (receiverObj != NULL) {
                        receiverObj->Resolve(EIID_IInterface, (IInterface**)&obj);
                    }
                    if (obj) {
                        AutoPtr<IInputEventReceiver> tmp = IInputEventReceiver::Probe(obj);
                        AutoPtr<InputEventReceiver> inputEventReceiver = (InputEventReceiver*)(tmp.Get());
                        if (NOERROR != inputEventReceiver->DispatchBatchedInputEventPending()) {
                            ALOGE("Exception dispatching batched input events.");
                            mBatchedInputEventPending = FALSE; // try again later
                        }
                    }

                }
                return android::OK;
            }
            ALOGE("channel '%s' ~ Failed to consume input event.  status=%d",
                getInputChannelName(), status);
            return status;
        }
        assert(inputEvent);

        if (!skipCallbacks) {
            AutoPtr<IInterface> obj;
            if (receiverObj != NULL) {
                receiverObj->Resolve(EIID_IInterface, (IInterface**)&obj);
            }
            if (obj == NULL) {
                receiverObj = mInputEventReceiver;
                if (receiverObj != NULL) {
                    receiverObj->Resolve(EIID_IInterface, (IInterface**)&obj);
                }
                if (obj == NULL) {
                    ALOGW("channel '%s' ~ Receiver object was finalized "
                            "without being disposed.", getInputChannelName());
                    return android::DEAD_OBJECT;
                }
            }
            AutoPtr<IInputEvent> inputEventObj;
            switch (inputEvent->getType()) {
            case AINPUT_EVENT_TYPE_KEY:
            {
#if DEBUG_DISPATCH_CYCLE
                ALOGD("channel '%s' ~ Received key event.", getInputChannelName());
#endif
                AutoPtr<IKeyEvent> keyEvent = CreateKeyEventFromNative(
                    static_cast<android::KeyEvent*>(inputEvent));
                inputEventObj = IInputEvent::Probe(keyEvent);
                break;
            }
            case AINPUT_EVENT_TYPE_MOTION:
            {
#if DEBUG_DISPATCH_CYCLE
                ALOGD("channel '%s' ~ Received motion event.", getInputChannelName());
#endif
                android::MotionEvent* nMotionEvent = static_cast<android::MotionEvent*>(inputEvent);
                if ((nMotionEvent->getAction() & AMOTION_EVENT_ACTION_MOVE) && outConsumedBatch) {
                    *outConsumedBatch = TRUE;
                }
                AutoPtr<IMotionEvent> motionEvent = CreateMotionEventFromNative(nMotionEvent);
                inputEventObj = IInputEvent::Probe(motionEvent);
                break;
            }

            default:
                assert(false); // InputConsumer should prevent this from ever happening
            }

            if (inputEventObj) {
#if DEBUG_DISPATCH_CYCLE
                ALOGD("channel '%s' ~ Dispatching input event.", getInputChannelName());
#endif
                AutoPtr<IInterface> obj;
                mInputEventReceiver->Resolve(EIID_IInterface, (IInterface**)&obj);
                if (obj) {
                    AutoPtr<IInputEventReceiver> tmp = (IInputEventReceiver*)(obj->Probe(EIID_IInputEventReceiver));
                    AutoPtr<InputEventReceiver> inputEventReceiver = (InputEventReceiver*)(tmp.Get());
                    if (NOERROR != inputEventReceiver->DispatchInputEvent(seq, inputEventObj)) {
                        ALOGE("Exception dispatching input event.");
                        skipCallbacks = true;
                    }
                }
            }
            else {
                ALOGW("channel '%s' ~ Failed to obtain event object.", getInputChannelName());
                skipCallbacks = true;
            }
        }

        if (skipCallbacks) {
            mInputConsumer.sendFinishedSignal(seq, false);
        }
    }
}

/**
 * Creates an input event receiver bound to the specified input channel.
 *
 * @param inputChannel The input channel.
 * @param looper The looper to use when invoking callbacks.
 */
InputEventReceiver::InputEventReceiver(
    /* [in] */ IInputChannel* inputChannel,
    /* [in] */ ILooper* looper)
{
    Init(inputChannel, looper);
}

InputEventReceiver::~InputEventReceiver()
{
    Dispose();
}

void InputEventReceiver::Init(
    /* [in] */ IInputChannel* inputChannel,
    /* [in] */ ILooper* looper)
{
    if (inputChannel == NULL) {
        //throw new IllegalArgumentException("inputChannel must not be NULL");
        assert(0);
    }

    if (looper == NULL) {
        //throw new IllegalArgumentException("looper must not be NULL");
        assert(0);
    }

    mInputChannel = inputChannel;
    AutoPtr<IMessageQueue> queue;
    looper->GetQueue((IMessageQueue**)&queue);
    Handle32 nativeQueue;
    queue->GetNativeMessageQueue(&nativeQueue);
    mMessageQueue = (NativeMessageQueue*)nativeQueue;
    ASSERT_SUCCEEDED(NativeInit())

    //mCloseGuard.open("dispose");
}

/**
 * Disposes the receiver.
 */
ECode InputEventReceiver::Dispose()
{
    // if (mCloseGuard != NULL) {
    //     mCloseGuard.close();
    // }
    if (mNativeReceiver != NULL) {
        NativeDispose();
        mNativeReceiver = NULL;
    }
    mInputChannel = NULL;
    mMessageQueue = NULL;

    return NOERROR;
}

/**
 * Called when an input event is received.
 * The recipient should process the input event and then call {@link #finishInputEvent}
 * to indicate whether the event was handled.  No new input events will be received
 * until {@link #finishInputEvent} is called.
 *
 * @param event The input event that was received.
 */
ECode InputEventReceiver::OnInputEvent(
    /* [in] */ IInputEvent* event)
{
    return FinishInputEvent(event, FALSE);
}

/**
 * Called when a batched input event is pending.
 *
 * The batched input event will continue to accumulate additional movement
 * samples until the recipient calls {@link #consumeBatchedInputEvents} or
 * an event is received that ends the batch and causes it to be consumed
 * immediately (such as a pointer up event).
 */
ECode InputEventReceiver::OnBatchedInputEventPending()
{
    Boolean tmp;
    return ConsumeBatchedInputEvents(-1, &tmp);
}

/**
 * Finishes an input event and indicates whether it was handled.
 * Must be called on the same Looper thread to which the receiver is attached.
 *
 * @param event The input event that was finished.
 * @param handled True if the event was handled.
 */
ECode InputEventReceiver::FinishInputEvent(
    /* [in] */ IInputEvent* event,
    /* [in] */ Boolean handled)
{
    if (event == NULL) {
        Logger::E(TAG, "event must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (mNativeReceiver == NULL) {
        Logger::W(TAG, "Attempted to finish an input event but the input event "
            "receiver has already been disposed.");
    }
    else {
        Int32 sequenceNumber = 0;
        event->GetSequenceNumber(&sequenceNumber);
        HashMap<Int32, Int32>::Iterator find = mSeqMap.Find(sequenceNumber);
        if (find == mSeqMap.End()) {
            Logger::W(TAG, "Attempted to finish an input event that is not in progress.");
        }
        else {
            Int32 seq = find->mSecond;
            mSeqMap.Erase(find);
            FAIL_RETURN(NativeFinishInputEvent(seq, handled))
        }
    }
    event->RecycleIfNeededAfterDispatch();

    return NOERROR;
}

/**
 * Consumes all pending batched input events.
 * Must be called on the same Looper thread to which the receiver is attached.
 *
 * This method forces all batched input events to be delivered immediately.
 * Should be called just before animating or drawing a new frame in the UI.
 *
 * @param frameTimeNanos The time in the {@link System#nanoTime()} time base
 * when the current display frame started rendering, or -1 if unknown.
 */
ECode InputEventReceiver::ConsumeBatchedInputEvents(
    /* [in] */ Int64 frameTimeNanos,
    /* [out] */ Boolean* result)
{
    if (mNativeReceiver == NULL) {
        Logger::W(TAG, String("Attempted to consume batched input events but the input event ")
            + "receiver has already been disposed.");
    }
    else {
        // ensure mNativeReceiver won't be destructed before NativeConsumeBatchedInputEvents finished
        android::sp<NativeInputEventReceiver> nativeReceiver = mNativeReceiver;
        return NativeConsumeBatchedInputEvents(frameTimeNanos, result);
    }

    *result = FALSE;
    return NOERROR;
}

// Called from native code.
//@SuppressWarnings("unused")
ECode InputEventReceiver::DispatchInputEvent(
    /* [in] */ Int32 seq,
    /* [in] */ IInputEvent* event)
{
    Int32 sequenceNumber;
    event->GetSequenceNumber(&sequenceNumber);
    mSeqMap[sequenceNumber] = seq;
    return OnInputEvent(event);
}

// Called from native code.
//@SuppressWarnings("unused")
ECode InputEventReceiver::DispatchBatchedInputEventPending()
{
    return OnBatchedInputEventPending();
}

ECode InputEventReceiver::NativeInit()
{
    InputChannel* nChannel = (InputChannel*)mInputChannel.Get();
    Handle64 nInputChannel = nChannel->mNative;
    NativeInputChannel* nativeInputChannel = reinterpret_cast<NativeInputChannel*>(nInputChannel);
    android::sp<android::InputChannel> inputChannel =
            nativeInputChannel != NULL ? nativeInputChannel->getInputChannel() : NULL;
    if (inputChannel == NULL) {
        Logger::W(TAG, "inputChannel is not initialized.");
        return E_RUNTIME_EXCEPTION;
    }

    AutoPtr<IWeakReference> weakThis;
    GetWeakReference((IWeakReference**)&weakThis);
    mNativeReceiver = new NativeInputEventReceiver(
        weakThis, inputChannel, mMessageQueue);
    android::status_t status = mNativeReceiver->initialize();
    if (status) {
        ALOGE("Failed to initialize input event receiver.  status=%d", status);
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

ECode InputEventReceiver::NativeDispose()
{
    mNativeReceiver->dispose();
    return NOERROR;
}

ECode InputEventReceiver::NativeFinishInputEvent(
    /* [in] */ Int32 seq,
    /* [in] */ Boolean handled)
{
    android::status_t status = mNativeReceiver->finishInputEvent(seq, handled);
    if (status && status != android::DEAD_OBJECT) {
        ALOGE("Failed to finish input event.  status=%d", status);
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

ECode InputEventReceiver::NativeConsumeBatchedInputEvents(
    /* [in] */ Int64 frameTimeNanos,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    android::status_t status = mNativeReceiver->consumeEvents(true /*consumeBatches*/, frameTimeNanos, result);
    if (status && status != android::DEAD_OBJECT) {
        ALOGE("Failed to consume batched input event.  status=%d", status);
        *result = FALSE;
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

} // namespace View
} // namepsace Droid
} // namespace Elastos
