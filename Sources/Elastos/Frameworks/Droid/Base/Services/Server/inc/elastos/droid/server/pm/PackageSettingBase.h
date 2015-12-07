
#ifndef __ELASTOS_DROID_SERVER_PM_PACKAGESETTINGBASE_H__
#define __ELASTOS_DROID_SERVER_PM_PACKAGESETTINGBASE_H__

#include "elastos/droid/ext/frameworkext.h"
#include "pm/GrantedPermissions.h"
#include "pm/PackageSignatures.h"
#include "elastos/droid/content/pm/PackageUserState.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;
using Elastos::IO::IFile;

using Elastos::Droid::Content::Pm::PackageUserState;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {

class PackageSettingBase : public GrantedPermissions
{
public:
    PackageSettingBase(
        /* [in] */ const String& name,
        /* [in] */ const String& realName,
        /* [in] */ IFile* codePath,
        /* [in] */ IFile* resourcePath,
        /* [in] */ const String& nativeLibraryPathString,
        /* [in] */ Int32 pVersionCode,
        /* [in] */ Int32 pkgFlags);

    PackageSettingBase(
        /* [in] */ const PackageSettingBase* base);

    CARAPI_(void) Init(
        /* [in] */ IFile* codePath,
        /* [in] */ IFile* resourcePath,
        /* [in] */ const String& nativeLibraryPathString,
        /* [in] */ Int32 pVersionCode);

    CARAPI_(void) SetInstallerPackageName(
        /* [in] */ const String& packageName);

    CARAPI_(String) GetInstallerPackageName();

    CARAPI_(void) SetInstallStatus(
        /* [in] */ Int32 newStatus);

    CARAPI_(Int32) GetInstallStatus();

    CARAPI_(void) SetTimeStamp(
        /* [in] */ Int64 newStamp);

    CARAPI_(void) CopyFrom(
        /* [in] */ PackageSettingBase* base);

    CARAPI_(AutoPtr<PackageUserState>) ReadUserState(
        /* [in] */ Int32 userId);

    CARAPI_(void) SetEnabled(
        /* [in] */ Int32 state,
        /* [in] */ Int32 userId);

    CARAPI_(Int32) GetEnabled(
        /* [in] */ Int32 userId);

    CARAPI_(void) SetInstalled(
        /* [in] */ Boolean inst,
        /* [in] */ Int32 userId);

    CARAPI_(Boolean) GetInstalled(
        /* [in] */ Int32 userId);

    CARAPI_(Boolean) IsAnyInstalled(
        /* [in] */ const ArrayOf<Int32>& users);

    CARAPI_(AutoPtr< ArrayOf<Int32> >) QueryInstalledUsers(
        /* [in] */ const ArrayOf<Int32>& users,
        /* [in] */ Boolean installed);

    CARAPI_(Boolean) GetStopped(
        /* [in] */ Int32 userId);

    CARAPI_(void) SetStopped(
        /* [in] */ Boolean stop,
        /* [in] */ Int32 userId);

    CARAPI_(Boolean) GetNotLaunched(
        /* [in] */ Int32 userId);

    CARAPI_(void) SetNotLaunched(
        /* [in] */ Boolean stop,
        /* [in] */ Int32 userId);

    CARAPI_(void) SetUserState(
        /* [in] */ Int32 userId,
        /* [in] */ Int32 enabled,
        /* [in] */ Boolean installed,
        /* [in] */ Boolean stopped,
        /* [in] */ Boolean notLaunched,
        /* [in] */ HashSet<String>* enabledComponents,
        /* [in] */ HashSet<String>* disabledComponents);

    CARAPI_(AutoPtr<HashSet<Elastos::String> >) GetEnabledComponents(
        /* [in] */ Int32 userId);

    CARAPI_(AutoPtr<HashSet<Elastos::String> >) GetDisabledComponents(
        /* [in] */ Int32 userId);

    CARAPI_(void) SetEnabledComponents(
        /* [in] */ HashSet<String>* components,
        /* [in] */ Int32 userId);

    CARAPI_(void) SetDisabledComponents(
        /* [in] */ HashSet<String>* components,
        /* [in] */ Int32 userId);

    CARAPI_(void) SetEnabledComponentsCopy(
        /* [in] */ HashSet<String>* components,
        /* [in] */ Int32 userId);

    CARAPI_(void) SetDisabledComponentsCopy(
        /* [in] */ HashSet<String>* components,
        /* [in] */ Int32 userId);

    CARAPI_(AutoPtr<PackageUserState>) ModifyUserStateComponents(
        /* [in] */ Int32 userId,
        /* [in] */ Boolean disabled,
        /* [in] */ Boolean enabled);

    CARAPI_(void) AddDisabledComponent(
        /* [in] */ const String& componentClassName,
        /* [in] */ Int32 userId);

    CARAPI_(void) AddEnabledComponent(
        /* [in] */ const String& componentClassName,
        /* [in] */ Int32 userId);

    CARAPI_(Boolean) EnableComponentLPw(
        /* [in] */ const String& componentClassName,
        /* [in] */ Int32 userId);

    CARAPI_(Boolean) DisableComponentLPw(
        /* [in] */ const String& componentClassName,
        /* [in] */ Int32 userId);

    CARAPI_(Boolean) RestoreComponentLPw(
        /* [in] */ const String& componentClassName,
        /* [in] */ Int32 userId);

    CARAPI_(Int32) GetCurrentEnabledStateLPr(
        /* [in] */ const String& componentName,
        /* [in] */ Int32 userId);

    CARAPI_(void) RemoveUser(
        /* [in] */ Int32 userId);

private:
    CARAPI_(AutoPtr<PackageUserState>) ModifyUserState(
        /* [in] */ Int32 userId);

public:
    /**
     * Indicates the state of installation. Used by PackageManager to figure out
     * incomplete installations. Say a package is being installed (the state is
     * set to PKG_INSTALL_INCOMPLETE) and remains so till the package
     * installation is successful or unsuccessful in which case the
     * PackageManager will no longer maintain state information associated with
     * the package. If some exception(like device freeze or battery being pulled
     * out) occurs during installation of a package, the PackageManager needs
     * this information to clean up the previously failed installation.
     */
    static const Int32 PKG_INSTALL_COMPLETE = 1;
    static const Int32 PKG_INSTALL_INCOMPLETE = 0;

    String mName;
    String mRealName;
    AutoPtr<IFile> mCodePath;
    String mCodePathString;
    AutoPtr<IFile> mResourcePath;
    String mResourcePathString;
    String mNativeLibraryPathString;
    Int64 mTimeStamp;
    Int64 mFirstInstallTime;
    Int64 mLastUpdateTime;
    Int32 mVersionCode;

    Boolean mUidError;

    AutoPtr<PackageSignatures> mSignatures;

    Boolean mPermissionsFixed;
    Boolean mHaveGids;

    Int32 mInstallStatus;

    AutoPtr<PackageSettingBase> mOrigPackage;

    /* package name of the app that installed this package */
    String mInstallerPackageName;

private:
    static const AutoPtr<PackageUserState> DEFAULT_USER_STATE;

    // Whether this package is currently stopped, thus can not be
    // started until explicitly launched by the user.
    HashMap<Int32, AutoPtr<PackageUserState> > mUserState;
};

} // namespace Pm
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_PM_PACKAGESETTINGBASE_H__
