#include "ext/frameworkext.h"
#include "app/CActivityManagerHelper.h"
#include "app/CActivityManager.h"

namespace Elastos {
namespace Droid {
namespace App {

/** @hide */
ECode CActivityManagerHelper::GetMemoryClass(
    /* [out] */ Int32* cls)
{
    VALIDATE_NOT_NULL(cls);
    *cls = CActivityManager::GetMemoryClass();
    return NOERROR;
}

/** @hide */
ECode CActivityManagerHelper::GetLargeMemoryClass(
    /* [out] */ Int32* cls)
{
    VALIDATE_NOT_NULL(cls);
    *cls = CActivityManager::GetLargeMemoryClass();
    return NOERROR;
}

ECode CActivityManagerHelper::IsHighEndGfx(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = CActivityManager::IsHighEndGfx();
    return NOERROR;
}

ECode CActivityManagerHelper::IsLargeRAM(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = CActivityManager::IsLargeRAM();
    return NOERROR;
}

ECode CActivityManagerHelper::GetMyMemoryState(
    /* [out] */ IActivityManagerRunningAppProcessInfo** outState)
{
    VALIDATE_NOT_NULL(outState);
    AutoPtr<IActivityManagerRunningAppProcessInfo> info = CActivityManager::GetMyMemoryState();
    *outState = info.Get();
    INTERFACE_ADDREF(*outState)
    return NOERROR;
}

ECode CActivityManagerHelper::IsUserAMonkey(
        /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = CActivityManager::IsUserAMonkey();
    return NOERROR;
}

ECode CActivityManagerHelper::IsRunningInTestHarness(
        /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = CActivityManager::IsRunningInTestHarness();
    return NOERROR;
}

ECode CActivityManagerHelper::CheckComponentPermission(
    /* [in] */ const String& permission,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 owningUid,
    /* [in] */ Boolean exported,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = CActivityManager::CheckComponentPermission(
            permission, uid, owningUid, exported);
    return NOERROR;
}

ECode CActivityManagerHelper::CheckUidPermission(
    /* [in] */ const String& permission,
    /* [in] */ Int32 uid,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = CActivityManager::CheckUidPermission(permission, uid);
    return NOERROR;
}

ECode CActivityManagerHelper::HandleIncomingUser(
    /* [in] */ Int32 callingPid,
    /* [in] */ Int32 callingUid,
    /* [in] */ Int32 userId,
    /* [in] */ Boolean allowAll,
    /* [in] */ Boolean requireFull,
    /* [in] */ const String& name,
    /* [in] */ const String& callerPackage,
    /* [out] */ Int32* value)
{
    return CActivityManager::HandleIncomingUser(
            callingPid, callingUid, userId, allowAll, requireFull,
            name , callerPackage, value);
}

ECode CActivityManagerHelper::GetCurrentUser(
    /* [out] */ Int32* currentUser)
{
    VALIDATE_NOT_NULL(currentUser);
    *currentUser = CActivityManager::GetCurrentUser();
    return NOERROR;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
