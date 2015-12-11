
#ifndef __ELASTOS_DROID_SERVER_AM_COMPATMODEPACKAGES_H__
#define __ELASTOS_DROID_SERVER_AM_COMPATMODEPACKAGES_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/server/am/CActivityManagerService.h"
#include <Elastos.Droid.Core.h>
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;
using Elastos::Core::IInteger32;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {


class CompatModePackages : public Object
{
public:
    class MyHandler : public HandlerBase
    {
    public:
        MyHandler(
            /* [in] */ CompatModePackages* host)
            : mHost(host)
        {}

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        CompatModePackages* mHost;
    };

    CompatModePackages(
        /* [in] */ CActivityManagerService* service,
        /* [in] */ IFile* systemDir);

    ~CompatModePackages();

    AutoPtr<HashMap<String, AutoPtr<IInteger32> > > GetPackages();

    CARAPI_(void) HandlePackageAddedLocked(
        /* [in] */ const String& packageName,
        /* [in] */ Boolean updated);

    CARAPI_(AutoPtr<ICompatibilityInfo>) CompatibilityInfoForPackageLocked(
        /* [in] */ IApplicationInfo* ai);

    CARAPI_(Int32) ComputeCompatModeLocked(
        /* [in] */ IApplicationInfo* ai);

    CARAPI_(Boolean) GetFrontActivityAskCompatModeLocked();

    CARAPI_(Boolean) GetPackageAskCompatModeLocked(
        /* [in] */ const String& packageName);

    CARAPI_(void) SetFrontActivityAskCompatModeLocked(
        /* [in] */ Boolean ask);

    CARAPI_(void) SetPackageAskCompatModeLocked(
        /* [in] */ const String& packageName,
        /* [in] */ Boolean ask);

    CARAPI_(Int32) GetFrontActivityScreenCompatModeLocked();

    CARAPI_(void) SetFrontActivityScreenCompatModeLocked(
        /* [in] */ Int32 mode);

    CARAPI_(Int32) GetPackageScreenCompatModeLocked(
        /* [in] */ const String& packageName);

    CARAPI_(void) SetPackageScreenCompatModeLocked(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 mode);

    CARAPI_(void) SaveCompatModes();

private:
    CARAPI_(Int32) GetPackageFlags(
        /* [in] */ const String& packageName);

    CARAPI_(void) SetPackageScreenCompatModeLocked(
        /* [in] */ IApplicationInfo* ai,
        /* [in] */ Int32 mode);

    CARAPI_(void) HandleMsgWrite();

private:
    const String TAG;
    const Boolean DEBUG_CONFIGURATION;

    CActivityManagerService* mService;
    AutoPtr<IAtomicFile> mFile;

    // Compatibility state: no longer ask user to select the mode.
    static const Int32 COMPAT_FLAG_DONT_ASK = 1<<0;
    // Compatibility state: compatibility mode is enabled.
    static const Int32 COMPAT_FLAG_ENABLED = 1<<1;

    AutoPtr<HashMap<String, AutoPtr<IInteger32> > > mPackages;

    static const Int32 MSG_WRITE;

    AutoPtr<IHandler> mHandler;
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_AM_COMPATMODEPACKAGES_H__
