
#ifndef __ELASTOS_DROID_SERVER_USB_USBDEVICEMANAGER_H__
#define __ELASTOS_DROID_SERVER_USB_USBDEVICEMANAGER_H__

#include "ext/frameworkext.h"
#include "os/UEventObserver.h"
#include "os/HandlerBase.h"
#include "usb/UsbSettingsManager.h"
#include "usb/UsbDebuggingManager.h"
#include "database/ContentObserver.h"
#include "content/BroadcastReceiver.h"
#include <elastos/utility/etl/List.h>
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::List;
using Elastos::Utility::Etl::Pair;
using Elastos::Utility::Etl::HashMap;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Os::IParcelFileDescriptor;
using Elastos::Droid::Os::IPowerManagerWakeLock;
using Elastos::Droid::App::INotificationManager;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Hardware::Usb::IUsbAccessory;
using Elastos::Droid::Database::ContentObserver;
using Elastos::Droid::Database::IContentObserver;

//class UsbSettingsManager;
namespace Elastos {
namespace Droid {
namespace Server {
namespace Usb {

typedef List< Pair<String, String> > StringPairList;

/**
 * UsbDeviceManager manages USB state in device mode.
 */
class UsbDeviceManager : public ElRefBase
{
private:
    class AdbSettingsObserver : public ContentObserver
    {
    public:
        AdbSettingsObserver(
            /* [in] */ UsbDeviceManager* host);

        CARAPI OnChange(
            /* [in] */ Boolean selfChange);

    private:
        UsbDeviceManager* mHost;
    };

    /*
     * Listens for uevent messages from the kernel to monitor the USB state
     */
    class MyUEventObserver : public UEventObserver
    {
    public:
        MyUEventObserver(
            /* [in] */ UsbDeviceManager* host);

        CARAPI_(void) OnUEvent(
            /* [in] */ UEvent* event);

    private:
        UsbDeviceManager* mHost;
    };

    class UsbHandler
        : public HandlerBase
    {
    private:
        class BootCompletedReceiver : public BroadcastReceiver
        {
        public:
            BootCompletedReceiver(
                /* [in] */ UsbHandler* host);

            CARAPI OnReceive(
                /* [in] */ IContext* context,
                /* [in] */ IIntent* intent);

            CARAPI ToString(
                /* [out] */ String* info)
            {
                VALIDATE_NOT_NULL(info);
                *info = String("UsbHandler::BootCompletedReceiver: ");
                (*info).AppendFormat("%p", this);
                return NOERROR;
            }
        private:
            UsbHandler* mHost;
        };

        class UserSwitchedReceiver : public BroadcastReceiver
        {
        public:
            UserSwitchedReceiver(
                /* [in] */ UsbHandler* host);

            CARAPI OnReceive(
                /* [in] */ IContext* context,
                /* [in] */ IIntent* intent);

            CARAPI ToString(
                /* [out] */ String* info)
            {
                VALIDATE_NOT_NULL(info);
                *info = String("UsbHandler::UserSwitchedReceiver: ");
                (*info).AppendFormat("%p", this);
                return NOERROR;
            }
        private:
            UsbHandler* mHost;
        };

        class BootFastReceiver : public BroadcastReceiver
        {
        public:
            BootFastReceiver(
                /* [in] */ UsbHandler* host);

            CARAPI OnReceive(
                /* [in] */ IContext* context,
                /* [in] */ IIntent* intent);

            CARAPI ToString(
                /* [out] */ String* info)
            {
                VALIDATE_NOT_NULL(info);
                *info = String("UsbHandler::BootFastReceiver: ");
                (*info).AppendFormat("%p", this);
                return NOERROR;
            }
        private:
            UsbHandler* mHost;
        };

    public:
        UsbHandler(
            /* [in] */ UsbDeviceManager* host,
            /* [in] */ ILooper* looper);

        CARAPI Init();

        virtual CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

        CARAPI_(void) UpdateState(
            /* [in] */ const String& state);

        using HandlerBase::SendMessage;

        void SendMessage(
            /* [in] */ Int32 what,
            /* [in] */ Boolean arg);

        void SendMessage(
            /* [in] */ Int32 what,
            /* [in] */ IInterface* arg);

        void SendMessage(
            /* [in] */ Int32 what,
            /* [in] */ IInterface* arg1,
            /* [in] */ Boolean arg2);

        CARAPI_(void) HandleMsgUpdateState(
            /* [in] */ Boolean connected,
            /* [in] */ Boolean configured);

        CARAPI_(void) HandleMsgEnableAdb(
            /* [in] */ Boolean enabled);

        CARAPI_(void) HandleMsgSetCurrentFunctions(
            /* [in] */ const String& str,
            /* [in] */ Boolean useDeafault);

        CARAPI_(void) HandleMsgSystemReady();

        CARAPI_(void) HandleMsgBootCompleted();

        CARAPI_(void) HandleMsgUserSwitched(
            /* [in] */ Int32 currentUser);

        CARAPI_(void) HandleMsgBootFastSwitched(
            /* [in] */ Int32 boot);

        CARAPI_(AutoPtr<IUsbAccessory>) GetCurrentAccessory();

    private:
        CARAPI_(Boolean) WaitForState(
            /* [in] */ const String& state);

        CARAPI_(Boolean) SetUsbConfig(
            /* [in] */ const String& config);

        CARAPI_(void) SetAdbEnabled(
            /* [in] */ Boolean enable);

        CARAPI_(void) SetEnabledFunctions(
            /* [in] */ const String& functions,
            /* [in] */ Boolean makeDefault);

        CARAPI_(void) UpdateCurrentAccessory();

        CARAPI_(void) UpdateUsbState();

        CARAPI_(void) UpdateAudioSourceFunction();

        CARAPI_(void) UpdateUsbNotification();

        CARAPI_(void) UpdateAdbNotification();

        // public void dump(FileDescriptor fd, PrintWriter pw)

    private:
        // current USB state
        UsbDeviceManager* mHost;
        Boolean mConnected;
        Boolean mConfigured;
        String mCurrentFunctions;
        String mDefaultFunctions;
        AutoPtr<IUsbAccessory> mCurrentAccessory;
        Int32 mUsbNotificationId;
        Boolean mAdbNotificationShown;
        Int32 mCurrentUser;

        AutoPtr<IBroadcastReceiver> mBootCompletedReceiver;
        AutoPtr<IBroadcastReceiver> mUserSwitchedReceiver;
        AutoPtr<IBroadcastReceiver> mBootFastReceiver;
    };

public:
    UsbDeviceManager(
        /* [in] */ IContext* context);

    ~UsbDeviceManager();

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI_(void) SetCurrentSettings(
        /* [in] */ UsbSettingsManager* settings);

    CARAPI_(void) SystemReady();

    /* returns the currently attached USB accessory */
    CARAPI_(AutoPtr<IUsbAccessory>) GetCurrentAccessory();

    /* opens the currently attached USB accessory */
    CARAPI OpenAccessory(
        /* [in] */ IUsbAccessory* accessory,
        /* [out] */ IParcelFileDescriptor** pfd);

    CARAPI_(void) SetCurrentFunctions(
        /* [in] */ const String& functions,
        /* [in] */ Boolean makeDefault);

    CARAPI_(void) SetMassStorageBackingFile(
        /* [in] */ const String& path);

    CARAPI_(void) AllowUsbDebugging(
        /* [in] */ Boolean alwaysAllow,
        /* [in] */ const String& publicKey);

    CARAPI_(void) DenyUsbDebugging();

    // public void dump(FileDescriptor fd, PrintWriter pw);

private:
    CARAPI_(AutoPtr<UsbSettingsManager>) GetCurrentSettings();

    CARAPI_(void) EnableWakeLock(
        /* [in] */ Boolean enable);

    CARAPI_(void) StartAccessoryMode();

    static CARAPI_(void) InitRndisAddress();

    static CARAPI_(String) AddFunction(
        /* [in] */ const String& functions,
        /* [in] */ const String& function);

    static CARAPI_(String) RemoveFunction(
        /* [in] */ const String& functions,
        /* [in] */ const String& function);

    static CARAPI_(Boolean) ContainsFunction(
        /* [in] */ const String& functions,
        /* [in] */ const String& function);

    CARAPI_(void) ReadOemUsbOverrideConfig();

    CARAPI_(Boolean) NeedsOemUsbOverride();

    CARAPI_(String) ProcessOemUsbOverride(
        /* [in] */ const String& usbFunctions);

    CARAPI_(AutoPtr< ArrayOf<String> >) NativeGetAccessoryStrings();

    CARAPI_(AutoPtr<IParcelFileDescriptor>) NativeOpenAccessory();

    CARAPI_(Boolean) NativeIsStartRequested();

    CARAPI_(Int32) NativeGetAudioMode();

private:
    static const String TAG;
    static const Boolean DEBUG;

    static const String USB_STATE_MATCH;
    static const String ACCESSORY_START_MATCH;
    static const String FUNCTIONS_PATH;
    static const String STATE_PATH;
    static const String MASS_STORAGE_FILE_PATH;
    static const String RNDIS_ETH_ADDR_PATH;
    static const String AUDIO_SOURCE_PCM_PATH;

    static const Int32 AUDIO_MODE_NONE = 0;
    static const Int32 AUDIO_MODE_SOURCE = 1;

    //add by kinier for detect usb port
    static const String USB_PORT_PM_STATE;

    static const String USB_PORT_STATE_SYS_PATH;

    //add by kinier end

    static const Int32 MSG_UPDATE_STATE;
    static const Int32 MSG_ENABLE_ADB;
    static const Int32 MSG_SET_CURRENT_FUNCTIONS;
    static const Int32 MSG_SYSTEM_READY;
    static const Int32 MSG_BOOT_COMPLETED;
    static const Int32 MSG_USER_SWITCHED;
    static const Int32 MSG_BOOTFAST_SWITCHED;

    // Delay for debouncing USB disconnects.
    // We often get rapid connect/disconnect events when enabling USB functions,
    // which need debouncing.
    static const Int32 UPDATE_DELAY;

    static const String BOOT_MODE_PROPERTY;

    static const String DRIVER_NAME;

    AutoPtr<UsbHandler> mHandler;

    Boolean mBootCompleted;

    Object mLock;

    AutoPtr<IContext> mContext;
    AutoPtr<IContentResolver> mContentResolver;
    // @GuardedBy("mLock")
    AutoPtr<UsbSettingsManager> mCurrentSettings;
    AutoPtr<INotificationManager> mNotificationManager;
    Boolean mHasUsbAccessory;
    Boolean mUseUsbNotification;
    Boolean mAdbEnabled;
    Boolean mAudioSourceEnabled;

    AutoPtr<HashMap<String, AutoPtr<StringPairList> > > mOemModeMap;

    AutoPtr<ArrayOf<String> > mAccessoryStrings;
    AutoPtr<UsbDebuggingManager> mDebuggingManager;

    AutoPtr<IPowerManagerWakeLock> mWl;
    Int32 mWlref;

    AutoPtr<MyUEventObserver> mUEventObserver;

    friend class UsbHandler;
};

} // namespace Usb
} // namespace Server
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SERVER_USB_USBDEVICEMANAGER_H__
