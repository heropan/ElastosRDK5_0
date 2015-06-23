
#ifndef __CAPPLICATION_H__
#define __CAPPLICATION_H__

#include "ext/frameworkext.h"
#include "_CApplication.h"
#include "app/LoadedPkg.h"
#include "content/ContextWrapper.h"

#include <elastos/utility/etl/List.h>

using Elastos::Core::Mutex;
using Elastos::Utility::Etl::List;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::App::IActivity;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::ContextWrapper;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IComponentCallbacks;
using Elastos::Droid::Content::IComponentCallbacks2;
using Elastos::Droid::Content::ISharedPreferences;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentSender;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::IServiceConnection;

namespace Elastos {
namespace Droid {
namespace App {

class _CApplicationInner
    : public ElRefBase
    , public IObject
    , public IWeakReferenceSource
    , public IApplication
    , public IComponentCallbacks2
    , public ContextWrapper
{
public:
    CAR_INTERFACE_DECL()

    ICONTEXT_METHODS_DECL()

    CARAPI Aggregate(
        /* [in] */ AggrType aggrType,
        /* [in] */ PInterface pObject);

    CARAPI GetDomain(
        /* [out] */ PInterface *ppObject);

    CARAPI GetClassID(
        /* [out] */ ClassID *pCLSID);

    CARAPI Equals(
        /* [in] */ IInterface* other,
        /* [out] */ Boolean * result);

    CARAPI GetHashCode(
        /* [out] */ Int32* hash);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI GetWeakReference(
        /* [out] */ IWeakReference** weakReference);

    CARAPI GetBaseContext(
        /* [out] */ IContext** context);
};

class CApplication : public _CApplicationInner
{
public:
    CARAPI constructor();

    CARAPI Initialize();

    /**
     * Called when the application is starting, before any other application
     * objects have been created.  Implementations should be as quick as
     * possible (for example using lazy initialization of state) since the time
     * spent in this function directly impacts the performance of starting the
     * first activity, service, or receiver in a process.
     * If you override this method, be sure to call super.onCreate().
     */
    CARAPI OnCreate();

    /**
     * This method is for use in emulated process environments.  It will
     * never be called on a production Android device, where processes are
     * removed by simply killing them; no user code (including this callback)
     * is executed when doing so.
     */
    CARAPI OnTerminate();

    CARAPI OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    CARAPI OnLowMemory();

    CARAPI OnTrimMemory(
        /* [in] */ Int32 level);

    CARAPI RegisterComponentCallbacks(
        /* [in] */ IComponentCallbacks* componentCallback);

    CARAPI UnregisterComponentCallbacks(
        /* [in] */ IComponentCallbacks* componentCallback);

    CARAPI RegisterActivityLifecycleCallbacks(
        /* [in] */ IActivityLifecycleCallbacks* cb);

    CARAPI UnregisterActivityLifecycleCallbacks(
        /* [in] */ IActivityLifecycleCallbacks* cb);

    /**
     * @hide
     */
    /* package */
    CARAPI Attach(
        /* [in] */ IContext* context);

    /* package */
    CARAPI DispatchActivityCreated(
        /* [in] */ IActivity* activity,
        /* [in] */ IBundle* b);

    /* package */
    CARAPI DispatchActivityStarted(
        /* [in] */ IActivity* activity);

    /* package */
    CARAPI DispatchActivityResumed(
        /* [in] */ IActivity* activity);

    /* package */
    CARAPI DispatchActivityPaused(
        /* [in] */ IActivity* activity);

    /* package */
    CARAPI DispatchActivityStopped(
        /* [in] */ IActivity* activity);

    /* package */
    CARAPI DispatchActivitySaveInstanceState(
        /* [in] */ IActivity* activity,
        /* [in] */ IBundle* b);

    /* package */
    CARAPI DispatchActivityDestroyed(
        /* [in] */ IActivity* activity);

private:
    CARAPI_(AutoPtr< ArrayOf<IComponentCallbacks*> >) CollectComponentCallbacks();

    CARAPI_(AutoPtr< ArrayOf<IActivityLifecycleCallbacks*> >) CollectActivityLifecycleCallbacks();

public:
    AutoPtr<LoadedPkg> mLoadedPkg;

private:
    List< AutoPtr<IComponentCallbacks> > mComponentCallbacks;
    Mutex mComponentCallbacksLock;
    List< AutoPtr<IActivityLifecycleCallbacks> > mActivityLifecycleCallbacks;
    Mutex mActivityLifecycleCallbacksLock;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__CAPPLICATION_H__
