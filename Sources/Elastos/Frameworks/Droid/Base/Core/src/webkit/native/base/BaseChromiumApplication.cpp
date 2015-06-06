
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Base {

//============================================================================================
//   BaseChromiumApplication::InnerActivityLifecycleCallbacks::InnerWindowCallbackWrapper
//============================================================================================

BaseChromiumApplication::InnerActivityLifecycleCallbacks::InnerWindowCallbackWrapper::InnerWindowCallbackWrapper(
    /* [in] */ InnerActivityLifecycleCallbacks* owner)
    : mOwner(owner)
{
}

//@Override
ECode BaseChromiumApplication::InnerActivityLifecycleCallbacks::InnerWindowCallbackWrapper::OnWindowFocusChanged(
    /* [in] */ Boolean hasFocus)
{
    WindowCallbackWrapper::OnWindowFocusChanged(hasFocus);

    for (WindowFocusChangedListener listener : mWindowFocusListeners) {
        listener.onWindowFocusChanged(activity, hasFocus);
    }

    return NOERROR;
}

//===============================================================
//   BaseChromiumApplication::InnerActivityLifecycleCallbacks
//===============================================================

BaseChromiumApplication::InnerActivityLifecycleCallbacks::InnerActivityLifecycleCallbacks(
    /* [in] */ BaseChromiumApplication* owner)
    : mOwner(owner)
{
}

//@Override
ECode BaseChromiumApplication::InnerActivityLifecycleCallbacks::OnActivityCreated(
    /* [in] */ IActivity* activity,
    /* [in] */ IBundle* savedInstanceState)
{
    AutoPtr<IWindow> window;
    activity->GetWindow((IWindow**)&window);
    AutoPtr<IWindowCallback> callback;
    window->GetCallback((IWindowCallback**)&callback);

    AutoPtr<IWindowCallback> cb = new InnerWindowCallbackWrapper(callback);
    window->SetCallback(cb);

    return NOERROR;
}

//@Override
ECode BaseChromiumApplication::InnerActivityLifecycleCallbacks::OnActivityDestroyed(
    /* [in] */ Activity activity)
{
    AutoPtr<IWindow> window;
    activity->GetWindow((IWindow**)&window);
    AutoPtr<IWindowCallback> callback;
    window->GetCallback((IWindowCallback**)&callback);
    assert(callback->Probe(EIID_WindowCallbackWrapper));

    return NOERROR;
}

ECode BaseChromiumApplication::InnerActivityLifecycleCallbacks::OnActivityPaused(
    /* [in] */ IActivity* activity)
{
    AutoPtr<IWindow> window;
    activity->GetWindow((IWindow**)&window);
    AutoPtr<IWindowCallback> callback;
    window->GetCallback((IWindowCallback**)&callback);
    assert(callback->Probe(EIID_WindowCallbackWrapper));

    return NOERROR;
}

ECode BaseChromiumApplication::InnerActivityLifecycleCallbacks::OnActivityResumed(
    /* [in] */ IActivity* activity)
{
    AutoPtr<IWindow> window;
    activity->GetWindow((IWindow**)&window);
    AutoPtr<IWindowCallback> callback;
    window->GetCallback((IWindowCallback**)&callback);
    assert(callback->Probe(EIID_WindowCallbackWrapper));

    return NOERROR;
}

ECode BaseChromiumApplication::InnerActivityLifecycleCallbacks::OnActivitySaveInstanceState(
    /* [in] */ IActivity* activity,
    /* [in] */ IBundle* outState)
{
    AutoPtr<IWindow> window;
    activity->GetWindow((IWindow**)&window);
    AutoPtr<IWindowCallback> callback;
    window->GetCallback((IWindowCallback**)&callback);
    assert(callback->Probe(EIID_WindowCallbackWrapper));

    return NOERROR;
}

//@Override
ECode BaseChromiumApplication::InnerActivityLifecycleCallbacks::OnActivityStarted(
    /* [in] */ IActivity* activity)
{
    AutoPtr<IWindow> window;
    activity->GetWindow((IWindow**)&window);
    AutoPtr<IWindowCallback> callback;
    window->GetCallback((IWindowCallback**)&callback);
    assert(callback->Probe(EIID_WindowCallbackWrapper));

    return NOERROR;
}

//@Override
ECode BaseChromiumApplication::InnerActivityLifecycleCallbacks::OnActivityStopped(
    /* [in] */ IActivity* activity)
{
    AutoPtr<IWindow> window;
    activity->GetWindow((IWindow**)&window);
    AutoPtr<IWindowCallback> callback;
    window->GetCallback((IWindowCallback**)&callback);
    assert(callback->Probe(EIID_WindowCallbackWrapper));

    return NOERROR;
}

//===============================================================
//                   BaseChromiumApplication
//===============================================================

//@Override
ECode BaseChromiumApplication::OnCreate()
{
    super.onCreate();
    ApplicationStatus::Initialize(this);

    AutoPtr<IActivityLifecycleCallbacks> callback = new InnerActivityLifecycleCallbacks(this);
    RegisterActivityLifecycleCallbacks(callback);

    return NOERROR;
}

/**
 * Registers a listener to receive window focus updates on activities in this application.
 * @param listener Listener to receive window focus events.
 */
void BaseChromiumApplication::RegisterWindowFocusChangedListener(
    /* [in] */ WindowFocusChangedListener* listener)
{
    mWindowFocusListeners.addObserver(listener);
}

/**
 * Unregisters a listener from receiving window focus updates on activities in this application.
 * @param listener Listener that doesn't want to receive window focus events.
 */
void BaseChromiumApplication::UnregisterWindowFocusChangedListener(
    /* [in] */ WindowFocusChangedListener* listener)
{
    mWindowFocusListeners.removeObserver(listener);
}

} // namespace Base
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
