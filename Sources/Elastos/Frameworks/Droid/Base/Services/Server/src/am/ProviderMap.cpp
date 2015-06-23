
#include "am/ProviderMap.h"
#include "am/ContentProviderRecord.h"
#include "am/ProcessRecord.h"
#include <elastos/utility/logging/Slogger.h>
#include "os/Binder.h"
#include "os/UserHandle.h"

using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Os::Binder;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::Os::IUserHandle;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

const String ProviderMap::TAG("ProviderMap");
const Boolean ProviderMap::DBG = FALSE;

ProviderMap::ProviderMap(
    /* [in] */ CActivityManagerService* am)
{
    mAm = am;
}

ProviderMap::~ProviderMap()
{
    mSingletonByName.Clear();
    mSingletonByClass.Clear();
    mProvidersByNamePerUser.Clear();
    mProvidersByClassPerUser.Clear();
}

AutoPtr<ContentProviderRecord> ProviderMap::GetProviderByName(
    /* [in] */ const String& name)
{
    return GetProviderByName(name, -1);
}

AutoPtr<ContentProviderRecord> ProviderMap::GetProviderByName(
    /* [in] */ const String& name,
    /* [in] */ Int32 userId)
{
    if (DBG) {
        Slogger::I(TAG, "getProviderByName: %s , callingUid = %d",
                name.string(), Binder::GetCallingUid());
    }

    AutoPtr<ContentProviderRecord> result;

    // Try to find it in the global list
    NameRecordIterator it = mSingletonByName.Find(name);
    if (it != mSingletonByName.End()) {
        result = it->mSecond;
    }
    else {
        // Check the current user's list
        AutoPtr<NameRecordHashMap> map = GetProvidersByName(userId);
        if (map != NULL) {
            it = map->Find(name);
            if (it != map->End()) {
                result = it->mSecond;
            }
        }
    }

    return result;
}

AutoPtr<ContentProviderRecord> ProviderMap::GetProviderByClass(
    /* [in] */ IComponentName* name)
{
    return GetProviderByClass(name, -1);
}

AutoPtr<ContentProviderRecord> ProviderMap::GetProviderByClass(
    /* [in] */ IComponentName* name,
    /* [in] */ Int32 userId)
{
    assert(name != NULL && "Error: name cannot be null in ProviderMap::GetProviderByClass(...)");

    if (DBG) {
        String str;
        name->ToString(&str);
        Slogger::I(TAG, "getProviderByClass: %s , callingUid = %d",
                str.string(), Binder::GetCallingUid());
    }

    AutoPtr<ContentProviderRecord> result;

     // Try to find it in the global list
    ClassRecordIterator it = mSingletonByClass.Find(name);
    if (it != mSingletonByClass.End()) {
        result = it->mSecond;
    }
    else {
        // Check the current user's list
        AutoPtr<ClassRecordHashMap> map = GetProvidersByClass(userId);
        if (map != NULL) {
            it = map->Find(name);
            if (it != map->End()) {
                result = it->mSecond;
            }
        }
    }

    return result;
}

ECode ProviderMap::PutProviderByName(
    /* [in] */ const String& name,
    /* [in] */ ContentProviderRecord* aRecord)
{
    VALIDATE_NOT_NULL(aRecord);

    AutoPtr<ContentProviderRecord> record = aRecord;
    Int32 uid;
    record->mAppInfo->GetUid(&uid);

    if (DBG) {
        Slogger::I(TAG, "putProviderByName: %s , callingUid = %d, record uid = %d",
                name.string(), Binder::GetCallingUid(), uid);
    }

    if (record->mSingleton) {
        mSingletonByName.Insert(NameRecordValueType(name, record));
    }
    else {
        Int32 userId = UserHandle::GetUserId(uid);
        AutoPtr<NameRecordHashMap> map = GetProvidersByName(userId);
        if (map != NULL) {
            map->Insert(NameRecordValueType(name, record));
        }
        else {
            map = new NameRecordHashMap();
            map->Insert(NameRecordValueType(name, record));
            mProvidersByNamePerUser.Insert(ProviderByNameValueType(userId, map));
        }
    }

    return NOERROR;
}

ECode ProviderMap::PutProviderByClass(
    /* [in] */ IComponentName* aName,
    /* [in] */ ContentProviderRecord* aRecord)
{
    VALIDATE_NOT_NULL(aName);
    VALIDATE_NOT_NULL(aRecord);

    AutoPtr<IComponentName> name = aName;
    AutoPtr<ContentProviderRecord> record = aRecord;

    Int32 uid;
    record->mAppInfo->GetUid(&uid);

    if (DBG) {
        String str;
        name->ToString(&str);
        Slogger::I(TAG, "putProviderByClass: %s , callingUid = %d, record uid = %d",
                str.string(), Binder::GetCallingUid(), uid);
    }

    if (record->mSingleton) {
        mSingletonByClass.Insert(ClassRecordValueType(name, record));
    }
    else {
        Int32 userId = UserHandle::GetUserId(uid);
        AutoPtr<ClassRecordHashMap> map = GetProvidersByClass(userId);
        if (map != NULL) {
            map->Insert(ClassRecordValueType(name, record));
        }
        else {
            map = new ClassRecordHashMap();
            map->Insert(ClassRecordValueType(name, record));
            mProvidersByClassPerUser.Insert(ProviderByClassValueType(userId, map));
        }
    }

    return NOERROR;
}

ECode ProviderMap::RemoveProviderByName(
    /* [in] */ const String& name,
    /* [in] */ Int32 userId)
{
    NameRecordIterator it = mSingletonByName.Find(name);
    if (it != mSingletonByName.End()) {
        if (DBG)
            Slogger::I(TAG, "Removing from globalByName name=%s", name.string());

        mSingletonByName.Erase(it);
    }
    else {
        if (userId < 0) {
            //throw new IllegalArgumentException("Bad user " + userId);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }

        if (DBG) {
            Slogger::I(TAG, "Removing from providersByName name=%s user=%d",
                    name.string(), userId);
        }

        AutoPtr<NameRecordHashMap> map = GetProvidersByName(userId);
        if (map != NULL) {
            map->Erase(name);
            if (map->IsEmpty()) {
                mProvidersByNamePerUser.Erase(userId);
            }
        }
    }

    return NOERROR;
}

ECode ProviderMap::RemoveProviderByClass(
    /* [in] */ IComponentName* aName,
    /* [in] */ Int32 userId)
{
    VALIDATE_NOT_NULL(aName);
    AutoPtr<IComponentName> name = aName;

    ClassRecordIterator it = mSingletonByClass.Find(name);
    if (it != mSingletonByClass.End()) {
        if (DBG) {
            String str;
            name->ToString(&str);
            Slogger::I(TAG, "Removing from globalByClass name=%s", str.string());
        }

        mSingletonByClass.Erase(it);
    }
    else {
        if (userId < 0) {
            //throw new IllegalArgumentException("Bad user " + userId);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }

        if (DBG) {
            String str;
            name->ToString(&str);
            Slogger::I(TAG, "Removing from providersByClass name=%s user=%d",
                    str.string(), userId);
        }

        AutoPtr<ClassRecordHashMap> map = GetProvidersByClass(userId);
        if (map != NULL) {
            map->Erase(name);
            if (map->IsEmpty()) {
                mProvidersByClassPerUser.Erase(userId);
            }
        }
    }

    return NOERROR;
}

AutoPtr<HashMap<String, AutoPtr<ContentProviderRecord> > > ProviderMap::GetProvidersByName(
    /* [in] */ Int32 userId)
{
    if (userId < 0) {
        // throw new IllegalArgumentException("Bad user " + userId);
        return NULL;
    }

    HashMap<Int32, AutoPtr<NameRecordHashMap> >::Iterator it = mProvidersByNamePerUser.Find(userId);
    if (it != mProvidersByNamePerUser.End()) {
        return it->mSecond;
    }
    else {
        AutoPtr<NameRecordHashMap> map = new NameRecordHashMap();
        mProvidersByNamePerUser.Insert(ProviderByNameValueType(userId, map));
        return map;
    }
}

AutoPtr<HashMap<AutoPtr<IComponentName>, AutoPtr<ContentProviderRecord> > > ProviderMap::GetProvidersByClass(
    /* [in] */ Int32 userId)
{
    if (userId < 0) {
        // throw new IllegalArgumentException("Bad user " + userId);
        return NULL;
    }

    HashMap<Int32, AutoPtr<ClassRecordHashMap> >::Iterator it = mProvidersByClassPerUser.Find(userId);
    if (it != mProvidersByClassPerUser.End()) {
        return it->mSecond;
    }
    else {
        AutoPtr<ClassRecordHashMap> map = new ClassRecordHashMap();
        mProvidersByClassPerUser.Insert(ProviderByClassValueType(userId, map));
        return map;
    }
}

Boolean ProviderMap::CollectForceStopProvidersLocked(
    /* [in] */ const String& name,
    /* [in] */ Int32 appId,
    /* [in] */ Boolean doit,
    /* [in] */ Boolean evenPersistent,
    /* [in] */ Int32 userId,
    /* [in] */ ProviderMap::ClassRecordHashMap* providers,
    /* [in] */ List<AutoPtr<ContentProviderRecord> >* result)
{
    if (providers == NULL || result == NULL)
        return FALSE;

    Boolean didSomething = FALSE;
    ClassRecordIterator it = providers->Begin();
    for (; it != providers->End(); ++it) {
        AutoPtr<ContentProviderRecord> provider = it->mSecond;
        if (provider != NULL) {
            String pkgName;
            provider->mInfo->GetPackageName(&pkgName);
            if ((name.IsNull() || pkgName.Equals(name))
                    && (provider->mProc == NULL || evenPersistent || !provider->mProc->mPersistent)) {
                if (!doit) {
                    return TRUE;
                }
                didSomething = TRUE;
                result->PushBack(provider);
            }
        }
    }

    return didSomething;
}

Boolean ProviderMap::CollectForceStopProviders(
    /* [in] */ const String& name,
    /* [in] */ Int32 appId,
    /* [in] */ Boolean doit,
    /* [in] */ Boolean evenPersistent,
    /* [in] */ Int32 userId,
    /* [in] */ List<AutoPtr<ContentProviderRecord> >* result)
{
    if (result == NULL)
        return FALSE;

    Boolean didSomething = CollectForceStopProvidersLocked(
            name, appId, doit, evenPersistent, userId, &mSingletonByClass, result);
    if (!doit && didSomething) {
        return TRUE;
    }

    if (userId == IUserHandle::USER_ALL) {
        HashMap<Int32, AutoPtr<ClassRecordHashMap> >::Iterator it = mProvidersByClassPerUser.Begin();
        for (; it != mProvidersByClassPerUser.End(); ++it) {
            AutoPtr<ClassRecordHashMap> map = it->mSecond;
            if (CollectForceStopProvidersLocked(
                    name, appId, doit, evenPersistent, userId, map, result)) {
                if (!doit) {
                    return TRUE;
                }
                didSomething = TRUE;
            }
        }
    }
    else {
        AutoPtr<ClassRecordHashMap> items = GetProvidersByClass(userId);
        if (items != NULL) {
            didSomething |= CollectForceStopProvidersLocked(
                    name, appId, doit, evenPersistent, userId, items, result);
        }
    }
    return didSomething;
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
