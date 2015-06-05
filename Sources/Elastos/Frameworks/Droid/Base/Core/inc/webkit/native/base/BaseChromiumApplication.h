
#ifndef __BASECHROMIUMAPPLICATION_H__
#define __BASECHROMIUMAPPLICATION_H__

// import android.app.Activity;
// import android.app.Application;
// import android.os.Bundle;
// import android.view.Window;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Base {

/**
 * Basic application functionality that should be shared among all browser applications.
 */
class BaseChromiumApplication
    : public Object
    , public IApplication
{
public:
    /**
     * Interface to be implemented by listeners for window focus events.
     */
    class WindowFocusChangedListener
    {
    public:
        /**
         * Called when the window focus changes for {@code activity}.
         * @param activity The {@link Activity} that has a window focus changed event.
         * @param hasFocus Whether or not {@code activity} gained or lost focus.
         */
        CARAPI_(void) OnWindowFocusChanged(
            /* [in] */ IActivity* activity,
            /* [in] */ Boolean hasFocus);
    };

private:
    class InnerActivityLifecycleCallbacks
        : public Object
        , public IActivityLifecycleCallbacks
    {
    private:
        class InnerWindowCallbackWrapper : public WindowCallbackWrapper
        {
        public:
            InnerWindowCallbackWrapper(
                /* [in] */ InnerActivityLifecycleCallbacks* owner);

            //@Override
            CARAPI OnWindowFocusChanged(
                /* [in] */ Boolean hasFocus);

        private:
            InnerActivityLifecycleCallbacks* mOwner;
        };

    public:
        InnerActivityLifecycleCallbacks(
            /* [in] */ BaseChromiumApplication* owner);

        //@Override
        CARAPI OnActivityCreated(
            /* [in] */ IActivity* activity,
            /* [in] */ IBundle* savedInstanceState);

        //@Override
        CARAPI OnActivityDestroyed(
            /* [in] */ IActivity* activity);

        //@Override
        CARAPI OnActivityPaused(
            /* [in] */ IActivity* activity);

        //@Override
        CARAPI OnActivityResumed(
            /* [in] */ IActivity* activity);

        //@Override
        CARAPI OnActivitySaveInstanceState(
            /* [in] */ IActivity* activity,
            /* [in] */ IBundle* outState);

        //@Override
        CARAPI OnActivityStarted(
            /* [in] */ IActivity* activity);

        //@Override
        CARAPI OnActivityStopped(
            /* [in] */ IActivity* activity);

    private:
        BaseChromiumApplication* mOwner;
    };

public:
    //@Override
    CARAPI OnCreate();

    /**
     * Registers a listener to receive window focus updates on activities in this application.
     * @param listener Listener to receive window focus events.
     */
    CARAPI_(void) RegisterWindowFocusChangedListener(
        /* [in] */ WindowFocusChangedListener* listener);

    /**
     * Unregisters a listener from receiving window focus updates on activities in this application.
     * @param listener Listener that doesn't want to receive window focus events.
     */
    CARAPI_(void) UnregisterWindowFocusChangedListener(
        /* [in] */ WindowFocusChangedListener* listener);

private:
    ObserverList<WindowFocusChangedListener> mWindowFocusListeners =
            new ObserverList<WindowFocusChangedListener>();
};

} // namespace Base
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__BASECHROMIUMAPPLICATION_H__
