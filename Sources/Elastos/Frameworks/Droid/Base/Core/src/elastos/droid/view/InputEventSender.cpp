#include "elastos/droid/view/InputEventSender.h"

namespace Elastos {
namespace Droid {
namespace View {

//=========================================================
//                  NativeInputChannel
//=========================================================

typedef void (*InputChannelObjDisposeCallback)(jobject inputChannelObj,
        const sp<InputChannel>& inputChannel, void* data)

class NativeInputChannel
{
public:
    NativeInputChannel(const sp<InputChannel>& inputChannel);
    ~NativeInputChannel();

    inline sp<InputChannel> getInputChannel() { return mInputChannel; }

    void setDisposeCallback(InputChannelObjDisposeCallback callback, void* data);
    void invokeAndRemoveDisposeCallback(jobject obj);

private:
    sp<InputChannel> mInputChannel;
    InputChannelObjDisposeCallback mDisposeCallback;
    void* mDisposeData;
};

NativeInputChannel::NativeInputChannel(const sp<InputChannel>& inputChannel) :
    mInputChannel(inputChannel), mDisposeCallback(NULL)
{}

NativeInputChannel::~NativeInputChannel() {
}

void NativeInputChannel::setDisposeCallback(InputChannelObjDisposeCallback callback, void* data)
{
    mDisposeCallback = callback;
    mDisposeData = data;
}

void NativeInputChannel::invokeAndRemoveDisposeCallback(jobject obj) {
    if (mDisposeCallback) {
        mDisposeCallback(obj, mInputChannel, mDisposeData);
        mDisposeCallback = NULL;
        mDisposeData = NULL;
    }
}

//==========================================================
//              NativeInputEventSender
//==========================================================

class NativeInputEventSender
    : public LooperCallback
{
public:
    NativeInputEventSender(
        AutoPtr<IWeakReference> senderWeak,
        const sp<InputChannel>& inputChannel,
        const sp<MessageQueue>& messageQueue);

    status_t initialize();
    void dispose();
    status_t sendKeyEvent(uint32_t seq, const KeyEvent* event);
    status_t sendMotionEvent(uint32_t seq, const MotionEvent* event);

protected:
    virtual ~NativeInputEventSender();

private:
    AutoPtr<IWeakReference> mSenderWeakGlobal;
    InputPublisher mInputPublisher;
    sp<MessageQueue> mMessageQueue;
    KeyedVector<uint32_t, uint32_t> mPublishedSeqMap;
    uint32_t mNextPublishedSeq;

    const char* getInputChannelName() {
        return mInputPublisher.getChannel()->getName().string();
    }

    virtual int handleEvent(int receiveFd, int events, void* data);
    status_t receiveFinishedSignals();
};


NativeInputEventSender::NativeInputEventSender(
    jobject senderWeak,
    const sp<InputChannel>& inputChannel,
    const sp<MessageQueue>& messageQueue)
    : mSenderWeakGlobal(env->NewGlobalRef(senderWeak))
    , mInputPublisher(inputChannel)
    , mMessageQueue(messageQueue)
    , mNextPublishedSeq(1)
{
#if DEBUG_DISPATCH_CYCLE
    ALOGD("channel '%s' ~ Initializing input event sender.", getInputChannelName());
#endif
}

NativeInputEventSender::~NativeInputEventSender()
{
    JNIEnv* env = AndroidRuntime::getJNIEnv();
    env->DeleteGlobalRef(mSenderWeakGlobal);
}

status_t NativeInputEventSender::initialize()
{
    int receiveFd = mInputPublisher.getChannel()->getFd();
    mMessageQueue->getLooper()->addFd(receiveFd, 0, ALOOPER_EVENT_INPUT, this, NULL);
    return OK;
}

void NativeInputEventSender::dispose()
{
#if DEBUG_DISPATCH_CYCLE
    ALOGD("channel '%s' ~ Disposing input event sender.", getInputChannelName());
#endif

    mMessageQueue->getLooper()->removeFd(mInputPublisher.getChannel()->getFd());
}

status_t NativeInputEventSender::sendKeyEvent(uint32_t seq, const KeyEvent* event)
{
#if DEBUG_DISPATCH_CYCLE
    ALOGD("channel '%s' ~ Sending key event, seq=%u.", getInputChannelName(), seq);
#endif

    uint32_t publishedSeq = mNextPublishedSeq++;
    status_t status = mInputPublisher.publishKeyEvent(publishedSeq,
            event->getDeviceId(), event->getSource(), event->getAction(), event->getFlags(),
            event->getKeyCode(), event->getScanCode(), event->getMetaState(),
            event->getRepeatCount(), event->getDownTime(), event->getEventTime());
    if (status) {
        ALOGW("Failed to send key event on channel '%s'.  status=%d",
                getInputChannelName(), status);
        return status;
    }
    mPublishedSeqMap.add(publishedSeq, seq);
    return OK;
}

status_t NativeInputEventSender::sendMotionEvent(uint32_t seq, const MotionEvent* event)
{
#if DEBUG_DISPATCH_CYCLE
    ALOGD("channel '%s' ~ Sending motion event, seq=%u.", getInputChannelName(), seq);
#endif

    uint32_t publishedSeq;
    for (size_t i = 0; i <= event->getHistorySize(); i++) {
        publishedSeq = mNextPublishedSeq++;
        status_t status = mInputPublisher.publishMotionEvent(publishedSeq,
                event->getDeviceId(), event->getSource(), event->getAction(), event->getFlags(),
                event->getEdgeFlags(), event->getMetaState(), event->getButtonState(),
                event->getXOffset(), event->getYOffset(),
                event->getXPrecision(), event->getYPrecision(),
                event->getDownTime(), event->getHistoricalEventTime(i),
                event->getPointerCount(), event->getPointerProperties(),
                event->getHistoricalRawPointerCoords(0, i));
        if (status) {
            ALOGW("Failed to send motion event sample on channel '%s'.  status=%d",
                    getInputChannelName(), status);
            return status;
        }
    }
    mPublishedSeqMap.add(publishedSeq, seq);
    return OK;
}

int NativeInputEventSender::handleEvent(int receiveFd, int events, void* data)
{
    if (events & (ALOOPER_EVENT_ERROR | ALOOPER_EVENT_HANGUP)) {
#if DEBUG_DISPATCH_CYCLE
        // This error typically occurs when the consumer has closed the input channel
        // as part of finishing an IME session, in which case the publisher will
        // soon be disposed as well.
        ALOGD("channel '%s' ~ Consumer closed input channel or an error occurred.  "
                "events=0x%x", getInputChannelName(), events);
#endif
        return 0; // remove the callback
    }

    if (!(events & ALOOPER_EVENT_INPUT)) {
        ALOGW("channel '%s' ~ Received spurious callback for unhandled poll event.  "
                "events=0x%x", getInputChannelName(), events);
        return 1;
    }

    JNIEnv* env = AndroidRuntime::getJNIEnv();
    status_t status = receiveFinishedSignals(env);
    mMessageQueue->raiseAndClearException(env, "handleReceiveCallback");
    return status == OK || status == NO_MEMORY ? 1 : 0;
}

status_t NativeInputEventSender::receiveFinishedSignals()
{
#if DEBUG_DISPATCH_CYCLE
    ALOGD("channel '%s' ~ Receiving finished signals.", getInputChannelName());
#endif

    ScopedLocalRef<jobject> senderObj(env, NULL);
    bool skipCallbacks = false;
    for (;;) {
        uint32_t publishedSeq;
        bool handled;
        status_t status = mInputPublisher.receiveFinishedSignal(&publishedSeq, &handled);
        if (status) {
            if (status == WOULD_BLOCK) {
                return OK;
            }
            ALOGE("channel '%s' ~ Failed to consume finished signals.  status=%d",
                    getInputChannelName(), status);
            return status;
        }

        ssize_t index = mPublishedSeqMap.indexOfKey(publishedSeq);
        if (index >= 0) {
            uint32_t seq = mPublishedSeqMap.valueAt(index);
            mPublishedSeqMap.removeItemsAt(index);

#if DEBUG_DISPATCH_CYCLE
            ALOGD("channel '%s' ~ Received finished signal, seq=%u, handled=%s, "
                    "pendingEvents=%u.",
                    getInputChannelName(), seq, handled ? "true" : "false",
                    mPublishedSeqMap.size());
#endif

            if (!skipCallbacks) {
                if (!senderObj.get()) {
                    senderObj.reset(jniGetReferent(env, mSenderWeakGlobal));
                    if (!senderObj.get()) {
                        ALOGW("channel '%s' ~ Sender object was finalized "
                                "without being disposed.", getInputChannelName());
                        return DEAD_OBJECT;
                    }
                }

                env->CallVoidMethod(senderObj.get(),
                        gInputEventSenderClassInfo.dispatchInputEventFinished,
                        jint(seq), jboolean(handled));
                if (env->ExceptionCheck()) {
                    ALOGE("Exception dispatching finished signal.");
                    skipCallbacks = true;
                }
            }
        }
    }
}

//==========================================================
//          NativeMessageQueue
//==========================================================

class NativeMessageQueue
    : public MessageQueue
{
public:
    NativeMessageQueue();
    virtual ~NativeMessageQueue();

    virtual void raiseException(const char* msg, ECode exceptionObj);

    void pollOnce(int timeoutMillis);

    void wake();

private:
    bool mInCallback;
    ECode mExceptionObj;
};


MessageQueue::MessageQueue() {
}

MessageQueue::~MessageQueue() {
}

bool MessageQueue::raiseAndClearException(const char* msg) {
    jthrowable exceptionObj = env->ExceptionOccurred();
    if (exceptionObj) {
        env->ExceptionClear();
        raiseException(env, msg, exceptionObj);
        env->DeleteLocalRef(exceptionObj);
        return true;
    }
    return false;
}

NativeMessageQueue::NativeMessageQueue() : mInCallback(false), mExceptionObj(NULL) {
    mLooper = Looper::getForThread();
    if (mLooper == NULL) {
        mLooper = new Looper(false);
        Looper::setForThread(mLooper);
    }
}

NativeMessageQueue::~NativeMessageQueue() {
}

void NativeMessageQueue::raiseException(JNIEnv* env, const char* msg, jthrowable exceptionObj) {
    if (exceptionObj) {
        if (mInCallback) {
            if (mExceptionObj) {
                env->DeleteLocalRef(mExceptionObj);
            }
            mExceptionObj = jthrowable(env->NewLocalRef(exceptionObj));
            ALOGE("Exception in MessageQueue callback: %s", msg);
            jniLogException(env, ANDROID_LOG_ERROR, LOG_TAG, exceptionObj);
        } else {
            ALOGE("Exception: %s", msg);
            jniLogException(env, ANDROID_LOG_ERROR, LOG_TAG, exceptionObj);
            LOG_ALWAYS_FATAL("raiseException() was called when not in a callback, exiting.");
        }
    }
}

void NativeMessageQueue::pollOnce(JNIEnv* env, int timeoutMillis) {
    mInCallback = true;
    mLooper->pollOnce(timeoutMillis);
    mInCallback = false;
    if (mExceptionObj) {
        env->Throw(mExceptionObj);
        env->DeleteLocalRef(mExceptionObj);
        mExceptionObj = NULL;
    }
}

void NativeMessageQueue::wake() {
    mLooper->wake();
}

//==========================================================

//==========================================================

const String InputEventSender::TAG = String("InputEventSender");

CAR_INTERFACE_IMPL(InputEventSender, Object, IInputEventSender)

InputEventSender::InputEventSender()
    : mSenderPtr(0)
{
    mCloseGuard = CCloseGuard::Get();
}

ECode InputEventSender::constructor(
    /* [in] */ IInputChannel* inputChannel,
    /* [in] */ ILooper* looper)
{
    if (inputChannel == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (looper == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mInputChannel = inputChannel;
    looper->GetQueue((IMessageQueue**)&mMessageQueue);
    /*mSenderPtr = nativeInit(new WeakReference<InputEventSender>(this),
            inputChannel, mMessageQueue);*/

    mCloseGuard->Open(String("dispose"));
}

ECode InputEventSender::Finalize()
{
    ECode ec = Dispose(TRUE);
    if (FAILED(ec)) {
        //super.finalize();
    }
    return ec;
}

/**
 * Disposes the receiver.
 */
ECode InputEventSender::Dispose()
{
    return Dispose(FALSE);
}

ECode InputEventSender::Dispose(
    /* [in] */ Boolean finalized)
{
    if (mCloseGuard != NULL) {
        if (finalized) {
            mCloseGuard->WarnIfOpen();
        }
        mCloseGuard->Close();
    }

    if (mSenderPtr != 0) {
        NativeDispose(mSenderPtr);
        mSenderPtr = 0;
    }
    mInputChannel = NULL;
    mMessageQueue = NULL;
}

/**
 * Called when an input event is finished.
 *
 * @param seq The input event sequence number.
 * @param handled True if the input event was handled.
 */
ECode InputEventSender::OnInputEventFinished(
    /* [in] */ Int32 seq,
    /* [in] */ Boolean handled)
{
}

/**
 * Sends an input event.
 * Must be called on the same Looper thread to which the sender is attached.
 *
 * @param seq The input event sequence number.
 * @param event The input event to send.
 * @return True if the entire event was sent successfully.  May return false
 * if the input channel buffer filled before all samples were dispatched.
 */
ECode InputEventSender::SendInputEvent(
    /* [in] */ Int32 seq,
    /* [in] */ IInputEvent* event,
    /* [out] */ Boolean res)
{
    VALIDATE_NOT_NULL(res)
    if (event == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (mSenderPtr == 0) {
        /*Log.w(TAG, "Attempted to send an input event but the input event "
                + "sender has already been disposed.");*/
        *res = FALSE;
        return NOERROR;
    }

    if (IKeyEvent::Probe(event)) {
        return NativeSendKeyEvent(mSenderPtr, seq, IKeyEvent::Probe(event));
    } else {
        return NativeSendMotionEvent(mSenderPtr, seq, IKeyEvent::Probe(event));
    }
}

// Called from native code.
ECode InputEventSender::DispatchInputEventFinished(
    /* [in] */ Int32 seq,
    /* [in] */ Boolean handled)
{
    return OnInputEventFinished(seq, handled);
}

ECode InputEventSender::NativeInit(
    /* [in] */ IWeakReference* senderWeak,
    /* [in] */ IInputChannel* inputChannelObj,
    /* [in] */ IMessageQueue* messageQueueObj,
    /* [out] */ Int64 res)
{
    sp<InputChannel> inputChannel = GetInputChannel(inputChannelObj);

    if (inputChannel == NULL) {
        Slogger::D("InputChannel is not initialized.");
        *res = 0;
        return E_RUNTIME_EXCEPTION;
    }

    sp<MessageQueue> messageQueue = GetMessageQueue(messageQueueObj);
    if (inputChannel == NULL) {
        Slogger::D("InputChannel is not initialized.");
        *res = 0;
        return E_RUNTIME_EXCEPTION;
    }

    sp<MessageQueue> messageQueue = GetMessageQueue( messageQueueObj);
    if (messageQueue == NULL) {
        Slogger::D("MessageQueue is not initialized.");
        *res = 0;
        return E_RUNTIME_EXCEPTION;
    }

    sp<NativeInputEventSender> sender =
        new NativeInputEventSender(senderWeak, inputChannel, messageQueue);

    status_t status = sender->initialize();
    if (status) {
        String8 message;
        message.appendFormat("Failed to initialize input event sender.  status=%d", status);
        jniThrowRuntimeException(env, message.string());
        return 0;
    }

    sender->incStrong(gInputEventSenderClassInfo.clazz); // retain a reference for the object
    return reinterpret_cast<jlong>(sender.get());
}

void InputEventSender::NativeDispose(
    /* [in] */ Int64 senderPtr)
{
    sp<NativeInputEventSender> sender =
            reinterpret_cast<NativeInputEventSender*>(senderPtr);
    sender->dispose();
    sender->decStrong(gInputEventSenderClassInfo.clazz);
}

Boolean InputEventSender::NativeSendKeyEvent(
    /* [in] */ Int64 senderPtr,
    /* [in] */ Int32 seq,
    /* [in] */ IKeyEvent* eventObj)
{
    sp<NativeInputEventSender> sender =
            reinterpret_cast<NativeInputEventSender*>(senderPtr);
    KeyEvent event;
    android_view_KeyEvent_toNative(env, eventObj, &event);
    status_t status = sender->sendKeyEvent(seq, &event);
    return !status;
}

Boolean InputEventSender::NativeSendMotionEvent(
    /* [in] */ Int64 senderPtr,
    /* [in] */ Int32 seq,
    /* [in] */ IMotionEvent* eventObj)
{
    sp<NativeInputEventSender> sender =
            reinterpret_cast<NativeInputEventSender*>(senderPtr);
    MotionEvent* event = android_view_MotionEvent_getNativePtr(env, eventObj);
    status_t status = sender->sendMotionEvent(seq, event);
    return !status;
}

sp<InputChannel> InputEventSender::GetInputChannel(
    /* [in] */ IInputChannel* inputChannelObj)
{

    Handle64 longPtr = ((InputChannel*)inputChannelObj)->mNative;
    AutoPtr<NativeInputChannel> nativeInputChannel =
        reinterpret_cast<NativeInputChannel>(longPtr);
    return nativeInputChannel != NULL ? nativeInputChannel->getInputChannel() : NULL;
}

sp<MessageQueue> InputEventSender::GetMessageQueue(
    /* [in] */ IMessageQueue* messageQueueObj)
{
    Int64 ptr = ((CMessageQueue*)messageQueueObj)->mPtr;
    return reinterpret_cast<NativeMessageQueue*>(ptr);
}

} // namespace View
} // namespace Droid
} // namespace Elastos