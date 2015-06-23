
#include "ext/frameworkdef.h"
#include "pm/Settings.h"
#include "pm/CPackageManagerService.h"
#include "pm/PackageSettingBase.h"
#include "os/Process.h"
#include "os/FileUtils.h"
#include "os/UserHandle.h"
#include "os/Binder.h"
#include "util/Xml.h"
#include "util/XmlUtils.h"
#include "util/JournaledFile.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Core::EIID_IInteger32;
using Elastos::Core::IBoolean;
using Elastos::Core::CBoolean;
using Elastos::IO::CFile;
using Elastos::IO::IFileInputStream;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::CFileInputStream;
using Elastos::IO::CFileOutputStream;
using Elastos::IO::IBufferedOutputStream;
using Elastos::IO::CBufferedOutputStream;
using Elastos::IO::IIoUtils;
using Elastos::IO::CIoUtils;
using Elastos::IO::IFlushable;
using Elastos::Text::ISimpleDateFormat;
using Elastos::Text::CSimpleDateFormat;
using Elastos::Utility::IDate;
using Elastos::Utility::CDate;
using Elastos::Droid::Internal::Utility::JournaledFile;
using Elastos::Utility::Logging::Slogger;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Utility::CFastXmlSerializer;
using Elastos::Droid::Utility::IFastXmlSerializer;
using Elastos::Droid::Utility::Xml;
using Elastos::Droid::Content::Pm::CPackageCleanItem;
using Elastos::Droid::Content::Pm::CUserInfo;
using Elastos::Droid::Content::Pm::CVerifierDeviceIdentityHelper;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Pm::IVerifierDeviceIdentityHelper;
using Elastos::Droid::Content::Pm::CPermissionInfo;
using Elastos::Droid::Content::Pm::IPermissionInfoHelper;
using Elastos::Droid::Content::Pm::CPermissionInfoHelper;
using Elastos::Droid::Os::IEnvironment;
using Elastos::Droid::Os::CEnvironment;
using Elastos::Droid::Os::FileUtils;
using Elastos::Droid::Os::IProcess;
using Elastos::Droid::Os::IUserHandleHelper;
using Elastos::Droid::Os::CUserHandleHelper;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::Os::Binder;
using Elastos::Droid::Server::Pm::CPackageManagerService;
using Elastos::Droid::Server::Pm::PackageSettingBase;
using Elastos::Droid::Utility::XmlUtils;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {

const String Settings::TAG = String("PackageSettings");
const Boolean Settings::DEBUG_STOPPED;
const Boolean Settings::DEBUG_MU;
const String Settings::TAG_READ_EXTERNAL_STORAGE = String("read-external-storage");
const String Settings::ATTR_ENFORCEMENT = String("enforcement");
const String Settings::TAG_ITEM = String("item");
const String Settings::TAG_DISABLED_COMPONENTS = String("disabled-components");
const String Settings::TAG_ENABLED_COMPONENTS = String("enabled-components");
const String Settings::TAG_PACKAGE_RESTRICTIONS = String("package-restrictions");
const String Settings::TAG_PACKAGE = String("pkg");
const String Settings::ATTR_NAME = String("name");
const String Settings::ATTR_USER = String("user");
const String Settings::ATTR_CODE = String("code");
const String Settings::ATTR_NOT_LAUNCHED = String("nl");
const String Settings::ATTR_ENABLED = String("enabled");
const String Settings::ATTR_STOPPED = String("stopped");
const String Settings::ATTR_INSTALLED = String("inst");

Settings::Settings(
    /* [in] */ IContext* context)
    : mPackages(50)
    , mInternalSdkPlatform(0)
    , mExternalSdkPlatform(0)
    , mPreferredActivities(20)
    , mSharedUsers(20)
    , mPermissions(50)
    , mPermissionTrees(50)
    , mRenamedPackages(50)
    , mDisabledSysPackages(20)
    , mOtherUserIds(20)
{
    AutoPtr<IEnvironment> env;
    CEnvironment::AcquireSingleton((IEnvironment**)&env);
    AutoPtr<IFile> dataDir;
    env->GetDataDirectory((IFile**)&dataDir);
    Init(context, dataDir);
}

Settings::Settings(
    /* [in] */ IContext* context,
    /* [in] */ IFile* dataDir)
    : mPackages(50)
    , mInternalSdkPlatform(0)
    , mExternalSdkPlatform(0)
    , mPreferredActivities(20)
    , mSharedUsers(20)
    , mPermissions(50)
    , mPermissionTrees(50)
    , mRenamedPackages(50)
    , mDisabledSysPackages(20)
    , mOtherUserIds(20)
{
    Init(context, dataDir);
}

void Settings::Init(
    /* [in] */ IContext* context,
    /* [in] */ IFile* dataDir)
{
    mContext = context;
    CFile::New(dataDir, String("system"), (IFile**)&mSystemDir);
    Boolean result;
    String temp;
    mSystemDir->Mkdirs(&result);
    FileUtils::SetPermissions((mSystemDir->ToString(&temp), temp),
            FileUtils::sS_IRWXU|FileUtils::sS_IRWXG
            |FileUtils::sS_IROTH|FileUtils::sS_IXOTH,
            -1, -1);
    CFile::New(mSystemDir, String("packages.xml"), (IFile**)&mSettingsFilename);
    CFile::New(mSystemDir, String("packages-backup.xml"), (IFile**)&mBackupSettingsFilename);
    CFile::New(mSystemDir, String("packages.list"), (IFile**)&mPackageListFilename);
    // Deprecated: Needed for migration
    CFile::New(mSystemDir, String("packages-stopped.xml"), (IFile**)&mStoppedPackagesFilename);
    CFile::New(mSystemDir, String("packages-stopped-backup.xml"), (IFile**)&mBackupStoppedPackagesFilename);
    mReadMessages = new StringBuilder();
}

AutoPtr<PackageSetting> Settings::GetPackageLPw(
    /* [in] */ PackageParser::Package* pkg,
    /* [in] */ PackageSetting* origPackage,
    /* [in] */ const String& realName,
    /* [in] */ SharedUserSetting* sharedUser,
    /* [in] */ IFile* codePath,
    /* [in] */ IFile* resourcePath,
    /* [in] */ const String& nativeLibraryPathString,
    /* [in] */ Int32 pkgFlags,
    /* [in] */ IUserHandle* user,
    /* [in] */ Boolean add)
{
    const String& name = pkg->mPackageName;
    AutoPtr<PackageSetting> p = GetPackageLPw(name, origPackage, realName, sharedUser, codePath,
            resourcePath, nativeLibraryPathString, pkg->mVersionCode, pkgFlags,
            user, add, TRUE /* allowInstall */);
    return p;
}

AutoPtr<PackageSetting> Settings::PeekPackageLPr(
    /* [in] */ const String& name)
{
    AutoPtr<PackageSetting> p;
    HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mPackages.Find(name);
    if (it != mPackages.End()) {
        p = it->mSecond;
    }
    return p;
}

void Settings::SetInstallStatus(
    /* [in] */ const String& pkgName,
    /* [in] */ Int32 status)
{
    AutoPtr<PackageSetting> p;
    HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mPackages.Find(pkgName);
    if (it != mPackages.End()) {
        p = it->mSecond;
    }
    if(p != NULL) {
        if(p->GetInstallStatus() != status) {
            p->SetInstallStatus(status);
        }
    }
}

void Settings::SetInstallerPackageName(
    /* [in] */ const String& pkgName,
    /* [in] */ const String& installerPkgName)
{
    AutoPtr<PackageSetting> p;
    HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mPackages.Find(pkgName);
    if (it != mPackages.End()) {
        p = it->mSecond;
    }
    if(p != NULL) {
        p->SetInstallerPackageName(installerPkgName);
    }
}

AutoPtr<SharedUserSetting> Settings::GetSharedUserLPw(
    /* [in] */ const String& name,
    /* [in] */ Int32 pkgFlags,
    /* [in] */ Boolean create)
{
    AutoPtr<SharedUserSetting> s;
    HashMap<String, AutoPtr<SharedUserSetting> >::Iterator it = mSharedUsers.Find(name);
    if (it != mSharedUsers.End()) {
        s = it->mSecond;
    }
    if (s == NULL) {
        if (!create) {
            return NULL;
        }
        s = new SharedUserSetting(name, pkgFlags);
        s->mUserId = NewUserIdLPw(s);
        Slogger::I(CPackageManagerService::TAG, "New shared user %s: id=%d", name.string(), s->mUserId);
        // < 0 means we couldn't assign a userid; fall out and return
        // s, which is currently null
        if (s->mUserId >= 0) {
            mSharedUsers[name] = s;
        }
    }

    return s;
}

Boolean Settings::DisableSystemPackageLPw(
    /* [in] */ const String& name)
{
    AutoPtr<PackageSetting> p;
    HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mPackages.Find(name);
    if (it != mPackages.End()) {
        p = it->mSecond;
    }
    if(p == NULL) {
        Slogger::W(CPackageManagerService::TAG, "Package:%s is not an installed package", name.string());
        return FALSE;
    }
    AutoPtr<PackageSetting> dp;
    HashMap<String, AutoPtr<PackageSetting> >::Iterator dpIt = mDisabledSysPackages.Find(name);
    if (dpIt != mDisabledSysPackages.End()) {
        dp = dpIt->mSecond;
    }
    // always make sure the system package code and resource paths dont change
    if (dp == NULL) {
        if((p->mPkg != NULL) && (p->mPkg->mApplicationInfo != NULL)) {
            Int32 flags;
            p->mPkg->mApplicationInfo->GetFlags(&flags);
            flags |= IApplicationInfo::FLAG_UPDATED_SYSTEM_APP;
            p->mPkg->mApplicationInfo->SetFlags(flags);
        }
        mDisabledSysPackages[name] = p;

        // a little trick...  when we install the new package, we don't
        // want to modify the existing PackageSetting for the built-in
        // version.  so at this point we need a new PackageSetting that
        // is okay to muck with.
        AutoPtr<PackageSetting> newp = new PackageSetting(p);
        ReplacePackageLPw(name, newp);
        return TRUE;
    }
    return FALSE;
}

AutoPtr<PackageSetting> Settings::EnableSystemPackageLPw(
    /* [in] */ const String& name)
{
    AutoPtr<PackageSetting> p;
    HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mDisabledSysPackages.Find(name);
    if (it != mDisabledSysPackages.End()) {
        p = it->mSecond;
    }
    if(p == NULL) {
        Slogger::W(CPackageManagerService::TAG, "Package:%s  is not disabled", name.string());
        return NULL;
    }
    // Reset flag in ApplicationInfo object
    if((p->mPkg != NULL) && (p->mPkg->mApplicationInfo != NULL)) {
        Int32 flags;
        p->mPkg->mApplicationInfo->GetFlags(&flags);
        flags &= ~IApplicationInfo::FLAG_UPDATED_SYSTEM_APP;
        p->mPkg->mApplicationInfo->SetFlags(flags);
    }
    AutoPtr<PackageSetting> ret = AddPackageLPw(name, p->mRealName, p->mCodePath, p->mResourcePath,
            p->mNativeLibraryPathString, p->mAppId, p->mVersionCode, p->mPkgFlags);
    mDisabledSysPackages.Erase(name);
    return ret;
}

Boolean Settings::IsDisabledSystemPackageLPr(
    /* [in] */ const String& name)
{
    return mDisabledSysPackages.Find(name) != mDisabledSysPackages.End();
}

void Settings::RemoveDisabledSystemPackageLPw(
    /* [in] */ const String& name)
{
    mDisabledSysPackages.Erase(name);
}

AutoPtr<PackageSetting> Settings::AddPackageLPw(
    /* [in] */ const String& name,
    /* [in] */ const String& realName,
    /* [in] */ IFile* codePath,
    /* [in] */ IFile* resourcePath,
    /* [in] */ const String& nativeLibraryPathString,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 vc,
    /* [in] */ Int32 pkgFlags)
{
    AutoPtr<PackageSetting> p;
    HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mPackages.Find(name);
    if (it != mPackages.End()) {
        p = it->mSecond;
    }
    if (p != NULL) {
        if (p->mAppId == uid) {
            return p;
        }
        CPackageManagerService::ReportSettingsProblem(Logger::ERROR,
               String("Adding duplicate package, keeping first: ") + name);
        return NULL;
    }
    p = new PackageSetting(name, realName, codePath, resourcePath, nativeLibraryPathString,
            vc, pkgFlags);
    p->mAppId = uid;
    AutoPtr<ICharSequence> csName;
    CStringWrapper::New(name, (ICharSequence**)&csName);
    if (AddUserIdLPw(uid, p->Probe(EIID_IInterface), csName)) {
        mPackages[name] = p;
        return p;
    }
    return NULL;
}

AutoPtr<SharedUserSetting> Settings::AddSharedUserLPw(
    /* [in] */ const String& name,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 pkgFlags)
{
    AutoPtr<SharedUserSetting> s;
    HashMap<String, AutoPtr<SharedUserSetting> >::Iterator it = mSharedUsers.Find(name);
    if (it != mSharedUsers.End()) {
        s = it->mSecond;
    }
    if (s != NULL) {
        if (s->mUserId == uid) {
            return s;
        }
        CPackageManagerService::ReportSettingsProblem(Logger::ERROR,
               String("Adding duplicate shared user, keeping first: ") + name);
        return NULL;
    }
    s = new SharedUserSetting(name, pkgFlags);
    s->mUserId = uid;
    AutoPtr<ICharSequence> csName;
    CStringWrapper::New(name, (ICharSequence**)&csName);
    if (AddUserIdLPw(uid, s->Probe(EIID_IInterface), csName)) {
        mSharedUsers[name] = s;
        return s;
    }
    return NULL;
}

void Settings::TransferPermissionsLPw(
    /* [in] */ const String& origPkg,
    /* [in] */ const String& newPkg)
{
    // Transfer ownership of permissions to the new package.
    for (Int32 i = 0; i < 2; i++) {
        HashMap<String, AutoPtr<BasePermission> >& permissions =
                i == 0 ? mPermissionTrees : mPermissions;
        HashMap<String, AutoPtr<BasePermission> >::Iterator it;
        for (it = permissions.Begin(); it != permissions.End(); ++it) {
            AutoPtr<BasePermission> bp = it->mSecond;
            if (origPkg.Equals(bp->mSourcePackage)) {
                if (CPackageManagerService::DEBUG_UPGRADE) Slogger::V(CPackageManagerService::TAG,
                       "Moving permission %s  from pkg %s  to ", bp->mName.string(), bp->mSourcePackage.string(), newPkg.string());
                bp->mSourcePackage = newPkg;
                bp->mPackageSetting = NULL;
                bp->mPerm = NULL;
                if (bp->mPendingInfo != NULL) {
                    bp->mPendingInfo->SetPackageName(newPkg);
                }
                bp->mUid = 0;
                bp->mGids = NULL;
            }
        }
    }
}

AutoPtr<PackageSetting> Settings::GetPackageLPw(
    /* [in] */ const String& name_,
    /* [in] */ PackageSetting* origPackage,
    /* [in] */ const String& realName,
    /* [in] */ SharedUserSetting* sharedUser,
    /* [in] */ IFile* codePath,
    /* [in] */ IFile* resourcePath,
    /* [in] */ const String& nativeLibraryPathString,
    /* [in] */ Int32 vc,
    /* [in] */ Int32 pkgFlags,
    /* [in] */ IUserHandle* installUser,
    /* [in] */ Boolean add,
    /* [in] */ Boolean allowInstall)
{
    String name(name_);
    AutoPtr<PackageSetting> p;
    HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mPackages.Find(name);
    if (it != mPackages.End()) {
        p = it->mSecond;
    }
    if (p != NULL) {
        Boolean isEqual;
        if (p->mCodePath->Equals(codePath, &isEqual), !isEqual) {
            String codePathStr;
            codePath->ToString(&codePathStr);
            // Check to see if its a disabled system app
            if ((p->mPkgFlags & IApplicationInfo::FLAG_SYSTEM) != 0) {
                // This is an updated system app with versions in both system
                // and data partition. Just let the most recent version
                // take precedence.
                Slogger::W(CPackageManagerService::TAG, "Trying to update system app code path from %s to %s"
                       , p->mCodePathString.string(), codePathStr.string());
            }
            else {
                // Just a change in the code path is not an issue, but
                // let's log a message about it.
                Slogger::I(CPackageManagerService::TAG, "Package %s  codePath changed from %s to %s; Retaining data and using new", name.string()
                       , p->mCodePathString.string(), codePathStr.string());
                /*
                 * Since we've changed paths, we need to prefer the new
                 * native library path over the one stored in the
                 * package settings since we might have moved from
                 * internal to external storage or vice versa.
                 */
                p->mNativeLibraryPathString = nativeLibraryPathString;
            }
        }
        if (p->mSharedUser.Get() != sharedUser) {
            StringBuilder sb;
            sb += "Package ";
            sb += name;
            sb += " shared user changed from ";
            sb += (p->mSharedUser != NULL ? p->mSharedUser->mName : String("<nothing>"));
            sb += " to ";
            sb += (sharedUser != NULL ? sharedUser->mName : String("<nothing>"));
            sb += "; replacing with new";
            String msg;
            CPackageManagerService::ReportSettingsProblem(Logger::WARN, (sb.ToString(&msg), msg));
            p = NULL;
        }
        else {
            if ((pkgFlags & IApplicationInfo::FLAG_SYSTEM) != 0) {
                // If what we are scanning is a system package, then
                // make it so, regardless of whether it was previously
                // installed only in the data partition.
                p->mPkgFlags |= IApplicationInfo::FLAG_SYSTEM;
            }
        }
    }
    if (p == NULL) {
        if (origPackage != NULL) {
            // We are consuming the data from an existing package.
            p = new PackageSetting(origPackage->mName, name, codePath, resourcePath,
                    nativeLibraryPathString, vc, pkgFlags);
//            if (PackageManagerService.DEBUG_UPGRADE) Log.v(PackageManagerService.TAG, "Package "
//                    + name + " is adopting original package " + origPackage.name);
            // Note that we will retain the new package's signature so
            // that we can keep its data.
            AutoPtr<PackageSignatures> s = p->mSignatures;
            p->CopyFrom(origPackage);
            p->mSignatures = s;
            p->mSharedUser = origPackage->mSharedUser;
            p->mAppId = origPackage->mAppId;
            p->mOrigPackage = origPackage;
            mRenamedPackages[name] = origPackage->mName;
            name = origPackage->mName;
            // Update new package state.
            Int64 time;
            codePath->LastModified(&time);
            p->SetTimeStamp(time);
        }
        else {
            p = new PackageSetting(name, realName, codePath, resourcePath,
                    nativeLibraryPathString, vc, pkgFlags);
            Int64 time;
            codePath->LastModified(&time);
            p->SetTimeStamp(time);
            p->mSharedUser = sharedUser;
            // If this is not a system app, it starts out stopped.
            if ((pkgFlags & IApplicationInfo::FLAG_SYSTEM) == 0) {
                if (DEBUG_STOPPED) {
//                    RuntimeException e = new RuntimeException("here");
//                    e.fillInStackTrace();
//                    Slog.i(PackageManagerService.TAG, "Stopping package " + name, e);
                }
                AutoPtr< List< AutoPtr<IUserInfo> > > users = GetAllUsers();
                if (users != NULL && allowInstall) {
                    List< AutoPtr<IUserInfo> >::Iterator it;
                    for (it = users->Begin(); it != users->End(); ++it) {
                        AutoPtr<IUserInfo> user = *it;
                        assert(user != NULL);
                        // By default we consider this app to be installed
                        // for the user if no user has been specified (which
                        // means to leave it at its original value, and the
                        // original default value is true), or we are being
                        // asked to install for all users, or this is the
                        // user we are installing for.
                        Int32 id, userId;
                        user->GetId(&userId);
                        Boolean installed = installUser == NULL
                                || (installUser->GetIdentifier(&id), id == IUserHandle::USER_ALL)
                                || id == userId;
                        p->SetUserState(userId, IPackageManager::COMPONENT_ENABLED_STATE_DEFAULT,
                                installed,
                                TRUE, // stopped,
                                TRUE, // notLaunched
                                NULL, NULL);
                        WritePackageRestrictionsLPr(userId);
                    }
                }
            }
            if (sharedUser != NULL) {
                p->mAppId = sharedUser->mUserId;
            }
            else {
                // Clone the setting here for disabled system packages
                AutoPtr<PackageSetting> dis;
                HashMap<String, AutoPtr<PackageSetting> >::Iterator disIt = mDisabledSysPackages.Find(name);
                if (disIt != mDisabledSysPackages.End()) {
                    dis = disIt->mSecond;
                }
                if (dis != NULL) {
                    // For disabled packages a new setting is created
                    // from the existing user id. This still has to be
                    // added to list of user id's
                    // Copy signatures from previous setting
                    if (dis->mSignatures->mSignatures != NULL) {
                        p->mSignatures->mSignatures = dis->mSignatures->mSignatures->Clone();
                    }
                    p->mAppId = dis->mAppId;
                    // Clone permissions
                    p->mGrantedPermissions.Clear();
                    p->mGrantedPermissions.Insert(dis->mGrantedPermissions.Begin(), dis->mGrantedPermissions.End());
                    // Clone component info
                    AutoPtr< List< AutoPtr<IUserInfo> > > users = GetAllUsers();
                    if (users != NULL) {
                        List< AutoPtr<IUserInfo> >::Iterator it;
                        for (it = users->Begin(); it != users->End(); ++it) {
                            Int32 userId;
                            (*it)->GetId(&userId);
                            p->SetDisabledComponentsCopy(
                                    dis->GetDisabledComponents(userId), userId);
                            p->SetEnabledComponentsCopy(
                                    dis->GetEnabledComponents(userId), userId);
                        }
                    }
                    // Add new setting to list of user ids
                    AutoPtr<ICharSequence> csName;
                    CStringWrapper::New(name, (ICharSequence**)&csName);
                    AddUserIdLPw(p->mAppId, p->Probe(EIID_IInterface), csName);
                }
                else {
                    // Assign new user id
                    p->mAppId = NewUserIdLPw(p);
                }
            }
        }
        if (p->mAppId < 0) {
            CPackageManagerService::ReportSettingsProblem(Logger::WARN,
                   String("Package ") + name + String(" could not be assigned a valid uid"));
            return NULL;
        }
        if (add) {
            // Finish adding new package by adding it and updating shared
            // user preferences
            AddPackageSettingLPw(p, name, sharedUser);
        }
    }
    else {
        if (installUser != NULL && allowInstall) {
            // The caller has explicitly specified the user they want this
            // package installed for, and the package already exists.
            // Make sure it conforms to the new request.
            AutoPtr< List< AutoPtr<IUserInfo> > > users = GetAllUsers();
            if (users != NULL) {
                Int32 id;
                installUser->GetIdentifier(&id);
                List< AutoPtr<IUserInfo> >::Iterator it;
                for (it = users->Begin(); it != users->End(); ++it) {
                    Int32 userId;
                    (*it)->GetId(&userId);
                    if (id == IUserHandle::USER_ALL
                            || id == userId) {
                        Boolean installed = p->GetInstalled(userId);
                        if (!installed) {
                            p->SetInstalled(TRUE, userId);
                            WritePackageRestrictionsLPr(userId);
                        }
                    }
                }
            }
        }
    }
    return p;
}

void Settings::InsertPackageSettingLPw(
    /* [in] */ PackageSetting* p,
    /* [in] */ PackageParser::Package* pkg)
{
    p->mPkg = pkg;
    String codePath, resourcePath;
    pkg->mApplicationInfo->GetSourceDir(&codePath);
    pkg->mApplicationInfo->GetPublicSourceDir(&resourcePath);
    // Update code path if needed
    if (!codePath.EqualsIgnoreCase(p->mCodePathString)) {
        Slogger::W(CPackageManagerService::TAG, "Code path for pkg : %s  changing from %s  to ", p->mPkg->mPackageName.string(),
               p->mCodePathString.string(), codePath.string());
        p->mCodePath = NULL;
        CFile::New(codePath, (IFile**)&p->mCodePath);
        p->mCodePathString = codePath;
    }
    //Update resource path if needed
    if (!resourcePath.EqualsIgnoreCase(p->mResourcePathString)) {
        Slogger::W(CPackageManagerService::TAG, "Resource path for pkg : %s  changing from %s  to %s", p->mPkg->mPackageName.string(),
               p->mResourcePathString.string(), resourcePath.string());
        p->mResourcePath = NULL;
        CFile::New(resourcePath, (IFile**)&p->mResourcePath);
        p->mResourcePathString = resourcePath;
    }
    // Update the native library path if needed
    String nativeLibraryPath;
    pkg->mApplicationInfo->GetNativeLibraryDir(&nativeLibraryPath);
    if (!nativeLibraryPath.IsNull()
            && !nativeLibraryPath.EqualsIgnoreCase(p->mNativeLibraryPathString)) {
        p->mNativeLibraryPathString = nativeLibraryPath;
    }
    // Update version code if needed
    if (pkg->mVersionCode != p->mVersionCode) {
        p->mVersionCode = pkg->mVersionCode;
    }
    // Update signatures if needed.
    if (p->mSignatures->mSignatures == NULL) {
        p->mSignatures->AssignSignatures(pkg->mSignatures);
    }
    // Update flags if needed.
    Int32 flags;
    pkg->mApplicationInfo->GetFlags(&flags);
    if (flags != p->mPkgFlags) {
        p->mPkgFlags = flags;
    }
    // If this app defines a shared user id initialize
    // the shared user signatures as well.
    if (p->mSharedUser != NULL && p->mSharedUser->mSignatures->mSignatures == NULL) {
        p->mSharedUser->mSignatures->AssignSignatures(pkg->mSignatures);
    }
    AddPackageSettingLPw(p, pkg->mPackageName, p->mSharedUser);
}

void Settings::AddPackageSettingLPw(
    /* [in] */ PackageSetting* p,
    /* [in] */ const String& name,
    /* [in] */ SharedUserSetting* sharedUser)
{
    mPackages[name] = p;
    if (sharedUser != NULL) {
        StringBuilder sb;
        String msg;
        if (p->mSharedUser != NULL && p->mSharedUser.Get() != sharedUser) {
            sb += "Package ";
            sb += p->mName;
            sb += " was user ";
            sb += "p->mSharedUser";
            sb += " but is now ";
            sb += "sharedUser";
            sb += "; I am not changing its files so it will probably fail!";
            CPackageManagerService::ReportSettingsProblem(Logger::ERROR, (sb.ToString(&msg), msg));
            p->mSharedUser->mPackages.Erase(p);
        }
        else if (p->mAppId != sharedUser->mUserId) {
            sb += "Package ";
            sb += p->mName;
            sb += " was user id ";
            sb += p->mAppId;
            sb += " but is now user ";
            sb += "sharedUser";
            sb += " with id ";
            sb += sharedUser->mUserId;
            sb += "; I am not changing its files so it will probably fail!";
            CPackageManagerService::ReportSettingsProblem(Logger::ERROR, (sb.ToString(&msg), msg));
        }

        sharedUser->mPackages.Insert(p);
        p->mSharedUser = sharedUser;
        p->mAppId = sharedUser->mUserId;
    }
}

void Settings::UpdateSharedUserPermsLPw(
    /* [in] */ PackageSetting* deletedPs,
    /* [in] */ ArrayOf<Int32>* globalGids)
{
    if ((deletedPs == NULL) || (deletedPs->mPkg == NULL)) {
        Slogger::I(CPackageManagerService::TAG,
               "Trying to update info for null package. Just ignoring");
        return;
    }
    // No sharedUserId
    if (deletedPs->mSharedUser == NULL) {
        return;
    }
    AutoPtr<SharedUserSetting> sus = deletedPs->mSharedUser;
    // Update permissions
    List<String>::Iterator it;
    for (it = deletedPs->mPkg->mRequestedPermissions.Begin();
            it != deletedPs->mPkg->mRequestedPermissions.End(); ++it) {
        String eachPerm = *it;
        Boolean used = FALSE;
        if (sus->mGrantedPermissions.Find(eachPerm) == sus->mGrantedPermissions.End()) {
            continue;
        }
        HashSet<PackageSetting*>::Iterator pit;
        for (pit = sus->mPackages.Begin(); pit != sus->mPackages.End(); ++pit) {
            PackageSetting* pkg = *pit;
            if (pkg->mPkg != NULL &&
                    !pkg->mPkg->mPackageName.Equals(deletedPs->mPkg->mPackageName) &&
                    (Find(pkg->mPkg->mRequestedPermissions.Begin(),
                        pkg->mPkg->mRequestedPermissions.End(), eachPerm) !=
                        pkg->mPkg->mRequestedPermissions.End())) {
                used = TRUE;
                break;
            }
        }
        if (!used) {
            // can safely delete this permission from list
            sus->mGrantedPermissions.Erase(eachPerm);
        }
    }
    // Update gids
    AutoPtr< ArrayOf<Int32> > newGids = globalGids;
    HashSet<String>::Iterator sit;
    for (sit = sus->mGrantedPermissions.Begin(); sit != sus->mGrantedPermissions.End(); ++sit) {
        String eachPerm = *sit;
        AutoPtr<BasePermission> bp;
        HashMap<String, AutoPtr<BasePermission> >::Iterator bit = mPermissions.Find(eachPerm);
        if (bit != mPermissions.End()) {
            bp = bit->mSecond;
        }
        if (bp != NULL) {
            newGids = CPackageManagerService::AppendInts(newGids, bp->mGids);
        }
    }
    sus->mGids = newGids;
}

Int32 Settings::RemovePackageLPw(
    /* [in] */ const String& name)
{
    AutoPtr<PackageSetting> p;
    HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mPackages.Find(name);
    if (it != mPackages.End()) {
        p = it->mSecond;
    }
    if (p != NULL) {
        mPackages.Erase(it);
        if (p->mSharedUser != NULL) {
            p->mSharedUser->mPackages.Erase(p);
            if (p->mSharedUser->mPackages.Begin() == p->mSharedUser->mPackages.End()) {
                mSharedUsers.Erase(p->mSharedUser->mName);
                RemoveUserIdLPw(p->mSharedUser->mUserId);
                p->mPkg = NULL;
                return p->mSharedUser->mUserId;
            }
        }
        else {
            RemoveUserIdLPw(p->mAppId);
            p->mPkg = NULL;
            return p->mAppId;
        }
    }
    return -1;
}

void Settings::ReplacePackageLPw(
    /* [in] */ const String& name,
    /* [in] */ PackageSetting* newp)
{
    AutoPtr<PackageSetting> p;
    HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mPackages.Find(name);
    if (it != mPackages.End()) {
        p = it->mSecond;
    }
    if (p != NULL) {
        if (p->mSharedUser != NULL) {
            p->mSharedUser->mPackages.Erase(p);
            p->mSharedUser->mPackages.Insert(newp);
        }
        else {
            ReplaceUserIdLPw(p->mAppId, newp);
        }
    }
    mPackages[name] = newp;
}

Boolean Settings::AddUserIdLPw(
    /* [in] */ Int32 uid,
    /* [in] */ IInterface* obj,
    /* [in] */ IInterface* name)
{
    if (uid > IProcess::LAST_APPLICATION_UID) {
        return FALSE;
    }

    if (uid >= IProcess::FIRST_APPLICATION_UID) {
        Int32 N = mUserIds.GetSize();
        Int32 index = uid - IProcess::FIRST_APPLICATION_UID;
        while (index >= N) {
            mUserIds.PushBack(NULL);
            N++;
        }
        if (mUserIds[index] != NULL) {
            StringBuilder sb;
            sb += "Adding duplicate user id: ";
            sb += uid;
            sb += " name=";
            sb += "name";
            String msg;
            CPackageManagerService::ReportSettingsProblem(Logger::ERROR, (sb.ToString(&msg), msg));
            return FALSE;
        }
        mUserIds[index] = obj;
    }
    else {
        if (mOtherUserIds[uid] != NULL) {
            StringBuilder sb;
            sb += "Adding duplicate shared id: ";
            sb += uid;
            sb += " name=";
            sb += "name";
            String msg;
            CPackageManagerService::ReportSettingsProblem(Logger::ERROR, (sb.ToString(&msg), msg));
            return FALSE;
        }
        mOtherUserIds[uid] = obj;
    }
    return TRUE;
}

AutoPtr<IInterface> Settings::GetUserIdLPr(
    /* [in] */ Int32 uid)
{
    if (uid >= IProcess::FIRST_APPLICATION_UID) {
        Int32 N = mUserIds.GetSize();
        Int32 index = uid - IProcess::FIRST_APPLICATION_UID;
        return index < N ? mUserIds[index] : NULL;
    }
    else {
        AutoPtr<IInterface> obj;
        HashMap<Int32, AutoPtr<IInterface> >::Iterator it = mOtherUserIds.Find(uid);
        if (it != mOtherUserIds.End()) {
            obj = it->mSecond;
        }
        return obj;
    }
}

void Settings::RemoveUserIdLPw(
    /* [in] */ Int32 uid)
{
    if (uid >= IProcess::FIRST_APPLICATION_UID) {
        Int32 N = mUserIds.GetSize();
        Int32 index = uid - IProcess::FIRST_APPLICATION_UID;
        if (index < N) mUserIds[index] = NULL;
    }
    else {
        mOtherUserIds.Erase(uid);
    }
}

void Settings::ReplaceUserIdLPw(
    /* [in] */ Int32 uid,
    /* [in] */ IInterface* obj)
{
    if (uid >= IProcess::FIRST_APPLICATION_UID) {
        Int32 N = mUserIds.GetSize();
        Int32 index = uid - IProcess::FIRST_APPLICATION_UID;
        if (index < N) mUserIds[index] = obj;
    }
    else {
        mOtherUserIds[uid] = obj;
    }
}

AutoPtr<PreferredIntentResolver> Settings::EditPreferredActivitiesLPw(
    /* [in] */ Int32 userId)
{
    AutoPtr<PreferredIntentResolver> pir = mPreferredActivities[userId];
    if (pir == NULL) {
        pir = new PreferredIntentResolver();
        mPreferredActivities[userId] = pir;
    }
    return pir;
}

AutoPtr<IFile> Settings::GetUserPackagesStateFile(
    /* [in] */ Int32 userId)
{
    AutoPtr<IFile> stateFile;
    AutoPtr<IEnvironment> env;
    CEnvironment::AcquireSingleton((IEnvironment**)&env);
    AutoPtr<IFile> dir;
    env->GetUserSystemDirectory(userId, (IFile**)&dir);
    CFile::New(dir, String("package-restrictions.xml"), (IFile**)&stateFile);
    return stateFile;
}

AutoPtr<IFile> Settings::GetUserPackagesStateBackupFile(
    /* [in] */ Int32 userId)
{
    AutoPtr<IFile> backupFile;
    AutoPtr<IEnvironment> env;
    CEnvironment::AcquireSingleton((IEnvironment**)&env);
    AutoPtr<IFile> dir;
    env->GetUserSystemDirectory(userId, (IFile**)&dir);
    CFile::New(dir, String("package-restrictions-backup.xml"), (IFile**)&backupFile);
    return backupFile;
}

void Settings::WriteAllUsersPackageRestrictionsLPr()
{
    AutoPtr< List< AutoPtr<IUserInfo> > > users = GetAllUsers();
    if (users == NULL) return;

    List< AutoPtr<IUserInfo> >::Iterator it;
    for (it = users->Begin(); it != users->End(); ++it) {
        Int32 userId;
        (*it)->GetId(&userId);
        WritePackageRestrictionsLPr(userId);
    }
}

void Settings::ReadAllUsersPackageRestrictionsLPr()
{
    AutoPtr< List< AutoPtr<IUserInfo> > > users = GetAllUsers();
    if (users == NULL) {
        ReadPackageRestrictionsLPr(0);
        return;
    }

    List< AutoPtr<IUserInfo> >::Iterator it;
    for (it = users->Begin(); it != users->End(); ++it) {
        Int32 userId;
        (*it)->GetId(&userId);
        ReadPackageRestrictionsLPr(userId);
    }
}

ECode Settings::ReadPreferredActivitiesLPw(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ Int32 userId)
{
    Int32 outerDepth, type, depth;
    parser->GetDepth(&outerDepth);
    while (((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT)
            && (type != IXmlPullParser::END_TAG || (parser->GetDepth(&depth), depth) > outerDepth)) {
        if (type == IXmlPullParser::END_TAG || type == IXmlPullParser::TEXT) {
            continue;
        }

        if (type != IXmlPullParser::START_TAG) {
            continue;
        }

        String tagName;
        parser->GetName(&tagName);
        if (tagName.Equals(TAG_ITEM)) {
            AutoPtr<PreferredActivity> pa = new PreferredActivity(parser);
            if (pa->mPref->GetParseError().IsNull()) {
                EditPreferredActivitiesLPw(userId)->AddFilter(pa);
            }
            else {
                String parserPositionDesc;
                parser->GetPositionDescription(&parserPositionDesc);
                CPackageManagerService::ReportSettingsProblem(Logger::WARN,
                       String("Error in package manager settings: <preferred-activity> ")
                               + pa->mPref->GetParseError() + String(" at ")
                               + parserPositionDesc);
            }
        }
        else {
            String nameStr;
            CPackageManagerService::ReportSettingsProblem(Logger::WARN,
                   String("Unknown element under <preferred-activities>: ") + (parser->GetName(&nameStr), nameStr));
            XmlUtils::SkipCurrentTag(parser);
        }
    }
    return NOERROR;
}

void Settings::ReadPackageRestrictionsLPr(
    /* [in] */ Int32 userId)
{
    if (DEBUG_MU) {
        Slogger::I(TAG, "Reading package restrictions for user= %d", userId);
    }
    AutoPtr<IFileInputStream> str;
    AutoPtr<IFile> userPackagesStateFile = GetUserPackagesStateFile(userId);
    AutoPtr<IFile> backupFile = GetUserPackagesStateBackupFile(userId);
    Boolean result;
    String temp;
    ECode ecode = NOERROR;
    if (backupFile->Exists(&result), result) {
        //try {
        CFileInputStream::New(backupFile, (IFileInputStream**)&str);
        mReadMessages->AppendString(String("Reading from backup stopped packages file\n"));
        CPackageManagerService::ReportSettingsProblem(Logger::INFO,
                String("Need to read from backup stopped packages file"));
        if (userPackagesStateFile->Exists(&result), result) {
            // If both the backup and normal file exist, we
            // ignore the normal one since it might have been
            // corrupted.
            Slogger::W(CPackageManagerService::TAG, String("Cleaning up stopped packages file ")
                    + (userPackagesStateFile->ToString(&temp), temp));
            userPackagesStateFile->Delete(&result);
        }
        // } catch (java.io.IOException e) {
        //     // We'll try for the normal settings file.
        // }
    }

    //try
    {
        if (str == NULL) {
            if (userPackagesStateFile->Exists(&result), !result) {
                mReadMessages->AppendString(String("No stopped packages file found\n"));
                CPackageManagerService::ReportSettingsProblem(Logger::INFO,
                         String("No stopped packages file; assuming all started"));
                // At first boot, make sure no packages are stopped.
                // We usually want to have third party apps initialize
                // in the stopped state, but not at first boot.  Also
                // consider all applications to be installed.
                HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mPackages.Begin();
                for (; it != mPackages.End(); it++) {
                    AutoPtr<PackageSetting> pkg = it->mSecond;
                    pkg->SetUserState(userId, IPackageManager::COMPONENT_ENABLED_STATE_DEFAULT,
                            TRUE,   // installed
                            FALSE,  // stopped
                            FALSE,  // notLaunched
                            NULL, NULL);
                }
                return;
            }
            FAIL_GOTO(ecode = CFileInputStream::New(userPackagesStateFile, (IFileInputStream**)&str), EXIT);
        }
        AutoPtr<IXmlPullParser> parser = Xml::NewPullParser();
        FAIL_GOTO(ecode = parser->SetInput(str, String(NULL)), EXIT);

        Int32 type;
        while ((parser->Next(&type), type) != IXmlPullParser::START_TAG
                   && type != IXmlPullParser::END_DOCUMENT) {
            ;
        }

        if (type != IXmlPullParser::START_TAG) {
            mReadMessages->AppendString(String("No start tag found in package restrictions file\n"));
            CPackageManagerService::ReportSettingsProblem(Logger::WARN,
                    String("No start tag found in package manager stopped packages"));
            return;
        }

        Int32 outerDepth, outerDepth1;
        FAIL_GOTO(ecode = parser->GetDepth(&outerDepth), EXIT);
        AutoPtr<PackageSetting> ps;
        while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
               && (type != IXmlPullParser::END_TAG
                       || (parser->GetDepth(&outerDepth1), outerDepth1) > outerDepth)) {
            if (type == IXmlPullParser::END_TAG || type == IXmlPullParser::TEXT) {
                continue;
            }
            if (type != IXmlPullParser::START_TAG) {
                continue;
            }

            String tagName;
            FAIL_GOTO(ecode = parser->GetName(&tagName), EXIT);
            if (tagName.Equals(TAG_PACKAGE)) {
                String name;
                FAIL_GOTO(ecode = parser->GetAttributeValue(String(NULL), ATTR_NAME, &name), EXIT);
                HashMap<String, AutoPtr<PackageSetting> >::Iterator it2 = mPackages.Find(name);
                ps = (it2 != mPackages.End() ? it2->mSecond : NULL);
                if (ps == NULL) {
                    Slogger::W(CPackageManagerService::TAG, String("No package known for stopped package: ")
                            + name);
                    XmlUtils::SkipCurrentTag(parser);
                    continue;
                }
                String enabledStr;
                FAIL_GOTO(ecode = parser->GetAttributeValue(String(NULL), ATTR_ENABLED, &enabledStr), EXIT);
                Int32 enabled = enabledStr == NULL
                        ? IPackageManager::COMPONENT_ENABLED_STATE_DEFAULT : StringUtils::ParseInt32(enabledStr);
                String installedStr;
                FAIL_GOTO(ecode = parser->GetAttributeValue(String(NULL), ATTR_INSTALLED, &installedStr), EXIT);
                Boolean installed = installedStr == NULL
                        ? TRUE : StringUtils::ParseInt32(installedStr);
                String stoppedStr;
                FAIL_GOTO(ecode = parser->GetAttributeValue(String(NULL), ATTR_STOPPED, &stoppedStr), EXIT);
                Boolean stopped = stoppedStr == NULL
                        ? FALSE : StringUtils::ParseInt32(stoppedStr);
                String notLaunchedStr;
                FAIL_GOTO(ecode = parser->GetAttributeValue(String(NULL), ATTR_NOT_LAUNCHED, &notLaunchedStr), EXIT);
                Boolean notLaunched = stoppedStr == NULL
                        ? FALSE : StringUtils::ParseInt32(notLaunchedStr);

                AutoPtr<HashSet<String> > enabledComponents;
                AutoPtr<HashSet<String> > disabledComponents;

                Int32 packageDepth, packageDepth1;
                FAIL_GOTO(ecode = parser->GetDepth(&packageDepth), EXIT);
                while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
                        && (type != IXmlPullParser::END_TAG
                        || (parser->GetDepth(&packageDepth1), packageDepth1) > packageDepth)) {

                    if (type == IXmlPullParser::END_TAG || type == IXmlPullParser::TEXT) {
                        continue;
                    }

                    if (type != IXmlPullParser::START_TAG) {
                        continue;
                    }

                    FAIL_GOTO(ecode = parser->GetName(&tagName), EXIT);
                    if (tagName.Equals(TAG_ENABLED_COMPONENTS)) {
                        assert(enabledComponents == NULL);
                        enabledComponents = ReadComponentsLPr(parser);
                    }
                    else if (tagName.Equals(TAG_DISABLED_COMPONENTS)) {
                        assert(disabledComponents == NULL);
                        disabledComponents = ReadComponentsLPr(parser);
                    }
                }

                ps->SetUserState(userId, enabled, installed, stopped, notLaunched,
                        enabledComponents, disabledComponents);
            }
            else if (tagName.Equals("preferred-activities")) {
                ReadPreferredActivitiesLPw(parser, userId);
            }
            else {
                Slogger::W(CPackageManagerService::TAG, String("Unknown element under <stopped-packages>: ")
                      + (parser->GetName(&temp), temp.IsNull() ? String("NULL") : temp));
                XmlUtils::SkipCurrentTag(parser);
            }
        }

        FAIL_GOTO(ecode = str->Close(), EXIT);

    // } catch (XmlPullParserException e) {
    //     mReadMessages.append("Error reading: " + e.toString());
    //     CPackageManagerService::ReportSettingsProblem(Logger::ERROR,
    //             "Error reading stopped packages: " + e);
    //     Logger::wtf(CPackageManagerService::TAG, "Error reading package manager stopped packages", e);

//     } catch (java.io.IOException e) {
//         mReadMessages.append("Error reading: " + e.toString());
//         PackageManagerService.reportSettingsProblem(Log.ERROR, "Error reading settings: " + e);
//         Log.wtf(PackageManagerService.TAG, "Error reading package manager stopped packages", e);
//     } catch (Exception e) {
//         //add by aw lisidong@allwinnertech.com
//         //For some reason,Parse xml will fail for all other situations,remove some files so late will recreate them
//         mReadMessages.append("Error reading: " + e.toString());
//         PackageManagerService.reportSettingsProblem(Log.ERROR, "Error reading settings: " + e);
//         Log.wtf(PackageManagerService.TAG, "Error reading package manager stopped packages", e);
//         userPackagesStateFile.delete();
//         backupFile.delete();
//         throw new RuntimeException("Error readPackageRestrictionsLPr userId=" + userId + ", remove that bad file");
//     }
    }

EXIT:
    if (E_XML_PULL_PARSER_EXCEPTION == ecode) {
        mReadMessages->AppendString(String("Error reading:  + e.toString()"));
        CPackageManagerService::ReportSettingsProblem(Logger::ERROR,
                String("Error reading stopped packages: "));
        Slogger::E(CPackageManagerService::TAG, "Error reading package manager stopped packages, e");
    }
    else if (E_IO_EXCEPTION == ecode) {
        mReadMessages->AppendString(String("Error reading:  + e.toString()"));
        CPackageManagerService::ReportSettingsProblem(Logger::ERROR, String("Error reading settings:  + e"));
        Slogger::E(CPackageManagerService::TAG, "Error reading package manager stopped packages, e");
    }
    else if (NOERROR != ecode) {
        //add by aw lisidong@allwinnertech.com
        //For some reason,Parse xml will fail for all other situations,remove some files so late will recreate them
        mReadMessages->AppendString(String("Error reading:  + e.toString()"));
        CPackageManagerService::ReportSettingsProblem(Logger::ERROR, String("Error reading settings:  + e"));
//         Log.wtf(PackageManagerService.TAG, "Error reading package manager stopped packages", e);
        Boolean deleted;
        userPackagesStateFile->Delete(&deleted);
        backupFile->Delete(&deleted);
//         throw new RuntimeException("Error readPackageRestrictionsLPr userId=" + userId + ", remove that bad file");
    }
}

AutoPtr< HashSet<String> > Settings::ReadComponentsLPr(
    /* [in] */ IXmlPullParser* parser)
{
    AutoPtr< HashSet<String> > components;
    Int32 outerDepth, type, depth;
    parser->GetDepth(&outerDepth);
    String tagName;
    while (((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT)
            && (type != IXmlPullParser::END_TAG
            || (parser->GetDepth(&depth), depth) > outerDepth)) {
        if (type == IXmlPullParser::END_TAG
                || type == IXmlPullParser::TEXT) {
            continue;
        }
        if (type != IXmlPullParser::START_TAG) {
            continue;
        }
        parser->GetName(&tagName);
        if (tagName.Equals(TAG_ITEM)) {
            String componentName;
            parser->GetAttributeValue(String(NULL), ATTR_NAME, &componentName);
            if (!componentName.IsNull()) {
                if (components == NULL) {
                    components = new HashSet<String>();
                }
                components->Insert(componentName);
            }
        }
    }
    return components;
}

ECode Settings::WritePreferredActivitiesLPr(
    /* [in] */ IXmlSerializer* serializer,
    /* [in] */ Int32 userId)
{
    serializer->WriteStartTag(String(NULL), String("preferred-activities"));
    AutoPtr<PreferredIntentResolver> pir;
    HashMap<Int32, AutoPtr<PreferredIntentResolver> >::Iterator it = mPreferredActivities.Find(userId);
    if (it != mPreferredActivities.End()) {
        pir = it->mSecond;
    }
    if (pir != NULL) {
        AutoPtr< Set< AutoPtr<PreferredActivity> > > set = pir->FilterSet();
        Set< AutoPtr<PreferredActivity> >::Iterator sit;
        for (sit = set->Begin(); sit != set->End(); ++sit) {
            serializer->WriteStartTag(String(NULL), TAG_ITEM);
            (*sit)->WriteToXml(serializer);
            serializer->WriteEndTag(String(NULL), TAG_ITEM);
        }
    }
    serializer->WriteEndTag(String(NULL), String("preferred-activities"));
    return NOERROR;
}

void Settings::WritePackageRestrictionsLPr(
    /* [in] */ Int32 userId)
{
    if (DEBUG_MU) {
        Slogger::I(TAG, "Writing package restrictions for user= %d", userId);
    }
    // Keep the old stopped packages around until we know the new ones have
    // been successfully written.
    AutoPtr<IFile> userPackagesStateFile = GetUserPackagesStateFile(userId);
    AutoPtr<IFile> backupFile = GetUserPackagesStateBackupFile(userId);
    String temp;
    Boolean result;
    ECode ecode = NOERROR;
    userPackagesStateFile->GetParent(&temp);
    AutoPtr<IFile> stateFileParent;
    CFile::New(temp, (IFile**)&stateFileParent);
    stateFileParent->Mkdirs(&result);

    if (userPackagesStateFile->Exists(&result), result) {
        // Presence of backup settings file indicates that we failed
        // to persist packages earlier. So preserve the older
        // backup for future reference since the current packages
        // might have been corrupted.
        if (backupFile->Exists(&result), !result) {
            if (userPackagesStateFile->RenameTo(backupFile, &result), !result) {
                Slogger::E(CPackageManagerService::TAG, String("Unable to backup user packages state file, ")
                        + String("current changes will be lost at reboot"));
                return;
            }
        } else {
            userPackagesStateFile->Delete(&result);
            Slogger::W(CPackageManagerService::TAG, "Preserving older stopped packages backup");
        }
    }
    //try
    {
        AutoPtr<IFileOutputStream> fstr;
        FAIL_GOTO(ecode = CFileOutputStream::New(userPackagesStateFile, (IFileOutputStream**)&fstr), EXIT);
        AutoPtr<IBufferedOutputStream> str;
        FAIL_GOTO(ecode = CBufferedOutputStream::New(fstr, (IBufferedOutputStream**)&str), EXIT);

        AutoPtr<IFastXmlSerializer> serializer;
        CFastXmlSerializer::New((IFastXmlSerializer**)&serializer);
        serializer->SetOutput(str, String("utf-8"));
        AutoPtr<IBoolean> objBoolean;
        CBoolean::New(TRUE, (IBoolean**)&objBoolean);
        serializer->StartDocument(String(NULL), objBoolean);
        serializer->SetFeature(String("http://xmlpull.org/v1/doc/features.html#indent-output"), TRUE);

        serializer->WriteStartTag(String(NULL), TAG_PACKAGE_RESTRICTIONS);
        HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mPackages.Begin();
        for (; it  != mPackages.End(); it++) {
            AutoPtr<PackageSetting> pkg = it->mSecond;
            AutoPtr<PackageUserState> ustate = pkg->ReadUserState(userId);
            if (ustate->mStopped || ustate->mNotLaunched || !ustate->mInstalled
                    || ustate->mEnabled != IPackageManager::COMPONENT_ENABLED_STATE_DEFAULT
                    || (ustate->mEnabledComponents != NULL
                            && ustate->mEnabledComponents->IsEmpty() == FALSE)
                    || (ustate->mDisabledComponents != NULL
                            && ustate->mDisabledComponents->IsEmpty() == FALSE)) {
                serializer->WriteStartTag(String(NULL), TAG_PACKAGE);
                serializer->WriteAttribute(String(NULL), ATTR_NAME, pkg->mName);
                if (DEBUG_MU) Slogger::I(TAG, String("  pkg=") + pkg->mName + String(", state= %d"), ustate->mEnabled);

                if (!ustate->mInstalled) {
                    serializer->WriteAttribute(String(NULL), ATTR_INSTALLED, String("false"));
                }
                if (ustate->mStopped) {
                    serializer->WriteAttribute(String(NULL), ATTR_STOPPED, String("true"));
                }
                if (ustate->mNotLaunched) {
                    serializer->WriteAttribute(String(NULL), ATTR_NOT_LAUNCHED, String("true"));
                }
                if (ustate->mEnabled != IPackageManager::COMPONENT_ENABLED_STATE_DEFAULT) {
                    serializer->WriteAttribute(String(NULL), ATTR_ENABLED,
                            StringUtils::Int32ToString(ustate->mEnabled));
                }
                if (ustate->mEnabledComponents != NULL
                        && ustate->mEnabledComponents->IsEmpty() == FALSE) {
                    serializer->WriteStartTag(String(NULL), TAG_ENABLED_COMPONENTS);
                    HashSet<String>::Iterator it2 = ustate->mEnabledComponents->Begin();
                    for (; it2 != ustate->mEnabledComponents->End(); it2++) {
                        String name = (*it2);
                        serializer->WriteStartTag(String(NULL), TAG_ITEM);
                        serializer->WriteAttribute(String(NULL), ATTR_NAME, name);
                        serializer->WriteEndTag(String(NULL), TAG_ITEM);
                    }
                    serializer->WriteEndTag(String(NULL), TAG_ENABLED_COMPONENTS);
                }
                if (ustate->mDisabledComponents != NULL
                        && ustate->mDisabledComponents->IsEmpty() == FALSE) {
                    serializer->WriteStartTag(String(NULL), TAG_DISABLED_COMPONENTS);
                    HashSet<String>::Iterator it3 = ustate->mDisabledComponents->Begin();
                    for (; it3 != ustate->mDisabledComponents->End(); it3++) {
                        String name = (*it3);
                        serializer->WriteStartTag(String(NULL), TAG_ITEM);
                        serializer->WriteAttribute(String(NULL), ATTR_NAME, name);
                        serializer->WriteEndTag(String(NULL), TAG_ITEM);
                    }
                    serializer->WriteEndTag(String(NULL), TAG_DISABLED_COMPONENTS);
                }
                serializer->WriteEndTag(String(NULL), TAG_PACKAGE);
            }
        }

        WritePreferredActivitiesLPr(serializer, userId);

        serializer->WriteEndTag(String(NULL), TAG_PACKAGE_RESTRICTIONS);
        serializer->EndDocument();

        FAIL_GOTO(ecode = IFlushable::Probe(str)->Flush(), EXIT);
        FAIL_GOTO(ecode = FileUtils::Sync(fstr, &result), EXIT);
        FAIL_GOTO(ecode = str->Close(), EXIT);

        // New settings successfully written, old ones are no longer
        // needed.
        FAIL_GOTO(ecode = backupFile->Delete(&result), EXIT);
        FAIL_GOTO(ecode = FileUtils::SetPermissions((userPackagesStateFile->ToString(&temp), temp),
                FileUtils::sS_IRUSR|FileUtils::sS_IWUSR
                |FileUtils::sS_IRGRP|FileUtils::sS_IWGRP,
            -1, -1), EXIT);

        // Done, all is good!
        return;
    // } catch(java.io.IOException e) {
    //     Log.wtf(CPackageManagerService::TAG,
    //             "Unable to write package manager user packages state, "
    //             + " current changes will be lost at reboot", e);
    // }
    }

    EXIT:
        if (E_IO_EXCEPTION == ecode) {
            Slogger::E(CPackageManagerService::TAG,
                String("Unable to write package manager user packages state, ")
                + String(" current changes will be lost at reboot e"));
        }
        // Clean up partially written files
        if (userPackagesStateFile->Exists(&result), result) {
            if (userPackagesStateFile->Delete(&result), !result) {
                Slogger::I(CPackageManagerService::TAG, String("Failed to clean up mangled file: ")
                        + (mStoppedPackagesFilename->ToString(&temp), temp));
            }
        }
}

void Settings::ReadStoppedLPw()
{
    AutoPtr<IFileInputStream> str;
    Boolean result;
    String temp;
    ECode ecode = NOERROR;
    if (mBackupStoppedPackagesFilename->Exists(&result), result) {
        //try {
        CFileInputStream::New(mBackupStoppedPackagesFilename, (IFileInputStream**)&str);
        mReadMessages->AppendString(String("Reading from backup stopped packages file\n"));
        CPackageManagerService::ReportSettingsProblem(Logger::INFO,
                String("Need to read from backup stopped packages file"));
        if (mSettingsFilename->Exists(&result), result) {
            // If both the backup and normal file exist, we
            // ignore the normal one since it might have been
            // corrupted.
            Slogger::W(CPackageManagerService::TAG, String("Cleaning up stopped packages file ")
                    + (mStoppedPackagesFilename->ToString(&temp), temp));
            mStoppedPackagesFilename->Delete(&result);
        }
        // } catch (java.io.IOException e) {
        //     // We'll try for the normal settings file.
        // }
    }
    //try
    {
        if (str == NULL) {
            if (mStoppedPackagesFilename->Exists(&result), !result) {
                mReadMessages->AppendString(String("No stopped packages file found\n"));
                CPackageManagerService::ReportSettingsProblem(Logger::INFO,
                        String("No stopped packages file file; assuming all started"));
                // At first boot, make sure no packages are stopped.
                // We usually want to have third party apps initialize
                // in the stopped state, but not at first boot.
                HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mPackages.Begin();
                for (; it != mPackages.End(); it++) {
                    AutoPtr<PackageSetting> pkg = it->mSecond;
                    pkg->SetStopped(FALSE, 0);
                    pkg->SetNotLaunched(FALSE, 0);
                }
                return;
            }
            CFileInputStream::New(mStoppedPackagesFilename, (IFileInputStream**)&str);
        }
        AutoPtr<IXmlPullParser> parser = Xml::NewPullParser();
        parser->SetInput(str, String(NULL));

        Int32 type;
        while ((parser->Next(&type), type) != IXmlPullParser::START_TAG
                   && type != IXmlPullParser::END_DOCUMENT) {
            ;
        }

        if (type != IXmlPullParser::START_TAG) {
            mReadMessages->AppendString(String("No start tag found in stopped packages file\n"));
            CPackageManagerService::ReportSettingsProblem(Logger::WARN,
                    String("No start tag found in package manager stopped packages"));
            return;
        }

        Int32 outerDepth, outerDepth1;
        parser->GetDepth(&outerDepth);
        while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
               && (type != IXmlPullParser::END_TAG
                       || (parser->GetDepth(&outerDepth1), outerDepth1) > outerDepth)) {
            if (type == IXmlPullParser::END_TAG
                    || type == IXmlPullParser::TEXT) {
                continue;
            }

            if (type != IXmlPullParser::START_TAG) {
                continue;
            }

            String tagName;
            parser->GetName(&tagName);
            if (tagName.Equals(TAG_PACKAGE)) {
                String name;
                parser->GetAttributeValue(String(NULL), ATTR_NAME, &name);
                HashMap<String, AutoPtr<PackageSetting> >::Iterator it2 = mPackages.Find(name);
                AutoPtr<PackageSetting> ps = (it2 != mPackages.End() ? it2->mSecond : NULL);
                if (ps != NULL) {
                    ps->SetStopped(TRUE, 0);
                    parser->GetAttributeValue(String(NULL), ATTR_NOT_LAUNCHED, &temp);
                    if (CString("1").Equals(temp)) {
                        ps->SetNotLaunched(TRUE, 0);
                    }
                } else {
                    Slogger::W(CPackageManagerService::TAG,
                            String("No package known for stopped package: ") + name);
                }
                XmlUtils::SkipCurrentTag(parser);
            } else {
                Slogger::W(CPackageManagerService::TAG, String("Unknown element under <stopped-packages>: ")
                      + (parser->GetName(&temp), temp.IsNull() ? String("NULL") : temp));
                XmlUtils::SkipCurrentTag(parser);
            }
        }

        str->Close();

    // } catch (XmlPullParserException e) {
    //     mReadMessages->append("Error reading: " + e.toString());
    //     CPackageManagerService::ReportSettingsProblem(Logger::ERROR,
    //             "Error reading stopped packages: " + e);
    //     Log.wtf(CPackageManagerService::TAG, "Error reading package manager stopped packages", e);

    // } catch (java.io.IOException e) {
    //     mReadMessages->append("Error reading: " + e.toString());
    //     CPackageManagerService::ReportSettingsProblem(Log.ERROR, "Error reading settings: "ger::+ e);
    //     Log.wtf(CPackageManagerService::TAG, "Error reading package manager stopped packages", e);

    // }
    }
    EXIT:
        if (E_IO_EXCEPTION == ecode) {
            mReadMessages->AppendString(String("Error reading:  + e.toString()"));
            CPackageManagerService::ReportSettingsProblem(Logger::ERROR,
                    String("Error reading stopped packages:  + e"));
            Slogger::E(CPackageManagerService::TAG, "Error reading package manager stopped packages, e");
        }
        else if (E_XML_PULL_PARSER_EXCEPTION == ecode) {
            mReadMessages->AppendString(String("Error reading:  + e.toString()"));
            CPackageManagerService::ReportSettingsProblem(Logger::ERROR, String("Error reading settings: +e"));
            Slogger::E(CPackageManagerService::TAG, "Error reading package manager stopped packages, e");
        }
}

void Settings::WriteLPr()
{
    //Debug.startMethodTracing("/data/system/packageprof", 8 * 1024 * 1024);

    // Keep the old settings around until we know the new ones have
    // been successfully written.
    Boolean result;
    ECode ecode = NOERROR;
    String temp;
    Int32 val;
    if (mSettingsFilename->Exists(&result), result) {
        // Presence of backup settings file indicates that we failed
        // to persist settings earlier. So preserve the older
        // backup for future reference since the current settings
        // might have been corrupted.
        if (mBackupSettingsFilename->Exists(&result), !result) {
            if (mSettingsFilename->RenameTo(mBackupSettingsFilename, &result), !result) {
                // Log.wtf(CPackageManagerService::TAG, "Unable to backup package manager settings, "
                //         + " current changes will be lost at reboot");
                Slogger::E(CPackageManagerService::TAG, String("Unable to backup package manager settings, ")
                        + String(" current changes will be lost at reboot"));
                return;
            }
        } else {
            mSettingsFilename->Delete(&result);
            Slogger::W(CPackageManagerService::TAG, String("Preserving older settings backup"));
        }
    }

    mPastSignatures.Clear();

    //try
    {

        AutoPtr<IFileOutputStream> fstr;
        CFileOutputStream::New(mSettingsFilename, (IFileOutputStream**)&fstr);
        AutoPtr<IBufferedOutputStream> str;
        CBufferedOutputStream::New(fstr, (IBufferedOutputStream**)&str);

        //XmlSerializer serializer = XmlUtils.serializerInstance();
        AutoPtr<IFastXmlSerializer> serializer;
        CFastXmlSerializer::New((IFastXmlSerializer**)&serializer);
        FAIL_GOTO(ecode = serializer->SetOutput(str, String("utf-8")), EXIT);

        AutoPtr<IBoolean> ibObj;
        CBoolean::New(TRUE, (IBoolean**)&ibObj);
        FAIL_GOTO(ecode = serializer->StartDocument(String(NULL), ibObj), EXIT);
        FAIL_GOTO(ecode = serializer->SetFeature(String("http://xmlpull.org/v1/doc/features.html#indent-output"), TRUE), EXIT);

        FAIL_GOTO(ecode = serializer->WriteStartTag(String(NULL), String("packages")), EXIT);

        FAIL_GOTO(ecode = serializer->WriteStartTag(String(NULL), String("last-platform-version")), EXIT);
        FAIL_GOTO(ecode = serializer->WriteAttribute(String(NULL), String("internal"), StringUtils::Int32ToString(mInternalSdkPlatform)), EXIT);
        FAIL_GOTO(ecode = serializer->WriteAttribute(String(NULL), String("external"), StringUtils::Int32ToString(mExternalSdkPlatform)), EXIT);
        FAIL_GOTO(ecode = serializer->WriteEndTag(String(NULL), String("last-platform-version")), EXIT);

        if (mVerifierDeviceIdentity != NULL) {
            FAIL_GOTO(ecode = serializer->WriteStartTag(String(NULL), String("verifier")), EXIT);
            mVerifierDeviceIdentity->ToString(&temp);
            FAIL_GOTO(ecode = serializer->WriteAttribute(String(NULL), String("device"), temp), EXIT);
            FAIL_GOTO(ecode = serializer->WriteEndTag(String(NULL), String("verifier")), EXIT);
        }

        if (mReadExternalStorageEnforced != NULL) {
            Boolean value;
            mReadExternalStorageEnforced->GetValue(&value);
            FAIL_GOTO(ecode = serializer->WriteStartTag(String(NULL), TAG_READ_EXTERNAL_STORAGE), EXIT);
            FAIL_GOTO(ecode = serializer->WriteAttribute(
                    String(NULL), ATTR_ENFORCEMENT, value ? String("1") : String("0")), EXIT);
            FAIL_GOTO(ecode = serializer->WriteEndTag(String(NULL), TAG_READ_EXTERNAL_STORAGE), EXIT);
        }

        FAIL_GOTO(ecode = serializer->WriteStartTag(String(NULL), String("permission-trees")), EXIT);
        HashMap<String, AutoPtr<BasePermission> >::Iterator it = mPermissionTrees.Begin();
        for (; it != mPermissionTrees.End(); it++) {
            AutoPtr<BasePermission> bp = it->mSecond;
            WritePermissionLPr(serializer, bp);
        }

        FAIL_GOTO(ecode = serializer->WriteEndTag(String(NULL), String("permission-trees")), EXIT);

        FAIL_GOTO(ecode = serializer->WriteStartTag(String(NULL), String("permissions")), EXIT);

        HashMap<String, AutoPtr<BasePermission> >::Iterator it2 = mPermissions.Begin();
        for (; it2 != mPermissions.End(); it2++) {
            AutoPtr<BasePermission> bp = it2->mSecond;
            WritePermissionLPr(serializer, bp);
        }

        FAIL_GOTO(ecode = serializer->WriteEndTag(String(NULL), String("permissions")), EXIT);

        HashMap<String, AutoPtr<PackageSetting> >::Iterator it3 = mPackages.Begin();
        for (; it3 != mPackages.End(); it3++) {
            AutoPtr<PackageSetting> pkg = it3->mSecond;
            WritePackageLPr(serializer, pkg);
        }

        HashMap<String, AutoPtr<PackageSetting> >::Iterator it4 = mDisabledSysPackages.Begin();
        for (; it4 != mDisabledSysPackages.End(); it4++) {
            AutoPtr<PackageSetting> pkg = it4->mSecond;
            WriteDisabledSysPackageLPr(serializer, pkg);
        }

        HashMap<String, AutoPtr<SharedUserSetting> >::Iterator it5 = mSharedUsers.Begin();
        for (; it5 != mSharedUsers.End(); it5++) {
            AutoPtr<SharedUserSetting> user = it5->mSecond;
            FAIL_GOTO(ecode = serializer->WriteStartTag(String(NULL), String("shared-user")), EXIT);
            FAIL_GOTO(ecode = serializer->WriteAttribute(String(NULL), ATTR_NAME, user->mName), EXIT);
            FAIL_GOTO(ecode = serializer->WriteAttribute(String(NULL), String("userId"),
                    StringUtils::Int32ToString(user->mUserId)), EXIT);
            user->mSignatures->WriteXml(serializer, String("sigs"), mPastSignatures);
            FAIL_GOTO(ecode = serializer->WriteStartTag(String(NULL), String("perms")), EXIT);

            HashSet<String>::Iterator it6 = user->mGrantedPermissions.Begin();
            for (; it6 != user->mGrantedPermissions.End(); it6++) {
                String name = (*it6);
                FAIL_GOTO(ecode = serializer->WriteStartTag(String(NULL), TAG_ITEM), EXIT);
                FAIL_GOTO(ecode = serializer->WriteAttribute(String(NULL), ATTR_NAME, name), EXIT);
                FAIL_GOTO(ecode = serializer->WriteEndTag(String(NULL), TAG_ITEM), EXIT);
            }
            FAIL_GOTO(ecode = serializer->WriteEndTag(String(NULL), String("perms")), EXIT);
            FAIL_GOTO(ecode = serializer->WriteEndTag(String(NULL), String("shared-user")), EXIT);
        }

        if (mPackagesToBeCleaned.IsEmpty() == FALSE) {
            List< AutoPtr<IPackageCleanItem> >::Iterator it7 = mPackagesToBeCleaned.Begin();
            for (; it7 != mPackagesToBeCleaned.End(); it7++) {
                AutoPtr<IPackageCleanItem> item = it7->Get();
                String userStr = StringUtils::Int32ToString(item->GetUserId(&val), val);
                FAIL_GOTO(ecode = serializer->WriteStartTag(String(NULL), String("cleaning-package")), EXIT);
                FAIL_GOTO(ecode = serializer->WriteAttribute(String(NULL), ATTR_NAME, (item->GetPackageName(&temp), temp)), EXIT);
                FAIL_GOTO(ecode = serializer->WriteAttribute(String(NULL), ATTR_CODE, (item->GetAndCode(&result), result) ? String("true") : String("FALSE")), EXIT);
                FAIL_GOTO(ecode = serializer->WriteAttribute(String(NULL), ATTR_USER, userStr), EXIT);
                FAIL_GOTO(ecode = serializer->WriteEndTag(String(NULL), String("cleaning-package")), EXIT);
            }
        }

        if (mRenamedPackages.IsEmpty() == FALSE) {
            HashMap<String, String>::Iterator it8 = mRenamedPackages.Begin();
            for (; it8 != mRenamedPackages.End(); it8++) {
                FAIL_GOTO(ecode = serializer->WriteStartTag(String(NULL), String("renamed-package")), EXIT);
                FAIL_GOTO(ecode = serializer->WriteAttribute(String(NULL), String("new"), it8->mFirst), EXIT);
                FAIL_GOTO(ecode = serializer->WriteAttribute(String(NULL), String("old"), it8->mSecond), EXIT);
                FAIL_GOTO(ecode = serializer->WriteEndTag(String(NULL), String("renamed-package")), EXIT);
            }
        }

        FAIL_GOTO(ecode = serializer->WriteEndTag(String(NULL), String("packages")), EXIT);

        FAIL_GOTO(ecode = serializer->EndDocument(), EXIT);

        FAIL_GOTO(ecode = IFlushable::Probe(str)->Flush(), EXIT);
        FAIL_GOTO(ecode = FileUtils::Sync(fstr, &result), EXIT);
        FAIL_GOTO(ecode = str->Close(), EXIT);

        // New settings successfully written, old ones are no longer
        // needed.
        FAIL_GOTO(ecode = mBackupSettingsFilename->Delete(&result), EXIT);
        mSettingsFilename->ToString(&temp);
        FAIL_GOTO(ecode = FileUtils::SetPermissions(temp,
                FileUtils::sS_IRUSR|FileUtils::sS_IWUSR
                |FileUtils::sS_IRGRP|FileUtils::sS_IWGRP,
            -1, -1), EXIT);

        // Write package list file now, use a JournaledFile.
        //
        AutoPtr<IFile> tempFile;
        mPackageListFilename->ToString(&temp);
        FAIL_GOTO(ecode = CFile::New(temp + String(".tmp"), (IFile**)&tempFile), EXIT);
        AutoPtr<JournaledFile> journal = new JournaledFile(mPackageListFilename, tempFile);

        fstr = NULL;
        FAIL_GOTO(ecode = CFileOutputStream::New(journal->ChooseForWrite(), (IFileOutputStream**)&fstr), EXIT);
        str = NULL;
        FAIL_GOTO(ecode = CBufferedOutputStream::New(fstr, (IBufferedOutputStream**)&str), EXIT);
        //try
        ECode ec = NOERROR;
        do {
            StringBuilder sb;
            HashMap<String, AutoPtr<PackageSetting> >::Iterator it9 = mPackages.Begin();
            for (; it9 != mPackages.End(); it9++) {
                AutoPtr<PackageSetting> pkg = it9->mSecond;
                if (pkg->mPkg == NULL)
                    continue;

                AutoPtr<IApplicationInfo> ai = pkg->mPkg->mApplicationInfo;
                String dataPath;
                ai->GetDataDir(&dataPath);
                ai->GetFlags(&val);
                Boolean isDebug  = (val & IApplicationInfo::FLAG_DEBUGGABLE) != 0;

                // Avoid any application that has a space in its path
                // or that is handled by the system.
                ai->GetUid(&val);
                if (dataPath.IndexOf(" ") >= 0 || val < IProcess::FIRST_APPLICATION_UID)
                    continue;

                // we store on each line the following information for now:
                //
                // pkgName    - package name
                // userId     - application-specific user id
                // debugFlag  - 0 or 1 if the package is debuggable.
                // dataPath   - path to package's data path
                //
                // NOTE: We prefer not to expose all ApplicationInfo flags for now.
                //
                // DO NOT MODIFY THIS FORMAT UNLESS YOU CAN ALSO MODIFY ITS USERS
                // FROM NATIVE CODE. AT THE MOMENT, LOOK AT THE FOLLOWING SOURCES:
                //   system/core/run-as/run-as.c
                //
                //sb.setLength(0);

                sb.AppendString((ai->GetPackageName(&temp), temp));
                sb.AppendString(String(" "));
                ai->GetUid(&val);
                sb.AppendInt32((Int32)val);
                sb.AppendString(isDebug ? String(" 1 ") : String(" 0 "));
                dataPath.IsNull() ? sb.AppendString(String("NULL")) : sb.AppendString(dataPath);
                sb.AppendString(String("\n"));
                sb.ToString(&temp);
                AutoPtr<ArrayOf<Byte> > bytes = ArrayOf<Byte>::Alloc((Byte*)temp.string(), temp.GetByteLength());
                ec = str->WriteBytes(*bytes);
                if (FAILED(ec)) {
                    break;
                }
            }

            if (FAILED(ec)) break;
            ec = IFlushable::Probe(str)->Flush();
            if (FAILED(ec)) break;
            ec = FileUtils::Sync(fstr, &result);
            if (FAILED(ec)) break;
            ec = str->Close();
            if (FAILED(ec)) break;
            ec = journal->Commit();
            if (FAILED(ec)) break;
        // } catch (Exception e) {
        //     IoUtils.closeQuietly(str);
        //     journal.rollback();
        // }
        } while(0);

        if (FAILED(ec)) {
            AutoPtr<IIoUtils> ioUtils;
            ASSERT_SUCCEEDED(CIoUtils::AcquireSingleton((IIoUtils**)&ioUtils));
            ioUtils->CloseQuietly(str);
            journal->Rollback();
        }

        mPackageListFilename->ToString(&temp);
        ecode = FileUtils::SetPermissions(temp,
                FileUtils::sS_IRUSR|FileUtils::sS_IWUSR
                |FileUtils::sS_IRGRP|FileUtils::sS_IWGRP,
                -1, -1);

        WriteAllUsersPackageRestrictionsLPr();
        return;
    }
        // } catch(XmlPullParserException e) {
        //     Log.wtf(CPackageManagerService::TAG, "Unable to write package manager settings, "
        //             + "current changes will be lost at reboot", e);
        // } catch(java.io.IOException e) {
        //     Log.wtf(CPackageManagerService::TAG, "Unable to write package manager settings, "
        //             + "current changes will be lost at reboot", e);
        // }
        //Debug.stopMethodTracing();
        EXIT:
            if (E_XML_PULL_PARSER_EXCEPTION == ecode) {
                Slogger::E(CPackageManagerService::TAG, String("Unable to write package manager settings, ")
                            + String("current changes will be lost at reboot exception E_XML_PULL_PARSER_EXCEPTION"));
            }
            else if (E_IO_EXCEPTION == ecode) {
                    Slogger::E(CPackageManagerService::TAG, String("Unable to write package manager settings, ")
                            + String("current changes will be lost at reboot E_IO_EXCEPTION"));
            }

        // Clean up partially written files
        if (mSettingsFilename->Exists(&result), result) {
            if (mSettingsFilename->Delete(&result), !result) {
                Slogger::E(CPackageManagerService::TAG, String("Failed to clean up mangled file: ")
                        + (mSettingsFilename->ToString(&temp), temp));
            }
        }


}

ECode Settings::WriteDisabledSysPackageLPr(
    /* [in] */ IXmlSerializer* serializer,
    /* [in] */ PackageSetting* pkg)
{

    serializer->WriteStartTag(String(NULL), String("updated-package"));
    serializer->WriteAttribute(String(NULL), ATTR_NAME, pkg->mName);
    if (pkg->mRealName != NULL) {
        serializer->WriteAttribute(String(NULL), String("realName"), pkg->mRealName);
    }
    serializer->WriteAttribute(String(NULL), String("codePath"), pkg->mCodePathString);
    serializer->WriteAttribute(String(NULL), String("ft"), StringUtils::Int64ToHexString(pkg->mTimeStamp));
    serializer->WriteAttribute(String(NULL), String("it"), StringUtils::Int64ToHexString(pkg->mFirstInstallTime));
    serializer->WriteAttribute(String(NULL), String("ut"), StringUtils::Int64ToHexString(pkg->mLastUpdateTime));
    serializer->WriteAttribute(String(NULL), String("version"), StringUtils::Int32ToString(pkg->mVersionCode));
    if (!pkg->mResourcePathString.Equals(pkg->mCodePathString)) {
        serializer->WriteAttribute(String(NULL), String("resourcePath"), pkg->mResourcePathString);
    }
    if (pkg->mNativeLibraryPathString != NULL) {
        serializer->WriteAttribute(String(NULL), String("nativeLibraryPath"), pkg->mNativeLibraryPathString);
    }
    if (pkg->mSharedUser == NULL) {
        serializer->WriteAttribute(String(NULL), String("userId"), StringUtils::Int32ToString(pkg->mAppId));
    } else {
        serializer->WriteAttribute(String(NULL), String("sharedUserId"), StringUtils::Int32ToString(pkg->mAppId));
    }
    serializer->WriteStartTag(String(NULL), String("perms"));
    if (pkg->mSharedUser == NULL) {
        // If this is a shared user, the permissions will
        // be written there. We still need to write an
        // empty permissions list so permissionsFixed will
        // be set.
        HashSet<String>::Iterator it = pkg->mGrantedPermissions.Begin();
        for (; it != pkg->mGrantedPermissions.End(); it++) {
            String name = (*it);
            HashMap<String, AutoPtr<BasePermission> >::Iterator it2 = mPermissions.Find(name);
            AutoPtr<BasePermission> bp = it2->mSecond;
            if (bp != NULL) {
                // We only need to write signature or system permissions but
                // this wont
                // match the semantics of grantedPermissions. So write all
                // permissions.
                serializer->WriteStartTag(String(NULL), TAG_ITEM);
                serializer->WriteAttribute(String(NULL), ATTR_NAME, name);
                serializer->WriteEndTag(String(NULL), TAG_ITEM);
            }
        }
    }
    serializer->WriteEndTag(String(NULL), String("perms"));
    serializer->WriteEndTag(String(NULL), String("updated-package"));
    return NOERROR;
}

ECode Settings::WritePackageLPr(
    /* [in] */ IXmlSerializer* serializer,
    /* [in] */ PackageSetting* pkg)
{
    serializer->WriteStartTag(String(NULL), String("package"));
    serializer->WriteAttribute(String(NULL), ATTR_NAME, pkg->mName);
    if (!pkg->mRealName.IsNull()) {
        serializer->WriteAttribute(String(NULL), String("realName"), pkg->mRealName);
    }
    serializer->WriteAttribute(String(NULL), String("codePath"), pkg->mCodePathString);
    if (!pkg->mResourcePathString.Equals(pkg->mCodePathString)) {
        serializer->WriteAttribute(String(NULL), String("resourcePath"), pkg->mResourcePathString);
    }
    if (!pkg->mNativeLibraryPathString.IsNull()) {
        serializer->WriteAttribute(String(NULL), String("nativeLibraryPath"), pkg->mNativeLibraryPathString);
    }
    serializer->WriteAttribute(String(NULL), String("flags"), StringUtils::Int32ToString(pkg->mPkgFlags));
    serializer->WriteAttribute(String(NULL), String("ft"), StringUtils::Int64ToHexString(pkg->mTimeStamp));
    serializer->WriteAttribute(String(NULL), String("it"), StringUtils::Int64ToHexString(pkg->mFirstInstallTime));
    serializer->WriteAttribute(String(NULL), String("ut"), StringUtils::Int64ToHexString(pkg->mLastUpdateTime));
    serializer->WriteAttribute(String(NULL), String("version"), StringUtils::Int32ToString(pkg->mVersionCode));
    if (pkg->mSharedUser == NULL) {
        serializer->WriteAttribute(String(NULL), String("userId"), StringUtils::Int32ToString(pkg->mAppId));
    } else {
        serializer->WriteAttribute(String(NULL), String("sharedUserId"), StringUtils::Int32ToString(pkg->mAppId));
    }
    if (pkg->mUidError) {
        serializer->WriteAttribute(String(NULL), String("uidError"), String("true"));
    }
    if (pkg->mInstallStatus == PackageSettingBase::PKG_INSTALL_INCOMPLETE) {
        serializer->WriteAttribute(String(NULL), String("installStatus"), String("false"));
    }
    if (!pkg->mInstallerPackageName.IsNull()) {
        serializer->WriteAttribute(String(NULL), String("installer"), pkg->mInstallerPackageName);
    }
    pkg->mSignatures->WriteXml(serializer, String("sigs"), mPastSignatures);
    if ((pkg->mPkgFlags & IApplicationInfo::FLAG_SYSTEM) == 0) {
        serializer->WriteStartTag(String(NULL), String("perms"));
        if (pkg->mSharedUser == NULL) {
            // If this is a shared user, the permissions will
            // be written there. We still need to write an
            // empty permissions list so permissionsFixed will
            // be set.
            HashSet<String>::Iterator it = pkg->mGrantedPermissions.Begin();
            for (; it != pkg->mGrantedPermissions.End(); it++) {
                String name = *it;
                serializer->WriteStartTag(String(NULL), TAG_ITEM);
                serializer->WriteAttribute(String(NULL), ATTR_NAME, name);
                serializer->WriteEndTag(String(NULL), TAG_ITEM);
            }
        }
        serializer->WriteEndTag(String(NULL), String("perms"));
    }

    serializer->WriteEndTag(String(NULL), String("package"));
    return NOERROR;
}

ECode Settings::WritePermissionLPr(
    /* [in] */ IXmlSerializer* serializer,
    /* [in] */ BasePermission* bp)
{
    if (bp->mType != BasePermission::TYPE_BUILTIN && !bp->mSourcePackage.IsNull()) {
        serializer->WriteStartTag(String(NULL), TAG_ITEM);
        serializer->WriteAttribute(String(NULL), ATTR_NAME, bp->mName);
        serializer->WriteAttribute(String(NULL), String("package"), bp->mSourcePackage);
        if (bp->mProtectionLevel != IPermissionInfo::PROTECTION_NORMAL) {
            serializer->WriteAttribute(String(NULL), String("protection"), StringUtils::Int32ToString(bp->mProtectionLevel));
        }
        if (CPackageManagerService::DEBUG_SETTINGS)
            Slogger::I(CPackageManagerService::TAG, String("Writing perm: name=") + bp->mName + String(" type=")
                    + StringUtils::Int32ToString(bp->mType));
        if (bp->mType == BasePermission::TYPE_DYNAMIC) {
            AutoPtr<IPermissionInfo> pi = bp->mPerm != NULL ? bp->mPerm->mInfo : bp->mPendingInfo;
            Int32 val;
            String temp;
            if (pi != NULL) {
                serializer->WriteAttribute(String(NULL), String("type"), String("dynamic"));
                pi->GetIcon(&val);
                if (val != 0) {
                    serializer->WriteAttribute(String(NULL), String("icon"), StringUtils::Int32ToString(val));
                }
                AutoPtr<ICharSequence> label;
                pi->GetNonLocalizedLabel((ICharSequence**)&label);
                if (label != NULL) {
                    serializer->WriteAttribute( String(NULL), String("label"), (label->ToString(&temp), temp));
                }
            }
        }
        serializer->WriteEndTag(String(NULL), TAG_ITEM);
    }
    return NOERROR;
}

AutoPtr<List< AutoPtr<PackageSetting> > > Settings::GetListOfIncompleteInstallPackagesLPr()
{
    AutoPtr<List< AutoPtr<PackageSetting> > > ret = new List< AutoPtr<PackageSetting> >();
    HashMap<String, AutoPtr<PackageSetting> >::Iterator it;
    for (it = mPackages.Begin(); it != mPackages.End(); ++it) {
        AutoPtr<PackageSetting> ps = it->mSecond;
        if (ps->GetInstallStatus() == PackageSettingBase::PKG_INSTALL_INCOMPLETE) {
            ret->PushBack(ps);
        }
    }
    return ret;
}

void Settings::AddPackageToCleanLPw(
    /* [in] */ IPackageCleanItem* pkg)
{
    if (Find(mPackagesToBeCleaned.Begin(), mPackagesToBeCleaned.End(),
            AutoPtr<IPackageCleanItem>(pkg)) == mPackagesToBeCleaned.End()) {
        mPackagesToBeCleaned.PushBack(pkg);
    }
}

Boolean Settings::ReadLPw(
    /* [in] */ List< AutoPtr<IUserInfo> >& users,
    /* [in] */ Int32 sdkVersion,
    /* [in] */ Boolean onlyCore)
{
    AutoPtr<IFileInputStream> str;
    Boolean result;
    String temp;
    ECode ecode = NOERROR;
    if (mBackupSettingsFilename->Exists(&result), result) {
        //try {
            CFileInputStream::New(mBackupSettingsFilename, (IFileInputStream**)&str);
            mReadMessages->AppendString(String("Reading from backup settings file\n"));
            CPackageManagerService::ReportSettingsProblem(Logger::INFO,
                    String("Need to read from backup settings file"));
            if (mSettingsFilename->Exists(&result), result) {
                // If both the backup and settings file exist, we
                // ignore the settings since it might have been
                // corrupted.
                mSettingsFilename->ToString(&temp);
                Slogger::W(CPackageManagerService::TAG, String("Cleaning up settings file ")
                        + temp);
                FAIL_GOTO(ecode = mSettingsFilename->Delete(&result), EXIT);
            }
        //} catch (java.io.IOException e) {
        //    // We'll try for the normal settings file.
        //}
    }

    mPendingPackages.Clear();
    mPastSignatures.Clear();

    //try {
    if (str == NULL) {
        if (mSettingsFilename->Exists(&result), !result) {
            mReadMessages->AppendString(String("No settings file found\n"));
            CPackageManagerService::ReportSettingsProblem(Logger::INFO,
                    String("No settings file; creating initial state"));
            if (!onlyCore) {
                ReadDefaultPreferredAppsLPw(0);
            }
            mInternalSdkPlatform = mExternalSdkPlatform = sdkVersion;
            return FALSE;
        }
        CFileInputStream::New(mSettingsFilename, (IFileInputStream**)&str);
    }
    {
        AutoPtr<IXmlPullParser> parser = Xml::NewPullParser();
        parser->SetInput(str, String(NULL));
        Int32 type;
        while ((parser->Next(&type), type) != IXmlPullParser::START_TAG
                && type != IXmlPullParser::END_DOCUMENT) {
            ;
        }

        if (type != IXmlPullParser::START_TAG) {
            mReadMessages->AppendString(String("No start tag found in settings file\n"));
            CPackageManagerService::ReportSettingsProblem(Logger::WARN,
                   String("No start tag found in package manager settings"));
            // Logger::Wtf(CPackageManagerService.TAG,
            //         "No start tag found in package manager settings");
            return FALSE;
        }

        Int32 outerDepth, val;
        FAIL_GOTO(ecode = parser->GetDepth(&outerDepth), EXIT);
        while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
                && (type != IXmlPullParser::END_TAG || (parser->GetDepth(&val), val) > outerDepth)) {

            if (type == IXmlPullParser::END_TAG || type == IXmlPullParser::TEXT) {
                continue;
            }

            if (type != IXmlPullParser::START_TAG) {
                continue;
            }

            String tagName;
            FAIL_GOTO(ecode = parser->GetName(&tagName), EXIT);
            Boolean x = tagName.Equals("package");
            if (tagName.Equals("package")) {
                ReadPackageLPw(parser);
            }
            else if (tagName.Equals("permissions")) {
                ReadPermissionsLPw(mPermissions, parser);
            }
            else if (tagName.Equals("permission-trees")) {
                ReadPermissionsLPw(mPermissionTrees, parser);
            }
            else if (tagName.Equals("shared-user")) {
                ReadSharedUserLPw(parser);
            }
            else if (tagName.Equals("preferred-packages")) {
                // no longer used.
            }
            else if (tagName.Equals("preferred-activities")) {
                // Upgrading from old single-user implementation;
                // these are the preferred activities for user 0.
                ReadPreferredActivitiesLPw(parser, 0);
            }
            else if (tagName.Equals("updated-package")) {
                ReadDisabledSysPackageLPw(parser);
            }
            else if (tagName.Equals("cleaning-package")) {
                String name;
                FAIL_GOTO(ecode = parser->GetAttributeValue(String(NULL), ATTR_NAME, &name), EXIT);
                String userStr;
                FAIL_GOTO(ecode = parser->GetAttributeValue(String(NULL), ATTR_USER, &userStr), EXIT);
                String codeStr;
                FAIL_GOTO(ecode = parser->GetAttributeValue(String(NULL), ATTR_CODE, &codeStr), EXIT);
                if (!name.IsNull()) {
                    Int32 userId = 0;
                    Boolean andCode = TRUE;
                    //try {
                        if (!userStr.IsNull()) {
                            userId = StringUtils::ParseInt32(userStr);
                        }
                    // } catch (NumberFormatException e) {
                    // }
                    if (!codeStr.IsNull()) {
                        andCode = StringUtils::ParseInt32(codeStr);
                    }
                    AutoPtr<IPackageCleanItem> pkg;
                    CPackageCleanItem::New(userId, name, andCode, (IPackageCleanItem**)&pkg);
                    AddPackageToCleanLPw(pkg);
                }

            }
            else if (tagName.Equals("renamed-package")) {
                String nname;
                FAIL_GOTO(ecode = parser->GetAttributeValue(String(NULL), String("new"), &nname), EXIT);
                String oname;
                FAIL_GOTO(ecode = parser->GetAttributeValue(String(NULL), String("old"), &oname), EXIT);
                if (!nname.IsNull() && !oname.IsNull()) {
                    mRenamedPackages[nname] = oname;
                }
            }
            else if (tagName.Equals("last-platform-version")) {
                mInternalSdkPlatform = mExternalSdkPlatform = 0;
                //try {
                    String internal;
                    FAIL_GOTO(ecode = parser->GetAttributeValue(String(NULL), String("internal"), &internal), EXIT);
                    if (!internal.IsNull()) {
                        mInternalSdkPlatform = StringUtils::ParseInt32(internal);
                    }
                    String external;
                    FAIL_GOTO(ecode = parser->GetAttributeValue(String(NULL), String("external"), &external), EXIT);
                    if (!external.IsNull()) {
                        mExternalSdkPlatform = StringUtils::ParseInt32(external);
                    }
                // } catch (NumberFormatException e) {
                // }
            }
            else if (tagName.Equals("verifier")) {
                String deviceIdentity;
                FAIL_GOTO(ecode = parser->GetAttributeValue(String(NULL), String("device"), &deviceIdentity), EXIT);
                //try {
                    AutoPtr<IVerifierDeviceIdentityHelper> vdiHelper;
                    CVerifierDeviceIdentityHelper::AcquireSingleton((IVerifierDeviceIdentityHelper**)&vdiHelper);
                    mVerifierDeviceIdentity = NULL;
                    ecode = vdiHelper->Parse(deviceIdentity, (IVerifierDeviceIdentity**)&mVerifierDeviceIdentity);
                    if (FAILED(ecode)) {
                        Slogger::W(CPackageManagerService::TAG, "Discard invalid verifier device id: ");
                    }
                //} catch (IllegalArgumentException e) {
                    // Slogger::W(CPackageManagerService::TAG, "Discard invalid verifier device id: "
                    //         + e.getMessage());
                //}
            }
            else if (TAG_READ_EXTERNAL_STORAGE.Equals(tagName)) {
                String enforcement;
                FAIL_GOTO(ecode = parser->GetAttributeValue(String(NULL), ATTR_ENFORCEMENT, &enforcement), EXIT);
                mReadExternalStorageEnforced = NULL;
                CBoolean::New(CString("1").Equals(enforcement), (IBoolean**)&mReadExternalStorageEnforced);
            }
            else {
                // Slogger::W(CPackageManagerService::TAG, String("Unknown element under <packages>: ")
                //         + (parser->GetName(&temp), temp.IsNull() ? String("NULL") : temp));
                FAIL_GOTO(ecode = XmlUtils::SkipCurrentTag(parser), EXIT);
            }
        }
        str->Close();
    }
    // } catch (XmlPullParserException e) {
    // mReadMessages.append("Error reading: " + e.toString());
    // PackageManagerService.reportSettingsProblem(Log.ERROR, "Error reading settings: " + e);
    // Log.wtf(PackageManagerService.TAG, "Error reading package manager settings", e);

    // } catch (java.io.IOException e) {
    //     mReadMessages.append("Error reading: " + e.toString());
    //     PackageManagerService.reportSettingsProblem(Log.ERROR, "Error reading settings: " + e);
    //     Log.wtf(PackageManagerService.TAG, "Error reading package manager settings", e);
    // }

EXIT:
    if (E_XML_PULL_PARSER_EXCEPTION == ecode) {
        String str("Error reading accounts ecode = XmlPullParserException");
        mReadMessages->AppendString(String("Error reading: "));
        CPackageManagerService::ReportSettingsProblem(Logger::ERROR, String("Error reading settings: "));
        Slogger::W(TAG, str);
    }
    else if (E_IO_EXCEPTION == ecode) {
        mReadMessages->AppendString(String("Error reading: "));
        CPackageManagerService::ReportSettingsProblem(Logger::ERROR, String("Error reading settings: "));
    }

    Int32 N = mPendingPackages.GetSize();
    List< AutoPtr<PendingPackage> >::Iterator it = mPendingPackages.Begin();
    for (; it != mPendingPackages.End(); it++) {
        AutoPtr<PendingPackage> pp = (*it).Get();
        AutoPtr<IInterface> idObj = GetUserIdLPr(pp->mSharedId);
        if (idObj.Get() != NULL && idObj->Probe(EIID_SharedUserSetting)) {
            SharedUserSetting* sus = reinterpret_cast<SharedUserSetting*>(idObj->Probe(EIID_SharedUserSetting));
            AutoPtr<PackageSetting> p = GetPackageLPw(pp->mName, NULL, pp->mRealName,
                    sus, pp->mCodePath, pp->mResourcePath,
                    pp->mNativeLibraryPathString, pp->mVersionCode, pp->mPkgFlags,
                    NULL, TRUE /* add */, FALSE /* allowInstall */);
            if (p == NULL) {
                CPackageManagerService::ReportSettingsProblem(Logger::WARN,
                        String("Unable to create application package for ") + pp->mName);
                continue;
            }
            p->CopyFrom((PackageSettingBase*)pp);
        } else if (idObj != NULL) {
            String msg = String("Bad package setting: package ") + pp->mName + String(" has shared uid ")
                    + StringUtils::Int32ToString(pp->mSharedId) + String(" that is not a shared uid\n");
            mReadMessages->AppendString(msg);
            CPackageManagerService::ReportSettingsProblem(Logger::ERROR, msg);
        } else {
            String msg = String("Bad package setting: package ") + String(pp->mName) + String(" has shared uid ")
                    + StringUtils::Int32ToString(pp->mSharedId) + String(" that is not defined\n");
            mReadMessages->AppendString(msg);
            CPackageManagerService::ReportSettingsProblem(Logger::ERROR, msg);
        }
    }
    mPendingPackages.Clear();

    if ((mBackupStoppedPackagesFilename->Exists(&result), result)
                || (mStoppedPackagesFilename->Exists(&result), result)) {
            // Read old file
            ReadStoppedLPw();
            mBackupStoppedPackagesFilename->Delete(&result);
            mStoppedPackagesFilename->Delete(&result);
            // Migrate to new file format
            WritePackageRestrictionsLPr(0);
        } else {
            if (0 == users.GetSize()) {
                ReadPackageRestrictionsLPr(0);
            } else {
                List< AutoPtr<IUserInfo> >::Iterator it2 = users.Begin();
                for (; it2 != users.End(); it2++) {
                    AutoPtr<IUserInfo> user = it2->Get();
                    Int32 userId;
                    user->GetId(&userId);
                    ReadPackageRestrictionsLPr(userId);
                }
            }
        }

    /*
     * Make sure all the updated system packages have their shared users
     * associated with them.
     */
    HashMap<String, AutoPtr<PackageSetting> >::Iterator disabledIt = mDisabledSysPackages.Begin();
    //Iterator<PackageSetting> disabledIt = mDisabledSysPackages.values().iterator();
    for(; disabledIt != mDisabledSysPackages.End(); disabledIt++) {
        AutoPtr<PackageSetting> disabledPs = disabledIt->mSecond;
        AutoPtr<IInterface> id = GetUserIdLPr(disabledPs->mAppId);
        if (id != NULL && id->Probe(EIID_SharedUserSetting)) {
            disabledPs->mSharedUser = reinterpret_cast<SharedUserSetting*>(id->Probe(EIID_SharedUserSetting));
        }
    }

    mReadMessages->AppendString(String("Read completed successfully: ") + StringUtils::Int32ToString(mPackages.GetSize()) + String(" packages, ")
             + StringUtils::Int32ToString(mSharedUsers.GetSize()) + String(" shared uids\n"));
    return TRUE;
}

void Settings::ReadDefaultPreferredAppsLPw(
    /* [in] */ Int32 userId)
{
    // Read preferred apps from .../etc/preferred-apps directory.
    AutoPtr<IFile> preferredDir;
    AutoPtr<IEnvironment> env;
    CEnvironment::AcquireSingleton((IEnvironment**)&env);
    AutoPtr<IFile> dir;
    env->GetRootDirectory((IFile**)&dir);
    CFile::New(dir, String("etc/preferred-apps"), (IFile**)&preferredDir);
    Boolean result;
    String temp;
    ECode ecode = NOERROR;
    if ((preferredDir->Exists(&result), !result) || (preferredDir->IsDirectory(&result), !result)) {
        return;
    }
    if ((preferredDir->CanRead(&result), !result)) {
        Slogger::W(TAG, String("Directory ") + (preferredDir->ToString(&temp), temp) + String(" cannot be read"));
        return;
    }

    AutoPtr<ArrayOf<IFile*> > preferredFiles;
    preferredDir->ListFiles((ArrayOf<IFile*>**)&preferredFiles);

    // Iterate over the files in the directory and scan .xml files
    for (Int32 i = 0; preferredFiles != NULL && i < preferredFiles->GetLength(); i++) {
        AutoPtr<IFile> f = (*preferredFiles)[i];
        f->GetPath(&temp);
        if (!temp.EndWith(".xml")) {
            Slogger::I(TAG, String("Non-xml file ") + (f->ToString(&temp), temp) + String(" in ") + (preferredDir->ToString(&temp), temp) + String(" directory, ignoring"));
            continue;
        }
        if (f->CanRead(&result), result) {
            Slogger::W(TAG, String("Preferred apps file ") + (f->ToString(&temp), temp) + String(" cannot be read"));
            continue;
        }

        AutoPtr<IFileInputStream> str;
        //try
        {
            FAIL_GOTO(ecode = CFileInputStream::New(f, (IFileInputStream**)&str), EXIT);
            AutoPtr<IXmlPullParser> parser = Xml::NewPullParser();
            FAIL_GOTO(ecode = parser->SetInput(str, String(NULL)), EXIT);

            Int32 type;
            while ((parser->Next(&type), type) != IXmlPullParser::START_TAG
                    && type != IXmlPullParser::END_DOCUMENT) {
                ;
            }

            if (type != IXmlPullParser::START_TAG) {
                Slogger::W(TAG, String("Preferred apps file ") + (f->ToString(&temp), temp) + String(" does not have start tag"));
                continue;
            }
            parser->GetName(&temp);
            if (!temp.Equals("preferred-activities")) {
                Slogger::W(TAG, String("Preferred apps file ") + (f->ToString(&temp), temp)
                        + String(" does not start with 'preferred-activities'"));
                continue;
            }
            ReadPreferredActivitiesLPw(parser, userId);
        // } catch (XmlPullParserException e) {
        //     Slog.w(TAG, "Error reading apps file " + f, e);
        // } catch (IOException e) {
        //     Slog.w(TAG, "Error reading apps file " + f, e);
        // } finally {
        //     if (str != NULL) {
        //         try {
        //             str.close();
        //         } catch (IOException e) {
        //         }
        //     }
        // }
        }

        EXIT:
            if (E_XML_PULL_PARSER_EXCEPTION == ecode) {
                Slogger::W(TAG, String("Error reading apps file ") + (f->ToString(&temp), temp));
            }
            else if (E_IO_EXCEPTION == ecode) {
                Slogger::W(TAG, String("Error reading apps file ") + (f->ToString(&temp), temp));
            }
            if (str != NULL) {
                //try {
                    str->Close();
                // } catch (IOException e) {
                // }
            }
    }
}

Int32 Settings::ReadInt32(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ const String& ns,
    /* [in] */ const String& name,
    /* [in] */ Int32 defValue)
{
    String v;
    parser->GetAttributeValue(ns, name, &v);
    // try {
    if (v.IsNull()) {
        return defValue;
    }
    return StringUtils::ParseInt32(v, 10, defValue);
    // } catch (NumberFormatException e) {
    //     PackageManagerService.reportSettingsProblem(Log.WARN,
    //             "Error in package manager settings: attribute " + name
    //                     + " has bad integer value " + v + " at "
    //                     + parser.getPositionDescription());
    // }
    // return defValue;
}

ECode Settings::ReadPermissionsLPw(
    /* [in] */ HashMap<String, AutoPtr<BasePermission> >& out,
    /* [in] */ IXmlPullParser* parser)
{
    Int32 outerDepth, outerDepth1;
    parser->GetDepth(&outerDepth);
    Int32 type;
    String temp;
    while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
            && (type != IXmlPullParser::END_TAG || (parser->GetDepth(&outerDepth1), outerDepth1) > outerDepth)) {
        if (type == IXmlPullParser::END_TAG || type == IXmlPullParser::TEXT) {
            continue;
        }

        if (type != IXmlPullParser::START_TAG) {
            continue;
        }

        String tagName;
        parser->GetName(&tagName);
        if (tagName.Equals(TAG_ITEM)) {
            String name;
            parser->GetAttributeValue(String(NULL), ATTR_NAME, &name);
            String sourcePackage;
            parser->GetAttributeValue(String(NULL), String("package"), &sourcePackage);
            String ptype;
            parser->GetAttributeValue(String(NULL), String("type"), &ptype);
            if (!name.IsNull() && !sourcePackage.IsNull()) {
                Boolean dynamic = CString("dynamic").Equals(ptype);
                AutoPtr<BasePermission> bp = new BasePermission(name, sourcePackage,
                        dynamic ? BasePermission::TYPE_DYNAMIC : BasePermission::TYPE_NORMAL);
                bp->mProtectionLevel = ReadInt32(parser, String(NULL), String("protection"),
                        IPermissionInfo::PROTECTION_NORMAL);
                AutoPtr<IPermissionInfoHelper> helper;
                CPermissionInfoHelper::AcquireSingleton((IPermissionInfoHelper**)&helper);
                helper->FixProtectionLevel(bp->mProtectionLevel, &bp->mProtectionLevel);

                if (dynamic) {
                    AutoPtr<IPermissionInfo> pi;
                    CPermissionInfo::New((IPermissionInfo**)&pi);
                    pi->SetPackageName(sourcePackage);
                    pi->SetName(name);
                    pi->SetIcon(ReadInt32(parser, String(NULL), String("icon"), 0));

                    parser->GetAttributeValue(String(NULL), String("label"), &temp);
                    AutoPtr<ICharSequence> csq;
                    CStringWrapper::New(temp, (ICharSequence**)&csq);

                    pi->SetNonLocalizedLabel(csq);
                    pi->SetProtectionLevel(bp->mProtectionLevel);
                    bp->mPendingInfo = pi;
                }

                out[bp->mName] = bp;
            } else {
                CPackageManagerService::ReportSettingsProblem(Logger::WARN,
                        String("Error in package manager settings: permissions has") + String(" no name at ")
                                + (parser->GetPositionDescription(&temp), temp));
            }
        } else {
            CPackageManagerService::ReportSettingsProblem(Logger::WARN,
                    String("Unknown element reading permissions: ") + (parser->GetName(&temp), temp.IsNull() ? String("NULL") : temp) + String(" at ")
                            + (parser->GetPositionDescription(&temp), temp));
        }
        XmlUtils::SkipCurrentTag(parser);
    }
    return NOERROR;
}

ECode Settings::ReadDisabledSysPackageLPw(
    /* [in] */ IXmlPullParser* parser)
{
    String temp;
    String name;
    parser->GetAttributeValue(String(NULL), ATTR_NAME, &name);
    String realName;
    parser->GetAttributeValue(String(NULL), String("realName"), &realName);
    String codePathStr;
    parser->GetAttributeValue(String(NULL), String("codePath"), &codePathStr);
    String resourcePathStr;
    parser->GetAttributeValue(String(NULL), String("resourcePath"), &resourcePathStr);
    String nativeLibraryPathStr;
    parser->GetAttributeValue(String(NULL), String("nativeLibraryPath"), &nativeLibraryPathStr);
    if (resourcePathStr.IsNull()) {
        resourcePathStr = codePathStr;
    }
    String version;
    parser->GetAttributeValue(String(NULL), String("version"), &version);
    Int32 versionCode = 0;
    if (!version.IsNull()) {
        //try {
        versionCode = StringUtils::ParseInt32(version);
        // } catch (NumberFormatException e) {
        // }
    }

    Int32 pkgFlags = 0;
    pkgFlags |= IApplicationInfo::FLAG_SYSTEM;
    AutoPtr<IFile> codePathFile, resourcePathFile;
    CFile::New(codePathStr, (IFile**)&codePathFile);
    CFile::New(resourcePathStr, (IFile**)&resourcePathFile);
    AutoPtr<PackageSetting> ps = new PackageSetting(name, realName, codePathFile,
            resourcePathFile, nativeLibraryPathStr, versionCode, pkgFlags);
    String timeStampStr;
    parser->GetAttributeValue(String(NULL), String("ft"), &timeStampStr);
    if (!timeStampStr.IsNull()) {
        //try {
        Int64 timeStamp = StringUtils::ParseInt64(timeStampStr, 16);
        ps->SetTimeStamp(timeStamp);
        // } catch (NumberFormatException e) {
        // }
    } else {
        parser->GetAttributeValue(String(NULL), String("ts"), &timeStampStr);
        if (!timeStampStr.IsNull()) {
            //try {
                Int64 timeStamp = StringUtils::ParseInt64(timeStampStr);
                ps->SetTimeStamp(timeStamp);
            // } catch (NumberFormatException e) {
            // }
        }
    }

    parser->GetAttributeValue(String(NULL), String("it"), &timeStampStr);
    if (!timeStampStr.IsNull()) {
        //try {
            ps->mFirstInstallTime = StringUtils::ParseInt64(timeStampStr, 16);
        // } catch (NumberFormatException e) {
        // }
    }
    parser->GetAttributeValue(String(NULL), String("ut"), &timeStampStr);
    if (!timeStampStr.IsNull()) {
        //try {
            ps->mLastUpdateTime = StringUtils::ParseInt64(timeStampStr, 16);
        // } catch (NumberFormatException e) {
        // }
    }

    String idStr;
    parser->GetAttributeValue(String(NULL), String("userId"), &idStr);
    ps->mAppId = !idStr.IsNull() ? StringUtils::ParseInt32(idStr) : 0;
    if (ps->mAppId <= 0) {
        String sharedIdStr;
        parser->GetAttributeValue(String(NULL), String("sharedUserId"), &sharedIdStr);
        ps->mAppId = !sharedIdStr.IsNull() ? StringUtils::ParseInt32(sharedIdStr) : 0;
    }
    Int32 outerDepth, outerDepth1;
    parser->GetDepth(&outerDepth);
    Int32 type;
    while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
            && (type != IXmlPullParser::END_TAG || (parser->GetDepth(&outerDepth1), outerDepth1) > outerDepth)) {
        if (type != IXmlPullParser::START_TAG) {
            continue;
        }

        String tagName;
        parser->GetName(&tagName);
        if (tagName.Equals("perms")) {
            ReadGrantedPermissionsLPw(parser, ps->mGrantedPermissions);
        } else {
            CPackageManagerService::ReportSettingsProblem(Logger::WARN,
                    String("Unknown element under <updated-package>: ") + (parser->GetName(&temp), temp.IsNull() ? String("NULL") : temp));
            XmlUtils::SkipCurrentTag(parser);
        }
    }
    mDisabledSysPackages[name] = ps;
    return NOERROR;
}

ECode Settings::ReadPackageLPw(
    /* [in] */ IXmlPullParser* parser)
{
    String name;
    String realName;
    String idStr;
    String sharedIdStr;
    String codePathStr;
    String resourcePathStr;
    String nativeLibraryPathStr;
    String systemStr;
    String installerPackageName;
    String uidError;
    Int32 pkgFlags = 0;
    Int64 timeStamp = 0;
    Int64 firstInstallTime = 0;
    Int64 lastUpdateTime = 0;
    AutoPtr<PackageSettingBase> packageSetting;
    String version;
    Int32 versionCode = 0;
//     try {
    parser->GetAttributeValue(String(NULL), ATTR_NAME, &name);
    parser->GetAttributeValue(String(NULL), String("realName"), &realName);
    parser->GetAttributeValue(String(NULL), String("userId"), &idStr);
    parser->GetAttributeValue(String(NULL), String("uidError"), &uidError);
    parser->GetAttributeValue(String(NULL), String("sharedUserId"), &sharedIdStr);
    parser->GetAttributeValue(String(NULL), String("codePath"), &codePathStr);
    parser->GetAttributeValue(String(NULL), String("resourcePath"), &resourcePathStr);
    parser->GetAttributeValue(String(NULL), String("nativeLibraryPath"), &nativeLibraryPathStr);
    parser->GetAttributeValue(String(NULL), String("version"), &version);
    if (!version.IsNull()) {
        // try {
        versionCode = StringUtils::ParseInt32(version);
        // } catch (NumberFormatException e) {
        // }
    }
    parser->GetAttributeValue(String(NULL), String("installer"), &installerPackageName);

    parser->GetAttributeValue(String(NULL), String("flags"), &systemStr);
    if (!systemStr.IsNull()) {
        // try {
        pkgFlags = StringUtils::ParseInt32(systemStr);
        // } catch (NumberFormatException e) {
        // }
    }
    else {
        // For backward compatibility
        parser->GetAttributeValue(String(NULL), String("system"), &systemStr);
        if (!systemStr.IsNull()) {
            pkgFlags |= (CString("true").EqualsIgnoreCase(systemStr)) ? IApplicationInfo::FLAG_SYSTEM
                    : 0;
        }
        else {
            // Old settings that don't specify system... just treat
            // them as system, good enough.
            pkgFlags |= IApplicationInfo::FLAG_SYSTEM;
        }
    }
    String timeStampStr;
    parser->GetAttributeValue(String(NULL), String("ft"), &timeStampStr);
    if (!timeStampStr.IsNull()) {
        // try {
        timeStamp = StringUtils::ParseInt64(timeStampStr, 16);
        // } catch (NumberFormatException e) {
        // }
    }
    else {
        parser->GetAttributeValue(String(NULL), String("ts"), &timeStampStr);
        if (!timeStampStr.IsNull()) {
            // try {
            timeStamp = StringUtils::ParseInt64(timeStampStr);
            // } catch (NumberFormatException e) {
            // }
        }
    }
    parser->GetAttributeValue(String(NULL), String("it"), &timeStampStr);
    if (!timeStampStr.IsNull()) {
        // try {
        firstInstallTime = StringUtils::ParseInt64(timeStampStr, 16);
        // } catch (NumberFormatException e) {
        // }
    }
    parser->GetAttributeValue(String(NULL), String("ut"), &timeStampStr);
    if (!timeStampStr.IsNull()) {
        // try {
        lastUpdateTime = StringUtils::ParseInt64(timeStampStr, 16);
        // } catch (NumberFormatException e) {
        // }
    }
    if (CPackageManagerService::DEBUG_SETTINGS)
        Slogger::I(CPackageManagerService::TAG, String("Reading package: ") + name + String(" userId=") + idStr
               + String(" sharedUserId=") + sharedIdStr);
    Int32 userId = !idStr.IsNull() ? StringUtils::ParseInt32(idStr) : 0;
    if (resourcePathStr.IsNull()) {
        resourcePathStr = codePathStr;
    }
    // if (realName != null) {
    //     realName = realName.intern();
    // }
    String parserPositionDesc;
    parser->GetPositionDescription(&parserPositionDesc);
    StringBuilder sb;
    String msg;
    if (name.IsNull()) {
        sb += "Error in package manager settings: <package> has no name at ";
        sb += parserPositionDesc;
        CPackageManagerService::ReportSettingsProblem(Logger::WARN, (sb.ToString(&msg), msg));
    }
    else if (codePathStr.IsNull()) {
        sb += "Error in package manager settings: <package> has no codePath at ";
        sb += parserPositionDesc;
        CPackageManagerService::ReportSettingsProblem(Logger::WARN, (sb.ToString(&msg), msg));
    }
    else if (userId > 0) {
        AutoPtr<IFile> codePath, resourcePath;
        CFile::New(codePathStr, (IFile**)&codePath);
        CFile::New(resourcePathStr, (IFile**)&resourcePath);
        packageSetting = AddPackageLPw(name, realName, codePath,
                resourcePath, nativeLibraryPathStr, userId, versionCode, pkgFlags);
       if (CPackageManagerService::DEBUG_SETTINGS)
            sb += "Reading package ";
            sb += name;
            sb += ": userId=";
            sb += userId;
            sb += " pkg=";
            Slogger::I(CPackageManagerService::TAG, (sb.ToString(&msg), msg));
        if (packageSetting == NULL) {
            sb += "Failure adding uid ";
            sb += userId;
            sb += " while parsing settings at ";
            sb += parserPositionDesc;
            CPackageManagerService::ReportSettingsProblem(Logger::ERROR, (sb.ToString(&msg), msg));
        }
        else {
            packageSetting->SetTimeStamp(timeStamp);
            packageSetting->mFirstInstallTime = firstInstallTime;
            packageSetting->mLastUpdateTime = lastUpdateTime;
        }
    }
    else if (!sharedIdStr.IsNull()) {
        userId = StringUtils::ParseInt32(sharedIdStr);
        if (userId > 0) {
            AutoPtr<IFile> codePath, resourcePath;
            CFile::New(codePathStr, (IFile**)&codePath);
            CFile::New(resourcePathStr, (IFile**)&resourcePath);
            packageSetting = new PendingPackage(name, realName, codePath,
                    resourcePath, nativeLibraryPathStr, userId, versionCode, pkgFlags);
            packageSetting->SetTimeStamp(timeStamp);
            packageSetting->mFirstInstallTime = firstInstallTime;
            packageSetting->mLastUpdateTime = lastUpdateTime;
            mPendingPackages.PushBack((PendingPackage*)packageSetting.Get());
           if (CPackageManagerService::DEBUG_SETTINGS)
                sb += "Reading package ";
                sb += name;
                sb += ": sharedUserId=";
                sb += userId;
                sb += " pkg=";
               Slogger::I(CPackageManagerService::TAG, (sb.ToString(&msg), msg));
        }
        else {
            sb += "Error in package manager settings: package ";
            sb += name;
            sb += " has bad sharedId ";
            sb += sharedIdStr;
            sb += " at ";
            sb += parserPositionDesc;
            CPackageManagerService::ReportSettingsProblem(Logger::WARN, (sb.ToString(&msg), msg));
        }
    }
    else {
        sb += "Error in package manager settings: package ";
        sb += name;
        sb += " has bad userId ";
        sb += idStr;
        sb += " at ";
        sb += parserPositionDesc;
        CPackageManagerService::ReportSettingsProblem(Logger::WARN, (sb.ToString(&msg), msg));
    }
//     } catch (NumberFormatException e) {
//         PackageManagerService.reportSettingsProblem(Log.WARN,
//                 "Error in package manager settings: package " + name + " has bad userId "
//                         + idStr + " at " + parser.getPositionDescription());
//     }
    if (packageSetting != NULL) {
        packageSetting->mUidError = CString("true").Equals(uidError);
        packageSetting->mInstallerPackageName = installerPackageName;
        packageSetting->mNativeLibraryPathString = nativeLibraryPathStr;
        // Handle legacy string here for single-user mode
        String enabledStr;
        parser->GetAttributeValue(String(NULL), ATTR_ENABLED, &enabledStr);
        if (!enabledStr.IsNull()) {
            // try {
            packageSetting->SetEnabled(StringUtils::ParseInt32(enabledStr), 0 /* userId */);
            // } catch (NumberFormatException e) {
            //     if (enabledStr.equalsIgnoreCase("true")) {
            //         packageSetting.setEnabled(COMPONENT_ENABLED_STATE_ENABLED, 0);
            //     } else if (enabledStr.equalsIgnoreCase("false")) {
            //         packageSetting.setEnabled(COMPONENT_ENABLED_STATE_DISABLED, 0);
            //     } else if (enabledStr.equalsIgnoreCase("default")) {
            //         packageSetting.setEnabled(COMPONENT_ENABLED_STATE_DEFAULT, 0);
            //     } else {
            //         PackageManagerService.reportSettingsProblem(Log.WARN,
            //                 "Error in package manager settings: package " + name
            //                         + " has bad enabled value: " + idStr + " at "
            //                         + parser.getPositionDescription());
            //     }
            // }
        }
        else {
            packageSetting->SetEnabled(IPackageManager::COMPONENT_ENABLED_STATE_DEFAULT, 0);
        }

        String installStatusStr;
        parser->GetAttributeValue(String(NULL), String("installStatus"), &installStatusStr);
        if (!installStatusStr.IsNull()) {
            if (installStatusStr.EqualsIgnoreCase("false")) {
                packageSetting->mInstallStatus = PackageSettingBase::PKG_INSTALL_INCOMPLETE;
            }
            else {
                packageSetting->mInstallStatus = PackageSettingBase::PKG_INSTALL_COMPLETE;
            }
        }

        Int32 outerDepth, depth, type;
        parser->GetDepth(&outerDepth);
        while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
                && (type != IXmlPullParser::END_TAG || (parser->GetDepth(&depth), depth) > outerDepth)) {
            if (type == IXmlPullParser::END_TAG || type == IXmlPullParser::TEXT) {
                continue;
            }

            if (type != IXmlPullParser::START_TAG) {
                continue;
            }

            String tagName;
            parser->GetName(&tagName);
            // Legacy
            if (tagName.Equals(TAG_DISABLED_COMPONENTS)) {
                ReadDisabledComponentsLPw(packageSetting, parser, 0);
            }
            else if (tagName.Equals(TAG_ENABLED_COMPONENTS)) {
                ReadEnabledComponentsLPw(packageSetting, parser, 0);
            }
            else if (tagName.Equals("sigs")) {
                packageSetting->mSignatures->ReadXml(parser, mPastSignatures);
            }
            else if (tagName.Equals("perms")) {
                ReadGrantedPermissionsLPw(parser, packageSetting->mGrantedPermissions);
                packageSetting->mPermissionsFixed = TRUE;
            }
            else {
                String nameStr;
                parser->GetName(&nameStr);
                CPackageManagerService::ReportSettingsProblem(Logger::WARN,
                       String("Unknown element under <package>: ") + nameStr);
                XmlUtils::SkipCurrentTag(parser);
            }
        }
    }
    else {
        XmlUtils::SkipCurrentTag(parser);
    }
    return NOERROR;
}

ECode Settings::ReadDisabledComponentsLPw(
    /* [in] */ PackageSettingBase* packageSetting,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ Int32 userId)
{
    Int32 outerDepth, outerDepth1;
    String positionDes, name;
    parser->GetDepth(&outerDepth);
    Int32 type;
    while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
            && (type != IXmlPullParser::END_TAG || (parser->GetDepth(&outerDepth1), outerDepth1) > outerDepth)) {
        if (type == IXmlPullParser::END_TAG || type == IXmlPullParser::TEXT) {
            continue;
        }

        if (type != IXmlPullParser::START_TAG) {
            continue;
        }

        String tagName;
        parser->GetName(&tagName);
        if (tagName.Equals(TAG_ITEM)) {
            String name;
            parser->GetAttributeValue(String(NULL), ATTR_NAME, &name);
            if (!name.IsNull()) {
                packageSetting->AddDisabledComponent(name, userId);
            } else {
                CPackageManagerService::ReportSettingsProblem(Logger::WARN,
                        String("Error in package manager settings: <disabled-components> has")
                                + String(" no name at ") + (parser->GetPositionDescription(&positionDes), positionDes));
            }
        } else {
            CPackageManagerService::ReportSettingsProblem(Logger::WARN,
                    String("Unknown element under <disabled-components>: ") + (parser->GetName(&name), name.IsNull() ? String("NULL") : name));
        }
        XmlUtils::SkipCurrentTag(parser);
    }
    return NOERROR;
}

ECode Settings::ReadEnabledComponentsLPw(
    /* [in] */ PackageSettingBase* packageSetting,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ Int32 userId)
{
    Int32 outerDepth, outerDepth1;
    parser->GetDepth(&outerDepth);
    Int32 type;
    String positionDes, name;
    while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
            && (type != IXmlPullParser::END_TAG || (parser->GetDepth(&outerDepth1), outerDepth1) > outerDepth)) {
        if (type == IXmlPullParser::END_TAG || type == IXmlPullParser::TEXT) {
            continue;
        }

        if (type != IXmlPullParser::START_TAG) {
            continue;
        }

        String tagName;
        parser->GetName(&tagName);
        if (tagName.Equals(TAG_ITEM)) {
            String name;
            parser->GetAttributeValue(String(NULL), ATTR_NAME, &name);
            if (!name.IsNull()) {
                packageSetting->AddEnabledComponent(name, userId);
            } else {
                CPackageManagerService::ReportSettingsProblem(Logger::WARN,
                        String("Error in package manager settings: <enabled-components> has")
                                + String(" no name at ") + (parser->GetPositionDescription(&positionDes), positionDes));
            }
        } else {
            CPackageManagerService::ReportSettingsProblem(Logger::WARN,
                    String("Unknown element under <enabled-components>: ") + (parser->GetName(&name), name.IsNull() ? String("NULL") : name));
        }
        XmlUtils::SkipCurrentTag(parser);
    }
    return NOERROR;
}

ECode Settings::ReadSharedUserLPw(
    /* [in] */ IXmlPullParser* parser)
{
    String name;
    String idStr;
    String temp;
    Int32 pkgFlags = 0;
    AutoPtr<SharedUserSetting> su;
    //try
    {
        parser->GetAttributeValue(String(NULL), ATTR_NAME, &name);
        parser->GetAttributeValue(String(NULL), String("userId"), &idStr);
        Int32 userId = !idStr.IsNull() ? StringUtils::ParseInt32(idStr) : 0;
        parser->GetAttributeValue(String(NULL), String("system"), &temp);
        if (CString("true").Equals(temp)) {
            pkgFlags |= IApplicationInfo::FLAG_SYSTEM;
        }
        if (name.IsNull()) {
            CPackageManagerService::ReportSettingsProblem(Logger::WARN,
                    String("Error in package manager settings: <shared-user> has no name at ")
                            + (parser->GetPositionDescription(&temp), temp));
        } else if (userId == 0) {
            CPackageManagerService::ReportSettingsProblem(Logger::WARN,
                    String("Error in package manager settings: shared-user ") + name
                            + String(" has bad userId ") + idStr + String(" at ")
                            + (parser->GetPositionDescription(&temp), temp));
        } else {
            if ((su = AddSharedUserLPw(name, userId, pkgFlags)) == NULL) {
                CPackageManagerService::ReportSettingsProblem(Logger::ERROR, String("Occurred while parsing settings at ")
                                + (parser->GetPositionDescription(&temp), temp));
            }
        }
    // } catch (NumberFormatException e) {
    //     CPackageManagerService::ReportSettingsProblem(Logger::WARN,
    //             "Error in package manager settings: package " + name + " has bad userId "
    //                     + idStr + " at " + parser->GetPositionDescription());
    // }
    }
    ;

    if (su != NULL) {
        Int32 outerDepth, outerDepth1;
        parser->GetDepth(&outerDepth);
        Int32 type;
        while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
                && (type != IXmlPullParser::END_TAG || (parser->GetDepth(&outerDepth1), outerDepth1) > outerDepth)) {
            if (type == IXmlPullParser::END_TAG || type == IXmlPullParser::TEXT) {
                continue;
            }

            if (type != IXmlPullParser::START_TAG) {
                continue;
            }

            String tagName;
            parser->GetName(&tagName);
            if (tagName.Equals("sigs")) {
                su->mSignatures->ReadXml(parser, mPastSignatures);
            } else if (tagName.Equals("perms")) {
                ReadGrantedPermissionsLPw(parser, su->mGrantedPermissions);
            } else {
                CPackageManagerService::ReportSettingsProblem(Logger::WARN,
                        String("Unknown element under <shared-user>: ") + (parser->GetName(&temp), temp.IsNull() ? String("NULL") : temp));
                XmlUtils::SkipCurrentTag(parser);
            }
        }

    } else {
        XmlUtils::SkipCurrentTag(parser);
    }
    return NOERROR;
}

ECode Settings::ReadGrantedPermissionsLPw(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ HashSet<String>& outPerms)
{
    Int32 outerDepth, outerDepth1;
    parser->GetDepth(&outerDepth);
    Int32 type;
    String temp;
    while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
            && (type != IXmlPullParser::END_TAG || (parser->GetDepth(&outerDepth1), outerDepth1) > outerDepth)) {
        if (type == IXmlPullParser::END_TAG || type == IXmlPullParser::TEXT) {
            continue;
        }

        if (type != IXmlPullParser::START_TAG) {
            continue;
        }

        String tagName;
        parser->GetName(&tagName);
        if (tagName.Equals(TAG_ITEM)) {
            String name;
            parser->GetAttributeValue(String(NULL), ATTR_NAME, &name);
            if (!name.IsNull()) {
                outPerms.Insert(name);
            } else {
                CPackageManagerService::ReportSettingsProblem(Logger::WARN,
                        String("Error in package manager settings: <perms> has") + String(" no name at ")
                                + (parser->GetPositionDescription(&temp), temp));
            }
        } else {
            CPackageManagerService::ReportSettingsProblem(Logger::WARN,
                    String("Unknown element under <perms>: ") + (parser->GetName(&temp), temp.IsNull() ? String("NULL") : temp));
        }
        XmlUtils::SkipCurrentTag(parser);
    }
    return NOERROR;
}

void Settings::CreateNewUserLILPw(
    /* [in] */ Installer* installer,
    /* [in] */ Int32 userHandle,
    /* [in] */ IFile* path)
{
    String temp;
    Boolean result;
    path->Mkdir(&result);
    path->ToString(&temp);
    FileUtils::SetPermissions(temp, FileUtils::sS_IRWXU | FileUtils::sS_IRWXG
            | FileUtils::sS_IXOTH, -1, -1);

    HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mPackages.Begin();
    for (; it != mPackages.End(); it++) {
        AutoPtr<PackageSetting> ps = it->mSecond;
        // Only system apps are initially installed.
        ps->SetInstalled((ps->mPkgFlags & IApplicationInfo::FLAG_SYSTEM) != 0, userHandle);
        // Need to create a data directory for all apps under this user.
        installer->CreateUserData(ps->mName,
                UserHandle::GetUid(userHandle, ps->mAppId), userHandle);
    }
    ReadDefaultPreferredAppsLPw(userHandle);
    WritePackageRestrictionsLPr(userHandle);
}

void Settings::RemoveUserLPr(
    /* [in] */ Int32 userId)
{
    HashMap<String, AutoPtr<PackageSetting> >::Iterator it;
    for (it = mPackages.Begin(); it != mPackages.End(); ++it) {
        it->mSecond->RemoveUser(userId);
    }
    mPreferredActivities.Erase(userId);
    AutoPtr<IFile> file = GetUserPackagesStateFile(userId);
    Boolean result;
    file->Delete(&result);
    file = GetUserPackagesStateBackupFile(userId);
    file->Delete(&result);
}

Int32 Settings::NewUserIdLPw(
    /* [in] */ IInterface* obj)
{
    // Let's be stupidly inefficient for now...
    Int32 i;
    List< AutoPtr<IInterface> >::Iterator it;
    for (it = mUserIds.Begin(), i = 0; it != mUserIds.End(); ++it, ++i) {
        if (*it == NULL) {
            *it = obj;
            return IProcess::FIRST_APPLICATION_UID + i;
        }
    }

    // None left?
    const Int32 N = mUserIds.GetSize();
    if (N > (IProcess::LAST_APPLICATION_UID - IProcess::FIRST_APPLICATION_UID)) {
        return -1;
    }

    mUserIds.PushBack(obj);
    return IProcess::FIRST_APPLICATION_UID + N;
}

AutoPtr<IVerifierDeviceIdentity> Settings::GetVerifierDeviceIdentityLPw()
{
    if (mVerifierDeviceIdentity == NULL) {
        AutoPtr<IVerifierDeviceIdentityHelper> helper;
        CVerifierDeviceIdentityHelper::AcquireSingleton((IVerifierDeviceIdentityHelper**)&helper);
        helper->Generate((IVerifierDeviceIdentity**)&mVerifierDeviceIdentity);

        WriteLPr();
    }

    return mVerifierDeviceIdentity;
}

AutoPtr<PackageSetting> Settings::GetDisabledSystemPkgLPr(
    /* [in] */ const String& name)
{
    AutoPtr<PackageSetting> ps;
    HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mDisabledSysPackages.Find(name);
    if (it != mDisabledSysPackages.End()) {
        ps = it->mSecond;
    }
    return ps;
}

String Settings::CompToString(
    /* [in] */ HashSet<String>* cmp)
{
    if (cmp != NULL) {
        Int32 size = cmp->GetSize();
        StringBuilder sb(size * 7);
        sb.AppendChar('[');
        HashSet<String>::Iterator it;
        for (it = cmp->Begin(); it != cmp->End(); ++it) {
            sb.AppendString(*it);
        }
        sb.AppendChar(']');
        return sb.ToString();
    }
    else {
        return String("[]");
    }
}

Boolean Settings::IsEnabledLPr(
    /* [in] */ IComponentInfo* componentInfo,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId)
{
    if ((flags & IPackageManager::GET_DISABLED_COMPONENTS) != 0) {
        return TRUE;
    }
    String pkgName;
    componentInfo->GetPackageName(&pkgName);
    String name;
    componentInfo->GetName(&name);
    AutoPtr<PackageSetting> packageSettings;
    HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mPackages.Find(pkgName);
    if (it != mPackages.End()) {
        packageSettings = it->mSecond;
    }
    if (CPackageManagerService::DEBUG_SETTINGS) {
        Slogger::V(CPackageManagerService::TAG, "isEnabledLock - packageName = %s  componentName = %s"
                ,pkgName.string(), name.string());
        /*Slogger::V(CPackageManagerService::TAG, "enabledComponents: "
                + compToString(packageSettings.getEnabledComponents(userId)));
        Slogger::V(CPackageManagerService::TAG, "disabledComponents: "
                + compToString(packageSettings.getDisabledComponents(userId)));*/
    }
    if (packageSettings == NULL) {
        return FALSE;
    }
    AutoPtr<PackageUserState> ustate = packageSettings->ReadUserState(userId);
    Boolean enabled;
    if (ustate->mEnabled == IPackageManager::COMPONENT_ENABLED_STATE_DISABLED
            || ustate->mEnabled == IPackageManager::COMPONENT_ENABLED_STATE_DISABLED_USER
            || (packageSettings->mPkg != NULL && (packageSettings->mPkg->mApplicationInfo->GetEnabled(&enabled), !enabled)
                && ustate->mEnabled == IPackageManager::COMPONENT_ENABLED_STATE_DEFAULT)) {
        return FALSE;
    }

    if (ustate->mEnabledComponents != NULL
            && ustate->mEnabledComponents->Find(name) != ustate->mEnabledComponents->End()) {
        return TRUE;
    }
    if (ustate->mDisabledComponents != NULL
            && ustate->mDisabledComponents->Find(name) != ustate->mDisabledComponents->End()) {
        return FALSE;
    }
    componentInfo->GetEnabled(&enabled);
    return enabled;
}

ECode Settings::GetInstallerPackageNameLPr(
    /* [in] */ const String& packageName,
    /* [out] */ String* name)
{
    assert(name != NULL);

    AutoPtr<PackageSetting> pkg;
    HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mPackages.Find(packageName);
    if (it != mPackages.End()) {
        pkg = it->mSecond;
    }
    if (pkg == NULL) {
        // throw new IllegalArgumentException("Unknown package: " + packageName);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *name = pkg->mInstallerPackageName;
    return NOERROR;
}

ECode Settings::GetApplicationEnabledSettingLPr(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 userId,
    /* [out] */ Int32* result)
{
    assert(result != NULL);

    AutoPtr<PackageSetting> pkg;
    HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mPackages.Find(packageName);
    if (it != mPackages.End()) {
        pkg = it->mSecond;
    }
    if (pkg == NULL) {
        // throw new IllegalArgumentException("Unknown package: " + packageName);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *result = pkg->GetEnabled(userId);
    return NOERROR;
}

ECode Settings::GetComponentEnabledSettingLPr(
    /* [in] */ IComponentName* componentName,
    /* [in] */ Int32 userId,
    /* [out] */ Int32* result)
{
    String packageName;
    componentName->GetPackageName(&packageName);
    AutoPtr<PackageSetting> pkg;
    HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mPackages.Find(packageName);
    if (it != mPackages.End()) {
        pkg = it->mSecond;
    }
    if (pkg == NULL) {
        // throw new IllegalArgumentException("Unknown component: " + componentName);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    String classNameStr;
    componentName->GetClassName(&classNameStr);
    *result = pkg->GetCurrentEnabledStateLPr(classNameStr, userId);
    return NOERROR;
}

ECode Settings::SetPackageStoppedStateLPw(
    /* [in] */ const String& packageName,
    /* [in] */ Boolean stopped,
    /* [in] */ Boolean allowedByPermission,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 userId,
    /* [out] */ Boolean* result)
{
    assert(result);
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    Int32 appId;
    helper->GetAppId(uid, &appId);
    AutoPtr<PackageSetting> pkgSetting;
    HashMap<String, AutoPtr<PackageSetting> >::Iterator it = mPackages.Find(packageName);
    if (it != mPackages.End()) {
        pkgSetting = it->mSecond;
    }
    if (pkgSetting == NULL) {
        // throw new IllegalArgumentException("Unknown package: " + packageName);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (!allowedByPermission && (appId != pkgSetting->mAppId)) {
        // throw new SecurityException(
        //         "Permission Denial: attempt to change stopped state from pid="
        //         + Binder.getCallingPid()
        //         + ", uid=" + uid + ", package uid=" + pkgSetting.appId);
        return E_SECURITY_EXCEPTION;
    }
//     if (DEBUG_STOPPED) {
//         if (stopped) {
//             RuntimeException e = new RuntimeException("here");
//             e.fillInStackTrace();
//             Slog.i(TAG, "Stopping package " + packageName, e);
//         }
//     }
    if (pkgSetting->GetStopped(userId) != stopped) {
        pkgSetting->SetStopped(stopped, userId);
        if (pkgSetting->GetNotLaunched(userId)) {
            if (!pkgSetting->mInstallerPackageName.IsNull()) {
                AutoPtr<ArrayOf<Int32> > iArray = ArrayOf<Int32>::Alloc(1);
                iArray->Set(0, userId);
                CPackageManagerService::SendPackageBroadcast(IIntent::ACTION_PACKAGE_FIRST_LAUNCH,
                       pkgSetting->mName, NULL,
                       pkgSetting->mInstallerPackageName, NULL, iArray);
            }
            pkgSetting->SetNotLaunched(FALSE, userId);
        }
        return TRUE;
    }
    return FALSE;
}

AutoPtr< List< AutoPtr<IUserInfo> > > Settings::GetAllUsers()
{
    Int64 id = Binder::ClearCallingIdentity();
    Int32 val;
    Boolean hasNext;
    ECode ecode = NOERROR;
    //try {
    AutoPtr<IObjectContainer> users;
    AutoPtr<IObjectEnumerator> objEnumerator;
    ecode = CUserManagerService::GetInstance()->GetUsers(FALSE, (IObjectContainer**)&users);
    Binder::RestoreCallingIdentity(id);
    users->GetObjectCount(&val);

    if (SUCCEEDED(ecode) && val > 0) {
        users->GetObjectEnumerator((IObjectEnumerator**)&objEnumerator);

        AutoPtr< List< AutoPtr<IUserInfo> > > usersList = new List< AutoPtr<IUserInfo> >();
        while(objEnumerator->MoveNext(&hasNext), hasNext) {
            AutoPtr<IUserInfo> userInfo;
            objEnumerator->Current((IInterface**)&userInfo);
            usersList->PushBack(userInfo);
        }

        return usersList;
    }
    // } catch (NullPointerException npe) {
    //     // packagemanager not yet initialized
    // } finally {
    //     Binder.restoreCallingIdentity(id);
    // }
    return NULL;
}

void Settings::PrintFlags(
    /* [in] */ IPrintWriter* pw,
    /* [in] */ Int32 val,
    /* [in] */ ArrayOf<IInterface*>& spec)
{
    pw->PrintString(String("[ "));
    for (Int32 i = 0; i < spec.GetLength(); i += 2) {
        AutoPtr<IInteger32> mask = (IInteger32*)spec[i]->Probe(EIID_IInteger32);
        Int32 maskValue;
        mask->GetValue(&maskValue);
        if ((val & maskValue) != 0) {
            pw->PrintObject(spec[i+1]);
            pw->PrintString(String(" "));
        }
    }
    pw->PrintString(String("]"));
}

void Settings::DumpPackagesLPr(
    /* [in] */ IPrintWriter* pw,
    /* [in] */ const String& packageName,
    /* [in] */ DumpState* dumpState)
{
    AutoPtr<ISimpleDateFormat> sdf;
    CSimpleDateFormat::New(String("yyyy-MM-dd HH:mm:ss"), (ISimpleDateFormat**)&sdf);
    AutoPtr<IDate> date;
    CDate::New((IDate**)&date);
    Boolean printedSomething = FALSE;
    AutoPtr<List<AutoPtr<IUserInfo> > > users = GetAllUsers();
    HashMap<String, AutoPtr<PackageSetting> >::Iterator it;
    for (it = mPackages.Begin(); it != mPackages.End(); ++it) {
        AutoPtr<PackageSetting> ps = it->mSecond;
        if (!packageName.IsNull() && !packageName.Equals(ps->mRealName)
                && !packageName.Equals(ps->mName)) {
            continue;
        }

        if (!packageName.IsNull()) {
            dumpState->SetSharedUser(ps->mSharedUser);
        }

        if (!printedSomething) {
            if (dumpState->OnTitlePrinted())
                pw->PrintStringln(String(" "));
            pw->PrintStringln(String("Packages:"));
            printedSomething = TRUE;
        }
        pw->PrintString(String("  Package ["));
            pw->PrintString(!ps->mRealName.IsNull() ? ps->mRealName : ps->mName);
            pw->PrintString(String("] ("));
            pw->PrintInt32((Int32)ps.Get());
            pw->PrintStringln(String("):"));

        if (!ps->mRealName.IsNull()) {
            pw->PrintString(String("    compat name="));
            pw->PrintStringln(ps->mName);
        }

        pw->PrintString(String("    userId=")); pw->PrintInt32(ps->mAppId);
        pw->PrintString(String(" gids=")); pw->PrintStringln(CPackageManagerService::ArrayToString(ps->mGids));
        pw->PrintString(String("    sharedUser=")); pw->PrintObjectln(ps->mSharedUser);
        pw->PrintString(String("    pkg=")); pw->PrintObjectln(ps->mPkg);
        pw->PrintString(String("    codePath=")); pw->PrintStringln(ps->mCodePathString);
        pw->PrintString(String("    resourcePath=")); pw->PrintStringln(ps->mResourcePathString);
        pw->PrintString(String("    nativeLibraryPath=")); pw->PrintStringln(ps->mNativeLibraryPathString);
        pw->PrintString(String("    versionCode=")); pw->PrintInt32ln(ps->mVersionCode);
        if (ps->mPkg != NULL) {
            pw->PrintString(String("    applicationInfo="));
            String appInfoStr;
            ps->mPkg->mApplicationInfo->ToString(&appInfoStr);
            pw->PrintStringln(appInfoStr);
            Int32 flags;
            ps->mPkg->mApplicationInfo->GetFlags(&flags);
            pw->PrintString(String("    flags="));
            // PrintFlags(pw, flags, FLAG_DUMP_SPEC);
            pw->Println();
            pw->PrintString(String("    versionName="));
            pw->PrintStringln(ps->mPkg->mVersionName);
            pw->PrintString(String("    dataDir="));
            String dataDir;
            ps->mPkg->mApplicationInfo->GetDataDir(&dataDir);
            pw->PrintStringln(dataDir);
            pw->PrintString(String("    targetSdk="));
            Int32 targetSdkVersion;
            ps->mPkg->mApplicationInfo->GetTargetSdkVersion(&targetSdkVersion);
            pw->PrintInt32ln(targetSdkVersion);
            if (ps->mPkg->mOperationPending) {
                pw->PrintStringln(String("    mOperationPending=true"));
            }
            pw->PrintString(String("    supportsScreens=["));
            Boolean first = TRUE;
            if ((flags & IApplicationInfo::FLAG_SUPPORTS_SMALL_SCREENS) != 0) {
                if (!first)
                    pw->PrintString(String(", "));
                first = FALSE;
                pw->PrintString(String("small"));
            }
            if ((flags & IApplicationInfo::FLAG_SUPPORTS_NORMAL_SCREENS) != 0) {
                if (!first)
                    pw->PrintString(String(", "));
                first = FALSE;
                pw->PrintString(String("medium"));
            }
            if ((flags & IApplicationInfo::FLAG_SUPPORTS_LARGE_SCREENS) != 0) {
                if (!first)
                    pw->PrintString(String(", "));
                first = FALSE;
                pw->PrintString(String("large"));
            }
            if ((flags & IApplicationInfo::FLAG_SUPPORTS_XLARGE_SCREENS) != 0) {
                if (!first)
                    pw->PrintString(String(", "));
                first = FALSE;
                pw->PrintString(String("xlarge"));
            }
            if ((flags & IApplicationInfo::FLAG_RESIZEABLE_FOR_SCREENS) != 0) {
                if (!first)
                    pw->PrintString(String(", "));
                first = FALSE;
                pw->PrintString(String("resizeable"));
            }
            if ((flags & IApplicationInfo::FLAG_SUPPORTS_SCREEN_DENSITIES) != 0) {
                if (!first)
                    pw->PrintString(String(", "));
                first = FALSE;
                pw->PrintString(String("anyDensity"));
            }
            pw->PrintStringln(String("]"));
        }
        pw->PrintString(String("    timeStamp="));
            date->SetTime(ps->mTimeStamp);
            String dStr;
            sdf->FormatDate(date, &dStr);
            pw->PrintStringln(dStr);
        pw->PrintString(String("    firstInstallTime="));
            date->SetTime(ps->mFirstInstallTime);
            pw->PrintStringln(dStr);
        pw->PrintString(String("    lastUpdateTime="));
            date->SetTime(ps->mLastUpdateTime);
            pw->PrintStringln(dStr);
        if (!ps->mInstallerPackageName.IsNull()) {
            pw->PrintString(String("    installerPackageName=")); pw->PrintStringln(ps->mInstallerPackageName);
        }
        pw->PrintString(String("    signatures=")); pw->PrintInt32ln((Handle32)ps->mSignatures.Get());
        pw->PrintString(String("    permissionsFixed=")); pw->PrintBoolean(ps->mPermissionsFixed);
                pw->PrintString(String(" haveGids=")); pw->PrintBoolean(ps->mHaveGids);
                pw->PrintString(String(" installStatus=")); pw->PrintInt32ln(ps->mInstallStatus);
        pw->PrintString(String("    pkgFlags="));
        // PrintFlags(pw, ps->mPkgFlags, FLAG_DUMP_SPEC);
                pw->Println();
        List<AutoPtr<IUserInfo> >::Iterator uIt;
        for (uIt = users->Begin(); uIt != users->End(); ++uIt) {
            AutoPtr<IUserInfo> user = *uIt;
            pw->PrintString(String("    User "));
            Int32 id;
            user->GetId(&id);
            pw->PrintInt32(id); pw->PrintString(String(": "));
            pw->PrintString(String(" installed="));
            pw->PrintBoolean(ps->GetInstalled(id));
            pw->PrintString(String(" stopped="));
            pw->PrintBoolean(ps->GetStopped(id));
            pw->PrintString(String(" notLaunched="));
            pw->PrintBoolean(ps->GetNotLaunched(id));
            pw->PrintString(String(" enabled="));
            pw->PrintInt32ln(ps->GetEnabled(id));
            AutoPtr<HashSet<String> > cmp = ps->GetDisabledComponents(id);
            if (cmp != NULL && cmp->Begin() != cmp->End()) {
                pw->PrintStringln(String("      disabledComponents:"));
                HashSet<String>::Iterator cmpIt;
                for (cmpIt = cmp->Begin(); cmpIt != cmp->End(); ++cmpIt) {
                    String s = *cmpIt;
                    pw->PrintString(String("      ")); pw->PrintStringln(s);
                }
            }
            cmp = ps->GetEnabledComponents(id);
            if (cmp != NULL && cmp->Begin() != cmp->End()) {
                pw->PrintStringln(String("      enabledComponents:"));
                HashSet<String>::Iterator cmpIt;
                for (cmpIt = cmp->Begin(); cmpIt != cmp->End(); ++cmpIt) {
                    String s = *cmpIt;
                    pw->PrintString(String("      ")); pw->PrintStringln(s);
                }
            }
        }
        if (ps->mGrantedPermissions.Begin() != ps->mGrantedPermissions.End()) {
            pw->PrintStringln(String("    grantedPermissions:"));
            HashSet<String>::Iterator sIt;
            for (sIt = ps->mGrantedPermissions.Begin(); sIt != ps->mGrantedPermissions.End(); ++sIt) {
                String s = *sIt;
                pw->PrintString(String("      ")); pw->PrintStringln(s);
            }
        }
    }

    printedSomething = FALSE;
    if (mRenamedPackages.Begin() != mRenamedPackages.End()) {
        HashMap<String, String>::Iterator eIt;
        for (eIt = mRenamedPackages.Begin(); eIt != mRenamedPackages.End(); ++eIt) {
            if (!packageName.IsNull() && !packageName.Equals(eIt->mFirst)
                    && !packageName.Equals(eIt->mSecond)) {
                continue;
            }
            if (!printedSomething) {
                if (dumpState->OnTitlePrinted())
                    pw->PrintStringln(String(" "));
                pw->PrintStringln(String("Renamed packages:"));
                printedSomething = TRUE;
            }
            pw->PrintString(String("  "));
            pw->PrintString(eIt->mFirst);
            pw->PrintString(String(" -> "));
            pw->PrintStringln(eIt->mSecond);
        }
    }

    printedSomething = FALSE;
    if (mDisabledSysPackages.Begin() != mDisabledSysPackages.End()) {
        HashMap<String, AutoPtr<PackageSetting> >::Iterator psIt;
        for (psIt = mDisabledSysPackages.Begin(); psIt != mDisabledSysPackages.End(); ++psIt) {
            AutoPtr<PackageSetting> ps = psIt->mSecond;
            if (!packageName.IsNull() && !packageName.Equals(ps->mRealName)
                    && !packageName.Equals(ps->mName)) {
                continue;
            }
            if (!printedSomething) {
                if (dumpState->OnTitlePrinted())
                    pw->PrintStringln(String(" "));
                pw->PrintStringln(String("Hidden system packages:"));
                printedSomething = TRUE;
            }
            pw->PrintString(String("  Package ["));
            pw->PrintString(!ps->mRealName.IsNull() ? ps->mRealName : ps->mName);
            pw->PrintString(String("] ("));
            pw->PrintInt32((Int32)ps.Get());
            pw->PrintStringln(String("):"));
            if (!ps->mRealName.IsNull()) {
                pw->PrintString(String("    compat name="));
                pw->PrintStringln(ps->mName);
            }
            if (ps->mPkg != NULL && ps->mPkg->mApplicationInfo != NULL) {
                pw->PrintString(String("    applicationInfo="));
                String aStr;
                ps->mPkg->mApplicationInfo->ToString(&aStr);
                pw->PrintStringln(aStr);
            }
            pw->PrintString(String("    userId="));
            pw->PrintInt32ln(ps->mAppId);
            pw->PrintString(String("    sharedUser="));
            pw->PrintObjectln(ps->mSharedUser);
            pw->PrintString(String("    codePath="));
            pw->PrintStringln(ps->mCodePathString);
            pw->PrintString(String("    resourcePath="));
            pw->PrintStringln(ps->mResourcePathString);
        }
    }
}

void Settings::DumpPermissionsLPr(
    /* [in] */ IPrintWriter* pw,
    /* [in] */ const String& packageName,
    /* [in] */ DumpState* dumpState)
{
//     boolean printedSomething = false;
//     for (BasePermission p : mPermissions.values()) {
//         if (packageName != null && !packageName.equals(p.sourcePackage)) {
//             continue;
//         }
//         if (!printedSomething) {
//             if (dumpState.onTitlePrinted())
//                 pw.println(" ");
//             pw.println("Permissions:");
//             printedSomething = true;
//         }
//         pw.print("  Permission ["); pw.print(p.name); pw.print("] (");
//                 pw.print(Integer.toHexString(System.identityHashCode(p)));
//                 pw.println("):");
//         pw.print("    sourcePackage="); pw.println(p.sourcePackage);
//         pw.print("    uid="); pw.print(p.uid);
//                 pw.print(" gids="); pw.print(PackageManagerService.arrayToString(p.gids));
//                 pw.print(" type="); pw.print(p.type);
//                 pw.print(" prot=");
//                 pw.println(PermissionInfo.protectionToString(p.protectionLevel));
//         if (p.packageSetting != null) {
//             pw.print("    packageSetting="); pw.println(p.packageSetting);
//         }
//         if (p.perm != null) {
//             pw.print("    perm="); pw.println(p.perm);
//         }
//         if (READ_EXTERNAL_STORAGE.equals(p.name)) {
//             pw.print("    enforced=");
//             pw.println(mReadExternalStorageEnforced);
//         }
//     }
    assert(0);
}

void Settings::DumpSharedUsersLPr(
    /* [in] */ IPrintWriter* pw,
    /* [in] */ const String& packageName,
    /* [in] */ DumpState* dumpState)
{
//     boolean printedSomething = false;
//     for (SharedUserSetting su : mSharedUsers.values()) {
//         if (packageName != null && su != dumpState.getSharedUser()) {
//             continue;
//         }
//         if (!printedSomething) {
//             if (dumpState.onTitlePrinted())
//                 pw.println(" ");
//             pw.println("Shared users:");
//             printedSomething = true;
//         }
//         pw.print("  SharedUser [");
//         pw.print(su.name);
//         pw.print("] (");
//         pw.print(Integer.toHexString(System.identityHashCode(su)));
//                 pw.println("):");
//         pw.print("    userId=");
//         pw.print(su.userId);
//         pw.print(" gids=");
//         pw.println(PackageManagerService.arrayToString(su.gids));
//         pw.println("    grantedPermissions:");
//         for (String s : su.grantedPermissions) {
//             pw.print("      ");
//             pw.println(s);
//         }
//     }
    assert(0);
}

void Settings::DumpReadMessagesLPr(
    /* [in] */ IPrintWriter* pw,
    /* [in] */ DumpState* dumpState)
{
//     pw.println("Settings parse messages:");
//     pw.print(mReadMessages.toString());
    assert(0);
}

} // namespace Pm
} // namespace Server
} // namespace Droid
} // namespace Elastos
