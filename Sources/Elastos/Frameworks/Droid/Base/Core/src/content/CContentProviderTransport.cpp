
#include "content/CContentProviderTransport.h"
#include "ext/frameworkext.h"
#include "os/Binder.h"
#include "os/UserHandle.h"
#include "os/CCancellationSignalHelper.h"

using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Os::Binder;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::Os::IICancellationSignal;
using Elastos::Droid::Os::ICancellationSignalHelper;
using Elastos::Droid::Os::CCancellationSignalHelper;

namespace Elastos {
namespace Droid {
namespace Content {

ECode CContentProviderTransport::constructor(
    /* [in] */ Handle32 owner)
{
    mContentProvider = reinterpret_cast<ContentProvider*>(owner);
    return NOERROR;
}

ECode CContentProviderTransport::GetContentProvider(
    /* [out] */ IContentProvider** provider)
{
    VALIDATE_NOT_NULL(provider)
    *provider = (IContentProvider*)mContentProvider->Probe(EIID_IContentProvider);
    REFCOUNT_ADD(*provider)
    return NOERROR;
}

ECode CContentProviderTransport::GetProviderName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContentProviderTransport::Query(
    /* [in] */ IUri* uri,
    /* [in] */ ArrayOf<String>* projection,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& sortOrder,
    /* [in] */ IICancellationSignal* cancellationSignal,
    /* [out] */ ICursor** cursor)
{
    VALIDATE_NOT_NULL(cursor);
    FAIL_RETURN(EnforceReadPermission(uri))
    AutoPtr<ICancellationSignalHelper> helper;
    CCancellationSignalHelper::AcquireSingleton((ICancellationSignalHelper**)&helper);
    AutoPtr<ICancellationSignal> sig;
    helper->FromTransport(cancellationSignal, (ICancellationSignal**)&sig);
    return mContentProvider->Query(uri, projection, selection, selectionArgs, sortOrder, sig, cursor);
}

ECode CContentProviderTransport::GetType(
    /* [in] */ IUri* uri,
    /* [out] */ String* type)
{
    VALIDATE_NOT_NULL(type);
    return mContentProvider->GetType(uri, type);
}

ECode CContentProviderTransport::Insert(
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* initialValues,
    /* [out] */ IUri** insertedUri)
{
    VALIDATE_NOT_NULL(insertedUri);
    FAIL_RETURN(EnforceWritePermission(uri))
    return mContentProvider->Insert(uri, initialValues, insertedUri);
}

ECode CContentProviderTransport::BulkInsert(
    /* [in] */ IUri* uri,
    /* [in] */ ArrayOf<IContentValues*>* initialValues,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    FAIL_RETURN(EnforceWritePermission(uri))
    return mContentProvider->BulkInsert(uri, initialValues, number);
}

ECode CContentProviderTransport::ApplyBatch(
    /* [in] */ IObjectContainer* operations,
    /* [out, callee] */ ArrayOf<IContentProviderResult*>** providerResults)
{
    VALIDATE_NOT_NULL(providerResults)
    assert(operations != NULL);
    AutoPtr<IObjectEnumerator> ObjEnumerator;
    FAIL_RETURN(operations->GetObjectEnumerator((IObjectEnumerator**)&ObjEnumerator))
    Boolean hasNext = FALSE;
    while (ObjEnumerator->MoveNext(&hasNext), hasNext) {
        AutoPtr<IContentProviderOperation> operation;
        ObjEnumerator->Current((IInterface**)&operation);
        Boolean isRead = FALSE;
        Boolean isWrite = FALSE;
        if (operation->IsReadOperation(&isRead), isRead) {
            AutoPtr<IUri> uri;
            operation->GetUri((IUri**)&uri);
            FAIL_RETURN(EnforceReadPermission(uri))
        }

        if (operation->IsWriteOperation(&isWrite), isWrite) {
            AutoPtr<IUri> uri;
            operation->GetUri((IUri**)&uri);
            FAIL_RETURN(EnforceWritePermission(uri))
        }
    }
    return mContentProvider->ApplyBatch(operations, providerResults);
}

ECode CContentProviderTransport::Delete(
    /* [in] */ IUri* uri,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [out] */ Int32* rowsAffected)
{
    VALIDATE_NOT_NULL(rowsAffected);
    FAIL_RETURN(EnforceWritePermission(uri))
    return mContentProvider->Delete(uri, selection, selectionArgs, rowsAffected);
}

ECode CContentProviderTransport::Update(
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* values,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [out] */ Int32* rowsAffected)
{
    VALIDATE_NOT_NULL(rowsAffected);
    FAIL_RETURN(EnforceWritePermission(uri))
    return mContentProvider->Update(uri, values, selection, selectionArgs, rowsAffected);
}

ECode CContentProviderTransport::OpenFile(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IParcelFileDescriptor** fileDescriptor)
{
    VALIDATE_NOT_NULL(fileDescriptor);
    // if (mode != null && mode.startsWith("w")) enforceWritePermission(uri);
    if (!mode.IsNull() && mode.IndexOf('w') != -1) {
        FAIL_RETURN(EnforceWritePermission(uri))
    }
    else {
        FAIL_RETURN(EnforceReadPermission(uri))
    }
    return mContentProvider->OpenFile(uri, mode, fileDescriptor);
}

ECode CContentProviderTransport::OpenAssetFile(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IAssetFileDescriptor** fileDescriptor)
{
    VALIDATE_NOT_NULL(fileDescriptor);
    // if (mode != null && mode.startsWith("w")) enforceWritePermission(uri);
    if (!mode.IsNull() && mode.IndexOf('w') != -1) {
        FAIL_RETURN(EnforceWritePermission(uri))
    }
    else {
        FAIL_RETURN(EnforceReadPermission(uri))
    }
    return mContentProvider->OpenAssetFile(uri, mode, fileDescriptor);
}

ECode CContentProviderTransport::Call(
    /* [in] */ const String& method,
    /* [in] */ const String& arg,
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** bundle)
{
    VALIDATE_NOT_NULL(bundle);
    return mContentProvider->Call(method, arg, extras, bundle);
}

ECode CContentProviderTransport::GetStreamTypes(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mimeTypeFilter,
    /* [out, callee] */ ArrayOf<String>** streamTypes)
{
    VALIDATE_NOT_NULL(streamTypes);
    return mContentProvider->GetStreamTypes(uri, mimeTypeFilter, streamTypes);
}

ECode CContentProviderTransport::OpenTypedAssetFile(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mimeType,
    /* [in] */ IBundle* opts,
    /* [out] */ IAssetFileDescriptor** fileDescriptor)
{
    VALIDATE_NOT_NULL(fileDescriptor);
    FAIL_RETURN(EnforceReadPermission(uri));
    return mContentProvider->OpenTypedAssetFile(uri, mimeType, opts, fileDescriptor);
}

ECode CContentProviderTransport::CreateCancellationSignal(
    /* [out] */ IICancellationSignal** cancellationSignal)
{
    VALIDATE_NOT_NULL(cancellationSignal)
    AutoPtr<ICancellationSignalHelper> signalHelper;
    CCancellationSignalHelper::AcquireSingleton((ICancellationSignalHelper**)&signalHelper);
    return signalHelper->CreateTransport(cancellationSignal);
}

ECode CContentProviderTransport::ToString(
    /* [out] */ String* str)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CContentProviderTransport::EnforceReadPermission(
    /* [in] */ IUri* uri)
{
    AutoPtr<IContext> context;
    FAIL_RETURN(mContentProvider->GetContext((IContext**)&context))
    Int32 pid = Binder::GetCallingPid();
    Int32 uid = Binder::GetCallingUid();
    String missingPerm;

    if (UserHandle::IsSameApp(uid, mContentProvider->mMyUid)) {
        return NOERROR;
    }

    if (mContentProvider->mExported) {
        String componentPerm;
        mContentProvider->GetReadPermission(&componentPerm);
        if (!componentPerm.IsNull()) {
            Int32 permissionId = 0;
            FAIL_RETURN(context->CheckPermission(componentPerm, pid, uid, &permissionId))
            if (permissionId == IPackageManager::PERMISSION_GRANTED) {
                return NOERROR;
            }
            else {
                missingPerm = componentPerm;
            }
        }

        // track if unprotected read is allowed; any denied
        // <path-permission> below removes this ability
        Boolean allowDefaultRead = componentPerm.IsNull();

        AutoPtr<ArrayOf<IPathPermission*> > pps;
        mContentProvider->GetPathPermissions((ArrayOf<IPathPermission*>**)&pps);
        if (NULL != pps) {
            String path;
            uri->GetPath(&path);
            for (Int32 i = 0; i < pps->GetLength(); i++) {
                AutoPtr<IPathPermission> pp = (*pps)[i];
                String pathPerm;
                pp->GetReadPermission(&pathPerm);
                Boolean isMatch = FALSE;
                if (!pathPerm.IsNull() && (pp->Match(path, &isMatch), isMatch)) {
                    Int32 checkPermission = 0;
                    FAIL_RETURN(context->CheckPermission(pathPerm, pid, uid, &checkPermission))
                    if (checkPermission == IPackageManager::PERMISSION_GRANTED){
                        return NOERROR;
                    }
                    else {
                        // any denied <path-permission> means we lose
                        // default <provider> access.
                        allowDefaultRead = FALSE;
                        missingPerm = pathPerm;
                    }
                }
            }
        }

        // if we passed <path-permission> checks above, and no default
        // <provider> permission, then allow access.
        if (allowDefaultRead) return NOERROR;
    }

    // last chance, check against any uri grants
    Int32 uriPermission = 0;
    FAIL_RETURN(context->CheckUriPermission(uri, pid, uid, IIntent::FLAG_GRANT_READ_URI_PERMISSION, &uriPermission))
    if (uriPermission == IPackageManager::PERMISSION_GRANTED) return NOERROR;

//    final String failReason = mExported
//            ? " requires " + missingPerm + ", or grantUriPermission()"
//            : " requires the provider be exported, or grantUriPermission()";
//    throw new SecurityException("Permission Denial: reading "
//            + ContentProvider.this.getClass().getName() + " uri " + uri + " from pid=" + pid
//            + ", uid=" + uid + failReason);
    return E_SECURITY_EXCEPTION;
}

ECode CContentProviderTransport::EnforceWritePermission(
    /* [in] */ IUri* uri)
{
    AutoPtr<IContext> context;
    FAIL_RETURN(mContentProvider->GetContext((IContext**)&context))
    const Int32 pid = Binder::GetCallingPid();
    const Int32 uid = Binder::GetCallingUid();
    String missingPerm;

    if (UserHandle::IsSameApp(uid, mContentProvider->mMyUid)) {
        return NOERROR;
    }

    if (mContentProvider->mExported) {
        String componentPerm;
        mContentProvider->GetWritePermission(&componentPerm);
        if (!componentPerm.IsNull()) {
            Int32 permissionId = 0;
            FAIL_RETURN(context->CheckPermission(componentPerm, pid, uid, &permissionId))
            if (permissionId == IPackageManager::PERMISSION_GRANTED) {
                return NOERROR;
            }
            else {
                missingPerm = componentPerm;
            }
        }

        // track if unprotected write is allowed; any denied
        // <path-permission> below removes this ability
        Boolean allowDefaultWrite = componentPerm.IsNull();

        AutoPtr<ArrayOf<IPathPermission*> > pps;
        mContentProvider->GetPathPermissions((ArrayOf<IPathPermission*>**)&pps);
        if (NULL != pps) {
            String path;
            FAIL_RETURN(uri->GetPath(&path))
            for(Int32 i = 0; i < pps->GetLength(); i++) {
                AutoPtr<IPathPermission> pp = (*pps)[i];
                String pathPerm;
                pp->GetWritePermission(&pathPerm);
                Boolean isMatch = FALSE;
                if (!pathPerm.IsNull() && (pp->Match(path, &isMatch), isMatch)) {
                    Int32 checkPermission = 0;
                    FAIL_RETURN(context->CheckPermission(pathPerm, pid, uid, &checkPermission))
                    if (checkPermission == IPackageManager::PERMISSION_GRANTED){
                        return NOERROR;
                    }
                    else {
                        // any denied <path-permission> means we lose
                        // default <provider> access.
                        allowDefaultWrite = FALSE;
                        missingPerm = pathPerm;
                    }
                }
            }
        }

        // if we passed <path-permission> checks above, and no default
        // <provider> permission, then allow access.
        if (allowDefaultWrite) return NOERROR;
    }

    // last chance, check against any uri grants
    Int32 uriPermission = 0;
    FAIL_RETURN(context->CheckUriPermission(uri, pid, uid, IIntent::FLAG_GRANT_WRITE_URI_PERMISSION, &uriPermission))
    if (uriPermission == IPackageManager::PERMISSION_GRANTED) return NOERROR;

//    final String failReason = mExported
//            ? " requires " + missingPerm + ", or grantUriPermission()"
//            : " requires the provider be exported, or grantUriPermission()";
//    throw new SecurityException("Permission Denial: writing "
//            + ContentProvider.this.getClass().getName() + " uri " + uri + " from pid=" + pid
//           + ", uid=" + uid + failReason);
    return E_SECURITY_EXCEPTION;
}

} // namespace Content
} // namespace Droid
} // namespace Elastos
