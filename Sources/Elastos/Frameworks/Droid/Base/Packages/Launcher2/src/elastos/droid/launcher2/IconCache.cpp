
#include "elastos/droid/launcher2/IconCache.h"
#include "elastos/droid/launcher2/Utilities.h"
#include "elastos/droid/os/Process.h"
#include "Elastos.Droid.App.h"
#include "Elastos.Droid.Service.h"
#include "elastos/droid/R.h"
#include <elastos/core/Math.h>
#include <elastos/core/CoreUtils.h>
#include <elastos/core/AutoLock.h>
#include "R.h"

using Elastos::Droid::App::IActivityManager;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IResourcesHelper;
using Elastos::Droid::Content::Res::CResourcesHelper;
using Elastos::Droid::Content::Pm::IComponentInfo;
using Elastos::Droid::Content::Pm::IResolveInfo;
using Elastos::Droid::Content::Pm::ILauncherApps;
using Elastos::Droid::Graphics::CBitmapHelper;
using Elastos::Droid::Graphics::IBitmapHelper;
using Elastos::Droid::Graphics::CCanvas;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::BitmapConfig_ARGB_8888;
using Elastos::Droid::Os::Process;
using Elastos::Core::CoreUtils;
using Elastos::Utility::CHashMap;

namespace Elastos {
namespace Droid {
namespace Launcher2 {

IconCache::CacheKey::CacheKey(
    /* [in] */ IComponentName* componentName,
    /* [in] */ IUserHandle* user)
    : mComponentName(componentName)
    , mUser(user)
{
}

ECode IconCache::CacheKey::GetHashCode(
    /* [out] */ Int32 *hashCode)
{
    VALIDATE_NOT_NULL(hashCode);

    Int32 code;
    IObject::Probe(mComponentName)->GetHashCode(&code);
    Int32 code2;
    IObject::Probe(mUser)->GetHashCode(&code2);
    *hashCode = code + code2;
    return NOERROR;
}

ECode IconCache::CacheKey::Equals(
    /* [in] */ IInterface *obj,
    /* [out] */ Boolean *equal)
{
    VALIDATE_NOT_NULL(equal);

    CacheKey* other = (CacheKey*)IObject::Probe(obj);
    Boolean res1;
    IObject::Probe(other->mComponentName)->Equals(mComponentName, &res1);
    Boolean res2;
    IObject::Probe(other->mUser)->Equals(mUser, &res2);
    *equal = res1 && res2;
    return NOERROR;
}

const String IconCache::TAG("Launcher.IconCache");

const Int32 IconCache::INITIAL_ICON_CACHE_CAPACITY = 50;

CAR_INTERFACE_IMPL(IconCache, Object, IIconCache);

IconCache::IconCache()
    : mIconDpi(0)
{
}

ECode IconCache::constructor(
    /* [in] */ ILauncherApplication* context)
{
    CHashMap::New(INITIAL_ICON_CACHE_CAPACITY, (IHashMap**)&mCache);

    AutoPtr<IInterface> obj;
    IContext::Probe(context)->GetSystemService(IContext::ACTIVITY_SERVICE, (IInterface**)&obj);
    AutoPtr<IActivityManager> activityManager = IActivityManager::Probe(obj);

    mContext = context;
    IContext::Probe(context)->GetPackageManager((IPackageManager**)&mPackageManager);
    activityManager->GetLauncherLargeIconDensity(&mIconDpi);
    // need to set mIconDpi before getting default icon
    mDefaultIcon = MakeDefaultIcon();
    return NOERROR;
}

AutoPtr<IDrawable> IconCache::GetFullResDefaultActivityIcon()
{
    AutoPtr<IResources> resources;
    AutoPtr<IResourcesHelper> resourcesHelper;
    CResourcesHelper::AcquireSingleton((IResourcesHelper**)&resourcesHelper);
    resourcesHelper->GetSystem((IResources**)&resources);
    AutoPtr<IUserHandle> user;
    Process::MyUserHandle((IUserHandle**)&user);
    return GetFullResIcon(resources,
            Elastos::Droid::R::mipmap::sym_def_app_icon, user);
}

AutoPtr<IDrawable> IconCache::GetFullResIcon(
    /* [in] */ IResources* resources,
    /* [in] */ Int32 iconId,
    /* [in] */ IUserHandle* user)
{
    AutoPtr<IDrawable> d;
    //try {
    ECode ec = resources->GetDrawableForDensity(iconId, mIconDpi, (IDrawable**)&d);
    //} catch (Resources.NotFoundException e) {
    if (ec == (ECode)E_NOT_FOUND_EXCEPTION) {
        d = NULL;
    }
    //}

    if (d == NULL) {
        d = GetFullResDefaultActivityIcon();
    }

    AutoPtr<IDrawable> res;
    mPackageManager->GetUserBadgedIcon(d, user, (IDrawable**)&res);
    return res;
}

AutoPtr<IDrawable> IconCache::GetFullResIcon(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 iconId,
    /* [in] */ IUserHandle* user)
{
    AutoPtr<IResources> resources;
    //try {
    // TODO: Check if this needs to use the user param if we support
    // shortcuts/widgets from other profiles. It won't work as is
    // for packages that are only available in a different user profile.
    ECode ec = mPackageManager->GetResourcesForApplication(packageName, (IResources**)&resources);
    //} catch (PackageManager.NameNotFoundException e) {
    if (ec == (ECode)E_NAME_NOT_FOUND_EXCEPTION) {
        resources = NULL;
    }
    //}
    if (resources != NULL) {
        if (iconId != 0) {
            return GetFullResIcon(resources, iconId, user);
        }
    }
    return GetFullResDefaultActivityIcon();
}

AutoPtr<IDrawable> IconCache::GetFullResIcon(
    /* [in] */ IResolveInfo* info,
    /* [in] */ IUserHandle* user)
{
    AutoPtr<IActivityInfo> activityInfo;
    info->GetActivityInfo((IActivityInfo**)&activityInfo);
    return GetFullResIcon(activityInfo, user);
}

AutoPtr<IDrawable> IconCache::GetFullResIcon(
    /* [in] */ IActivityInfo* info,
    /* [in] */ IUserHandle* user)
{
    AutoPtr<IResources> resources;
    //try {
    AutoPtr<Elastos::Droid::Content::Pm::IApplicationInfo> ainfo;
    IComponentInfo::Probe(info)->GetApplicationInfo((Elastos::Droid::Content::Pm::IApplicationInfo**)&ainfo);
    ECode ec = mPackageManager->GetResourcesForApplication(ainfo, (IResources**)&resources);
    //} catch (PackageManager.NameNotFoundException e) {
    if (ec == (ECode)E_NAME_NOT_FOUND_EXCEPTION) {
        resources = NULL;
    }
    //}
    if (resources != NULL) {
        Int32 iconId;
        IComponentInfo::Probe(info)->GetIconResource(&iconId);
        if (iconId != 0) {
            return GetFullResIcon(resources, iconId, user);
        }
    }
    return GetFullResDefaultActivityIcon();
}

AutoPtr<IBitmap> IconCache::MakeDefaultIcon()
{
    AutoPtr<IDrawable> d = GetFullResDefaultActivityIcon();
    AutoPtr<IBitmap> b;
    Int32 width;
    d->GetIntrinsicWidth(&width);
    Int32 height;
    d->GetIntrinsicHeight(&height);
    AutoPtr<IBitmapHelper> helper;
    CBitmapHelper::AcquireSingleton((IBitmapHelper**)&helper);
    helper->CreateBitmap(Elastos::Core::Math::Max(width, 1),
            Elastos::Core::Math::Max(height, 1),
            BitmapConfig_ARGB_8888, (IBitmap**)&b);
    AutoPtr<ICanvas> c;
    CCanvas::New(b, (ICanvas**)&c);
    Int32 width2;
    b->GetWidth(&width2);
    Int32 height2;
    b->GetHeight(&height2);
    d->SetBounds(0, 0, width2, height2);
    d->Draw(c);
    c->SetBitmap(NULL);
    return b;
}

void IconCache::Remove(
    /* [in] */ IComponentName* componentName)
{
    synchronized(mCacheLock) {
        mCache->Remove(TO_IINTERFACE(componentName));
    }
    return;
}

void IconCache::Flush()
{
    synchronized(mCacheLock) {
        mCache->Clear();
    }
    return;
}

void IconCache::GetTitleAndIcon(
    /* [in] */ IApplicationInfo* application,
    /* [in] */ ILauncherActivityInfo* info,
    /* [in] */ IHashMap* labelCache)
{
    synchronized(mCacheLock) {
        assert(0 && "need class ApplicationInfo");
        // ApplicationInfo* _info = (ApplicationInfo*)application;
        // AutoPtr<IUserHandle> handle;
        // info->GetUser((IUserHandle**)&handle);
        // AutoPtr<CacheEntry> entry = CacheLocked(ApplicationInfo->mComponentName, info, labelCache,
        //         handle);

        // _info->mIitle = entry->mTitle;
        // _info->mIconBitmap = entry->mIcon;
        // _info->mContentDescription = entry->mContentDescription;
    }
    return;
}

AutoPtr<IBitmap> IconCache::GetIcon(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    synchronized(mCacheLock) {
        AutoPtr<IInterface> obj;
        IContext::Probe(mContext)->GetSystemService(IContext::LAUNCHER_APPS_SERVICE, (IInterface**)&obj);
        AutoPtr<ILauncherApps> launcherApps = ILauncherApps::Probe(obj);
        AutoPtr<ILauncherActivityInfo> launcherActInfo;
        launcherApps->ResolveActivity(intent, user, (ILauncherActivityInfo**)&launcherActInfo);
        AutoPtr<IComponentName> component;
        intent->GetComponent((IComponentName**)&component);

        if (launcherActInfo == NULL || component == NULL) {
            return mDefaultIcon;
        }

        AutoPtr<CacheEntry> entry = CacheLocked(component, launcherActInfo, NULL, user);
        return entry->mIcon;
    }
    return NULL;
}

AutoPtr<IBitmap> IconCache::GetIcon(
    /* [in] */ IComponentName* component,
    /* [in] */ ILauncherActivityInfo* info,
    /* [in] */ IHashMap* labelCache)
{
    synchronized(mCacheLock) {
        if (info == NULL || component == NULL) {
            return NULL;
        }

        AutoPtr<IUserHandle> handle;
        info->GetUser((IUserHandle**)&handle);
        AutoPtr<CacheEntry> entry = CacheLocked(component, info, labelCache, handle);
        return entry->mIcon;
    }
    return NULL;
}

Boolean IconCache::IsDefaultIcon(
    /* [in] */ IBitmap* icon)
{
    return TO_IINTERFACE(mDefaultIcon) == TO_IINTERFACE(icon);
}

AutoPtr<IconCache::CacheEntry> IconCache::CacheLocked(
    /* [in] */ IComponentName* componentName,
    /* [in] */ ILauncherActivityInfo* info,
    /* [in] */ IHashMap* labelCache,
    /* [in] */ IUserHandle* user)
{
    AutoPtr<CacheKey> cacheKey = new CacheKey(componentName, user);
    AutoPtr<IInterface> obj;
    mCache->Get(TO_IINTERFACE(cacheKey), (IInterface**)&obj);
    AutoPtr<IconCache::CacheEntry> entry = (IconCache::CacheEntry*)IObject::Probe(obj);
    if (entry == NULL) {
        entry = new IconCache::CacheEntry();

        mCache->Put(TO_IINTERFACE(cacheKey), TO_IINTERFACE(entry));

        AutoPtr<IComponentName> key;
        info->GetComponentName((IComponentName**)&key);
        if (labelCache != NULL) {
            Boolean res;
            labelCache->ContainsKey(TO_IINTERFACE(key), &res);
            if (res) {
                AutoPtr<IInterface> obj;
                labelCache->Get(TO_IINTERFACE(key), (IInterface**)&obj);
                AutoPtr<ICharSequence> cchar = ICharSequence::Probe(obj);
                cchar->ToString(&(entry->mTitle));
            }
        }
        else {
            AutoPtr<ICharSequence> lable;
            info->GetLabel((ICharSequence**)&lable);
            lable->ToString(&(entry->mTitle));
            if (labelCache != NULL) {
                AutoPtr<ICharSequence> cchar = CoreUtils::Convert(entry->mTitle);
                labelCache->Put(TO_IINTERFACE(key), TO_IINTERFACE(cchar));
            }
        }
        if (entry->mTitle == NULL) {
            AutoPtr<IComponentName> name;
            info->GetComponentName((IComponentName**)&name);
            name->GetShortClassName(&(entry->mTitle));
        }
        AutoPtr<ICharSequence> label = CoreUtils::Convert(entry->mTitle);;
        mPackageManager->GetUserBadgedLabel(label, user, (ICharSequence**)&(entry->mContentDescription));
        AutoPtr<IDrawable> drawable;
        info->GetBadgedIcon(mIconDpi, (IDrawable**)&drawable);
        entry->mIcon = Utilities::CreateIconBitmap(drawable, IContext::Probe(mContext));
    }
    return entry;
}


} // namespace Launcher2
} // namespace Droid
} // namespace Elastos