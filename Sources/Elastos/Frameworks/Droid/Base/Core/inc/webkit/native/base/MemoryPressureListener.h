
#ifndef __MEMORYPRESSURELISTENER_H__
#define __MEMORYPRESSURELISTENER_H__

// import android.app.Activity;
// import android.content.ComponentCallbacks2;
// import android.content.Context;
// import android.content.res.Configuration;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Base {

/**
 * This is an internal implementation of the C++ counterpart.
 * It registers a ComponentCallbacks2 with the system, and dispatches into
 * native for levels that are considered actionable.
 */
class MemoryPressureListener
{
private:
    class InnerComponentCallbacks2
        : public Object
        , public IComponentCallbacks2
    {
    public:
        InnerComponentCallbacks2(
            /* [in] */ MemoryPressureListener* owner);

        //@Override
        CARAPI OnTrimMemory(
            /* [in] */ Int32 level);

        //@Override
        CARAPI OnLowMemory();

        //@Override
        CARAPI OnConfigurationChanged(
            /* [in] */ IConfiguration* configuration);

    private:
        MemoryPressureListener* mOwner;
    };

public:
    /**
     * Used by applications to simulate a memory pressure signal. By throwing certain intent
     * actions.
     */
    static CARAPI_(Boolean) HandleDebugIntent(
        /* [in] */ IActivity* activity,
        /* [in] */ String action);

    static CARAPI_(void) MaybeNotifyMemoryPresure(
        /* [in] */ Int32 level);

private:
    /**
     * Sending an intent with this action to Chrome will cause it to issue a call to onLowMemory
     * thus simulating a low memory situations.
     */
    static const String ACTION_LOW_MEMORY;

    /**
     * Sending an intent with this action to Chrome will cause it to issue a call to onTrimMemory
     * thus simulating a low memory situations.
     */
    static const String ACTION_TRIM_MEMORY;

    /**
     * Sending an intent with this action to Chrome will cause it to issue a call to onTrimMemory
     * with notification level TRIM_MEMORY_RUNNING_CRITICAL thus simulating a low memory situation
     */
    static const String ACTION_TRIM_MEMORY_RUNNING_CRITICAL;

    /**
     * Sending an intent with this action to Chrome will cause it to issue a call to onTrimMemory
     * with notification level TRIM_MEMORY_MODERATE thus simulating a low memory situation
     */
    static const String ACTION_TRIM_MEMORY_MODERATE;

private:
    //@CalledByNative
    static CARAPI_(void) RegisterSystemCallback(
        /* [in] */ IContex*t context);

    static CARAPI_(void) SimulateLowMemoryPressureSignal(
        /* [in] */ IActivity* activity);

    static CARAPI_(void) SimulateTrimMemoryPressureSignal(
        /* [in] */ IActivity* activity,
        /* [in] */ Int32 level);

    static CARAPI_(void) NativeOnMemoryPressure(
        /* [in] */ Int32 memoryPressureType);
};

} // namespace Base
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__MEMORYPRESSURELISTENER_H__