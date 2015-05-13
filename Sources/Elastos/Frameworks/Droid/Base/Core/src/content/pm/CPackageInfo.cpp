
#include "ext/frameworkext.h"
#include "content/pm/CPackageInfo.h"
#include <elastos/Slogger.h>

using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CPackageInfo::CPackageInfo()
    : mVersionCode(0)
    , mSharedUserLabel(0)
    , mFirstInstallTime(0)
    , mLastUpdateTime(0)
    , mInstallLocation(INSTALL_LOCATION_INTERNAL_ONLY)
{}

CPackageInfo::~CPackageInfo()
{}

ECode CPackageInfo::constructor()
{
    return NOERROR;
}

ECode CPackageInfo::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    // return "PackageInfo{"
    //         + Integer.toHexString(System.identityHashCode(this))
    //         + " " + packageName + "}";
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CPackageInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    assert(source != NULL);
    source->ReadString(&mPackageName);
    source->ReadInt32(&mVersionCode);
    source->ReadString(&mVersionName);
    source->ReadString(&mSharedUserId);
    source->ReadInt32(&mSharedUserLabel);
    AutoPtr<IInterface> obj;
    source->ReadInterfacePtr((Handle32*)&obj);
    mApplicationInfo = obj != NULL ? IApplicationInfo::Probe(obj) : NULL;
    source->ReadInt64(&mFirstInstallTime);
    source->ReadInt64(&mLastUpdateTime);
    source->ReadArrayOf((Handle32*)&mGids);
    source->ReadArrayOf((Handle32*)&mActivities);
    source->ReadArrayOf((Handle32*)&mReceivers);
    source->ReadArrayOf((Handle32*)&mServices);
    source->ReadArrayOf((Handle32*)&mProviders);
    source->ReadArrayOf((Handle32*)&mInstrumentation);
    source->ReadArrayOf((Handle32*)&mPermissions);
    source->ReadArrayOfString((ArrayOf<String>**)&mRequestedPermissions);
    source->ReadArrayOf((Handle32*)&mRequestedPermissionsFlags);
    source->ReadArrayOf((Handle32*)&mSignatures);
    source->ReadArrayOf((Handle32*)&mConfigPreferences);
    source->ReadArrayOf((Handle32*)&mReqFeatures);
    source->ReadInt32(&mInstallLocation);
    return NOERROR;
}

ECode CPackageInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    assert(dest != NULL);
    dest->WriteString(mPackageName);
    dest->WriteInt32(mVersionCode);
    dest->WriteString(mVersionName);
    dest->WriteString(mSharedUserId);
    dest->WriteInt32(mSharedUserLabel);
    dest->WriteInterfacePtr(mApplicationInfo);
    dest->WriteInt64(mFirstInstallTime);
    dest->WriteInt64(mLastUpdateTime);
    dest->WriteArrayOf((Handle32)mGids.Get());
    dest->WriteArrayOf((Handle32)mActivities.Get());
    dest->WriteArrayOf((Handle32)mReceivers.Get());
    dest->WriteArrayOf((Handle32)mServices.Get());
    dest->WriteArrayOf((Handle32)mProviders.Get());
    dest->WriteArrayOf((Handle32)mInstrumentation.Get());
    dest->WriteArrayOf((Handle32)mPermissions.Get());
    dest->WriteArrayOfString(mRequestedPermissions.Get());
    dest->WriteArrayOf((Handle32)mRequestedPermissionsFlags.Get());
    dest->WriteArrayOf((Handle32)mSignatures.Get());
    dest->WriteArrayOf((Handle32)mConfigPreferences.Get());
    dest->WriteArrayOf((Handle32)mReqFeatures.Get());
    dest->WriteInt32(mInstallLocation);
    return NOERROR;
}

ECode CPackageInfo::GetPackageName(
    /* [out] */ String* packageName)
{
    VALIDATE_NOT_NULL(packageName);
    *packageName = mPackageName;
    return NOERROR;
}

ECode CPackageInfo::SetPackageName(
    /* [in] */ const String& packageName)
{
    mPackageName = packageName;
    return NOERROR;
}

ECode CPackageInfo::GetVersionCode(
    /* [out] */ Int32* versionCode)
{
    VALIDATE_NOT_NULL(versionCode);
    *versionCode = mVersionCode;
    return NOERROR;
}

ECode CPackageInfo::SetVersionCode(
    /* [in] */ Int32 versionCode)
{
    mVersionCode = versionCode;
    return NOERROR;
}

ECode CPackageInfo::GetVersionName(
    /* [out] */ String* versionName)
{
    VALIDATE_NOT_NULL(versionName);
    *versionName = mVersionName;
    return NOERROR;
}

ECode CPackageInfo::SetVersionName(
    /* [in] */ const String& versionName)
{
    mVersionName = versionName;
    return NOERROR;
}

ECode CPackageInfo::GetSharedUserId(
    /* [out] */ String* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mSharedUserId;
    return NOERROR;
}

ECode CPackageInfo::SetSharedUserId(
    /* [in] */ const String& id)
{
    mSharedUserId = id;
    return NOERROR;
}

ECode CPackageInfo::GetSharedUserLabel(
    /* [out] */ Int32* label)
{
    VALIDATE_NOT_NULL(label);
    *label = mSharedUserLabel;
    return NOERROR;
}

ECode CPackageInfo::SetSharedUserLabel(
    /* [in] */ Int32 label)
{
    mSharedUserLabel = label;
    return NOERROR;
}

ECode CPackageInfo::GetApplicationInfo(
    /* [out] */ IApplicationInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = mApplicationInfo;
    INTERFACE_ADDREF(*info);
    return NOERROR;
}

ECode CPackageInfo::SetApplicationInfo(
    /* [in] */ IApplicationInfo* info)
{
    mApplicationInfo = info;
    return NOERROR;
}

ECode CPackageInfo::GetFirstInstallTime(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);
    *time = mFirstInstallTime;
    return NOERROR;
}

ECode CPackageInfo::SetFirstInstallTime(
    /* [in] */ Int64 time)
{
    mFirstInstallTime = time;
    return NOERROR;
}

ECode CPackageInfo::GetLastUpdateTime(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);
    *time = mLastUpdateTime;
    return NOERROR;
}

ECode CPackageInfo::SetLastUpdateTime(
    /* [in] */ Int64 time)
{
    mLastUpdateTime = time;
    return NOERROR;
}

ECode CPackageInfo::GetGids(
    /* [out, callee] */ ArrayOf<Int32>** gids)
{
    VALIDATE_NOT_NULL(gids);
    *gids = mGids;
    INTERFACE_ADDREF(*gids);
    return NOERROR;
}

ECode CPackageInfo::SetGids(
    /* [in] */ ArrayOf<Int32>* gids)
{
    mGids = gids;
    return NOERROR;
}

ECode CPackageInfo::GetActivities(
    /* [out, callee] */ ArrayOf<IActivityInfo*>** activities)
{
    VALIDATE_NOT_NULL(activities);
    *activities = mActivities;
    INTERFACE_ADDREF(*activities);
    return NOERROR;
}

ECode CPackageInfo::SetActivities(
    /* [in] */ ArrayOf<IActivityInfo*>* activities)
{
    mActivities = activities;
    return NOERROR;
}

ECode CPackageInfo::GetReceivers(
    /* [out, callee] */ ArrayOf<IActivityInfo*>** receivers)
{
    VALIDATE_NOT_NULL(receivers);
    *receivers = mReceivers;
    INTERFACE_ADDREF(*receivers);
    return NOERROR;
}

ECode CPackageInfo::SetReceivers(
    /* [in] */ ArrayOf<IActivityInfo*>* receivers)
{
    mReceivers = receivers;
    return NOERROR;
}

ECode CPackageInfo::GetServices(
    /* [out, callee] */ ArrayOf<IServiceInfo*>** services)
{
    VALIDATE_NOT_NULL(services);
    *services = mServices;
    INTERFACE_ADDREF(*services);
    return NOERROR;
}

ECode CPackageInfo::SetServices(
    /* [in] */ ArrayOf<IServiceInfo*>* services)
{
    mServices = services;
    return NOERROR;
}

ECode CPackageInfo::GetProviders(
    /* [out, callee] */ ArrayOf<IProviderInfo*>** providers)
{
    VALIDATE_NOT_NULL(providers);
    *providers = mProviders;
    INTERFACE_ADDREF(*providers);
    return NOERROR;
}

ECode CPackageInfo::SetProviders(
    /* [in] */ ArrayOf<IProviderInfo*>* providers)
{
    mProviders = providers;
    return NOERROR;
}

ECode CPackageInfo::GetInstrumentation(
    /* [out, callee] */ ArrayOf<IInstrumentationInfo*>** instrumentation)
{
    VALIDATE_NOT_NULL(instrumentation);
    *instrumentation = mInstrumentation;
    INTERFACE_ADDREF(*instrumentation);
    return NOERROR;
}

ECode CPackageInfo::SetInstrumentation(
    /* [in] */ ArrayOf<IInstrumentationInfo*>* instrumentation)
{
    mInstrumentation = instrumentation;
    return NOERROR;
}

ECode CPackageInfo::GetPermissions(
    /* [out, callee] */ ArrayOf<IPermissionInfo*>** permissions)
{
    VALIDATE_NOT_NULL(permissions);
    *permissions = mPermissions;
    INTERFACE_ADDREF(*permissions);
    return NOERROR;
}

ECode CPackageInfo::SetPermissions(
    /* [in] */ ArrayOf<IPermissionInfo*>* permissions)
{
    mPermissions = permissions;
    return NOERROR;
}

ECode CPackageInfo::GetRequestedPermissions(
    /* [out, callee] */ ArrayOf<String>** requestedPermissions)
{
    VALIDATE_NOT_NULL(requestedPermissions);
    *requestedPermissions = mRequestedPermissions;
    INTERFACE_ADDREF(*requestedPermissions);
    return NOERROR;
}

ECode CPackageInfo::SetRequestedPermissions(
    /* [in] */ ArrayOf<String>* requestedPermissions)
{
    mRequestedPermissions = requestedPermissions;
    return NOERROR;
}

ECode CPackageInfo::GetRequestedPermissionsFlags(
    /* [out, callee] */ ArrayOf<Int32>** requestedPermissionsFlags)
{
    VALIDATE_NOT_NULL(requestedPermissionsFlags);
    *requestedPermissionsFlags = mRequestedPermissionsFlags;
    INTERFACE_ADDREF(*requestedPermissionsFlags);
    return NOERROR;
}

ECode CPackageInfo::SetRequestedPermissionsFlags(
    /* [in] */ ArrayOf<Int32>* requestedPermissionsFlags)
{
    mRequestedPermissionsFlags = requestedPermissionsFlags;
    return NOERROR;
}

ECode CPackageInfo::GetSignatures(
    /* [out, callee] */ ArrayOf<ISignature*>** signatures)
{
    VALIDATE_NOT_NULL(signatures);
    *signatures = mSignatures;
    INTERFACE_ADDREF(*signatures);
    return NOERROR;
}

ECode CPackageInfo::SetSignatures(
    /* [in] */ ArrayOf<ISignature*>* signatures)
{
    mSignatures = signatures;
    return NOERROR;
}

ECode CPackageInfo::GetConfigPreferences(
    /* [out, callee] */ ArrayOf<IConfigurationInfo*>** configPreferences)
{
    VALIDATE_NOT_NULL(configPreferences);
    *configPreferences = mConfigPreferences;
    INTERFACE_ADDREF(*configPreferences);
    return NOERROR;
}

ECode CPackageInfo::SetConfigPreferences(
    /* [in] */ ArrayOf<IConfigurationInfo*>* configPreferences)
{
    mConfigPreferences = configPreferences;
    return NOERROR;
}

ECode CPackageInfo::GetReqFeatures(
    /* [out, callee] */ ArrayOf<IFeatureInfo*>** reqFeatures)
{
    VALIDATE_NOT_NULL(reqFeatures);
    *reqFeatures = mReqFeatures;
    INTERFACE_ADDREF(*reqFeatures);
    return NOERROR;
}

ECode CPackageInfo::SetReqFeatures(
    /* [in] */ ArrayOf<IFeatureInfo*>* reqFeatures)
{
    mReqFeatures = reqFeatures;
    return NOERROR;
}

ECode CPackageInfo::GetInstallLocation(
    /* [out] */ Int32* installLocation)
{
    VALIDATE_NOT_NULL(installLocation);
    *installLocation = mInstallLocation;
    return NOERROR;
}

ECode CPackageInfo::SetInstallLocation(
    /* [in] */ Int32 installLocation)
{
    mInstallLocation = installLocation;
    return NOERROR;
}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos
