
#ifndef __ELASTOS_DROID_APP_APPLICATIONPACKAGEMANAGER_H__
#define __ELASTOS_DROID_APP_APPLICATIONPACKAGEMANAGER_H__

#include "Elastos.Droid.Core_server.h"
#include "elastos/droid/app/CContextImpl.h"
#include "elastos/droid/content/pm/PackageManager.h"

using Elastos::Core::ICharSequence;

using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::IIntentSender;
using Elastos::Droid::Content::Pm::IIPackageManager;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Pm::PackageManager;
using Elastos::Droid::Content::Pm::IPackageInfo;
using Elastos::Droid::Content::Pm::IPermissionInfo;
using Elastos::Droid::Content::Pm::IPermissionGroupInfo;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::Pm::IActivityInfo;
using Elastos::Droid::Content::Pm::IServiceInfo;
using Elastos::Droid::Content::Pm::IProviderInfo;
using Elastos::Droid::Content::Pm::IFeatureInfo;
using Elastos::Droid::Content::Pm::IPermissionInfo;
using Elastos::Droid::Content::Pm::IResolveInfo;
using Elastos::Droid::Content::Pm::IComponentInfo;
using Elastos::Droid::Content::Pm::IInstrumentationInfo;
using Elastos::Droid::Content::Pm::IPackageInstallObserver;
using Elastos::Droid::Content::Pm::IPackageMoveObserver;
using Elastos::Droid::Content::Pm::IPackageDeleteObserver;
using Elastos::Droid::Content::Pm::IPackageDataObserver;
using Elastos::Droid::Content::Pm::IPackageStatsObserver;
using Elastos::Droid::Content::Pm::IManifestDigest;
using Elastos::Droid::Content::Pm::IContainerEncryptionParams;
using Elastos::Droid::Content::Pm::IVerificationParams;
using Elastos::Droid::Content::Pm::IVerifierDeviceIdentity;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::IXmlResourceParser;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace App {

class ApplicationPackageManager
    : public ElRefBase
    , public PackageManager
    , public IPackageManager
{
public:
    class ResourceName: public ElRefBase
    {
    public:
        ResourceName(
            /* [in] */ const String& packageName,
            /* [in] */ Int32 iconId);

        ResourceName(
            /* [in] */ IApplicationInfo* aInfo,
            /* [in] */ Int32 iconId);

        ResourceName(
            /* [in] */ IComponentInfo* cInfo,
            /* [in] */ Int32 iconId);

        ResourceName(
            /* [in] */ IResolveInfo* rInfo,
            /* [in] */ Int32 iconId);

        CARAPI_(Boolean) Equals(
            /* [in] */ ResourceName* o);

        CARAPI_(Int32) GetHashCode();

        CARAPI_(String) ToString();

    public:
        String mPackageName;
        Int32 mIconId;
    };

public:
    ApplicationPackageManager(
        /* [in] */ CContextImpl* context,
        /* [in] */ IIPackageManager* pm);

    AutoPtr<IUserManager> GetUserManager();

    CARAPI GetPackageInfo(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 flags,
        /* [out] */ IPackageInfo** info);

    CARAPI CurrentToCanonicalPackageNames(
        /* [in] */ const ArrayOf<String>& cuNames,
        /* [out, callee] */ ArrayOf<String>** caNames);

    CARAPI CanonicalToCurrentPackageNames(
        /* [in] */ const ArrayOf<String>& caNames,
        /* [out, callee] */ ArrayOf<String>** cuNames);

    CARAPI GetLaunchIntentForPackage(
        /* [in] */ const String& packageName,
        /* [out] */ IIntent** intent);

    CARAPI GetLeanbackLaunchIntentForPackage(
        /* [in] */ const String& packageName,
        /* [out] */ IIntent** intent);

    CARAPI GetPackageGids(
        /* [in] */ const String& packageName,
        /* [out, callee] */ ArrayOf<Int32>** pgids);

    CARAPI GetPermissionInfo(
        /* [in] */ const String& name,
        /* [in] */ Int32 flags,
        /* [out] */ IPermissionInfo** info);

    CARAPI GetPackageUid(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 userHandle,
        /* [out] */ Int32* uid);

    CARAPI QueryPermissionsByGroup(
        /* [in] */ const String& group,
        /* [in] */ Int32 flags,
        /* [out] */ IObjectContainer** permissions);

    CARAPI GetPermissionGroupInfo(
        /* [in] */ const String& name,
        /* [in] */ Int32 flags,
        /* [out] */ IPermissionGroupInfo** info);

    CARAPI GetAllPermissionGroups(
        /* [in] */ Int32 flags,
        /* [out] */ IObjectContainer** groups);

    CARAPI GetApplicationInfo(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 flags,
        /* [out] */ IApplicationInfo** info);

    CARAPI GetActivityInfo(
        /* [in] */ IComponentName* component,
        /* [in] */ Int32 flags,
        /* [out] */ IActivityInfo** info);

    CARAPI GetReceiverInfo(
        /* [in] */ IComponentName* component,
        /* [in] */ Int32 flags,
        /* [out] */ IActivityInfo** info);

    CARAPI GetServiceInfo(
        /* [in] */ IComponentName* component,
        /* [in] */ Int32 flags,
        /* [out] */ IServiceInfo** info);

    CARAPI GetProviderInfo(
        /* [in] */ IComponentName* component,
        /* [in] */ Int32 flags,
        /* [out] */ IProviderInfo** info);

    CARAPI GetSystemSharedLibraryNames(
        /* [out, callee] */ ArrayOf<String>** names);

    CARAPI GetSystemAvailableFeatures(
        /* [out, callee] */ ArrayOf<IFeatureInfo*>** features);

    CARAPI HasSystemFeature(
        /* [in] */ const String& name,
        /* [out] */ Boolean* result);

    CARAPI CheckPermission(
        /* [in] */ const String& permName,
        /* [in] */ const String& pkgName,
        /* [out] */ Int32* result);

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

    CARAPI CheckSignatures(
        /* [in] */ const String& pkg1,
        /* [in] */ const String& pkg2,
        /* [out] */ Int32* result);

    CARAPI CheckSignatures(
        /* [in] */ Int32 uid1,
        /* [in] */ Int32 uid2,
        /* [out] */ Int32* result);

    CARAPI GetPackagesForUid(
        /* [in] */ Int32 uid,
        /* [out, callee] */ ArrayOf<String>** packages);

    CARAPI GetNameForUid(
        /* [in] */ Int32 uid,
        /* [out] */ String* name);

    CARAPI GetUidForSharedUser(
        /* [in] */ const String& sharedUserName,
        /* [out] */ Int32* useruid);

    CARAPI GetInstalledPackages(
        /* [in] */ Int32 flags,
        /* [out] */ IList** infos);

    CARAPI GetInstalledPackages(
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IList** infos);

    CARAPI GetPackagesHoldingPermissions(
        /* [in] */ ArrayOf<String>* permissions,
        /* [in] */ Int32 flags,
        /* [out] */ IList** permissions);

    CARAPI GetInstalledApplications(
        /* [in] */ Int32 flags,
        /* [out] */ IList** apps);

    CARAPI ResolveActivity(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 flags,
        /* [out] */ IResolveInfo** resolve);

    CARAPI ResolveActivityAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IResolveInfo** resolve);

    CARAPI QueryIntentActivities(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 flags,
        /* [out] */ IObjectContainer** resolves);

    CARAPI QueryIntentActivitiesAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IObjectContainer** resolves);

    CARAPI QueryIntentActivityOptions(
        /* [in] */ IComponentName* caller,
        /* [in] */ ArrayOf<IIntent*>* specifics,
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 flags,
        /* [out] */ IObjectContainer** resolves);

    CARAPI QueryBroadcastReceivers(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IObjectContainer** resolves);

    CARAPI QueryBroadcastReceivers(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 flags,
        /* [out] */ IObjectContainer** resolves);

    CARAPI ResolveService(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 flags,
        /* [out] */ IResolveInfo** resolve);

    CARAPI QueryIntentServicesAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IObjectContainer** resolves);

    CARAPI QueryIntentServices(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 flags,
        /* [out] */ IObjectContainer** resolves);

    CARAPI QueryIntentContentProvidersAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IList** resolveInfos); //List<ResolveInfo>

    CARAPI QueryIntentContentProviders(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 flags,
        /* [out] */ IList** resolveInfos); //List<ResolveInfo>

    CARAPI ResolveContentProvider(
        /* [in] */ const String& name,
        /* [in] */ Int32 flags,
        /* [out] */ IProviderInfo** provider);

    CARAPI ResolveContentProvider(
        /* [in] */ const String& name,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IProviderInfo** provider);

    CARAPI QueryContentProviders(
        /* [in] */ const String& processName,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 flags,
        /* [out] */ IObjectContainer** providers);

    CARAPI GetInstrumentationInfo(
        /* [in] */ IComponentName* className,
        /* [in] */ Int32 flags,
        /* [out] */ IInstrumentationInfo** instrumentation);

    CARAPI QueryInstrumentation(
        /* [in] */ const String& targetPackage,
        /* [in] */ Int32 flags,
        /* [out] */ IObjectContainer** instrumentations);

    CARAPI GetDrawable(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 resid,
        /* [in] */ IApplicationInfo* appInfo,
        /* [out] */ IDrawable** drawable);

    CARAPI GetActivityIcon(
        /* [in] */ IComponentName* activityName,
        /* [out] */ IDrawable** icon);

    CARAPI GetActivityIcon(
        /* [in] */ IIntent* intent,
        /* [out] */ IDrawable** icon);

    CARAPI GetDefaultActivityIcon(
        /* [out] */ IDrawable** icon);

    CARAPI GetApplicationIcon(
        /* [in] */ IApplicationInfo* info,
        /* [out] */ IDrawable** icon);

    CARAPI GetApplicationIcon(
        /* [in] */ const String& packageName,
        /* [out] */ IDrawable** icon);

    CARAPI GetActivityBanner(
        /* [in] */ IComponentName* activityName,
        /* [out] */ IDrawable** icon);

    CARAPI GetActivityBanner(
        /* [in] */ IIntent* intent,
        /* [out] */ IDrawable** icon);

    CARAPI GetApplicationBanner(
        /* [in] */ IApplicationInfo* info,
        /* [out] */ IDrawable** icon);

    CARAPI GetApplicationBanner(
        /* [in] */ const String& packageName,
        /* [out] */ IDrawable** icon);

    CARAPI GetActivityLogo(
        /* [in] */ IComponentName* activityName,
        /* [out] */ IDrawable** logo);

    CARAPI GetActivityLogo(
        /* [in] */ IIntent* intent,
        /* [out] */ IDrawable** logo);

    CARAPI GetApplicationLogo(
        /* [in] */ IApplicationInfo* info,
        /* [out] */ IDrawable** logo);

    CARAPI GetApplicationLogo(
        /* [in] */ const String& packageName,
        /* [out] */ IDrawable** logo);

    CARAPI GetResourcesForActivity(
        /* [in] */ IComponentName* activityName,
        /* [out] */ IResources** res);

    CARAPI GetResourcesForApplication(
        /* [in] */ IApplicationInfo* app,
        /* [out] */ IResources** res);

    CARAPI GetResourcesForApplication(
        /* [in] */ const String& appPackageName,
        /* [out] */ IResources** res);

    CARAPI GetResourcesForApplicationAsUser(
        /* [in] */ const String& appPackageName,
        /* [in] */ Int32 userId,
        /* [out] */ IResources** res);

    CARAPI IsSafeMode(
        /* [out] */ Boolean* result);

    static CARAPI_(void) ConfigurationChanged();

    static CARAPI_(void) HandlePackageBroadcast(
        /* [in] */ Int32 cmd,
        /* [in] */ ArrayOf<String>* pkgList,
        /* [in] */ Boolean hasPkgInfo);

    CARAPI GetText(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 resid,
        /* [in] */ IApplicationInfo* appInfo,
        /* [out] */ ICharSequence** text);

    CARAPI GetXml(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 resid,
        /* [in] */ IApplicationInfo* appInfo,
        /* [out] */ IXmlResourceParser** parser);

    CARAPI GetApplicationLabel(
        /* [in] */ IApplicationInfo* info,
        /* [out] */ ICharSequence** label);

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

    CARAPI SetInstallerPackageName(
        /* [in] */ const String& targetPackage,
        /* [in] */ const String& installerPackageName);

    CARAPI MovePackage(
        /* [in] */ const String& packageName,
        /* [in] */ IPackageMoveObserver* observer,
        /* [in] */ Int32 flags);

    CARAPI GetInstallerPackageName(
        /* [in] */ const String& packageName,
        /* [out] */ String* name);

    CARAPI DeletePackage(
        /* [in] */ const String& packageName,
        /* [in] */ IPackageDeleteObserver* observer,
        /* [in] */ Int32 flags);

    CARAPI ClearApplicationUserData(
        /* [in] */ const String& packageName,
        /* [in] */ IPackageDataObserver* observer);

    CARAPI DeleteApplicationCacheFiles(
        /* [in] */ const String& packageName,
        /* [in] */ IPackageDataObserver* observer);

    CARAPI FreeStorageAndNotify(
        /* [in] */ Int64 freeStorageSize,
        /* [in] */ IPackageDataObserver* observer);

    CARAPI FreeStorage(
        /* [in] */ Int64 freeStorageSize,
        /* [in] */ IIntentSender* pi);

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
        /* [out] */ IObjectContainer** packages);

    CARAPI AddPreferredActivity(
        /* [in] */ IIntentFilter* filter,
        /* [in] */ Int32 match,
        /* [in] */ ArrayOf<IComponentName*>* set,
        /* [in] */ IComponentName* activity);

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

    CARAPI GetPreferredActivities(
        /* [in] */ IObjectContainer* outFilters,
        /* [in] */ IObjectContainer* outActivities,
        /* [in] */ const String& packageName,
        /* [out] */ Int32* num);

    CARAPI SetComponentEnabledSetting(
        /* [in] */ IComponentName* componentName,
        /* [in] */ Int32 newState,
        /* [in] */ Int32 flags);

    CARAPI GetComponentEnabledSetting(
        /* [in] */ IComponentName* componentName,
        /* [out] */ Int32* value);

    CARAPI SetApplicationEnabledSetting(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 newState,
        /* [in] */ Int32 flags);

    CARAPI GetApplicationEnabledSetting(
        /* [in] */ const String& packageName,
        /* [out] */ Int32* value);

    CARAPI GetVerifierDeviceIdentity(
        /* [out] */ IVerifierDeviceIdentity** identity);

    CARAPI GetPackageArchiveInfo(
        /* [in] */ const String& archiveFilePath,
        /* [in] */ Int32 flags,
        /* [out] */ IPackageInfo** info);

    CARAPI GetPackageSizeInfo(
        /* [in] */ const String& packageName,
        /* [in] */ IPackageStatsObserver* observer);

private:
    CARAPI_(AutoPtr<IDrawable>) GetCachedIcon(
        /* [in] */ ResourceName* name);

    static CARAPI_(void) MaybeAdjustApplicationInfo(
        /* [in] */ IApplicationInfo* info);

    CARAPI_(void) PutCachedIcon(
        /* [in] */ ResourceName* name,
        /* [in] */ IDrawable* dr);

    CARAPI_(AutoPtr<ICharSequence>) GetCachedString(
        /* [in] */ ResourceName* name);

    CARAPI_(void) PutCachedString(
        /* [in] */ ResourceName* name,
        /* [in] */ ICharSequence* cs);

private:
    static const String TAG;
    static const Boolean DEBUG;
    static const Boolean DEBUG_ICONS;

    // Default flags to use with PackageManager when no flags are given.
    static Int32 sDefaultFlags;// = PackageManager.GET_SHARED_LIBRARY_FILES;

    Object mLock;// = new Object();

    // @GuardedBy("mLock")
    AutoPtr<IUserManager> mUserManager;
    // @GuardedBy("mLock")
    AutoPtr<IPackageInstaller> mInstaller;

    CContextImpl* mContext;
    AutoPtr<IIPackageManager> mPM;

    static Object sSync;
    /*WeakReference<Drawable.ConstantState>*/
    static HashMap<AutoPtr<ResourceName>, AutoPtr<IWeakReference> > sIconCache;
    /*WeakReference<CharSequence>*/
    static HashMap<AutoPtr<ResourceName>, AutoPtr<IWeakReference> > sStringCache;

    Int32 mCachedSafeMode;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

_ETL_NAMESPACE_BEGIN

#define AUTOPTR_APM_RESOURCESNAME_HASH_FUNC

template<> struct Hash<AutoPtr<ApplicationPackageManager::ResourceName> >
{
    size_t operator()(AutoPtr<ApplicationPackageManager::ResourceName> s) const
    {
        assert(s != NULL);
        return (size_t)s->GetHashCode();
    }
};

template<> struct EqualTo<AutoPtr<ApplicationPackageManager::ResourceName> >
{
    size_t operator()(
        const AutoPtr<ApplicationPackageManager::ResourceName> x,
        const AutoPtr<ApplicationPackageManager::ResourceName> y) const
    {
        assert(x != NULL);
        return x->Equals(y);
    }
};

#endif //AUTOPTR_APM_RESOURCESNAME_HASH_FUNC
_ETL_NAMESPACE_END

#endif //__ELASTOS_DROID_APP_APPLICATIONPACKAGEMANAGER_H__
