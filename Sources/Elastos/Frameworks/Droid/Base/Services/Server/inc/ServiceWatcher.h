#ifndef __SERVICEWATCHER_H__
#define __SERVICEWATCHER_H__

#include <ext/frameworkext.h>
#include <elastos/core/Math.h>
#include <elastos/utility/etl/List.h>
#include <Elastos.Droid.Core.h>
#include <content/PackageMonitor.h>
#include <os/UserHandle.h>

using Elastos::Utility::Etl::List;
using Elastos::Core::IRunnable;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::IServiceConnection;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Internal::Content::PackageMonitor;
using Elastos::Droid::Content::Pm::IResolveInfo;
using Elastos::Droid::Content::Pm::ISignature;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Pm::IPackageInfo;
using Elastos::Droid::Content::Pm::IServiceInfo;
using Elastos::Droid::Content::Pm::IPackageInfo;


namespace Elastos{
namespace Droid{
namespace Server{
namespace Location{

using Elastos::Core::Math;

class ServiceWatcher
        : public IServiceConnection
        , public ElRefBase
{
private:
    class ServiceWatcherPackageMonitor : public PackageMonitor
    {
    public:
        ServiceWatcherPackageMonitor(
                /* [in] */ ServiceWatcher* host);

        virtual CARAPI OnPackageUpdateFinished(
            /* [in] */ const String& packageName,
            /* [in] */ Int32 uid);

        virtual CARAPI OnPackageAdded(
            /* [in] */ const String& packageName,
            /* [in] */ Int32 uid);

        virtual CARAPI OnPackageRemoved(
            /* [in] */ const String& packageName,
            /* [in] */ Int32 uid);
    private:

        ServiceWatcher* mHost;
    };
public:

    CAR_INTERFACE_DECL()

    static CARAPI_(List<HashSet<AutoPtr<ISignature> > >) GetSignatureSets(
        /* [in] */ IContext* context,
        /* [in] */ List<String>* initialPackageNames);

    static CARAPI_(Boolean) IsSignatureMatch(
        /* [in] */ ArrayOf<ISignature*>* signatures,
        /* [in] */ List<HashSet<AutoPtr<ISignature> > > sigSets);

    ServiceWatcher(
        /* [in] */ IContext* context,
        /* [in] */ const String& logTag,
        /* [in] */ const String& action,
        /* [in] */ List<String>* initialPackageNames,
        /* [in] */ IRunnable* newServiceWork,
        /* [in] */ IHandler* handler,
        /* [in] */ Int32 userId);

    CARAPI_(Boolean) Start();

    CARAPI OnServiceConnected(
        /* [in] */ IComponentName* name,
        /* [in] */ IBinder* binder);

    CARAPI OnServiceDisconnected(
        /* [in] */ IComponentName* name);

    CARAPI_(String) GetBestPackageName();

    CARAPI_(Int32) GetBestVersion();

    CARAPI_(AutoPtr<IBinder>) GetBinder();

    CARAPI_(void) SwitchUser(
        /* [in] */ Int32 userId);

private:

    CARAPI_(Boolean) BindBestPackageLocked(
        /* [in] */ const String& justCheckThisPackage);

    CARAPI_(void) UnbindLocked();

    CARAPI_(void) BindToPackageLocked(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 version);

    CARAPI_(Boolean) IsSignatureMatch(
        /* [in] */ ArrayOf<ISignature*>* signatures);



public:
    const static String EXTRA_SERVICE_VERSION;

private:
    const static Boolean D = false;

    const String mTag;
    AutoPtr<IContext> mContext;
    AutoPtr<IPackageManager> mPm;
    List<HashSet<AutoPtr<ISignature> > > mSignatureSets;
    String mAction;
    AutoPtr<IRunnable> mNewServiceWork;
    AutoPtr<IHandler> mHandler;
    AutoPtr<ServiceWatcherPackageMonitor> mPackageMonitor;
    Mutex mLock;

    // all fields below synchronized on mLock
    AutoPtr<IBinder> mBinder;   // connected service
    String mPackageName;  // current best package
    Int32 mVersion;  // current best version
    Int32 mCurrentUserId;
};

} // namespace Location
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif
