
#include "content/ContentProvider.h"
#ifdef DROID_CORE
#include "content/CContentProviderTransport.h"
#include "content/CClipDescriptionHelper.h"
#include "content/CContentResolverHelper.h"
#include "content/res/CAssetFileDescriptor.h"
#include "os/CParcelFileDescriptorHelper.h"
#endif
#include "os/Process.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;
using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Content::Res::CAssetFileDescriptor;
using Elastos::Droid::Os::IParcelFileDescriptorHelper;
using Elastos::Droid::Os::CParcelFileDescriptorHelper;
using Elastos::Droid::Os::Process;

namespace Elastos {
namespace Droid {
namespace Content {

ContentProvider::OpenPipeAsyncTask::OpenPipeAsyncTask(
    /* [in] */ IContentProviderPipeDataWriter* func,
    /* [in] */ ArrayOf<IParcelFileDescriptor*>* fds,
    /* [in] */ IUri* uri,
    /* [in] */ const String& mimeType,
    /* [in] */ IBundle* opts,
    /* [in] */ IInterface* args)
    : mFunc(func)
    , mFds(fds)
    , mUri(uri)
    , mMimeType(mimeType)
    , mOpts(opts)
    , mArgs(args)
{}

AutoPtr<IInterface> ContentProvider::OpenPipeAsyncTask::DoInBackground(
    /* [in] */ ArrayOf<IInterface*>* params)
{
    mFunc->WriteDataToPipe((*mFds)[1], mUri, mMimeType, mOpts, mArgs);
    // try {
    if (FAILED((*mFds)[1]->Close())) {
        Logger::W("ContentProvider", "Failure closing pipe");
    }
    // } catch (IOException e) {
    //     Log.w(TAG, "Failure closing pipe", e);
    // }
    return NULL;
}


const String ContentProvider::TAG("ContentProvider");

ContentProvider::ContentProvider()
    : mMyUid(0)
    , mExported(FALSE)
{
    ASSERT_SUCCEEDED(CContentProviderTransport::New(
            (Handle32)this, (IContentProviderTransport**)&mTransport));
}

ECode ContentProvider::Init(
    /* [in] */ IContext* context,
    /* [in] */ const String& readPermission,
    /* [in] */ const String& writePermission,
    /* [in] */ ArrayOf<IPathPermission*>* pathPermissions)
{
    mContext = context;
    mReadPermission = readPermission;
    mWritePermission = writePermission;
    mPathPermissions = pathPermissions;
    return CContentProviderTransport::New(
            (Handle32)this, (IContentProviderTransport**)&mTransport);
}

ContentProvider::~ContentProvider()
{}

UInt32 ContentProvider::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ContentProvider::Release()
{
    return ElRefBase::Release();
}

PInterface ContentProvider::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IContentProvider*)this;
    }
    else if (riid == EIID_IObject) {
        return (IObject*)this;
    }
    else if (riid == EIID_IContentProvider) {
       return (IContentProvider*)this;
    }
    else if (riid == EIID_IComponentCallbacks2) {
       return (IComponentCallbacks2*)this;
    }
    else if (riid == EIID_IComponentCallbacks) {
        return (IComponentCallbacks*)this;
    }
    else if (riid == EIID_IWeakReferenceSource) {
        return (IWeakReferenceSource*)this;
    }

    return NULL;
}

ECode ContentProvider::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IObject*)this) {
        *pIID = EIID_IObject;
    }
    else if (pObject == (IInterface*)(IContentProvider*)this) {
        *pIID = EIID_IContentProvider;
    }
    else if (pObject == (IInterface*)(IComponentCallbacks2*)this) {
        *pIID = EIID_IComponentCallbacks2;
    }
    else if (pObject == (IInterface*)(IComponentCallbacks*)this) {
        *pIID = EIID_IComponentCallbacks;
    }
    else if (pObject == (IInterface*)(IWeakReferenceSource*)this) {
        *pIID = EIID_IWeakReferenceSource;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode ContentProvider::Initialize()
{
    return NOERROR;
}

ECode ContentProvider::Aggregate(
    /* [in] */ AggrType aggrType,
    /* [in] */ PInterface pObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode ContentProvider::GetDomain(
    /* [out] */ PInterface *ppObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode ContentProvider::GetClassID(
    /* [out] */ ClassID *pCLSID)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode ContentProvider::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(other);

    IContentProvider * o = IContentProvider::Probe(other);
    if (o != NULL) {
        *result = (o == THIS_PROBE(IContentProvider));
    }
    return NOERROR;
}

ECode ContentProvider::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash);
    *hash = (Int32)THIS_PROBE(IContentProvider);
    return NOERROR;
}

ECode ContentProvider::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    StringBuilder sb("ContentProvider:(");
    sb += (Int32)THIS_PROBE(IContentProvider);
    sb += ")";
    sb.ToString(info);
    return NOERROR;
}

ECode ContentProvider::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(THIS_PROBE(IInterface), CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}

ECode ContentProvider::CoerceToLocalContentProvider(
    /* [in] */ IIContentProvider* abstractInterface,
    /* [out] */ IContentProvider** localProvider)
{
    VALIDATE_NOT_NULL(localProvider)
    if (IContentProviderTransport::Probe(abstractInterface) != NULL) {
        return IContentProviderTransport::Probe(abstractInterface)->GetContentProvider(localProvider);
    }
    *localProvider = NULL;
    return NOERROR;
}

ECode ContentProvider::GetContext(
    /* [out] */ IContext** context)
{
    VALIDATE_NOT_NULL(context)
    *context = mContext;
    REFCOUNT_ADD(*context)
    return NOERROR;
}

ECode ContentProvider::SetReadPermission(
    /* [in] */ const String& permission)
{
    mReadPermission = permission;
    return NOERROR;
}

ECode ContentProvider::GetReadPermission(
    /* [out] */ String* permissionName)
{
    VALIDATE_NOT_NULL(permissionName)
    *permissionName = mReadPermission;
    return NOERROR;
}

ECode ContentProvider::SetWritePermission(
    /* [in] */ const String& permission)
{
    mWritePermission = permission;
    return NOERROR;
}

ECode ContentProvider::GetWritePermission(
    /* [out] */ String* permissionName)
{
    VALIDATE_NOT_NULL(permissionName)
    *permissionName = mWritePermission;
    return NOERROR;
}

ECode ContentProvider::SetPathPermissions(
    /* [in] */ ArrayOf<IPathPermission*>* permissions)
{
    mPathPermissions = permissions;
    return NOERROR;
}

ECode ContentProvider::GetPathPermissions(
    /* [out, callee] */ ArrayOf<IPathPermission*>** pathPermissions)
{
    VALIDATE_NOT_NULL(pathPermissions)
    *pathPermissions = mPathPermissions;
    ARRAYOF_ADDREF(*pathPermissions);
    return NOERROR;
}

ECode ContentProvider::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    return NOERROR;
}

ECode ContentProvider::OnLowMemory()
{
    return NOERROR;
}

ECode ContentProvider::OnTrimMemory(
    /* [in] */ Int32 level)
{
    return NOERROR;
}

ECode ContentProvider::Query(
    /* [in] */ IUri* uri,
    /* [in] */ ArrayOf<String>* projection,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& sortOrder,
    /* [in] */ ICancellationSignal* cancellationSignal,
    /* [out] */ ICursor** cursor)
{
    return Query(uri, projection, selection, selectionArgs, sortOrder, cursor);
}

ECode ContentProvider::BulkInsert(
    /* [in] */ IUri* uri,
    /* [in] */ ArrayOf<IContentValues*>* values,
    /* [out] */ Int32* number)
{
    Int32 numValues = values->GetLength();
    for (Int32 i = 0; i < numValues; i++) {
        AutoPtr<IUri> insertUri;
        FAIL_RETURN(Insert(uri, (*values)[i], (IUri**)&insertUri))
    }
    *number = numValues;
    return NOERROR;
}

ECode ContentProvider::OpenFile(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IParcelFileDescriptor** fileDescriptor)
{
    VALIDATE_NOT_NULL(fileDescriptor)
    *fileDescriptor = NULL;
    VALIDATE_NOT_NULL(uri);

    String str;
    uri->ToString(&str);
    Logger::E(TAG, "No files supported by provider at %s", str.string());
    return E_FILE_NOT_FOUND_EXCEPTION;
}

ECode ContentProvider::OpenAssetFile(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IAssetFileDescriptor** fileDescriptor)
{
    VALIDATE_NOT_NULL(fileDescriptor)
    *fileDescriptor = NULL;

    AutoPtr<IParcelFileDescriptor> fd;
    FAIL_RETURN(OpenFile(uri, mode, (IParcelFileDescriptor**)&fd));
    if (NULL != fd) {
        return CAssetFileDescriptor::New(fd, 0, -1, fileDescriptor);
    }
    return NOERROR;
}

ECode ContentProvider::OpenFileHelper(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IParcelFileDescriptor** fileDescriptor)
{
    VALIDATE_NOT_NULL(fileDescriptor)
    AutoPtr< ArrayOf<String> > projection = ArrayOf<String>::Alloc(1);
    (*projection)[0] = "_data";
    AutoPtr<ICursor> c;
    FAIL_RETURN(Query(uri, projection, String(NULL), NULL, String(NULL), (ICursor**)&c));
    Int32 count = 0;
    if (c != NULL) {
        c->GetCount(&count);
    }
    if (count != 1) {
        // If there is not exactly one result, throw an appropriate
        // exception.
        if (c != NULL) {
            c->Close();
        }
        if (count == 0) {
            //throw new FileNotFoundException("No entry for " + uri);
            String str;
            uri->ToString(&str);
            Logger::E(TAG, "No entry for %s", str.string());
            return E_FILE_NOT_FOUND_EXCEPTION;
        }
        //throw new FileNotFoundException("Multiple items at " + uri);
        String str;
        uri->ToString(&str);
        Logger::E(TAG, "Multiple items at %s", str.string());
        return E_FILE_NOT_FOUND_EXCEPTION;
    }

    Boolean succeeded = FALSE;
    c->MoveToFirst(&succeeded);
    Int32 i = 0;
    c->GetColumnIndex(String("_data"), &i);
    String path;
    if (i >= 0) {
        c->GetString(i, &path);
    }
    c->Close();
    if (path.IsNull()) {
        //throw new FileNotFoundException("Column _data not found.");
        Logger::E(TAG, "Column _data not found.");
        return E_FILE_NOT_FOUND_EXCEPTION;
    }

    Int32 modeBits = 0;
    AutoPtr<IContentResolverHelper> resolverHelper;
    CContentResolverHelper::AcquireSingleton((IContentResolverHelper**)&resolverHelper);
    resolverHelper->ModeToMode(uri, mode, &modeBits);
    AutoPtr<IFile> file;
    CFile::New(path, (IFile**)&file);
    AutoPtr<IParcelFileDescriptorHelper> parcelHelper;
    CParcelFileDescriptorHelper::AcquireSingleton((IParcelFileDescriptorHelper**)&parcelHelper);
    return parcelHelper->Open(file, modeBits, fileDescriptor);
}

ECode ContentProvider::GetStreamTypes(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mimeTypeFilter,
    /* [out, callee] */ ArrayOf<String>** streamTypes)
{
    VALIDATE_NOT_NULL(streamTypes)
    *streamTypes = NULL;
    return NOERROR;
}

ECode ContentProvider::OpenTypedAssetFile(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mimeTypeFilter,
    /* [in] */ IBundle* opts,
    /* [out] */ IAssetFileDescriptor** fileDescriptor)
{
    VALIDATE_NOT_NULL(fileDescriptor)
    *fileDescriptor = NULL;

    if (String("*/*").Equals(mimeTypeFilter)) {
        // If they can take anything, the untyped open call is good enough.
        return OpenAssetFile(uri, String("r"), fileDescriptor);
    }
    String baseType;
    GetType(uri, &baseType);
    if (!baseType.IsNull()) {
        AutoPtr<IClipDescriptionHelper> descriptionHelper;
        CClipDescriptionHelper::AcquireSingleton((IClipDescriptionHelper**)&descriptionHelper);
        Boolean result = FALSE;
        descriptionHelper->CompareMimeTypes(baseType, mimeTypeFilter, &result);
        // Use old untyped open call if this provider has a type for this
        // URI and it matches the request.
        if (result) return OpenAssetFile(uri, String("r"), fileDescriptor);
    }
    // throw new FileNotFoundException("Can't open " + uri + " as type " + mimeTypeFilter);
    String str;
    uri->ToString(&str);
    Logger::E(TAG, "Can't open %s as type %s", str.string(), mimeTypeFilter.string());
    return E_FILE_NOT_FOUND_EXCEPTION;
}

ECode ContentProvider::OpenPipeHelper(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mimeType,
    /* [in] */ IBundle* opts,
    /* [in] */ IInterface* args,
    /* [in] */ IContentProviderPipeDataWriter* func,
    /* [out] */ IParcelFileDescriptor** fileDescriptor)
{
    VALIDATE_NOT_NULL(fileDescriptor)
    *fileDescriptor = NULL;
    // try {
    AutoPtr<IParcelFileDescriptorHelper> helper;
    CParcelFileDescriptorHelper::AcquireSingleton((IParcelFileDescriptorHelper**)&helper);
    AutoPtr< ArrayOf<IParcelFileDescriptor*> > fds;
    ECode ec = helper->CreatePipe((ArrayOf<IParcelFileDescriptor*>**)&fds);
    if (FAILED(ec)) {
        Logger::E(TAG, "failure making pipe");
        return ec;
    }

    AutoPtr<OpenPipeAsyncTask> task = new OpenPipeAsyncTask(func, fds, uri, mimeType, opts, args);
    ec = task->ExecuteOnExecutor(AsyncTask::THREAD_POOL_EXECUTOR, NULL);
    if (FAILED(ec)) {
        Logger::E(TAG, "failure making pipe");
        return E_FILE_NOT_FOUND_EXCEPTION;
    }
    *fileDescriptor = (*fds)[0];
    REFCOUNT_ADD(*fileDescriptor)
    return NOERROR;
    // } catch (IOException e) {
    //     throw new FileNotFoundException("failure making pipe");
    // }
}

Boolean ContentProvider::IsTemporary()
{
    return FALSE;
}

ECode ContentProvider::GetIContentProvider(
    /* [out] */ IIContentProvider** provider)
{
    VALIDATE_NOT_NULL(provider)
    *provider = IIContentProvider::Probe(mTransport);
    REFCOUNT_ADD(*provider)
    return NOERROR;
}

ECode ContentProvider::AttachInfo(
    /* [in] */ IContext* context,
    /* [in] */ IProviderInfo* info)
{
    /*
     * We may be using AsyncTask from binder threads.  Make it init here
     * so its static handler is on the main thread.
     */
    AsyncTask::Init();

    /*
     * Only allow it to be set once, so after the content service gives
     * this to us clients can't change it.
     */
    if (NULL == mContext) {
        mContext = context;
        mMyUid = Process::MyUid();
        if (NULL != info) {
            String readPermission;
            String writePermission;
            AutoPtr< ArrayOf<IPathPermission*> > permissions;
            info->GetReadPermission(&readPermission);
            SetReadPermission(readPermission);
            info->GetWritePermission(&writePermission);
            SetWritePermission(writePermission);
            info->GetPathPermissions((ArrayOf<IPathPermission*>**)&permissions);
            SetPathPermissions(permissions);
            info->GetExported(&mExported);
        }
        Boolean result = FALSE;
        return OnCreate(&result);
    }
    return NOERROR;
}

ECode ContentProvider::ApplyBatch(
    /* [in] */ IObjectContainer* operations,
    /* [out, callee] */ ArrayOf<IContentProviderResult*>** providerResults)
{
    VALIDATE_NOT_NULL(operations)
    VALIDATE_NOT_NULL(providerResults)
    Int32 i = 0;
    Int32 count = 0;
    operations->GetObjectCount(&count);
    AutoPtr< ArrayOf<IContentProviderResult*> > results = ArrayOf<IContentProviderResult*>::Alloc(count);
    AutoPtr<IObjectEnumerator> ObjEnumerator;
    FAIL_RETURN(operations->GetObjectEnumerator((IObjectEnumerator**)&ObjEnumerator))
    Boolean hasNext = FALSE;
    while ((ObjEnumerator->MoveNext(&hasNext), hasNext)) {
        AutoPtr<IContentProviderOperation> operation;
        ObjEnumerator->Current((IInterface**)&operation);
        AutoPtr<IContentProviderResult> result;
        FAIL_RETURN(operation->Apply(this, results, i, (IContentProviderResult**)&result))
        results->Set(i, result);
        i++;
    }
    *providerResults = results;
    REFCOUNT_ADD(*providerResults);
    return NOERROR;
}

ECode ContentProvider::Call(
    /* [in] */ const String& method,
    /* [in] */ const String& arg,
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** bundle)
{
    VALIDATE_NOT_NULL(bundle)
    *bundle = NULL;
    return NOERROR;
}

ECode ContentProvider::Shutdown()
{
    Logger::W(TAG, "implement ContentProvider shutdown() to make sure all database connections are gracefully shutdown");
    return NOERROR;
}

ECode ContentProvider::Dump(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* writer,
    /* [in] */ ArrayOf<String>* args)
{
    return writer->PrintStringln(String("nothing to dump"));
}

} // namespace Content
} // namespace Droid
} // namespace Elastos
