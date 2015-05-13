#ifndef __ELASTOS_DROID_SERVER_PM_SETTINGS_H__
#define __ELASTOS_DROID_SERVER_PM_SETTINGS_H__

#include "Elastos.Droid.Server_server.h"
#include "pm/PackageSetting.h"
#include "pm/SharedUserSetting.h"
#include "pm/PreferredIntentResolver.h"
#include "pm/BasePermission.h"
#include "pm/Installer.h"
#include "pm/DumpState.h"
#include "pm/PendingPackage.h"
#include "content/pm/PackageParser.h"
#include <elastos/StringBuilder.h>

using Elastos::Core::IBoolean;
using Elastos::Core::StringBuilder;
using Elastos::IO::IFile;

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Pm::PackageParser;
using Elastos::Droid::Content::Pm::IPackageCleanItem;
using Elastos::Droid::Content::Pm::IUserInfo;
using Elastos::Droid::Content::Pm::IVerifierDeviceIdentity;
using Elastos::Droid::Content::Pm::IComponentInfo;
using Elastos::Droid::Os::IUserHandle;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {

class Settings : public ElRefBase
{
public:
    Settings(
        /* [in] */ IContext* context);

    Settings(
        /* [in] */ IContext* context,
        /* [in] */ IFile* dataDir);

    CARAPI_(AutoPtr<PackageSetting>) GetPackageLPw(
        /* [in] */ PackageParser::Package* pkg,
        /* [in] */ PackageSetting* origPackage,
        /* [in] */ const String& realName,
        /* [in] */ SharedUserSetting* sharedUser,
        /* [in] */ IFile* codePath,
        /* [in] */ IFile* resourcePath,
        /* [in] */ const String& nativeLibraryPathString,
        /* [in] */ Int32 pkgFlags,
        /* [in] */ IUserHandle* user,
        /* [in] */ Boolean add);

    CARAPI_(AutoPtr<PackageSetting>) PeekPackageLPr(
        /* [in] */ const String& name);

    CARAPI_(void) SetInstallStatus(
        /* [in] */ const String& pkgName,
        /* [in] */ Int32 status);

    CARAPI_(void) SetInstallerPackageName(
        /* [in] */ const String& pkgName,
        /* [in] */ const String& installerPkgName);

    CARAPI_(AutoPtr<SharedUserSetting>) GetSharedUserLPw(
        /* [in] */ const String& name,
        /* [in] */ Int32 pkgFlags,
        /* [in] */ Boolean create);

    CARAPI_(Boolean) DisableSystemPackageLPw(
        /* [in] */ const String& name);

    CARAPI_(AutoPtr<PackageSetting>) EnableSystemPackageLPw(
        /* [in] */ const String& name);

    CARAPI_(Boolean) IsDisabledSystemPackageLPr(
        /* [in] */ const String& name);

    CARAPI_(void) RemoveDisabledSystemPackageLPw(
        /* [in] */ const String& name);

    CARAPI_(AutoPtr<PackageSetting>) AddPackageLPw(
        /* [in] */ const String& name,
        /* [in] */ const String& realName,
        /* [in] */ IFile* codePath,
        /* [in] */ IFile* resourcePath,
        /* [in] */ const String& nativeLibraryPathString,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 vc,
        /* [in] */ Int32 pkgFlags);

    CARAPI_(AutoPtr<SharedUserSetting>) AddSharedUserLPw(
        /* [in] */ const String& name,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 pkgFlags);

    // Transfer ownership of permissions from one package to another.
    CARAPI_(void) TransferPermissionsLPw(
        /* [in] */ const String& origPkg,
        /* [in] */ const String& newPkg);

    CARAPI_(void) InsertPackageSettingLPw(
        /* [in] */ PackageSetting* p,
        /* [in] */ PackageParser::Package* pkg);

    /*
     * Update the shared user setting when a package using
     * specifying the shared user id is removed. The gids
     * associated with each permission of the deleted package
     * are removed from the shared user's gid list only if its
     * not in use by other permissions of packages in the
     * shared user setting.
     */
    CARAPI_(void) UpdateSharedUserPermsLPw(
        /* [in] */ PackageSetting* deletedPs,
        /* [in] */ ArrayOf<Int32>* globalGids);

    CARAPI_(Int32) RemovePackageLPw(
        /* [in] */ const String& name);

    CARAPI_(AutoPtr<IInterface>) GetUserIdLPr(
        /* [in] */ Int32 uid);

    CARAPI_(AutoPtr<PreferredIntentResolver>) EditPreferredActivitiesLPw(
        /* [in] */ Int32 userId);

    CARAPI_(void) WriteAllUsersPackageRestrictionsLPr();

    CARAPI_(void) ReadAllUsersPackageRestrictionsLPr();

    CARAPI_(void) ReadPackageRestrictionsLPr(
        /* [in] */ Int32 userId);

    CARAPI WritePreferredActivitiesLPr(
        /* [in] */ IXmlSerializer* serializer,
        /* [in] */ Int32 userId);

    CARAPI_(void) WritePackageRestrictionsLPr(
        /* [in] */ Int32 userId);

    // Note: assumed "stopped" field is already cleared in all packages.
    // Legacy reader, used to read in the old file format after an upgrade. Not used after that.
    CARAPI_(void) ReadStoppedLPw();

    CARAPI_(void) WriteLPr();

    CARAPI WriteDisabledSysPackageLPr(
        /* [in] */ IXmlSerializer* serializer,
        /* [in] */ PackageSetting* pkg);

    CARAPI WritePackageLPr(
        /* [in] */ IXmlSerializer* serializer,
        /* [in] */ PackageSetting* pkg);

    CARAPI WritePermissionLPr(
        /* [in] */ IXmlSerializer* serializer,
        /* [in] */ BasePermission* bp);

    CARAPI_(AutoPtr< List< AutoPtr<PackageSetting> > >) GetListOfIncompleteInstallPackagesLPr();

    CARAPI_(void) AddPackageToCleanLPw(
        /* [in] */ IPackageCleanItem* pkg);

    CARAPI_(Boolean) ReadLPw(
        /* [in] */ List< AutoPtr<IUserInfo> >& users,
        /* [in] */ Int32 sdkVersion,
        /* [in] */ Boolean onlyCore);

    CARAPI_(void) CreateNewUserLILPw(
        /* [in] */ Installer* installer,
        /* [in] */ Int32 userHandle,
        /* [in] */ IFile* path);

    CARAPI_(void) RemoveUserLPr(
        /* [in] */ Int32 userId);

    CARAPI_(AutoPtr<IVerifierDeviceIdentity>) GetVerifierDeviceIdentityLPw();

    CARAPI_(AutoPtr<PackageSetting>) GetDisabledSystemPkgLPr(
        /* [in] */ const String& name);

    CARAPI_(Boolean) IsEnabledLPr(
        /* [in] */ IComponentInfo* componentInfo,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId);

    CARAPI GetInstallerPackageNameLPr(
        /* [in] */ const String& packageName,
        /* [out] */ String* name);

    CARAPI GetApplicationEnabledSettingLPr(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 userId,
        /* [out] */ Int32* result);

    CARAPI GetComponentEnabledSettingLPr(
        /* [in] */ IComponentName* componentName,
        /* [in] */ Int32 userId,
        /* [out] */ Int32* result);

    CARAPI SetPackageStoppedStateLPw(
        /* [in] */ const String& packageName,
        /* [in] */ Boolean stopped,
        /* [in] */ Boolean allowedByPermission,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 userId,
        /* [out] */ Boolean* result);

    static CARAPI_(void) PrintFlags(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ Int32 val,
        /* [in] */ ArrayOf<IInterface*>& spec);

    CARAPI_(void) DumpPackagesLPr(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& packageName,
        /* [in] */ DumpState* dumpState);

    CARAPI_(void) DumpPermissionsLPr(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& packageName,
        /* [in] */ DumpState* dumpState);

    CARAPI_(void) DumpSharedUsersLPr(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& packageName,
        /* [in] */ DumpState* dumpState);

    CARAPI_(void) DumpReadMessagesLPr(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ DumpState* dumpState);

private:
    CARAPI_(void) Init(
        /* [in] */ IContext* context,
        /* [in] */ IFile* dataDir);

    CARAPI_(AutoPtr<PackageSetting>) GetPackageLPw(
        /* [in] */ const String& name,
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
        /* [in] */ Boolean allowInstall);

    // Utility method that adds a PackageSetting to mPackages and
    // completes updating the shared user attributes
    CARAPI_(void) AddPackageSettingLPw(
        /* [in] */ PackageSetting* p,
        /* [in] */ const String& name,
        /* [in] */ SharedUserSetting* sharedUser);

    CARAPI_(void) ReplacePackageLPw(
        /* [in] */ const String& name,
        /* [in] */ PackageSetting* newp);

    CARAPI_(Boolean) AddUserIdLPw(
        /* [in] */ Int32 uid,
        /* [in] */ IInterface* obj,
        /* [in] */ IInterface* name);

    CARAPI_(void) RemoveUserIdLPw(
        /* [in] */ Int32 uid);

    CARAPI_(void) ReplaceUserIdLPw(
        /* [in] */ Int32 uid,
        /* [in] */ IInterface* obj);

    CARAPI_(AutoPtr<IFile>) GetUserPackagesStateFile(
        /* [in] */ Int32 userId);

    CARAPI_(AutoPtr<IFile>) GetUserPackagesStateBackupFile(
        /* [in] */ Int32 userId);

    CARAPI ReadPreferredActivitiesLPw(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ Int32 userId);

    CARAPI_(AutoPtr<HashSet<Elastos::String> >) ReadComponentsLPr(
        /* [in] */ IXmlPullParser* parser);

    CARAPI_(void) ReadDefaultPreferredAppsLPw(
        /* [in] */ Int32 userId);

    CARAPI_(Int32) ReadInt32(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ const String& ns,
        /* [in] */ const String& name,
        /* [in] */ Int32 defValue);

    CARAPI ReadPermissionsLPw(
        /* [in] */ HashMap<String, AutoPtr<BasePermission> >& out,
        /* [in] */ IXmlPullParser* parser);

    CARAPI ReadDisabledSysPackageLPw(
        /* [in] */ IXmlPullParser* parser);

    CARAPI ReadPackageLPw(
        /* [in] */ IXmlPullParser* parser);

    CARAPI ReadDisabledComponentsLPw(
        /* [in] */ PackageSettingBase* packageSetting,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ Int32 userId);

    CARAPI ReadEnabledComponentsLPw(
        /* [in] */ PackageSettingBase* packageSetting,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ Int32 userId);

    CARAPI ReadSharedUserLPw(
        /* [in] */ IXmlPullParser* parser);

    CARAPI ReadGrantedPermissionsLPw(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ HashSet<String>& outPerms);

    // Returns -1 if we could not find an available UserId to assign
    CARAPI_(Int32) NewUserIdLPw(
        /* [in] */ IInterface* obj);

    CARAPI_(String) CompToString(
        /* [in] */ HashSet<String>* cmp);

    CARAPI_(AutoPtr< List< AutoPtr<IUserInfo> > >) GetAllUsers();

public:
    HashMap<String, AutoPtr<PackageSetting> > mPackages;

    // These are the last platform API version we were using for
    // the apps installed on internal and external storage.  It is
    // used to grant newer permissions one time during a system upgrade.
    Int32 mInternalSdkPlatform;
    Int32 mExternalSdkPlatform;

    AutoPtr<IBoolean> mReadExternalStorageEnforced;

    // The user's preferred activities associated with particular intent
    // filters.
    HashMap<Int32, AutoPtr<PreferredIntentResolver> > mPreferredActivities;

    HashMap<String, AutoPtr<SharedUserSetting> > mSharedUsers;

    // Mapping from permission names to info about them.
    HashMap<String, AutoPtr<BasePermission> > mPermissions;

    // Mapping from permission tree names to info about them.
    HashMap<String, AutoPtr<BasePermission> > mPermissionTrees;

    // Packages that have been uninstalled and still need their external
    // storage data deleted.
    List< AutoPtr<IPackageCleanItem> > mPackagesToBeCleaned;

    // Packages that have been renamed since they were first installed.
    // Keys are the new names of the packages, values are the original
    // names.  The packages appear everwhere else under their original
    // names.
    HashMap<String, String> mRenamedPackages;

    AutoPtr<StringBuilder> mReadMessages;

private:
    static const String TAG;

    static const Boolean DEBUG_STOPPED = FALSE;
    static const Boolean DEBUG_MU = FALSE;

    static const String TAG_READ_EXTERNAL_STORAGE;
    static const String ATTR_ENFORCEMENT;

    static const String TAG_ITEM;
    static const String TAG_DISABLED_COMPONENTS;
    static const String TAG_ENABLED_COMPONENTS;
    static const String TAG_PACKAGE_RESTRICTIONS;
    static const String TAG_PACKAGE;

    static const String ATTR_NAME;
    static const String ATTR_USER;
    static const String ATTR_CODE;
    static const String ATTR_NOT_LAUNCHED;
    static const String ATTR_ENABLED;
    static const String ATTR_STOPPED;
    static const String ATTR_INSTALLED;

    AutoPtr<IFile> mSettingsFilename;
    AutoPtr<IFile> mBackupSettingsFilename;
    AutoPtr<IFile> mPackageListFilename;
    AutoPtr<IFile> mStoppedPackagesFilename;
    AutoPtr<IFile> mBackupStoppedPackagesFilename;

    // List of replaced system applications
    HashMap<String, AutoPtr<PackageSetting> > mDisabledSysPackages;

    /** Device identity for the purpose of package verification. */
    AutoPtr<IVerifierDeviceIdentity> mVerifierDeviceIdentity;

    List< AutoPtr<IInterface> > mUserIds;
    HashMap<Int32, AutoPtr<IInterface> > mOtherUserIds;

    // For reading/writing settings file.
    List< AutoPtr<ISignature> > mPastSignatures;

    /**
     * Used to track packages that have a shared user ID that hasn't been read
     * in yet.
     * <p>
     * TODO: make this just a local variable that is passed in during package
     * scanning to make it less confusing.
     */
    List< AutoPtr<PendingPackage> > mPendingPackages;

    AutoPtr<IContext> mContext;

    AutoPtr<IFile> mSystemDir;
};

} // namespace Pm
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_PM_SETTINGS_H__
