
#ifndef __ASYNCCHANNEL_H__
#define __ASYNCCHANNEL_H__

#include "ext/frameworkdef.h"
#include "os/HandlerBase.h"
#include "os/Runnable.h"
#include <elastos/Core/Object.h>
#include <elastos/Stack.h>

using Elastos::Utility::Stack;
using Elastos::Core::Object;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IServiceConnection;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Os::IHandlerThread;
using Elastos::Droid::Os::IMessenger;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Utility::IProtocol;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Utility {

class AsyncChannel
    : public ElRefBase
    , public IWeakReferenceSource
{
private:
    /**
     * Helper class to send messages synchronously
     */
    class SyncMessenger : public ElRefBase
    {
        friend class AsyncChannel;

    private:
        class SyncHandler
            : public HandlerBase
        {
        public:
            /** Constructor */
            SyncHandler(
                /* [in] */ ILooper* looper)
                : HandlerBase(looper)
            {}

            /** Handle of the reply message */
            // @Override
            CARAPI HandleMessage(
                /* [in] */ IMessage* msg);

        public:
            /** The object used to wait/notify */
            Object mLockObject;
            /** The resulting message */
            AutoPtr<IMessage> mResultMsg;
        };

    private:
        /** private constructor */
        SyncMessenger()
        {}

        /**
         * @return the SyncMessenger
         */
        static CARAPI_(AutoPtr<SyncMessenger>) Obtain();

        /**
         * Recycle this object
         */
        CARAPI_(void) Recycle();

        /**
         * Send a message synchronously.
         *
         * @param msg to send
         * @return result message or null if an error occurs
         */
        static CARAPI_(AutoPtr<IMessage>) SendMessageSynchronously(
            /* [in] */ IMessenger* dstMessenger,
            /* [in] */ IMessage* msg);

    private:
        /** A stack of SyncMessengers */
        static Stack< AutoPtr<SyncMessenger> > sStack;
        static Mutex sStackLock;
        /** A number of SyncMessengers created */
        static Int32 sCount;
        /** The handler thread */
        AutoPtr<IHandlerThread> mHandlerThread;
        /** The handler that will receive the result */
        AutoPtr<SyncHandler> mHandler;
        /** The messenger used to send the message */
        AutoPtr<IMessenger> mMessenger;
    };

    class AsyncChannelConnection
        : public ElRefBase
        , public IServiceConnection
    {
    public:
        CAR_INTERFACE_DECL()

        AsyncChannelConnection(
            /* [in] */ IWeakReference* host);

        CARAPI OnServiceConnected(
            /* [in] */ IComponentName* name,
            /* [in] */ IBinder* service);

        CARAPI OnServiceDisconnected(
            /* [in] */ IComponentName* name);
    private:
        AutoPtr<IWeakReference> mWeakHost;
    };

    class ConnectAsyncRunnable : public Runnable
    {
    public:
        ConnectAsyncRunnable(
            /* [in] */ IWeakReference* wr,
            /* [in] */ IContext* srcContext,
            /* [in] */ IHandler* srcHandler,
            /* [in] */ const String& dstPackageName,
            /* [in] */ const String& dstClassName);

        CARAPI Run();

    private:
        AutoPtr<IWeakReference> mWeakHost;
        AutoPtr<IContext> mSrcCtx;
        AutoPtr<IHandler> mSrcHdlr;
        String mDstPackageName;
        String mDstClassName;
    };
public:
    CAR_INTERFACE_DECL();

    AsyncChannel();

    virtual ~AsyncChannel();

    CARAPI GetWeakReference(
        /* [out] */ IWeakReference** weakReference);

    /**
     * Connect handler to named package/class synchronously.
     *
     * @param srcContext is the context of the source
     * @param srcHandler is the hander to receive CONNECTED & DISCONNECTED
     *            messages
     * @param dstPackageName is the destination package name
     * @param dstClassName is the fully qualified class name (i.e. contains
     *            package name)
     *
     * @return STATUS_SUCCESSFUL on success any other value is an error.
     */
    CARAPI_(Int32) ConnectSrcHandlerToPackageSync(
        /* [in] */ IContext* srcContext,
        /* [in] */ IHandler* srcHandler,
        /* [in] */ const String& dstPackageName,
        /* [in] */ const String& dstClassName);

    /**
     * Connect a handler to Messenger synchronously.
     *
     * @param srcContext is the context of the source
     * @param srcHandler is the hander to receive CONNECTED & DISCONNECTED
     *            messages
     * @param dstMessenger is the hander to send messages to.
     *
     * @return STATUS_SUCCESSFUL on success any other value is an error.
     */
    CARAPI_(Int32) ConnectSync(
        /* [in] */ IContext* srcContext,
        /* [in] */ IHandler* srcHandler,
        /* [in] */ IMessenger* dstMessenger);

    /**
     * connect two local Handlers synchronously.
     *
     * @param srcContext is the context of the source
     * @param srcHandler is the hander to receive CONNECTED & DISCONNECTED
     *            messages
     * @param dstHandler is the hander to send messages to.
     *
     * @return STATUS_SUCCESSFUL on success any other value is an error.
     */
    CARAPI_(Int32) ConnectSync(
        /* [in] */ IContext* srcContext,
        /* [in] */ IHandler* srcHandler,
        /* [in] */ IHandler* dstHandler);

    /**
     * Connect handler and messenger.
     *
     * Sends a CMD_CHANNEL_HALF_CONNECTED message to srcHandler when complete.
     *      msg.arg1 = status
     *      msg.obj = the AsyncChannel
     *
     * @param srcContext
     * @param srcHandler
     * @param dstMessenger
     */
    CARAPI_(void) Connect(
        /* [in] */ IContext* srcContext,
        /* [in] */ IHandler* srcHandler,
        /* [in] */ IMessenger* dstMessenger);

    /**
     * Connect handler to messenger. This method is typically called
     * when a server receives a CMD_CHANNEL_FULL_CONNECTION request
     * and initializes the internal instance variables to allow communication
     * with the dstMessenger.
     *
     * @param srcContext
     * @param srcHandler
     * @param dstMessenger
     */
    CARAPI_(void) Connected(
        /* [in] */ IContext* srcContext,
        /* [in] */ IHandler* srcHandler,
        /* [in] */ IMessenger* dstMessenger);

    /**
     * Connect two local Handlers.
     *
     * @param srcContext is the context of the source
     * @param srcHandler is the hander to receive CONNECTED & DISCONNECTED
     *            messages
     * @param dstHandler is the hander to send messages to.
     */
    CARAPI_(void) Connect(
        /* [in] */ IContext* srcContext,
        /* [in] */ IHandler* srcHandler,
        /* [in] */ IHandler* dstHandler);

    /**
     * Fully connect two local Handlers synchronously.
     *
     * @param srcContext is the context of the source
     * @param srcHandler is the hander to receive CONNECTED & DISCONNECTED
     *            messages
     * @param dstHandler is the hander to send messages to.
     *
     * @return STATUS_SUCCESSFUL on success any other value is an error.
     */
    CARAPI_(Int32) FullyConnectSync(
        /* [in] */ IContext* srcContext,
        /* [in] */ IHandler* srcHandler,
        /* [in] */ IHandler* dstHandler);

    /**
     * Connect handler to named package/class.
     *
     * Sends a CMD_CHANNEL_HALF_CONNECTED message to srcHandler when complete.
     *      msg.arg1 = status
     *      msg.obj = the AsyncChannel
     *
     * @param srcContext is the context of the source
     * @param srcHandler is the hander to receive CONNECTED & DISCONNECTED
     *            messages
     * @param dstPackageName is the destination package name
     * @param dstClassName is the fully qualified class name (i.e. contains
     *            package name)
     */
    CARAPI_(void) Connect(
        /* [in] */ IContext* srcContext,
        /* [in] */ IHandler* srcHandler,
        /* [in] */ const String& dstPackageName,
        /* [in] */ const String& dstClassName);

    /**
     * To close the connection call when handler receives CMD_CHANNEL_DISCONNECTED
     */
    CARAPI_(void) Disconnected();

    /**
     * Disconnect
     */
    CARAPI_(void) Disconnect();

    /**
     * Send a message to the destination handler.
     *
     * @param msg
     */
    CARAPI_(void) SendMessage(
        /* [in] */ IMessage* msg);

    /**
     * Send a message to the destination handler
     *
     * @param what
     */
    CARAPI_(void) SendMessage(
        /* [in] */ Int32 what);

    /**
     * Send a message to the destination handler
     *
     * @param what
     * @param arg1
     */
    CARAPI_(void) SendMessage(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1);

    /**
     * Send a message to the destination handler
     *
     * @param what
     * @param arg1
     * @param arg2
     */
    CARAPI_(void) SendMessage(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2);

    /**
     * Send a message to the destination handler
     *
     * @param what
     * @param arg1
     * @param arg2
     * @param obj
     */
    CARAPI_(void) SendMessage(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ IInterface* obj);

    /**
     * Send a message to the destination handler
     *
     * @param what
     * @param obj
     */
    CARAPI_(void) SendMessage(
        /* [in] */ Int32 what,
        /* [in] */ IInterface* obj);

    /**
     * Reply to srcMsg sending dstMsg
     *
     * @param srcMsg
     * @param dstMsg
     */
    CARAPI_(void) ReplyToMessage(
        /* [in] */ IMessage* srcMsg,
        /* [in] */ IMessage* dstMsg);

    /**
     * Reply to srcMsg
     *
     * @param srcMsg
     * @param what
     * @param arg1
     */
    CARAPI_(void) ReplyToMessage(
        /* [in] */ IMessage* srcMsg,
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1);

    /**
     * Reply to srcMsg
     *
     * @param srcMsg
     * @param what
     * @param obj
     */
    CARAPI_(void) ReplyToMessage(
        /* [in] */ IMessage* srcMsg,
        /* [in] */ Int32 what,
        /* [in] */ IInterface* obj);

    /**
     * Send the Message synchronously.
     *
     * @param msg to send
     * @return reply message or null if an error.
     */
    CARAPI_(AutoPtr<IMessage>) SendMessageSynchronously(
        /* [in] */ IMessage* msg);

    /**
     * Send the Message synchronously.
     *
     * @param what
     * @return reply message or null if an error.
     */
    CARAPI_(AutoPtr<IMessage>) SendMessageSynchronously(
        /* [in] */ Int32 what);

    /**
     * Send the Message synchronously.
     *
     * @param what
     * @param arg1
     * @return reply message or null if an error.
     */
    CARAPI_(AutoPtr<IMessage>) SendMessageSynchronously(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1);

    /**
     * Send the Message synchronously.
     *
     * @param what
     * @param arg1
     * @param arg2
     * @return reply message or null if an error.
     */
    CARAPI_(AutoPtr<IMessage>) SendMessageSynchronously(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2);

    /**
     * Send the Message synchronously.
     *
     * @param what
     * @param obj
     * @return reply message or null if an error.
     */
    CARAPI_(AutoPtr<IMessage>) SendMessageSynchronously(
        /* [in] */ Int32 what,
        /* [in] */ IInterface* obj);

private:
    /**
     * Reply to the src handler that we're half connected.
     * see: CMD_CHANNEL_HALF_CONNECTED for message contents
     *
     * @param status to be stored in msg.arg1
     */
    CARAPI_(void) ReplyHalfConnected(
        /* [in] */ Int32 status);

    /**
     * Reply to the src handler that we are disconnected
     * see: CMD_CHANNEL_DISCONNECTED for message contents
     *
     * @param status to be stored in msg.arg1
     */
    CARAPI_(void) ReplyDisconnected(
        /* [in] */ Int32 status);

private:
    static const Int32 BASE = IProtocol::BASE_SYSTEM_ASYNC_CHANNEL;

public:
    /**
     * Command sent when the channel is half connected. Half connected
     * means that the channel can be used to send commends to the destination
     * but the destination is unaware that the channel exists. The first
     * command sent to the destination is typically CMD_CHANNEL_FULL_CONNECTION if
     * it is desired to establish a long term connection, but any command maybe
     * sent.
     *
     * msg.arg1 == 0 : STATUS_SUCCESSFUL
     *             1 : STATUS_BINDING_UNSUCCESSFUL
     * msg.obj  == the AsyncChannel
     * msg.replyTo == dstMessenger if successful
     */
    static const Int32 CMD_CHANNEL_HALF_CONNECTED = BASE + 0;

    /**
     * Command typically sent when after receiving the CMD_CHANNEL_HALF_CONNECTED.
     * This is used to initiate a long term connection with the destination and
     * typically the destination will reply with CMD_CHANNEL_FULLY_CONNECTED.
     *
     * msg.replyTo = srcMessenger.
     */
    static const Int32 CMD_CHANNEL_FULL_CONNECTION = BASE + 1;

    /**
     * Command typically sent after the destination receives a CMD_CHANNEL_FULL_CONNECTION.
     * This signifies the acceptance or rejection of the channel by the sender.
     *
     * msg.arg1 == 0 : Accept connection
     *               : All other values signify the destination rejected the connection
     *                 and {@link AsyncChannel#disconnect} would typically be called.
     */
    static const Int32 CMD_CHANNEL_FULLY_CONNECTED = BASE + 2;

    /**
     * Command sent when one side or the other wishes to disconnect. The sender
     * may or may not be able to receive a reply depending upon the protocol and
     * the state of the connection. The receiver should call {@link AsyncChannel#disconnect}
     * to close its side of the channel and it will receive a CMD_CHANNEL_DISCONNECTED
     * when the channel is closed.
     *
     * msg.replyTo = messenger that is disconnecting
     */
    static const Int32 CMD_CHANNEL_DISCONNECT = BASE + 3;

    /**
     * Command sent when the channel becomes disconnected. This is sent when the
     * channel is forcibly disconnected by the system or as a reply to CMD_CHANNEL_DISCONNECT.
     *
     * msg.arg1 == 0 : STATUS_SUCCESSFUL
     *             1 : STATUS_BINDING_UNSUCCESSFUL
     *             2 : STATUS_SEND_UNSUCCESSFUL
     *               : All other values signify failure and the channel state is indeterminate
     * msg.obj  == the AsyncChannel
     * msg.replyTo = messenger disconnecting or null if it was never connected.
     */
    static const Int32 CMD_CHANNEL_DISCONNECTED = BASE + 4;

    /** Successful status always 0, !0 is an unsuccessful status */
    static const Int32 STATUS_SUCCESSFUL = 0;

    /** Error attempting to bind on a connect */
    static const Int32 STATUS_BINDING_UNSUCCESSFUL = 1;

    /** Error attempting to send a message */
    static const Int32 STATUS_SEND_UNSUCCESSFUL = 2;

    /** CMD_FULLY_CONNECTED refused because a connection already exists*/
    static const Int32 STATUS_FULL_CONNECTION_REFUSED_ALREADY_CONNECTED = 3;

private:
    /** Log tag */
    static const String TAG;

    /** Enable to turn on debugging */
    static const Boolean DBG;

    /** Context for source */
    AutoPtr<IContext> mSrcContext;

    /** Handler for source */
    AutoPtr<IHandler> mSrcHandler;

    /** Messenger for source */
    AutoPtr<IMessenger> mSrcMessenger;

    /** Messenger for destination */
    AutoPtr<IMessenger> mDstMessenger;

    /** Service connection */
    AutoPtr<IServiceConnection> mConnection;
};

} // namespace Utility
} // namespace Internal
} // namespace Droid
} // namespace Elastos


#endif //__ASYNCCHANNEL_H__
