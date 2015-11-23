#ifndef __ELASTOS_DROID_VIEW_INPUTEVENTSENDER_H__
#define __ELASTOS_DROID_VIEW_INPUTEVENTSENDER_H__

namespace Elastos {
namespace Droid {
namespace View {

class InputEventSender
    : public Object
    , public IInputEventSender
{
public:
    InputEventSender();

    CARAPI constructor(
        /* [in] */ IInputChannel* inputChannel,
        /* [in] */ ILooper* looper);

    CAR_INTERFACE_DECL()

    CARAPI Dispose();

    CARAPI OnInputEventFinished(
        /* [in] */ Int32 seq,
        /* [in] */ Boolean handled);

    CARAPI SendInputEvent(
        /* [in] */ Int32 seq,
        /* [in] */ IInputEvent* event,
        /* [out] */ Boolean res);

protected:
    CARAPI Finalize();

private:
    CARAPI Dispose(
        /* [in] */ Boolean finalized);

    CARAPI DispatchInputEventFinished(
        /* [in] */ Int32 seq,
        /* [in] */ Boolean handled);

    static CARAPI NativeInit(
        /* [in] */ IWeakReference* senderWeak,
        /* [in] */ IInputChannel* inputChannelObj,
        /* [in] */ IMessageQueue* messageQueueObj,
        /* [out] */ Int64 res);

    static CARAPI_(void) NativeDispose(
        /* [in] */ Int64 senderPtr);

    static CARAPI_(Boolean) NativeSendKeyEvent(
        /* [in] */ Int64 senderPtr,
        /* [in] */ Int32 seq,
        /* [in] */ IKeyEvent* eventObj);

    static CARAPI_(Boolean) NativeSendMotionEvent(
        /* [in] */ Int64 senderPtr,
        /* [in] */ Int32 seq,
        /* [in] */ IMotionEvent* eventObj);

    CARAPI_(AutoPtr<IInputChannel>) GetInputChannel
        /* [in] */ IInterface* inputChannelObj);

    CARAPI_(sp<InputChannel>) GetNativeInputChannel(
        /* [in] */ IInputChannel* inputChannelObj);

    CARAPI_(sp<MessageQueue>) GetMessageQueue(
        /* [in] */ IMessageQueue* messageQueueObj);



private:
    static const String TAG;// = "InputEventSender";

    AutoPtr<ICloseGuard> mCloseGuard;// = CloseGuard.get();

    Int64 mSenderPtr;

    // We keep references to the input channel and message queue objects here so that
    // they are not GC'd while the native peer of the receiver is using them.
    AutoPtr<IInputChannel> mInputChannel;
    AutoPtr<IMessageQueue> mMessageQueue;
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_VIEW_INPUTEVENTSENDER_H__