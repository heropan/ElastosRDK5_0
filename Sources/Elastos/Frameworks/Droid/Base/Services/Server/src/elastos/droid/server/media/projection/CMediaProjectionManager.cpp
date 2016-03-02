
#include "elastos/droid/server/media/projection/CMediaProjectionManager.h"
#include "elastos/droid/server/media/projection/CMediaProjection.h"
#include "elastos/droid/os/Binder.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Media::Projection::EIID_IIMediaProjection;
using Elastos::Droid::Os::Binder;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Media {
namespace Projection {

CAR_INTERFACE_IMPL_2(CMediaProjectionManager, Object, IIMediaProjectionManager, IBinder)

CAR_OBJECT_IMPL(CMediaProjectionManager)

ECode CMediaProjectionManager::constructor(
    /* [in] */ Handle64 host)
{
    mHost = (MediaProjectionManagerService*)host;
    return NOERROR;
}

ECode CMediaProjectionManager::HasProjectionPermission(
    /* [in] */ Int32 uid,
    /* [in] */ const String& packageName,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    Int64 token = Binder::ClearCallingIdentity();
    Boolean hasPermission = FALSE;
    // try {
    Int32 value;
    mHost->mAppOps->NoteOpNoThrow(IAppOpsManager::OP_PROJECT_MEDIA, uid, packageName, &value);
    hasPermission |= CheckPermission(packageName, Elastos::Droid::Manifest::permission::CAPTURE_VIDEO_OUTPUT)
            || value == IAppOpsManager::MODE_ALLOWED;
    // } finally {
    //     Binder.restoreCallingIdentity(token);
    // }
    Binder::RestoreCallingIdentity(token);
    *result = hasPermission;
    return NOERROR;
}

ECode CMediaProjectionManager::CreateProjection(
    /* [in] */ Int32 uid,
    /* [in] */ const String& packageName,
    /* [in] */ Int32 type,
    /* [in] */ Boolean permanentGrant,
    /* [out] */ IIMediaProjection** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;

    Int32 perm;
    mHost->mContext->CheckCallingPermission(
            Elastos::Droid::Manifest::permission::MANAGE_MEDIA_PROJECTION, &prem);
    if (perm != IPackageManager::PERMISSION_GRANTED) {
        Slogger::E("CMediaProjectionManager", "Requires MANAGE_MEDIA_PROJECTION in order to grant projection permission");
        return E_SECURITY_EXCEPTION;
    }
    if (packageName.IsNull() || packageName.IsEmpty()) {
        Slogger::E("CMediaProjectionManager", "package name must not be empty");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int64 callingToken = Binder::ClearCallingIdentity();
    AutoPtr<CMediaProjection> projection;
    // try {
    CMediaProjection::NewByFriend(type, uid, packageName, (CMediaProjection**)&projection);
    if (isPermanentGrant) {
        mHost->mAppOps->SetMode(IAppOpsManager::OP_PROJECT_MEDIA,
                projection->mUid, projection->mPackageName, IAppOpsManager::MODE_ALLOWED);
    }
    // } finally {
    //     Binder.restoreCallingIdentity(callingToken);
    // }
    Binder::RestoreCallingIdentity(token);
    *result = (IIMediaProjection*)projection->Probe(EIID_IIMediaProjection);
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CMediaProjectionManager::IsValidMediaProjection(
    /* [in] */ IIMediaProjection* projection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mHost->IsValidMediaProjection(IBinder::Probe(projection));
    return NOERROR;
}

ECode CMediaProjectionManager::GetActiveProjectionInfo(
    /* [out] */ IMediaProjectionInfo** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;
    Int32 perm;
    mContext->CheckCallingPermission(
            Elastos::Droid::Manifest::permission::MANAGE_MEDIA_PROJECTION, &perm);
    if (perm != IPackageManager::PERMISSION_GRANTED) {
        Slogger::E("CMediaProjectionManager", "Requires MANAGE_MEDIA_PROJECTION in order to add projection callbacks");
        return E_SECURITY_EXCEPTION;
    }
    Int64 token = Binder::ClearCallingIdentity();
    // try {
    *result = mHost->GetActiveProjectionInfo();
    REFCOUNT_ADD(*result)
    Binder::RestoreCallingIdentity(token);
    return NOERROR;
    // } finally {
    //     Binder.restoreCallingIdentity(token);
    // }
}

ECode CMediaProjectionManager::StopActiveProjection()
{
    Int32 perm;
    mContext->CheckCallingPermission(
            Elastos::Droid::Manifest::permission::MANAGE_MEDIA_PROJECTION, &perm);
    if (perm != IPackageManager::PERMISSION_GRANTED) {
        Slogger::E("CMediaProjectionManager", "Requires MANAGE_MEDIA_PROJECTION in order to add projection callbacks");
        return E_SECURITY_EXCEPTION;
    }
    Int64 token = Binder::ClearCallingIdentity();
    // try {
    if (mHost->mProjectionGrant != NULL) {
        mHost->mProjectionGrant->Stop();
    }
    // } finally {
    //     Binder.restoreCallingIdentity(token);
    // }
    Binder::RestoreCallingIdentity(token);
    return NOERROR;
}

ECode CMediaProjectionManager::AddCallback(
    /* [in] */ IIMediaProjectionWatcherCallback* cb)
{
    Int32 perm;
    mContext->CheckCallingPermission(
            Elastos::Droid::Manifest::permission::MANAGE_MEDIA_PROJECTION, &perm);
    if (perm != IPackageManager::PERMISSION_GRANTED) {
        Slogger::E("CMediaProjectionManager", "Requires MANAGE_MEDIA_PROJECTION in order to add projection callbacks");
        return E_SECURITY_EXCEPTION;
    }
    Int64 token = Binder::ClearCallingIdentity();
    // try {
    mHost->AddCallback(callback);
    // } finally {
    //     Binder.restoreCallingIdentity(token);
    // }
    Binder::RestoreCallingIdentity(token);
    return NOERROR;
}

ECode CMediaProjectionManager::RemoveCallback(
    /* [in] */ IIMediaProjectionWatcherCallback* cb)
{
    Int32 perm;
    mContext->CheckCallingPermission(
            Elastos::Droid::Manifest::permission::MANAGE_MEDIA_PROJECTION, &perm);
    if (perm != IPackageManager::PERMISSION_GRANTED) {
        Slogger::E("CMediaProjectionManager", "Requires MANAGE_MEDIA_PROJECTION in order to remove projection callbacks");
        return E_SECURITY_EXCEPTION;
    }
    Int64 token = Binder::ClearCallingIdentity();
    // try {
    mHost->RemoveCallback(callback);
    // } finally {
    //     Binder.restoreCallingIdentity(token);
    // }
    Binder::RestoreCallingIdentity(token);
    return NOERROR;
}

Boolean CMediaProjectionManager::CheckPermission(
    /* [in] */ const String& packageName,
    /* [in] */ const String& permission)
{
    AutoPtr<IPackageManager> pm;
    mContext->GetPackageManager((IPackageManager**)&pm);
    Int32 perm;
    pm->CheckPermission(permission, packageName, &perm);
    return perm == IPackageManager::PERMISSION_GRANTED;
}

} // namespace Projection
} // namespace Media
} // namespace Server
} // namespace Droid
} // namespace Elastos
