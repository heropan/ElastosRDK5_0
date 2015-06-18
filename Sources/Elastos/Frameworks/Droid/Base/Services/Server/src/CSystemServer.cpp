
#include "CSystemServer.h"
#include "am/CActivityManagerService.h"
#include "am/CBatteryStatsService.h"
#include "CContentService.h"
#include "CVibratorService.h"
#include "CAlarmManagerService.h"
#include "CStatusBarManagerService.h"
#include "CNotificationManagerService.h"
#include "pm/Installer.h"
#include "pm/CPackageManagerService.h"
#include "pm/CUserManagerService.h"
#include "wm/InputMonitor.h"
#include "CPrivacySettingsManagerService.h"
#include "CClipboardService.h"
#include "CLockSettingsService.h"
#include "os/Looper.h"
#include "os/ServiceManager.h"
#include "input/CInputManagerService.h"
#include "power/CPowerManagerService.h"
#include "accounts/CAccountManagerService.h"
#include "AttributeCache.h"
#include "CNsdService.h"
#include "CWifiService.h"
#include "CEthernetService.h"
#include "CSerialService.h"
#include "CDiskStatsService.h"
#include "CUpdateLockService.h"
#include "CWifiDisplayManagerService.h"
#include "CSamplingProfilerService.h"
#include "CDropBoxManagerService.h"
#include "CDevicePolicyManagerService.h"
#include "CDeviceStorageMonitorService.h"
#include "CDisplayManagerServiceAw.h"
#include "display/CDisplayManagerService.h"
#include "search/CSearchManagerService.h"
#include "accessibility/CAccessibilityManagerService.h"
#include "CBackupManagerService.h"
#include "dreams/CDreamManagerService.h"
#include "AudioDeviceManagerObserver.h"
#include "CKortideRemoteService.h"
#include "CZigbeeService.h"

#include <elastos/Slogger.h>
#include <unistd.h>
#include <SurfaceFlinger.h>
#include <SensorService.h>
#include <cutils/properties.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>

using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::IO::CFileOutputStream;
using Elastos::IO::IFlushable;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::App::Backup::IIBackupManager;
using Elastos::Droid::App::IIWallpaperManager;
using Elastos::Droid::App::IIAlarmManager;
using Elastos::Droid::App::Admin::IIDevicePolicyManager;
using Elastos::Droid::App::Admin::EIID_IIDevicePolicyManager;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Hardware::Display::IIDisplayManager;
using Elastos::Droid::Hardware::Input::IIInputManager;
using Elastos::Droid::Hardware::IISerialManager;
using Elastos::Droid::Hardware::EIID_IISerialManager;
using Elastos::Droid::Hardware::Usb::IIUsbManager;
using Elastos::Droid::Hardware::Usb::EIID_IIUsbManager;
using Elastos::Droid::Net::IIConnectivityManager;
using Elastos::Droid::Net::Wifi::IIWifiManager;
using Elastos::Droid::Net::Wifi::P2p::IWifiP2pService;
using Elastos::Droid::Net::Wifi::P2p::CWifiP2pService;
using Elastos::Droid::Net::Wifi::P2p::IIWifiP2pManager;
using Elastos::Droid::Net::Ethernet::IIEthernetManager;
using Elastos::Droid::Net::Nsd::IINsdManager;
using Elastos::Droid::Net::Nsd::EIID_IINsdManager;
using Elastos::Droid::Net::IIThrottleManager;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::Looper;
using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::Os::IServiceManager;
using Elastos::Droid::Os::CServiceManager;
using Elastos::Droid::Os::IHandlerThread;
using Elastos::Droid::Os::CHandlerThread;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Os::ISystemProperties;
using Elastos::Droid::Os::CSystemProperties;
using Elastos::Droid::Os::IIPowerManager;
using Elastos::Droid::Os::Storage::IMountService;
using Elastos::Droid::Os::IVibratorService;
using Elastos::Droid::Os::IIUpdateLock;
using Elastos::Droid::Os::IIUserManager;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::IUserHandleHelper;
using Elastos::Droid::Os::CUserHandleHelper;
using Elastos::Droid::Internal::Os::IDropBoxManagerService;
using Elastos::Droid::View::IDisplayManagerAw;
using Elastos::Droid::View::CDisplayManagerAw;
using Elastos::Droid::View::IDispListHelper;
using Elastos::Droid::View::CDispListHelper;
using Elastos::Droid::View::IDispListDispFormat;
using Elastos::Droid::View::CDispListDispFormat;
using Elastos::Droid::View::IIWindowManager;
using Elastos::Droid::View::IDispList;
using Elastos::Droid::View::Textservice::IITextServicesManager;
using Elastos::Droid::View::Accessibility::IIAccessibilityManager;
using Elastos::Droid::Widget::Internal::ILockSettings;
using Elastos::Droid::Widget::Internal::CLockSettingsService;
using Elastos::Droid::Server::Am::CActivityManagerService;
using Elastos::Droid::Server::Am::CBatteryStatsService;
using Elastos::Droid::Server::CContentService;
using Elastos::Droid::Server::Display::CDisplayManagerService;
using Elastos::Droid::Server::CVibratorService;
using Elastos::Droid::Server::CAlarmManagerService;
using Elastos::Droid::Server::CBackupManagerService;
using Elastos::Droid::Server::CSerialService;
using Elastos::Droid::Server::CDiskStatsService;
using Elastos::Droid::Server::CUpdateLockService;
using Elastos::Droid::Server::CWifiDisplayManagerService;
using Elastos::Droid::Server::CSamplingProfilerService;
using Elastos::Droid::Server::CWifiService;
using Elastos::Droid::Server::Pm::Installer;
using Elastos::Droid::Server::Pm::CPackageManagerService;
using Elastos::Droid::Server::Pm::CUserManagerService;
using Elastos::Droid::Server::Input::CInputManagerService;
using Elastos::Droid::Server::CPrivacySettingsManagerService;
using Elastos::Droid::Server::CClipboardService;
using Elastos::Droid::Server::CDropBoxManagerService;
using Elastos::Droid::Server::Power::CPowerManagerService;
using Elastos::Droid::Server::Accounts::CAccountManagerService;
using Elastos::Droid::Server::CStatusBarManagerService;
using Elastos::Droid::Server::CDeviceStorageMonitorService;
using Elastos::Droid::Server::Search::CSearchManagerService;
using Elastos::Droid::Server::Display::IDisplayManagerServiceWindowManagerFuncs;
using Elastos::Droid::Server::Display::IDisplayManagerServiceInputManagerFuncs;
using Elastos::Droid::Server::Accessibility::CAccessibilityManagerService;
// using Elastos::Droid::Server::Dreams::IDreamService;
using Elastos::Droid::Server::Dreams::IDreamManagerService;
using Elastos::Droid::Server::Dreams::CDreamManagerService;
using Elastos::Droid::StatusBar::IIStatusBarService;
using Elastos::Droid::Accounts::IIAccountManager;
using Elastos::Droid::Location::IILocationManager;
using Elastos::Droid::Location::IICountryDetector;
using Elastos::Droid::Media::IIAudioService;
using Elastos::Droid::Media::IAudioService;
using Elastos::Droid::Media::CAudioService;
using Elastos::Droid::Media::IAudioManager;
using Elastos::Droid::Media::CAudioManager;
using Elastos::Droid::Provider::ISettingsSystem;
using Elastos::Droid::Provider::CSettingsSystem;

namespace Elastos {
namespace Droid {
namespace Server {

static const char* TAG = "CSystemServer";

const String ServerThread::ENCRYPTING_STATE("trigger_restart_min_framework");
const String ServerThread::ENCRYPTED_STATE("1");

/**
 * This class is used to kill this process when the runtime dies.
 */
class GrimReaper : public android::IBinder::DeathRecipient
{
public:
    GrimReaper() { }

    virtual void binderDied(const android::wp<android::IBinder>& who)
    {
        Slogger::I(TAG, "Grim Reaper killing system_server...");
        kill(getpid(), SIGKILL);
    }
};

ServerThread::ServerThread()
    : mDeviceHasYpbpr(FALSE)
{
    Thread::Init();
}

#include <unistd.h>
ECode ServerThread::Run()
{
    Looper::PrepareMainLooper();

    ECode ec = NOERROR;

    // String factoryTestStr = SystemProperties.get("ro.factorytest");
    // int factoryTest = "".equals(factoryTestStr) ? SystemServer.FACTORY_TEST_OFF
    //         : Integer.parseInt(factoryTestStr);
    Int32 factoryTest = CSystemServer::FACTORY_TEST_OFF;

    Slogger::I(TAG, "== BEGIN == Initializing Services, tid: %d...", gettid());
    AutoPtr<ISystemProperties> sysProp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
    String value;
    sysProp->GetEx(String("ro.config.headless"), String("0"), &value);
    Boolean headless = String("1").Equals(value);

    AutoPtr<Installer> installer;
    AutoPtr<CAccountManagerService> accountManager;
    AutoPtr<LightsService> lights;
    AutoPtr<CDisplayManagerService> display;
    AutoPtr<CBatteryService> battery;
    AutoPtr<CVibratorService> vibrator;
    AutoPtr<CAlarmManagerService> alarm;
    AutoPtr<IIPackageManager> pm;
    AutoPtr<IContext> context;
    AutoPtr<CWindowManagerService> wm;
    AutoPtr<CInputManagerService> inputManager;
    AutoPtr<CNetworkManagementService> networkManagement;
    AutoPtr<CNetworkStatsService> networkStats;
    AutoPtr<CNetworkPolicyManagerService> networkPolicy;
    AutoPtr<CConnectivityService> connectivity;
    AutoPtr<IWifiP2pService> wifiP2p;
    AutoPtr<CWifiService> wifi;
    AutoPtr<CEthernetService> ethernet;    /*  EthernetService (add by shugeLinux@gmail.com)  */
    AutoPtr<CNsdService> serviceDiscovery;
    AutoPtr<CCountryDetectorService> countryDetector;
    AutoPtr<CPowerManagerService> power;
//    AutoPtr<CDynamicPManagerService> dpm;
    AutoPtr<CMountService> mountService;
    AutoPtr<CUsbService> usb;
    AutoPtr<CThrottleService> throttle;
    AutoPtr<CSerialService> serial;
    AutoPtr<TwilightService> twilight;
    AutoPtr<CUiModeManagerService> uiMode;
    AutoPtr<CRecognitionManagerService> recognition;
    AutoPtr<NetworkTimeUpdateService> networkTimeUpdater;
    AutoPtr<CCommonTimeManagementService> commonTimeMgmtService;
    // Create a shared handler thread for UI within the system server.
    // This thread is used by at least the following components:
    // - WindowManagerPolicy
    // - KeyguardViewManager
    // - DisplayManagerService
    AutoPtr<IHandlerThread> uiHandlerThread;
    CHandlerThread::New(String("UI"), (IHandlerThread**)&uiHandlerThread);
    uiHandlerThread->Start();
    AutoPtr<ILooper> looper;
    uiHandlerThread->GetLooper((ILooper**)&looper);
    AutoPtr<IHandler> uiHandler;
    CHandler::New(looper, (IHandler**)&uiHandler);
//    uiHandler.post(new Runnable() {
//        @Override
//        public void run() {
//            //Looper.myLooper().setMessageLogging(new LogPrinter(
//            //        Log.VERBOSE, "WindowManagerPolicy", Log.LOG_ID_SYSTEM));
//            android.os.Process.setThreadPriority(
//                    android.os.Process.THREAD_PRIORITY_FOREGROUND);
//            android.os.Process.setCanSelfBackground(false);
//
//            // For debug builds, log event loop stalls to dropbox for analysis.
//            if (StrictMode.conditionallyEnableDebugLogging()) {
//                Slog.i(TAG, "Enabled StrictMode logging for UI Looper");
//            }
//        }
//    });

    // Create a handler thread just for the window manager to enjoy.
    AutoPtr<IHandlerThread> wmHandlerThread;
    CHandlerThread::New(String("WindowManager"), (IHandlerThread**)&wmHandlerThread);
    wmHandlerThread->Start();
    looper = NULL;
    wmHandlerThread->GetLooper((ILooper**)&looper);
    AutoPtr<IHandler> wmHandler;
    CHandler::New(looper, (IHandler**)&wmHandler);
//    wmHandler.post(new Runnable() {
//        @Override
//        public void run() {
//            //Looper.myLooper().setMessageLogging(new LogPrinter(
//            //        android.util.Log.DEBUG, TAG, android.util.Log.LOG_ID_SYSTEM));
//            android.os.Process.setThreadPriority(
//                    android.os.Process.THREAD_PRIORITY_DISPLAY);
//            android.os.Process.setCanSelfBackground(false);
//
//            // For debug builds, log event loop stalls to dropbox for analysis.
//            if (StrictMode.conditionallyEnableDebugLogging()) {
//                Slog.i(TAG, "Enabled StrictMode logging for WM Looper");
//            }
//        }
//    });

    // Critical services...
    Boolean onlyCore = FALSE;
//    try {
    // Wait for installd to finished starting up so that it has a chance to
    // create critical directories such as /data/user with the appropriate
    // permissions.  We need this to complete before we initialize other services.
    Slogger::I(TAG, "Waiting for installd to be ready.");
    installer = new Installer();
    installer->Ping();

    // Slog.i(TAG, "Entropy Mixer");
    // ServiceManager.addService("entropy", new EntropyMixer());

    Slogger::I(TAG, "Power Manager");
    CPowerManagerService::NewByFriend((CPowerManagerService**)&power);
    ServiceManager::AddService(IContext::POWER_SERVICE, (IIPowerManager*)power.Get());

    Slogger::I(TAG, "Activity Manager");
    context = CActivityManagerService::Main(factoryTest);

    Slogger::I(TAG, "Display Manager");
    CDisplayManagerService::NewByFriend(context, wmHandler, uiHandler, (CDisplayManagerService**)&display);
    ServiceManager::AddService(IContext::DISPLAY_SERVICE, (IIDisplayManager*)display, TRUE);

    // Slog.i(TAG, "Telephony Registry");
    // telephonyRegistry = new TelephonyRegistry(context);
    // ServiceManager.addService("telephony.registry", telephonyRegistry);

    // Slog.i(TAG, "Scheduling Policy");
    // ServiceManager.addService(Context.SCHEDULING_POLICY_SERVICE,
    //         new SchedulingPolicyService());

    AttributeCache::Init(context);

    Boolean res;
    display->WaitForDefaultDisplay(&res);
    if (!res) {
        // reportWtf("Timeout waiting for default display to be initialized.",
        //         new Throwable());
        Slogger::W(TAG, "***********************************************");
        Logger::E(TAG, "BOOT FAILURE Timeout waiting for default display to be initialized.");
        assert(0);
    }

    Slogger::I(TAG, "Package Manager");
    // Only run "core" apps if we're encrypting the device.
    String cryptState;
    sysProp->Get(String("vold.decrypt"), &cryptState);
    if (ENCRYPTING_STATE.Equals(cryptState)) {
        Slogger::W(TAG, "Detected encryption in progress - only parsing core apps");
        onlyCore = TRUE;
    }
    else if (ENCRYPTED_STATE.Equals(cryptState)) {
        Slogger::W(TAG, "Device encrypted - only parsing core apps");
        onlyCore = TRUE;
    }

    pm = CPackageManagerService::Main(context, installer,
        factoryTest != CSystemServer::FACTORY_TEST_OFF,
        onlyCore);
    Boolean firstBoot = FALSE;
//    try {
    pm->IsFirstBoot(&firstBoot);
//    } catch (RemoteException e) {
//    }

    Slogger::I(TAG, "Set System Process");
    CActivityManagerService::SetSystemProcess();

    Slogger::I(TAG, "User Service");
    ServiceManager::AddService(IContext::USER_SERVICE,
        (IIUserManager*)(CUserManagerService::GetInstance()));

    context->GetContentResolver((IContentResolver**)&mContentResolver);

    // The AccountManager must come before the ContentService
    // try {
    Slogger::I(TAG, "Account Manager");
    CAccountManagerService::NewByFriend(context, (CAccountManagerService**)&accountManager);
    ServiceManager::AddService(IContext::ACCOUNT_SERVICE, (IIAccountManager*)accountManager.Get());
    // } catch (Throwable e) {
    //     Slog.e(TAG, "Failure starting Account Manager", e);
    // }

    Slogger::I(TAG, "Content Manager");
    AutoPtr<CContentService> contentService;
    CContentService::NewByFriend(context, factoryTest == CSystemServer::FACTORY_TEST_LOW_LEVEL, (CContentService**)&contentService);
    ServiceManager::AddService(IContentResolver::CONTENT_SERVICE_NAME, (IContentService*)contentService.Get());

    Slogger::I(TAG, "System Content Providers");
    CActivityManagerService::InstallSystemProviders();

    Slogger::I(TAG, "Lights Service");
    lights = new LightsService(context);

    Slogger::I(TAG, "Battery Service");
    CBatteryService::NewByFriend(context, (Handle32)lights.Get(), (CBatteryService**)&battery);
    ServiceManager::AddService(String("battery"), (IBinder*)battery.Get());

//    Slog.i(TAG, "DynamicPManager");
//    dpm = new DynamicPManagerService(context);
//    ServiceManager.addService(DynamicPManager.DPM_SERVICE, dpm);

    Slogger::I(TAG, "Vibrator Service");
    CVibratorService::NewByFriend(context, (CVibratorService**)&vibrator);
    ServiceManager::AddService(String("vibrator"), (IVibratorService*)vibrator.Get());

    // only initialize the power service after we have started the
    // lights service, content providers and the battery service.
    power->Init(context, lights, CActivityManagerService::Self(), battery,
            CBatteryStatsService::GetService(), display);

    Slogger::I(TAG, "Alarm Manager");
    CAlarmManagerService::NewByFriend(context, (CAlarmManagerService**)&alarm);
    ServiceManager::AddService(IContext::ALARM_SERVICE, (IIAlarmManager*)alarm.Get());

    // Slogger::I(TAG, "Init Watchdog");
    // Watchdog.getInstance().init(context, battery, power, alarm,
    //         ActivityManagerService.self());

    Slogger::I(TAG, "Input Manager");
    CInputManagerService::NewByFriend(
        context, wmHandler, (CInputManagerService**)&inputManager);
    ServiceManager::AddService(IContext::INPUT_SERVICE, (IIInputManager*)inputManager.Get());

    Slogger::I(TAG, "Window Manager");
    wm = CWindowManagerService::Main(context, power, display, inputManager,
        uiHandler, wmHandler,
        factoryTest != CSystemServer::FACTORY_TEST_LOW_LEVEL,
        !firstBoot, onlyCore);
    ServiceManager::AddService(IContext::WINDOW_SERVICE, (IIWindowManager*)wm.Get());

    sysProp->Get(String("ro.display.switch"), &value);
    Slogger::I(TAG, "Display Manager AW: %s", value.string());
    if (value.Equals("1")) {
        AutoPtr<CDisplayManagerServiceAw> display_aw;
        CDisplayManagerServiceAw::NewByFriend(context, power, (CDisplayManagerServiceAw**)&display_aw);
        ServiceManager::AddService(IContext::DISPLAY_SERVICE_AW, display_aw->Probe(EIID_IInterface));

        // try {
            ECode ec = display_aw->SystemReady();
        // } catch (Throwable e) {
            if (FAILED(ec)) {
                Slogger::E(TAG, "Failure starting AW Display Manager");
            }
        // }
    }

    sysProp->Get(String("ro.wifidisplay.switch"), &value);
    Slogger::I(TAG, "Display Manager wifi: %s", value.Equals("1") ? "TRUE" : "FALSE");
    //TODO if (value.Equals("1"))
    {
        AutoPtr<CWifiDisplayManagerService> wifidisplay;
        CWifiDisplayManagerService::NewByFriend(context, power, (CWifiDisplayManagerService**)&wifidisplay);
        ServiceManager::AddService(IContext::WIFIDISPLAY_SERVICE, wifidisplay->Probe(EIID_IInterface));
    }

    CActivityManagerService::Self()->SetWindowManager(wm);

    inputManager->SetWindowManagerCallbacks(wm->GetInputMonitor());
    inputManager->Start();
    //TODO: inorder to start input dispatch, should be remove
    inputManager->SetInputDispatchMode(TRUE, FALSE);

    display->SetWindowManager(IDisplayManagerServiceWindowManagerFuncs::Probe(wm));
    display->SetInputManager(IDisplayManagerServiceInputManagerFuncs::Probe(inputManager));

//    } catch (RuntimeException e) {
//        Slog.e("System", "******************************************");
//        Slog.e("System", "************ Failure starting core service", e);
//    }

    AutoPtr<CDevicePolicyManagerService> devicePolicy;
    AutoPtr<CStatusBarManagerService> statusBar;
    AutoPtr<CInputMethodManagerService> imm;
    AutoPtr<CBackupManagerService> backupManager;
    AutoPtr<CAppWidgetService> appWidget;
    AutoPtr<CNotificationManagerService> notification;
    AutoPtr<CWallpaperManagerService> wallpaper;
    AutoPtr<CLocationManagerService> locationManager;
    // CountryDetectorService countryDetector = null;
    AutoPtr<CTextServicesManagerService> tsms;
    AutoPtr<CLockSettingsService> lockSettings;
    AutoPtr<IDreamManagerService> dreamy;

    // Bring up services needed for UI.
    if (factoryTest != CSystemServer::FACTORY_TEST_LOW_LEVEL) {
        // try {
        Slogger::I(TAG, "Input Method Service");
        CInputMethodManagerService::NewByFriend(context, (Handle32)wm.Get(), (CInputMethodManagerService**)&imm);
        ServiceManager::AddService(IContext::INPUT_METHOD_SERVICE, (IIInputMethodManager*)imm.Get());
        // } catch (Throwable e) {
        //     reportWtf("starting Input Manager Service", e);
        // }

        // try {
        Slogger::I(TAG, "Accessibility Manager");
        AutoPtr<CAccessibilityManagerService> accessibilityService;
        CAccessibilityManagerService::NewByFriend(context, (CAccessibilityManagerService**)&accessibilityService);
        ServiceManager::AddService(IContext::ACCESSIBILITY_SERVICE, (IIAccessibilityManager*)accessibilityService.Get());
        // } catch (Throwable e) {
        //     reportWtf("starting Accessibility Manager", e);
        // }
    }

    // try{
    wm->DisplayReady();
    // } catch (Throwable e) {
    //     reportWtf("making display ready", e);
    // }

    // try {
    pm->PerformBootDexOpt();
    // } catch (Throwable e) {
    //     reportWtf("performing boot dexopt", e);
    // }

    // try {
    //     ActivityManagerNative.getDefault().showBootMessage(
    //             context.getResources().getText(
    //                     com.android.internal.R.string.android_upgrading_starting_apps),
    //                     false);
    // } catch (RemoteException e) {
    // }

    if (factoryTest != CSystemServer::FACTORY_TEST_LOW_LEVEL) {
        Slogger::I(TAG, "PrivacySettings Service");
        AutoPtr<IIPrivacySettingsManager> privacySettingsManager;
        CPrivacySettingsManagerService::New(context, (IIPrivacySettingsManager**)&privacySettingsManager);
        ServiceManager::AddService(String("privacy"), (IInterface*)privacySettingsManager);

        if (sysProp->Get(String("system_init.startmountservice"), &value), !String("0").Equals(value)) {
            // try {
            /*
             * NotificationManagerService is dependant on MountService,
             * (for media / usb notifications) so we must start MountService first.
             */
            Slogger::I(TAG, "Mount Service");
            CMountService::NewByFriend(context, (CMountService**)&mountService);
            ServiceManager::AddService(String("mount"), (IMountService*)mountService.Get());
            // } catch (Throwable e) {
            //     reportWtf("starting Mount Service", e);
            // }
        }

        // try {
        Slogger::I(TAG,  "LockSettingsService");
        CLockSettingsService::NewByFriend(context, (CLockSettingsService**)&lockSettings);
        ServiceManager::AddService(String("lock_settings"), (ILockSettings*)lockSettings.Get());
        // } catch (Throwable e) {
        //     reportWtf("starting LockSettingsService service", e);
        // }

        // try {
        Slogger::I(TAG, "Device Policy");
        CDevicePolicyManagerService::NewByFriend(context, (CDevicePolicyManagerService**)&devicePolicy);
        assert(devicePolicy != NULL);
        ServiceManager::AddService(IContext::DEVICE_POLICY_SERVICE, (IIDevicePolicyManager*)devicePolicy->Probe(EIID_IIDevicePolicyManager));
        // } catch (Throwable e) {
        //     reportWtf("starting DevicePolicyService", e);
        // }

        // try {
        Slogger::I(TAG, "Status Bar");
        CStatusBarManagerService::NewByFriend(context, (Handle32)wm.Get(), (CStatusBarManagerService**)&statusBar);
        assert(statusBar != NULL);
        ServiceManager::AddService(IContext::STATUS_BAR_SERVICE, (IIStatusBarService*)statusBar.Get());
        // } catch (Throwable e) {
        //     reportWtf("starting StatusBarManagerService", e);
        // }

        // try {
        Slogger::I(TAG, "Clipboard Service");
        AutoPtr<CClipboardService> clipboardService;
        CClipboardService::NewByFriend(context, (CClipboardService**)&clipboardService);
        ServiceManager::AddService(IContext::CLIPBOARD_SERVICE, (IClipboard*)clipboardService.Get());
        // } catch (Throwable e) {
        //     reportWtf("starting Clipboard Service", e);
        // }

        Slogger::I(TAG, "NetworkManagement Service");
        networkManagement = CNetworkManagementService::Create(context);
        ServiceManager::AddService(IContext::NETWORKMANAGEMENT_SERVICE, (INetworkManagementService*)networkManagement.Get());

        //try {
        Slogger::I(TAG, "Text Service Manager Service");
        CTextServicesManagerService::NewByFriend(context, (CTextServicesManagerService**)&tsms);
        ServiceManager::AddService(IContext::TEXT_SERVICES_MANAGER_SERVICE, (IITextServicesManager*)tsms.Get());
        //} catch (Throwable e) {
        //    reportWtf("starting Text Service Manager Service", e);
        //}

        // try {
        Slogger::I(TAG, "NetworkStats Service");
        CNetworkStatsService::NewByFriend(
            context, (INetworkManagementService*)networkManagement,
            (IIAlarmManager*)alarm, (CNetworkStatsService**)&networkStats);
        ServiceManager::AddService(IContext::NETWORK_STATS_SERVICE, (INetworkStatsService*)networkStats.Get());
        // } catch (Throwable e) {
        //     reportWtf("starting NetworkStats Service", e);
        // }

        // try {
        Slogger::I(TAG, "NetworkPolicy Service");
        CNetworkPolicyManagerService::NewByFriend(
            context, (IIActivityManager*)CActivityManagerService::Self(),
            (IIPowerManager*)power, (INetworkStatsService*)networkStats,
            (INetworkManagementService*)networkManagement, (CNetworkPolicyManagerService**)&networkPolicy);
        ServiceManager::AddService(IContext::NETWORK_POLICY_SERVICE, (IINetworkPolicyManager*)networkPolicy.Get());
        // } catch (Throwable e) {
        //     reportWtf("starting NetworkPolicy Service", e);
        // }

        // try {
        Slogger::I(TAG, "Wi-Fi P2pService");
        CWifiP2pService::New(context, (IWifiP2pService**)&wifiP2p);
        IIWifiP2pManager* wpm = IIWifiP2pManager::Probe(wifiP2p);
        assert(wpm != NULL);
        ServiceManager::AddService(IContext::WIFI_P2P_SERVICE, wpm);
        //  } catch (Throwable e) {
        //      reportWtf("starting Wi-Fi P2pService", e);
        //  }

        Slogger::I(TAG, "Ethernet Service");
        CEthernetService::NewByFriend(context, (CEthernetService**)&ethernet);
        ServiceManager::AddService(IContext::ETHERNET_SERVICE, (IIEthernetManager*)ethernet.Get());


        // try {
        Slogger::I(TAG, "Wi-Fi Service");
        CWifiService::NewByFriend(context, (CWifiService**)&wifi);
        ServiceManager::AddService(IContext::WIFI_SERVICE, (IIWifiManager*)wifi.Get());
        // } catch (Throwable e) {
        //     reportWtf("starting Wi-Fi Service", e);
        // }

        // try{
        Slogger::I(TAG, "Connectivity Service");
        CConnectivityService::NewByFriend(
           context, (INetworkManagementService*)networkManagement,
           (INetworkStatsService*)networkStats, (IINetworkPolicyManager*)networkPolicy,
           (CConnectivityService**)&connectivity);
        ServiceManager::AddService(IContext::CONNECTIVITY_SERVICE, (IIConnectivityManager*)connectivity.Get());
        networkStats->BindConnectivityManager((IIConnectivityManager*)connectivity);
        networkPolicy->BindConnectivityManager((IIConnectivityManager*)connectivity);
        wifi->CheckAndStartWifi();
        wifiP2p->ConnectivityServiceReady();
        // } catch (Throwable e) {
        //     reportWtf("starting Connectivity Service", e);
        // }

        // try {
        Slogger::I(TAG, "Network Service Discovery Service");
        CNsdService::Create(context, (CNsdService**)&serviceDiscovery);
        ServiceManager::AddService(IContext::NSD_SERVICE, (IINsdManager*)serviceDiscovery->Probe(EIID_IINsdManager));
        // } catch (Throwable e) {
        //     reportWtf("starting Service Discovery Service", e);
        // }

        // try {
         Slogger::I(TAG, "Throttle Service");
         CThrottleService::NewByFriend(context, (CThrottleService**)&throttle);
         ServiceManager::AddService(IContext::THROTTLE_SERVICE, (IIThrottleManager*)throttle.Get());
        // } catch (Throwable e) {
        //     reportWtf("starting ThrottleService", e);
        // }

        //try {
        Slogger::I(TAG, "UpdateLock Service");
        AutoPtr<CUpdateLockService> updateLock;
        CUpdateLockService::NewByFriend(context, (CUpdateLockService**)&updateLock);
        ServiceManager::AddService(IContext::UPDATE_LOCK_SERVICE, (IIUpdateLock*)updateLock.Get());
        //} catch (Throwable e) {
        //    reportWtf("starting UpdateLockService", e);
        //}

        /*
         * MountService has a few dependencies: Notification Manager and
         * AppWidget Provider. Make sure MountService is completely started
         * first before continuing.
         */
        if (mountService != NULL) {
            mountService->WaitForAsecScan();
        }

        // try {
        if (accountManager != NULL)
            accountManager->SystemReady();
        // } catch (Throwable e) {
        //     reportWtf("making Account Manager Service ready", e);
        // }

        // try {
        if (contentService != NULL)
            contentService->SystemReady();
        // } catch (Throwable e) {
        //     reportWtf("making Content Service ready", e);
        // }

        Slogger::I(TAG, "Notification Manager");
        CNotificationManagerService::NewByFriend(context,
            (Handle32)statusBar.Get(), (Handle32)lights.Get(),
            (CNotificationManagerService**)&notification);
        ServiceManager::AddService(IContext::NOTIFICATION_SERVICE,
            (IINotificationManager*)notification.Get());
        // networkPolicy->BindNotificationManager(notification);

        // try {
        Slogger::I(TAG, "Device Storage Monitor");
        AutoPtr<CDeviceStorageMonitorService> deviceStorageMonitor;
        CDeviceStorageMonitorService::NewByFriend(context, (CDeviceStorageMonitorService**)&deviceStorageMonitor);
        ServiceManager::AddService(CDeviceStorageMonitorService::SERVICE,
            (IBinder*)deviceStorageMonitor.Get());
        // } catch (Throwable e) {
        //     reportWtf("starting DeviceStorageMonitor service", e);
        // }

        Slogger::I(TAG, "Location Manager");
        CLocationManagerService::NewByFriend(context, (CLocationManagerService**)&locationManager);
        ServiceManager::AddService(IContext::LOCATION_SERVICE, (IILocationManager*)locationManager.Get());

        //try {
        Slogger::I(TAG, "Country Detector");
        CCountryDetectorService::NewByFriend(context, (CCountryDetectorService**)&countryDetector);
        ServiceManager::AddService(IContext::COUNTRY_DETECTOR, (IICountryDetector*)countryDetector.Get());
        //} catch (Throwable e) {
        //    reportWtf("starting Country Detector", e);
        //}

        // try {
        Slogger::I(TAG, "Search Manager");
        AutoPtr<CSearchManagerService> searchManagerService;
        CSearchManagerService::NewByFriend(context, (CSearchManagerService**)&searchManagerService);
        ServiceManager::AddService(IContext::SEARCH_SERVICE, (IISearchManager*)searchManagerService.Get());
        // } catch (Throwable e) {
        //     reportWtf("starting Search Service", e);
        // }

        //try {
        Slogger::I(TAG, "DropBox Service");
        AutoPtr<IFile> file;
        CFile::New(String("/data/system/dropbox"), (IFile**)&file);
        AutoPtr<CDropBoxManagerService> dropBox;
        CDropBoxManagerService::NewByFriend(context, file, (CDropBoxManagerService**)&dropBox);
        ServiceManager::AddService(IContext::DROPBOX_SERVICE, (IDropBoxManagerService*)dropBox.Get());
        //} catch (Throwable e) {
        //    reportWtf("starting DropBoxManagerService", e);
        //}

        AutoPtr<IResources> r;
        context->GetResources((IResources**)&r);
        Boolean enable = TRUE;
        r->GetBoolean(R::bool_::config_enableWallpaperService, &enable);
        if (enable) {
            //try {
                Slogger::I(TAG, "Wallpaper Service");
                if (!headless) {
                    CWallpaperManagerService::NewByFriend(
                        context, (CWallpaperManagerService**)&wallpaper);
                    ServiceManager::AddService(
                        IContext::WALLPAPER_SERVICE, (IIWallpaperManager*)wallpaper.Get());
                }
            // } catch (Throwable e) {
            //     reportWtf("starting Wallpaper Service", e);
            // }
        }

        String properties;
        sysProp->Get(String("system_init.startaudioservice"), &properties);
        Slogger::I(TAG, "Audio Service : %s", properties.string());
        if (!properties.Equals("0")) {
            // try {
                AutoPtr<IAudioService> as;
                CAudioService::New(context, (IAudioService**)&as);
                ServiceManager::AddService(IContext::AUDIO_SERVICE, IIAudioService::Probe(as));
            // } catch (Throwable e) {
            //     reportWtf("starting Audio Service", e);
            // }
        }

        // try {
        //     Slog.i(TAG, "Dock Observer");
        //     // Listen for dock station changes
        //     dock = new DockObserver(context);
        // } catch (Throwable e) {
        //     reportWtf("starting DockObserver", e);
        // }

        // try {
        //     Slog.i(TAG, "Wired Accessory Manager");
        //     // Listen for wired headset changes
        //     inputManager.setWiredAccessoryCallbacks(
        //             new WiredAccessoryManager(context, inputManager));
        // } catch (Throwable e) {
        //     reportWtf("starting WiredAccessoryManager", e);
        // }

        // try {
            Slogger::I(TAG, "Audio device manager Observer");
            // Listen for wired headset changes
            AudioDeviceManagerObserver::GetInstance(context);
        // } catch (Throwable e) {
        //     reportWtf("starting AudioDeviceManagerObserver", e);
        // }

        Slogger::I(TAG, "Usb Service");
        CUsbService::NewByFriend(context, (CUsbService**)&usb);
        ServiceManager::AddService(IContext::USB_SERVICE, (IIUsbManager*)usb->Probe(EIID_IIUsbManager));

        // try {
        //     Slog.i(TAG, "Usb Camera device manager Observer");
        //     // Listen for wired headset changes
        //     UsbCameraDeviceManagerObserver.getInstance(context);
        // } catch (Throwable e) {
        //     reportWtf("starting UsbCameraDeviceManagerObserver", e);
        // }

        Slogger::I(TAG, "Serial Service");
        // Serial port support
        CSerialService::NewByFriend(context, (CSerialService**)&serial);
        ServiceManager::AddService(IContext::SERIAL_SERVICE, (IInterface*)serial->Probe(EIID_IInterface));

        // try {
        Slogger::I(TAG, "Twilight Service");
        twilight = new TwilightService(context);
        // } catch (Throwable e) {
        //     reportWtf("starting TwilightService", e);
        // }

        // try {
        Slogger::I(TAG, "UI Mode Manager Service");
        // Listen for UI mode changes
        CUiModeManagerService::NewByFriend(context, (Handle32)twilight.Get(), (CUiModeManagerService**)&uiMode);
        // } catch (Throwable e) {
        //     reportWtf("starting UiModeManagerService", e);
        // }

        //try {
        Slogger::I(TAG, "Backup Service");
        CBackupManagerService::NewByFriend(context, (CBackupManagerService**)&backupManager);
        ServiceManager::AddService(IContext::BACKUP_SERVICE, (IIBackupManager*)backupManager.Get());
        //} catch (Throwable e) {
        //    Slogger::E(TAG, "Failure starting Backup Service", e);
        //}

        // try {
        Slogger::I(TAG, "AppWidget Service");
        CAppWidgetService::NewByFriend(
            context, (CAppWidgetService**)&appWidget);
        ServiceManager::AddService(IContext::APPWIDGET_SERVICE, (IIAppWidgetService*)appWidget.Get());
        // } catch (Throwable e) {
        //     reportWtf("starting AppWidget Service", e);
        // }
        //try {
        Slogger::I(TAG, "Recognition Service");
        CRecognitionManagerService::NewByFriend(context, (CRecognitionManagerService**)&recognition);
        // } catch (Throwable e) {
        //     reportWtf("starting Recognition Service", e);
        // }

        Slogger::I(TAG, "DiskStats Service");
        AutoPtr<CDiskStatsService> diskStats;
        CDiskStatsService::NewByFriend(context, (CDiskStatsService**)&diskStats);
        ServiceManager::AddService(String("diskstats"), (IBinder*)diskStats);

        Slogger::I(TAG, "KortideRemote Service");
        AutoPtr<CKortideRemoteService> krService;
        CKortideRemoteService::NewByFriend(context, (CKortideRemoteService**)&krService);
        ServiceManager::AddService(String("KortideRemote"), (IBinder*)krService.Get());

        Slogger::I(TAG, "Zigbee Service");
        AutoPtr<CZigbeeService> zigbeeService;
        CZigbeeService::NewByFriend(context, (CZigbeeService**)&zigbeeService);
        ServiceManager::AddService(String("zigbee"), (IBinder*)zigbeeService.Get());

        // need to add this service even if SamplingProfilerIntegration.isEnabled()
        // is false, because it is this service that detects system property change and
        // turns on SamplingProfilerIntegration. Plus, when sampling profiler doesn't work,
        // there is little overhead for running this service.
        Slogger::I(TAG, "SamplingProfiler Service");
        AutoPtr<CSamplingProfilerService> samplingProfiler;
        CSamplingProfilerService::NewByFriend(context, (CSamplingProfilerService**)&samplingProfiler);
        ServiceManager::AddService(String("samplingprofiler"), (IBinder*)samplingProfiler.Get());

        // try {
        Slogger::I(TAG, "NetworkTimeUpdateService");
        networkTimeUpdater = new NetworkTimeUpdateService(context);
        // } catch (Throwable e) {
        //     reportWtf("starting NetworkTimeUpdate service", e);
        // }

        // try {
        Slogger::I(TAG, "CommonTimeManagementService");
        CCommonTimeManagementService::NewByFriend(context, (CCommonTimeManagementService**)&commonTimeMgmtService);
        ServiceManager::AddService(String("commontime_management"), (IBinder*)commonTimeMgmtService.Get());
        // } catch (Throwable e) {
        //     reportWtf("starting CommonTimeManagementService service", e);
        // }

        // try {
        //     Slog.i(TAG, "CertBlacklister");
        //     CertBlacklister blacklister = new CertBlacklister(context);
        // } catch (Throwable e) {
        //     reportWtf("starting CertBlacklister", e);
        // }
        r->GetBoolean(R::bool_::config_dreamsSupported, &enable);
        if (enable) {
            CDreamManagerService::New(context, wmHandler, (IDreamManagerService**)&dreamy);
            ServiceManager::AddService(String("dreams")/*IDreamService::DREAM_SERVICE*/, (IBinder*)dreamy.Get());
        }
        // if (context.getResources().getBoolean(
        //         com.android.internal.R.bool.config_dreamsSupported)) {
        //     try {
        //         Slog.i(TAG, "Dreams Service");
        //         // Dreams (interactive idle-time views, a/k/a screen savers)
        //         dreamy = new DreamManagerService(context, wmHandler);
        //         ServiceManager.addService(DreamService.DREAM_SERVICE, dreamy);
        //     } catch (Throwable e) {
        //         reportWtf("starting DreamManagerService", e);
        //     }
        // }
    }

    // Before things start rolling, be sure we have decided whether
    // we are in safe mode.
    Boolean safeMode = wm->DetectSafeMode();
    if (safeMode) {
        CActivityManagerService::Self()->EnterSafeMode();
        // TODO
        // Post the safe mode state in the Zygote class
        // Zygote.systemInSafeMode = TRUE;
        // Disable the JIT for the system_server process
        // VMRuntime.getRuntime().disableJitCompilation();
    } else {
    //     // Enable the JIT for the system_server process
    //     // VMRuntime.getRuntime().startJitCompilation();
    }

    // It is now time to start up the app processes...

    //try {
    ec = vibrator->SystemReady();
    Slogger::I(TAG, "Making Vibrator Manager Service ready, ec=%08x", ec);
    // } catch (Throwable e) {
    //     reportWtf("making Vibrator Service ready", e);
    // }

    // try {
    lockSettings->SystemReady();
    // } catch (Throwable e) {
    //     reportWtf("making Lock Settings Service ready", e);
    // }

    if (devicePolicy != NULL) {
    // try {
        ec = devicePolicy->SystemReady();
        Slogger::I(TAG, "Making Device Policy Service ready, ec=%08x", ec);
    // } catch (Throwable e) {
    //     reportWtf("making Device Policy Service ready", e);
    // }
    }

    if (notification != NULL) {
        ec = notification->SystemReady();
        Slogger::I(TAG, "Making Notification Manager Service ready, ec=%08x", ec);
    }

    // try {
    ec = wm->SystemReady();
    Slogger::I(TAG, "Making Notification Manager Service ready, ec=%08x", ec);
    // } catch (Throwable e) {
    //     reportWtf("making Window Manager Service ready", e);
    // }

    // try {
    ec = power->SystemReady(twilight/*dreamy*/);
    Slogger::I(TAG, "Making Power Manager Service ready, ec=%08x", ec);
    // } catch (Throwable e) {
    //     reportWtf("making Power Manager Service ready", e);
    // }

    //     try {
    ec = pm->SystemReady();
    Slogger::I(TAG, "Making Package Manager Service ready, ec=%08x", ec);
    // } catch (Throwable e) {
    //     reportWtf("making Package Manager Service ready", e);
    // }

    // try {

    ec = display->SystemReady(safeMode, onlyCore);
    Slogger::I(TAG, "Making Display Manager Service ready, safeMode=%d, onlyCore=%d, ec=%08x",
        safeMode, onlyCore, ec);
    // } catch (Throwable e) {
    //     reportWtf("making Display Manager Service ready", e);
    // }


    // We now tell the activity manager it is okay to run third party
    // code.  It will call back into us once it has gotten to the state
    // where third party code can really run (but before it has actually
    // started launching the initial applications), for us to complete our
    // initialization.
    AutoPtr<SystemReadyRunnable> task = new SystemReadyRunnable();
    task->mImm = imm;
    task->mWallpaper = wallpaper;
    task->mUsb = usb;
    task->mBattery = battery;
    task->mThrottle = throttle;
    task->mTwilight = twilight;
    task->mUiMode = uiMode;
    task->mConnectivity = connectivity;
    task->mNetworkManage = networkManagement;
    task->mNetworkStats = networkStats;
    task->mNetworkPolicy = networkPolicy;
    task->mMountService = mountService;
    task->mAppWidget = appWidget;
    task->mStatusBar = statusBar;
    task->mRecognition = recognition;
    task->mTextServiceManagerService = tsms;
    task->mCountryDetector = countryDetector;
    task->mNetworkTimeUpdater = networkTimeUpdater;
    task->mCommonTimeMgmtService = commonTimeMgmtService;
    task->mLocationF = locationManager;
    task->mContext = context;
    task->mHeadless = headless;
    ec = CActivityManagerService::Self()->SystemReady(task);
    Slogger::I(TAG, "Making Activity Manager Service ready");

    /*
     * Author : huanglong@allwinnertech.com
     * Description : Initialize the DisplayManagerAw, to set the display area.
     * Date : 2013-3-15
     */
    AutoPtr<IDisplayManagerAw> displayManager;
    CDisplayManagerAw::New((IDisplayManagerAw**)&displayManager);
    if (displayManager == NULL) {
        Logger::W(TAG, "Failed in creating DisplayManager.");
        assert(0);
        // init the display area
        // Int32 ratio = GetDispRatio();
        // Slogger::I(TAG, "Display area ratio = %d", ratio);
        // Int32 result;
        // displayManager->SetDisplayAreaPercent(0, ratio, &result);
    }

    // /* add by Gary. start {{----------------------------------- */
    // /* 2011-12-10 */
    /* init display output */
    Boolean ypbprIsConnected = FALSE;
    Boolean cvbsIsConnected = FALSE;
    Boolean hdmiIsConnected = FALSE;
    AutoPtr<IDispListDispFormat> finalFormat;

    if (displayManager == NULL) {
        Logger::W(TAG, "Fail in creating DisplayManager.");
    }

    // /* check cable's connection status */
    Int32 hdmiStatus;
    displayManager->GetHdmiHotPlugStatus(&hdmiStatus);
    if (hdmiStatus != 0)
        hdmiIsConnected = TRUE;

    Int32 tvStatus;
    displayManager->GetTvHotPlugStatus(&tvStatus);
    Logger::D(TAG, "tv connect status = %d", tvStatus);
    if (tvStatus == IDisplayManagerAw::DISPLAY_TVDAC_CVBS)
        cvbsIsConnected = TRUE;
    else if (tvStatus == IDisplayManagerAw::DISPLAY_TVDAC_YPBPR)
        ypbprIsConnected = TRUE;
    else if (tvStatus == IDisplayManagerAw::DISPLAY_TVDAC_ALL) {
        cvbsIsConnected = TRUE;
        ypbprIsConnected = TRUE;
    }
    Logger::D(TAG, "HDMI connect status = %d, av connnect status = %d, YPbPr connect status = %d",
        hdmiIsConnected, cvbsIsConnected, ypbprIsConnected);

    AutoPtr<IContentResolver> cr;
    context->GetContentResolver((IContentResolver**)&cr);
    AutoPtr<ISettingsSystem> systemSettings;
    CSettingsSystem::AcquireSingleton((ISettingsSystem**)&systemSettings);
    AutoPtr<IDispListHelper> dispListHelper;
    CDispListHelper::AcquireSingleton((IDispListHelper**)&dispListHelper);

    AutoPtr<IDispListDispFormat> savedFormat;
    String savedFormatName;
    systemSettings->GetString(cr, ISettingsSystem::DISPLY_OUTPUT_FORMAT, &savedFormatName);
    if (savedFormatName.IsNull()) {
        Logger::W(TAG, "Fail in getting saved display format.");
        dispListHelper->GetStaticDispFormat(IDispList::HDMI_DEFAULT_FORMAT_ID, (IDispListDispFormat**)&savedFormat);
    }
    else {
        dispListHelper->ItemName2Code(savedFormatName, (IDispListDispFormat**)&savedFormat);
        Logger::D(TAG, "saved display = %s", savedFormatName.string());
    }

    Int32 savedType;
    savedFormat->GetOutputType(&savedType);

    Int32 doType, doFormat;
    displayManager->GetDisplayOutputType(0, &doType);
    displayManager->GetDisplayOutputFormat(0, &doFormat);
    AutoPtr<IDispListDispFormat> curFormat;
    CDispListDispFormat::New(doType, doFormat, (IDispListDispFormat**)&curFormat);
    Logger::D(TAG, "current format is (type, format): (%d, %d)", doType, doFormat);

    Int32 savedIntType;
    dispListHelper->GetAdvancedDisplayType(savedFormat, &savedIntType);
    Int32 finalIntType = IDispList::ADVANCED_DISPLAY_TYPE_HDMI;

    Boolean bval;
    if (hdmiIsConnected && (dispListHelper->IsHDMI(curFormat, &bval), bval)) {
        finalIntType = IDispList::ADVANCED_DISPLAY_TYPE_HDMI;
        finalFormat = curFormat;
    }
    else if (cvbsIsConnected && (dispListHelper->IsCVBS(curFormat, &bval), bval)) {
        finalIntType = IDispList::ADVANCED_DISPLAY_TYPE_CVBS;
        finalFormat = curFormat;
    }
    else if (mDeviceHasYpbpr && ypbprIsConnected
        && (dispListHelper->IsYPbPr(curFormat, &bval), bval)) {
        finalIntType = IDispList::ADVANCED_DISPLAY_TYPE_YPBPR;
        finalFormat = curFormat;
    }
    else if (!mDeviceHasYpbpr && ypbprIsConnected
        && (dispListHelper->IsVGA(curFormat, &bval), bval)) {
        finalIntType = IDispList::ADVANCED_DISPLAY_TYPE_VGA;
        dispListHelper->GetStaticDispFormat(
            IDispList::VGA_DEFAULT_FORMAT_ID, (IDispListDispFormat**)&finalFormat);
    }
    else if (hdmiIsConnected) {
        finalIntType = IDispList::ADVANCED_DISPLAY_TYPE_HDMI;
        dispListHelper->GetStaticDispFormat(
            IDispList::HDMI_DEFAULT_FORMAT_ID, (IDispListDispFormat**)&finalFormat);
    }
    else if (cvbsIsConnected) {
        finalIntType = IDispList::ADVANCED_DISPLAY_TYPE_CVBS;
        dispListHelper->GetStaticDispFormat(
            IDispList::CVBS_DEFAULT_FORMAT_ID, (IDispListDispFormat**)&finalFormat);
    }
    else if (mDeviceHasYpbpr && ypbprIsConnected) {
        finalIntType = IDispList::ADVANCED_DISPLAY_TYPE_YPBPR;
        dispListHelper->GetStaticDispFormat(
            IDispList::YPBPR_DEFAULT_FORMAT_ID, (IDispListDispFormat**)&finalFormat);
    }
    else if (!mDeviceHasYpbpr && ypbprIsConnected){
        finalIntType = IDispList::ADVANCED_DISPLAY_TYPE_VGA;
        dispListHelper->GetStaticDispFormat(
            IDispList::VGA_DEFAULT_FORMAT_ID, (IDispListDispFormat**)&finalFormat);
    }
    else {
        finalIntType = savedIntType;
        finalFormat = savedFormat;
    }

    if (finalIntType == savedIntType)
        finalFormat = savedFormat;

    Int32 finalOutputType, finalFormatFormat;
    finalFormat->GetOutputType(&finalOutputType);
    finalFormat->GetFormat(&finalFormatFormat);
    if (finalOutputType == IDisplayManagerAw::DISPLAY_OUTPUT_TYPE_HDMI) {
        Int32 ival;
        displayManager->IsSupportHdmiMode(finalFormatFormat, &ival);
        Boolean isSupport = ival != 0;
        if (!isSupport) {
            String str;
            dispListHelper->ItemCode2Name(finalFormat, &str);
            Logger::D(TAG, "HDMI mode %s is NOT supported by the TV.", str.string());
            finalFormat = NULL;
            dispListHelper->GetStaticDispFormat(
                IDispList::HDMI_DEFAULT_FORMAT_ID, (IDispListDispFormat**)&finalFormat);
        }
    }
    String finalFormatStr;
    dispListHelper->ItemCode2Name(finalFormat, &finalFormatStr);
    Logger::D(TAG, "final format is %s", finalFormatStr.string());

    finalFormat->GetOutputType(&finalOutputType);
    finalFormat->GetFormat(&finalFormatFormat);
    String srcPath("/data/displaysetmode");
    AutoPtr<IFile> FILE;
    CFile::New(srcPath, (IFile**)&FILE);
    String values;
    values.AppendFormat("%d\n%d", finalOutputType, finalFormatFormat);
    // try {
    AutoPtr<IFileOutputStream> fos;
    CFileOutputStream::New(FILE, (IFileOutputStream**)&fos);
    fos->WriteBytes(*values.GetBytes());
    IFlushable::Probe(fos)->Flush();
    AutoPtr<IFileDescriptor> fd;
    fos->GetFD((IFileDescriptor**)&fd);
    fd->Sync();
    fos->Close();
    // } catch (FileNotFoundException e) {
    // } catch (IOException e) {
    // }

    systemSettings->PutString(cr, ISettingsSystem::DISPLY_OUTPUT_FORMAT, finalFormatStr, &res);

    AutoPtr<IAudioManager> audioManager;
    CAudioManager::New(context, (IAudioManager**)&audioManager);
    if (audioManager == NULL) {
        Logger::E(TAG, "audioManager is NULL");
    }
    else {
        /* modified by chenjd,chenjd@allwinnertech.com,20120710, start {{--------
        * init audio output mode,may be multi channels */
        String audioOutputChannelName;
        systemSettings->GetString(cr, ISettingsSystem::AUDIO_OUTPUT_CHANNEL, &audioOutputChannelName);

        if (audioOutputChannelName.IsNull()) {
            audioOutputChannelName = IAudioManager::AUDIO_NAME_CODEC;
            Logger::D(TAG,"use default audio output channel:%s", audioOutputChannelName.string());
        }
        Logger::D(TAG, "saved audio channel is %s", audioOutputChannelName.string());
        List<String> audioOutputChannels;
        AutoPtr<ArrayOf<String> > audioList;
        StringUtils::Split(audioOutputChannelName, ",", (ArrayOf<String>**)&audioList);
        for (Int32 i = 0; i < audioList->GetLength(); i++){
            String audio = (*audioList)[i];
            if (!audio.IsEmpty()) {
                audioOutputChannels.PushBack(audio);
            }
        }
        // Logger::D(TAG, "active the saved audio channels " + audioOutputChannels);
        AutoPtr<ArrayOf<String> > activedChannels = ArrayOf<String>::Alloc(1);
        if (Find(audioOutputChannels.Begin(), audioOutputChannels.End(),
            IAudioManager::AUDIO_NAME_SPDIF) != audioOutputChannels.End()) {
             activedChannels->Set(0, IAudioManager::AUDIO_NAME_SPDIF);
        }
        else if (finalOutputType == IDisplayManagerAw::DISPLAY_OUTPUT_TYPE_HDMI){
             activedChannels->Set(0, IAudioManager::AUDIO_NAME_HDMI);
        }
        else {
             activedChannels->Set(0, IAudioManager::AUDIO_NAME_CODEC);
        }
        audioManager->SetAudioDeviceActive(*activedChannels, IAudioManager::AUDIO_OUTPUT_ACTIVE);

        /* record the audio output channel */
        String st;
        List<String>::Iterator iter = audioOutputChannels.Begin();
        for(; iter != audioOutputChannels.End(); ++iter){
            if (st.IsNull()){
                st = *iter;
            }
            else {
                st = st + "," + *iter;
            }
        }
        systemSettings->PutString(cr, ISettingsSystem::AUDIO_OUTPUT_CHANNEL, st, &res);
        Logger::D(TAG, "audio channels change to {%s}", st.string());
    }

    Slogger::I(TAG, "== END == Initializing Services... ");

    //todo: Notify Init2
    {
        Lock();
        NotifyAll();
        Unlock();
    }

    Looper::Loop();
    Slogger::D(TAG, "System ServerThread is exiting!");
    return NOERROR;
}


CAR_INTERFACE_IMPL(ServerThread::SystemReadyRunnable, IRunnable)

ServerThread::SystemReadyRunnable::SystemReadyRunnable()
    : mImm(NULL)
    , mWallpaper(NULL)
    , mNetworkManage(NULL)
    , mNetworkStats(NULL)
    , mNetworkPolicy(NULL)
    , mConnectivity(NULL)
    , mUsb(NULL)
    , mMountService(NULL)
    , mThrottle(NULL)
    , mBattery(NULL)
    , mAppWidget(NULL)
    , mStatusBar(NULL)
    , mRecognition(NULL)
    , mTextServiceManagerService(NULL)
    , mCountryDetector(NULL)
    , mNetworkTimeUpdater(NULL)
    , mCommonTimeMgmtService(NULL)
    , mTwilight(NULL)
    , mUiMode(NULL)
    , mLocationF(NULL)
    , mContext(NULL)
    , mHeadless(FALSE)
{}

ECode ServerThread::SystemReadyRunnable::Run()
{
    Slogger::I(TAG, "Making services ready.");
    if (!mHeadless) ServerThread::StartSystemUi(mContext);
//            try {
    if (mMountService != NULL) mMountService->SystemReady();
//            } catch (Throwable e) {
//                reportWtf("making Mount Service ready", e);
//            }
//            try {
    if (mBattery != NULL) mBattery->SystemReady();
//            } catch (Throwable e) {
//                reportWtf("making Battery Service ready", e);
//            }
//            try {
    if (mNetworkManage != NULL) mNetworkManage->SystemReady();
//            } catch (Throwable e) {
//                reportWtf("making Network Managment Service ready", e);
//            }
//            try {
//                if (networkStatsF != null) networkStatsF.systemReady();
//            } catch (Throwable e) {
//                reportWtf("making Network Stats Service ready", e);
//            }
//            try {
//                if (networkPolicyF != null) networkPolicyF.systemReady();
//            } catch (Throwable e) {
//                reportWtf("making Network Policy Service ready", e);
//            }
//            try {
    if (mConnectivity != NULL) mConnectivity->SystemReady();
//            } catch (Throwable e) {
//                reportWtf("making Connectivity Service ready", e);
//            }
//            try {
//                if (dockF != null) dockF.systemReady();
//            } catch (Throwable e) {
//                reportWtf("making Dock Service ready", e);
//            }
//            try {
    if (mUsb != NULL) mUsb->SystemReady();
//            } catch (Throwable e) {
//                reportWtf("making USB Service ready", e);
//            }
//            try {
    if (mTwilight != NULL) mTwilight->SystemReady();
//            } catch (Throwable e) {
//                reportWtf("makin Twilight Service ready", e);
//            }
//            try {
    if (mUiMode != NULL) mUiMode->SystemReady();
//            } catch (Throwable e) {
//                reportWtf("making UI Mode Service ready", e);
//            }
//            try {
    if (mRecognition != NULL) mRecognition->SystemReady();
//            } catch (Throwable e) {
//                reportWtf("making Recognition Service ready", e);
//            }
//            Watchdog.getInstance().start();
//
//            // It is now okay to let the various system services start their
//            // third party code...
//
//            try {
    if (mAppWidget != NULL) mAppWidget->SystemReady(/*safeMode*/TRUE);
//            } catch (Throwable e) {
//                reportWtf("making App Widget Service ready", e);
//            }
//            try {
    if (mWallpaper != NULL) mWallpaper->SystemReady();
//            } catch (Throwable e) {
//                reportWtf("making Wallpaper Service ready", e);
//            }
//            try {
    if (mImm != NULL) mImm->SystemReady(mStatusBar);
//            } catch (Throwable e) {
//                reportWtf("making Input Method Service ready", e);
//            }
//            try {
    if (mLocationF != NULL) mLocationF->SystemReady();
//            } catch (Throwable e) {
//                reportWtf("making Location Service ready", e);
//            }
//            try {
    if (mCountryDetector != NULL) mCountryDetector->SystemReady();
//            } catch (Throwable e) {
//                reportWtf("making Country Detector Service ready", e);
//            }
//            try {
    if (mThrottle != NULL) mThrottle->SystemReady();
//            } catch (Throwable e) {
//                reportWtf("making Throttle Service ready", e);
//            }
//            try {
    if (mNetworkTimeUpdater != NULL) mNetworkTimeUpdater->SystemReady();
//            } catch (Throwable e) {
//                reportWtf("making Network Time Service ready", e);
//            }
//            try {
    if (mCommonTimeMgmtService != NULL) mCommonTimeMgmtService->SystemReady();
//            } catch (Throwable e) {
//                reportWtf("making Common time management service ready", e);
//            }
//            try {
    if (mTextServiceManagerService != NULL) mTextServiceManagerService->SystemReady();
//            } catch (Throwable e) {
//                reportWtf("making Text Services Manager Service ready", e);
//            }
//            try {
//                if (dreamyF != null) dreamyF.systemReady();
//            } catch (Throwable e) {
//                reportWtf("making DreamManagerService ready", e);
//            }
//            try {
//                // TODO(BT) Pass parameter to input manager
//                if (inputManagerF != null) inputManagerF.systemReady();
//            } catch (Throwable e) {
//                reportWtf("making InputManagerService ready", e);
//            }
//            try {
//                if (telephonyRegistryF != null) telephonyRegistryF.systemReady();
//            } catch (Throwable e) {
//                reportWtf("making TelephonyRegistry ready", e);
//            }

    return NOERROR;
}

void ServerThread::StartSystemUi(
    /* [in] */ IContext* context)
{
    AutoPtr<IComponentName> cn, result;
    CComponentName::New(
        String("SystemUI"),
        String("SystemUI.CSystemUIService"),
        (IComponentName**)&cn);

    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    AutoPtr<IUserHandle> uh;
    helper->GetOWNER((IUserHandle**)&uh);

    AutoPtr<IIntent> intent;
    CIntent::New((IIntent**)&intent);
    intent->SetComponent(cn);
    context->StartServiceAsUser(intent, uh, (IComponentName**)&result);
}

const Int32 CSystemServer::FACTORY_TEST_OFF;
const Int32 CSystemServer::FACTORY_TEST_LOW_LEVEL;
const Int32 CSystemServer::FACTORY_TEST_HIGH_LEVEL;

ECode CSystemServer::Init1(
    /* [in] */ const ArrayOf<String>& args)
{
    android::sp<android::IServiceManager> sm = android::defaultServiceManager();

    android::sp<GrimReaper> grim = new GrimReaper();
    sm->asBinder()->linkToDeath(grim, grim.get(), 0);

    android::sp<android::IBinder> esm = sm->getService(android::String16("elastos.servicemanager"));
    Slogger::I(TAG, "ServiceManager: %p\n", esm.get());
    if (esm == 0) {
        Slogger::E(TAG, "Get Elastos service manager fail.\n");
        return E_RUNTIME_EXCEPTION;
    }
    esm->linkToDeath(grim, grim.get(), 0);

    char propBuf[PROPERTY_VALUE_MAX];
    property_get("system_init.startsurfaceflinger", propBuf, "1");
    if (strcmp(propBuf, "1") == 0) {
        // Start the SurfaceFlinger
        android::SurfaceFlinger::instantiate();
    }

    property_get("system_init.startsensorservice", propBuf, "1");
    if (strcmp(propBuf, "1") == 0) {
        // Start the sensor service
        android::SensorService::instantiate();
    }

    Slogger::I(TAG, "System server: starting Elastos services.\n");
    FAIL_RETURN(Init2());

    ALOGI("System server: entering thread pool.\n");
    android::ProcessState::self()->startThreadPool();
    android::IPCThreadState::self()->joinThreadPool();
    ALOGI("System server: exiting thread pool.\n");
    return NOERROR;
}

ECode CSystemServer::Init2()
{
    Slogger::I(TAG, "Entered the Android system server!");
    mThread = new ServerThread();
    mThread->SetName(String("elastos.server.ServerThread"));
    ECode ec = mThread->Start();
    //todo: wait for mThread
    {
        ISynchronize* sync = ISynchronize::Probe(mThread);
        sync->Lock();
        sync->Wait();
        sync->Unlock();
    }
    return ec;
}

ECode CSystemServer::Main(
    /* [in] */ const ArrayOf<String>& args)
{
    return Init1(args);
}

} // namespace Server
} // namespace Droid
} // namespace Elastos
