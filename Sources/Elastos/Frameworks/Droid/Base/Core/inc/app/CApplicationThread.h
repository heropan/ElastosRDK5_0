
#ifndef __CAPPLICATIONTHREAD_H__
#define __CAPPLICATIONTHREAD_H__

#include "_CApplicationThread.h"
#include "app/CActivityThread.h"

using Elastos::Core::Mutex;
using Elastos::IO::IFileDescriptor;
using Elastos::IO::IPrintWriter;
using Elastos::Utility::IObjectStringMap;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentReceiver;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::Pm::IActivityInfo;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::Pm::IServiceInfo;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Content::Res::ICompatibilityInfo;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IDebugMemoryInfo;
using Elastos::Droid::Os::IParcelFileDescriptor;

namespace Elastos {
namespace Droid {
namespace App {

CarClass(CApplicationThread)
{
    friend class CActivityThread;

public:
    CARAPI SchedulePauseActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean finished,
        /* [in] */ Boolean userLeaving,
        /* [in] */ Int32 configChanges);

    CARAPI ScheduleStopActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean showWindow,
        /* [in] */ Int32 configChanges);

    CARAPI ScheduleWindowVisibility(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean showWindow);

    CARAPI ScheduleSleeping(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean sleeping);

    CARAPI ScheduleResumeActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean isForward);

    CARAPI ScheduleSendResult(
        /* [in] */ IBinder* token,
        /* [in] */ IObjectContainer* results);

    // we use token to identify this activity without having to send the
    // activity itself back to the activity manager. (matters more with ipc)
    CARAPI ScheduleLaunchActivity(
        /* [in] */ IIntent* intent,
        /* [in] */ IBinder* token,
        /* [in] */ Int32 ident,
        /* [in] */ IActivityInfo* info,
        /* [in] */ IConfiguration* curConfig,
        /* [in] */ ICompatibilityInfo* compatInfo,
        /* [in] */ IBundle* state,
        /* [in] */ IObjectContainer* pendingResults,
        /* [in] */ IObjectContainer* pendingNewIntents,
        /* [in] */ Boolean notResumed,
        /* [in] */ Boolean isForward,
        /* [in] */ const String& profileName,
        /* [in] */ IParcelFileDescriptor* profileFd,
        /* [in] */ Boolean autoStopProfiler);

    CARAPI ScheduleRelaunchActivity(
        /* [in] */ IBinder* token,
        /* [in] */ ArrayOf<IResultInfo*>* pendingResults,
        /* [in] */ ArrayOf<IIntent*>* pendingNewIntents,
        /* [in] */ Int32 configChanges,
        /* [in] */ Boolean notResumed,
        /* [in] */ IConfiguration* config);

    CARAPI ScheduleNewIntent(
        /* [in] */ IObjectContainer* intents,
        /* [in] */ IBinder* token);

    CARAPI ScheduleDestroyActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean finishing,
        /* [in] */ Int32 configChanges);

    CARAPI ScheduleReceiver(
        /* [in] */ IIntent* intent,
        /* [in] */ IActivityInfo* info,
        /* [in] */ ICompatibilityInfo* compatInfo,
        /* [in] */ Int32 resultCode,
        /* [in] */ const String& data,
        /* [in] */ IBundle* extras,
        /* [in] */ Boolean sync,
        /* [in] */ Int32 sendingUser);

    CARAPI ScheduleCreateBackupAgent(
        /* [in] */ IApplicationInfo* app,
        /* [in] */ ICompatibilityInfo* compatInfo,
        /* [in] */ Int32 backupMode);

    CARAPI ScheduleDestroyBackupAgent(
        /* [in] */ IApplicationInfo* app,
        /* [in] */ ICompatibilityInfo* compatInfo);

    CARAPI ScheduleCreateService(
        /* [in] */ IBinder* token,
        /* [in] */ IServiceInfo* info,
        /* [in] */ ICompatibilityInfo* compatInfo);

    CARAPI ScheduleBindService(
        /* [in] */ IBinder* token,
        /* [in] */ IIntent* intent,
        /* [in] */ Boolean rebind);

    CARAPI ScheduleUnbindService(
        /* [in] */ IBinder* token,
        /* [in] */ IIntent* intent);

    CARAPI ScheduleServiceArgs(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean taskRemoved,
        /* [in] */ Int32 startId,
        /* [in] */ Int32 flags,
        /* [in] */ IIntent* args);

    CARAPI ScheduleStopService(
        /* [in] */ IBinder* token);

    CARAPI BindApplication(
        /* [in] */ const String& processName,
        /* [in] */ IApplicationInfo* appInfo,
        /* [in] */ IObjectContainer* providers,
        /* [in] */ IComponentName* instrumentationName,
        /* [in] */ const String& profileFile,
        /* [in] */ IParcelFileDescriptor* profileFd,
        /* [in] */ Boolean autoStopProfiler,
        /* [in] */ IBundle* instrumentationArgs,
        /* [in] */ IInstrumentationWatcher* instrumentationWatcher,
        /* [in] */ Int32 debugMode,
        /* [in] */ Boolean enableOpenGlTrace,
        /* [in] */ Boolean isRestrictedBackupMode,
        /* [in] */ Boolean persistent,
        /* [in] */ IConfiguration* config,
        /* [in] */ ICompatibilityInfo* compatInfo,
        /* [in] */ IObjectStringMap* services,
        /* [in] */ IBundle* coreSettings);

    CARAPI ScheduleExit();

    CARAPI ScheduleSuicide();

    CARAPI RequestThumbnail(
        /* [in] */ IBinder* token);

    CARAPI ScheduleConfigurationChanged(
        /* [in] */ IConfiguration* config);

    CARAPI UpdateTimeZone();

    CARAPI ClearDnsCache();

    CARAPI SetHttpProxy(
        /* [in] */ const String& host,
        /* [in] */ const String& port,
        /* [in] */ const String& exclList);

    CARAPI ProcessInBackground();

    CARAPI DumpService(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IBinder* servicetoken,
        /* [in] */ ArrayOf<String>* args);

    // This function exists to make sure all receiver dispatching is
    // correctly ordered, since these are one-way calls and the binder driver
    // applies transaction ordering per object for such calls.
    CARAPI ScheduleRegisteredReceiver(
        /* [in] */ IIntentReceiver* receiver,
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 resultCode,
        /* [in] */ const String& dataStr,
        /* [in] */ IBundle* extras,
        /* [in] */ Boolean ordered,
        /* [in] */ Boolean sticky,
        /* [in] */ Int32 sendingUser);

    CARAPI ScheduleLowMemory();

    CARAPI ScheduleActivityConfigurationChanged(
        /* [in] */ IBinder* token);

    CARAPI ProfilerControl(
        /* [in] */ Boolean start,
        /* [in] */ const String& path,
        /* [in] */ IParcelFileDescriptor* fd,
        /* [in] */ Int32 profileType);

    CARAPI DumpHeap(
        /* [in] */ Boolean managed,
        /* [in] */ const String& path,
        /* [in] */ IParcelFileDescriptor* fd);

    CARAPI SetSchedulingGroup(
        /* [in] */ Int32 group);

    CARAPI GetMemoryInfo(
        /* [in] */ IDebugMemoryInfo* outInfo);

    CARAPI DispatchPackageBroadcast(
        /* [in] */ Int32 cmd,
        /* [in] */ ArrayOf<String>* packages);

    CARAPI ScheduleCrash(
        /* [in] */ const String& msg);

    CARAPI DumpActivity(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IBinder* activitytoken,
        /* [in] */ const String& prefix,
        /* [in] */ ArrayOf<String>* args);

    CARAPI DumpProvider(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IBinder* providertoken,
        /* [in] */ ArrayOf<String>* args);

    CARAPI DumpMemInfo(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Boolean checkin,
        /* [in] */ Boolean all,
        /* [in] */ ArrayOf<String>* args,
        /* [out] */ IDebugMemoryInfo** mInfo);

    CARAPI DumpGfxInfo(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ ArrayOf<String>* args);

    CARAPI DumpDbInfo(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ ArrayOf<String>* args);

    CARAPI UnstableProviderDied(
        /* [in] */ IBinder* provider);

    CARAPI_(void) PrintRow(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& format,
        /* [in] */ ArrayOf<IInterface>* objs);

    CARAPI SetCoreSettings(
        /* [in] */ IBundle* coreSettings);

    CARAPI UpdatePackageCompatibilityInfo(
        /* [in] */ const String& pkg,
        /* [in] */ ICompatibilityInfo* info);

    CARAPI ScheduleTrimMemory(
        /* [in] */ Int32 level);

    CARAPI ToString(
        /* [out] */ String* string);

private:
    CARAPI_(void) UpdatePendingConfiguration(
        /* [in] */ IConfiguration* config);

    CARAPI_(AutoPtr<IDebugMemoryInfo>) DumpMemInfo(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ Boolean checkin,
        /* [in] */ Boolean all);

private:
    static const String HEAP_COLUMN;
    static const String ONE_COUNT_COLUMN;
    static const String TWO_COUNT_COLUMNS;
    static const String DB_INFO_FORMAT;

    // Formatting for checkin service - update version if row format changes
    static const Int32 ACTIVITY_THREAD_CHECKIN_VERSION = 1;

    CActivityThread* mAThread;//CActivityThread is host of CApplicationThread

    Mutex mPackagesLock;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__CACTIVITYTHREADAPPLICATIONTHREAD_H__
