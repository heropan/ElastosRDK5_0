
#include "elastos/droid/view/InputEventReceiver.h"
#include "elastos/droid/view/NativeInputChannel.h"
#ifdef DROID_CORE
#include "elastos/droid/view/CKeyEvent.h"
#include "elastos/droid/view/CMotionEvent.h"
#endif
#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::IMessageQueue;

namespace Elastos {
namespace Droid {
namespace View {

const char* InputEventReceiver::TAG = "InputEventReceiver";

#define DEBUG_DISPATCH_CYCLE 0

static AutoPtr<IKeyEvent> CreateKeyEventFromNative(
    /* [in] */ const android::KeyEvent* event)
{
    AutoPtr<IKeyEvent> eventObj;
    ECode ec = CKeyEvent::New(nanoseconds_to_milliseconds(event->getDownTime()),
            nanoseconds_to_milliseconds(event->getEventTime()),
            event->getAction(),
            event->getKeyCode(),
            event->getRepeatCount(),
            event->getMetaState(),
            event->getDeviceId(),
            event->getScanCode(),
            event->getFlags(),
            event->getSource(),
            (IKeyEvent**)&eventObj);
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
    if (FAILED(CMotionEvent::New((IMotionEvent**)&eventObj))) {
        return NULL;
    }

    Handle32 native;
    eventObj->GetNative(&native);
    android::MotionEvent* destEvent = (android::MotionEvent*)native;
    if (!destEvent) {
        destEvent = new android::MotionEvent();
        eventObj->SetNative((Handle32)destEvent);
    }

    destEvent->copyFrom(event, true);
    return eventObj;
}

CAR_INTERFACE_IMPL_2(InputEventReceiver, IInputEventReceiver, IWeakReferenceSource);

InputEventReceiver::NativeInputEventReceiver::NativeInputEventReceiver(
    /* [in] */ IWeakReference* inputEventReceiver,
    /* [in] */ const android::sp<android::InputChannel>& inputChannel,
    /* [in] */ const android::sp<android::Looper>& looper) :
    mInputEventReceiver(inputEventReceiver),
    mInputConsumer(inputChannel),
    mLooper(looper),
    mBatchedInputEventPending(false)
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
    int receiveFd = mInputConsumer.getChannel()->getFd();
    mLooper->addFd(receiveFd, 0, ALOOPER_EVENT_INPUT, this, NULL);
    return android::OK;
}

void InputEventReceiver::NativeInputEventReceiver::dispose()
{
#if DEBUG_DISPATCH_CYCLE
    ALOGD("channel '%s' ~ Disposing input event receiver.", getInputChannelName());
#endif

    mLooper->removeFd(mInputConsumer.getChannel()->getFd());
}

android::status_t InputEventReceiver::NativeInputEventReceiver::finishInputEvent(uint32_t seq, bool handled)
{
#if DEBUG_DISPATCH_CYCLE
    ALOGD("channel '%s' ~ Finished input event.", getInputChannelName());
#endif

    android::status_t status = mInputConsumer.sendFinishedSignal(seq, handled);
    if (status) {
        ALOGW("Failed to send finished signal on channel '%s'.  status=%d",
                getInputChannelName(), status);
    }
    return status;
}

int InputEventReceiver::NativeInputEventReceiver::handleEvent(int receiveFd, int events, void* data)
{
    if (events & (ALOOPER_EVENT_ERROR | ALOOPER_EVENT_HANGUP)) {
        ALOGE("channel '%s' ~ Publisher closed input channel or an error occurred.  "
                "events=0x%x", getInputChannelName(), events);
        return 0; // remove the callback
    }

    if (!(events & ALOOPER_EVENT_INPUT)) {
        ALOGW("channel '%s' ~ Received spurious callback for unhandled poll event.  "
                "events=0x%x", getInputChannelName(), events);
        return 1;
    }

    android::status_t status = consumeEvents(false /*consumeBatches*/, -1);
    //mMessageQueue->raiseAndClearException(env, "handleReceiveCallback");
    return status == android::OK || status == android::NO_MEMORY ? 1 : 0;
}

android::status_t InputEventReceiver::NativeInputEventReceiver::consumeEvents(
    bool consumeBatches, nsecs_t frameTime) {
#if DEBUG_DISPATCH_CYCLE
    ALOGD("channel '%s' ~ Consuming input events, consumeBatches=%s, frameTime=%lld.",
            getInputChannelName(), consumeBatches ? "true" : "false", frameTime);
#endif

    if (consumeBatches) {
        mBatchedInputEventPending = false;
    }

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
                    mBatchedInputEventPending = true;
#if DEBUG_DISPATCH_CYCLE
                    ALOGD("channel '%s' ~ Dispatching batched input event pending notification.",
                            getInputChannelName());
#endif
                    AutoPtr<IInterface> obj;
                    mInputEventReceiver->Resolve(EIID_IInterface, (IInterface**)&obj);
                    if (obj) {
                        AutoPtr<IInputEventReceiver> tmp = (IInputEventReceiver*)(obj->Probe(EIID_IInputEventReceiver));
                        AutoPtr<InputEventReceiver> inputEventReceiver = (InputEventReceiver*)(tmp.Get());
                        if (NOERROR != inputEventReceiver->DispatchBatchedInputEventPending()) {
                        ALOGE("Exception dispatching batched input events.");
                        mBatchedInputEventPending = false; // try again later
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
                AutoPtr<IMotionEvent> motionEvent = CreateMotionEventFromNative(
                    static_cast<android::MotionEvent*>(inputEvent));
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
    NativeInit();

    //mCloseGuard.open("dispose");
}

InputEventReceiver::~InputEventReceiver()
{
    Dispose();
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
    return ConsumeBatchedInputEvents(-1);
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
            NativeFinishInputEvent(seq, handled);
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
    /* [in] */ Int64 frameTimeNanos)
{
    if (mNativeReceiver == NULL) {
        Logger::W(TAG, String("Attempted to consume batched input events but the input event ")
            + "receiver has already been disposed.");
    }
    else {
        // ensure mNativeReceiver won't be destructed before NativeConsumeBatchedInputEvents finished
        android::sp<NativeInputEventReceiver> nativeReceiver = mNativeReceiver;
        NativeConsumeBatchedInputEvents(frameTimeNanos);
    }

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

void InputEventReceiver::NativeInit()
{
    NativeInputChannel* nativeInputChannel = NULL;
    mInputChannel->GetNativeInputChannel((Handle32*)&nativeInputChannel);
    android::sp<android::InputChannel> inputChannel =
            nativeInputChannel != NULL ? nativeInputChannel->getInputChannel() : NULL;
    if (inputChannel == NULL) {
        Logger::W(TAG, "inputChannel is not initialized.");
        return ;
    }

    AutoPtr<IWeakReference> weakThis;
    GetWeakReference((IWeakReference**)&weakThis);
    mNativeReceiver = new NativeInputEventReceiver(
        weakThis, inputChannel, mMessageQueue->GetLooper());
    android::status_t status = mNativeReceiver->initialize();
    if (status) {
        ALOGE("Failed to initialize input event receiver.  status=%d", status);
    }
}

void InputEventReceiver::NativeDispose()
{
    mNativeReceiver->dispose();
}

void InputEventReceiver::NativeFinishInputEvent(
    /* [in] */ Int32 seq,
    /* [in] */ Boolean handled)
{
    android::status_t status = mNativeReceiver->finishInputEvent(seq, handled);
    if (status && status != android::DEAD_OBJECT) {
        ALOGE("Failed to finish input event.  status=%d", status);
    }
}

void InputEventReceiver::NativeConsumeBatchedInputEvents(
    /* [in] */ Int64 frameTimeNanos)
{
    android::status_t status = mNativeReceiver->consumeEvents(true /*consumeBatches*/, frameTimeNanos);
    if (status && status != android::DEAD_OBJECT) {
        ALOGE("Failed to consume batched input event.  status=%d", status);
    }
}

ECode InputEventReceiver::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(Probe(EIID_IInterface), CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}

} // namespace View
} // namepsace Droid
} // namespace Elastos
