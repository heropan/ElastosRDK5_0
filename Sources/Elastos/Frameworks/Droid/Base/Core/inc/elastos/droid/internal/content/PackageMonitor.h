
#ifndef __ELASTOS_DROID_INTERNAL_CONTENT_PACKAGEMONITOR_H__
#define __ELASTOS_DROID_INTERNAL_CONTENT_PACKAGEMONITOR_H__

#include "elastos/droid/content/BroadcastReceiver.h"
#include "elastos/droid/ext/frameworkext.h"
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
using Elastos::Droid::Os::IUserHandle;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Content {

/**
 * Helper class for monitoring the state of packages: adding, removing,
 * updating, and disappearing and reappearing on the SD card.
 */
class ECO_PUBLIC PackageMonitor
    : public BroadcastReceiver
    , public IPackageMonitor
{
public:
    CAR_INTERFACE_DECL()

    PackageMonitor();

    virtual ~PackageMonitor();

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

    CARAPI OnBeginPackageChanges();

    /**
     * Called when a package is really added (and not replaced).
     */
    CARAPI OnPackageAdded(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid);

    /**
     * Called when a package is really removed (and not replaced).
     */
    CARAPI OnPackageRemoved(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid);

    /**
     * Called when a package is really removed (and not replaced) for
     * all users on the device.
     */
    CARAPI OnPackageRemovedAllUsers(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid);

    CARAPI OnPackageUpdateStarted(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid);

    CARAPI OnPackageUpdateFinished(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid);

    CARAPI OnPackageChanged(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid,
        /* [in] */ ArrayOf<String>* components,
        /* [out] */ Boolean* result);

    CARAPI OnHandleForceStop(
        /* [in] */ IIntent* intent,
        /* [in] */ ArrayOf<String>* packages,
        /* [in] */ Int32 uid,
        /* [in] */ Boolean doit,
        /* [out] */ Boolean* result);

    CARAPI OnHandleUserStop(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 userHandle);

    CARAPI OnUidRemoved(
        /* [in] */ Int32 uid);

    CARAPI OnPackagesAvailable(
        /* [in] */ ArrayOf<String>* packages);

    CARAPI OnPackagesUnavailable(
        /* [in] */ ArrayOf<String>* packages);

    /**
     * Called when a package disappears for any reason.
     */
    CARAPI OnPackageDisappeared(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 reason);

    /**
     * Called when a package appears for any reason.
     */
    CARAPI OnPackageAppeared(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 reason);

    CARAPI OnPackageModified(
        /* [in] */ const String& packageName);

    CARAPI DidSomePackagesChange(
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

    CARAPI IsReplacing(
        /* [out] */ Boolean* result);

    CARAPI IsPackageModified(
        /* [in] */ const String& packageName,
        /* [out] */ Boolean* isModified);

    CARAPI OnSomePackagesChanged();

    CARAPI OnFinishPackageChanges();

    CARAPI GetChangingUserId(
        /* [out] */ Int32* userId);

    CARAPI OnReceive(
        /* [in] */ IContext* context,
        /* [in] */ IIntent* intent);

    CARAPI ToString(
        /* [out] */ String* info);

private:
    //not yet implemented
    ECO_LOCAL CARAPI_(Boolean) IsPackageUpdating(
        /* [in] */ const String& packageName);

    ECO_LOCAL CARAPI_(String) GetPackageName(
        /* [in] */ IIntent* intent);

private:
    ECO_LOCAL static AutoPtr<IIntentFilter> sPackageFilt;
    ECO_LOCAL static AutoPtr<IIntentFilter> sNonDataFilt;
    ECO_LOCAL static AutoPtr<IIntentFilter> sExternalFilt;

private:
    ECO_LOCAL HashSet<String> mUpdatingPackages;
    ECO_LOCAL AutoPtr<IContext> mRegisteredContext;
    ECO_LOCAL AutoPtr<IHandler> mRegisteredHandler;
    ECO_LOCAL AutoPtr<ArrayOf<String> > mDisappearingPackages;
    ECO_LOCAL AutoPtr<ArrayOf<String> > mAppearingPackages;
    ECO_LOCAL AutoPtr<ArrayOf<String> > mModifiedPackages;
    ECO_LOCAL Int32 mChangeType;
    ECO_LOCAL Int32 mChangeUserId;
    ECO_LOCAL Boolean mSomePackagesChanged;
    ECO_LOCAL AutoPtr<ArrayOf<String> > mTempArray;
    ECO_LOCAL Object mUpdatingPackagesLock;

};

}
}
}
}

#endif // __ELASTOS_DROID_INTERNAL_CONTENT_PACKAGEMONITOR_H__
