
#include "content/CContentProviderOperation.h"
#include "content/CContentProviderResult.h"
#include "content/CContentUris.h"
#include "content/CContentValues.h"
#include "net/CStringUri.h"
#include "net/COpaqueUri.h"
#include "net/CHierarchicalUri.h"
#include "text/TextUtils.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::CStringWrapper;
using Elastos::Core::CInteger64;
using Elastos::Droid::Database::ICursor;
using Elastos::Droid::Net::CStringUri;
using Elastos::Droid::Net::COpaqueUri;
using Elastos::Droid::Net::CHierarchicalUri;
using Elastos::Droid::Text::TextUtils;
using Elastos::Utility::IObjectStringMap;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Content {

#define FAIL_WithGoto(ecode)       \
        do {                       \
            ECode ec = ecode;      \
            if (FAILED(ec)) {      \
                goto EXIT;         \
            }                      \
        } while (0);

const String CContentProviderOperation::TAG("CContentProviderOperation");

CContentProviderOperation::CContentProviderOperation()
    : mType(0)
    , mValues(NULL)
    , mExpectedCount(0)
    , mYieldAllowed(FALSE)
{}

CContentProviderOperation::~CContentProviderOperation()
{
}

ECode CContentProviderOperation::GetUri(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri)
    *uri = mUri;
    REFCOUNT_ADD(*uri)
    return NOERROR;
}

ECode CContentProviderOperation::IsYieldAllowed(
    /* [out] */ Boolean* isAllowed)
{
    VALIDATE_NOT_NULL(isAllowed)
    *isAllowed = mYieldAllowed;
    return NOERROR;
}

ECode CContentProviderOperation::GetType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type)
    *type = mType;
    return NOERROR;
}

ECode CContentProviderOperation::IsWriteOperation(
    /* [out] */ Boolean* isWrite)
{
    VALIDATE_NOT_NULL(isWrite)
    *isWrite = mType == IContentProviderOperation::TYPE_DELETE || mType == IContentProviderOperation::TYPE_INSERT
            || mType == IContentProviderOperation::TYPE_UPDATE;
    return NOERROR;
}

ECode CContentProviderOperation::IsReadOperation(
    /* [out] */ Boolean* isRead)
{
    VALIDATE_NOT_NULL(isRead)
    *isRead = mType == IContentProviderOperation::TYPE_ASSERT;
    return NOERROR;
}

ECode CContentProviderOperation::Apply(
    /* [in] */ IContentProvider* provider,
    /* [in] */ ArrayOf<IContentProviderResult*>* backRefs,
    /* [in] */ Int32 numBackRefs,
    /* [out] */ IContentProviderResult** providerResult)
{
    VALIDATE_NOT_NULL(provider)
    VALIDATE_NOT_NULL(providerResult)
    AutoPtr<IContentValues> values;
    FAIL_RETURN(ResolveValueBackReferences(backRefs, numBackRefs, (IContentValues**)&values))
    AutoPtr<ArrayOf<String> > selectionArgs;
    FAIL_RETURN(ResolveSelectionArgsBackReferences(backRefs, numBackRefs, (ArrayOf<String>**)&selectionArgs))

    if (mType == IContentProviderOperation::TYPE_INSERT) {
        AutoPtr<IUri> newUri;
        FAIL_RETURN(provider->Insert(mUri, values, (IUri**)&newUri))
        if (NULL == newUri) {
            //throw new OperationApplicationException("insert failed");
            return E_OPERATION_APPLICATION_EXCEPTION;
        }
        FAIL_RETURN(CContentProviderResult::New(newUri, providerResult))
        return NOERROR;
    }

    Int32 numRows = 0;

    if (mType == IContentProviderOperation::TYPE_DELETE) {
        FAIL_RETURN(provider->Delete(mUri, mSelection, selectionArgs, &numRows))
    }
    else if (mType == IContentProviderOperation::TYPE_UPDATE) {
        FAIL_RETURN(provider->Update(mUri, values, mSelection, selectionArgs, &numRows))
    }
    else if (mType == IContentProviderOperation::TYPE_ASSERT) {
        // Assert that all rows match expected values
        AutoPtr<ArrayOf<String> > projection;
        if (NULL != values) {
            // Build projection map from expected values
            AutoPtr<IObjectStringMap> objStringMap;
            AutoPtr<IObjectContainer> objContainer;
            FAIL_RETURN(values->ValueSet((IObjectStringMap**)&objStringMap))

            if (NULL != objStringMap) {
                FAIL_RETURN(objStringMap->GetAllItems((ArrayOf<String>**)&projection, (IObjectContainer**)&objContainer))
            }

        }

        AutoPtr<ICursor> cursor;
        String sortOrder;
        ECode ecode = NOERROR;
        FAIL_RETURN(provider->Query(mUri, projection, mSelection, selectionArgs, sortOrder, (ICursor**)&cursor))
        ecode = cursor->GetCount(&numRows);
        FAIL_WithGoto(ecode)
        if (NULL != projection) {
            Boolean hasNext = FALSE;
            while ((cursor->MoveToNext(&hasNext), hasNext)) {
                for (Int32 i = 0; i < projection->GetLength(); i++) {
                    String cursorValue;
                    ecode = cursor->GetString(i, &cursorValue);
                    FAIL_WithGoto(ecode)
                    String expectedValue;
                    ecode = values->GetAsString((*projection)[i], &expectedValue);
                    FAIL_WithGoto(ecode)
                    AutoPtr<ICharSequence> charSeq;
                    AutoPtr<ICharSequence> charSeq2;
                    ecode = CStringWrapper::New(cursorValue, (ICharSequence**)&charSeq);
                    FAIL_WithGoto(ecode)
                    ecode = CStringWrapper::New(expectedValue, (ICharSequence**)&charSeq2);
                    FAIL_WithGoto(ecode)

                    if (!TextUtils::Equals(charSeq, charSeq2)) {
                        // Throw exception when expected values don't match
                        String str;
                        ecode = ToString(&str);
                        FAIL_WithGoto(ecode)
                        Logger::E(TAG, str);
                        //throw new OperationApplicationException("Found value " + cursorValue
                        //        + " when expected " + expectedValue + " for column "
                        //        + projection[i]);
                        return E_OPERATION_APPLICATION_EXCEPTION;
                    }
                }
            }
        }

EXIT:
        FAIL_RETURN(cursor->Close())
        FAIL_RETURN(ecode)
    }
    else {
        String str;
        FAIL_RETURN(ToString(&str))
        Logger::E(TAG, str);
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (mExpectedCount != 0 && mExpectedCount != numRows) {
        String str;
        FAIL_RETURN(ToString(&str))
        Logger::E(TAG, str);
        // throw new OperationApplicationException("wrong number of rows: " + numRows);
        return E_OPERATION_APPLICATION_EXCEPTION;
    }

    FAIL_RETURN(CContentProviderResult::New(numRows, providerResult))
    return NOERROR;
}

ECode CContentProviderOperation::ResolveValueBackReferences(
    /* [in] */ ArrayOf<IContentProviderResult*>* backRefs,
    /* [in] */ Int32 numBackRefs,
    /* [out] */ IContentValues** contentValues)
{
    VALIDATE_NOT_NULL(backRefs)
    VALIDATE_NOT_NULL(contentValues)

    if (NULL == mValuesBackReferences) {
        *contentValues = mValues;
        REFCOUNT_ADD(*contentValues);
        return NOERROR;
    }

    if (NULL == mValues) {
        FAIL_RETURN(CContentValues::New(contentValues))
    } else {
        FAIL_RETURN(CContentValues::New(mValues, contentValues))
    }

    AutoPtr<IObjectStringMap> objStringMap;
    AutoPtr<IObjectContainer> objContainer;
    FAIL_RETURN(mValuesBackReferences->ValueSet((IObjectStringMap**)&objStringMap))

    if (NULL != objStringMap) {
        AutoPtr<ArrayOf<String> > keyArray;
        FAIL_RETURN(objStringMap->GetAllItems((ArrayOf<String>**)&keyArray, (IObjectContainer**)&objContainer))
        Int32 len = keyArray->GetLength();
        for (Int32 i = 0; i < len; i++) {
            String key = (*keyArray)[i];
            Int32 backRefIndex;
            FAIL_RETURN(mValuesBackReferences->GetAsInt32(key, &backRefIndex))
            if (0 == backRefIndex) {
                String str;
                FAIL_RETURN(ToString(&str))
                Logger::E(TAG, str);
                // throw new IllegalArgumentException("values backref " + key + " is not an integer");
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
            Int64 backRefValue;
            FAIL_RETURN(BackRefToValue(backRefs, numBackRefs, backRefIndex, &backRefValue))
            AutoPtr<IInteger64> backRefValueObj;
            FAIL_RETURN(CInteger64::New(backRefValue, (IInteger64**)&backRefValueObj))
            FAIL_RETURN((*contentValues)->PutInt64(key, backRefValueObj))
        }
    }

    return NOERROR;
}

ECode CContentProviderOperation::ResolveSelectionArgsBackReferences(
    /* [in] */ ArrayOf<IContentProviderResult*>* backRefs,
    /* [in] */ Int32 numBackRefs,
    /* [out, callee] */ ArrayOf<String>** stringArray)
{
    VALIDATE_NOT_NULL(backRefs)
    VALIDATE_NOT_NULL(stringArray)

    if (NULL == mSelectionArgsBackReferences) {
        *stringArray = mSelectionArgs;
        REFCOUNT_ADD(*stringArray);
        return NOERROR;
    }

    AutoPtr<ArrayOf<String> > newArgs = ArrayOf<String>::Alloc(mSelectionArgs->GetLength());
    newArgs->Copy(mSelectionArgs);
    HashMap<Int32, Int32>::Iterator iter = mSelectionArgsBackReferences->Begin();

    for (; iter != mSelectionArgsBackReferences->End(); ++iter) {
        const Int32 selectionArgIndex = iter->mFirst;
        const Int32 backRefIndex = iter->mSecond;
        Int64 backRefValue = 0;
        FAIL_RETURN(BackRefToValue(backRefs, numBackRefs, backRefIndex, &backRefValue))
        (*newArgs)[selectionArgIndex] = StringUtils::Int64ToString(backRefValue);
    }

    *stringArray = newArgs;
    REFCOUNT_ADD(*stringArray);
    return NOERROR;
}

ECode CContentProviderOperation::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    String str1("mType: ");
    String strType;
    String strUri("null");
    String strExpectedCount;
    String strYieldAllowed;
    String strValues("null");
    String strValuesBackReferences("null");
    String strSelectionArgsBackReferences("null");
    strType = StringUtils::Int32ToString(mType);

    if (NULL != mUri) {
        FAIL_RETURN(mUri->ToString(&strUri));
    }

    strExpectedCount = StringUtils::Int32ToString(mExpectedCount);
    strYieldAllowed = mYieldAllowed ? String("true") : String("false");

    if (NULL != mValues) {
        FAIL_RETURN(mValues->ToString(&strValues));
    }

    if (NULL != mValuesBackReferences) {
        FAIL_RETURN(mValuesBackReferences->ToString(&strValuesBackReferences));
    }

    if (NULL != mSelectionArgsBackReferences) {
        if (mSelectionArgsBackReferences->IsEmpty()) {
            strSelectionArgsBackReferences = String("{}");
        }
        else {
            String tmpStr("");
            tmpStr.Append("{");
            String key;
            String value;
            HashMap<Int32, Int32>::Iterator iter = mSelectionArgsBackReferences->Begin();
            for (; iter != mSelectionArgsBackReferences->End(); ++iter) {
                if (iter != mSelectionArgsBackReferences->Begin())
                    tmpStr.Append(", ");

                key = StringUtils::Int32ToString(iter->mFirst);
                value = StringUtils::Int32ToString(iter->mSecond);
                tmpStr.Append(key);
                tmpStr.Append("=");
                tmpStr.Append(value);
            }

            strSelectionArgsBackReferences = tmpStr;
            strSelectionArgsBackReferences.Append("}");
        }
    }

    str1.Append(strType);
    str1.Append(", mUri: ");
    str1.Append(strUri);
    str1.Append(", mSelection: ");

    if (mSelection.IsNull()) {
        str1.Append("null");
    }
    else {
        str1.Append(mSelection);
    }

    str1.Append(", mExpectedCount: ");
    str1.Append(strExpectedCount);
    str1.Append(", mYieldAllowed: ");
    str1.Append(strYieldAllowed);
    str1.Append(", mValues: ");
    str1.Append(strValues);
    str1.Append(", mValuesBackReferences: ");
    str1.Append(strValuesBackReferences);
    str1.Append(", mSelectionArgsBackReferences: ");
    str1.Append(strSelectionArgsBackReferences);
    *result = str1;
    return NOERROR;
}

ECode CContentProviderOperation::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source)
    source->ReadInt32(&mType);

    Int32 value = 0;
    if (source->ReadInt32(&value), value != 0) {
        Int32 pos;
        source->GetDataPosition(&pos);
        source->SetDataPosition(pos - sizeof(Int32));
        AutoPtr<IUri> uri;
        if (value == 1) {
            CStringUri::New((IUri**)&uri);
            IParcelable::Probe(uri)->ReadFromParcel(source);
        }
        else if (value == 2) {
            COpaqueUri::New((IUri**)&uri);
            IParcelable::Probe(uri)->ReadFromParcel(source);
        }
        else if (value == 3) {
            CHierarchicalUri::New((IUri**)&uri);
            IParcelable::Probe(uri)->ReadFromParcel(source);
        }
        mUri = uri;
    }

    if (source->ReadInt32(&value), value != 0) {
        AutoPtr<IContentValues> values;
        CContentValues::New((IContentValues**)&values);
        IParcelable::Probe(values)->ReadFromParcel(source);
        mValues = values;
    }

    if (source->ReadInt32(&value), value != 0) {
        source->ReadString(&mSelection);
    }

    if (source->ReadInt32(&value), value != 0) {
        source->ReadArrayOfString((ArrayOf<String>**)&mSelectionArgs);
    }

    if (source->ReadInt32(&value), value != 0) {
        source->ReadInt32(&mExpectedCount);
    }

    if (source->ReadInt32(&value), value != 0) {
        AutoPtr<IContentValues> values;
        CContentValues::New((IContentValues**)&values);
        IParcelable::Probe(values)->ReadFromParcel(source);
        mValuesBackReferences = values;
    }

    if (source->ReadInt32(&value), value != 0) {
        mSelectionArgsBackReferences = new HashMap<Int32, Int32>();
        Int32 count;
        source->ReadInt32(&count);
        Int32 key = 0;
        Int32 value = 0;
        for (Int32 i = 0; i < count; i++) {
            source->ReadInt32(&key);
            source->ReadInt32(&value);
            (*mSelectionArgsBackReferences)[key] = value;
        }
    }

    mYieldAllowed = (source->ReadInt32(&value), value) != 0;
    return NOERROR;
}

ECode CContentProviderOperation::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest)
    dest->WriteInt32(mType);

    if (NULL != mUri) {
        AutoPtr<IParcelable> parcelable = (IParcelable*) mUri->Probe(EIID_IParcelable);
        FAIL_RETURN(parcelable->WriteToParcel(dest))
    }
    else {
        dest->WriteInt32(0);
    }

    if (NULL != mValues) {
        dest->WriteInt32(1);
        AutoPtr<IParcelable> valueParcelable = (IParcelable*) mValues->Probe(EIID_IParcelable);
        FAIL_RETURN(valueParcelable->WriteToParcel(dest))
    }
    else {
        dest->WriteInt32(0);
    }

    if (!mSelection.IsNull()) {
        dest->WriteInt32(1);
        dest->WriteString(mSelection);
    }
    else {
        dest->WriteInt32(0);
    }

    if (NULL != mSelectionArgs) {
        dest->WriteInt32(1);
        dest->WriteArrayOfString(mSelectionArgs);
    }
    else {
        dest->WriteInt32(0);
    }

    if (mExpectedCount != 0) {
        dest->WriteInt32(1);
        dest->WriteInt32(mExpectedCount);
    }
    else {
        dest->WriteInt32(0);
    }

    if (NULL != mValuesBackReferences) {
        dest->WriteInt32(1);
        AutoPtr<IParcelable> valueRefParcelable = (IParcelable*) mValuesBackReferences->Probe(EIID_IParcelable);
        FAIL_RETURN(valueRefParcelable->WriteToParcel(dest))
    }
    else {
        dest->WriteInt32(0);
    }

    if (NULL != mSelectionArgsBackReferences) {
        dest->WriteInt32(1);
        dest->WriteInt32(mSelectionArgsBackReferences->GetSize());
        HashMap<Int32, Int32>::Iterator iter = mSelectionArgsBackReferences->Begin();
        for (; iter != mSelectionArgsBackReferences->End(); ++iter) {
            dest->WriteInt32(iter->mFirst);
            dest->WriteInt32(iter->mSecond);
        }
    }
    else {
        dest->WriteInt32(0);
    }

    dest->WriteInt32(mYieldAllowed ? 1 : 0);
    return NOERROR;
}

ECode CContentProviderOperation::constructor()
{
    return NOERROR;
}

ECode CContentProviderOperation::constructor(
    /* [in] */ IContentProviderOperationBuilder* operationBuilder)
{
    VALIDATE_NOT_NULL(operationBuilder)
    Boolean hasNext = FALSE;
    Int32 key = 0;
    Int32 value = 0;
    AutoPtr<IObjectContainer> selectionArgsBackRef;
    AutoPtr<IObjectEnumerator> ObjEnumerator;
    AutoPtr<IIntegerMapWrapper> wrapper;
    FAIL_RETURN(operationBuilder->GetType(&mType))
    FAIL_RETURN(operationBuilder->GetUri((IUri**)&mUri))
    FAIL_RETURN(operationBuilder->GetValues((IContentValues**)&mValues))
    FAIL_RETURN(operationBuilder->GetSelection(&mSelection))
    FAIL_RETURN(operationBuilder->GetSelectionArgs((ArrayOf<String>**)&mSelectionArgs))
    FAIL_RETURN(operationBuilder->GetExpectedCount(&mExpectedCount))
    FAIL_RETURN(operationBuilder->GetSelectionArgsBackReferences((IObjectContainer**)&selectionArgsBackRef))

    if (NULL != selectionArgsBackRef) {
        FAIL_RETURN(selectionArgsBackRef->GetObjectEnumerator((IObjectEnumerator**)&ObjEnumerator))
        mSelectionArgsBackReferences = new HashMap<Int32, Int32>();

        while ((ObjEnumerator->MoveNext(&hasNext), hasNext)) {
            wrapper = NULL;
            FAIL_RETURN(ObjEnumerator->Current((IInterface**)&wrapper))
            FAIL_RETURN(wrapper->GetKey(&key))
            FAIL_RETURN(wrapper->GetValue(&value))
            (*mSelectionArgsBackReferences)[key] = value;
        }

    }

    FAIL_RETURN(operationBuilder->GetValuesBackReferences((IContentValues**)&mValuesBackReferences))
    FAIL_RETURN(operationBuilder->GetYieldAllowed(&mYieldAllowed))
    return NOERROR;
}

ECode CContentProviderOperation::BackRefToValue(
    /* [in] */ ArrayOf<IContentProviderResult*>* backRefs,
    /* [in] */ const Int32 numBackRefs,
    /* [in] */ const Int32 backRefIndex,
    /* [out] */ Int64* backRefValue)
{
    VALIDATE_NOT_NULL(backRefs)
    VALIDATE_NOT_NULL(backRefValue)

    if (backRefIndex >= numBackRefs) {
        String str;
        FAIL_RETURN(ToString(&str))
        Logger::E(TAG, str);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    AutoPtr<IContentProviderResult> backRef = (*backRefs)[backRefIndex];
    Int64 tmpBackRefValue = 0;
    AutoPtr<IUri> uri;
    FAIL_RETURN(backRef->GetUri((IUri**)&uri));

    if (NULL != uri) {
        AutoPtr<IContentUris> contentUris;
        FAIL_RETURN(CContentUris::AcquireSingleton((IContentUris**)&contentUris))
        FAIL_RETURN(contentUris->ParseId(uri, &tmpBackRefValue))
    } else {
        FAIL_RETURN(backRef->GetCount((Int32*)&tmpBackRefValue))
    }

    *backRefValue = tmpBackRefValue;
    return NOERROR;
}

}
}
}

