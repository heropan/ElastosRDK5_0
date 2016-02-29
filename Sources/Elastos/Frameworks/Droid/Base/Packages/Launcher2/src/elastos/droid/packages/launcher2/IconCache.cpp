
#include "elastos/droid/packages/launcher2/IconCache.h"
#include "Elastos.Droid.Service.h"
#include "R.h"

namespace Elastos {
namespace Droid {
namespace Packages {
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

    CacheKey* other = (CacheKey*)IObject::Probe(o);
    Boolean res;
    IObject::Probe(other->mComponentName)->Equals(componentName, &res);
    Boolean res2;
    IObject::Probe(other->mUser)->Equals(user, &res2);
    *equal = user && res2;
    return NOERROR;
}

const String IconCache::TAG("Launcher.IconCache");

const Int32 IconCache::INITIAL_ICON_CACHE_CAPACITY = 50;

IconCache::IconCache(
    /* [in] */ ILauncherApplication* context)
{
    CHashMap::New(INITIAL_ICON_CACHE_CAPACITY, (IHashMap**)&mCache);

    AutoPtr<IInterface> obj;
    context->GetSystemService(IContext::ACTIVITY_SERVICE, (IInterface**)&obj);
    AutoPtr<IActivityManager> activityManager = IActivityManager::Probe(obj);

    mContext = context;
    context->GetPackageManager((IPackageManager**)&mPackageManager);
    activityManager->GetLauncherLargeIconDensity(&mIconDpi);
    // need to set mIconDpi before getting default icon
    mDefaultIcon = MakeDefaultIcon();
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
            Elastos::Droid::Packages::Launcher2::R::mipmap::sym_def_app_icon, user);
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
    AutoPtr<IApplicationInfo> ainfo;
    IComponentInfo::Probe(info)->GetApplicationInfo((IApplicationInfo**)&ainfo);
    ECode ec = mPackageManager->GetResourcesForApplication(ainfo, (IResources**)&resources);
    //} catch (PackageManager.NameNotFoundException e) {
    if (ec == (ECode)E_NAME_NOT_FOUND_EXCEPTION) {
        resources = NULL;
    }
    //}
    if (resources != NULL) {
        Int32 iconId;
        info->GetIconResource(7iconId);
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
    Bitmap::CreateBitmap(Math::Max(width, 1),
            Math::Max(height, 1),
            Bitmap::Config::ARGB_8888, (IBitmap**)&b);
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
        ApplicationInfo* _info = (ApplicationInfo*)application;
        AutoPtr<IUserHandle> handle;
        info->GetUser((IUserHandle**)&handle);
        AutoPtr<CacheEntry> entry = CacheLocked(ApplicationInfo->mComponentName, info, labelCache,
                handle);

        _info->mIitle = entry->mTitle;
        _info->mIconBitmap = entry->mIcon;
        _info->mContentDescription = entry->mContentDescription;
    }
    return;
}

AutoPtr<IBitmap> IconCache::GetIcon(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    synchronized(mCacheLock) {
        AutoPtr<IInterface> obj;
        mContext->GetSystemService(IContext::LAUNCHER_APPS_SERVICE, (IInterface**)&obj);
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
        AutoPtr<CacheEntry> entry = cacheLocked(component, info, labelCache, handle);
        return entry->mIcon;
    }
    return NULL;
}

Boolean IconCache::IsDefaultIcon(
    /* [in] */ IBitmap* icon)
{
    return mDefaultIcon == icon;
}

AutoPtr<CacheEntry> IconCache::CacheLocked(
    /* [in] */ IComponentName* componentName,
    /* [in] */ ILauncherActivityInfo* info,
    /* [in] */ IHashMap* labelCache,
    /* [in] */ IUserHandle* user)
{
    AutoPtr<CacheKey> cacheKey = new CacheKey(componentName, user);
    AutoPtr<IInterface> obj;
    mCache->Get(TO_IINTERFACE(cacheKey), (IInterface**)&obj);
    AutoPtr<CacheEntry> entry = (CacheEntry*)IObject::Probe(obj);
    if (entry == NULL) {
        entry = new CacheEntry();

        mCache->Put(TO_IINTERFACE(cacheKey), TO_IINTERFACE(entry));

        AutoPtr<IComponentName> key;
        info->GetComponentName((IComponentName**)&key);
        if (labelCache != NULL) {
            Boolean res;
            labelCache->ContainsKey(TO_IINTERFACE(key), &res);
            if (res) {
                AutoPtr<IInterface> obj;
                labelCache->Get(TO_IINTERFACE(key), (IInterface**)&obj);
                AutoPtr<CharSequence> cchar = CharSequence::Probe(obj);
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
        mPackageManager->GetUserBadgedLabel(entry->mTitle, user, (ICharSequence**)&(entry->mContentDescription));
        AutoPtr<IDrawable> drawable;
        info->GetBadgedIcon(mIconDpi, (IDrawable**)&drawable);
        entry->mIcon = Utilities::CreateIconBitmap(drawable, mContext);
    }
    return entry;
}


} // namespace Launcher2
} // namespace Packages
} // namespace Droid
} // namespace Elastos