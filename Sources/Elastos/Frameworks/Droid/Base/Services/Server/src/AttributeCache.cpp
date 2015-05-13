#include "AttributeCache.h"

using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::CUserHandle;
using Elastos::Droid::Content::Res::CConfiguration;
using Elastos::Droid::Content::Pm::IActivityInfo;

namespace Elastos {
namespace Droid {
namespace Server {

AutoPtr<AttributeCache> AttributeCache::sInstance;

AttributeCache::Entry::Entry(
    /* [in] */ IContext* c,
    /* [in] */ ITypedArray* ta)
    : mContext(c)
    , mArray(ta)
{
}

AttributeCache::Package::Package(
    /* [in] */ IContext* c)
{
    mContext = c;
}

AttributeCache::Package::~Package()
{
    if (mMap.GetSize() > 0) {
        EntryIterator it = mMap.Begin();
        for (; it != mMap.End(); it++) {
            HashMap<AutoPtr<ArrayOf<Int32> >, AutoPtr<Entry> >* map = it->mSecond;
            if (map != NULL) {
                delete map;
            }
        }
    }
}

void AttributeCache::Init(
        /* [in] */ IContext* context)
{
    if (sInstance == NULL)
        sInstance = new AttributeCache(context);
}

AutoPtr<AttributeCache> AttributeCache::GetInstance()
{
    assert(sInstance && "AttributeCache is not init");
    return sInstance;
}

AttributeCache::AttributeCache(
    /* [in] */ IContext* context)
    : mContext(context)
{
    CConfiguration::New((IConfiguration**)&mConfiguration);
}

AttributeCache::~AttributeCache()
{
    if (mPackages.GetSize() > 0) {
        HashMap<Int32, HashMap<String, AutoPtr<Package> >* >::Iterator it  = mPackages.Begin();
        for (;it != mPackages.End(); it++) {
            HashMap<String, AutoPtr<Package> >* map = it->mSecond;
            if (map != NULL) {
                delete map;
            }
        }
    }
}

void AttributeCache::UpdateConfiguration(
    /* [in] */ IConfiguration* config)
{
    Mutex::Autolock lock(mLock);
    Int32 changes;
    mConfiguration->UpdateFrom(config, &changes);
    if ((changes & ~(IActivityInfo::CONFIG_FONT_SCALE |
            IActivityInfo::CONFIG_KEYBOARD_HIDDEN |
            IActivityInfo::CONFIG_ORIENTATION)) != 0) {
        // The configurations being masked out are ones that commonly
        // change so we don't want flushing the cache... all others
        // will flush the cache.
        HashMap<Int32, HashMap<String, AutoPtr<Package> >* >::Iterator it = mPackages.Begin();
        for (; it != mPackages.End() ; it++)
        {
            HashMap<String, AutoPtr<Package> >* map = it->mSecond;
            if (map != NULL) {
                delete map;
            }
        }
        mPackages.Clear();
    }
}

ECode AttributeCache::RemoveUser(
        /* [in] */ Int32 userId)
{
    Mutex::Autolock lock(mLock);
    HashMap<String, AutoPtr<Package> >* map = mPackages[userId];
    if (map != NULL)
        delete map;
    mPackages.Erase(userId);
    return NOERROR;
}

void AttributeCache::RemovePackage(
    /* [in] */ const String& packageName)
{
    Mutex::Autolock lock(mLock);
    HashMap<Int32, HashMap<String, AutoPtr<Package> >* >::Iterator it = mPackages.Begin();
    for (; it != mPackages.End(); it++) {
        HashMap<String, AutoPtr<Package> >* map = it->mSecond;
        if (map != NULL)
            map->Erase(packageName);
    }
}

AutoPtr<AttributeCache::Entry> AttributeCache::Get(
    /* [in] */ Int32 userId,
    /* [in] */ const String& packageName,
    /* [in] */ Int32 resId,
    /* [in] */ ArrayOf<Int32>* styleable)
{
    Mutex::Autolock lock(mLock);
    HashMap<String, AutoPtr<Package> >* packages = mPackages[userId];
    if (packages == NULL) {
        packages = new HashMap<String, AutoPtr<Package> >();
        mPackages[userId] = packages;
    }
    AutoPtr<Package> pkg = (*packages)[packageName];
    EntryMap* map = NULL;
    AutoPtr<Entry> ent = NULL;
    if (pkg != NULL) {
        map = (pkg->mMap)[resId];
        if (map != NULL) {
            AutoPtr<ArrayOf<Int32> > key = styleable;
            ent = (*map)[key];
            if (ent != NULL) {
                return ent;
            }
        }
    }
    else {
        AutoPtr<IContext> context;
        AutoPtr<IUserHandle> userHandle;
        CUserHandle::New(userId, (IUserHandle**)&userHandle);
        ECode ec = mContext->CreatePackageContextAsUser(packageName, 0, userHandle, (IContext**)&context);
        if (FAILED(ec) || context == NULL) {
            return NULL;
        }
        pkg = new Package(context);
        (*packages)[packageName] = pkg;
    }

    if (map == NULL) {
        map = new HashMap<AutoPtr<ArrayOf<Int32> >, AutoPtr<Entry> >();
        (pkg->mMap)[resId] = map;
    }

    AutoPtr<ITypedArray> typeArray;
    ECode ec = pkg->mContext->ObtainStyledAttributesEx(resId, styleable, (ITypedArray**)&typeArray);
    if (FAILED(ec)) {
        return NULL;
    }
    ent = new Entry(pkg->mContext, typeArray);
    (*map)[styleable] = ent;
    return ent;
}

}// Server
}// Droid
}// Elastos
