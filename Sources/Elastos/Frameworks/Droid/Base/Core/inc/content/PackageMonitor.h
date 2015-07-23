
#ifndef __PACKAGEMONITOR_H__
#define __PACKAGEMONITOR_H__

#include "BroadcastReceiver.h"

#include <ext/frameworkext.h>
#include <elastos/utility/etl/HashSet.h>

using Elastos::Utility::Etl::HashSet;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IPendingResult;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::IHandlerThread;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::IProcess;
using Elastos::Droid::Os::IUserHandle;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Content {

/**
 * Helper class for monitoring the state of packages: adding, removing,
 * updating, and disappearing and reappearing on the SD card.
 */
class PackageMonitor
    : public BroadcastReceiver
    , public IPackageMonitor
{
public:
    PackageMonitor();

    ~PackageMonitor();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetInterfaceID(
        /* [in] */ IInterface* object,
        /* [out] */ InterfaceID* iid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    IBROADCASTRECEIVER_METHODS_DECL();

    CARAPI ToString(
        /* [out] */ String* info)
    {
        VALIDATE_NOT_NULL(info);
        *info = String("PackageMonitor:");
        (*info).AppendFormat("%p", this);
        return NOERROR;
    }

    CARAPI Register(
        /* [in] */ IContext* context,
        /* [in] */ ILooper* thread,
        /* [in] */ Boolean externalStorage);

    CARAPI Register(
        /* [in] */ IContext* context,
        /* [in] */ ILooper* thread,
        /* [in] */ IUserHandle* user,
        /* [in] */ Boolean externalStorage);

    CARAPI GetRegisteredHandler(
        /* [out] */ IHandler** handler);

    CARAPI Unregister();

    virtual CARAPI OnBeginPackageChanges();

    /**
     * Called when a package is really added (and not replaced).
     */
    virtual CARAPI OnPackageAdded(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid);

    /**
     * Called when a package is really removed (and not replaced).
     */
    virtual CARAPI OnPackageRemoved(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid);

    /**
     * Called when a package is really removed (and not replaced) for
     * all users on the device.
     */
    virtual CARAPI OnPackageRemovedAllUsers(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid);

    virtual CARAPI OnPackageUpdateStarted(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid);

    virtual CARAPI OnPackageUpdateFinished(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid);

    virtual CARAPI OnPackageChanged(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid,
        /* [in] */ ArrayOf<String>* components);

    virtual CARAPI OnHandleForceStop(
        /* [in] */ IIntent* intent,
        /* [in] */ ArrayOf<String>* packages,
        /* [in] */ Int32 uid,
        /* [in] */ Boolean doit,
        /* [out] */ Boolean* result);

    virtual CARAPI OnHandleUserStop(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 userHandle);

    virtual CARAPI OnUidRemoved(
        /* [in] */ Int32 uid);

    virtual CARAPI OnPackagesAvailable(
        /* [in] */ ArrayOf<String>* packages);

    virtual CARAPI OnPackagesUnavailable(
        /* [in] */ ArrayOf<String>* packages);

    /**
     * Called when a package disappears for any reason.
     */
    virtual CARAPI OnPackageDisappeared(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 reason);

    /**
     * Called when a package appears for any reason.
     */
    virtual CARAPI OnPackageAppeared(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 reason);

    virtual CARAPI OnPackageModified(
        /* [in] */ const String& packageName);

    virtual CARAPI DidSomePackagesChange(
        /* [out] */ Boolean* isChanged);

    CARAPI IsPackageAppearing(
        /* [in] */ const String& packageName,
        /* [out] */ Int32* result);

    CARAPI AnyPackagesAppearing(
        /* [out] */ Boolean* result);

    CARAPI IsPackageDisappearing(
        /* [in] */ const String& packageName,
        /* [out] */ Int32* result);

    CARAPI AnyPackagesDisappearing(
        /* [out] */ Boolean* result);

    CARAPI IsPackageModified(
        /* [in] */ const String& packageName,
        /* [out] */ Boolean* isModified);

    CARAPI OnSomePackagesChanged();

    CARAPI OnFinishPackageChanges();

    CARAPI GetChangingUserId(
        /* [out] */ Int32* userId);

private:
    //not yet implemented
    CARAPI IsPackageUpdating(
        /* [in] */ const String& packageName,
        /* [out] */ Boolean* result);

    CARAPI GetPackageName(
        /* [in] */ IIntent* intent,
        /* [out] */ String* pkgName);

private:
    static AutoPtr<IIntentFilter> sPackageFilt;
    static AutoPtr<IIntentFilter> sNonDataFilt;
    static AutoPtr<IIntentFilter> sExternalFilt;
    static Object sLock;
    static AutoPtr<IHandlerThread> sBackgroundThread;
    static AutoPtr<IHandler> sBackgroundHandler;

private:
    HashSet<String> mUpdatingPackages;
    AutoPtr<IContext> mRegisteredContext;
    AutoPtr<IHandler> mRegisteredHandler;
    AutoPtr<ArrayOf<String> > mDisappearingPackages;
    AutoPtr<ArrayOf<String> > mAppearingPackages;
    AutoPtr<ArrayOf<String> > mModifiedPackages;
    Int32 mChangeType;
    Int32 mChangeUserId;
    Boolean mSomePackagesChanged;
    AutoPtr<ArrayOf<String> > mTempArray;
    Object mUpdatingPackagesLock;

};

}
}
}
}

#endif // __PACKAGEMONITOR_H__
