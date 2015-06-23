
#include <elastos/core/StringBuilder.h>
#include "am/UriPermissionOwner.h"
#include "am/CActivityManagerService.h"
#include "am/CUriPermissionOwnerExternalToken.h"

using Elastos::Core::StringBuilder;
using Elastos::Droid::Os::EIID_IBinder;
using Elastos::Droid::Content::IIntent;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

//==============================================================================
// UriPermissionOwner
//==============================================================================

UriPermissionOwner::UriPermissionOwner(
    /* [in] */ CActivityManagerService* service,
    /* [in] */ Handle32 owner)
    : mService(service)
    , mOwner(owner)
{
}

UriPermissionOwner::~UriPermissionOwner()
{
}

AutoPtr<IBinder> UriPermissionOwner::GetExternalTokenLocked()
{
    if (mExternalToken == NULL) {
        AutoPtr<IUriPermissionOwnerExternalToken> tmp;
        CUriPermissionOwnerExternalToken::New((Handle32)this, (IUriPermissionOwnerExternalToken**)&tmp);
        IWeakReferenceSource::Probe(tmp.Get())->GetWeakReference((IWeakReference**)&mExternalToken);
    }
    AutoPtr<IInterface> tmp;
    mExternalToken->Resolve(EIID_IInterface, (IInterface**)&tmp);

    return IBinder::Probe(tmp);
}

AutoPtr<UriPermissionOwner> UriPermissionOwner::FromExternalToken(
    /* [in] */ IBinder* token)
{
    Handle32 owner = NULL;
    if (IUriPermissionOwnerExternalToken::Probe(token) != NULL) {
        IUriPermissionOwnerExternalToken::Probe(token)->GetOwner(&owner);
    }

    return (UriPermissionOwner*)owner;
}

ECode UriPermissionOwner::RemoveUriPermissionsLocked()
{
    return RemoveUriPermissionsLocked(IIntent::FLAG_GRANT_READ_URI_PERMISSION
        | IIntent::FLAG_GRANT_WRITE_URI_PERMISSION);
}

ECode UriPermissionOwner::RemoveUriPermissionsLocked(
    /* [in] */ Int32 mode)
{
    if ((mode & IIntent::FLAG_GRANT_READ_URI_PERMISSION) != 0
            && mReadUriPermissions != NULL) {
        HashSet< AutoPtr<UriPermission> >::Iterator it = mReadUriPermissions->Begin();
        for (; it != mReadUriPermissions->End(); it++) {
            (*it)->mReadOwners.Erase(this);
            if ((*it)->mReadOwners.IsEmpty() && ((*it)->mGlobalModeFlags
                    & IIntent::FLAG_GRANT_READ_URI_PERMISSION) == 0) {
                (*it)->mModeFlags &= ~IIntent::FLAG_GRANT_READ_URI_PERMISSION;
                mService->RemoveUriPermissionIfNeededLocked(*it);
            }
        }

        mReadUriPermissions = NULL;
    }
    if ((mode & IIntent::FLAG_GRANT_WRITE_URI_PERMISSION) != 0
            && mWriteUriPermissions != NULL) {
        HashSet< AutoPtr<UriPermission> >::Iterator it = mWriteUriPermissions->Begin();
        for (; it != mWriteUriPermissions->End(); it++) {
            (*it)->mWriteOwners.Erase(this);
            if ((*it)->mWriteOwners.IsEmpty() && ((*it)->mGlobalModeFlags
                    & IIntent::FLAG_GRANT_WRITE_URI_PERMISSION) == 0) {
                (*it)->mModeFlags &= ~IIntent::FLAG_GRANT_WRITE_URI_PERMISSION;
                mService->RemoveUriPermissionIfNeededLocked(*it);
            }
        }

        mWriteUriPermissions = NULL;
    }
    return NOERROR;
}

ECode UriPermissionOwner::RemoveUriPermissionLocked(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 mode)
{
    if ((mode & IIntent::FLAG_GRANT_READ_URI_PERMISSION) != 0
            && mReadUriPermissions != NULL) {
        HashSet< AutoPtr<UriPermission> >::Iterator it = mReadUriPermissions->Begin();
        while (it != mReadUriPermissions->End()) {
            UriPermission* perm = (*it);
            Boolean result;
            uri->Equals(perm->mUri, &result);
            if (result) {
                perm->mReadOwners.Erase(this);
                if (perm->mReadOwners.IsEmpty() && (perm->mGlobalModeFlags
                        & IIntent::FLAG_GRANT_READ_URI_PERMISSION) == 0) {
                    perm->mModeFlags &= ~IIntent::FLAG_GRANT_READ_URI_PERMISSION;
                    mService->RemoveUriPermissionIfNeededLocked(perm);
                }
                mReadUriPermissions->Erase(it++);
            }
            else {
                it++;
            }
        }
        if (mReadUriPermissions->IsEmpty()) {
            mReadUriPermissions = NULL;
        }
    }
    if ((mode & IIntent::FLAG_GRANT_WRITE_URI_PERMISSION) != 0
            && mWriteUriPermissions != NULL) {
        HashSet< AutoPtr<UriPermission> >::Iterator it = mWriteUriPermissions->Begin();
        while (it != mWriteUriPermissions->End()) {
            UriPermission* perm = (*it);
            Boolean result;
            uri->Equals(perm->mUri, &result);
            if (result) {
                perm->mWriteOwners.Erase(this);
                if (perm->mWriteOwners.IsEmpty() && (perm->mGlobalModeFlags
                        & IIntent::FLAG_GRANT_WRITE_URI_PERMISSION) == 0) {
                    perm->mModeFlags &= ~IIntent::FLAG_GRANT_WRITE_URI_PERMISSION;
                    mService->RemoveUriPermissionIfNeededLocked(perm);
                }
                mWriteUriPermissions->Erase(it++);
            }
            else {
                ++it;
            }
        }
        if (mWriteUriPermissions->IsEmpty()) {
            mWriteUriPermissions = NULL;
        }
    }
    return NOERROR;
}

ECode UriPermissionOwner::AddReadPermission(
    /* [in] */ UriPermission* perm)
{
    if (mReadUriPermissions == NULL) {
        mReadUriPermissions = new HashSet<AutoPtr<UriPermission> >();
    }
    mReadUriPermissions->Insert(perm);
    return NOERROR;
}

ECode UriPermissionOwner::AddWritePermission(
    /* [in] */ UriPermission* perm)
{
    if (mWriteUriPermissions == NULL) {
        mWriteUriPermissions = new HashSet< AutoPtr<UriPermission> >();
    }
    mWriteUriPermissions->Insert(perm);
    return NOERROR;
}

ECode UriPermissionOwner::RemoveReadPermission(
    /* [in] */ UriPermission* perm)
{
    if (perm != NULL) {
        if (mReadUriPermissions != NULL) {
            mReadUriPermissions->Erase(perm);
            if (mReadUriPermissions->IsEmpty()) {
                mReadUriPermissions = NULL;
            }
        }
    }
    return NOERROR;
}

ECode UriPermissionOwner::RemoveWritePermission(
    /* [in] */ UriPermission* perm)
{
    if (perm != NULL) {
        if (mWriteUriPermissions != NULL) {
            mWriteUriPermissions->Erase(perm);
            if (mWriteUriPermissions->IsEmpty()) {
                mWriteUriPermissions = NULL;
            }
        }
    }
    return NOERROR;
}

String UriPermissionOwner::ToString()
{
    StringBuilder sb("UriPermissionOwner:");
    sb += mOwner;
    return sb.ToString();
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
