#ifndef __ELASTOS_DROID_SERVER_PM_CPACKAGEMANAGERSERVICE_H__
#define __ELASTOS_DROID_SERVER_PM_CPACKAGEMANAGERSERVICE_H__

#include "elastos/droid/ext/frameworkext.h"
#include "_Elastos_Droid_Server_Pm_CPackageManagerService.h"
#include "pm/PackageVerificationResponse.h"
#include "pm/PackageVerificationState.h"
#include "pm/Installer.h"
#include "pm/Settings.h"
#include "pm/CUserManagerService.h"
#include "pm/ServiceThread.h"
#include "pm/PackageInstallerService.h"
#include "elastos/droid/content/pm/PackageParser.h"
#include "elastos/droid/content/BroadcastReceiver.h"
#include "elastos/droid/os/Build.h"
#include "elastos/droid/os/FileObserver.h"
#include "elastos/droid/os/Handler.h"
#include "elastos/droid/os/Runnable.h"
#include <elastos/core/Thread.h>
#include <elastos/utility/etl/List.h>
#include <elastos/utility/etl/HashMap.h>
#include <elastos/utility/etl/HashSet.h>

using Elastos::Core::IInteger32;
using Elastos::Core::Thread;
using Elastos::IO::IFile;
using Elastos::IO::IFilenameFilter;
using Elastos::Utility::Etl::List;
using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::Etl::HashSet;
using Elastos::Utility::ISet;
using Elastos::Utility::IArrayList;
using Elastos::Utility::IArrayMap;
using Elastos::Utility::IHashMap;
using Elastos::Utility::IList;
using Elastos::Utility::Concurrent::Atomic::IAtomicInteger64;
using Elastos::Utility::Concurrent::Atomic::IAtomicBoolean;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::IServiceConnection;
using Elastos::Droid::Content::IIntentReceiver;
using Elastos::Droid::Content::IIntentSender;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::Pm::IActivityInfo;
using Elastos::Droid::Content::Pm::IServiceInfo;
using Elastos::Droid::Content::Pm::IResolveInfo;
using Elastos::Droid::Content::Pm::IFeatureInfo;
using Elastos::Droid::Content::Pm::IProviderInfo;
using Elastos::Droid::Content::Pm::IIPackageManager;
using Elastos::Droid::Content::Pm::IInstrumentationInfo;
using Elastos::Droid::Content::Pm::IPackageInfo;
using Elastos::Droid::Content::Pm::IPermissionGroupInfo;
using Elastos::Droid::Content::Pm::IParceledListSlice;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Pm::PackageParser;
using Elastos::Droid::Content::Pm::IManifestDigest;
using Elastos::Droid::Content::Pm::IPackageInstallObserver;
using Elastos::Droid::Content::Pm::IContainerEncryptionParams;
using Elastos::Droid::Content::Pm::IVerificationParams;
using Elastos::Droid::Content::Pm::IVerifierInfo;
using Elastos::Droid::Content::Pm::IPackageDeleteObserver;
using Elastos::Droid::Content::Pm::IPackageDataObserver;
using Elastos::Droid::Content::Pm::IPackageStatsObserver;
using Elastos::Droid::Content::Pm::IPackageMoveObserver;
using Elastos::Droid::Content::Pm::IPackageInfoLite;
using Elastos::Droid::Internal::App::IMediaContainerService;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IProcess;
using Elastos::Droid::Os::IFileObserver;
using Elastos::Droid::Os::FileObserver;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::Build;
using Elastos::Droid::Os::Handler;
using Elastos::Droid::Os::IHandlerThread;
using Elastos::Droid::Text::Format::IDateUtils;
using Elastos::Droid::Utility::ISparseArray;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {

class CPackageManagerService;

class InstallArgs : public Object
{
public:
    InstallArgs(
        /* [in] */ IUri* packageURI,
        /* [in] */ IPackageInstallObserver* observer,
        /* [in] */ Int32 flags,
        /* [in] */ const String& installerPackageName,
        /* [in] */ IManifestDigest* manifestDigest,
        /* [in] */ IUserHandle* user,
        /* [in] */ CPackageManagerService* owner);

    virtual ~InstallArgs() {};

    virtual CARAPI_(void) CreateCopyFile() = 0;

    virtual CARAPI CopyPkg(
        /* [in] */ IMediaContainerService* imcs,
        /* [in] */ Boolean temp,
        /* [out] */ Int32* result) = 0;

    virtual CARAPI_(Int32) DoPreInstall(
        /* [in] */ Int32 status) = 0;

    virtual CARAPI_(Boolean) DoRename(
        /* [in] */ Int32 status,
        /* [in] */ const String& pkgName,
        /* [in] */ const String& oldCodePath) = 0;

    virtual CARAPI_(Int32) DoPostInstall(
        /* [in] */ Int32 status,
        /* [in] */ Int32 uid) = 0;

    virtual CARAPI_(String) GetCodePath() = 0;

    virtual CARAPI_(String) GetResourcePath() = 0;

    virtual CARAPI_(String) GetNativeLibraryPath() = 0;

    // Need installer lock especially for dex file removal.
    virtual CARAPI_(void) CleanUpResourcesLI() = 0;

    virtual CARAPI_(Boolean) DoPostDeleteLI(
        /* [in] */ Boolean del) = 0;

    virtual CARAPI CheckFreeStorage(
        /* [in] */ IMediaContainerService* imcs,
        /* [out] */ Boolean* result) = 0;

    /**
     * Called before the source arguments are copied. This is used mostly
     * for MoveParams when it needs to read the source file to put it in the
     * destination.
     */
    virtual CARAPI_(Int32) DoPreCopy();

    /**
     * Called after the source arguments are copied. This is used mostly for
     * MoveParams when it needs to read the source file to put it in the
     * destination.
     *
     * @return
     */
    virtual CARAPI_(Int32) DoPostCopy(
        /* [in] */ Int32 uid);

    virtual CARAPI_(AutoPtr<IUserHandle>) GetUser();

protected:
    virtual CARAPI_(Boolean) IsFwdLocked();

public:
    AutoPtr<IPackageInstallObserver> mObserver;
    // Always refers to PackageManager flags only
    Int32 mFlags;
    AutoPtr<IUri> mPackageURI;
    String mInstallerPackageName;
    AutoPtr<IManifestDigest> mManifestDigest;
    AutoPtr<IUserHandle> mUser;
    CPackageManagerService* mHost;
    // for epk
    Boolean mIsEpk;
};

/**
 * Keep track of all those .apks everywhere.
 *
 * This is very central to the platform's security; please run the unit
 * tests whenever making modifications here:
 *
mmm frameworks/base/tests/AndroidTests
adb install -r -f out/target/product/passion/data/app/AndroidTests.apk
adb shell am instrument -w -e class com.android.unit_tests.PackageManagerTests com.android.unit_tests/android.test.InstrumentationTestRunner
 *
 * {@hide}
 */
CarClass(CPackageManagerService)
{
public:/* package */
    friend class CResourcesChangedReceiver;

    class SharedLibraryEntry : public Object
    {
    public:
        SharedLibraryEntry(
            /* [in] */ const String& path,
            /* [in] */ const String& apk)
            : mPath(path)
            , mApk(apk)
        {}

    public:
        String mPath;
        String mApk;
    };

    // Set of pending broadcasts for aggregating enable/disable of components.
    class PendingPackageBroadcasts : public Object
    {
    public:
        PendingPackageBroadcasts();

        CARAPI_(AutoPtr<IList>) Get(
            /* [in] */ Int32 userId,
            /* [in] */ const String& packageName);

        CARAPI_(void) Put(
            /* [in] */ Int32 userId,
            /* [in] */ const String& packageName,
            /* [in] */ IArrayList* components);

        CARAPI_(void) Remove(
            /* [in] */ Int32 userId,
            /* [in] */ const String& packageName);

        CARAPI_(void) Remove(
            /* [in] */ Int32 userId);

        CARAPI_(Int32) UserIdCount();

        CARAPI_(Int32) UserIdAt(
            /* [in] */ Int32 n);

        CARAPI_(AutoPtr<IHashMap>) PackagesForUserId(
            /* [in] */ Int32 userId);

        CARAPI_(Int32) Size();

        CARAPI_(void) Clear();

    private:
        CARAPI_(AutoPtr<IHashMap>) GetOrAllocate(
            /* [in] */ Int32 userId);

    public:
        // for each user id, a map of <package name -> components within that package>
        AutoPtr<ISparseArray> mUidMap;
    };

    class DefaultContainerConnection
        : public Object
        , public IServiceConnection
    {
    public:
        DefaultContainerConnection(
            /* [in] */ CPackageManagerService* owner)
            : mHost(owner)
        {}

        CAR_INTERFACE_DECL()

        CARAPI OnServiceConnected(
            /* [in] */ IComponentName* name,
            /* [in] */ IBinder* service);

        CARAPI OnServiceDisconnected(
            /* [in] */ IComponentName* name);

    private:
        CPackageManagerService* mHost;
    };

    // Recordkeeping of restore-after-install operations that are currently in flight
    // between the Package Manager and the Backup Manager
    class PostInstallData : public Object
    {
    public:
        PostInstallData(
            /* [in] */ InstallArgs* a,
            /* [in] */ PackageInstalledInfo* r)
            : mArgs(a)
            , mRes(r)
        {}

    public:
        AutoPtr<InstallArgs> mArgs;
        AutoPtr<PackageInstalledInfo> mRes;
    };

    class HandlerParams;

    class PackageHandler : public Handler
    {
    public:
        PackageHandler(
            /* [in] */ ILooper* looper,
            /* [in] */ CPackageManagerService* host)
            : HandlerBase(looper)
            , mBound(FALSE)
            , mHost(host)
        {}

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

        CARAPI_(void) DoHandleMessage(
            /* [in] */ IMessage* msg);

    private:
        CARAPI_(Boolean) ConnectToService();

        CARAPI_(void) DisconnectService();

    private:
        Boolean mBound;
        List<AutoPtr<HandlerParams> > mPendingInstalls
        CPackageManagerService* mHost;
    };

    class ActivityIntentResolver
        : public IntentResolver<PackageParser::ActivityIntentInfo, IResolveInfo>
    {
    public:
        typedef IntentResolver<PackageParser::ActivityIntentInfo, IResolveInfo>  Super;

    public:
        ActivityIntentResolver(
            /* [in] */ CPackageManagerService* owner);

        CARAPI_(AutoPtr<List<AutoPtr<IResolveInfo> > >) QueryIntent(
            /* [in] */ IIntent* intent,
            /* [in] */ const String& resolvedType,
            /* [in] */ Boolean defaultOnly,
            /* [in] */ Int32 userId);

        CARAPI_(AutoPtr<List<AutoPtr<IResolveInfo> > >) QueryIntent(
            /* [in] */ IIntent* intent,
            /* [in] */ const String& resolvedType,
            /* [in] */ Int32 flags,
            /* [in] */ Int32 userId);

        CARAPI_(AutoPtr<List<AutoPtr<IResolveInfo> > >) QueryIntentForPackage(
            /* [in] */ IIntent* intent,
            /* [in] */ const String& resolvedType,
            /* [in] */ Int32 flags,
            /* [in] */ List<AutoPtr<PackageParser::Activity> >* packageActivities,
            /* [in] */ Int32 userId);

        CARAPI_(void) AddActivity(
            /* [in] */ PackageParser::Activity* activity,
            /* [in] */ const String& type);

        CARAPI_(void) RemoveActivity(
            /* [in] */ PackageParser::Activity* activity,
            /* [in] */ const String& type);

    protected:
        //@Override
        CARAPI_(Boolean) AllowFilterResult(
            /* [in] */ PackageParser::ActivityIntentInfo* filter,
            /* [in] */ List<AutoPtr<IResolveInfo> > * dest);

        //@Override
        CARAPI_(AutoPtr<ArrayOf<PackageParser::ActivityIntentInfo*> >) NewArray(
            /* [in] */ Int32 size);

        //@Override
        CARAPI_(Boolean) IsFilterStopped(
            /* [in] */ PackageParser::ActivityIntentInfo* filter,
            /* [in] */ Int32 userId);

        //@Override
        CARAPI_(Boolean) IsPackageForFilter(
            /* [in] */ const String& packageName,
            /* [in] */ PackageParser::ActivityIntentInfo* info);

        //@Override
        CARAPI_(AutoPtr<IResolveInfo>) NewResult(
            /* [in] */ PackageParser::ActivityIntentInfo* info,
            /* [in] */ Int32 match,
            /* [in] */ Int32 userId);

        //@Override
        CARAPI_(void) SortResults(
            /* [in] */ List<AutoPtr<IResolveInfo> >* results);

        //@Override
        CARAPI_(void) DumpFilter(
            /* [in] */ IPrintWriter* out,
            /* [in] */ const String& prefix,
            /* [in] */ PackageParser::ActivityIntentInfo* filter);

    public:
        // Keys are String (activity class name), values are Activity.
        HashMap<AutoPtr<IComponentName>, AutoPtr<PackageParser::Activity> > mActivities;
        Int32 mFlags;
        CPackageManagerService* mHost;
    };

    class DeletePackageRunnable : public Runnable
    {
    public:
        DeletePackageRunnable(
            /* [in] */ const String& packageName,
            /* [in] */ IPackageDeleteObserver* observer,
            /* [in] */ Int32 flags,
            /* [in] */ Int32 uid,
            /* [in] */ CPackageManagerService* host)
            : mPackageName(packageName)
            , mObserver(observer)
            , mFlags(flags)
            , mUid(uid)
            , mHost(host)
        {}

        CARAPI Run();

    private:
        String mPackageName;
        AutoPtr<IPackageDeleteObserver> mObserver;
        Int32 mFlags;
        Int32 mUid;
        CPackageManagerService* mHost;
    };

    class ProcessPendingInstallRunnable : public Runnable
    {
    public:
        ProcessPendingInstallRunnable(
            /* [in] */ InstallArgs* args,
            /* [in] */ Int32 currentStatus,
            /* [in] */ CPackageManagerService* host)
            : mInstallArgs(args)
            , mCurrentStatus(currentStatus)
            , mHost(host)
        {}

        CARAPI Run();

    private:
        AutoPtr<InstallArgs> mInstallArgs;
        Int32 mCurrentStatus;
        CPackageManagerService* mHost;
    };

    class PackageInstalledInfo;

    class HandlerParams : public Object
    {
    public:
        HandlerParams()
            : mRetries(0)
            , mHost(NULL)
        {}

        HandlerParams(
            /* [in] */ IUserHandle* user,
            /* [in] */ CPackageManagerService* owner)
            : mRetries(0)
            , mUser(user)
            , mHost(owner)
        {}

        virtual ~HandlerParams() {};

        virtual CARAPI_(AutoPtr<IUserHandle>) GetUser()
        { return mUser; }

        virtual CARAPI_(Boolean) StartCopy();

        virtual CARAPI_(void) ServiceError();

        virtual CARAPI HandleStartCopy() = 0;

        virtual CARAPI_(void) HandleServiceError() = 0;

        virtual CARAPI_(void) HandleReturnCode() = 0;

    public:
        static const Int32 MAX_RETRIES = 4;

        /**
         * Number of times startCopy() has been attempted and had a non-fatal
         * error.
         */
        Int32 mRetries;

        /** User handle for the user requesting the information or installation. */
        AutoPtr<IUserHandle> mUser;

        CPackageManagerService* mHost;
    };

    class MeasureParams : public HandlerParams
    {
    public:
        MeasureParams(
            /* [in] */ IPackageStats* stats,
            /* [in] */ IUserHandle* userHandle,
            /* [in] */ IPackageStatsObserver* observer,
            /* [in] */ CPackageManagerService* owner);

        //@Override
        CARAPI HandleStartCopy();

        //@Override
        CARAPI_(void) HandleReturnCode();

        //@Override
        CARAPI_(void) HandleServiceError();

    private:
        AutoPtr<IPackageStats> mStats;
        Boolean mSuccess;

        AutoPtr<IPackageStatsObserver> mObserver;
    };

    class InstallParams : public HandlerParams
    {
    public:
        class CopyBroadcastReceiver
            : public BroadcastReceiver
        {
        public:
            CopyBroadcastReceiver(
                /* [in] */ CPackageManagerService* host,
                /* [in] */ Int32 id)
                : mHost(host)
                , mId(id)
            {}

            CARAPI OnReceive(
                /* [in] */ IContext* context,
                /* [in] */ IIntent* intent);

            CARAPI ToString(
                /* [out] */ String* info)
            {
                VALIDATE_NOT_NULL(info);
                *info = String("CPackageManagerService::InstallParams::CopyBroadcastReceiver: ");
                (*info).AppendFormat("%p", this);
                return NOERROR;
            }
        private:
            CPackageManagerService* mHost;
            Int32 mId;
        };

    public:
        InstallParams(
            /* [in] */ IUri* packageURI,
            /* [in] */ IPackageInstallObserver* observer,
            /* [in] */ Int32 flags,
            /* [in] */ const String& installerPackageName,
            /* [in] */ IVerificationParams* verificationParams,
            /* [in] */ IContainerEncryptionParams* encryptionParams,
            /* [in] */ IUserHandle* user,
            /* [in] */ CPackageManagerService* owner)
            : HandlerParams(user, owner)
            , mObserver(observer)
            , mFlags(flags)
            , mPackageURI(packageURI)
            , mInstallerPackageName(installerPackageName)
            , mVerificationParams(verificationParams)
            , mRet(0)
            , mEncryptionParams(encryptionParams)
        {
        }

        ~InstallParams() {};

        CARAPI_(AutoPtr<IManifestDigest>) GetManifestDigest();

        /*
         * Invoke remote method to get package information and install
         * location values. Override install location based on default
         * policy if needed and then create install arguments based
         * on the install location.
         */
        //@Override
        CARAPI HandleStartCopy();

        //@Override
        CARAPI_(void) HandleReturnCode();

        //@Override
        CARAPI_(void) HandleServiceError();

        CARAPI_(Boolean) IsForwardLocked();

        CARAPI_(AutoPtr<IUri>) GetPackageUri();

    private:
        CARAPI_(Int32) InstallLocationPolicy(
            /* [in] */ IPackageInfoLite* pkgLite,
            /* [in] */ Int32 flags);

    public:
        AutoPtr<IPackageInstallObserver> mObserver;
        Int32 mFlags;

        AutoPtr<IUri> mPackageURI;
        String mInstallerPackageName;
        AutoPtr<IVerificationParams> mVerificationParams;
        AutoPtr<InstallArgs> mArgs;
        Int32 mRet;
        AutoPtr<IFile> mTempPackage;
        AutoPtr<IContainerEncryptionParams> mEncryptionParams;
    };

    /*
     * Utility class used in movePackage api.
     * srcArgs and targetArgs are not set for invalid flags and make
     * sure to do null checks when invoking methods on them.
     * We probably want to return ErrorPrams for both failed installs
     * and moves.
     */
    class MoveParams : public HandlerParams
    {
    public:
        MoveParams(
            /* [in] */ InstallArgs* srcArgs,
            /* [in] */ IPackageMoveObserver* observer,
            /* [in] */ Int32 flags,
            /* [in] */ const String& packageName,
            /* [in] */ const String& dataDir,
            /* [in] */ Int32 uid,
            /* [in] */ IUserHandle* user,
            /* [in] */ CPackageManagerService* owner);

        //@Override
        CARAPI HandleStartCopy();

        //@Override
        CARAPI_(void) HandleReturnCode();

        //@Override
        CARAPI_(void) HandleServiceError();

    public:
        AutoPtr<IPackageMoveObserver> mObserver;
        Int32 mFlags;
        String mPackageName;
        AutoPtr<InstallArgs> mSrcArgs;
        AutoPtr<InstallArgs> mTargetArgs;
        Int32 mUid;
        Int32 mRet;
    };

    class FileInstallArgs : public InstallArgs
    {
    public:
        FileInstallArgs(
            /* [in] */ InstallParams* params,
            /* [in] */ CPackageManagerService* owner);

        FileInstallArgs(
            /* [in] */ const String& fullCodePath,
            /* [in] */ const String& fullResourcePath,
            /* [in] */ const String& nativeLibraryPath,
            /* [in] */ CPackageManagerService* owner);

        FileInstallArgs(
            /* [in] */ IUri* packageURI,
            /* [in] */ const String& pkgName,
            /* [in] */ const String& dataDir,
            /* [in] */ CPackageManagerService* owner);

        CARAPI CheckFreeStorage(
            /* [in] */ IMediaContainerService* imcs,
            /* [out] */ Boolean* result);

        CARAPI_(String) GetCodePath();

        CARAPI_(void) CreateCopyFile();

        CARAPI CopyPkg(
            /* [in] */ IMediaContainerService* imcs,
            /* [in] */ Boolean temp,
            /* [out] */ Int32* result);

        CARAPI_(Int32) DoPreInstall(
            /* [in] */ Int32 status);

        CARAPI_(Boolean) DoRename(
            /* [in] */ Int32 status,
            /* [in] */ const String& pkgName,
            /* [in] */ const String& oldCodePath);

        CARAPI_(Int32) DoPostInstall(
            /* [in] */ Int32 status,
            /* [in] */ Int32 uid);

        CARAPI_(String) GetResourcePath();

        virtual CARAPI_(String) GetResourcePathFromCodePath();

        //@Override
        CARAPI_(String) GetNativeLibraryPath();

        CARAPI_(void) CleanUpResourcesLI();

        CARAPI_(Boolean) DoPostDeleteLI(
            /* [in] */ Boolean del);

    private:
        CARAPI_(String) GetLibraryPathFromCodePath();

        CARAPI_(Boolean) CleanUp();

        CARAPI_(Boolean) SetPermissions();

    public:
        AutoPtr<IFile> mInstallDir;
        String mCodeFileName;
        String mResourceFileName;
        String mLibraryPath;
        Boolean mCreated;
    };

    class AsecInstallArgs : public InstallArgs
    {
    public:
        friend class CPackageManagerService;

        AsecInstallArgs(
            /* [in] */ InstallParams* params,
            /* [in] */ CPackageManagerService* owner);

        AsecInstallArgs(
            /* [in] */ const String& fullCodePath,
            /* [in] */ const String& fullResourcePath,
            /* [in] */ const String& nativeLibraryPath,
            /* [in] */ Boolean isExternal,
            /* [in] */ Boolean isForwardLocked,
            /* [in] */ CPackageManagerService* owner);

        AsecInstallArgs(
            /* [in] */ const String& cid,
            /* [in] */ Boolean isForwardLocked,
            /* [in] */ CPackageManagerService* owner);

        AsecInstallArgs(
            /* [in] */ IUri* packageURI,
            /* [in] */ const String& cid,
            /* [in] */ Boolean isExternal,
            /* [in] */ Boolean isForwardLocked,
            /* [in] */ CPackageManagerService* owner);

        CARAPI_(void) CreateCopyFile();

        CARAPI CheckFreeStorage(
            /* [in] */ IMediaContainerService* imcs,
            /* [out] */ Boolean* result);

        CARAPI CopyPkg(
            /* [in] */ IMediaContainerService* imcs,
            /* [in] */ Boolean temp,
            /* [out] */ Int32* result);

        CARAPI_(String) GetCodePath();

        CARAPI_(String) GetResourcePath();

        CARAPI_(String) GetNativeLibraryPath();

        CARAPI_(Int32) DoPreInstall(
            /* [in] */ Int32 status);

        CARAPI_(Boolean) DoRename(
            /* [in] */ Int32 status,
            /* [in] */ const String& pkgName,
            /* [in] */ const String& oldCodePath);

        CARAPI_(Int32) DoPostInstall(
            /* [in] */ Int32 status,
            /* [in] */ Int32 uid);

        CARAPI_(void) CleanUpResourcesLI();

        CARAPI_(Boolean) MatchContainer(
            /* [in] */ const String& app);

        CARAPI_(String) GetPackageName();

        CARAPI_(Boolean) DoPostDeleteLI(
            /* [in] */ Boolean del);

        CARAPI_(Int32) DoPreCopy();

        CARAPI_(Int32) DoPostCopy(
            /* [in] */ Int32 uid);

    private:
        CARAPI_(Boolean) IsExternal();

        CARAPI_(void) SetCachePath(
            /* [in] */ const String& newCachePath);

        CARAPI_(void) CleanUp();

    public:
        static const String RES_FILE_NAME;
        static const String PUBLIC_RES_FILE_NAME;

        String mCid;
        String mPackagePath;
        String mResourcePath;
        String mLibraryPath;
    };

    class PackageRemovedInfo;

    class PackageInstalledInfo : public ElRefBase
    {
    public:
        PackageInstalledInfo()
            : mUid(0)
            , mReturnCode(0)
        {}

    public:
        String mName;
        Int32 mUid;
        // The set of users that originally had this package installed.
        AutoPtr< ArrayOf<Int32> > mOrigUsers;
        // The set of users that now have this package installed.
        AutoPtr< ArrayOf<Int32> > mNewUsers;
        AutoPtr<PackageParser::Package> mPkg;
        Int32 mReturnCode;
        AutoPtr<PackageRemovedInfo> mRemovedInfo;
    };

    class PackageRemovedInfo : public ElRefBase
    {
    public:
        PackageRemovedInfo(
            /* [in] */ CPackageManagerService* owner)
            : mUid(-1)
            , mRemovedAppId(-1)
            , mIsRemovedPackageSystemUpdate(FALSE)
            , mHost(owner)
        {}

        CARAPI_(void) SendBroadcast(
            /* [in] */ Boolean fullRemove,
            /* [in] */ Boolean replacing,
            /* [in] */ Boolean removedForAllUsers);

    public:
        String mRemovedPackage;
        Int32 mUid;
        Int32 mRemovedAppId;
        AutoPtr< ArrayOf<Int32> > mRemovedUsers;
        Boolean mIsRemovedPackageSystemUpdate;
        // Clean up resources deleted capsules.
        AutoPtr<InstallArgs> mArgs;
        CPackageManagerService* mHost;
    };

private:
    class PackageUsage : public Object
    {
    private:
        class WriteThread : public Thread
        {
        public:
            WriteThread(
                /* [in] */ const String& threadName,
                /* [in] */ PackageUsage* host);

            CARAPI Run();

        private:
            PackageUsage* mHost;
        };

    public:
        PackageUsage(
            /* [in] */ CPackageManagerService* host);

        CARAPI_(Boolean) IsHistoricalPackageUsageAvailable();

        CARAPI_(void) Write(
            /* [in] */ Boolean force);

        CARAPI_(void) ReadLP();

    private:
        CARAPI_(void) WriteInternal();

        CARAPI ReadToken(
            /* [in] */ IInputStream* in,
            /* [in] */ StringBuilder sb,
            /* [in] */ Char32 endOfToken,
            /* [in] */ String* token);

        CARAPI_(AutoPtr<IAtomicFile>) GetFile();

    private:
        static const Int32 WRITE_INTERVAL = (DEBUG_DEXOPT) ? 0 : 30*60*1000; // 30m in ms

        Object mFileLock;
        AutoPtr<IAtomicInteger64> mLastWritten;
        AutoPtr<IAtomicBoolean> mBackgroundWriteRunning;

        Boolean mIsHistoricalPackageUsageAvailable;

        CPackageManagerService* mHost;

        friend class WriteThread;
    };

    class NotifyRunnable : public Runnable
    {
    public:
        NotifyRunnable(
            /* [in] */ CPackageManagerService* owner,
            /* [in] */ Int64 freeStorageSize,
            /* [in] */ IPackageDataObserver* observer)
            : mHost(owner)
            , mFreeStorageSize(freeStorageSize)
            , mObserver(observer)
        {}

        CARAPI Run();

    private:
        CPackageManagerService* mHost;
        Int32 mFreeStorageSize;
        AutoPtr<IPackageDataObserver> mObserver;
    };

    class FreeStorageRunnable : public Runnable
    {
    public:
        FreeStorageRunnable(
            /* [in] */ CPackageManagerService* owner,
            /* [in] */ Int64 freeStorageSize,
            /* [in] */ IIntentSender* pi)
            : mHost(owner)
            , mFreeStorageSize(freeStorageSize)
            , mPi(pi)
        {}

        CARAPI Run();

    private:
        CPackageManagerService* mHost;
        Int64 mFreeStorageSize;
        AutoPtr<IIntentSender> mPi;
    };

    class ServiceIntentResolver
        : public IntentResolver<PackageParser::ServiceIntentInfo, IResolveInfo>
    {
    public:
        typedef IntentResolver<PackageParser::ServiceIntentInfo, IResolveInfo> Super;

    public:
        ServiceIntentResolver(
            /* [in] */ CPackageManagerService* owner);

        CARAPI_(AutoPtr<List<AutoPtr<IResolveInfo> > >) QueryIntent(
            /* [in] */ IIntent* intent,
            /* [in] */ const String& resolvedType,
            /* [in] */ Boolean defaultOnly,
            /* [in] */ Int32 userId);

        CARAPI_(AutoPtr<List<AutoPtr<IResolveInfo> > >) QueryIntent(
            /* [in] */ IIntent* intent,
            /* [in] */ const String& resolvedType,
            /* [in] */ Int32 flags,
            /* [in] */ Int32 userId);

        CARAPI_(AutoPtr<List<AutoPtr<IResolveInfo> > >) QueryIntentForPackage(
            /* [in] */ IIntent* intent,
            /* [in] */ const String& resolvedType,
            /* [in] */ Int32 flags,
            /* [in] */ List< AutoPtr<PackageParser::Service> >* packageServices,
            /* [in] */ Int32 userId);

        CARAPI_(void) AddService(
            /* [in] */ PackageParser::Service* service);

        CARAPI_(void) RemoveService(
            /* [in] */ PackageParser::Service* service);

    protected:
        //@Override
        CARAPI_(Boolean) AllowFilterResult(
            /* [in] */ PackageParser::ServiceIntentInfo* filter,
            /* [in] */ List<AutoPtr<IResolveInfo> >* dest);

        //@Override
        CARAPI_(AutoPtr< ArrayOf<PackageParser::ServiceIntentInfo*> >) NewArray(
            /* [in] */ Int32 size);

        //@Override
        CARAPI_(Boolean) IsFilterStopped(
            /* [in] */ PackageParser::ServiceIntentInfo* filter,
            /* [in] */ Int32 userId);

        //@Override
        CARAPI_(Boolean) IsPackageForFilter(
            /* [in] */ const String& packageName,
            /* [in] */ PackageParser::ServiceIntentInfo* info);

        //@Override
        CARAPI_(AutoPtr<IResolveInfo>) NewResult(
            /* [in] */ PackageParser::ServiceIntentInfo* info,
            /* [in] */ Int32 match,
            /* [in] */ Int32 userId);

        //@Override
        CARAPI_(void) SortResults(
            /* [in] */ List<AutoPtr<IResolveInfo> >* results);

        // @Override
        CARAPI_(void) DumpFilter(
            /* [in] */ IPrintWriter* out,
            /* [in] */ const String& prefix,
            /* [in] */ PackageParser::ServiceIntentInfo* filter);

    public:
        // Keys are String (activity class name), values are Activity.
        HashMap<AutoPtr<IComponentName>, AutoPtr<PackageParser::Service> > mServices;
        Int32 mFlags;
        CPackageManagerService* mHost;
    };

    class ProviderIntentResolver
        : public IntentResolver<PackageParser::ProviderIntentInfo, IResolveInfo>
    {
    public:
        typedef IntentResolver<PackageParser::ProviderIntentInfo, IResolveInfo> Super;

    public:
        ProviderIntentResolver(
            /* [in] */ CPackageManagerService* owner);

        CARAPI_(AutoPtr<List<AutoPtr<IResolveInfo> > >) QueryIntent(
            /* [in] */ IIntent* intent,
            /* [in] */ const String& resolvedType,
            /* [in] */ Boolean defaultOnly,
            /* [in] */ Int32 userId);

        CARAPI_(AutoPtr<List<AutoPtr<IResolveInfo> > >) QueryIntent(
            /* [in] */ IIntent* intent,
            /* [in] */ const String& resolvedType,
            /* [in] */ Int32 flags,
            /* [in] */ Int32 userId);

        CARAPI_(AutoPtr<List<AutoPtr<IResolveInfo> > >) QueryIntentForPackage(
            /* [in] */ IIntent* intent,
            /* [in] */ const String& resolvedType,
            /* [in] */ Int32 flags,
            /* [in] */ List<AutoPtr<PackageParser::Service> >* packageServices,
            /* [in] */ Int32 userId);

        CARAPI_(void) AddProvider(
            /* [in] */ PackageParser::Provider* p);

        CARAPI_(void) Removeprovider(
            /* [in] */ PackageParser::Provider* p);

    protected:
        //@Override
        CARAPI_(Boolean) AllowFilterResult(
            /* [in] */ PackageParser::ProviderIntentInfo* filter,
            /* [in] */ List<AutoPtr<IResolveInfo> >* dest);

        //@Override
        CARAPI_(AutoPtr< ArrayOf<PackageParser::ProviderIntentInfo*> >) NewArray(
            /* [in] */ Int32 size);

        //@Override
        CARAPI_(Boolean) IsFilterStopped(
            /* [in] */ PackageParser::ProviderIntentInfo* filter,
            /* [in] */ Int32 userId);

        //@Override
        CARAPI_(Boolean) IsPackageForFilter(
            /* [in] */ const String& packageName,
            /* [in] */ PackageParser::ProviderIntentInfo* info);

        //@Override
        CARAPI_(AutoPtr<IResolveInfo>) NewResult(
            /* [in] */ PackageParser::ProviderIntentInfo* info,
            /* [in] */ Int32 match,
            /* [in] */ Int32 userId);

        //@Override
        CARAPI_(void) SortResults(
            /* [in] */ List<AutoPtr<IResolveInfo> >* results);

        // @Override
        CARAPI_(void) DumpFilter(
            /* [in] */ IPrintWriter* out,
            /* [in] */ const String& prefix,
            /* [in] */ PackageParser::ServiceIntentInfo* filter);

    public:
        // Keys are String (activity class name), values are Activity.
        HashMap<AutoPtr<IComponentName>, AutoPtr<PackageParser::Service> > mServices;
        Int32 mFlags;
        CPackageManagerService* mHost;
    };

    class ResolvePrioritySorter
        : public Object
        , public IComparator
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI Compare(
            /* [in] */ IInterface* lhs,
            /* [in] */ IInterface* rhs,
            /* [out] */ Int32* result);
    }

    class ProviderInitOrderSorter
        : public Object
        , public IComparator
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI Compare(
            /* [in] */ IInterface* lhs,
            /* [in] */ IInterface* rhs,
            /* [out] */ Int32* result);
    }

    class PackageComparator
        : public Object
        , public IComparator
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI Compare(
            /* [in] */ IInterface* lhs,
            /* [in] */ IInterface* rhs,
            /* [out] */ Int32* result);
    };

    class DeleteFilenameFilter
        : public ElRefBase
        , public IFilenameFilter
    {
    public:
        CARAPI Accept(
            /* [in] */ IFile* dir,
            /* [in] */ const String& filename,
            /* [out] */ Boolean* succeeded);

        CAR_INTERFACE_DECL()
    };

    class ClearRunnable
        : public ElRefBase
        , public IRunnable
    {
    public:
        ClearRunnable(
            /* [in] */ CPackageManagerService* owner,
            /* [in] */ const String& packageName,
            /* [in] */ IPackageDataObserver* observer,
            /* [in] */ Int32 userId)
            : mHost(owner)
            , mPackageName(packageName)
            , mObserver(observer)
            , mUserId(userId)
        {}

        CAR_INTERFACE_DECL()

        CARAPI Run();

    private:
        CPackageManagerService* mHost;
        String mPackageName;
        AutoPtr<IPackageDataObserver> mObserver;
        Int32 mUserId;
    };

    class DeleteRunnable
        : public ElRefBase
        , public IRunnable
    {
    public:
        DeleteRunnable(
            /* [in] */ CPackageManagerService* owner,
            /* [in] */ const String& packageName,
            /* [in] */ IPackageDataObserver* observer,
            /* [in] */ Int32 userId)
            : mHost(owner)
            , mPackageName(packageName)
            , mObserver(observer)
            , mUserId(userId)
        {}

        CAR_INTERFACE_DECL()

        CARAPI Run();

    private:
        CPackageManagerService* mHost;
        String mPackageName;
        AutoPtr<IPackageDataObserver> mObserver;
        Int32 mUserId;
    };

    class UpdateRunnable
        : public ElRefBase
        , public IRunnable
    {
    public:
        UpdateRunnable(
            /* [in] */ CPackageManagerService* owner,
            /* [in] */ Boolean mediaStatus,
            /* [in] */ Boolean reportStatus)
            : mHost(owner)
            , mMediaStatus(mediaStatus)
            , mReportStatus(reportStatus)
        {}

        CAR_INTERFACE_DECL()

        CARAPI Run();

    private:
        CPackageManagerService* mHost;
        Boolean mMediaStatus;
        Boolean mReportStatus;
    };

    class ProcessRunnable : public Runnable
    {
    public:
        ProcessRunnable(
            /* [in] */ CPackageManagerService* owner,
            /* [in] */ MoveParams* mp,
            /* [in] */ Int32 currentStatus)
            : mHost(owner)
            , mMp(mp)
            , mCurrentStatus(currentStatus)
        {}

        CARAPI Run();

    private:
        CPackageManagerService* mHost;
        AutoPtr<MoveParams> mMp;
        Int32 mCurrentStatus;
    };

    class AppDirObserver : public FileObserver
    {
    public:
        AppDirObserver(
            /* [in] */ const String& path,
            /* [in] */ Int32 mask,
            /* [in] */ Boolean isrom,
            /* [in] */ CPackageManagerService* owner);

        CARAPI OnEvent(
            /* [in] */ Int32 event,
            /* [in] */ const String& path);

    private:
        String mRootDir;
        Boolean mIsRom;
        CPackageManagerService* mHost;
    };

    class ClearStorageConnection
        : public ElRefBase
        , public IServiceConnection
    {
    public:
        ClearStorageConnection() {}

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnServiceConnected(
            /* [in] */ IComponentName* name,
            /* [in] */ IBinder* service);

        CARAPI OnServiceDisconnected(
            /* [in] */ IComponentName* name);

    public:
        AutoPtr<IMediaContainerService> mContainerService;
        Object mLock;
    };

public:
    CPackageManagerService();

    ~CPackageManagerService();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Handle32 installer,
        /* [in] */ Boolean factoryTest,
        /* [in] */ Boolean onlyCore);

    CARAPI_(AutoPtr<IBundle>) ExtrasForInstallResult(
        /* [in] */ PackageInstalledInfo* res);

    CARAPI_(void) ScheduleWriteSettingsLocked();

    CARAPI_(void) ScheduleWritePackageRestrictionsLocked(
        /* [in] */ Int32 userId);

    static CARAPI_(AutoPtr<CPackageManagerService>) Main(
        /* [in] */ IContext* context,
        /* [in] */ Installer* installer,
        /* [in] */ Boolean factoryTest,
        /* [in] */ Boolean onlyCore);

    static CARAPI_(AutoPtr< ArrayOf<String> >) SplitString(
        /* [in] */ const String& str,
        /* [in] */ Char32 sep);

    CARAPI IsFirstBoot(
        /* [out] */ Boolean* result);

    CARAPI IsOnlyCoreApps(
        /* [out] */ Boolean* result);

    CARAPI_(void) CleanupInstallFailedPackage(
        /* [in] */ PackageSetting* ps);

    CARAPI_(void) ReadPermission(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ const String& name);

    static CARAPI_(AutoPtr< ArrayOf<Int32> >) AppendInt(
        /* [in] */ ArrayOf<Int32>* cur,
        /* [in] */ Int32 val);

    static CARAPI_(AutoPtr< ArrayOf<Int32> >) AppendInts(
        /* [in] */ ArrayOf<Int32>* cur,
        /* [in] */ ArrayOf<Int32>* add);

    static CARAPI_(AutoPtr< ArrayOf<Int32> >) RemoveInt(
        /* [in] */ ArrayOf<Int32>* cur,
        /* [in] */ Int32 val);

    static CARAPI_(AutoPtr< ArrayOf<Int32> >) RemoveInts(
        /* [in] */ ArrayOf<Int32>* cur,
        /* [in] */ ArrayOf<Int32>* rem);

    CARAPI_(AutoPtr<IPackageInfo>) GeneratePackageInfo(
        /* [in] */ PackageParser::Package* p,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId);

    CARAPI IsPackageAvailable(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 userId,
        /* [out] */ Boolean* result);

    CARAPI GetPackageInfo(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IPackageInfo** pkgInfo);

    CARAPI CurrentToCanonicalPackageNames(
        /* [in] */ const ArrayOf<String>& names,
        /* [out, callee] */ ArrayOf<String>** result);

    CARAPI CanonicalToCurrentPackageNames(
        /* [in] */ const ArrayOf<String>& names,
        /* [out, callee] */ ArrayOf<String>** result);

    CARAPI GetPackageUid(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 userId,
        /* [out] */ Int32* uid);

    CARAPI GetPackageGids(
        /* [in] */ const String& packageName,
        /* [out, callee] */ ArrayOf<Int32>** gids);

    static CARAPI_(AutoPtr<IPermissionInfo>) GeneratePermissionInfo(
        /* [in] */ BasePermission* bp,
        /* [in] */ Int32 flags);

    CARAPI GetPermissionInfo(
        /* [in] */ const String& name,
        /* [in] */ Int32 flags,
        /* [out] */ IPermissionInfo** info);

    CARAPI QueryPermissionsByGroup(
        /* [in] */ const String& group,
        /* [in] */ Int32 flags,
        /* [out, callee] */ IList** infos);

    CARAPI GetPermissionGroupInfo(
        /* [in] */ const String& name,
        /* [in] */ Int32 flags,
        /* [out] */ IPermissionGroupInfo** info);

    CARAPI GetAllPermissionGroups(
        /* [in] */ Int32 flags,
        /* [out, callee] */ IList** infos);

    CARAPI_(AutoPtr<IApplicationInfo>) GenerateApplicationInfoFromSettingsLPw(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId);

    CARAPI_(AutoPtr<IPackageInfo>) GeneratePackageInfoFromSettingsLPw(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId);

    CARAPI GetApplicationInfo(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IApplicationInfo** appInfo);

    CARAPI FreeStorageAndNotify(
        /* [in] */ Int64 freeStorageSize,
        /* [in] */ IPackageDataObserver* observer);

    CARAPI FreeStorage(
        /* [in] */ Int64 freeStorageSize,
        /* [in] */ IIntentSender* pi);

    CARAPI FreeStorage(
        /* [in] */ Int64 freeStorageSize);

    CARAPI GetActivityInfo(
        /* [in] */ IComponentName* component,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IActivityInfo** info);

    CARAPI ActivitySupportsIntent(
        /* [in] */ IComponentName* className,
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [out] */ Boolean* result);

    CARAPI GetReceiverInfo(
        /* [in] */ IComponentName* component,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IActivityInfo** info);

    CARAPI GetServiceInfo(
        /* [in] */ IComponentName* component,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IServiceInfo** info);

    CARAPI GetProviderInfo(
        /* [in] */ IComponentName* component,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IProviderInfo** info);

    CARAPI GetSystemSharedLibraryNames(
        /* [out, callee] */ ArrayOf<String>** names);

    // ActionsCode(songzhining, new code: add extra hardware feature support)
    CARAPI_(AutoPtr< HashMap<String, AutoPtr<IFeatureInfo> > >) GetFeaturesLocked();

    CARAPI GetSystemAvailableFeatures(
        /* [out] */ ArrayOf<IFeatureInfo*>** infos);

    CARAPI HasSystemFeature(
        /* [in] */ const String& name,
        /* [out] */ Boolean* result);

    CARAPI CheckPermission(
        /* [in] */ const String& permName,
        /* [in] */ const String& pkgName,
        /* [out] */ Int32* result);

    CARAPI CheckUidPermission(
        /* [in] */ const String& permName,
        /* [in] */ Int32 uid,
        /* [out] */ Int32* result);

    static CARAPI_(Boolean) CompareStrings(
        /* [in] */ ICharSequence* s1,
        /* [in] */ ICharSequence* s2);

    static CARAPI_(Boolean) CompareStrings(
        /* [in] */ const String& s1,
        /* [in] */ const String& s2);

    static CARAPI_(Boolean) ComparePermissionInfos(
        /* [in] */ IPermissionInfo* pi1,
        /* [in] */ IPermissionInfo* pi2);

    CARAPI_(Int32) PermissionInfoFootprint(
        /* [in] */ IPermissionInfo* info);

    CARAPI_(Int32) CalculateCurrentPermissionFootprintLocked(
        /* [in] */ BasePermission* tree);

    CARAPI EnforcePermissionCapLocked(
        /* [in] */ IPermissionInfo* info,
        /* [in] */ BasePermission* tree);

    CARAPI AddPermissionLocked(
        /* [in] */ IPermissionInfo* info,
        /* [in] */ Boolean async,
        /* [out] */ Boolean* result);

    CARAPI AddPermission(
        /* [in] */ IPermissionInfo* info,
        /* [out] */ Boolean* result);

    CARAPI AddPermissionAsync(
        /* [in] */ IPermissionInfo* info,
        /* [out] */ Boolean* result);

    CARAPI RemovePermission(
        /* [in] */ const String& name);

    CARAPI GrantPermission(
        /* [in] */ const String& packageName,
        /* [in] */ const String& permissionName);

    CARAPI RevokePermission(
        /* [in] */ const String& packageName,
        /* [in] */ const String& permissionName);

    CARAPI IsProtectedBroadcast(
        /* [in] */ const String& actionName,
        /* [out] */ Boolean* result);

    CARAPI CheckSignatures(
        /* [in] */ const String& pkg1,
        /* [in] */ const String& pkg2,
        /* [out] */ Int32* result);

    CARAPI CheckUidSignatures(
        /* [in] */ Int32 uid1,
        /* [in] */ Int32 uid2,
        /* [out] */ Int32* result);

    /**
     * Compares two sets of signatures. Returns:
     * <br />
     * {@link PackageManager#SIGNATURE_NEITHER_SIGNED}: if both signature sets are null,
     * <br />
     * {@link PackageManager#SIGNATURE_FIRST_NOT_SIGNED}: if the first signature set is null,
     * <br />
     * {@link PackageManager#SIGNATURE_SECOND_NOT_SIGNED}: if the second signature set is null,
     * <br />
     * {@link PackageManager#SIGNATURE_MATCH}: if the two signature sets are identical,
     * <br />
     * {@link PackageManager#SIGNATURE_NO_MATCH}: if the two signature sets differ.
     */
    CARAPI_(Int32) CompareSignatures(
        /* [in] */ ArrayOf<ISignature*>* s1,
        /* [in] */ ArrayOf<ISignature*>* s2);

    CARAPI GetPackagesForUid(
        /* [in] */ Int32 uid,
        /* [out, callee] */ ArrayOf<String>** packages);

    CARAPI GetNameForUid(
        /* [in] */ Int32 uid,
        /* [out] */ String* name);

    CARAPI GetUidForSharedUser(
        /* [in] */ const String& sharedUserName,
        /* [out] */ Int32* uid);

    CARAPI GetFlagsForUid(
        /* [in] */ Int32 uid,
        /* [out] */ Int32* result);

    CARAPI IsUidPrivileged(
        /* [in] */ Int32 uid,
        /* [out] */ Boolean* result);

    CARAPI ResolveIntent(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IResolveInfo** resolveInfo);

    CARAPI SetLastChosenActivity(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [in] */ IIntentFilter* filter,
        /* [in] */ Int32 match,
        /* [in] */ IComponentName* activity);

    CARAPI GetLastChosenActivity(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [out] */ IResolveInfo** info);

    CARAPI_(AutoPtr<IResolveInfo>) FindPreferredActivity(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [in] */ IList* query,
        /* [in] */ Int32 priority,
        /* [in] */ Boolean always,
        /* [in] */ Boolean removeMatches,
        /* [in] */ Boolean debug,
        /* [in] */ Int32 userId);

    /*
     * Returns if intent can be forwarded from the sourceUserId to the targetUserId
     */
    CARAPI CanForwardTo(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 sourceUserId,
        /* [in] */ Int32 targetUserId,
        /* [out] */ Boolean* result);

    CARAPI QueryIntentActivities(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out, callee] */ IList** infos);

    CARAPI QueryIntentActivityOptions(
        /* [in] */ IComponentName* caller,
        /* [in] */ ArrayOf<IIntent*>* specifics,
        /* [in] */ ArrayOf<String>* specificTypes,
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IList** infos);

    CARAPI QueryIntentReceivers(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IList** receivers);

    CARAPI ResolveService(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IResolveInfo** resolveInfo);

    CARAPI QueryIntentServices(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IList** services);

    CARAPI QueryIntentContentProviders(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IList** infos);

    CARAPI GetInstalledPackages(
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IParceledListSlice** slice);

    CARAPI GetPackagesHoldingPermissions(
        /* [in] */ ArrayOf<String>* permissions,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IParceledListSlice** slice);

    CARAPI GetInstalledApplications(
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IParceledListSlice** slice);

    CARAPI GetPersistentApplications(
        /* [in] */ Int32 flags,
        /* [out, callee] */ IList** infos);

    CARAPI ResolveContentProvider(
        /* [in] */ const String& name,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IProviderInfo** info);

    CARAPI QuerySyncProviders(
        /* [in, out] */ IList* outNames,
        /* [in, out] */ IList* outInfo);

    CARAPI QueryContentProviders(
        /* [in] */ const String& processName,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 flags,
        /* [out] */ IList** providers);

    CARAPI GetInstrumentationInfo(
        /* [in] */ IComponentName* name,
        /* [in] */ Int32 flags,
        /* [out] */ IInstrumentationInfo** instInfo);

    CARAPI QueryInstrumentation(
        /* [in] */ const String& targetPackage,
        /* [in] */ Int32 flags,
        /* [out, callee] */ IList** infos);

    static CARAPI_(void) ReportSettingsProblem(
        /* [in] */ Int32 priority,
        /* [in] */ const String& msg);

    static CARAPI_(void) LogCriticalInfo(
        /* [in] */ Int32 priority,
        /* [in] */ const String& msg);

    CARAPI PerformBootDexOpt();

    CARAPI PerformDexOptIfNeeded(
        /* [in] */ const String& packageName,
        /* [in] */ const String& instructionSet,
        /* [out] */ Boolean* result);

    CARAPI_(Boolean) PerformDexOpt(
        /* [in] */ const String& packageName,
        /* [in] */ const String& instructionSet,
        /* [in] */ Boolean backgroundDexopt);

    CARAPI_(AutoPtr< HashSet<String> >) GetPackagesThatNeedDexOpt();

    CARAPI_(void) Shutdown();

    /**
     * Returns deduplicated list of supported instructions for dex code.
     */
    static CARAPI_(AutoPtr< ArrayOf<String> >) GetAllDexCodeInstructionSets();

    CARAPI ForceDexOpt(
        /* [in] */ const String& packageName);

    CARAPI_(AutoPtr<IFile>) GetDataPathForUser(
        /* [in] */ Int32 userId);

    CARAPI_(void) RemovePackageLI(
        /* [in] */ PackageSetting* ps,
        /* [in] */ Boolean chatty);

    CARAPI_(void) RemoveInstalledPackageLI(
        /* [in] */ PackageParser::Package* pkg,
        /* [in] */ Boolean chatty);

    CARAPI_(void) CleanPackageDataStructuresLILPw(
        /* [in] */ PackageParser::Package* pkg,
        /* [in] */ Boolean chatty);

    static CARAPI_(void) SendPackageBroadcast(
        /* [in] */ const String& action,
        /* [in] */ const String& pkg,
        /* [in] */ IBundle* extras,
        /* [in] */ const String& targetPkg,
        /* [in] */ IIntentReceiver* finishedReceiver,
        /* [in] */ ArrayOf<Int32>* userIds);

    CARAPI NextPackageToClean(
        /* [in] */ IPackageCleanItem* lastPackage,
        /* [out] */ IPackageCleanItem** nextPackage);

    CARAPI_(void) SchedulePackageCleaning(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 userId,
        /* [in] */ Boolean andCode);

    CARAPI_(void) StartCleaningPackages();

    CARAPI InstallPackage(
        /* [in] */ IUri* packageURI,
        /* [in] */ IPackageInstallObserver* observer,
        /* [in] */ Int32 flags);

    CARAPI InstallPackage(
        /* [in] */ IUri* packageURI,
        /* [in] */ IPackageInstallObserver* observer,
        /* [in] */ Int32 flags,
        /* [in] */ const String& installerPackageName);

    CARAPI InstallPackageWithVerification(
        /* [in] */ IUri* packageURI,
        /* [in] */ IPackageInstallObserver* observer,
        /* [in] */ Int32 flags,
        /* [in] */ const String& installerPackageName,
        /* [in] */ IUri* verificationURI,
        /* [in] */ IManifestDigest* manifestDigest,
        /* [in] */ IContainerEncryptionParams* encryptionParams);

    CARAPI InstallPackageWithVerificationAndEncryption(
        /* [in] */ IUri* packageURI,
        /* [in] */ IPackageInstallObserver* observer,
        /* [in] */ Int32 flags,
        /* [in] */ const String& installerPackageName,
        /* [in] */ IVerificationParams* verificationParams,
        /* [in] */ IContainerEncryptionParams* encryptionParams);

    CARAPI InstallExistingPackage(
        /* [in] */ const String& packageName,
        /* [out] */ Int32* result);

    CARAPI VerifyPendingInstall(
        /* [in] */ Int32 id,
        /* [in] */ Int32 verificationCode);

    CARAPI ExtendVerificationTimeout(
        /* [in] */ Int32 id,
        /* [in] */ Int32 verificationCodeAtTimeout,
        /* [in] */ Int64 millisecondsToDelay);

    CARAPI FinishPackageInstall(
        /* [in] */ Int32 token);

    CARAPI SetInstallerPackageName(
        /* [in] */ const String& targetPackage,
        /* [in] */ const String& installerPackageName);

    CARAPI ProcessPendingInstall(
        /* [in] */ InstallArgs* args,
        /* [in] */ Int32 currentStatus);

    static CARAPI_(String) CidFromCodePath(
        /* [in] */ const String& fullCodePath);

    CARAPI_(String) GetAsecPackageName(
        /* [in] */ const String& packageCid);

    static CARAPI_(String) GetApkName(
        /* [in] */ const String& codePath);

    CARAPI DeletePackage(
        /* [in] */ const String& packageName,
        /* [in] */ IPackageDeleteObserver* observer,
        /* [in] */ Int32 flags);

    CARAPI ClearApplicationUserData(
        /* [in] */ const String& packageName,
        /* [in] */ IPackageDataObserver* observer,
        /* [in] */ Int32 userId);

    CARAPI DeleteApplicationCacheFiles(
        /* [in] */ const String& packageName,
        /* [in] */ IPackageDataObserver* observer);

    CARAPI GetPackageSizeInfo(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 userHandle,
        /* [in] */ IPackageStatsObserver* observer);

    CARAPI AddPackageToPreferred(
        /* [in] */ const String& packageName);

    CARAPI RemovePackageFromPreferred(
        /* [in] */ const String& packageName);

    CARAPI GetPreferredPackages(
        /* [in] */ Int32 flags,
        /* [out, callee] */ IList** infos);

    CARAPI AddPreferredActivity(
        /* [in] */ IIntentFilter* filter,
        /* [in] */ Int32 match,
        /* [in] */ ArrayOf<IComponentName*>* set,
        /* [in] */ IComponentName* activity,
        /* [in] */ Int32 userId);

    CARAPI ReplacePreferredActivity(
        /* [in] */ IIntentFilter* filter,
        /* [in] */ Int32 match,
        /* [in] */ ArrayOf<IComponentName*>* set,
        /* [in] */ IComponentName* activity);

    CARAPI ClearPackagePreferredActivities(
        /* [in] */ const String& packageName);

    CARAPI_(Boolean) ClearPackagePreferredActivitiesLPw(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 userId);

    CARAPI GetPreferredActivities(
        /* [in, out] */ IList* outFilters,
        /* [in, out] */ IList* outActivities,
        /* [in] */ const String& packageName,
        /* [out] */ Int32* count);

    CARAPI SetApplicationEnabledSetting(
        /* [in] */ const String& appPackageName,
        /* [in] */ Int32 newState,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId);

    CARAPI SetComponentEnabledSetting(
        /* [in] */ IComponentName* componentName,
        /* [in] */ Int32 newState,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId);

    CARAPI SetPackageStoppedState(
        /* [in] */ const String& packageName,
        /* [in] */ Boolean stopped,
        /* [in] */ Int32 userId);

    CARAPI GetInstallerPackageName(
        /* [in] */ const String& packageName,
        /* [out] */ String* name);

    CARAPI GetApplicationEnabledSetting(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 userId,
        /* [out] */ Int32* setting);

    CARAPI GetComponentEnabledSetting(
        /* [in] */ IComponentName* componentName,
        /* [in] */ Int32 userId,
        /* [out] */ Int32* setting);

    CARAPI EnterSafeMode();

    CARAPI SystemReady();

    CARAPI IsSafeMode(
        /* [out] */ Boolean* isSafeMode);

    CARAPI HasSystemUidErrors(
        /* [out] */ Boolean* result);

    static CARAPI_(String) ArrayToString(
        /* [in] */ ArrayOf<Int32>* array);

    // protected void dump(FileDescriptor fd, PrintWriter pw, String[] args)

    static CARAPI_(String) GetTempContainerId();

    CARAPI UpdateExternalMediaStatus(
        /* [in] */ Boolean mediaStatus,
        /* [in] */ Boolean reportStatus);

    CARAPI ScanAvailableAsecs();

    CARAPI MovePackage(
        /* [in] */ const String& packageName,
        /* [in] */ IPackageMoveObserver* observer,
        /* [in] */ Int32 flags);

    CARAPI SetInstallLocation(
        /* [in] */ Int32 loc,
        /* [out] */ Boolean* result);

    CARAPI GetInstallLocation(
        /* [out] */ Int32* loc);

    CARAPI_(void) CleanUpUserLILPw(
        /* [in] */ Int32 userHandle);

    CARAPI_(void) CreateNewUserLILPw(
        /* [in] */ Int32 userHandle,
        /* [in] */ IFile* path);

    CARAPI GetVerifierDeviceIdentity(
        /* [out] */ IVerifierDeviceIdentity** identity);

    CARAPI SetPermissionEnforced(
        /* [in] */ const String& permission,
        /* [in] */ Boolean enforced);

    CARAPI IsPermissionEnforced(
        /* [in] */ const String& permission,
        /* [out] */ Boolean* result);

    CARAPI IsStorageLow(
        /* [out] */ Boolean* result);

private:
    static CARAPI_(void) GetDefaultDisplayMetrics(
        /* [in] */ IContext* context,
        /* [in] */ IDisplayMetrics* metrics);

    CARAPI_(String) GetRequiredVerifierLPr();

    CARAPI_(void) ReadPermissionsFromXml(
        /* [in] */ IFile* permFile);

    CARAPI CheckValidCaller(
        /* [in] */ Int32 uid,
        /* [in] */ Int32 userId);

    /**
     * Checks if the request is from the system or an app that has INTERACT_ACROSS_USERS
     * or INTERACT_ACROSS_USERS_FULL permissions, if the userid is not for the caller.
     * @param checkShell TODO(yamasani):
     * @param message the message to log on security exception
     */
    CARAPI EnforceCrossUserPermission(
        /* [in] */ Int32 callingUid,
        /* [in] */ Int32 userId,
        /* [in] */ Boolean requireFullPermission,
        /* [in] */ Boolean checkShell,
        /* [in] */ const String& message);

    CARAPI EnforceShellRestriction(
        /* [in] */ const String& restriction,
        /* [in] */ Int32 callingUid,
        /* [in] */ Int32 userHandle);

    CARAPI_(AutoPtr<BasePermission>) FindPermissionTreeLP(
        /* [in] */ const String& permName);

    CARAPI CheckPermissionTreeLP(
        /* [in] */ const String& permName,
        /* [out] */ BasePermission** permission);

    static CARAPI CheckGrantRevokePermissions(
        /* [in] */ PackageParser::Package* pkg,
        /* [in] */ BasePermission* bp);

    /**
     * If the database version for this type of package (internal storage or
     * external storage) is less than the version where package signatures
     * were updated, return true.
     */
    CARAPI_(Boolean) IsCompatSignatureUpdateNeeded(
        /* [in] */ PackageParser::Package* scannedPkg);

    /**
     * Used for backward compatibility to make sure any packages with
     * certificate chains get upgraded to the new style. {@code existingSigs}
     * will be in the old format (since they were stored on disk from before the
     * system upgrade) and {@code scannedSigs} will be in the newer format.
     */
    CARAPI_(Int32) CompareSignaturesCompat(
        /* [in] */ PackageSignatures* existingSigs,
        /* [in] */ PackageParser::Package* scannedPkg);

    CARAPI_(Boolean) IsRecoverSignatureUpdateNeeded(
        /* [in] */ PackageParser::Package* scannedPkg);

    CARAPI_(Int32) CompareSignaturesRecover(
        /* [in] */ PackageSignatures* existingSigs,
        /* [in] */ PackageParser::Package* scannedPkg);

    CARAPI ChooseBestActivity(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [in] */ IList* query,
        /* [in] */ Int32 userId,
        /* [out] */ IResolveInfo** resolveInfo);

    CARAPI_(AutoPtr<IResolveInfo>) FindPersistentPreferredActivityLP(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [in] */ IList* query,
        /* [in] */ Boolean debug,
        /* [in] */ Int32 userId);

    CARAPI(AutoPtr< List<AutoPtr<CrossProfileIntentFilter> > >) GetMatchingCrossProfileIntentFilters(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 userId);

    CARAPI(AutoPtr<IResolveInfo>) QuerySkipCurrentProfileIntents(
        /* [in] */ List<AutoPtr<CrossProfileIntentFilter> >* matchingFilters,
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 sourceUserId);

    // Return matching ResolveInfo if any for skip current profile intent filters.
    CARAPI(AutoPtr<IResolveInfo>) QueryCrossProfileIntents(
        /* [in] */ List<AutoPtr<CrossProfileIntentFilter> >* matchingFilters,
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 sourceUserId);

    CARAPI(AutoPtr<IResolveInfo>) CheckTargetCanHandle(
        /* [in] */ CrossProfileIntentFilter* filter,
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 sourceUserId);

    CARAPI(AutoPtr<IResolveInfo>) CreateForwardingResolveInfo(
        /* [in] */ IIntentFilter* filter,
        /* [in] */ Int32 sourceUserId,
        /* [in] */ Int32 targetUserId);

    CARAPI_(void) AddPackageHoldingPermissions(
        /* [in] */ IArrayList* list,
        /* [in] */ PackageSetting* ps,
        /* [in] */ ArrayOf<String>* permissions,
        /* [in] */ ArrayOf<Boolean>* tmp,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId);

    CARAPI_(void) CreateIdmapsForPackageLI(
        /* [in] */ PackageParser::Package* pkg);

    CARAPI_(Boolean) CreateIdmapForPackagePairLI(
        /* [in] */ PackageParser::Package* pkg,
        /* [in] */ PackageParser::Package* opkg);

    CARAPI_(void) ScanDirLI(
        /* [in] */ IFile* dir,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 scanMode,
        /* [in] */ Int64 currentTime);

    static CARAPI_(AutoPtr<IFile>) GetSettingsProblemFile();

    CARAPI CollectCertificatesLI(
        /* [in] */ PackageParser* pp,
        /* [in] */ PackageSetting* ps,
        /* [in] */ PackageParser::Package* pkg,
        /* [in] */ IFile* srcFile,
        /* [in] */ Int32 parseFlags);

    CARAPI ScanPackageLI(
        /* [in] */ IFile* scanFile,
        /* [in] */ Int32 parseFlags,
        /* [in] */ Int32 scanFlags,
        /* [in] */ Int64 currentTime,
        /* [in] */ IUserHandle* user,
        /* [out] */ PackageParser::Packag** pkg);

    static CARAPI_(String) FixProcessName(
        /* [in] */ const String& defProcessName,
        /* [in] */ const String& processName,
        /* [in] */ Int32 uid);

    CARAPI VerifySignaturesLP(
        /* [in] */ PackageSetting* pkgSetting,
        /* [in] */ PackageParser::Package* pkg);

    static CARAPI EnforceSystemOrRoot(
        /* [in] */ const String& message);

    CARAPI_(void) FilterRecentlyUsedApps(
        /* [in] */ HashSet<AutoPtr<PackageParser::Package> >* pkgs);

    CARAPI_(HashSet<String>&) GetPackageNamesForIntent(
        /* [in] */ IIntent* intent);

    CARAPI_(void) PerformBootDexOpt(
        /* [in] */ PackageParser::Package* pkg,
        /* [in] */ Int32 curr,
        /* [in] */ Int32 total);

    static CARAPI_(String) GetPrimaryInstructionSet(
        /* [in] */ IApplicationInfo* info);

    CARAPI_(void) PerformDexOptLibsLI(
        /* [in] */ List<String>* libs,
        /* [in] */ ArrayOf<String>* instructionSets,
        /* [in] */ Boolean forceDex,
        /* [in] */ Boolean defer,
        /* [in] */ HashSet<String>* done);

    CARAPI_(Int32) PerformDexOptLI(
        /* [in] */ PackageParser::Package* pkg,
        /* [in] */ ArrayOf<String>* targetInstructionSets,
        /* [in] */ Boolean forceDex,
        /* [in] */ Boolean defer,
        /* [in] */ HashSet<String>* done);

    static CARAPI_(AutoPtr< ArrayOf<String> >) GetAppDexInstructionSets(
        /* [in] */ IApplicationInfo* info);

    static CARAPI_(AutoPtr< ArrayOf<String> >) GetAppDexInstructionSets(
        /* [in] */ PackageSetting* ps);

    static CARAPI_(String) GetPreferredInstructionSet();

    static CARAPI_(List<String>&) GetAllInstructionSets();

    /**
     * Returns the instruction set that should be used to compile dex code. In the presence of
     * a native bridge this might be different than the one shared libraries use.
     */
    static CARAPI_(String) GetDexCodeInstructionSet(
        /* [in] */ const String& sharedLibraryIsa);

    static CARAPI_(AutoPtr< ArrayOf<String> >) GetDexCodeInstructionSets(
        /* [in] */ ArrayOf<String>* instructionSets);

    CARAPI_(Int32) PerformDexOptLI(
        /* [in] */ PackageParser::Package* pkg,
        /* [in] */ ArrayOf<String>* instructionSets,
        /* [in] */ Boolean forceDex,
        /* [in] */ Boolean defer,
        /* [in] */ Boolean inclDependencies);

    CARAPI_(Boolean) VerifyPackageUpdateLPr(
        /* [in] */ PackageSetting* oldPkg,
        /* [in] */ PackageParser::Package* newPkg);

    CARAPI_(AutoPtr<IFile>) GetDataPathForPackage(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 userId);

    CARAPI_(Int32) CreateDataDirsLI(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid,
        /* [in] */ const String& seinfo);

    CARAPI_(Int32) RemoveDataDirsLI(
        /* [in] */ const String& packageName);

    CARAPI_(Int32) DeleteCodeCacheDirsLI(
        /* [in] */ const String& packageName);

    CARAPI_(void) AddSharedLibraryLPw(
        /* [in] */ List<String>* usesLibraryFiles,
        /* [in] */ SharedLibraryEntry* file,
        /* [in] */ PackageParser::Package* changingLib);

    CARAPI UpdateSharedLibrariesLPw(
        /* [in] */ PackageParser::Package* pkg,
        /* [in] */ PackageParser::Package* changingLib);

    static CARAPI_(Boolean) HasString(
        /* [in] */ List<String>* list,
        /* [in] */ List<String>* which);

    CARAPI_(void) UpdateAllSharedLibrariesLPw();

    CARAPI_(AutoPtr< List<AutoPtr<PackageParser::Package> > >) UpdateAllSharedLibrariesLPw(
        /* [in] */ PackageParser::Package* changingPkg);

    /**
     * Derive the value of the {@code cpuAbiOverride} based on the provided
     * value and an optional stored value from the package settings.
     */
    static CARAPI_(String) DeriveAbiOverride(
        /* [in] */ const String& abiOverride,
        /* [in] */ PackageSetting* settings);

    CARAPI_(AutoPtr<PackageParser::Package>) ScanPackageLI(
        /* [in] */ PackageParser::Package* pkg,
        /* [in] */ Int32 parseFlags,
        /* [in] */ Int32 scanFlags,
        /* [in] */ Int64 currentTime,
        /* [in] */ IUserHandle* user);

    CARAPI ScanPackageDirtyLI(
        /* [in] */ PackageParser::Package* pkg,
        /* [in] */ Int32 parseFlags,
        /* [in] */ Int32 scanFlags,
        /* [in] */ Int64 currentTime,
        /* [in] */ IUserHandle* user,
        /* [out] */ PackageParser::Package* outPkg);

    /**
     * Adjusts ABIs for a set of packages belonging to a shared user so that they all match.
     * i.e, so that all packages can be run inside a single process if required.
     *
     * Optionally, callers can pass in a parsed package via {@code newPackage} in which case
     * this function will either try and make the ABI for all packages in {@code packagesForUser}
     * match {@code scannedPackage} or will update the ABI of {@code scannedPackage} to match
     * the ABI selected for {@code packagesForUser}. This variant is used when installing or
     * updating a package that belongs to a shared user.
     *
     * NOTE: We currently only match for the primary CPU abi string. Matching the secondary
     * adds unnecessary complexity.
     */
    CARAPI_(void) AdjustCpuAbisForSharedUserLPw(
        /* [in] */ HashSet<AutoPtr<PackageSetting> >& packagesForUser,
        /* [in] */ PackageParser::Package* scannedPackage,
        /* [in] */ Boolean forceDexOpt,
        /* [in] */ Boolean deferDexOpt);

    CARAPI_(void) SetUpCustomResolverActivity(
        /* [in] */ PackageParser::Package* pkg);

    static CARAPI_(String) CalculateBundledApkRoot(
        /* [in] */ const String& codePathString);

    /**
     * Derive and set the location of native libraries for the given package,
     * which varies depending on where and how the package was installed.
     */
    CARAPI_(void) SetNativeLibraryPaths(
        /* [in] */ PackageParser::Package* pkg);

    /**
     * Calculate the abis and roots for a bundled app. These can uniquely
     * be determined from the contents of the system partition, i.e whether
     * it contains 64 or 32 bit shared libraries etc. We do not validate any
     * of this information, and instead assume that the system was built
     * sensibly.
     */
    CARAPI_(void) SetBundledAppAbisAndRoots(
        /* [in] */ PackageParser::Package* pkg,
        /* [in] */ PackageSetting* pkgSetting);

    /**
     * Deduces the ABI of a bundled app and sets the relevant fields on the
     * parsed pkg object.
     *
     * @param apkRoot the root of the installed apk, something like {@code /system} or {@code /oem}
     *        under which system libraries are installed.
     * @param apkName the name of the installed package.
     */
    static CARAPI_(void) SetBundledAppAbi(
        /* [in] */ PackageParser::Package* pkg,
        /* [in] */ const String& apkRoot,
        /* [in] */ const String& apkName);

    CARAPI_(void) KillApplication(
        /* [in] */ const String& pkgName,
        /* [in] */ Int32 appId,
        /* [in] */ const String& reason);

    static CARAPI_(Boolean) HasPermission(
        /* [in] */ PackageParser::Package* pkgInfo,
        /* [in] */ const String& perm);

    CARAPI_(void) UpdatePermissionsLPw(
        /* [in] */ const String& changingPkg,
        /* [in] */ PackageParser::Package* pkgInfo,
        /* [in] */ Int32 flags);

    CARAPI_(void) GrantPermissionsLPw(
        /* [in] */ PackageParser::Package* pkg,
        /* [in] */ Boolean replace,
        /* [in] */ const String& packageOfInterest);

    CARAPI_(Boolean) IsNewPlatformPermissionForPackage(
        /* [in] */ const String& perm,
        /* [in] */ PackageParser::Package* pkg);

    CARAPI_(Boolean) GrantSignaturePermission(
        /* [in] */ const String& perm,
        /* [in] */ PackageParser::Package* pkg,
        /* [in] */ BasePermission* bp,
        /* [in] */ HashSet<String>* origPermissions);

    CARAPI_(Boolean) IsExternalMediaAvailable();

    CARAPI BroadcastPackageVerified(
        /* [in] */ Int32 verificationId,
        /* [in] */ IUri* packageUri,
        /* [in] */ Int32 verificationCode,
        /* [in] */ IUserHandle* user);

    CARAPI_(AutoPtr<IComponentName>) MatchComponentForVerifier(
        /* [in] */ const String& packageName,
        /* [in] */ List< AutoPtr<IResolveInfo> >* receivers);

    CARAPI_(AutoPtr< List< AutoPtr<IComponentName> > >) MatchVerifiers(
        /* [in] */ IPackageInfoLite* pkgInfo,
        /* [in] */ List< AutoPtr<IResolveInfo> >* receivers,
        /* [in] */ PackageVerificationState* verificationState);

    CARAPI_(Int32) GetUidForVerifier(
        /* [in] */ IVerifierInfo* verifierInfo);

    CARAPI_(Int64) GetVerificationTimeout();

    CARAPI_(Int32) GetDefaultVerificationResponse();

    CARAPI_(Boolean) IsVerificationEnabled(
        /* [in] */ Int32 flags);

    CARAPI_(Int32) GetUnknownSourcesSettings();

    static CARAPI_(Boolean) InstallOnSd(
        /* [in] */ Int32 flags);

    static CARAPI_(Boolean) InstallForwardLocked(
        /* [in] */ Int32 flags);

    CARAPI_(AutoPtr<InstallArgs>) CreateInstallArgs(
        /* [in] */ InstallParams* params);

    CARAPI_(AutoPtr<InstallArgs>) CreateInstallArgs(
        /* [in] */ Int32 flags,
        /* [in] */ const String& fullCodePath,
        /* [in] */ const String& fullResourcePath,
        /* [in] */ const String& nativeLibraryPath);

    CARAPI_(AutoPtr<InstallArgs>) CreateInstallArgs(
        /* [in] */ IUri* packageURI,
        /* [in] */ Int32 flags,
        /* [in] */ const String& pkgName,
        /* [in] */ const String& dataDir);

    CARAPI_(Boolean) IsAsecExternal(
        /* [in] */ const String& cid);

    static CARAPI_(String) GetNextCodePath(
        /* [in] */ const String& oldCodePath,
        /* [in] */ const String& prefix,
        /* [in] */ const String& suffix);

    static CARAPI_(Boolean) IgnoreCodePath(
        /* [in] */ const String& fullPathStr);

    CARAPI_(void) InstallNewPackageLI(
        /* [in] */ PackageParser::Package* pkg,
        /* [in] */ Int32 parseFlags,
        /* [in] */ Int32 scanMode,
        /* [in] */ IUserHandle* user,
        /* [in] */ const String& installerPackageName,
        /* [in] */ PackageInstalledInfo* res,
        /* [in] */ ArrayOf<Byte>* readBuffer);

    CARAPI_(void) ReplacePackageLI(
        /* [in] */ PackageParser::Package* pkg,
        /* [in] */ Int32 parseFlags,
        /* [in] */ Int32 scanMode,
        /* [in] */ IUserHandle* user,
        /* [in] */ const String& installerPackageName,
        /* [in] */ PackageInstalledInfo* res,
        /* [in] */ ArrayOf<Byte>* readBuffer);

    CARAPI_(void) ReplaceNonSystemPackageLI(
        /* [in] */ PackageParser::Package* deletedPackage,
        /* [in] */ PackageParser::Package* pkg,
        /* [in] */ Int32 parseFlags,
        /* [in] */ Int32 scanMode,
        /* [in] */ IUserHandle* user,
        /* [in] */ const String& installerPackageName,
        /* [in] */ PackageInstalledInfo* res,
        /* [in] */ ArrayOf<Byte>* readBuffer);

    CARAPI_(void) ReplaceSystemPackageLI(
        /* [in] */ PackageParser::Package* deletedPackage,
        /* [in] */ PackageParser::Package* pkg,
        /* [in] */ Int32 parseFlags,
        /* [in] */ Int32 scanMode,
        /* [in] */ IUserHandle* user,
        /* [in] */ const String& installerPackageName,
        /* [in] */ PackageInstalledInfo* res,
        /* [in] */ ArrayOf<Byte>* readBuffer);

    CARAPI_(Int32) MoveDexFilesLI(
        /* [in] */ PackageParser::Package* newPackage);

    // for epk
    CARAPI_(Int32) MoveEcoFilesLI(
        /* [in] */ PackageParser::Package* newPackage);

    CARAPI_(void) UpdateSettingsLI(
        /* [in] */ PackageParser::Package* newPackage,
        /* [in] */ const String& installerPackageName,
        /* [in] */ PackageInstalledInfo* res);

    CARAPI_(void) InstallPackageLI(
        /* [in] */ InstallArgs* args,
        /* [in] */ Boolean newInstall,
        /* [in] */ PackageInstalledInfo* res,
        /* [in] */ ArrayOf<Byte>* readBuffer);

    static CARAPI_(Boolean) IsForwardLocked(
        /* [in] */ PackageParser::Package* pkg);

    static CARAPI_(Boolean) IsForwardLocked(
        /* [in] */ PackageSetting* ps);

    static CARAPI_(Boolean) IsExternal(
        /* [in] */ PackageParser::Package* pkg);

    static CARAPI_(Boolean) IsExternal(
        /* [in] */ PackageSetting* ps);

    static CARAPI_(Boolean) IsSystemApp(
        /* [in] */ PackageParser::Package* pkg);

    static CARAPI_(Boolean) IsSystemApp(
        /* [in] */ IApplicationInfo* info);

    static CARAPI_(Boolean) IsSystemApp(
        /* [in] */ PackageSetting* ps);

    static CARAPI_(Boolean) IsUpdatedSystemApp(
        /* [in] */ PackageParser::Package* pkg);

    CARAPI_(Int32) PackageFlagsToInstallFlags(
        /* [in] */ PackageSetting* ps);

    CARAPI_(void) DeleteTempPackageFiles();

    static CARAPI_(void) DeleteTempPackageFilesInDirectory(
        /* [in] */ IFile* directory,
        /* [in] */ IFilenameFilter* filter);

    CARAPI_(AutoPtr<IFile>) CreateTempPackageFile(
        /* [in] */ IFile* installDir);

    CARAPI_(Int32) DeletePackageX(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 flags);

    CARAPI_(void) RemovePackageDataLI(
        /* [in] */ PackageSetting* ps,
        /* [in] */ PackageRemovedInfo* outInfo,
        /* [in] */ Int32 flags,
        /* [in] */ Boolean writeSettings);

    CARAPI_(Boolean) DeleteSystemPackageLI(
        /* [in] */ PackageSetting* newPs,
        /* [in] */ Int32 flags,
        /* [in] */ PackageRemovedInfo* outInfo,
        /* [in] */ Boolean writeSettings,
        /* [in] */ ArrayOf<Byte>* readBuffer);

    CARAPI_(Boolean) DeleteInstalledPackageLI(
        /* [in] */ PackageSetting* ps,
        /* [in] */ Boolean deleteCodeAndResources,
        /* [in] */ Int32 flags,
        /* [in] */ PackageRemovedInfo* outInfo,
        /* [in] */ Boolean writeSettings);

    CARAPI_(Boolean) DeletePackageLI(
        /* [in] */ const String& packageName,
        /* [in] */ IUserHandle* user,
        /* [in] */ Boolean deleteCodeAndResources,
        /* [in] */ Int32 flags,
        /* [in] */ PackageRemovedInfo* outInfo,
        /* [in] */ Boolean writeSettings,
        /* [in] */ ArrayOf<Byte>* readBuffer);

    CARAPI_(void) ClearExternalStorageDataSync(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 userId,
        /* [in] */ Boolean allData);

    CARAPI_(Boolean) ClearApplicationUserDataLI(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 userId);

    CARAPI_(Boolean) DeleteApplicationCacheFilesLI(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 userId);

    CARAPI_(Boolean) GetPackageSizeInfoLI(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 userHandle,
        /* [in] */ IPackageStats* pStats);

    CARAPI_(Int32) GetUidTargetSdkVersionLockedLPr(
        /* [in] */ Int32 uid);

    CARAPI SetEnabledSetting(
        /* [in] */ const String& packageName,
        /* [in] */ const String& className,
        /* [in] */ Int32 newState,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId);

    CARAPI_(void) SendPackageChangedBroadcast(
        /* [in] */ const String& packageName,
        /* [in] */ Boolean killFlag,
        /* [in] */ List<String>* componentNames,
        /* [in] */ Int32 packageUid);

    CARAPI_(String) GetEncryptKey();

    CARAPI_(void) UpdateExternalMediaStatusInner(
        /* [in] */ Boolean isMounted,
        /* [in] */ Boolean reportStatus,
        /* [in] */ Boolean externalStorage);

    CARAPI SendResourcesChangedBroadcast(
        /* [in] */ Boolean mediaStatus,
        /* [in] */ List<String>& pkgList,
        /* [in] */ ArrayOf<Int32>* uidArr,
        /* [in] */ IIntentReceiver* finishedReceiver);

    CARAPI_(void) LoadMediaPackages(
        /* [in] */ HashMap<AutoPtr<AsecInstallArgs>, String>& processCids,
        /* [in] */ ArrayOf<Int32>& uidArr,
        /* [in] */ List<String>* removeCids);

    CARAPI_(void) UnloadAllContainers(
        /* [in] */ ISet* cidArgs);

    CARAPI UnloadMediaPackages(
        /* [in] */ HashMap<AutoPtr<AsecInstallArgs>, String>& processCids,
        /* [in] */ ArrayOf<Int32>& uidArr,
        /* [in] */ Boolean reportStatus);

    CARAPI_(void) ProcessPendingMove(
        /* [in] */ MoveParams* mp,
        /* [in] */ Int32 currentStatus);

    CARAPI_(Boolean) IsPermissionEnforcedDefault(
        /* [in] */ const String& permission);

    CARAPI_(Boolean) IsPermissionEnforcedLocked(
        /* [in] */ const String& permission,
        /* [in] */ Boolean enforcedDefault);

    CARAPI_(void) HandlePendingInstallRun(
        /* [in] */ InstallArgs* args,
        /* [in] */ Int32 currentStatus);

    CARAPI_(void) HandleDeletePackage(
        /* [in] */ const String& packageName,
        /* [in] */ IPackageDeleteObserver* observer,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 uid);

public:/*package*/
    static const String TAG;
    static const Boolean DEBUG_SETTINGS = FALSE;
    static const Boolean DEBUG_PREFERRED = FALSE;
    static const Boolean DEBUG_UPGRADE = FALSE;
    static const Int32 SCAN_NO_DEX = 1 << 1;
    static const Int32 SCAN_FORCE_DEX = 1 << 2;
    static const Int32 SCAN_UPDATE_SIGNATURE = 1 << 3;
    static const Int32 SCAN_NEW_INSTALL = 1 << 4;
    static const Int32 SCAN_NO_PATHS = 1 << 5;
    static const Int32 SCAN_UPDATE_TIME = 1 << 6;
    static const Int32 SCAN_DEFER_DEX = 1 << 7;
    static const Int32 SCAN_BOOTING = 1 << 8;
    static const Int32 SCAN_TRUSTED_OVERLAY = 1<<9;
    static const Int32 SCAN_DELETE_DATA_ON_FAILURES = 1<<10;
    static const Int32 SCAN_REPLACING = 1<<11;

    static const Int32 REMOVE_CHATTY = 1 << 16;

    static const String DEFAULT_CONTAINER_PACKAGE;

    static const AutoPtr<IComponentName> DEFAULT_CONTAINER_COMPONENT;

    static const Int32 SEND_PENDING_BROADCAST = 1;
    static const Int32 MCS_BOUND = 3;
    static const Int32 END_COPY = 4;
    static const Int32 INIT_COPY = 5;
    static const Int32 MCS_UNBIND = 6;
    static const Int32 START_CLEANING_PACKAGE = 7;
    static const Int32 FIND_INSTALL_LOC = 8;
    static const Int32 POST_INSTALL = 9;
    static const Int32 MCS_RECONNECT = 10;
    static const Int32 MCS_GIVE_UP = 11;
    static const Int32 UPDATED_MEDIA_STATUS = 12;
    static const Int32 WRITE_SETTINGS = 13;
    static const Int32 WRITE_PACKAGE_RESTRICTIONS = 14;
    static const Int32 PACKAGE_VERIFIED = 15;
    static const Int32 CHECK_PENDING_VERIFICATION = 16;

    static const Int32 WRITE_SETTINGS_DELAY = 10 * 1000;  // 10 seconds

    // Delay time in millisecs
    static const Int32 BROADCAST_DELAY = 10 * 1000;

    static AutoPtr<CUserManagerService> sUserManager;

    static const Int32 DEX_OPT_SKIPPED = 0;
    static const Int32 DEX_OPT_PERFORMED = 1;
    static const Int32 DEX_OPT_DEFERRED = 2;
    static const Int32 DEX_OPT_FAILED = -1;

    static const Int32 UPDATE_PERMISSIONS_ALL = 1 << 0;
    static const Int32 UPDATE_PERMISSIONS_REPLACE_PKG = 1 << 1;
    static const Int32 UPDATE_PERMISSIONS_REPLACE_ALL = 1 << 2;

    // ------- apps on sdcard specific code -------
    static const Boolean DEBUG_SD_INSTALL;

    AutoPtr<ServiceThread> mHandlerThread;
    AutoPtr<IHandler> mHandler;//   final PackageHandler mHandler;

    AutoPtr<IContext> mContext;
    Boolean mFactoryTest;
    Boolean mOnlyCore;
    Boolean mLazyDexOpt;
    AutoPtr<IDisplayMetrics> mMetrics;
    Int32 mDefParseFlags;
    AutoPtr< ArrayOf<String> > mSeparateProcesses;

    // This is where all application persistent data goes.
    AutoPtr<IFile> mAppDataDir;

    // This is where all application persistent data goes for secondary users.
    AutoPtr<IFile> mUserAppDataDir;

    /** The location for ASEC container files on internal storage. */
    String mAsecInternalPath;

    // Used for priviledge escalation.  MUST NOT BE CALLED WITH mPackages
    // LOCK HELD.  Can be called with mInstallLock held.
    AutoPtr<Installer> mInstaller;

    /** Directory where installed third-party apps stored */
    AutoPtr<IFile> mAppInstallDir;

    // Directory containing the private parts (e.g. code and non-resource assets) of forward-locked
    // apps.
    AutoPtr<IFile> mDrmAppPrivateInstallDir;

    // ----------------------------------------------------------------

    // Lock for state used when installing and doing other long running
    // operations.  Methods that must be called with this lock held have
    // the suffix "LI".
    Object mInstallLock;

    // Keys are String (package name), values are Package.  This also serves
    // as the lock for the global state.  Methods that must be called with
    // this lock held have the prefix "LP".
    HashMap<String, AutoPtr<PackageParser::Package> > mPackages;
    Object mPackagesLock;

    // Tracks available target package names -> overlay package paths.
    HashMap<String, AutoPtr< HashMap<String, AutoPtr<PackageParser::Package> > > > mOverlays;

    AutoPtr<Settings> mSettings;
    Boolean mRestoredSettings;

    // System configuration read by SystemConfig.
    AutoPtr< ArrayOf<Int32> > mGlobalGids;
    AutoPtr<ISparseArray> mSystemPermissions;/*SparseArray<HashSet<String>> ;*/
    AutoPtr< HashMap<String, AutoPtr<IFeatureInfo> > > mAvailableFeatures;
    // ActionsCode(songzhining, new code: add extra hardware feature support)
    AutoPtr< HashMap<String, AutoPtr<IFeatureInfo> > > mExtraFeatures;
    // If mac_permissions.xml was found for seinfo labeling.
    Boolean mFoundPolicyFile;

    // Currently known shared libraries.
    HashMap<String, AutoPtr<SharedLibraryEntry> > mSharedLibraries;

    // All available activities, for your resolving pleasure.
    AutoPtr<ActivityIntentResolver> mActivities;

    // All available receivers, for your resolving pleasure.
    AutoPtr<ActivityIntentResolver> mReceivers;

    // All available services, for your resolving pleasure.
    AutoPtr<ServiceIntentResolver> mServices;

    // All available providers, for your resolving pleasure.
    AutoPtr<ProviderIntentResolver> mProviders;

    // Mapping from provider base names (first directory in content URI codePath)
    // to the provider information.
    HashMap<String, AutoPtr<PackageParser::Provider> > mProvidersByAuthority;

    // Mapping from instrumentation class names to info about them.
    HashMap<AutoPtr<IComponentName>, AutoPtr<PackageParser::Instrumentation> > mInstrumentation;

    // Mapping from permission names to info about them.
    HashMap<String, AutoPtr<PackageParser::PermissionGroup> > mPermissionGroups;

    // Packages whose data we have transfered into another package, thus
    // should no longer exist.
    HashSet<String> mTransferedPackages;

    // Broadcast actions that are only available to the system.
    HashSet<String> mProtectedBroadcasts;

    /** List of packages waiting for verification. */
    HashMap<Int32, AutoPtr<PackageVerificationState> > mPendingVerification;

    /** Set of packages associated with each app op permission. */
    HashMap<String, AutoPtr<HashSet<String> > > mAppOpPermissionPackages;

    AutoPtr<PackageInstallerService> mInstallerService;

    AutoPtr< HashSet<AutoPtr<PackageParser::Package> > > mDeferredDexOpt;

    // Cache of users who need badging.
    AutoPtr<ISparseBooleanArray> mUserNeedsBadging;

    Boolean mSystemReady;
    Boolean mSafeMode;
    Boolean mHasSystemUidErrors;

    AutoPtr<IApplicationInfo> mElastosApplication;
    AutoPtr<IActivityInfo> mResolveActivity;
    AutoPtr<IResolveInfo> mResolveInfo;
    AutoPtr<IComponentName> mResolveComponentName;
    AutoPtr<PackageParser::Package> mPlatformPackage;
    AutoPtr<IComponentName> mCustomResolverComponentName;

    Boolean mResolverReplaced;

    AutoPtr<PendingPackageBroadcasts> mPendingBroadcasts;

    HashMap<Int32, AutoPtr<PostInstallData> > mRunningInstalls;
    Int32 mNextInstallToken; // nonzero; will be wrapped back to 1 when ++ overflows

private:
    static const Boolean DEBUG_INSTALL = FALSE;
    static const Boolean DEBUG_REMOVE = FALSE;
    static const Boolean DEBUG_BROADCASTS = FALSE;
    static const Boolean DEBUG_SHOW_INFO = FALSE;
    static const Boolean DEBUG_PACKAGE_INFO = FALSE;
    static const Boolean DEBUG_INTENT_MATCHING = FALSE;
    static const Boolean DEBUG_PACKAGE_SCANNING = FALSE;
    static const Boolean DEBUG_VERIFY = FALSE;
    static const Boolean DEBUG_DEXOPT = FALSE;
    static const Boolean DEBUG_ABI_SELECTION = FALSE;

    static const Int32 RADIO_UID = IProcess::PHONE_UID;
    static const Int32 LOG_UID = IProcess::LOG_UID;
    static const Int32 NFC_UID = IProcess::NFC_UID;
    static const Int32 BLUETOOTH_UID = IProcess::BLUETOOTH_UID;
    static const Int32 SHELL_UID = IProcess::SHELL_UID;

    // Cap the size of permission trees that 3rd party apps can define
    static const Int32 MAX_PERMISSION_TREE_FOOTPRINT = 32768;     // characters of text

    // Suffix used during package installation when copying/moving
    // package apks to install directory.
    static const String INSTALL_PACKAGE_SUFFIX;

    /**
     * Timeout (in milliseconds) after which the watchdog should declare that
     * our handler thread is wedged.  The usual default for such things is one
     * minute but we sometimes do very lengthy I/O operations on this thread,
     * such as installing multi-gigabyte applications, so ours needs to be longer.
     */
    static const Int64 WATCHDOG_TIMEOUT = 1000 * 60 * 10;     // ten minutes

    /**
     * Wall-clock timeout (in milliseconds) after which we *require* that an fstrim
     * be run on this device.  We use the value in the Settings.Global.MANDATORY_FSTRIM_INTERVAL
     * settings entry if available, otherwise we use the hardcoded default.  If it's been
     * more than this long since the last fstrim, we force one during the boot sequence.
     *
     * This backstops other fstrim scheduling:  if the device is alive at midnight+idle,
     * one gets run at the next available charging+idle time.  This final mandatory
     * no-fstrim check kicks in only of the other scheduling criteria is never met.
     */
    static const Int64 DEFAULT_MANDATORY_FSTRIM_INTERVAL = 3 * IDateUtils::DAY_IN_MILLIS;

    /**
     * Whether verification is enabled by default.
     */
    static const Boolean DEFAULT_VERIFY_ENABLE = TRUE;

    /**
     * The default maximum time to wait for the verification agent to return in
     * milliseconds.
     */
    static const Int64 DEFAULT_VERIFICATION_TIMEOUT = 10 * 1000;

    /**
     * The default response for package verification timeout.
     *
     * This can be either PackageManager.VERIFICATION_ALLOW or
     * PackageManager.VERIFICATION_REJECT.
     */
    static const Int32 DEFAULT_VERIFICATION_RESPONSE = IPackageManager::VERIFICATION_ALLOW;

    static const String PACKAGE_MIME_TYPE;

    static const String VENDOR_OVERLAY_DIR;
    static const String VENDOR_APP_DIR;

    static String sPreferredInstructionSet;

    static const String IDMAP_PREFIX;
    static const String IDMAP_SUFFIX;

    static const String SD_ENCRYPTION_KEYSTORE_NAME;

    static const String SD_ENCRYPTION_ALGORITHM;

    /**
     * Messages for {@link #mHandler} that need to wait for system ready before
     * being dispatched.
     */
    AutoPtr<IArrayList> mPostSystemReadyMessages;

    /**
     * Directory to which applications installed internally have their
     * 32 bit native libraries copied.
     */
    AutoPtr<IFile> mAppLib32InstallDir;

    // If a recursive restorecon of /data/data/<pkg> is needed.
    Boolean mShouldRestoreconData;

    /** Token for keys in mPendingVerification. */
    Int32 mPendingVerificationToken;

    // Service Connection to remote media container service to copy
    // package uri's from external media onto secure containers
    // or internal storage.
    AutoPtr<IMediaContainerService> mContainerService;

    // Stores a list of users whose package restrictions file needs to be updated
    HashSet<Int32> mDirtyUsers;

    AutoPtr<DefaultContainerConnection> mDefContainerConn;

    String mRequiredVerifierPackage;

    AutoPtr<PackageUsage> mPackageUsage;

    static AutoPtr<IComparator> mResolvePrioritySorter;

    static AutoPtr<IComparator> mProviderInitOrderSorter;

    Boolean mMediaMounted;

    friend class PackageUsage;
    friend class PackageHandler;
};

} // namespace Pm
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_PM_CPACKAGEMANAGERSERVICE_H__
