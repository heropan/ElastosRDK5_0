
#ifndef __ELASTOS_DROID_SERVER_CSYSTEMSERVER_H__
#define __ELASTOS_DROID_SERVER_CSYSTEMSERVER_H__

#include "_Elastos_Droid_Server_CSystemServer.h"
#include <elastos/core/Singleton.h>
#include <elastos/core/Thread.h>
#include "elastos/droid/os/Runnable.h"
// #include "CInputMethodManagerService.h"
// #include "CWallpaperManagerService.h"
// #include "CStatusBarManagerService.h"
// #include "usb/CUsbService.h"
// #include "CMountService.h"
// #include "CNetworkManagementService.h"
// #include "elastos/droid/net/CNetworkStatsService.h"
// #include "elastos/droid/net/CNetworkPolicyManagerService.h"
// #include "CAppWidgetService.h"
// #include "CConnectivityService.h"
// #include "CRecognitionManagerService.h"
// #include "CTextServicesManagerService.h"
// #include "CCountryDetectorService.h"
// #include "NetworkTimeUpdateService.h"
// #include "CCommonTimeManagementService.h"
// #include "TwilightService.h"
// #include "CUiModeManagerService.h"
// #include "CBatteryService.h"
// #include "CThrottleService.h"
// #include "CLocationManagerService.h"

using Elastos::Core::Thread;
using Elastos::Core::IThread;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Content::IContext;
// using Elastos::Droid::Content::IContentResolver;
// using Elastos::Droid::Server::Usb::CUsbService;
// using Elastos::Droid::Server::CRecognitionManagerService;
// using Elastos::Droid::Server::CTextServicesManagerService;
// using Elastos::Droid::Server::CCountryDetectorService;
// using Elastos::Droid::Server::NetworkTimeUpdateService;
// using Elastos::Droid::Server::CCommonTimeManagementService;
// using Elastos::Droid::Server::TwilightService;
// using Elastos::Droid::Server::CUiModeManagerService;
// using Elastos::Droid::Server::CBatteryService;
// using Elastos::Droid::Server::Net::CNetworkStatsService;
// using Elastos::Droid::Server::Net::CNetworkPolicyManagerService;
// using Elastos::Droid::Server::CThrottleService;
// using Elastos::Droid::Server::Location::CLocationManagerService;

namespace Elastos {
namespace Droid {
namespace Server {

class ServerThread
    : public Thread
{
private:
    class SystemReadyRunnable
        : public Runnable
    {
    public:
        SystemReadyRunnable();

        CARAPI Run();

    public:
        // CInputMethodManagerService* mImm;
        // CWallpaperManagerService* mWallpaper;
        // CNetworkManagementService* mNetworkManage;
        // CNetworkStatsService* mNetworkStats;
        // CNetworkPolicyManagerService* mNetworkPolicy;
        // CConnectivityService* mConnectivity;
        // CUsbService* mUsb;
        // CThrottleService* mThrottle;
        // CMountService* mMountService;
        // CBatteryService* mBattery;
        // CAppWidgetService* mAppWidget;
        // CStatusBarManagerService* mStatusBar;
        // CRecognitionManagerService* mRecognition;
        // CTextServicesManagerService* mTextServiceManagerService;
        // CCountryDetectorService* mCountryDetector;
        // NetworkTimeUpdateService* mNetworkTimeUpdater;
        // CCommonTimeManagementService* mCommonTimeMgmtService;
        // TwilightService* mTwilight;
        // CUiModeManagerService* mUiMode;
        // CLocationManagerService* mLocationF;
        // AutoPtr<IContext> mContext;
        Boolean mHeadless;
    };

public:
    ServerThread();

    static void StartSystemUi(
        /* [in] */ IContext* context);

    virtual CARAPI Run();

private:
    static const String ENCRYPTING_STATE;// = "trigger_restart_min_framework";
    static const String ENCRYPTED_STATE;// = "1";

    // AutoPtr<IContentResolver> mContentResolver;
    const Boolean mDeviceHasYpbpr;
};

CarClass(CSystemServer)
    , public Singleton
    , public ISystemServer
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    CARAPI Init1(
        /* [in] */ const ArrayOf<String>& args);

    CARAPI Init2();

    CARAPI Main(
        /* [in] */ const ArrayOf<String>& args);

public:
    static const Int32 FACTORY_TEST_OFF = 0;
    static const Int32 FACTORY_TEST_LOW_LEVEL = 1;
    static const Int32 FACTORY_TEST_HIGH_LEVEL = 2;

private:
    AutoPtr<IThread> mThread;
};

} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__CSYSTEMSERVER_H__
