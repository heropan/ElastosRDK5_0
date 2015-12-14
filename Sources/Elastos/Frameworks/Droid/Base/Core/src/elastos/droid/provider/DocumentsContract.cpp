#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/provider/DocumentsContract.h"
#include "elastos/droid/content/CContentProviderClient.h"
#include "elastos/droid/content/CIntent.h"
#include "elastos/droid/content/res/CAssetFileDescriptor.h"
#include "elastos/droid/graphics/BitmapFactory.h"
#include "elastos/droid/graphics/CBitmap.h"
#include "elastos/droid/graphics/CBitmapFactoryOptions.h"
#include "elastos/droid/graphics/CMatrix.h"
// #include "elastos/droid/media/CExifInterface.h"
// #include "elastos/droid/net/CUriBuilder.h"
#include "elastos/droid/os/CBundle.h"
#include "elastos/droid/os/ParcelFileDescriptor.h"
#include "elastos/droid/system/Os.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/Math.h>

using Elastos::Droid::Content::CContentProviderClient;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IContentProviderClient;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Pm::IIPackageManager;
using Elastos::Droid::Content::Pm::IProviderInfo;
using Elastos::Droid::Content::Pm::IResolveInfo;
using Elastos::Droid::Content::Res::CAssetFileDescriptor;
using Elastos::Droid::Content::Res::IAssetFileDescriptor;
using Elastos::Droid::Graphics::BitmapFactory;
using Elastos::Droid::Graphics::CBitmap;
using Elastos::Droid::Graphics::CBitmapFactoryOptions;
using Elastos::Droid::Graphics::CMatrix;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::IBitmapFactory;
using Elastos::Droid::Graphics::IBitmapFactoryOptions;
using Elastos::Droid::Graphics::IMatrix;
using Elastos::Droid::Media::IExifInterface;
using Elastos::Droid::Net::ITrafficStats;
using Elastos::Droid::Net::IUriBuilder;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::ICancellationSignal;
using Elastos::Droid::Os::IParcelFileDescriptor;
using Elastos::Droid::Os::IParcelFileDescriptorOnCloseListener;
using Elastos::Droid::Os::ParcelFileDescriptor;
using Elastos::Droid::Provider::IDocumentsContract;
using Elastos::IO::CBufferedInputStream;
using Elastos::IO::CFileInputStream;
using Libcore::IO::CIoUtils;
using Elastos::IO::IBufferedInputStream;
using Elastos::IO::IFile;
using Elastos::IO::IFileDescriptor;
using Elastos::IO::IFileInputStream;
using Elastos::IO::IInputStream;
using Libcore::IO::IIoUtils;

using Elastos::Utility::IList;
using Elastos::Utility::Logging::Slogger;



namespace Elastos {
namespace Droid {
namespace Provider {

const String DocumentsContract::TAG("Documents");
const Int32 DocumentsContract::THUMBNAIL_BUFFER_SIZE = (Int32) (128 * ITrafficStats::KB_IN_BYTES);
const String DocumentsContract::PATH_ROOT("root");
const String DocumentsContract::PATH_RECENT("recent");
const String DocumentsContract::PATH_DOCUMENT("document");
const String DocumentsContract::PATH_CHILDREN("children");
const String DocumentsContract::PATH_SEARCH("search");
const String DocumentsContract::PATH_TREE("tree");

const String DocumentsContract::PARAM_QUERY("query");
const String DocumentsContract::PARAM_MANAGE("manage");

ECode DocumentsContract::BuildRootsUri(
    /* [in] */ const String& authority,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    AutoPtr<IUriBuilder> iub;
    assert(0 && "TODO"); // CUriBuilder
    // CUriBuilder::New((IUriBuilder**)&iub);

    // AutoPtr<IUriBuilder> ub;
    // iub->Scheme(IContentResolver::SCHEME_CONTENT, (IUriBuilder**)&ub);

    // AutoPtr<IUriBuilder> ubi;
    // ub->Authority(authority, (IUriBuilder**)&ubi);

    // AutoPtr<IUriBuilder> ubr;
    // ubi->AppendPath(PATH_ROOT, (IUriBuilder**)&ubr);

    // return ubr->Build(uri);
}

ECode DocumentsContract::BuildRootUri(
    /* [in] */ const String& authority,
    /* [in] */ const String& rootId,
    /* [out] */ IUri** uri)
{
    // VALIDATE_NOT_NULL(uri);
    // AutoPtr<IUriBuilder> iub;
    // CUriBuilder::New((IUriBuilder**)&iub);

    // AutoPtr<IUriBuilder> ub;
    // iub->Scheme(IContentResolver::SCHEME_CONTENT, (IUriBuilder**)&ub);

    // AutoPtr<IUriBuilder> ubi;
    // ub->Authority(authority, (IUriBuilder**)&ubi);

    // AutoPtr<IUriBuilder> ubr;
    // ubi->AppendPath(PATH_ROOT, (IUriBuilder**)&ubr);

    // AutoPtr<IUriBuilder> ubd;
    // ubr->AppendPath(rootId, (IUriBuilder**)&ubd);

    // return ubd->Build(uri);
}

ECode DocumentsContract::BuildRecentDocumentsUri(
    /* [in] */ const String& authority,
    /* [in] */ const String& rootId,
    /* [out] */ IUri** uri)
{
    // VALIDATE_NOT_NULL(uri);
    // AutoPtr<IUriBuilder> iub;
    // CUriBuilder::New((IUriBuilder**)&iub);

    // AutoPtr<IUriBuilder> ub;
    // iub->Scheme(IContentResolver::SCHEME_CONTENT, (IUriBuilder**)&ub);

    // AutoPtr<IUriBuilder> ubi;
    // ub->Authority(authority, (IUriBuilder**)&ubi);

    // AutoPtr<IUriBuilder> ubir;
    // ubi->AppendPath(PATH_ROOT, (IUriBuilder**)&ubir);

    // AutoPtr<IUriBuilder> ubd;
    // ubir->AppendPath(rootId, (IUriBuilder**)&ubd);

    // AutoPtr<IUriBuilder> ubdr;
    // ubdr->AppendPath(PATH_RECENT, (IUriBuilder**)&ubdr);

    // return ubdr->Build(uri);
}

ECode DocumentsContract::BuildTreeDocumentUri(
    /* [in] */ const String& authority,
    /* [in] */ const String& documentId,
    /* [out] */ IUri** uri)
{
    // VALIDATE_NOT_NULL(uri);
    //  AutoPtr<IUriBuilder> iub;
    // // CUriBuilder::New((IUriBuilder**)&iub);

    // AutoPtr<IUriBuilder> ub;
    // iub->Scheme(IContentResolver::SCHEME_CONTENT, (IUriBuilder**)&ub);

    // AutoPtr<IUriBuilder> ubi;
    // ub->Authority(authority, (IUriBuilder**)&ubi);

    // AutoPtr<IUriBuilder> ubr;
    // ubi->AppendPath(PATH_TREE, (IUriBuilder**)&ubr);

    // AutoPtr<IUriBuilder> ubd;
    // ubr->AppendPath(documentId, (IUriBuilder**)&ubd);

    // return ubd->Build(uri);
}

ECode DocumentsContract::BuildDocumentUri(
    /* [in] */ const String& authority,
    /* [in] */ const String& documentId,
    /* [out] */ IUri** uri)
{
    // VALIDATE_NOT_NULL(uri);
    // AutoPtr<IUriBuilder> iub;
    // // CUriBuilder::New((IUriBuilder**)&iub);

    // AutoPtr<IUriBuilder> ub;
    // iub->Scheme(IContentResolver::SCHEME_CONTENT, (IUriBuilder**)&ub);

    // AutoPtr<IUriBuilder> ubi;
    // ub->Authority(authority, (IUriBuilder**)&ubi);

    // AutoPtr<IUriBuilder> ubr;
    // ubi->AppendPath(PATH_DOCUMENT, (IUriBuilder**)&ubr);

    // AutoPtr<IUriBuilder> ubd;
    // ubr->AppendPath(documentId, (IUriBuilder**)&ubd);

    // return ubd->Build(uri);
}

ECode DocumentsContract::BuildDocumentUriUsingTree(
    /* [in] */ IUri* treeUri,
    /* [in] */ const String& documentId,
    /* [out] */ IUri** uri)
{
    // VALIDATE_NOT_NULL(uri);
    // String authority;
    // treeUri->GetAuthority(&authority);

    // String docId;
    // GetTreeDocumentId(treeUri, &docId);

    // AutoPtr<IUriBuilder> iub;
    // // CUriBuilder::New((IUriBuilder**)&iub);

    // AutoPtr<IUriBuilder> ub;
    // iub->Scheme(IContentResolver::SCHEME_CONTENT, (IUriBuilder**)&ub);

    // AutoPtr<IUriBuilder> ubi;
    // ub->Authority(authority, (IUriBuilder**)&ubi);

    // AutoPtr<IUriBuilder> ubr;
    // ubi->AppendPath(PATH_TREE, (IUriBuilder**)&ubr);

    // AutoPtr<IUriBuilder> ubd;
    // ubr->AppendPath(docId, (IUriBuilder**)&ubd);

    // AutoPtr<IUriBuilder> ubdr;
    // ubd->AppendPath(PATH_DOCUMENT, (IUriBuilder**)&ubdr);

    // AutoPtr<IUriBuilder> ubder;
    // ubdr->AppendPath(documentId, (IUriBuilder**)&ubder);

    // return ubder->Build(uri);
}

ECode DocumentsContract::BuildDocumentUriMaybeUsingTree(
    /* [in] */ IUri* baseUri,
    /* [in] */ const String& documentId,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    VALIDATE_NOT_NULL(baseUri);

    Boolean bflag = FALSE;
    if ((IsTreeUri(baseUri, &bflag), bflag)) {
        return BuildDocumentUriUsingTree(baseUri, documentId, uri);
    } else {
        String authority;
        baseUri->GetAuthority(&authority);
        return BuildDocumentUri(authority, documentId, uri);
    }
}

ECode DocumentsContract::BuildChildDocumentsUri(
    /* [in] */ const String& authority,
    /* [in] */ const String& parentDocumentId,
    /* [out] */ IUri** uri)
{
    // AutoPtr<IUriBuilder> iub;
    // CUriBuilder::New((IUriBuilder**)&iub);

    // AutoPtr<IUriBuilder> ub;
    // iub->Scheme(IContentResolver::SCHEME_CONTENT, (IUriBuilder**)&ub);

    // AutoPtr<IUriBuilder> ubi;
    // ub->Authority(authority, (IUriBuilder**)&ubi);

    // AutoPtr<IUriBuilder> ubr;
    // ubi->AppendPath(PATH_DOCUMENT, (IUriBuilder**)&ubr);

    // AutoPtr<IUriBuilder> ubd;
    // ubr->AppendPath(parentDocumentId, (IUriBuilder**)&ubd);

    // AutoPtr<IUriBuilder> ubdr;
    // ubd->AppendPath(PATH_CHILDREN, (IUriBuilder**)&ubdr);

    // return ubdr->Build(uri);
}

ECode DocumentsContract::BuildChildDocumentsUriUsingTree(
    /* [in] */ IUri* treeUri,
    /* [in] */ const String& parentDocumentId,
    /* [out] */ IUri** uri)
{
    // VALIDATE_NOT_NULL(uri);
    // String authority;
    // treeUri->GetAuthority(&authority);

    // String docId;
    // GetTreeDocumentId(treeUri, &docId);

    // AutoPtr<IUriBuilder> iub;
    // // CUriBuilder::New((IUriBuilder**)&iub);

    // AutoPtr<IUriBuilder> ub;
    // iub->Scheme(IContentResolver::SCHEME_CONTENT, (IUriBuilder**)&ub);

    // AutoPtr<IUriBuilder> ubi;
    // ub->Authority(authority, (IUriBuilder**)&ubi);

    // AutoPtr<IUriBuilder> ubr;
    // ubi->AppendPath(PATH_TREE, (IUriBuilder**)&ubr);

    // AutoPtr<IUriBuilder> ubd;
    // ubr->AppendPath(docId, (IUriBuilder**)&ubd);

    // AutoPtr<IUriBuilder> ubdr;
    // ubd->AppendPath(PATH_DOCUMENT, (IUriBuilder**)&ubdr);

    // AutoPtr<IUriBuilder> ubder;
    // ubdr->AppendPath(parentDocumentId, (IUriBuilder**)&ubder);

    // AutoPtr<IUriBuilder> ublder;
    // ubder->AppendPath(PATH_CHILDREN, (IUriBuilder**)&ublder);

    // return ublder->Build(uri);
}

ECode DocumentsContract::BuildSearchDocumentsUri(
    /* [in] */ const String& authority,
    /* [in] */ const String& rootId,
    /* [in] */ const String& query,
    /* [out] */ IUri** uri)
{
    // AutoPtr<IUriBuilder> iub;
    // // CUriBuilder::New((IUriBuilder**)&iub);

    // AutoPtr<IUriBuilder> ub;
    // iub->Scheme(IContentResolver::SCHEME_CONTENT, (IUriBuilder**)&ub);

    // AutoPtr<IUriBuilder> ubi;
    // ub->Authority(authority, (IUriBuilder**)&ubi);

    // AutoPtr<IUriBuilder> ubr;
    // ubi->AppendPath(PATH_ROOT, (IUriBuilder**)&ubr);

    // AutoPtr<IUriBuilder> ubd;
    // ubr->AppendPath(rootId, (IUriBuilder**)&ubd);

    // AutoPtr<IUriBuilder> ubdr;
    // ubd->AppendPath(PATH_SEARCH, (IUriBuilder**)&ubdr);

    // AutoPtr<IUriBuilder> ubder;
    // ubdr->AppendQueryParameter(PARAM_QUERY, query, (IUriBuilder**)&ubder);

    // return ubder->Build(uri);
}

ECode DocumentsContract::IsDocumentUri(
    /* [in] */ IContext* context,
    /* [in] */ IUri* uri,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IList> paths;
    uri->GetPathSegments((IList**)&paths);

    Int32 size;
    paths->GetSize(&size);

    AutoPtr<IInterface> obj;
    paths->Get(0, (IInterface**)&obj);
    String str = Object::ToString(obj);

    AutoPtr<IInterface> obj_;
    paths->Get(2, (IInterface**)&obj_);
    String str_ = Object::ToString(obj_);

    String authority;
    uri->GetAuthority(&authority);

    if (size == 2 && PATH_DOCUMENT.Equals(str)) {
        *result = IsDocumentsProvider(context, authority);
        return NOERROR;
    }

    if (size == 4 && PATH_TREE.Equals(str)
            && PATH_DOCUMENT.Equals(str_)) {
        *result = IsDocumentsProvider(context, authority);
        return NOERROR;
    }

    *result = FALSE;
    return NOERROR;
}

ECode DocumentsContract::IsTreeUri(
    /* [in] */ IUri* uri,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    VALIDATE_NOT_NULL(uri);
    AutoPtr<IList> paths;
    uri->GetPathSegments((IList**)&paths);
    Int32 size;
    paths->GetSize(&size);

    AutoPtr<IInterface> obj;
    paths->Get(0, (IInterface**)&obj);
    String str = Object::ToString(obj);

    *result = (size >= 2) && PATH_TREE.Equals(str);
    return NOERROR;
}

Boolean DocumentsContract::IsDocumentsProvider(
    /* [in] */ IContext* context,
    /* [in] */ const String& authority)
{
    // VALIDATE_NOT_NULL(context);
    // AutoPtr<IIntent> intent;
    // CIntent::New(IDocumentsContract::PROVIDER_INTERFACE, (IIntent**)&intent);
    // AutoPtr<IIPackageManager> pmr;
    // context->GetPackageManager((IIPackageManager**)&pmr);
    // AutoPtr<IList> infos;
    // assert(0 && "TODO"); // QueryIntentContentProviders not implement
    // pmr->QueryIntentContentProviders(intent, 0, (IList**)&infos);
    // Int32 size;
    // infos->GetSize(&size);
    // for (Int32 i = 0; i < size; ++i) {
    //     AutoPtr<IInterface> tmp;
    //     if (tmp != NULL) {
    //         tmp = NULL;
    //     }
    //     infos->Get(i, (IInterface**)&tmp);
    //     AutoPtr<IProviderInfo> pi = ((CResolveInfo*)IObject::Probe(tmp))->mProviderInfo;
    //     String authority_;
    //     pi->GetAuthority(&authority_);
    //     if (authority.Equals(authority_)) {
    //         return TRUE;
    //     }
    // }

    // return FALSE;
}

ECode DocumentsContract::GetRootId(
    /* [in] */ IUri* rootUri,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    VALIDATE_NOT_NULL(rootUri);
    AutoPtr<IList> paths;
    rootUri->GetPathSegments((IList**)&paths);
    Int32 size;
    paths->GetSize(&size);

    AutoPtr<IInterface> obj;
    paths->Get(0, (IInterface**)&obj);
    String str = Object::ToString(obj);

    if(size >= 2 && PATH_ROOT.Equals(str)) {
        obj = NULL;
        paths->Get(1, (IInterface**)&obj);
        *result = Object::ToString(obj);
        return NOERROR;
    }
    // throw new IllegalArgumentException("Invalid URI: " + rootUri);
    Slogger::E("DocumentsContract", "Invalid URI: %p", rootUri);
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode DocumentsContract::GetDocumentId(
    /* [in] */ IUri* documentUri,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    VALIDATE_NOT_NULL(documentUri);
    AutoPtr<IList> paths;
    documentUri->GetPathSegments((IList**)&paths);
    Int32 size;
    paths->GetSize(&size);

    AutoPtr<IInterface> obj;
    paths->Get(0, (IInterface**)&obj);
    String str = Object::ToString(obj);

    if(size >= 2 && PATH_DOCUMENT.Equals(str)) {
        obj = NULL;
        paths->Get(1, (IInterface**)&obj);
        *result = Object::ToString(obj);
        return NOERROR;
    }

    obj = NULL;
    paths->Get(2, (IInterface**)&obj);
    String str2 = Object::ToString(obj);
    if (size >= 4 && PATH_TREE.Equals(str)
            && PATH_DOCUMENT.Equals(str2)) {
        obj = NULL;
        paths->Get(3, (IInterface**)&obj);
        *result = Object::ToString(obj);
        return NOERROR;
    }
    // throw new IllegalArgumentException("Invalid URI: " + documentUri);
    Slogger::E("DocumentsContract", "Invalid URI: %p", documentUri);
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode DocumentsContract::GetTreeDocumentId(
    /* [in] */ IUri* documentUri,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    VALIDATE_NOT_NULL(documentUri);
    AutoPtr<IList> paths;
    documentUri->GetPathSegments((IList**)&paths);
    Int32 size;
    paths->GetSize(&size);

    AutoPtr<IInterface> obj;
    paths->Get(0, (IInterface**)&obj);
    String str = Object::ToString(obj);

    if(size >= 2 && PATH_TREE.Equals(str)) {
        obj = NULL;
        paths->Get(1, (IInterface**)&obj);
        *result = Object::ToString(obj);
        return NOERROR;
    }
    // throw new IllegalArgumentException("Invalid URI: " + documentUri);
    Slogger::E("DocumentsContract", "Invalid URI: %p", documentUri);
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode DocumentsContract::GetSearchDocumentsQuery(
    /* [in] */ IUri* searchDocumentsUri,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    VALIDATE_NOT_NULL(searchDocumentsUri);
    return searchDocumentsUri->GetQueryParameter(PARAM_QUERY, result);
}

ECode DocumentsContract::SetManageMode(
    /* [in] */ IUri* uri,
    /* [out] */ IUri** result)
{
    VALIDATE_NOT_NULL(result);
    VALIDATE_NOT_NULL(uri);
    AutoPtr<IUriBuilder> uriBuilder;
    uri->BuildUpon((IUriBuilder**)&uriBuilder);
    uriBuilder->AppendQueryParameter(PARAM_MANAGE, String("TRUE"));
    return uriBuilder->Build(result);
}

ECode DocumentsContract::IsManageMode(
    /* [in] */ IUri* uri,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    VALIDATE_NOT_NULL(uri);
    return uri->GetBooleanQueryParameter(PARAM_MANAGE, FALSE, result);
}

ECode DocumentsContract::GetDocumentThumbnail(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ IUri* documentUri,
    /* [in] */ IPoint* size,
    /* [in] */ ICancellationSignal* signal,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    VALIDATE_NOT_NULL(resolver);
    String authority;
    documentUri->GetAuthority(&authority);

    AutoPtr<IContentProviderClient> client;
    resolver->AcquireUnstableContentProviderClient(authority, (IContentProviderClient**)&client);
    // try {
    // return getDocumentThumbnail(client, documentUri, size, signal);
    ECode ec = GetDocumentThumbnail(client, documentUri, size, signal, bitmap);
    if (SUCCEEDED(ec)) {
        goto EXIT;
        return ec;
    }
    // } catch (Exception e) {
        // if (!(e instanceof OperationCanceledException)) {
        //         Log.w(TAG, "Failed to load thumbnail for " + documentUri + ": " + e);
        //     }
        //     return null;
    if(!SUCCEEDED(ec)){
        if (ec != (ECode)E_OPERATION_CANCELED_EXCEPTION) {
            Slogger::W(TAG, "Failed to load thumbnail for %p : %d", documentUri, ec);
        }
        *bitmap = NULL;
        goto EXIT;
        return NOERROR;
    }
    // } finally {
EXIT:
    CContentProviderClient::ReleaseQuietly(client);
    // }
}

ECode DocumentsContract::GetDocumentThumbnail(
    /* [in] */ IContentProviderClient* client,
    /* [in] */ IUri* documentUri,
    /* [in] */ IPoint* size,
    /* [in] */ ICancellationSignal* signal,
    /* [out] */ IBitmap** bitmap_)
{
    AutoPtr<IBundle> openOpts;
    CBundle::New((IBundle**)&openOpts);
    openOpts->PutParcelable(IContentResolver::EXTRA_SIZE, IParcelable::Probe(size));

    AutoPtr<IAssetFileDescriptor> afd;
    AutoPtr<IBitmap> bitmap;
    // try {
    client->OpenTypedAssetFileDescriptor(documentUri, String("image/*"), openOpts, signal, (IAssetFileDescriptor**)&afd);

    AutoPtr<IFileDescriptor> fd;
    afd->GetFileDescriptor((IFileDescriptor**)&fd);
    Int64 offset;
    afd->GetStartOffset(&offset);

    // Try seeking on the returned FD, since it gives us the most
    // optimal decode path; otherwise fall back to buffering.
    AutoPtr<IBufferedInputStream> is;
    ECode ec = NOERROR;
    Int64 ret;
    ec = Elastos::Droid::System::Os::Lseek(fd.Get(), offset, SEEK_SET, &ret);
    if(!SUCCEEDED(ec)) {
        if (ec == (ECode)E_ERRNO_EXCEPTION) {
            AutoPtr<IFileInputStream> fis;
            CFileInputStream::New(fd, (IFileInputStream**)&fis);

            AutoPtr<IBufferedInputStream> is;
            CBufferedInputStream::New(IInputStream::Probe(fis), THUMBNAIL_BUFFER_SIZE, (IBufferedInputStream**)&is);
            IInputStream::Probe(is)->Mark(THUMBNAIL_BUFFER_SIZE);
        }
    }

    // We requested a rough thumbnail size, but the remote size may have
    // returned something giant, so defensively scale down as needed.
    AutoPtr<CBitmapFactoryOptions> opts;
    CBitmapFactoryOptions::NewByFriend((CBitmapFactoryOptions**)&opts);
    opts->mInJustDecodeBounds = TRUE;
    AutoPtr<IBitmap> btm;
    if (is != NULL) {
        BitmapFactory::DecodeStream(IInputStream::Probe(is), NULL, (IBitmapFactoryOptions*)opts.Get(), (IBitmap**)&btm);
    } else {
        btm = NULL;
        BitmapFactory::DecodeFileDescriptor(fd, NULL, (IBitmapFactoryOptions*)opts.Get(), (IBitmap**)&btm);
    }

    Int32 x, y;
    Int32 outWidth, outHeight;
    opts->GetOutWidth(&outWidth);
    opts->GetOutHeight(&outHeight);
    const Int32 widthSample = outWidth / (size->GetX(&x), x);
    const Int32 heightSample = outHeight / (size->GetY(&y), y);

    opts->mInJustDecodeBounds = FALSE;
    opts->mInSampleSize = Elastos::Core::Math::Min(widthSample, heightSample);
    if (is != NULL) {
        IInputStream::Probe(is)->Reset();
        BitmapFactory::DecodeStream(IInputStream::Probe(is), NULL, (IBitmapFactoryOptions*)opts.Get(), (IBitmap**)&bitmap);
    } else {
        // try {
        ec = Elastos::Droid::System::Os::Lseek(fd.Get(), offset, SEEK_SET, &ret);

        // } catch (ErrnoException e) {
        if(!SUCCEEDED(ec)) {
            // e.rethrowAsIOException();
            if (ec == E_ERRNO_EXCEPTION) {
                goto EXIT;
                return E_IO_EXCEPTION;
            }
        }
        // }
        BitmapFactory::DecodeFileDescriptor(fd, NULL, (IBitmapFactoryOptions*)opts.Get(), (IBitmap**)&bitmap);
    }

    // Transform the bitmap if requested. We use a side-channel to
    // communicate the orientation, since EXIF thumbnails don't contain
    // the rotation flags of the original image.
    Int32 orientation;
    {
        AutoPtr<IBundle> extras;
        afd->GetExtras((IBundle**)&extras);
        if (extras == NULL) {
            orientation = 0;
        } else {
            extras->GetInt32(IDocumentsContract::EXTRA_ORIENTATION, 0, &orientation);
        }
    }

    if (orientation != 0) {
        Int32 width, height;
        bitmap->GetWidth(&width);
        bitmap->GetHeight(&height);

        AutoPtr<IMatrix> m;
        CMatrix::New((IMatrix**)&m);
        m->SetRotate((Float)orientation, width / 2, height / 2);
        CBitmap::CreateBitmap(bitmap, 0, 0, width, height, m, FALSE, (IBitmap**)&bitmap);
    }

    *bitmap_ = bitmap;
    REFCOUNT_ADD(*bitmap_);
    goto EXIT;
    return NOERROR;
    // } finally {
EXIT:
    {
    AutoPtr<IIoUtils> iou;
    CIoUtils::AcquireSingleton((IIoUtils**)&iou);
    iou->CloseQuietly(ICloseable::Probe(afd));
    }
    //     }
}

ECode DocumentsContract::CreateDocument(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ IUri* parentDocumentUri,
    /* [in] */ const String& mimeType,
    /* [in] */ const String& displayName,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    VALIDATE_NOT_NULL(resolver);
    VALIDATE_NOT_NULL(parentDocumentUri);
    String authority;
    parentDocumentUri->GetAuthority(&authority);
    AutoPtr<IContentProviderClient> client;
    resolver->AcquireUnstableContentProviderClient(authority, (IContentProviderClient**)&client);
    // try {
    ECode ec = CreateDocument(client, parentDocumentUri, mimeType, displayName, uri);
    // } catch (Exception e) {
    if(SUCCEEDED(ec)) {
        goto EXIT;
        return ec;
    }

    if(!SUCCEEDED(ec)) {
        Slogger::W(TAG, "Failed to create document");
        *uri = NULL;
        goto EXIT;
        return NOERROR;
    }
    // } finally {
EXIT:
    CContentProviderClient::ReleaseQuietly(client);
    // }
}

/** {@hide} */
ECode DocumentsContract::CreateDocument(
    /* [in] */ IContentProviderClient* client,
    /* [in] */ IUri* parentDocumentUri,
    /* [in] */ const String& mimeType,
    /* [in] */ const String& displayName,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    VALIDATE_NOT_NULL(client);
    AutoPtr<IBundle> in;
    CBundle::New((IBundle**)&in);
    in->PutParcelable(IDocumentsContract::EXTRA_URI, IParcelable::Probe(parentDocumentUri));
    in->PutString(IDocumentsContractDocument::COLUMN_MIME_TYPE, mimeType);
    in->PutString(IDocumentsContractDocument::COLUMN_DISPLAY_NAME, displayName);

    AutoPtr<IBundle> out;
    client->Call(IDocumentsContract::METHOD_CREATE_DOCUMENT, String(NULL), in.Get(), (IBundle**)&out);
    AutoPtr<IParcelable> ipl;
    out->GetParcelable(IDocumentsContract::EXTRA_URI, (IParcelable**)&ipl);
    *uri = IUri::Probe(ipl);
    REFCOUNT_ADD(*uri);
    return NOERROR;
}

ECode DocumentsContract::RenameDocument(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ IUri* documentUri,
    /* [in] */ const String& displayName,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    VALIDATE_NOT_NULL(resolver);
    String authority;
    documentUri->GetAuthority(&authority);

    AutoPtr<IContentProviderClient> client;
    resolver->AcquireUnstableContentProviderClient(authority, (IContentProviderClient**)&client);
    // try {
    ECode ec = NOERROR;
    ec = RenameDocument(client, documentUri, displayName, uri);
    // } catch (Exception e) {
    if(SUCCEEDED(ec)) {
        goto EXIT;
        return ec;
    }

    if(!SUCCEEDED(ec)) {
        Slogger::W(TAG, "Failed to rename document");
        *uri = NULL;
        goto EXIT;
        return NOERROR;
    }
    // } finally {
EXIT:
    CContentProviderClient::ReleaseQuietly(client);
    //     ContentProviderClient.releaseQuietly(client);
    // }
}

/** {@hide} */
ECode DocumentsContract::RenameDocument(
    /* [in] */ IContentProviderClient* client,
    /* [in] */ IUri* documentUri,
    /* [in] */ const String& displayName,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    VALIDATE_NOT_NULL(client);
    AutoPtr<IBundle> in;
    CBundle::New((IBundle**)&in);
    in->PutParcelable(IDocumentsContract::EXTRA_URI, IParcelable::Probe(documentUri));
    in->PutString(IDocumentsContractDocument::COLUMN_DISPLAY_NAME, displayName);

    AutoPtr<IBundle> out;
    client->Call(IDocumentsContract::METHOD_RENAME_DOCUMENT, String(NULL), in.Get(), (IBundle**)&out);

    AutoPtr<IParcelable> pl;
    out->GetParcelable(IDocumentsContract::EXTRA_URI, (IParcelable**)&pl);
    AutoPtr<IUri> outUri = IUri::Probe(pl);
    *uri = (outUri != NULL) ? outUri.Get() : documentUri;
    REFCOUNT_ADD(*uri);
    return NOERROR;
}

ECode DocumentsContract::DeleteDocument(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ IUri* documentUri,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    VALIDATE_NOT_NULL(resolver);
    VALIDATE_NOT_NULL(documentUri);
    String authority;
    documentUri->GetAuthority(&authority);
    AutoPtr<IContentProviderClient> client;
    resolver->AcquireUnstableContentProviderClient(authority, (IContentProviderClient**)&client);
    // try {
    ECode ec = NOERROR;
    ec = DeleteDocument(client, documentUri);
    if (SUCCEEDED(ec)) {
        *result = TRUE;
        goto EXIT;
        return NOERROR;
    }

    if (!SUCCEEDED(ec)) {
        // } catch (Exception e) {
        Slogger::W(TAG, "Failed to delete document");
        *result = FALSE;
        goto EXIT;
        return NOERROR;
    }
    // } finally {
EXIT:
    CContentProviderClient::ReleaseQuietly(client);
}

/** {@hide} */
ECode DocumentsContract::DeleteDocument(
    /* [in] */ IContentProviderClient* client,
    /* [in] */ IUri* documentUri)
{
    AutoPtr<IBundle> in;
    CBundle::New((IBundle**)&in);
    in->PutParcelable(IDocumentsContract::EXTRA_URI, IParcelable::Probe(documentUri));

    AutoPtr<IBundle> bd;
    return client->Call(IDocumentsContract::METHOD_DELETE_DOCUMENT, String(NULL), in.Get(), (IBundle**)&bd);
}

/** {@hide} */
ECode DocumentsContract::OpenImageThumbnail(
    /* [in] */ IFile* file,
    /* [out] */ IAssetFileDescriptor** afd)
{
    VALIDATE_NOT_NULL(afd);
    AutoPtr<IParcelFileDescriptor> pfd;
    ParcelFileDescriptor::Open(file, IParcelFileDescriptor::MODE_READ_ONLY, (IParcelFileDescriptor**)&pfd);
    AutoPtr<IBundle> extras;

    String path;
    file->GetAbsolutePath(&path);
    AutoPtr<IExifInterface> exif;
    assert(0 && "TODO");
    // CExifInterface::New((IExifInterface**)&exif);

    Int32 i;
    exif->GetAttributeInt32(IExifInterface::TAG_ORIENTATION, -1, &i);
    switch (i) {
        case IExifInterface::ORIENTATION_ROTATE_90 :
            CBundle::New(1, (IBundle**)&extras);
            extras->PutInt32(IDocumentsContract::EXTRA_ORIENTATION, 90);
            break;
        case IExifInterface::ORIENTATION_ROTATE_180:
            extras =  NULL;
            CBundle::New(1, (IBundle**)&extras);
            extras->PutInt32(IDocumentsContract::EXTRA_ORIENTATION, 180);
            break;
        case IExifInterface::ORIENTATION_ROTATE_270:
            extras =  NULL;
            CBundle::New(1, (IBundle**)&extras);
            extras->PutInt32(IDocumentsContract::EXTRA_ORIENTATION, 270);
            break;
    }

    AutoPtr<ArrayOf<Int64> > thumb;
    exif->GetThumbnailRange((ArrayOf<Int64>**)&thumb);
    if (thumb != NULL) {
       CAssetFileDescriptor::New(pfd, (*thumb)[0], (*thumb)[1], extras.Get(), afd);
       return NOERROR;
    }

    CAssetFileDescriptor::New(pfd, 0, IAssetFileDescriptor::UNKNOWN_LENGTH, extras.Get(), afd);
    return NOERROR;
}

} // namespace Provider
} // namespace Droid
} // namespace Elastos