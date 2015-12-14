
#include "Elastos.CoreLibrary.IO.h"
#include "Elastos.CoreLibrary.Libcore.h"
#include "Elastos.Droid.Database.h"
#include "Elastos.Droid.Graphics.h"
#include "Elastos.Droid.Net.h"
#include "Elastos.Droid.Provider.h"
#include "elastos/droid/provider/DocumentsProvider.h"
#include "elastos/droid/provider/DocumentsContract.h"
#include "elastos/droid/content/CUriMatcher.h"
#include "elastos/droid/content/res/CAssetFileDescriptor.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/os/CBundle.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Droid::Content::CUriMatcher;
using Elastos::Droid::Content::EIID_IComponentCallbacks;
using Elastos::Droid::Content::EIID_IComponentCallbacks2;
using Elastos::Droid::Content::EIID_IContentProvider;
using Elastos::Droid::Content::IComponentCallbacks;
using Elastos::Droid::Content::IComponentCallbacks2;
using Elastos::Droid::Content::IContentProvider;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IUriMatcher;
using Elastos::Droid::Content::Pm::IComponentInfo;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Res::CAssetFileDescriptor;
using Elastos::Droid::Database::ICursor;
using Elastos::Droid::Graphics::IPoint;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::Provider::IDocumentsContractRoot;
using Elastos::Utility::Logging::Slogger;
using Elastos::IO::ICloseable;
using Libcore::IO::CIoUtils;
using Libcore::IO::IIoUtils;

namespace Elastos {
namespace Droid {
namespace Provider {

const String DocumentsProvider::TAG("DocumentsProvider");
const Int32 DocumentsProvider::MATCH_ROOTS = 1;
const Int32 DocumentsProvider::MATCH_ROOT = 2;
const Int32 DocumentsProvider::MATCH_RECENT = 3;
const Int32 DocumentsProvider::MATCH_SEARCH = 4;
const Int32 DocumentsProvider::MATCH_DOCUMENT = 5;
const Int32 DocumentsProvider::MATCH_CHILDREN = 6;
const Int32 DocumentsProvider::MATCH_DOCUMENT_TREE = 7;
const Int32 DocumentsProvider::MATCH_CHILDREN_TREE = 8;

DocumentsProvider::DocumentsProvider()
{}

DocumentsProvider::~DocumentsProvider()
{}

CAR_INTERFACE_IMPL_3(DocumentsProvider, Object, IContentProvider, IComponentCallbacks2, IComponentCallbacks)

//@Override
ECode DocumentsProvider::AttachInfo(
    /* [in] */ IContext* context,
    /* [in] */ IProviderInfo* info)
{
    String authority;
    info->GetAuthority(&authority);
    mAuthority = authority;

    CUriMatcher::New(IUriMatcher::NO_MATCH, (IUriMatcher**)&mMatcher);
    mMatcher->AddURI(mAuthority, String("root"), MATCH_ROOTS);
    mMatcher->AddURI(mAuthority, String("root/*"), MATCH_ROOT);
    mMatcher->AddURI(mAuthority, String("root/*/recent"), MATCH_RECENT);
    mMatcher->AddURI(mAuthority, String("root/*/search"), MATCH_SEARCH);
    mMatcher->AddURI(mAuthority, String("document/*"), MATCH_DOCUMENT);
    mMatcher->AddURI(mAuthority, String("document/*/children"), MATCH_CHILDREN);
    mMatcher->AddURI(mAuthority, String("tree/*/document/*"), MATCH_DOCUMENT_TREE);
    mMatcher->AddURI(mAuthority, String("tree/*/document/*/children"), MATCH_CHILDREN_TREE);

    // Sanity check our setup
    Boolean flag = FALSE;
    if (!(IComponentInfo::Probe(info)->GetExported(&flag), flag)) {
        // throw new SecurityException("Provider must be exported");
        Slogger::E("DocumentsProvider", "Provider must be exported");
        return E_SECURITY_EXCEPTION;
    }

    if (!(info->GetGrantUriPermissions(&flag), flag)) {
        // throw new SecurityException("Provider must grantUriPermissions");
        Slogger::E("DocumentsProvider", "Provider must grantUriPermissions");
        return E_SECURITY_EXCEPTION;
    }

    String readPermission;
    String writePermission;
    info->GetReadPermission(&readPermission);
    info->GetWritePermission(&writePermission);
    assert(0 && "TODO");
    // if (!Elastos::Droid::Manifest::permission::MANAGE_DOCUMENTS.Equals(readPermission)
    //         || !Elastos::Droid::Manifest::permission::MANAGE_DOCUMENTS.Equals(writePermission)) {
    //     // throw new SecurityException("Provider must be protected by MANAGE_DOCUMENTS");
    //     Slogger::E("DocumentsProvider", "Provider must be protected by MANAGE_DOCUMENTS");
    //     return E_SECURITY_EXCEPTION;
    // }

    return IContentProvider::Probe(this)->AttachInfo(context, info);
}

void DocumentsProvider::EnforceTree(
    /* [in] */ IUri* documentUri)
{
    Boolean flag = FALSE;
    DocumentsContract::IsTreeUri(documentUri, &flag);
    if (flag) {
        String parent;
        DocumentsContract::GetTreeDocumentId(documentUri, &parent);
        String child;
        DocumentsContract::GetDocumentId(documentUri, &child);
        if (parent.Equals(child)) {
            return;
        }
        if (!(IsChildDocument(parent, child, &flag), flag)) {
            // throw new SecurityException(
                    // "Document " + child + " is not a descendant of " + parent);
            Slogger::E("DocumentsProvider", "Document %s is not a descendant of %s", child.string(), parent.string());
            return;
        }
    }
}

ECode DocumentsProvider::IsChildDocument(
    /* [in] */ const String& parentDocumentId,
    /* [in] */ const String& documentId,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
}

//@SuppressWarnings("unused")
ECode DocumentsProvider::CreateDocument(
    /* [in] */ const String& parentDocumentId,
    /* [in] */ const String& mimeType,
    /* [in] */ const String& displayName,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    Slogger::E("DocumentsProvider", "Create not supported");
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

//@SuppressWarnings("unused")
ECode DocumentsProvider::RenameDocument(
    /* [in] */ const String& documentId,
    /* [in] */ const String& displayName,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    Slogger::E("DocumentsProvider", "Rename not supported");
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

//@SuppressWarnings("unused")
ECode DocumentsProvider::DeleteDocument(
    /* [in] */ const String& documentId)
{
    Slogger::E("DocumentsProvider", "Delete not supported");
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

//@SuppressWarnings("unused")
ECode DocumentsProvider::QueryRecentDocuments(
    /* [in] */ const String& rootId,
    /* [in] */ ArrayOf<String>* projection,
    /* [out] */ ICursor** cursor)
{
    Slogger::E("DocumentsProvider", "Recent not supported");
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

/** {@hide} */
// @SuppressWarnings("unused")
ECode DocumentsProvider::QueryChildDocumentsForManage(
    /* [in] */ const String& parentDocumentId,
    /* [in] */ ArrayOf<String>* projection,
    /* [in] */ const String& sortOrder,
    /* [out] */ ICursor** cursor)
{
    VALIDATE_NOT_NULL(cursor);
    Slogger::E("DocumentsProvider", "Manage not supported");
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

//@SuppressWarnings("unused")
ECode DocumentsProvider::QuerySearchDocuments(
    /* [in] */ const String& rootId,
    /* [in] */ const String& query,
    /* [in] */ ArrayOf<String>* projection,
    /* [out] */ ICursor** cursor)
{
    VALIDATE_NOT_NULL(cursor);
    Slogger::E("DocumentsProvider", "Search not supported");
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode DocumentsProvider::GetDocumentType(
    /* [in] */ const String& documentId,
    /* [out] */ String* type)
{
    AutoPtr<ICursor> cursor;
    QueryDocument(documentId, NULL, (ICursor**)&cursor);
    // try {
    Boolean bSucceeded = FALSE;
    if ((cursor->MoveToFirst(&bSucceeded), bSucceeded)) {
        Int32 columnIndex;
        cursor->GetColumnIndexOrThrow(IDocumentsContractDocument::COLUMN_MIME_TYPE, &columnIndex);
        ECode ec = cursor->GetString(columnIndex, type);
        goto EXIT;
        return ec;
    } else {
        *type = NULL;
        goto EXIT;
        return NOERROR;
    }
    // } finally {
EXIT:
    AutoPtr<IIoUtils> iou;
    CIoUtils::AcquireSingleton((IIoUtils**)&iou);
    iou->CloseQuietly(ICloseable::Probe(cursor));
    // }
}

//@SuppressWarnings("unused")
ECode DocumentsProvider::OpenDocumentThumbnail(
    /* [in] */ const String& documentId,
    /* [in] */ IPoint* sizeHint,
    /* [in] */ ICancellationSignal* signal,
    /* [out] */ IAssetFileDescriptor** afd)
{
    VALIDATE_NOT_NULL(afd);
    Slogger::E("DocumentsProvider", "Thumbnails not supported");
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode DocumentsProvider::Query(
    /* [in] */ IUri* uri,
    /* [in] */ ArrayOf<String>* projection,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& sortOrder,
    /* [out] */ ICursor** cursor)
{
    VALIDATE_NOT_NULL(cursor);
// try {
    Int32 matchCode;
    mMatcher->Match(uri, &matchCode);
    ECode ec = NOERROR;
    String rootId;
    String docID;
    switch (matchCode) {
        case MATCH_ROOTS:
            {
                ec = QueryRoots(projection, cursor);
                if (!SUCCEEDED(ec)) goto EXIT;
                return ec;
            }
        case MATCH_RECENT:
            {
                DocumentsContract::GetRootId(uri, &rootId);
                ec = QueryRecentDocuments(rootId, projection, cursor);
                if (!SUCCEEDED(ec)) goto EXIT;
                return ec;
            }
        case MATCH_SEARCH:
            {
                DocumentsContract::GetRootId(uri, &rootId);
                String docQuery;
                DocumentsContract::GetSearchDocumentsQuery(uri, &docQuery);
                ec = QuerySearchDocuments(rootId, docQuery, projection, cursor);
                if (!SUCCEEDED(ec)) goto EXIT;
                return ec;
            }
        case MATCH_DOCUMENT:
        case MATCH_DOCUMENT_TREE:
            {
                EnforceTree(uri);
                DocumentsContract::GetDocumentId(uri, &docID);
                ec = QueryDocument(docID, projection, cursor);
                if (!SUCCEEDED(ec)) goto EXIT;
                return ec;
            }

        case MATCH_CHILDREN:
        case MATCH_CHILDREN_TREE:
            {
                EnforceTree(uri);
                DocumentsContract::GetDocumentId(uri, &docID);
                Boolean isMngCode = FALSE;
                DocumentsContract::IsManageMode(uri, &isMngCode);
                if (isMngCode) {
                    ec = QueryChildDocumentsForManage(docID, projection, sortOrder, cursor);
                } else {
                    ec = QueryChildDocuments(docID, projection, sortOrder, cursor);
                }
                if (!SUCCEEDED(ec)) goto EXIT;
                return ec;
            }

        default:
            {
                // throw new UnsupportedOperationException("Unsupported Uri " + uri);
                Slogger::E("DocumentsProvider", "Unsupported Uri %p", uri);
                *cursor = NULL;
                return E_UNSUPPORTED_OPERATION_EXCEPTION;
            }
    }
// } catch (FileNotFoundException e) {
EXIT:
    if(ec == (ECode)E_FILE_NOT_FOUND_EXCEPTION) {
        Slogger::W(TAG, "Failed during query");
        *cursor = NULL;
        return NOERROR;
    }
// }
}

//@Override
ECode DocumentsProvider::GetType(
    /* [in] */ IUri* uri,
    /* [out] */ String* type)
{
    VALIDATE_NOT_NULL(type);
// try {
    Int32 matchCode;
    mMatcher->Match(uri, &matchCode);
    ECode ec = NOERROR;
    switch (matchCode) {
        case MATCH_ROOT:
            {
                *type = IDocumentsContractRoot::MIME_TYPE_ITEM;
                return NOERROR;
            }

        case MATCH_DOCUMENT:
        case MATCH_DOCUMENT_TREE:
            {
                EnforceTree(uri);
                String docID;
                DocumentsContract::GetDocumentId(uri, &docID);
                ec = GetDocumentType(docID, type);
                if (!SUCCEEDED(ec)) goto EXIT;
                return ec;
            }

        default:
            {
                *type = NULL;
                return NOERROR;
            }

    }
    // } catch (FileNotFoundException e) {
EXIT:
    if(ec == (ECode)E_FILE_NOT_FOUND_EXCEPTION) {
        Slogger::W(TAG, "Failed during getType");
        *type = NULL;
        return NOERROR;;
    }
    // }
}

//@Override
ECode DocumentsProvider::Canonicalize(
    /* [in] */ IUri* uri,
    /* [out] */ IUri** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IContext> context;
    IContentProvider::Probe(this)->GetContext((IContext**)&context);
    Int32 matchCode;
    mMatcher->Match(uri, &matchCode);
    switch (matchCode) {
        case MATCH_DOCUMENT_TREE:
            {
                EnforceTree(uri);
                String authority;
                uri->GetAuthority(&authority);
                String docID;
                DocumentsContract::GetDocumentId(uri, &docID);

                AutoPtr<IUri> narrowUri;
                DocumentsContract::BuildDocumentUri(authority, docID, (IUri**)&narrowUri);

                // Caller may only have prefix grant, so extend them a grant to
                // the narrow URI.
                Int32 modeFlags = GetCallingOrSelfUriPermissionModeFlags(context, uri);
                String calPack;
                IContentProvider::Probe(this)->GetCallingPackage(&calPack);
                context->GrantUriPermission(calPack, narrowUri, modeFlags);
                *result = narrowUri;
                REFCOUNT_ADD(*result);
                return NOERROR;
            }
    }

    *result = NULL;
    return NOERROR;
}

Int32 DocumentsProvider::GetCallingOrSelfUriPermissionModeFlags(
    /* [in] */ IContext* context,
    /* [in] */ IUri* uri)
{
    // TODO: move this to a direct AMS call
    Int32 modeFlags = 0;
    Int32 result;
    context->CheckCallingOrSelfUriPermission(uri, IIntent::FLAG_GRANT_READ_URI_PERMISSION, &result);
    if (result == IPackageManager::PERMISSION_GRANTED) {
        modeFlags |= IIntent::FLAG_GRANT_READ_URI_PERMISSION;
    }

    context->CheckCallingOrSelfUriPermission(uri, IIntent::FLAG_GRANT_WRITE_URI_PERMISSION, &result);
    if (result == IPackageManager::PERMISSION_GRANTED) {
        modeFlags |= IIntent::FLAG_GRANT_WRITE_URI_PERMISSION;
    }

    context->CheckCallingOrSelfUriPermission(uri, IIntent::FLAG_GRANT_READ_URI_PERMISSION | IIntent::FLAG_GRANT_PERSISTABLE_URI_PERMISSION, &result);
    if (result == IPackageManager::PERMISSION_GRANTED) {
        modeFlags |= IIntent::FLAG_GRANT_PERSISTABLE_URI_PERMISSION;
    }
    return modeFlags;
}

//@Override
ECode DocumentsProvider::Insert(
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* values,
    /* [out] */ IUri** insertedUri)
{
    VALIDATE_NOT_NULL(insertedUri);
    Slogger::E("DocumentsProvider", "Insert not supported");
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

//@Override
ECode DocumentsProvider::Delete(
    /* [in] */ IUri* uri,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [out] */ Int32* rowsAffected)
{
    VALIDATE_NOT_NULL(rowsAffected);
    Slogger::E("DocumentsProvider", "Delete not supported");
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

//@Override
ECode DocumentsProvider::Update(
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* values,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [out] */ Int32* rowsAffected)
{
    VALIDATE_NOT_NULL(rowsAffected);
    Slogger::E("DocumentsProvider", "Update not supported");
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

//@Override
ECode DocumentsProvider::Call(
    /* [in] */ const String& method,
    /* [in] */ const String& arg,
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** bundle)
{
    VALIDATE_NOT_NULL(bundle);
    if (!method.StartWith("android:")) {
        // Ignore non-platform methods
        return IContentProvider::Probe(this)->Call(method, arg, extras, bundle);
    }

    AutoPtr<IContext> context;
    IContentProvider::Probe(this)->GetContext((IContext**)&context);

    AutoPtr<IParcelable> pcl;
    extras->GetParcelable(IDocumentsContract::EXTRA_URI, (IParcelable**)&pcl);
    AutoPtr<IUri> documentUri = IUri::Probe(pcl);
    String authority;
    documentUri->GetAuthority(&authority);
    String documentId;
    DocumentsContract::GetDocumentId(documentUri, &documentId);

    if (mAuthority.Equals(authority)) {
        Slogger::E("DocumentsProvider", "Requested authority %s doesn't match provider %s ", authority.string(), mAuthority.string());
        return E_SECURITY_EXCEPTION;
    }

    EnforceTree(documentUri);

    AutoPtr<IBundle> out;
    CBundle::New((IBundle**)&out);
    // try {

    if (IDocumentsContract::METHOD_CREATE_DOCUMENT.Equals(method)) {
        IContentProvider::Probe(this)->EnforceWritePermissionInner(documentUri);

        String mimeType;
        String displayName;
        String newDocumentId;
        extras->GetString(IDocumentsContractDocument::COLUMN_MIME_TYPE, &mimeType);
        extras->GetString(IDocumentsContractDocument::COLUMN_DISPLAY_NAME, &displayName);
        CreateDocument(documentId, mimeType, displayName, &newDocumentId);

        // No need to issue new grants here, since caller either has
        // manage permission or a prefix grant. We might generate a
        // tree style URI if that's how they called us.
        AutoPtr<IUri> newDocumentUri;
        DocumentsContract::BuildDocumentUriMaybeUsingTree(documentUri, newDocumentId, (IUri**)&newDocumentUri);
        out->PutParcelable(IDocumentsContract::EXTRA_URI, IParcelable::Probe(newDocumentUri));
    } else if (IDocumentsContract::METHOD_RENAME_DOCUMENT.Equals(method)) {
            IContentProvider::Probe(this)->EnforceWritePermissionInner(documentUri);

            String displayName;
            extras->GetString(IDocumentsContractDocument::COLUMN_DISPLAY_NAME, &displayName);
            String newDocumentId;
            RenameDocument(documentId, displayName, &newDocumentId);

            if (newDocumentId != "") {
                AutoPtr<IUri> newDocumentUri;
                DocumentsContract::BuildDocumentUriMaybeUsingTree(documentUri, newDocumentId, (IUri**)&newDocumentUri);

                // If caller came in with a narrow grant, issue them a
                // narrow grant for the newly renamed document.
                Boolean isUri = FALSE;
                if (!(DocumentsContract::IsTreeUri(newDocumentUri, &isUri), isUri)) {
                    Int32 modeFlags = GetCallingOrSelfUriPermissionModeFlags(context, documentUri);
                    String callPack;
                    IContentProvider::Probe(this)->GetCallingPackage(&callPack);
                    context->GrantUriPermission(callPack, newDocumentUri, modeFlags);
                }

                out->PutParcelable(IDocumentsContract::EXTRA_URI, IParcelable::Probe(newDocumentUri));

                // Original document no longer exists, clean up any grants
                RevokeDocumentPermission(documentId);
            }

        } else if (IDocumentsContract::METHOD_DELETE_DOCUMENT.Equals(method)) {
            IContentProvider::Probe(this)->EnforceWritePermissionInner(documentUri);
            DeleteDocument(documentId);

            // Document no longer exists, clean up any grants
            RevokeDocumentPermission(documentId);

        } else {
            // throw new UnsupportedOperationException("Method not supported " + method);
            Slogger::E("DocumentsProvider", "Method not supported %s", method.string());
            return E_UNSUPPORTED_OPERATION_EXCEPTION;
        }
    // } catch (FileNotFoundException e) {
        // throw new IllegalStateException("Failed call " + method, e);
    // }
    *bundle = out;
    return NOERROR;
}

ECode DocumentsProvider::RevokeDocumentPermission(
    /* [in] */ const String& documentId)
{
    AutoPtr<IContext> context;
    IContentProvider::Probe(this)->GetContext((IContext**)&context);
    AutoPtr<IUri> uri;
    DocumentsContract::BuildDocumentUri(mAuthority, documentId, (IUri**)&uri);
    context->RevokeUriPermission(uri, ~0);
    AutoPtr<IUri> uri_;
    DocumentsContract::BuildTreeDocumentUri(mAuthority, documentId, (IUri**)&uri_);
    return context->RevokeUriPermission(uri_, ~0);
}

//@Override
ECode DocumentsProvider::OpenFile(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IParcelFileDescriptor** fileDescriptor)
{
    VALIDATE_NOT_NULL(fileDescriptor);
    EnforceTree(uri);
    String docID;
    DocumentsContract::GetDocumentId(uri, &docID);
    return OpenDocument(docID, mode, NULL, fileDescriptor);
}

//@Override
ECode DocumentsProvider::OpenFile(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [in] */ ICancellationSignal* signal,
    /* [out] */ IParcelFileDescriptor** fileDescriptor)
{
    VALIDATE_NOT_NULL(fileDescriptor);
    EnforceTree(uri);
    String docID;
    DocumentsContract::GetDocumentId(uri, &docID);
    return OpenDocument(docID, mode, signal, fileDescriptor);
}

//@Override
//@SuppressWarnings("resource")
ECode DocumentsProvider::OpenAssetFile(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IAssetFileDescriptor** fileDescriptor)
{
    VALIDATE_NOT_NULL(fileDescriptor);
    EnforceTree(uri);
    String docID;
    DocumentsContract::GetDocumentId(uri, &docID);
    AutoPtr<IParcelFileDescriptor> fd;
    OpenDocument(docID, mode, NULL, (IParcelFileDescriptor**)&fd);
    AutoPtr<IAssetFileDescriptor> afd;
    if (fd != NULL) {
        CAssetFileDescriptor::New(fd, 0, -1, (IAssetFileDescriptor**)&afd);
        *fileDescriptor = afd.Get();
        REFCOUNT_ADD(*fileDescriptor);
        return NOERROR;
    }
    *fileDescriptor = NULL;
    return NOERROR;
}

//@Override
//@SuppressWarnings("resource")
ECode DocumentsProvider:: OpenAssetFile(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [in] */ ICancellationSignal* signal,
    /* [out] */ IAssetFileDescriptor** fileDescriptor)
{
    VALIDATE_NOT_NULL(fileDescriptor);
    EnforceTree(uri);
    String docID;
    DocumentsContract::GetDocumentId(uri, &docID);
    AutoPtr<IParcelFileDescriptor> fd;
    OpenDocument(docID, mode, signal, (IParcelFileDescriptor**)&fd);
    AutoPtr<IAssetFileDescriptor> afd;
    if (fd != NULL) {
        CAssetFileDescriptor::New(fd, 0, -1, (IAssetFileDescriptor**)&afd);
        *fileDescriptor = afd.Get();
        REFCOUNT_ADD(*fileDescriptor);
        return NOERROR;
    }
    *fileDescriptor = NULL;
    return NOERROR;
}

//@Override
ECode DocumentsProvider::OpenTypedAssetFile(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mimeTypeFilter,
    /* [in] */ IBundle* opts,
    /* [out] */ IAssetFileDescriptor** fileDescriptor)
{
    VALIDATE_NOT_NULL(fileDescriptor);
    EnforceTree(uri);
    Boolean flag = FALSE;
    opts->ContainsKey(IContentResolver::EXTRA_SIZE, &flag);
    if (opts != NULL && flag) {
        String docID;
        DocumentsContract::GetDocumentId(uri, &docID);
        AutoPtr<IParcelable> pla;
        opts->GetParcelable(IContentResolver::EXTRA_SIZE, (IParcelable**)&pla);
        AutoPtr<IPoint> sizeHint = IPoint::Probe(pla);
        return OpenDocumentThumbnail(docID, sizeHint, NULL, fileDescriptor);
    } else {
        return IContentProvider::Probe(this)->OpenTypedAssetFile(uri, mimeTypeFilter, opts, fileDescriptor);
    }
}

//@Override
ECode DocumentsProvider:: OpenTypedAssetFile(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mimeTypeFilter,
    /* [in] */ IBundle* opts,
    /* [in] */ ICancellationSignal* signal,
    /* [out] */ IAssetFileDescriptor** fileDescriptor)
{
    VALIDATE_NOT_NULL(fileDescriptor);
    EnforceTree(uri);
    Boolean flag = FALSE;
    opts->ContainsKey(IContentResolver::EXTRA_SIZE, &flag);
    if (opts != NULL && flag) {
        String docID;
        DocumentsContract::GetDocumentId(uri, &docID);
        AutoPtr<IParcelable> pla;
        opts->GetParcelable(IContentResolver::EXTRA_SIZE, (IParcelable**)&pla);
        AutoPtr<IPoint> sizeHint = IPoint::Probe(pla);
        return OpenDocumentThumbnail(docID, sizeHint, signal, (IAssetFileDescriptor**)&fileDescriptor);
    } else {
        return IContentProvider::Probe(this)->OpenTypedAssetFile(uri, mimeTypeFilter, opts, signal, fileDescriptor);
    }
}

} // namespace Provider
} // namespace Droid
} // namespace Elastos
