
#include "content/ContentProvider.h"
#include "content/CContentProviderTransport.h"
#include "content/CClipDescriptionHelper.h"
#include "content/CContentResolverHelper.h"
#include "content/res/CAssetFileDescriptor.h"
#include "os/CParcelFileDescriptorHelper.h"
#include "os/Process.h"
#include "os/Binder.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringBuilder.h>

using Elastos::Droid::Content::Res::CAssetFileDescriptor;
using Elastos::Droid::Os::IParcelFileDescriptorHelper;
using Elastos::Droid::Os::CParcelFileDescriptorHelper;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Os::Binder;
using Elastos::Core::StringBuilder;
using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Content {

//================================================================
// ContentProvider::OpenPipeAsyncTask
//================================================================
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


//================================================================
// ContentProvider
//================================================================
const String ContentProvider::TAG("ContentProvider");

pthread_key_t ContentProvider::sTlsKey;
pthread_once_t ContentProvider::sKeyOnce = PTHREAD_ONCE_INIT;

CAR_INTERFACE_IMPL_3(ContentProvider, Object, IContentProvider, IComponentCallbacks, IComponentCallbacks2)

static void ThreadDestructor(void* st)
{
    ICharSequence* callingPackage = static_cast<String*>(st);
    if (callingPackage) {
        REFCOUNT_RELEASE(callingPackage)
    }
}

static void MakeKey()
{
    ASSERT_TRUE(pthread_key_create(&ContentProvider::sTlsKey, ThreadDestructor) == 0);
}

String ContentProvider::GetCallingPackage()
{
    pthread_once(&sKeyOnce, MakeKey);

    AutoPtr<ICharSequence> callingPackage = (ICharSequence*)pthread_getspecific(sTlsKey);
    if (callingPackage == NULL) {
        CString::New(String(""), (ICharSequence**)&callingPackage);

        ASSERT_TRUE(pthread_setspecific(sTlsKey, callingPackage.Get()) == 0);
        callingPackage->AddRef();
    }
    assert(callingPackage.Get() != NULL && "check GetCallingPackage failed!");
    String value;
    callingPackage->ToString(&value);
    return value;
}

void ContentProvider::SetCallingPackage(
    /* [in] */ const String& pakcage)
{
    pthread_once(&sKeyOnce, MakeKey);

    AutoPtr<ICharSequence> callingPackage = (ICharSequence*)pthread_getspecific(sTlsKey);
    if (callingPackage != NULL) {
        REFCOUNT_RELEASE(callingPackage)
    }

    callingPackage = NULL;
    CString::New(pakcage, (ICharSequence**)&callingPackage);
    ASSERT_TRUE(pthread_setspecific(sTlsKey, callingPackage.Get()) == 0);
    callingPackage->AddRef();
}

ContentProvider::ContentProvider()
    : mMyUid(0)
    , mExported(FALSE)
    , mNoPerms(FALSE)
    , mSingleUser(FALSE)
{
}

ContentProvider::~ContentProvider()
{}


ECode ContentProvider::constructor()
{
    ASSERT_SUCCEEDED(CContentProviderTransport::New(
            (Handle32)this, (IContentProviderTransport**)&mTransport));
}

ECode ContentProvider::constructor(
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

ECode ContentProvider::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    StringBuilder sb("ContentProvider:(");
    sb += StringUtils::ToString((Int32)this);
    sb += ")";
    sb.ToString(info);
    return NOERROR;
}

ECode ContentProvider::CoerceToLocalContentProvider(
    /* [in] */ IIContentProvider* abstractInterface,
    /* [out] */ IContentProvider** localProvider)
{
    VALIDATE_NOT_NULL(localProvider)

    IContentProviderTransport* transport = IContentProviderTransport::Probe(abstractInterface);
    if (transport != NULL) {
        return transport->GetContentProvider(localProvider);
    }

    *localProvider = NULL;
    return NOERROR;
}

Boolean ContentProvider::CheckUser(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ IContext* context)
{
    Int32 cuid, perm;
    context->GetUserId(&cuid);
    context->CheckPermission(INTERACT_ACROSS_USERS, pid, uid, &perm);
    return UserHandle::GetUserId(uid) == cuid
            || mSingleUser
            || perm == PERMISSION_GRANTED;
}

ECode ContentProvider::EnforceReadPermissionInner(
    /* [in] */ IUri* uri)
{
    AutoPtr<IContext> context = GetContext();
    Int32 pid = Binder::GetCallingPid();
    Int32 uid = Binder::GetCallingUid();
    String missingPerm;

    if (UserHandle::IsSameApp(uid, mMyUid)) {
        return NOERROR;
    }

    Int32 checkPerm;
    if (mExported && CheckUser(pid, uid, context)) {
        String componentPerm;
        GetReadPermission(&componentPerm);
        if (!componentPerm.IsNull()) {
            context->CheckPermission(componentPerm, pid, uid, &checkPerm);
            if (checkPerm == PERMISSION_GRANTED) {
                return NOERROR;
            }
            else {
                missingPerm = componentPerm;
            }
        }

        // track if unprotected read is allowed; any denied
        // <path-permission> below removes this ability
        Boolean allowDefaultRead = componentPerm.IsNull();

        AutoPtr<ArrayOf<IPathPermission*> > pps = GetPathPermissions();
        if (pps != NULL) {
            String path, pathPerm;
            Boolean match;
            uri->GetPath(&path);
            IPathPermission* pp;
            for (Int32 i = 0; i < pps->GetLength(); ++i) {
                pp = (*pps)[i];
                pp->GetReadPermission(&pathPerm);
                if (!pathPerm.IsNull() && (pp->Match(path, &match), match)) {
                    context->CheckPermission(pathPerm, pid, uid, &checkPerm)
                    if (checkPerm == PERMISSION_GRANTED) {
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
    context->CheckUriPermission(uri, pid, uid, IIntent::FLAG_GRANT_READ_URI_PERMISSION, &checkPerm);
    if (checkPerm == PERMISSION_GRANTED) {
        return NOERROR;
    }

    String failReason = mExported
            ? " requires " + missingPerm + ", or grantUriPermission()"
            : " requires the provider be exported, or grantUriPermission()";
    throw new SecurityException("Permission Denial: reading "
            + ContentProvider.this.getClass().getName() + " uri " + uri + " from pid=" + pid
            + ", uid=" + uid + failReason);
    StringBuilder sb;
    sb.Append("Permission Denial: reading ");
    sb.Append(Object::ToString(this));
    sb.Append(" uri ");
    sb.Append(Object::ToString(uri));
    sb.Append(" from pid=");
    sb.Append(pid);
    sb.Append(", uid=");
    sb.Append(uid);
    if (mExported) {
        sb.Append(" requires ");
        sb.Append(missingPerm);
        sb.Append(", or grantUriPermission()");
    }
    else {
        sb.Append( requires the provider be exported, or grantUriPermission());
    }
    Logger::E(TAG, sb.ToString());
    return E_SECURITY_EXCEPTION;
}

ECode ContentProvider::EnforceWritePermissionInner(
    /* [in] */ IUri* uri)
{
    AutoPtr<IContext> context = GetContext();
    Int32 pid = Binder::GetCallingPid();
    Int32 uid = Binder::GetCallingUid();
    String missingPerm = NULL;

    if (UserHandle::IsSameApp(uid, mMyUid)) {
        return;
    }

    if (mExported && checkUser(pid, uid, context)) {
        String componentPerm;
        GetWritePermission(&componentPerm);
        if (componentPerm != NULL) {
            if (context->CheckPermission(componentPerm, pid, uid) == PERMISSION_GRANTED) {
                return;
            } else {
                missingPerm = componentPerm;
            }
        }

        // track if unprotected write is allowed; any denied
        // <path-permission> below removes this ability
        Boolean allowDefaultWrite = (componentPerm == NULL);

        PathPermission[] pps = getPathPermissions();
        if (pps != NULL) {
            String path = uri.getPath();
            for (PathPermission pp : pps) {
                String pathPerm = pp.GetWritePermission();
                if (pathPerm != NULL && pp.match(path)) {
                    if (context->CheckPermission(pathPerm, pid, uid) == PERMISSION_GRANTED) {
                        return;
                    } else {
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
        if (allowDefaultWrite) return;
    }

    // last chance, check against any uri grants
    if (context.checkUriPermission(uri, pid, uid, Intent.FLAG_GRANT_WRITE_URI_PERMISSION)
            == PERMISSION_GRANTED) {
        return;
    }

    String failReason = mExported
            ? " requires " + missingPerm + ", or grantUriPermission()"
            : " requires the provider be exported, or grantUriPermission()";
    throw new SecurityException("Permission Denial: writing "
            + ContentProvider.this.getClass().getName() + " uri " + uri + " from pid=" + pid
            + ", uid=" + uid + failReason);
}

ECode ContentProvider::GetContext(
    /* [out] */ IContext** context)
{
    VALIDATE_NOT_NULL(context)
    *context = mContext;
    REFCOUNT_ADD(*context)
    return NOERROR;
}

String ContentProvider::SetCallingPackage(
    /* [in] */ const String& callingPackage)
{
    String original = mCallingPackage.get();
    mCallingPackage.set(callingPackage);
    return original;
}

ECode ContentProvider::GetCallingPackage(
    /* [out] */ String* callingPackage)
{
    String pkg = mCallingPackage.get();
    if (pkg != NULL) {
        mTransport.mAppOpsManager.checkPackage(Binder::GetCallingUid(), pkg);
    }
    return pkg;
}

ECode ContentProvider::SetAuthorities(
    /* [in] */ const String& authorities)
{
    if (authorities != NULL) {
        if (authorities.indexOf(';') == -1) {
            mAuthority = authorities;
            mAuthorities = NULL;
        } else {
            mAuthority = NULL;
            mAuthorities = authorities.split(";");
        }
    }
}

Boolean ContentProvider::MatchesOurAuthorities(
    /* [in] */ const String& authority)
{
    if (mAuthority != NULL) {
        return mAuthority.equals(authority);
    }
    if (mAuthorities != NULL) {
        Int32 length = mAuthorities.length;
        for (Int32 i = 0; i < length; i++) {
            if (mAuthorities[i].equals(authority)) return true;
        }
    }
    return FALSE;
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

ECode ContentProvider::SetAppOps(
    /* [in] */ Int32 readOp,
    /* [in] */ Int32 writeOp)
{
    if (!mNoPerms) {
        mTransport.mReadOp = readOp;
        mTransport.mWriteOp = writeOp;
    }
}

ECode ContentProvider::GetAppOpsManager(
    /* [out] */ IAppOpsManager** mgr)
{
    return mTransport.mAppOpsManager;
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

ECode ContentProvider::RejectQuery(
    /* [in] */ IUri* uri,
    /* [in] */ ArrayOf<String>* projection,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& sortOrder,
    /* [in] */ ICancellationSignal* cancellationSignal,
    /* [out] */ ICursor** cursor)
{
    // The read is not allowed...  to fake it out, we replace the given
    // selection statement with a dummy one that will always be FALSE.
    // This way we will get a cursor back that has the correct structure
    // but contains no rows.
    if (selection == NULL || selection.isEmpty()) {
        selection = "'A' = 'B'";
    } else {
        selection = "'A' = 'B' AND (" + selection + ")";
    }
    return Query(uri, projection, selection, selectionArgs, sortOrder, cancellationSignal);
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

ECode ContentProvider::Canonicalize(
    /* [in] */ IUri* uri,
    /* [out] */ IUri** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;
    return NOERROR;
}

ECode ContentProvider::Uncanonicalize(
    /* [in] */ IUri* uri,
    /* [out] */ IUri** result)
{
    VALIDATE_NOT_NULL(result)
    *result = uri;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode ContentProvider::RejectInsert(
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* values,
    /* [out] */ IUri** result)
{
    // If not allowed, we need to return some reasonable URI.  Maybe the
    // content provider should be responsible for this, but for now we
    // will just return the base URI with a dummy '0' tagged on to it.
    // You shouldn't be able to read if you can't write, anyway, so it
    // shouldn't matter much what is returned.
    return uri.buildUpon().appendPath("0").build();
    return NOERROR;
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

ECode ContentProvider::OpenFile(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [in] */  ICancellationSignal* signal,
    /* [out] */ IParcelFileDescriptor** fileDescriptor)
{
    return OpenFile(uri, mode, fileDescriptor);
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

ECode ContentProvider::OpenAssetFile(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [in] */  ICancellationSignal* signal,
    /* [out] */ IAssetFileDescriptor** fileDescriptor)
{
    return OpenAssetFile(uri, mode, fileDescriptor);
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

ECode ContentProvider::OpenTypedAssetFile(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mimeTypeFilter,
    /* [in] */ IBundle* opts,
    /* [in] */  ICancellationSignal* signal,
    /* [out] */ IAssetFileDescriptor** fileDescriptor)
{
    return OpenTypedAssetFile(uri, mimeTypeFilter, opts, fileDescriptor);
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

ECode ContentProvider::AttachInfoForTesting(
    /* [in] */ IContext* context,
    /* [in] */ IProviderInfo* info)
{
    return AttachInfo(context, info, TRUE);
}

ECode ContentProvider::AttachInfo(
    /* [in] */ IContext* context,
    /* [in] */ IProviderInfo* info)
{
    return AttachInfo(context, info, FALSE);
}

ECode ContentProvider::AttachInfo(
    /* [in] */ IContext* context,
    /* [in] */ IProviderInfo* info,
    /* [in] */ Boolean testing)
{
    /*
     * We may be using AsyncTask from binder threads.  Make it init here
     * so its static handler is on the main thread.
     */
    AsyncTask::Init();

    mNoPerms = testing;

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
    /* [in] */ IArrayList* operations,
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

ECode ContentProvider::ValidateIncomingUri(
    /* [in] */ IUri* uri)
{
    String auth = uri.getAuthority();
    Int32 userId = getUserIdFromAuthority(auth, UserHandle.USER_CURRENT);
    if (userId != UserHandle.USER_CURRENT && userId != mContext.getUserId()) {
        throw new SecurityException("trying to query a ContentProvider in user "
                + mContext.getUserId() + " with a uri belonging to user " + userId);
    }
    if (!matchesOurAuthorities(getAuthorityWithoutUserId(auth))) {
        String message = "The authority of the uri " + uri + " does not match the one of the "
                + "contentProvider: ";
        if (mAuthority != NULL) {
            message += mAuthority;
        } else {
            message += mAuthorities;
        }
        throw new SecurityException(message);
    }
}

Int32 ContentProvider::GetUserIdFromAuthority(
    /* [in] */ const String& auth,
    /* [in] */ Int32 defaultUserId)
{
    return getUserIdFromAuthority(auth, UserHandle.USER_CURRENT);
}

Int32 ContentProvider::GetUserIdFromUri(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 defaultUserId)
{
    if (uri == NULL) return defaultUserId;
    return getUserIdFromAuthority(uri.getAuthority(), defaultUserId);
}

Int32 ContentProvider::GetUserIdFromUri(
    /* [in] */ IUri* uri)
{
    return getUserIdFromUri(uri, UserHandle.USER_CURRENT);
}

String ContentProvider::GetAuthorityWithoutUserId(
    /* [in] */ const String& auth)
{
    if (auth == NULL) return NULL;
    Int32 end = auth.lastIndexOf('@');
    return auth.substring(end+1);
}

ECode ContentProvider::GetUriWithoutUserId(
    /* [in] */ IUri* uri,
    /* [out] */ IUri** result)
{
    if (uri == NULL) return NULL;
    Uri.Builder builder = uri.buildUpon();
    builder.authority(getAuthorityWithoutUserId(uri.getAuthority()));
    return builder.build();
}

Boolean ContentProvider::UriHasUserId(
    /* [in] */ IUri* uri)
{
    if (uri == NULL) return FALSE;
    return !TextUtils.isEmpty(uri.getUserInfo());
}

AutoPtr<IUri> ContentProvider::MaybeAddUserId(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 userId)
{
    if (uri == NULL) return NULL;
    if (userId != UserHandle.USER_CURRENT
            && ContentResolver.SCHEME_CONTENT.equals(uri.getScheme())) {
        if (!uriHasUserId(uri)) {
            //We don't add the user Id if there's already one
            Uri.Builder builder = uri.buildUpon();
            builder.encodedAuthority("" + userId + "@" + uri.getEncodedAuthority());
            return builder.build();
        }
    }
    return uri;
    return NOERROR;
}

} // namespace Content
} // namespace Droid
} // namespace Elastos
