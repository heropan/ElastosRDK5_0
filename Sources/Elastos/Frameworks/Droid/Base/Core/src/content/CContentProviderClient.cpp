
#include "content/CContentProviderClient.h"
#include "content/CContentProviderHelper.h"

using Elastos::Droid::Os::IICancellationSignal;

namespace Elastos {
namespace Droid {
namespace Content {

#define HANDLE_CATCH(ec)                                                    \
    do {                                                                    \
        if (E_DEAD_OBJECT_EXCEPTION == ec) {                                \
            if (!mStable) {                                                 \
                mContentResolver->UnstableProviderDied(mContentProvider);   \
            }                                                               \
            return ec;                                                      \
        }                                                                   \
        return ec;                                                          \
    } while(0);

CContentProviderClient::CContentProviderClient()
    : mContentProvider(NULL)
    , mContentResolver(NULL)
    , mStable(FALSE)
    , mReleased(FALSE)
{}

CContentProviderClient::~CContentProviderClient()
{}

ECode CContentProviderClient::Query(
    /* [in] */ IUri* uri,
    /* [in] */ ArrayOf<String>* projection,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& sortOrder,
    /* [out] */ ICursor** cursor)
{
    ECode ec = Query(uri, projection, selection, selectionArgs, sortOrder, NULL, cursor);
    HANDLE_CATCH(ec)
}

ECode CContentProviderClient::Query(
    /* [in] */ IUri* uri,
    /* [in] */ ArrayOf<String>* projection,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& sortOrder,
    /* [in] */ ICancellationSignal* cancellationSignal,
    /* [out] */ ICursor** cursor)
{
    AutoPtr<IICancellationSignal> remoteCancellationSignal;

    if (NULL != cancellationSignal) {
        FAIL_RETURN(mContentProvider->CreateCancellationSignal((IICancellationSignal**)&remoteCancellationSignal))
        FAIL_RETURN(cancellationSignal->SetRemote(remoteCancellationSignal))
    }

    ECode ec = mContentProvider->Query(uri, projection, selection, selectionArgs, sortOrder, remoteCancellationSignal, cursor);
    HANDLE_CATCH(ec)
}

ECode CContentProviderClient::GetType(
    /* [in] */ IUri* uri,
    /* [out] */ String* type)
{
    ECode ec = mContentProvider->GetType(uri, type);
    HANDLE_CATCH(ec)
}

ECode CContentProviderClient::GetStreamTypes(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mimeTypeFilter,
    /* [out, callee] */ ArrayOf<String> ** streamTypes)
{
    ECode ec = mContentProvider->GetStreamTypes(uri, mimeTypeFilter, streamTypes);
    HANDLE_CATCH(ec)
}

ECode CContentProviderClient::Insert(
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues * initialValues,
    /* [out] */ IUri** insertedUri)
{
    ECode ec = mContentProvider->Insert(uri, initialValues, insertedUri);
    HANDLE_CATCH(ec)
}

ECode CContentProviderClient::BulkInsert(
    /* [in] */ IUri* uri,
    /* [in] */ ArrayOf<IContentValues *>* initialValues,
    /* [out] */ Int32* number)
{
    ECode ec = mContentProvider->BulkInsert(uri, initialValues, number);
    HANDLE_CATCH(ec)
}

ECode CContentProviderClient::Delete(
    /* [in] */ IUri* uri,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [out] */ Int32* rowsAffected)
{
    ECode ec = mContentProvider->Delete(uri, selection, selectionArgs, rowsAffected);
    HANDLE_CATCH(ec)
}

ECode CContentProviderClient::Update(
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* values,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [out] */ Int32* rowsAffected)
{
    ECode ec = mContentProvider->Update(uri, values, selection, selectionArgs, rowsAffected);
    HANDLE_CATCH(ec)
}

ECode CContentProviderClient::OpenFile(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IParcelFileDescriptor** fileDescriptor)
{
    ECode ec = mContentProvider->OpenFile(uri, mode, fileDescriptor);
    HANDLE_CATCH(ec)
}

ECode CContentProviderClient::OpenAssetFile(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IAssetFileDescriptor** fileDescriptor)
{
    ECode ec = mContentProvider->OpenAssetFile(uri, mode, fileDescriptor);
    HANDLE_CATCH(ec)
}

ECode CContentProviderClient::OpenTypedAssetFileDescriptor(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mimeType,
    /* [in] */ IBundle* opts,
    /* [out] */ IAssetFileDescriptor** fileDescriptor)
{
    ECode ec = mContentProvider->OpenTypedAssetFile(uri, mimeType, opts, fileDescriptor);
    HANDLE_CATCH(ec)
}

ECode CContentProviderClient::ApplyBatch(
    /* [in] */ IObjectContainer* operations,
    /* [out, callee] */ ArrayOf<IContentProviderResult *>** providerResults)
{
    ECode ec = mContentProvider->ApplyBatch(operations, providerResults);
    HANDLE_CATCH(ec)
}

ECode CContentProviderClient::Call(
    /* [in] */ const String& method,
    /* [in] */ const String& arg,
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** bundle)
{
    ECode ec = mContentProvider->Call(method, arg, extras, bundle);
    HANDLE_CATCH(ec)
}

ECode CContentProviderClient::ReleaseProvider(
    /* [out] */ Boolean* isRelease)
{
    VALIDATE_NOT_NULL(isRelease)
    AutoLock lock(mCContentProviderClientLock);
    if (mReleased) return E_ILLEGAL_STATE_EXCEPTION;
    mReleased = TRUE;

    if (mStable) {
        return mContentResolver->ReleaseProvider(mContentProvider, isRelease);
    }
    else {
        return mContentResolver->ReleaseUnstableProvider(mContentProvider, isRelease);
    }
}

ECode CContentProviderClient::GetLocalContentProvider(
    /* [out] */ IContentProvider** localContentProvider)
{
    VALIDATE_NOT_NULL(localContentProvider)
    AutoPtr<IContentProviderHelper> providerHelper;
    FAIL_RETURN(CContentProviderHelper::AcquireSingleton((IContentProviderHelper**)&providerHelper))
    FAIL_RETURN(providerHelper->CoerceToLocalContentProvider(mContentProvider, localContentProvider))
    return NOERROR;
}

ECode CContentProviderClient::constructor(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ IIContentProvider* provider,
    /* [in] */ Boolean stable)
{
    VALIDATE_NOT_NULL(resolver)
    VALIDATE_NOT_NULL(provider)
    mContentProvider = provider;
    mContentResolver = resolver;
    mStable = stable;
    return NOERROR;
}

}
}
}

