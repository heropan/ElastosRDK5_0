#ifndef __CPACKAGEMANAGERSERVICE_H__
#define __CPACKAGEMANAGERSERVICE_H__

#include "ext/frameworkext.h"
#include "_CPackageManagerService.h"
#include "pm/PackageVerificationResponse.h"
#include "pm/PackageVerificationState.h"
#include "pm/Installer.h"
#include "pm/Settings.h"
#include "pm/CUserManagerService.h"
#include "content/pm/PackageParser.h"
#include "content/BroadcastReceiver.h"
#include "os/Build.h"
#include "os/FileObserver.h"
#include "os/HandlerBase.h"
#include "os/Runnable.h"
#include <elastos/utility/etl/List.h>
#include <elastos/utility/etl/HashMap.h>
#include <elastos/utility/etl/HashSet.h>

using Elastos::Core::IInteger32;
using Elastos::Utility::Etl::List;
using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::Etl::HashSet;
using Elastos::Utility::ISet;
using Elastos::IO::IFile;
using Elastos::IO::IFilenameFilter;

using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::IServiceConnection;
using Elastos::Droid::Content::IIntentReceiver;
using Elastos::Droid::Content::IIntentSender;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Internal::Content::CPackageHelper;
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
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IProcess;
using Elastos::Droid::Os::IFileObserver;
using Elastos::Droid::Os::FileObserver;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::Build;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Os::IHandlerThread;
using Elastos::Droid::Internal::App::IMediaContainerService;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {

class CPackageManagerService;

class InstallArgs
    : public ElRefBase
    , public IInterface
{
public:
    CAR_INTERFACE_DECL()

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

    class PackageHandler : public HandlerBase
    {
    public:
        PackageHandler(
            /* [in] */ ILooper* looper,
            /* [in] */ CPackageManagerService* host)
            : HandlerBase(looper)
            , mHost(host)
        {}

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

        CARAPI_(void) DoHandleMessage(
            /* [in] */ IMessage* msg);

    private:
        CPackageManagerService* mHost;
    };

    class DefaultContainerConnection
        : public ElRefBase
        , public IServiceConnection
    {
    public:
        DefaultContainerConnection(
            /* [in] */ CPackageManagerService* owner)
            : mHost(owner)
        {}

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

    private:
        CPackageManagerService* mHost;
    };

    class PackageInstalledInfo;

    // Recordkeeping of restore-after-install operations that are currently in flight
    // between the Package Manager and the Backup Manager
    class PostInstallData : public ElRefBase
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

    class HandlerParams
        : public ElRefBase
        , public IInterface
    {
    public:
        CAR_INTERFACE_DECL()

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

    class NotifyRunnable
        : public ElRefBase
        , public IRunnable
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

        CAR_INTERFACE_DECL()

        CARAPI Run();

    private:
        CPackageManagerService* mHost;
        Int32 mFreeStorageSize;
        AutoPtr<IPackageDataObserver> mObserver;
    };

    class FreeStorageRunnable
        : public ElRefBase
        , public IRunnable
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

        CAR_INTERFACE_DECL()

        CARAPI Run();

    private:
        CPackageManagerService* mHost;
        Int64 mFreeStorageSize;
        AutoPtr<IIntentSender> mPi;
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

    class ProcessRunnable
        : public ElRefBase
        , public IRunnable
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

        CAR_INTERFACE_DECL()

        CARAPI Run();

    private:
        CPackageManagerService* mHost;
        AutoPtr<MoveParams> mMp;
        Int32 mCurrentStatus;
    };

private:
    class ActivityIntentResolver
        : public IntentResolver<PackageParser::ActivityIntentInfo, IResolveInfo>
    {
    public:
        typedef IntentResolver<PackageParser::ActivityIntentInfo, IResolveInfo>  Super;

    public:
        ActivityIntentResolver(
            /* [in] */ CPackageManagerService* owner);

        ~ActivityIntentResolver();

        CARAPI_(AutoPtr< List< AutoPtr<IResolveInfo> > >) QueryIntent(
            /* [in] */ IIntent* intent,
            /* [in] */ const String& resolvedType,
            /* [in] */ Boolean defaultOnly,
            /* [in] */ Int32 userId);

        CARAPI_(AutoPtr< List< AutoPtr<IResolveInfo> > >) QueryIntent(
            /* [in] */ IIntent* intent,
            /* [in] */ const String& resolvedType,
            /* [in] */ Int32 flags,
            /* [in] */ Int32 userId);

        CARAPI_(AutoPtr< List< AutoPtr<IResolveInfo> > >) QueryIntentForPackage(
            /* [in] */ IIntent* intent,
            /* [in] */ const String& resolvedType,
            /* [in] */ Int32 flags,
            /* [in] */ List< AutoPtr<PackageParser::Activity> >* packageActivities,
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
            /* [in] */ List< AutoPtr<IResolveInfo> > * dest);

        //@Override
        CARAPI_(AutoPtr< ArrayOf<PackageParser::ActivityIntentInfo*> >) NewArray(
            /* [in] */ Int32 size);

        //@Override
        CARAPI_(Boolean) IsFilterStopped(
            /* [in] */ PackageParser::ActivityIntentInfo* filter,
            /* [in] */ Int32 userId);

        //@Override
        CARAPI_(String) PackageForFilter(
            /* [in] */ PackageParser::ActivityIntentInfo* info);

        //@Override
        CARAPI_(AutoPtr<IResolveInfo>) NewResult(
            /* [in] */ PackageParser::ActivityIntentInfo* info,
            /* [in] */ Int32 match,
            /* [in] */ Int32 userId);

        //@Override
        CARAPI_(void) SortResults(
            /* [in] */ List< AutoPtr<IResolveInfo> >* results);

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

    class ServiceIntentResolver
        : public IntentResolver<PackageParser::ServiceIntentInfo, IResolveInfo>
    {
    public:
        typedef IntentResolver<PackageParser::ServiceIntentInfo, IResolveInfo> Super;

    public:
        ServiceIntentResolver(
            /* [in] */ CPackageManagerService* owner);

        ~ServiceIntentResolver();

        CARAPI_(AutoPtr< List< AutoPtr<IResolveInfo> > >) QueryIntent(
            /* [in] */ IIntent* intent,
            /* [in] */ const String& resolvedType,
            /* [in] */ Boolean defaultOnly,
            /* [in] */ Int32 userId);

        CARAPI_(AutoPtr< List< AutoPtr<IResolveInfo> > >) QueryIntent(
            /* [in] */ IIntent* intent,
            /* [in] */ const String& resolvedType,
            /* [in] */ Int32 flags,
            /* [in] */ Int32 userId);

        CARAPI_(AutoPtr< List< AutoPtr<IResolveInfo> > >) QueryIntentForPackage(
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
            /* [in] */ List< AutoPtr<IResolveInfo> >* dest);

        //@Override
        CARAPI_(AutoPtr< ArrayOf<PackageParser::ServiceIntentInfo*> >) NewArray(
            /* [in] */ Int32 size);

        //@Override
        CARAPI_(Boolean) IsFilterStopped(
            /* [in] */ PackageParser::ServiceIntentInfo* filter,
            /* [in] */ Int32 userId);

        //@Override
        CARAPI_(String) PackageForFilter(
            /* [in] */ PackageParser::ServiceIntentInfo* info);

        //@Override
        CARAPI_(AutoPtr<IResolveInfo>) NewResult(
            /* [in] */ PackageParser::ServiceIntentInfo* info,
            /* [in] */ Int32 match,
            /* [in] */ Int32 userId);

        //@Override
        CARAPI_(void) SortResults(
            /* [in] */ List< AutoPtr<IResolveInfo> >* results);

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

    CARAPI_(void) ScheduleWriteSettingsLocked();

    CARAPI_(void) ScheduleWritePackageRestrictionsLocked(
        /* [in] */ Int32 userId);

    static CARAPI_(AutoPtr<IIPackageManager>) Main(
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

    CARAPI_(void) ReadPermissions();

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
        /* [out, callee] */ IObjectContainer** infos);

    CARAPI GetPermissionGroupInfo(
        /* [in] */ const String& name,
        /* [in] */ Int32 flags,
        /* [out] */ IPermissionGroupInfo** info);

    CARAPI GetAllPermissionGroups(
        /* [in] */ Int32 flags,
        /* [out, callee] */ IObjectContainer** infos);

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

    CARAPI GetActivityInfo(
        /* [in] */ IComponentName* component,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IActivityInfo** info);

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

    CARAPI ResolveIntent(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IResolveInfo** resolveInfo);

    CARAPI_(AutoPtr<IResolveInfo>) FindPreferredActivity(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [in] */ IObjectContainer* query,
        /* [in] */ Int32 priority,
        /* [in] */ Int32 userId);

    CARAPI QueryIntentActivities(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out, callee] */ IObjectContainer** infos);

    CARAPI QueryIntentActivityOptions(
        /* [in] */ IComponentName* caller,
        /* [in] */ ArrayOf<IIntent*>* specifics,
        /* [in] */ ArrayOf<String>* specificTypes,
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IObjectContainer** infos);

    CARAPI QueryIntentReceivers(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IObjectContainer** receivers);

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
        /* [out] */ IObjectContainer** services);

    CARAPI GetInstalledPackages(
        /* [in] */ Int32 flags,
        /* [in] */ const String& lastRead,
        /* [in] */ Int32 userId,
        /* [out] */ IParceledListSlice** slice);

    CARAPI GetInstalledApplications(
        /* [in] */ Int32 flags,
        /* [in] */ const String& lastRead,
        /* [in] */ Int32 userId,
        /* [out] */ IParceledListSlice** slice);

    CARAPI GetPersistentApplications(
        /* [in] */ Int32 flags,
        /* [out, callee] */ IObjectContainer** infos);

    CARAPI ResolveContentProvider(
        /* [in] */ const String& name,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IProviderInfo** info);

    CARAPI QuerySyncProviders(
        /* [in, out] */ IObjectContainer* outNames,
        /* [in, out] */ IObjectContainer* outInfo);

    CARAPI QueryContentProviders(
        /* [in] */ const String& processName,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 flags,
        /* [out] */ IObjectContainer** providers);

    CARAPI GetInstrumentationInfo(
        /* [in] */ IComponentName* name,
        /* [in] */ Int32 flags,
        /* [out] */ IInstrumentationInfo** instInfo);

    CARAPI QueryInstrumentation(
        /* [in] */ const String& targetPackage,
        /* [in] */ Int32 flags,
        /* [out, callee] */ IObjectContainer** infos);

    static CARAPI_(void) ReportSettingsProblem(
        /* [in] */ Int32 priority,
        /* [in] */ const String& msg);

    CARAPI PerformBootDexOpt();

    CARAPI PerformDexOpt(
        /* [in] */ const String& packageName,
        /* [out] */ Boolean* result);

    CARAPI_(Int32) PerformDexOptLI(
        /* [in] */ PackageParser::Package* pkg,
        /* [in] */ Boolean forceDex,
        /* [in] */ Boolean defer);

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
        /* [out, callee] */ IObjectContainer** infos);

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
        /* [in, out] */ IObjectContainer* outFilters,
        /* [in, out] */ IObjectContainer* outActivities,
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
    CARAPI_(String) GetRequiredVerifierLPr();

    CARAPI_(void) ReadPermissionsFromXml(
        /* [in] */ IFile* permFile);

    CARAPI CheckValidCaller(
        /* [in] */ Int32 uid,
        /* [in] */ Int32 userId);

    CARAPI EnforceCrossUserPermission(
        /* [in] */ Int32 callingUid,
        /* [in] */ Int32 userId,
        /* [in] */ Boolean requireFullPermission,
        /* [in] */ const String& message);

    CARAPI_(AutoPtr<BasePermission>) FindPermissionTreeLP(
        /* [in] */ const String& permName);

    CARAPI CheckPermissionTreeLP(
        /* [in] */ const String& permName,
        /* [out] */ BasePermission** permission);

    CARAPI ChooseBestActivity(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 flags,
        /* [in] */ IObjectContainer* query,
        /* [in] */ Int32 userId,
        /* [out] */ IResolveInfo** resolveInfo);

    CARAPI_(Int32) GetContinuationPoint(
        /* [in] */ const ArrayOf<String>& keys,
        /* [in] */ const String& key);

    CARAPI ScanDirLI(
        /* [in] */ IFile* dir,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 scanMode,
        /* [in] */ Int64 currentTime,
        /* [in] */ ArrayOf<Byte>* readBuffer);

    static CARAPI_(AutoPtr<IFile>) GetSettingsProblemFile();

    CARAPI_(Boolean) CollectCertificatesLI(
        /* [in] */ PackageParser* pp,
        /* [in] */ PackageSetting* ps,
        /* [in] */ PackageParser::Package* pkg,
        /* [in] */ IFile* srcFile,
        /* [in] */ Int32 parseFlags,
        /* [in] */ ArrayOf<Byte>* readBuffer);

    CARAPI_(AutoPtr<PackageParser::Package>) ScanPackageLI(
        /* [in] */ IFile* scanFile,
        /* [in] */ Int32 parseFlags,
        /* [in] */ Int32 scanMode,
        /* [in] */ Int64 currentTime,
        /* [in] */ IUserHandle* user,
        /* [in] */ ArrayOf<Byte>* readBuffer);

    static CARAPI_(void) SetApplicationInfoPaths(
        /* [in] */ PackageParser::Package* pkg,
        /* [in] */ const String& destCodePath,
        /* [in] */ const String& destResPath);

    static CARAPI_(String) FixProcessName(
        /* [in] */ const String& defProcessName,
        /* [in] */ const String& processName,
        /* [in] */ Int32 uid);

    CARAPI_(Boolean) VerifySignaturesLP(
        /* [in] */ PackageSetting* pkgSetting,
        /* [in] */ PackageParser::Package* pkg);

    static CARAPI EnforceSystemOrRoot(
        /* [in] */ const String& message);

    CARAPI_(Boolean) VerifyPackageUpdateLPr(
        /* [in] */ PackageSetting* oldPkg,
        /* [in] */ PackageParser::Package* newPkg);

    CARAPI_(AutoPtr<IFile>) GetDataPathForPackage(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 userId);

    CARAPI_(Int32) CreateDataDirsLI(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid);

    CARAPI_(Int32) RemoveDataDirsLI(
        /* [in] */ const String& packageName);

    CARAPI_(AutoPtr<PackageParser::Package>) ScanPackageLI(
        /* [in] */ PackageParser::Package* pkg,
        /* [in] */ Int32 parseFlags,
        /* [in] */ Int32 scanMode,
        /* [in] */ Int64 currentTime,
        /* [in] */ IUserHandle* user,
        /* [in] */ ArrayOf<Byte>* readBuffer);

    CARAPI_(void) SetInternalAppNativeLibraryPath(
        /* [in] */ PackageParser::Package* pkg,
        /* [in] */ PackageSetting* pkgSetting);

    static CARAPI CopyNativeLibrariesForInternalApp(
        /* [in] */ IFile* scanFile,
        /* [in] */ IFile* nativeLibraryDir,
        /* [out] */ Int32* result);

    CARAPI_(void) KillApplication(
        /* [in] */ const String& pkgName,
        /* [in] */ Int32 appId);

    static CARAPI_(Boolean) IsPackageFilename(
        /* [in] */ const String& name);

    static CARAPI_(Boolean) HasPermission(
        /* [in] */ PackageParser::Package* pkgInfo,
        /* [in] */ const String& perm);

    CARAPI_(void) UpdatePermissionsLPw(
        /* [in] */ const String& changingPkg,
        /* [in] */ PackageParser::Package* pkgInfo,
        /* [in] */ Int32 flags);

    CARAPI_(void) GrantPermissionsLPw(
        /* [in] */ PackageParser::Package* pkg,
        /* [in] */ Boolean replace);

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

    CARAPI_(Boolean) ConnectToService();

    CARAPI_(void) DisconnectService();

    CARAPI_(void) HandleInitCopy(
        /* [in] */ HandlerParams* params);

    CARAPI_(void) HandleMCSBound(
        /* [in] */ IMediaContainerService* service);

    CARAPI_(void) HandleMCSReconnect();

    CARAPI_(void) HandleMCSUnbind();

    CARAPI_(void) HandleMCSGiveUp();

    CARAPI_(void) HandleSendPendingBroadcast();

    CARAPI_(void) HandlePostInstall(
        /* [in] */ Int32 token);

    CARAPI_(void) HandleCheckPendingVerification(
        /* [in] */ Int32 verificationId);

    CARAPI_(void) HandlePackageVerified(
        /* [in] */ Int32 verificationId,
        /* [in] */ PackageVerificationResponse* response);

    CARAPI_(void) HandleDeletePackage(
        /* [in] */ const String& packageName,
        /* [in] */ IPackageDeleteObserver* observer,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 uid);

    CARAPI_(void) HandleStartCleaningPackage(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 userId,
        /* [in] */ Boolean andCode);

    CARAPI_(void) HandleUpdatedMediaStatus(
        /* [in] */ Boolean reportStatus,
        /* [in] */ Boolean doGc,
        /* [in] */ ISet* args);

    CARAPI_(void) HandleWriteSettings();

    CARAPI_(void) HandleWritePackageRestriction();


public:/*package*/
    static const String TAG;
    static const Boolean DEBUG_SETTINGS;
    static const Boolean DEBUG_UPGRADE;
    static const Int32 SCAN_MONITOR = 1 << 0;
    static const Int32 SCAN_NO_DEX = 1 << 1;
    static const Int32 SCAN_FORCE_DEX = 1 << 2;
    static const Int32 SCAN_UPDATE_SIGNATURE = 1 << 3;
    static const Int32 SCAN_NEW_INSTALL = 1 << 4;
    static const Int32 SCAN_NO_PATHS = 1 << 5;
    static const Int32 SCAN_UPDATE_TIME = 1 << 6;
    static const Int32 SCAN_DEFER_DEX = 1 << 7;
    static const Int32 SCAN_BOOTING = 1 << 8;

    static const Int32 REMOVE_CHATTY = 1 << 16;

    static const String DEFAULT_CONTAINER_PACKAGE;

    static const AutoPtr<IComponentName> DEFAULT_CONTAINER_COMPONENT;

    static const String sTempContainerPrefix;

    AutoPtr<IHandlerThread> mHandlerThread;
    AutoPtr<IHandler> mHandler;//   final PackageHandler mHandler;
    Boolean mBound;
    List<AutoPtr<HandlerParams> > mPendingInstalls;

    const Int32 mSdkVersion;
//      final String mSdkCodename = "REL".equals(Build.VERSION.CODENAME)
//              ? null : Build.VERSION.CODENAME;

    AutoPtr<IContext> mContext;
    Boolean mFactoryTest;
    Boolean mOnlyCore;
    Boolean mNoDexOpt;
    AutoPtr<IDisplayMetrics> mMetrics;
    Int32 mDefParseFlags;
    AutoPtr< ArrayOf<String> > mSeparateProcesses;

    // This is where all application persistent data goes.
    AutoPtr<IFile> mAppDataDir;

    // This is where all application persistent data goes for secondary users.
    AutoPtr<IFile> mUserAppDataDir;

    /** The location for ASEC container files on internal storage. */
    String mAsecInternalPath;

    // This is the object monitoring the framework dir.
    AutoPtr<IFileObserver> mFrameworkInstallObserver;

    // This is the object monitoring the system app dir.
    AutoPtr<IFileObserver> mSystemInstallObserver;

    // This is the object monitoring the system app dir.
    AutoPtr<IFileObserver> mVendorInstallObserver;

    // This is the object monitoring mAppInstallDir.
    AutoPtr<IFileObserver> mAppInstallObserver;

    // This is the object monitoring mDrmAppPrivateInstallDir.
    AutoPtr<IFileObserver> mDrmAppInstallObserver;

    // Used for priviledge escalation.  MUST NOT BE CALLED WITH mPackages
    // LOCK HELD.  Can be called with mInstallLock held.
    AutoPtr<Installer> mInstaller;

    AutoPtr<IFile> mFrameworkDir;
    AutoPtr<IFile> mSystemAppDir;
    AutoPtr<IFile> mVendorAppDir;
    AutoPtr<IFile> mAppInstallDir;
    AutoPtr<IFile> mDalvikCacheDir;

    // Directory containing the private parts (e.g. code and non-resource assets) of forward-locked
    // apps.
    AutoPtr<IFile> mDrmAppPrivateInstallDir;

    // ----------------------------------------------------------------

    // Lock for state used when installing and doing other long running
    // operations.  Methods that must be called with this lock held have
    // the prefix "LI".
    Mutex mInstallLock;

    // These are the directories in the 3rd party applications installed dir
    // that we have currently loaded packages from.  Keys are the application's
    // installed zip file (absolute codePath), and values are Package.
    HashMap<String, AutoPtr<PackageParser::Package> > mAppDirs;

    // Information for the parser to write more useful error messages.
    AutoPtr<IFile> mScanningPath;
    Int32 mLastScanError;

    // Keys are String (package name), values are Package.  This also serves
    // as the lock for the global state.  Methods that must be called with
    // this lock held have the prefix "LP".
    HashMap<String, AutoPtr<PackageParser::Package> > mPackages;
    Mutex mPackagesLock;

    AutoPtr<Settings> mSettings;
    Boolean mRestoredSettings;

    // Group-ids that are given to all packages as read from etc/permissions/*.xml.
    AutoPtr< ArrayOf<Int32> > mGlobalGids;

    // These are the built-in uid -> permission mappings that were read from the
    // etc/permissions.xml file.
    HashMap<Int32, AutoPtr<HashSet<String> > > mSystemPermissions;

    // These are the built-in shared libraries that were read from the
    // etc/permissions.xml file.
    HashMap<String, String> mSharedLibraries;

    // Temporary for building the final shared libraries for an .apk.
    AutoPtr< ArrayOf<String> > mTmpSharedLibraries;

    // These are the features this devices supports that were read from the
    // etc/permissions.xml file.
    HashMap<String, AutoPtr<IFeatureInfo> > mAvailableFeatures;

    // All available activities, for your resolving pleasure.
    AutoPtr<ActivityIntentResolver> mActivities;

    // All available receivers, for your resolving pleasure.
    AutoPtr<ActivityIntentResolver> mReceivers;

    // All available services, for your resolving pleasure.
    AutoPtr<ServiceIntentResolver> mServices;

    // Keys are String (provider class name), values are Provider.
    HashMap<AutoPtr<IComponentName>, AutoPtr<PackageParser::Provider> > mProvidersByComponent;

    // Mapping from provider base names (first directory in content URI codePath)
    // to the provider information.
    HashMap<String, AutoPtr<PackageParser::Provider> > mProviders;

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

    List< AutoPtr<PackageParser::Package> > mDeferredDexOpt;

    Boolean mSystemReady;
    Boolean mSafeMode;
    Boolean mHasSystemUidErrors;

    AutoPtr<IApplicationInfo> mElastosApplication;
    AutoPtr<IActivityInfo> mResolveActivity;
    AutoPtr<IResolveInfo> mResolveInfo;
    AutoPtr<IComponentName> mResolveComponentName;
    AutoPtr<PackageParser::Package> mPlatformPackage;

    // Set of pending broadcasts for aggregating enable/disable of components.
    HashMap< String, AutoPtr<List<String> > > mPendingBroadcasts;

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

    HashMap<Int32, AutoPtr<PostInstallData> > mRunningInstalls;
    Int32 mNextInstallToken; // nonzero; will be wrapped back to 1 when ++ overflows

    static const Int32 DEX_OPT_SKIPPED = 0;
    static const Int32 DEX_OPT_PERFORMED = 1;
    static const Int32 DEX_OPT_DEFERRED = 2;
    static const Int32 DEX_OPT_FAILED = -1;

    static const Int32 UPDATE_PERMISSIONS_ALL = 1 << 0;
    static const Int32 UPDATE_PERMISSIONS_REPLACE_PKG = 1 << 1;
    static const Int32 UPDATE_PERMISSIONS_REPLACE_ALL = 1 << 2;

    // ------- apps on sdcard specific code -------
    static const Boolean DEBUG_SD_INSTALL;

private:
    static const Boolean DEBUG_PREFERRED;
    static const Boolean DEBUG_INSTALL;
    static const Boolean DEBUG_REMOVE;
    static const Boolean DEBUG_BROADCASTS;
    static const Boolean DEBUG_SHOW_INFO;
    static const Boolean DEBUG_PACKAGE_INFO;
    static const Boolean DEBUG_INTENT_MATCHING;
    static const Boolean DEBUG_PACKAGE_SCANNING;
    static const Boolean DEBUG_APP_DIR_OBSERVER;
    static const Boolean DEBUG_VERIFY;

    static const Int32 RADIO_UID = IProcess::PHONE_UID;
    static const Int32 LOG_UID = IProcess::LOG_UID;
    static const Int32 NFC_UID = IProcess::NFC_UID;
    static const Int32 BLUETOOTH_UID = IProcess::BLUETOOTH_UID;

    static const Boolean GET_CERTIFICATES = TRUE;

    static const Int32 REMOVE_EVENTS =
        IFileObserver::CLOSE_WRITE | IFileObserver::DELETE | IFileObserver::MOVED_FROM;
    static const Int32 ADD_EVENTS =
        IFileObserver::CLOSE_WRITE /*| IFileObserver::CREATE*/ | IFileObserver::MOVED_TO;

    static const Int32 OBSERVER_EVENTS = REMOVE_EVENTS | ADD_EVENTS;
    // Suffix used during package installation when copying/moving
    // package apks to install directory.
    static const String INSTALL_PACKAGE_SUFFIX;

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

    static const String LIB_DIR_NAME;

    /**
     * Directory to which applications installed internally have native
     * libraries copied.
     */
    AutoPtr<IFile> mAppLibInstallDir;

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

    static const String SD_ENCRYPTION_KEYSTORE_NAME;

    static const String SD_ENCRYPTION_ALGORITHM;

    Boolean mMediaMounted;
};

} // namespace Pm
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__CPACKAGEMANAGERSERVICE_H__
