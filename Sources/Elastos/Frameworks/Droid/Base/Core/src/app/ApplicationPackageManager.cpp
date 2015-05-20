
#include "ext/frameworkdef.h"
#include "app/ApplicationPackageManager.h"
#include "os/Process.h"
#include "content/CIntent.h"
#include "content/res/CResourcesHelper.h"
#include "util/CParcelableObjectContainer.h"
#include <elastos/StringBuilder.h>
#include <elastos/Slogger.h>
#include <elastos/Logger.h>
#include "R.h"

using Elastos::Core::CObjectContainer;
using Elastos::Core::EIID_ICharSequence;
using Elastos::Utility::Logging::Slogger;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::R;
using Elastos::Droid::Content::Pm::ECLSID_CApplicationInfo;
using Elastos::Droid::Content::Pm::ECLSID_CPackageInfo;
using Elastos::Droid::Content::Pm::EIID_IPackageManager;
using Elastos::Droid::Content::Pm::IParceledListSlice;
using Elastos::Droid::Content::Res::IResourcesHelper;
using Elastos::Droid::Content::Res::CResourcesHelper;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Utility::CParcelableObjectContainer;
using Elastos::Droid::Graphics::Drawable::EIID_IDrawableConstantState;

namespace Elastos {
namespace Droid {
namespace App {

const String ApplicationPackageManager::TAG("ApplicationPackageManager");
const Boolean ApplicationPackageManager::DEBUG = FALSE;
const Boolean ApplicationPackageManager::DEBUG_ICONS = FALSE;

Mutex ApplicationPackageManager::sSync;
HashMap<AutoPtr<ApplicationPackageManager::ResourceName>, AutoPtr<IWeakReference> > ApplicationPackageManager::sIconCache;
HashMap<AutoPtr<ApplicationPackageManager::ResourceName>, AutoPtr<IWeakReference> > ApplicationPackageManager::sStringCache;

ApplicationPackageManager::ApplicationPackageManager(
    /* [in] */ CContextImpl* context,
    /* [in] */ IIPackageManager* pm)
    : mContext(context)
    , mPM(pm)
    , mCachedSafeMode(-1)
{}

PInterface ApplicationPackageManager::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IPackageManager) {
        return (IPackageManager*)this;
    }

    return NULL;
}

UInt32 ApplicationPackageManager::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ApplicationPackageManager::Release()
{
    return ElRefBase::Release();
}

ECode ApplicationPackageManager::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IPackageManager*)this) {
        *pIID = EIID_IPackageManager;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode ApplicationPackageManager::GetPackageInfo(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 flags,
    /* [out] */ IPackageInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;

//    try {
    Int32 id;
    mContext->GetUserId(&id);
    AutoPtr<IPackageInfo> pi;
    FAIL_RETURN(mPM->GetPackageInfo(packageName, flags, id, (IPackageInfo**)&pi));
    if (pi != NULL) {
        *info = pi;
        INTERFACE_ADDREF(*info);
        return NOERROR;
    }
//    } catch (RemoteException e) {
//        throw new RuntimeException("Package manager has died", e);
//    }

//    throw new NameNotFoundException(packageName);
    return E_NAME_NOT_FOUND_EXCEPTION;
}

ECode ApplicationPackageManager::CurrentToCanonicalPackageNames(
    /* [in] */ const ArrayOf<String>& cuNames,
    /* [out, callee] */ ArrayOf<String>** caNames)
{
    VALIDATE_NOT_NULL(caNames);
//    try {
    return mPM->CurrentToCanonicalPackageNames(cuNames, caNames);
//    } catch (RemoteException e) {
//        throw new RuntimeException("Package manager has died", e);
//    }
}

ECode ApplicationPackageManager::CanonicalToCurrentPackageNames(
    /* [in] */ const ArrayOf<String>& caNames,
    /* [out, callee] */ ArrayOf<String>** cuNames)
{
//    try {
    return mPM->CanonicalToCurrentPackageNames(caNames, cuNames);
//    } catch (RemoteException e) {
//        throw new RuntimeException("Package manager has died", e);
//    }
}

ECode ApplicationPackageManager::GetLaunchIntentForPackage(
    /* [in] */ const String& packageName,
    /* [out] */ IIntent** intent)
{
    VALIDATE_NOT_NULL(intent);
    // First see if the package has an INFO activity; the existence of
    // such an activity is implied to be the desired front-door for the
    // overall package (such as if it has multiple launcher entries).
    AutoPtr<IIntent> intentToResolve;
    CIntent::New(IIntent::ACTION_MAIN, (IIntent**)&intentToResolve);
    intentToResolve->AddCategory(IIntent::CATEGORY_INFO);
    intentToResolve->SetPackage(packageName);
    AutoPtr<IObjectContainer> ris;
    QueryIntentActivities(intentToResolve, 0, (IObjectContainer**)&ris);

    // Otherwise, try to find a main launcher activity.
    Int32 size;
    if (ris == NULL ||(ris->GetObjectCount(&size), size <= 0)) {
        // reuse the intent instance
        intentToResolve->RemoveCategory(IIntent::CATEGORY_INFO);
        intentToResolve->AddCategory(IIntent::CATEGORY_LAUNCHER);
        intentToResolve->SetPackage(packageName);
        ris = NULL;
        QueryIntentActivities(intentToResolve, 0, (IObjectContainer**)&ris);
    }
    if (ris == NULL ||(ris->GetObjectCount(&size), size <= 0)) {
        *intent = NULL;
        return NOERROR;
    }
    AutoPtr<IIntent> newIntent;
    CIntent::New(intentToResolve, (IIntent**)&newIntent);
    newIntent->SetFlags(IIntent::FLAG_ACTIVITY_NEW_TASK);
    AutoPtr<IObjectEnumerator> enumerator;
    ris->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Boolean hasNext;
    AutoPtr<IResolveInfo> resInfo;
    if (enumerator->MoveNext(&hasNext), hasNext) enumerator->Current((IInterface**)&resInfo);
    AutoPtr<IActivityInfo> aInfo;
    resInfo->GetActivityInfo((IActivityInfo**)&aInfo);
    String pkgName, name;
    aInfo->GetPackageName(&pkgName);
    aInfo->GetName(&name);
    newIntent->SetClassNameEx(pkgName, name);
    *intent = newIntent;
    INTERFACE_ADDREF(*intent);
    return NOERROR;
}

ECode ApplicationPackageManager::GetPackageGids(
    /* [in] */ const String& packageName,
    /* [out, callee] */ ArrayOf<Int32>** pgids)
{
    VALIDATE_NOT_NULL(pgids);
    *pgids = NULL;

//    try {
    AutoPtr< ArrayOf<Int32> > gids;
    FAIL_RETURN(mPM->GetPackageGids(packageName, (ArrayOf<Int32>**)&gids));
    if (gids == NULL || gids->GetLength() > 0) {
        *pgids = gids;
        INTERFACE_ADDREF(*pgids);
        return NOERROR;
    }
//    } catch (RemoteException e) {
//        throw new RuntimeException("Package manager has died", e);
//    }

//    throw new NameNotFoundException(packageName);
    return E_NAME_NOT_FOUND_EXCEPTION;
}

ECode ApplicationPackageManager::GetPermissionInfo(
    /* [in] */ const String& name,
    /* [in] */ Int32 flags,
    /* [out] */ IPermissionInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;

//     try {
    AutoPtr<IPermissionInfo> pi;
    FAIL_RETURN(mPM->GetPermissionInfo(name, flags, (IPermissionInfo**)&pi));
    if (pi != NULL) {
        *info = pi;
        INTERFACE_ADDREF(*info);
        return NOERROR;
    }
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }

//     throw new NameNotFoundException(name);
    return E_NAME_NOT_FOUND_EXCEPTION;
}

ECode ApplicationPackageManager::QueryPermissionsByGroup(
    /* [in] */ const String& group,
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** permissions)
{
    VALIDATE_NOT_NULL(permissions);
    *permissions = NULL;

//     try {
    AutoPtr<IObjectContainer> pi;
    FAIL_RETURN(mPM->QueryPermissionsByGroup(group, flags, (IObjectContainer**)&pi));
    if (pi != NULL) {
        *permissions = pi;
        INTERFACE_ADDREF(*permissions);
        return NOERROR;
    }
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }

//     throw new NameNotFoundException(group);
    return E_NAME_NOT_FOUND_EXCEPTION;
}

ECode ApplicationPackageManager::GetPermissionGroupInfo(
    /* [in] */ const String& name,
    /* [in] */ Int32 flags,
    /* [out] */ IPermissionGroupInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;

//     try {
    AutoPtr<IPermissionGroupInfo> pgi;
    FAIL_RETURN(mPM->GetPermissionGroupInfo(name, flags, (IPermissionGroupInfo**)&pgi));
    if (pgi != NULL) {
        *info = pgi;
        INTERFACE_ADDREF(*info);
        return NOERROR;
    }
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }

//     throw new NameNotFoundException(name);
    return E_NAME_NOT_FOUND_EXCEPTION;
}

ECode ApplicationPackageManager::GetAllPermissionGroups(
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** groups)
{
    VALIDATE_NOT_NULL(groups);
//     try {
    return mPM->GetAllPermissionGroups(flags, groups);
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }
}

ECode ApplicationPackageManager::GetApplicationInfo(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 flags,
    /* [out] */ IApplicationInfo** info)
{
    VALIDATE_NOT_NULL(info)
    AutoPtr<IApplicationInfo> ai;
    #if 0
    Int32 id;
    mContext->GetUserId(&id);
    FAIL_RETURN(mPM->GetApplicationInfo(packageName, flags, id, (IApplicationInfo**)&ai));
    #else
        FAIL_RETURN(mPM->GetApplicationInfo(packageName, flags, 0, (IApplicationInfo**)&ai));
    #endif
    if (ai != NULL) {
        *info = ai;
        INTERFACE_ADDREF(*info)
        return NOERROR;
    }
    return E_NAME_NOT_FOUND_EXCEPTION;
}

ECode ApplicationPackageManager::GetActivityInfo(
    /* [in] */ IComponentName* component,
    /* [in] */ Int32 flags,
    /* [out] */ IActivityInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;

//     try {
    Int32 id;
    mContext->GetUserId(&id);
    AutoPtr<IActivityInfo> ai;
    FAIL_RETURN(mPM->GetActivityInfo(component, flags, id, (IActivityInfo**)&ai));
    if (ai != NULL) {
        *info = ai;
        INTERFACE_ADDREF(*info);
        return NOERROR;
    }
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }

//     throw new NameNotFoundException(className.toString());
    return E_NAME_NOT_FOUND_EXCEPTION;
}

ECode ApplicationPackageManager::GetReceiverInfo(
    /* [in] */ IComponentName* component,
    /* [in] */ Int32 flags,
    /* [out] */ IActivityInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;

//     try {
    Int32 id;
    mContext->GetUserId(&id);
    AutoPtr<IActivityInfo> ai;
    FAIL_RETURN(mPM->GetReceiverInfo(component, flags, id, (IActivityInfo**)&ai));
    if (ai != NULL) {
        *info = ai;
        INTERFACE_ADDREF(*info);
        return NOERROR;
    }
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }

//     throw new NameNotFoundException(className.toString());
    return E_NAME_NOT_FOUND_EXCEPTION;
}

ECode ApplicationPackageManager::GetServiceInfo(
    /* [in] */ IComponentName* component,
    /* [in] */ Int32 flags,
    /* [out] */ IServiceInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;

//     try {
    Int32 id;
    mContext->GetUserId(&id);
    AutoPtr<IServiceInfo> si;
    FAIL_RETURN(mPM->GetServiceInfo(component, flags, id, (IServiceInfo**)&si));
    if (si != NULL) {
        *info = si;
        INTERFACE_ADDREF(*info);
        return NOERROR;
    }
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }

//     throw new NameNotFoundException(className.toString());
    return E_NAME_NOT_FOUND_EXCEPTION;
}

ECode ApplicationPackageManager::GetProviderInfo(
    /* [in] */ IComponentName* component,
    /* [in] */ Int32 flags,
    /* [out] */ IProviderInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;

//     try {
    Int32 id;
    mContext->GetUserId(&id);
    AutoPtr<IProviderInfo> pi;
    FAIL_RETURN(mPM->GetProviderInfo(component, flags, id, (IProviderInfo**)&pi));
    if (pi != NULL) {
        *info = pi;
        INTERFACE_ADDREF(*info);
        return NOERROR;
    }
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }

//     throw new NameNotFoundException(className.toString());
    return E_NAME_NOT_FOUND_EXCEPTION;
}

ECode ApplicationPackageManager::GetSystemSharedLibraryNames(
    /* [out, callee] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names);
//     try {
    return mPM->GetSystemSharedLibraryNames(names);
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }
}

ECode ApplicationPackageManager::GetSystemAvailableFeatures(
    /* [out, callee] */ ArrayOf<IFeatureInfo*>** features)
{
    VALIDATE_NOT_NULL(features);
//     try {
    return mPM->GetSystemAvailableFeatures(features);
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }
}

ECode ApplicationPackageManager::HasSystemFeature(
    /* [in] */ const String& name,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mPM->HasSystemFeature(name, result);
}

ECode ApplicationPackageManager::CheckPermission(
    /* [in] */ const String& permName,
    /* [in] */ const String& pkgName,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
//     try {
    return mPM->CheckPermission(permName, pkgName, result);
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }
}

ECode ApplicationPackageManager::AddPermission(
    /* [in] */ IPermissionInfo* info,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
//     try {
    return mPM->AddPermission(info, result);
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }
}

ECode ApplicationPackageManager::AddPermissionAsync(
    /* [in] */ IPermissionInfo* info,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
//     try {
    return mPM->AddPermissionAsync(info, result);
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }
}

ECode ApplicationPackageManager::RemovePermission(
    /* [in] */ const String& name)
{
//     try {
    return mPM->RemovePermission(name);
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }
}

ECode ApplicationPackageManager::GrantPermission(
    /* [in] */ const String& packageName,
    /* [in] */ const String& permissionName)
{
//     try {
    return mPM->GrantPermission(packageName, permissionName);
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }
}

ECode ApplicationPackageManager::RevokePermission(
    /* [in] */ const String& packageName,
    /* [in] */ const String& permissionName)
{
//     try {
    return mPM->RevokePermission(packageName, permissionName);
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }
}

ECode ApplicationPackageManager::CheckSignatures(
    /* [in] */ const String& pkg1,
    /* [in] */ const String& pkg2,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
//     try {
    return mPM->CheckSignatures(pkg1, pkg2, result);
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }
}

ECode ApplicationPackageManager::CheckSignaturesEx(
    /* [in] */ Int32 uid1,
    /* [in] */ Int32 uid2,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
//     try {
    return mPM->CheckUidSignatures(uid1, uid2, result);
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }
}

ECode ApplicationPackageManager::GetPackagesForUid(
    /* [in] */ Int32 uid,
    /* [out, callee] */ ArrayOf<String>** packages)
{
    VALIDATE_NOT_NULL(packages);
//     try {
    return mPM->GetPackagesForUid(uid, packages);
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }
}

ECode ApplicationPackageManager::GetNameForUid(
    /* [in] */ Int32 uid,
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
//     try {
    return mPM->GetNameForUid(uid, name);
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }
}

ECode ApplicationPackageManager::GetUidForSharedUser(
    /* [in] */ const String& sharedUserName,
    /* [out] */ Int32* useruid)
{
    VALIDATE_NOT_NULL(useruid);
//     try {
    Int32 uid;
    FAIL_RETURN(mPM->GetUidForSharedUser(sharedUserName, &uid));
    if(uid != -1) {
        *useruid = uid;
        return NOERROR;
    }
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }
//     throw new NameNotFoundException("No shared userid for user:"+sharedUserName);
    return E_NAME_NOT_FOUND_EXCEPTION;
}

ECode ApplicationPackageManager::GetInstalledPackages(
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** infos)
{
    VALIDATE_NOT_NULL(infos);
    Int32 id;
    mContext->GetUserId(&id);
    return GetInstalledPackagesEx(flags, id, infos);
}

ECode ApplicationPackageManager::GetInstalledPackagesEx(
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId,
    /* [out] */ IObjectContainer** infos)
{
    VALIDATE_NOT_NULL(infos);
//     try {
    AutoPtr<IObjectContainer> packageInfos;
    CObjectContainer::New((IObjectContainer**)&packageInfos);
    AutoPtr<IPackageInfo> lastItem = NULL;
    AutoPtr<IParceledListSlice> slice;

    Boolean lastSlice;
    String lastKey;
    do {
        if (lastItem != NULL) lastItem->GetPackageName(&lastKey);
        slice = NULL;
        FAIL_RETURN(mPM->GetInstalledPackages(flags, lastKey, userId, (IParceledListSlice**)&slice));
        lastItem = NULL;
        FAIL_RETURN(slice->PopulateList(packageInfos, ECLSID_CPackageInfo, (IInterface**)&lastItem));
    } while (slice->IsLastSlice(&lastSlice), !lastSlice);

    *infos = packageInfos;
    INTERFACE_ADDREF(*infos);
    return NOERROR;
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }
}

ECode ApplicationPackageManager::GetInstalledApplications(
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** apps)
{
    VALIDATE_NOT_NULL(apps);
    Int32 userId = 0;
    mContext->GetUserId(&userId);
    AutoPtr<IObjectContainer> applicationInfos;
    CObjectContainer::New((IObjectContainer**)&applicationInfos);
    AutoPtr<IApplicationInfo> lastItem = NULL;
    AutoPtr<IParceledListSlice> slice;

    Boolean lastSlice;
    String lastKey;
    do {
        if (lastItem != NULL) lastItem->GetPackageName(&lastKey);
        slice = NULL;
        FAIL_RETURN(mPM->GetInstalledApplications(flags, lastKey, userId, (IParceledListSlice**)&slice));
        lastItem = NULL;
        FAIL_RETURN(slice->PopulateList(applicationInfos, ECLSID_CApplicationInfo, (IInterface**)&lastItem));
    } while (!(slice->IsLastSlice(&lastSlice), lastSlice));
    *apps = applicationInfos;
    INTERFACE_ADDREF(*apps);
    return NOERROR;
}

ECode ApplicationPackageManager::ResolveActivity(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [out] */ IResolveInfo** resolve)
{
    VALIDATE_NOT_NULL(resolve);
    Int32 id;
    mContext->GetUserId(&id);
    return ResolveActivityAsUser(intent, flags, id, resolve);
}

ECode ApplicationPackageManager::ResolveActivityAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId,
    /* [out] */ IResolveInfo** resolve)
{
    VALIDATE_NOT_NULL(resolve);
//     try {
    AutoPtr<IContentResolver> cResolver;
    FAIL_RETURN(mContext->GetContentResolver((IContentResolver**)&cResolver));
    String type;
    FAIL_RETURN(intent->ResolveTypeIfNeeded(cResolver, &type));
    return mPM->ResolveIntent(
            intent,
            type,
            flags,
            userId, resolve);
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }
}

ECode ApplicationPackageManager::QueryIntentActivities(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** resolves)
{
    VALIDATE_NOT_NULL(resolves);
    Int32 id;
    mContext->GetUserId(&id);
    return QueryIntentActivitiesAsUser(intent, flags, id, resolves);
}

ECode ApplicationPackageManager::QueryIntentActivitiesAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId,
    /* [out] */ IObjectContainer** resolves)
{
    VALIDATE_NOT_NULL(resolves);
//     try {
    AutoPtr<IContentResolver> cResolver;
    FAIL_RETURN(mContext->GetContentResolver((IContentResolver**)&cResolver));
    String type;
    FAIL_RETURN(intent->ResolveTypeIfNeeded(cResolver, &type));
    return mPM->QueryIntentActivities(
            intent,
            type,
            flags,
            userId,
            resolves);
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }
}

ECode ApplicationPackageManager::QueryIntentActivityOptions(
    /* [in] */ IComponentName* caller,
    /* [in] */ ArrayOf<IIntent*>* specifics,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** resolves)
{
    VALIDATE_NOT_NULL(resolves);
    AutoPtr<IContentResolver> resolver;
    FAIL_RETURN(mContext->GetContentResolver((IContentResolver**)&resolver));

    AutoPtr<ArrayOf<String> > specificTypes;
    if (specifics != NULL) {
        const Int32 N = specifics->GetLength();
        for (Int32 i=0; i<N; i++) {
            AutoPtr<IIntent> sp = (*specifics)[i];
            if (sp != NULL) {
                String t;
                sp->ResolveTypeIfNeeded(resolver, &t);
                if (t != NULL) {
                    if (specificTypes == NULL) {
                        specificTypes = ArrayOf<String>::Alloc(N);
                    }
                    (*specificTypes)[i] = t;
                }
            }
        }
    }

//     try {
    String type;
    FAIL_RETURN(intent->ResolveTypeIfNeeded(resolver, &type));
    Int32 id;
    mContext->GetUserId(&id);
    return mPM->QueryIntentActivityOptions(caller, specifics,
                                              specificTypes, intent, type,
                                              flags, id, resolves);
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }
}

ECode ApplicationPackageManager::QueryBroadcastReceiversEx(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId,
    /* [out] */ IObjectContainer** resolves)
{
    VALIDATE_NOT_NULL(resolves);
//     try {
    AutoPtr<IContentResolver> resolver;
    FAIL_RETURN(mContext->GetContentResolver((IContentResolver**)&resolver));
    String type;
    FAIL_RETURN(intent->ResolveTypeIfNeeded(resolver, &type));
    return mPM->QueryIntentReceivers(
            intent,
            type,
            flags,
            userId,
            resolves);
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }
}

ECode ApplicationPackageManager::QueryBroadcastReceivers(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** resolves)
{
    VALIDATE_NOT_NULL(resolves);

    Int32 id;
    mContext->GetUserId(&id);
    return QueryBroadcastReceiversEx(intent, flags, id, resolves);
}

ECode ApplicationPackageManager::ResolveService(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [out] */ IResolveInfo** resolve)
{
    VALIDATE_NOT_NULL(resolve);
//     try {
    AutoPtr<IContentResolver> resolver;
    FAIL_RETURN(mContext->GetContentResolver((IContentResolver**)&resolver));
    String type;
    FAIL_RETURN(intent->ResolveTypeIfNeeded(resolver, &type));
    Int32 id;
    FAIL_RETURN(mContext->GetUserId(&id));
    return mPM->ResolveService(
            intent,
            type,
            flags,
            id,
            resolve);
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }
}

ECode ApplicationPackageManager::QueryIntentServicesAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId,
    /* [out] */ IObjectContainer** resolves)
{
    VALIDATE_NOT_NULL(resolves);
    // try {
    AutoPtr<IContentResolver> resolver;
    FAIL_RETURN(mContext->GetContentResolver((IContentResolver**)&resolver));
    String type;
    FAIL_RETURN(intent->ResolveTypeIfNeeded(resolver, &type));
    return mPM->QueryIntentServices(
        intent,
        type,
        flags,
        userId,
        resolves);
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }
}

ECode ApplicationPackageManager::QueryIntentServices(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** resolves)
{
    VALIDATE_NOT_NULL(resolves);
    Int32 id;
    mContext->GetUserId(&id);
    return QueryIntentServicesAsUser(intent, flags, id, resolves);
}

ECode ApplicationPackageManager::ResolveContentProvider(
    /* [in] */ const String& name,
    /* [in] */ Int32 flags,
    /* [out] */ IProviderInfo** provider)
{
//     try {
    Int32 id;
    mContext->GetUserId(&id);
    return mPM->ResolveContentProvider(name, flags, id, provider);
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }
}

ECode ApplicationPackageManager::QueryContentProviders(
    /* [in] */ const String& processName,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** providers)
{
//     try {
    return mPM->QueryContentProviders(processName, uid, flags, providers);
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }
}

ECode ApplicationPackageManager::GetInstrumentationInfo(
    /* [in] */ IComponentName* className,
    /* [in] */ Int32 flags,
    /* [out] */ IInstrumentationInfo** instrumentation)
{
    VALIDATE_NOT_NULL(instrumentation);
    *instrumentation = NULL;
//     try {
    AutoPtr<IInstrumentationInfo> ii;
    mPM->GetInstrumentationInfo(className, flags, (IInstrumentationInfo**)&ii);
    if (ii != NULL) {
        *instrumentation = ii;
        INTERFACE_ADDREF(*instrumentation);
        return NOERROR;
    }
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }

//     throw new NameNotFoundException(className.toString());
    return E_NAME_NOT_FOUND_EXCEPTION;
}

ECode ApplicationPackageManager::QueryInstrumentation(
    /* [in] */ const String& targetPackage,
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** instrumentations)
{
//     try {
    return mPM->QueryInstrumentation(targetPackage, flags, instrumentations);
//     } catch (RemoteException e) {
//         throw new RuntimeException("Package manager has died", e);
//     }
}

ECode ApplicationPackageManager::GetDrawable(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 resid,
    /* [in] */ IApplicationInfo* appInfo,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);

    AutoPtr<ResourceName> name = new ResourceName(packageName, resid);
    AutoPtr<IDrawable> dr = GetCachedIcon(name);
    if (dr != NULL) {
        *drawable = dr;
        INTERFACE_ADDREF(*drawable);
        return NOERROR;
    }
    if (appInfo == NULL) {
        if (FAILED(GetApplicationInfo(packageName, 0, &appInfo))) {
            *drawable = NULL;
            return NOERROR;
        }
    }

    AutoPtr<IResources> r;
    FAIL_GOTO(GetResourcesForApplication(appInfo, (IResources**)&r), EXCEPTION);
    FAIL_GOTO(r->GetDrawable(resid, (IDrawable**)&dr), EXCEPTION);

    PutCachedIcon(name, dr);
    *drawable = dr;
    INTERFACE_ADDREF(*drawable);
    return NOERROR;

EXCEPTION:
    *drawable = NULL;
    return NOERROR;
}

ECode ApplicationPackageManager::GetActivityIcon(
    /* [in] */ IComponentName* activityName,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    AutoPtr<IActivityInfo> aInfo;
    GetActivityInfo(activityName, 0, (IActivityInfo**)&aInfo);
    FAIL_RETURN(aInfo->LoadIcon(this, icon));
    return NOERROR;
}

ECode ApplicationPackageManager::GetActivityIconEx(
    /* [in] */ IIntent* intent,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    AutoPtr<IComponentName> component;
    intent->GetComponent((IComponentName**)&component);
    if (component != NULL) {
        return GetActivityIcon(component, icon);
    }

    AutoPtr<IResolveInfo> info;
    ResolveActivity(
        intent, IPackageManager::MATCH_DEFAULT_ONLY, (IResolveInfo**)&info);
    if (info != NULL) {
        AutoPtr<IActivityInfo> aInfo;
        info->GetActivityInfo((IActivityInfo**)&aInfo);
        return aInfo->LoadIcon(this, icon);
    }

//     throw new NameNotFoundException(intent.toUri(0));
    return E_NAME_NOT_FOUND_EXCEPTION;
}

ECode ApplicationPackageManager::GetDefaultActivityIcon(
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    AutoPtr<IResourcesHelper> resHelper;
    CResourcesHelper::AcquireSingleton((IResourcesHelper**)&resHelper);
    AutoPtr<IResources> resources;
    resHelper->GetSystem((IResources**)&resources);
    FAIL_RETURN(resources->GetDrawable(
           R::drawable::sym_def_app_icon, icon));
    return NOERROR;
}

ECode ApplicationPackageManager::GetApplicationIcon(
    /* [in] */ IApplicationInfo* info,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return info->LoadIcon(this, icon);
}

ECode ApplicationPackageManager::GetApplicationIconEx(
    /* [in] */ const String& packageName,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    AutoPtr<IApplicationInfo> appInfo;
    GetApplicationInfo(packageName, 0, (IApplicationInfo**)&appInfo);
    GetApplicationIcon(appInfo, icon);
    return NOERROR;
}

ECode ApplicationPackageManager::GetActivityLogo(
    /* [in] */ IComponentName* activityName,
    /* [out] */ IDrawable** logo)
{
    VALIDATE_NOT_NULL(logo);
    AutoPtr<IActivityInfo> aInfo;
    GetActivityInfo(activityName, 0, (IActivityInfo**)&aInfo);
    return aInfo->LoadLogo(this, logo);
}

ECode ApplicationPackageManager::GetActivityLogoEx(
    /* [in] */ IIntent* intent,
    /* [out] */ IDrawable** logo)
{
    AutoPtr<IComponentName> component;
    intent->GetComponent((IComponentName**)&component);
    if (component != NULL) {
        return GetActivityLogo(component, logo);
    }

    AutoPtr<IResolveInfo> info;
    ResolveActivity(
        intent, IPackageManager::MATCH_DEFAULT_ONLY, (IResolveInfo**)&info);
    if (info != NULL) {
        AutoPtr<IActivityInfo> aInfo;
        info->GetActivityInfo((IActivityInfo**)&aInfo);
        return aInfo->LoadLogo(this, logo);
    }

//     throw new NameNotFoundException(intent.toUri(0));
    return E_NAME_NOT_FOUND_EXCEPTION;
}

ECode ApplicationPackageManager::GetApplicationLogo(
    /* [in] */ IApplicationInfo* info,
    /* [out] */ IDrawable** logo)
{
    VALIDATE_NOT_NULL(logo);

    return info->LoadLogo(this, logo);
}

ECode ApplicationPackageManager::GetApplicationLogoEx(
    /* [in] */ const String& packageName,
    /* [out] */ IDrawable** logo)
{
    VALIDATE_NOT_NULL(logo);

    AutoPtr<IApplicationInfo> info;
    GetApplicationInfo(packageName, 0, (IApplicationInfo**)&info);
    return GetApplicationLogo(info, logo);
}

ECode ApplicationPackageManager::GetResourcesForActivity(
    /* [in] */ IComponentName* activityName,
    /* [out] */ IResources** res)
{
     VALIDATE_NOT_NULL(res);

     AutoPtr<IActivityInfo> aInfo;
     GetActivityInfo(activityName, 0, (IActivityInfo**)&aInfo);
     AutoPtr<IApplicationInfo> info;
     aInfo->GetApplicationInfo((IApplicationInfo**)&info);
     return GetResourcesForApplication(info, res);
}

ECode ApplicationPackageManager::GetResourcesForApplication(
    /* [in] */ IApplicationInfo* app,
    /* [out] */ IResources** res)
{
    VALIDATE_NOT_NULL(res);
    *res = NULL;

    String pkgName;
    app->GetPackageName(&pkgName);
    if (pkgName.Equals("system")) {
        AutoPtr<IContextImpl> sysContent;
        mContext->mMainThread->GetSystemContext((IContextImpl**)&sysContent);
        return sysContent->GetResources(res);
    }

    String resDir;
    Int32 appUid;
    app->GetUid(&appUid);
    if (appUid == Process::MyUid()) {
        app->GetSourceDir(&resDir);
    }
    else {
        app->GetPublicSourceDir(&resDir);
    }
    AutoPtr<IResources> r;
    mContext->mMainThread->GetTopLevelResources(
        resDir, IDisplay::DEFAULT_DISPLAY, NULL, mContext->mPackageInfo, (IResources**)&r);
    if (r != NULL) {
        *res = r;
        INTERFACE_ADDREF(*res);
        return NOERROR;
    }

    Slogger::E("ApplicationPackageManager", "Unable to open [%s]",  resDir.string());
    return E_NAME_NOT_FOUND_EXCEPTION;
}

ECode ApplicationPackageManager::GetResourcesForApplicationEx(
    /* [in] */ const String& appPackageName,
    /* [out] */ IResources** res)
{
    VALIDATE_NOT_NULL(res);

    AutoPtr<IApplicationInfo> info;
    GetApplicationInfo(appPackageName, 0, (IApplicationInfo**)&info);
    return GetResourcesForApplication(info, res);
}

ECode ApplicationPackageManager::GetResourcesForApplicationAsUser(
    /* [in] */ const String& appPackageName,
    /* [in] */ Int32 userId,
    /* [out] */ IResources** res)
{
    VALIDATE_NOT_NULL(res);
    *res = NULL;

    if (userId < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (appPackageName.Equals("system")) {
        AutoPtr<IContextImpl> ctx;
        mContext->mMainThread->GetSystemContext((IContextImpl**)&ctx);
        return ctx->GetResources(res);
    }

    // try {
    AutoPtr<IApplicationInfo> ai;
    if (FAILED(mPM->GetApplicationInfo(appPackageName, 0, userId, (IApplicationInfo**)&ai))) {
        return E_RUNTIME_EXCEPTION;
    }

    if (ai != NULL) {
        return GetResourcesForApplication(ai, res);
    }
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }

    Slogger::E("ApplicationPackageManager", "UPackage [%s] doesn't exist",  appPackageName.string());
    return E_NAME_NOT_FOUND_EXCEPTION;
}

ECode ApplicationPackageManager::IsSafeMode(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    // try {
    if (mCachedSafeMode < 0) {
        Boolean isSafeMode = FALSE;
        if (FAILED(mPM->IsSafeMode(&isSafeMode))) {
            Slogger::E(TAG, "Package manager has died");
            return E_RUNTIME_EXCEPTION;
        }
        mCachedSafeMode = isSafeMode ? 1 : 0;
    }
    *result = mCachedSafeMode != 0;
    return NOERROR;
    // } catch (RemoteException e) {
    //     throw new RuntimeException("Package manager has died", e);
    // }
}

void ApplicationPackageManager::ConfigurationChanged()
{
    Mutex::Autolock lock(sSync);
    sIconCache.Clear();
    sStringCache.Clear();
}

AutoPtr<IDrawable> ApplicationPackageManager::GetCachedIcon(
    /* [in] */ ResourceName* name)
{
    Mutex::Autolock lock(sSync);
    HashMap<AutoPtr<ResourceName>, AutoPtr<IWeakReference> >::Iterator it = sIconCache.Find(name);
    if (it != sIconCache.End()) {
        AutoPtr<IWeakReference> wr = it->mSecond;
        AutoPtr<IDrawableConstantState> state;
        wr->Resolve(EIID_IDrawableConstantState, (IInterface**)&state);

        if (state != NULL) {
            if (DEBUG_ICONS) {
                Logger::V(TAG, "Get cached drawable state for %s : %p", name->ToString().string(), state.Get());
            }
            // Note: It's okay here to not use the newDrawable(Resources) variant
            //       of the API. The ConstantState comes from a drawable that was
            //       originally created by passing the proper app Resources instance
            //       which means the state should already contain the proper
            //       resources specific information (like density.) See
            //       BitmapDrawable.BitmapState for instance.
            AutoPtr<IDrawable> dr;
            state->NewDrawable((IDrawable**)&dr);
            return dr;
        }

        // our entry has been purged
        sIconCache.Erase(it);
    }
    return NULL;
}

void ApplicationPackageManager::PutCachedIcon(
    /* [in] */ ResourceName* name,
    /* [in] */ IDrawable* dr)
{
    assert(dr != NULL);
    Mutex::Autolock lock(sSync);
    AutoPtr<IDrawableConstantState> state;
    dr->GetConstantState((IDrawableConstantState**)&state);

    IWeakReferenceSource* source = IWeakReferenceSource::Probe(state);
    if (source != NULL) {
        AutoPtr<IWeakReference> wr;
        source->GetWeakReference((IWeakReference**)&wr);
        sIconCache[name] = wr;
        if (DEBUG_ICONS) Logger::V(TAG, "Added cached drawable state for %s : %p", name->ToString().string(), dr);
    }
}

void ApplicationPackageManager::HandlePackageBroadcast(
    /* [in] */ Int32 cmd,
    /* [in] */ ArrayOf<String>* pkgList,
    /* [in] */ Boolean hasPkgInfo)
{
    Boolean immediateGc = FALSE;
    if (cmd == IApplicationThread::EXTERNAL_STORAGE_UNAVAILABLE) {
        immediateGc = TRUE;
    }

    if (pkgList != NULL && (pkgList->GetLength() > 0)) {
        Boolean needCleanup = FALSE;
        String ssp;
        for (Int32 i = 0; i < pkgList->GetLength(); ++i) {
            ssp = (*pkgList)[i];
            {
                Mutex::Autolock lock(sSync);
                if (!sIconCache.IsEmpty()) {
                    HashMap<AutoPtr<ResourceName>, AutoPtr<IWeakReference> >::Iterator it;
                    for (it = sIconCache.Begin(); it != sIconCache.End();) {
                        AutoPtr<ResourceName> nm = it->mFirst;
                        if (nm->mPackageName.Equals(ssp)) {
                            if (DEBUG_ICONS) Logger::V(TAG, "Removing cached drawable for %s", nm->ToString().string());
                            sIconCache.Erase(it++);
                            needCleanup = true;
                        }
                        else {
                            ++it;
                        }
                    }
                }

                if (!sStringCache.IsEmpty()) {
                    HashMap<AutoPtr<ResourceName>, AutoPtr<IWeakReference> >::Iterator it;
                    for (it = sStringCache.Begin(); it != sStringCache.End();) {
                        AutoPtr<ResourceName> nm = it->mFirst;
                        if (nm->mPackageName.Equals(ssp)) {
                            if (DEBUG_ICONS) Logger::V(TAG, "Removing cached string for %s", nm->ToString().string());
                            sStringCache.Erase(it++);
                            needCleanup = true;
                        }
                        else {
                            ++it;
                        }
                    }
                }
            }
        }

        if (needCleanup || hasPkgInfo) {
            if (immediateGc) {
                // Schedule an immediate gc.
                // Runtime.getRuntime().gc();
            } else {
                // ActivityThread.currentActivityThread().scheduleGcIdler();
            }
        }
    }
}

AutoPtr<ICharSequence> ApplicationPackageManager::GetCachedString(
    /* [in] */ ResourceName* name)
{
    Mutex::Autolock lock(sSync);

    HashMap<AutoPtr<ResourceName>, AutoPtr<IWeakReference> >::Iterator it = sStringCache.Find(name);
    if (it != sIconCache.End()) {
        AutoPtr<IWeakReference> wr = it->mSecond;
        AutoPtr<ICharSequence> cs;
        wr->Resolve(EIID_ICharSequence, (IInterface**)&cs);
        if (cs != NULL) {
            if (DEBUG_ICONS) {
                String str;
                cs->ToString(&str);
                Logger::V(TAG, "Get cached string for %s : %s wr:%p",
                    name->ToString().string(), str.string(), wr.Get());
            }

            return cs;
        }

        // our entry has been purged
        sStringCache.Erase(it);
    }

    return NULL;
}

void ApplicationPackageManager::PutCachedString(
    /* [in] */ ResourceName* name,
    /* [in] */ ICharSequence* cs)
{
    Mutex::Autolock lock(sSync);

    AutoPtr<IWeakReferenceSource> source = IWeakReferenceSource::Probe(cs);
    if (source != NULL) {
        AutoPtr<IWeakReference> wr;
        source->GetWeakReference((IWeakReference**)&wr);
        sStringCache[name] = wr;

        if (DEBUG_ICONS) {
            String str;
            cs->ToString(&str);
            Logger::V(TAG, "Added cached string for %s : %s wr:%p",
                name->ToString().string(), str.string(), wr.Get());
        }
    }
}

ECode ApplicationPackageManager::GetText(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 resid,
    /* [in] */ IApplicationInfo* appInfo,
    /* [out] */ ICharSequence** _text)
{
    VALIDATE_NOT_NULL(_text);
    *_text = NULL;

    AutoPtr<ResourceName> name = new ResourceName(packageName, resid);
    AutoPtr<ICharSequence> text = GetCachedString(name);
    if (text != NULL) {
        *_text = text;
        INTERFACE_ADDREF(*_text);
        return NOERROR;
    }

    ECode ec;
    if (appInfo == NULL) {
        ec = GetApplicationInfo(packageName, 0, (IApplicationInfo**)&appInfo);
        if (ec = (ECode)E_NAME_NOT_FOUND_EXCEPTION) {
            return NOERROR;
        }
    }

    AutoPtr<IResources> r;
    FAIL_GOTO((ec = GetResourcesForApplication(appInfo, (IResources**)&r)), ERROR)
    FAIL_GOTO((ec = r->GetText(resid, (ICharSequence**)&text)), ERROR)
    PutCachedString(name, text);
    *_text = text;
    INTERFACE_ADDREF(*_text)
    return NOERROR;

ERROR:
    switch(ec) {
        case E_NAME_NOT_FOUND_EXCEPTION:
        {
            String pkgName;
            Slogger::W(String("PackageManager"), "Failure retrieving resources for %s"
                  , (appInfo->GetPackageName(&pkgName), pkgName).string());
            ec = NOERROR;
            break;
        }
        case E_RUNTIME_EXCEPTION:
        {
           // If an exception was thrown, fall through to return
           // default icon.
            Slogger::W(String("PackageManager"), "Failure retrieving text 0x%08x  in package %s"
                , resid, packageName.string());
            ec = NOERROR;
            break;
        }
    }
    return ec;
}

ECode ApplicationPackageManager::GetXml(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 resid,
    /* [in] */ IApplicationInfo* _appInfo,
    /* [out] */ IXmlResourceParser** parser)
{
    VALIDATE_NOT_NULL(parser);
    AutoPtr<IApplicationInfo> appInfo = _appInfo;
    if (appInfo == NULL) {
        // try {
        ECode ec = GetApplicationInfo(packageName, 0, (IApplicationInfo**)&appInfo);
        if (ec == E_NAME_NOT_FOUND_EXCEPTION) {
            *parser = NULL;
            return ec;
        }
        // } catch (NameNotFoundException e) {
        //     return null;
        // }
    }

    // try {
    AutoPtr<IResources> r;
    GetResourcesForApplication(appInfo, (IResources**)&r);
    return r->GetXml(resid, parser);
    // } catch (RuntimeException e) {
    //     // If an exception was thrown, fall through to return
    //     // default icon.
    //     Log.w("PackageManager", "Failure retrieving xml 0x"
    //           + Integer.toHexString(resid) + " in package "
    //           + packageName, e);
    // } catch (NameNotFoundException e) {
    //     Log.w("PackageManager", "Failure retrieving resources for "
    //           + appInfo.packageName);
    // }
    // return null;
}

ECode ApplicationPackageManager::GetApplicationLabel(
    /* [in] */ IApplicationInfo* info,
    /* [out] */ ICharSequence** label)
{
    assert(info != NULL);
    return info->LoadLabel((IPackageManager*)this->Probe(EIID_IPackageManager), label);
}

ECode ApplicationPackageManager::InstallPackage(
    /* [in] */ IUri* packageURI,
    /* [in] */ IPackageInstallObserver* observer,
    /* [in] */ Int32 flags,
    /* [in] */ const String& installerPackageName)
{
    // try {
    return mPM->InstallPackage(packageURI, observer, flags, installerPackageName);
    // } catch (RemoteException e) {
    //     // Should never happen!
    // }
}

ECode ApplicationPackageManager::InstallPackageWithVerification(
    /* [in] */ IUri* packageURI,
    /* [in] */ IPackageInstallObserver* observer,
    /* [in] */ Int32 flags,
    /* [in] */ const String& installerPackageName,
    /* [in] */ IUri* verificationURI,
    /* [in] */ IManifestDigest* manifestDigest,
    /* [in] */ IContainerEncryptionParams* encryptionParams)
{
    // try {
    return mPM->InstallPackageWithVerification(packageURI, observer, flags, installerPackageName,
                verificationURI, manifestDigest, encryptionParams);
    // } catch (RemoteException e) {
    //     // Should never happen!
    // }
}

ECode ApplicationPackageManager::InstallPackageWithVerificationAndEncryption(
    /* [in] */ IUri* packageURI,
    /* [in] */ IPackageInstallObserver* observer,
    /* [in] */ Int32 flags,
    /* [in] */ const String& installerPackageName,
    /* [in] */ IVerificationParams* verificationParams,
    /* [in] */ IContainerEncryptionParams* encryptionParams)
{
    // try {
    return mPM->InstallPackageWithVerificationAndEncryption(packageURI, observer, flags,
                installerPackageName, verificationParams, encryptionParams);
    // } catch (RemoteException e) {
    //     // Should never happen!
    // }
}

ECode ApplicationPackageManager::InstallExistingPackage(
    /* [in] */ const String& packageName,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    ECode ec = mPM->InstallExistingPackage(packageName, result);
    if (FAILED(ec)) {
        *result = 0;
        Slogger::W("PackageManager", "Package %s doesn't exist", packageName.string());
        return E_NAME_NOT_FOUND_EXCEPTION;
    }

    if (*result == INSTALL_FAILED_INVALID_URI) {
        Slogger::W("PackageManager", "Package %s doesn't exist", packageName.string());
        return E_NAME_NOT_FOUND_EXCEPTION;
    }
    return NOERROR;
}

ECode ApplicationPackageManager::VerifyPendingInstall(
    /* [in] */ Int32 id,
    /* [in] */ Int32 verificationCode)
{
    return mPM->VerifyPendingInstall(id, verificationCode);
}

ECode ApplicationPackageManager::ExtendVerificationTimeout(
    /* [in] */ Int32 id,
    /* [in] */ Int32 verificationCodeAtTimeout,
    /* [in] */ Int64 millisecondsToDelay)
{
    return mPM->ExtendVerificationTimeout(id, verificationCodeAtTimeout, millisecondsToDelay);
}

ECode ApplicationPackageManager::SetInstallerPackageName(
    /* [in] */ const String& targetPackage,
    /* [in] */ const String& installerPackageName)
{
    return mPM->SetInstallerPackageName(targetPackage, installerPackageName);
}

ECode ApplicationPackageManager::MovePackage(
    /* [in] */ const String& packageName,
    /* [in] */ IPackageMoveObserver* observer,
    /* [in] */ Int32 flags)
{
    return mPM->MovePackage(packageName, observer, flags);
}

ECode ApplicationPackageManager::GetInstallerPackageName(
    /* [in] */ const String& packageName,
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    return mPM->GetInstallerPackageName(packageName, name);
}

ECode ApplicationPackageManager::DeletePackage(
    /* [in] */ const String& packageName,
    /* [in] */ IPackageDeleteObserver* observer,
    /* [in] */ Int32 flags)
{
    return mPM->DeletePackage(packageName, observer, flags);
}

ECode ApplicationPackageManager::ClearApplicationUserData(
    /* [in] */ const String& packageName,
    /* [in] */ IPackageDataObserver* observer)
{
    Int32 uid = 0;
    mContext->GetUserId(&uid);
    return mPM->ClearApplicationUserData(packageName, observer, uid);
}

ECode ApplicationPackageManager::DeleteApplicationCacheFiles(
    /* [in] */ const String& packageName,
    /* [in] */ IPackageDataObserver* observer)
{
    return mPM->DeleteApplicationCacheFiles(packageName, observer);
}

ECode ApplicationPackageManager::FreeStorageAndNotify(
    /* [in] */ Int64 freeStorageSize,
    /* [in] */ IPackageDataObserver* observer)
{
    return mPM->FreeStorageAndNotify(freeStorageSize, observer);
}

ECode ApplicationPackageManager::FreeStorage(
    /* [in] */ Int64 freeStorageSize,
    /* [in] */ IIntentSender* pi)
{
    return mPM->FreeStorage(freeStorageSize, pi);
}

ECode ApplicationPackageManager::GetPackageSizeInfo(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 userHandle,
    /* [in] */ IPackageStatsObserver* observer)
{
    return mPM->GetPackageSizeInfo(packageName, userHandle, observer);
}

ECode ApplicationPackageManager::AddPackageToPreferred(
    /* [in] */ const String& packageName)
{
    return mPM->AddPackageToPreferred(packageName);
}

ECode ApplicationPackageManager::RemovePackageFromPreferred(
    /* [in] */ const String& packageName)
{
    return mPM->RemovePackageFromPreferred(packageName);
}

ECode ApplicationPackageManager::GetPreferredPackages(
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** packages)
{
    VALIDATE_NOT_NULL(packages);
    ECode ec = mPM->GetPreferredPackages(flags, packages);
    if (FAILED(ec)) {
        CParcelableObjectContainer::New(packages);
    }

    return NOERROR;
}

ECode ApplicationPackageManager::AddPreferredActivity(
    /* [in] */ IIntentFilter* filter,
    /* [in] */ Int32 match,
    /* [in] */ ArrayOf<IComponentName*>* set,
    /* [in] */ IComponentName* activity)
{
    Int32 uid = 0;
    mContext->GetUserId(&uid);
    return mPM->AddPreferredActivity(filter, match, set, activity, uid);
}

ECode ApplicationPackageManager::AddPreferredActivityEx(
    /* [in] */ IIntentFilter* filter,
    /* [in] */ Int32 match,
    /* [in] */ ArrayOf<IComponentName*>* set,
    /* [in] */ IComponentName* activity,
    /* [in] */ Int32 userId)
{
    return mPM->AddPreferredActivity(filter, match, set, activity, userId);
}

ECode ApplicationPackageManager::ReplacePreferredActivity(
    /* [in] */ IIntentFilter* filter,
    /* [in] */ Int32 match,
    /* [in] */ ArrayOf<IComponentName*>* set,
    /* [in] */ IComponentName* activity)
{
    return mPM->ReplacePreferredActivity(filter, match, set, activity);
}

ECode ApplicationPackageManager::ClearPackagePreferredActivities(
    /* [in] */ const String& packageName)
{
    return mPM->ClearPackagePreferredActivities(packageName);
}

ECode ApplicationPackageManager::GetPreferredActivities(
    /* [in] */ IObjectContainer* outFilters,
    /* [in] */ IObjectContainer* outActivities,
    /* [in] */ const String& packageName,
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num);
    ECode ec = mPM->GetPreferredActivities(outFilters, outActivities, packageName, num);
    if (FAILED(ec)) {
        *num = 0;
    }

    return NOERROR;
}

ECode ApplicationPackageManager::SetComponentEnabledSetting(
    /* [in] */ IComponentName* componentName,
    /* [in] */ Int32 newState,
    /* [in] */ Int32 flags)
{
    Int32 uid = 0;
    mContext->GetUserId(&uid);
    return mPM->SetComponentEnabledSetting(componentName, newState, flags, uid);
}

ECode ApplicationPackageManager::GetComponentEnabledSetting(
    /* [in] */ IComponentName* componentName,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 uid = 0;
    mContext->GetUserId(&uid);
    ECode ec = mPM->GetComponentEnabledSetting(componentName, uid, value);
    if (FAILED(ec)) {
        *value = IPackageManager::COMPONENT_ENABLED_STATE_DEFAULT;
    }

    return NOERROR;
}

ECode ApplicationPackageManager::SetApplicationEnabledSetting(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 newState,
    /* [in] */ Int32 flags)
{
    Int32 uid = 0;
    mContext->GetUserId(&uid);
    return mPM->SetApplicationEnabledSetting(packageName, newState, flags, uid);
}

ECode ApplicationPackageManager::GetApplicationEnabledSetting(
    /* [in] */ const String& packageName,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 uid = 0;
    mContext->GetUserId(&uid);
    ECode ec = mPM->GetApplicationEnabledSetting(packageName, uid, value);
    if (FAILED(ec)) {
        *value = IPackageManager::COMPONENT_ENABLED_STATE_DEFAULT;
    }

    return NOERROR;
}

ECode ApplicationPackageManager::GetVerifierDeviceIdentity(
    /* [out] */ IVerifierDeviceIdentity** identity)
{
    VALIDATE_NOT_NULL(identity);
    ECode ec = mPM->GetVerifierDeviceIdentity(identity);
    if (FAILED(ec)) {
        *identity = NULL;
    }

    return NOERROR;
}

ECode ApplicationPackageManager::GetPackageArchiveInfo(
    /* [in] */ const String& archiveFilePath,
    /* [in] */ Int32 flags,
    /* [out] */ IPackageInfo** info)
{
    VALIDATE_NOT_NULL(info);
    return PackageManager::GetPackageArchiveInfo(archiveFilePath, flags, info);
}

ECode ApplicationPackageManager::GetPackageSizeInfoEx(
    /* [in] */ const String& packageName,
    /* [in] */ IPackageStatsObserver* observer)
{
    return PackageManager::GetPackageSizeInfo(packageName, observer);
}

ApplicationPackageManager::ResourceName::ResourceName(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 iconId)
    : mPackageName(packageName)
    , mIconId(iconId)
{}

ApplicationPackageManager::ResourceName::ResourceName(
    /* [in] */ IApplicationInfo* aInfo,
    /* [in] */ Int32 iconId)
{
    String pkgName;
    aInfo->GetPackageName(&pkgName);
    ResourceName(pkgName, iconId);
}

ApplicationPackageManager::ResourceName::ResourceName(
    /* [in] */ IComponentInfo* cInfo,
    /* [in] */ Int32 iconId)
{
    AutoPtr<IApplicationInfo> appInfo;
    cInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
    String pkgName;
    appInfo->GetPackageName(&pkgName);
    ResourceName(pkgName, iconId);
}

ApplicationPackageManager::ResourceName::ResourceName(
    /* [in] */ IResolveInfo* rInfo,
    /* [in] */ Int32 iconId)
{
    AutoPtr<IActivityInfo> acInfo;
    rInfo->GetActivityInfo((IActivityInfo**)&acInfo);
    AutoPtr<IApplicationInfo> appInfo;
    acInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
    String pkgName;
    appInfo->GetPackageName(&pkgName);
    ResourceName(pkgName, iconId);
}

Boolean ApplicationPackageManager::ResourceName::Equals(
    /* [in] */ ResourceName* o)
{
    if (this == o) return TRUE;

    if (mIconId != o->mIconId) return FALSE;
    return !(!mPackageName.IsNull() ?
        !mPackageName.Equals(o->mPackageName) : !o->mPackageName.IsNull());

}

Int32 ApplicationPackageManager::ResourceName::GetHashCode()
{
    Int32 result;
    result = mPackageName.GetHashCode();
    result = 31 * result + mIconId;
    return result;
}

String ApplicationPackageManager::ResourceName::ToString()
{
    StringBuilder sb("{ResourceName ");
    sb += mPackageName;
    sb += " / ";
    sb += mIconId;
    sb += "}";
    return sb.ToString();
}

} // namespace App
} // namespace Droid
} // namespace Elastos
