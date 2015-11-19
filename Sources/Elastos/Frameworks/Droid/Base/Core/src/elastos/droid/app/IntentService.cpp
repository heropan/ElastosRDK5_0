
#include "elastos/droid/app/IntentService.h"


namespace Elastos {
namespace Droid {
namespace App {

    class ServiceHandler
        : public Handler
    {
    public:
        ServiceHandler(
            /* [in] */ ILooper* looper,
            /* [in] */ IntentService* host)
        {
            super(looper);
        }

        //@Override
        CARAPI HandleMessage(
            /* [in] */ IMessage* msg)
        {
            onHandleIntent((Intent)msg.obj);
            stopSelf(msg.arg1);
        }
    private:
        IntentService* mHost;
    };


    CAR_INTERFACE_IMPL(IntentService, Service, IIntentService)
    /**
     * Creates an IntentService.  Invoked by your subclass's constructor.
     *
     * @param name Used to name the worker thread, important only for debugging.
     */
    CARAPI constructor(
        /* [in] */ String name)
    {
        super();
        mName = name;
    }

    /**
     * Sets intent redelivery preferences.  Usually called from the constructor
     * with your preferred semantics.
     *
     * <p>If enabled is true,
     * {@link #onStartCommand(Intent, Int32, Int32)} will return
     * {@link Service#START_REDELIVER_INTENT}, so if this process dies before
     * {@link #onHandleIntent(Intent)} returns, the process will be restarted
     * and the intent redelivered.  If multiple Intents have been sent, only
     * the most recent one is guaranteed to be redelivered.
     *
     * <p>If enabled is false (the default),
     * {@link #onStartCommand(Intent, Int32, Int32)} will return
     * {@link Service#START_NOT_STICKY}, and if the process dies, the Intent
     * dies along with it.
     */
    CARAPI SetIntentRedelivery(
        /* [in] */ Boolean enabled)
    {
        mRedelivery = enabled;
    }

    //@Override
    CARAPI OnCreate()
    {
        // TODO: It would be nice to have an option to hold a partial wakelock
        // during processing, and to have a static startService(Context, Intent)
        // method that would launch the service & hand off a wakelock.

        super.onCreate();
        HandlerThread thread = new HandlerThread("IntentService[" + mName + "]");
        thread.start();

        mServiceLooper = thread.getLooper();
        mServiceHandler = new ServiceHandler(mServiceLooper);
    }

    //@Override
    CARAPI OnStart(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 startId)
    {
        Message msg = mServiceHandler.obtainMessage();
        msg.arg1 = startId;
        msg.obj = intent;
        mServiceHandler.sendMessage(msg);
    }

    /**
     * You should not override this method for your IntentService. Instead,
     * override {@link #onHandleIntent}, which the system calls when the IntentService
     * receives a start request.
     * @see android.app.Service#onStartCommand
     */
    //@Override
    CARAPI OnStartCommand(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 startId,
        /* [out] */ Int32* result)
    {
        onStart(intent, startId);
        return mRedelivery ? START_REDELIVER_INTENT : START_NOT_STICKY;
    }

    //@Override
    CARAPI OnDestroy() {
        mServiceLooper.quit();
    }

    /**
     * Unless you provide binding for your service, you don't need to implement this
     * method, because the default implementation returns null.
     * @see android.app.Service#onBind
     */
    //@Override
    CARAPI OnBind(
        /* [in] */ IIntent* intent,
        /* [out] */ IBinder** binder)
    {
        return null;
    }

protected:
    /**
     * This method is invoked on the worker thread with a request to process.
     * Only one Intent is processed at a time, but the processing happens on a
     * worker thread that runs independently from other application logic.
     * So, if this code takes a long time, it will hold up other requests to
     * the same IntentService, but it will not hold up anything else.
     * When all requests have been handled, the IntentService stops itself,
     * so you should not call {@link #stopSelf}.
     *
     * @param intent The value passed to {@link
     *               android.content.Context#startService(Intent)}.
     */
    CARPAI OnHandleIntent(
        /* [in] */ IIntent* intent);

private:
    AutoPtr<ILooper> mServiceLooper;
    AutoPtr<IServiceHandler> mServiceHandler;
    String mName;
    Boolean mRedelivery;

};

} // namespace App
} // namespace Droid
} // namespace Elastos
