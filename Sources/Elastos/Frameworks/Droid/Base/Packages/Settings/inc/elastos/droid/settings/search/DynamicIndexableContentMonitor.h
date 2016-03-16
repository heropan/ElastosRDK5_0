
#ifndef __ELASTOS_DROID_SETTINGS_SEARCH_DYNAMICINDEXABLECONTENTMONITOR_H__
#define __ELASTOS_DROID_SETTINGS_SEARCH_DYNAMICINDEXABLECONTENTMONITOR_H__

#include "Elastos.Droid.Hardware.h"
#include "Elastos.CoreLibrary.Utility.h"
#include "elastos/droid/internal/content/PackageMonitor.h"
#include "elastos/droid/database/ContentObserver.h"
#include "elastos/droid/os/Handler.h"
#include "_Settings.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Database::ContentObserver;
using Elastos::Droid::Database::IContentObserver;
using Elastos::Droid::Hardware::Input::IInputDeviceListener;
using Elastos::Droid::Internal::Content::PackageMonitor;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Os::Handler;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::IMessage;
using Elastos::Utility::IList;

namespace Elastos {
namespace Droid {
namespace Settings {
namespace Search {

class DynamicIndexableContentMonitor
    : public PackageMonitor
    , public IInputDeviceListener
{
private:
    class MyHandler
        : public Handler
    {
    public:
        MyHandler(
            /* [in] */ DynamicIndexableContentMonitor* host);

        ~MyHandler();

        //@Override
        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        DynamicIndexableContentMonitor* mHost;
    };

    class UserDictionaryContentObserver
        : public ContentObserver
    {
    public:
        UserDictionaryContentObserver(
            /* [in] */ IHandler* handler,
            /* [in] */ DynamicIndexableContentMonitor* host);

        ~UserDictionaryContentObserver();

        //@Override
        CARAPI OnChange(
            /* [in] */ Boolean selfChange,
            /* [in] */ IUri* uri);

    private:
        DynamicIndexableContentMonitor* mHost;
    };

public:
    CAR_INTERFACE_DECL();

    DynamicIndexableContentMonitor();

    ~DynamicIndexableContentMonitor();

    CARAPI Register(
        /* [in] */ IContext* context);

    CARAPI Unregister();

    // Covers installed, appeared external storage with the package, upgraded.
    //@Override
    CARAPI OnPackageAppeared(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid);

    // Covers uninstalled, removed external storage with the package.
    //@Override
    CARAPI OnPackageDisappeared(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid);

    // Covers enabled, disabled.
    //@Override
    CARAPI OnPackageModified(
        /* [in] */ const String& packageName);

    //@Override
    CARAPI OnInputDeviceAdded(
        /* [in] */ Int32 deviceId);

    //@Override
    CARAPI OnInputDeviceRemoved(
        /* [in] */ Int32 deviceId);

    //@Override
    CARAPI OnInputDeviceChanged(
        /* [in] */ Int32 deviceId);

private:
    static CARAPI_(AutoPtr<IIntent>) GetAccessibilityServiceIntent(
        /* [in] */ const String& packageName);

    static CARAPI_(AutoPtr<IIntent>) GetPrintServiceIntent(
        /* [in] */ const String& packageName);

    static CARAPI_(AutoPtr<IIntent>) GetIMEServiceIntent(
        /* [in] */ const String& packageName);

    CARAPI_(void) PostMessage(
        /* [in] */ Int32 what,
        /* [in] */ const String& packageName);

    CARAPI_(void) HandlePackageAvailable(
        /* [in] */ const String& packageName);

    CARAPI_(void) HandlePackageUnavailable(
        /* [in] */ const String& packageName);

private:
    static const Int64 DELAY_PROCESS_PACKAGE_CHANGE;

    static const Int32 MSG_PACKAGE_AVAILABLE = 1;
    static const Int32 MSG_PACKAGE_UNAVAILABLE = 2;

    AutoPtr<IList> mAccessibilityServices;
    AutoPtr<IList> mPrintServices;
    AutoPtr<IList> mImeServices;

    AutoPtr<IHandler> mHandler;

    AutoPtr<IContentObserver> mUserDictionaryContentObserver;

    AutoPtr<IContext> mContext;
    Boolean mHasFeaturePrinting;
    Boolean mHasFeatureIme;
};

} // namespace Search
} // namespace Settings
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SETTINGS_SEARCH_DYNAMICINDEXABLECONTENTMONITOR_H__
