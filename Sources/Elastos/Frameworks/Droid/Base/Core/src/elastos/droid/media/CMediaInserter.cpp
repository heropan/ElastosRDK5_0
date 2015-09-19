
#include "media/CMediaInserter.h"
#include "content/CContentValues.h"

using Elastos::Droid::Content::CContentValues;

namespace Elastos {
namespace Droid {
namespace Media {

CMediaInserter::CMediaInserter()
    : mBufferSizePerUri(0)
{
}

CMediaInserter::~CMediaInserter()
{
    mRowMap.Clear();
    mPriorityRowMap.Clear();
}

ECode CMediaInserter::Init(
    /* [in] */ IIContentProvider* provider,
    /* [in] */ Int32 bufferSizePerUri)
{
    mProvider = provider;
    mBufferSizePerUri = bufferSizePerUri;
    return NOERROR;
}

ECode CMediaInserter::constructor(
    /* [in] */ IIContentProvider* provider,
    /* [in] */ Int32 bufferSizePerUri)
{
    return Init(provider, bufferSizePerUri);
}

ECode CMediaInserter::Insert(
    /* [in] */ IUri* tableUri,
    /* [in] */ IContentValues* values)
{
    return Insert(tableUri, values, FALSE);
}

ECode CMediaInserter::InsertwithPriority(
    /* [in] */ IUri* tableUri,
    /* [in] */ IContentValues* values)
{
    return Insert(tableUri, values, TRUE);
}

ECode CMediaInserter::Insert(
    /* [in] */ IUri* tableUri,
    /* [in] */ IContentValues* values,
    /* [in] */ Boolean priority)
{
    VALIDATE_NOT_NULL(tableUri);

    UriContentValuesMap& rowmap = priority ? mPriorityRowMap : mRowMap;
    AutoPtr<IUri> key = tableUri;
    UriContentValuesMapIterator it = rowmap.Find(key);
    AutoPtr<ContentValuesList> list;
    if (it == rowmap.End()) {
        list = new List< AutoPtr<IContentValues> >();
        rowmap[key] = list;
    }

    AutoPtr<IContentValues> contentValues;
    CContentValues::New(values, (IContentValues**)&contentValues);
    list->PushBack(contentValues);
    if (list->GetSize() >= mBufferSizePerUri) {
        FAIL_RETURN(FlushAllPriority());
        return Flush(tableUri, list);
    }
    return NOERROR;
}

ECode CMediaInserter::FlushAll()
{
    FAIL_RETURN(FlushAllPriority());

    AutoPtr<IUri> tableUri;
    UriContentValuesMapIterator it = mRowMap.Begin();
    AutoPtr<ContentValuesList> list;
    for (; it != mRowMap.End(); ++it){
        tableUri = it->mFirst;
        list = it->mSecond;
        Flush(tableUri, list);
    }
    mRowMap.Clear();

    return NOERROR;
}

ECode CMediaInserter::FlushAllPriority() // throws RemoteException
{
    AutoPtr<IUri> tableUri;
    UriContentValuesMapIterator it = mPriorityRowMap.Begin();
    AutoPtr<ContentValuesList> list;
    for (; it != mPriorityRowMap.End(); ++it){
        tableUri = it->mFirst;
        list = it->mSecond;
        FAIL_RETURN(Flush(tableUri, list));
    }
    mPriorityRowMap.Clear();
    return NOERROR;
}

ECode CMediaInserter::Flush(
    /* [in] */ IUri* tableUri,
    /* [in] */ List< AutoPtr<IContentValues> >* list)
{
    if (list != NULL && !list->IsEmpty()) {
        AutoPtr<ArrayOf<IContentValues* > > valuesArray = ArrayOf<IContentValues*>::Alloc(list->GetSize());
        ContentValuesListIterator it = list->Begin();
        for (UInt32 i = 0; it != list->End(); ++i, ++it) {
            valuesArray->Set(i, (*it));
        }
        Int32 result;
        FAIL_RETURN(mProvider->BulkInsert(tableUri, valuesArray.Get(), &result));
        list->Clear();
    }
    return NOERROR;
}


} // namespace Media
} // namepsace Droid
} // namespace Elastos
