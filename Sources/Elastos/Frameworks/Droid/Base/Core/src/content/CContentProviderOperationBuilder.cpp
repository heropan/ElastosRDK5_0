
#include "content/CContentProviderOperationBuilder.h"
//#include "content/CIntegerMapWrapper.h"
#include "content/CContentProviderOperation.h"
//#include "content/CContentValues.h"

using Elastos::Core::IInteger16;
using Elastos::Core::IInteger32;
using Elastos::Core::IInteger64;
using Elastos::Core::CInteger32;
using Elastos::Core::ICharSequence;
using Elastos::Core::IByte;
using Elastos::Core::IFloat;
using Elastos::Core::IDouble;
using Elastos::Core::IBoolean;
using Elastos::Core::IArrayOf;

namespace Elastos {
namespace Droid {
namespace Content {

CAR_INTERFACE_IMPL(CContentProviderOperationBuilder, Object, IContentProviderOperationBuilder)

CAR_OBJECT_IMPL(CContentProviderOperationBuilder)

CContentProviderOperationBuilder::CContentProviderOperationBuilder()
    : mType(0)
    , mExpectedCount(0)
    , mYieldAllowed(FALSE)
{}

CContentProviderOperationBuilder::~CContentProviderOperationBuilder()
{
}

ECode CContentProviderOperationBuilder::constructor(
    /* [in] */ Int32 type,
    /* [in] */ IUri* uri)
{
    VALIDATE_NOT_NULL(uri)
    mType = type;
    mUri = uri;
    return NOERROR;
}

ECode CContentProviderOperationBuilder::Build(
    /* [out] */ IContentProviderOperation** providerOperation)
{
    VALIDATE_NOT_NULL(providerOperation)
    // Int32 size = 0;

    // if (mType == IContentProviderOperation::TYPE_UPDATE) {
    //     if ((NULL == mValues || (mValues->GetSize(&size), size) == 0)
    //             && (NULL == mValuesBackReferences || (mValuesBackReferences->GetSize(&size), size) == 0)) {
    //         return E_ILLEGAL_ARGUMENT_EXCEPTION;
    //     }
    // }

    // if (mType == IContentProviderOperation::TYPE_ASSERT) {
    //     if ((NULL == mValues || (mValues->GetSize(&size), size) == 0)
    //             && (NULL == mValuesBackReferences || (mValuesBackReferences->GetSize(&size), size) == 0) && (mExpectedCount == 0)) {
    //         return E_ILLEGAL_ARGUMENT_EXCEPTION;
    //     }
    // }

    // FAIL_RETURN(CContentProviderOperation::New((IContentProviderOperationBuilder*) this, providerOperation))
    return NOERROR;
}

ECode CContentProviderOperationBuilder::WithValueBackReferences(
    /* [in] */ IContentValues* backReferences)
{
    VALIDATE_NOT_NULL(backReferences)

    // if (mType != IContentProviderOperation::TYPE_INSERT && mType != IContentProviderOperation::TYPE_UPDATE
    //         && mType != IContentProviderOperation::TYPE_ASSERT) {
    //     return E_ILLEGAL_ARGUMENT_EXCEPTION;
    // }

    // mValuesBackReferences = backReferences;
    return NOERROR;
}

ECode CContentProviderOperationBuilder::WithValueBackReference(
    /* [in] */ const String& key,
    /* [in] */ Int32 previousResult)
{
    // if (mType != IContentProviderOperation::TYPE_INSERT && mType != IContentProviderOperation::TYPE_UPDATE
    //         && mType != IContentProviderOperation::TYPE_ASSERT) {
    //     return E_ILLEGAL_ARGUMENT_EXCEPTION;
    // }

    // if (NULL == mValuesBackReferences) {
    //     FAIL_RETURN(CContentValues::New((IContentValues**)&mValuesBackReferences));
    // }

    // AutoPtr<IInteger32> previousResultObj;
    // FAIL_RETURN(CInteger32::New(previousResult, (IInteger32**)&previousResultObj))
    // FAIL_RETURN(mValuesBackReferences->PutInt32(key, previousResultObj))
    return NOERROR;
}

ECode CContentProviderOperationBuilder::WithSelectionBackReference(
    /* [in] */ Int32 selectionArgIndex,
    /* [in] */ Int32 previousResult)
{
    // if (mType != IContentProviderOperation::TYPE_UPDATE && mType != IContentProviderOperation::TYPE_DELETE
    //         && mType != IContentProviderOperation::TYPE_ASSERT) {
    //     return E_ILLEGAL_ARGUMENT_EXCEPTION;
    // }

    // if (NULL == mSelectionArgsBackReferences) {
    //     mSelectionArgsBackReferences = new HashMap<Int32, Int32>();
    // }

    // (*mSelectionArgsBackReferences)[selectionArgIndex] = previousResult;
    return NOERROR;
}

ECode CContentProviderOperationBuilder::WithValues(
    /* [in] */ IContentValues* values)
{
    // if (mType != IContentProviderOperation::TYPE_INSERT && mType != IContentProviderOperation::TYPE_UPDATE
    //         && mType != IContentProviderOperation::TYPE_ASSERT) {
    //     return E_ILLEGAL_ARGUMENT_EXCEPTION;
    // }

    // if (NULL == mValues) {
    //     FAIL_RETURN(CContentValues::New((IContentValues**)&mValues));
    // }

    // FAIL_RETURN(mValues->PutAll(values))
    return NOERROR;
}

ECode CContentProviderOperationBuilder::WithValue(
    /* [in] */ const String& key,
    /* [in] */ IInterface* value)
{
    // if (mType != IContentProviderOperation::TYPE_INSERT && mType != IContentProviderOperation::TYPE_UPDATE
    //         && mType != IContentProviderOperation::TYPE_ASSERT) {
    //     return E_ILLEGAL_ARGUMENT_EXCEPTION;
    // }

    // if (NULL == mValues) {
    //     FAIL_RETURN(CContentValues::New((IContentValues**)&mValues));
    // }

    // if (NULL == value) {
    //     FAIL_RETURN(mValues->PutNull(key))
    // }
    // else if (ICharSequence::Probe(value) != NULL) {
    //     FAIL_RETURN(mValues->PutString(key, (ICharSequence*)value))
    // }
    // else if (IByte::Probe(value) != NULL) {
    //     FAIL_RETURN(mValues->PutByte(key, (IByte*)value))
    // }
    // else if (IInteger16::Probe(value) != NULL) {
    //     FAIL_RETURN(mValues->PutInt16(key, (IInteger16*)value))
    // }
    // else if (IInteger32::Probe(value) != NULL) {
    //     FAIL_RETURN(mValues->PutInt32(key, (IInteger32*)value))
    // }
    // else if (IInteger64::Probe(value) != NULL) {
    //     FAIL_RETURN(mValues->PutInt64(key, (IInteger64*)value))
    // }
    // else if (IFloat::Probe(value) != NULL) {
    //     FAIL_RETURN(mValues->PutFloat(key, (IFloat*)value))
    // }
    // else if (IDouble::Probe(value) != NULL) {
    //     FAIL_RETURN(mValues->PutDouble(key, (IDouble*)value))
    // }
    // else if (IBoolean::Probe(value) != NULL) {
    //     FAIL_RETURN(mValues->PutBoolean(key, (IBoolean*)value))
    // }
    // else if (IArrayOf::Probe(value) != NULL) {
    //     FAIL_RETURN(mValues->PutBytes(key, (IArrayOf*)value))
    // }
    // else {
    //     return E_ILLEGAL_ARGUMENT_EXCEPTION;
    // }

    return NOERROR;
}

ECode CContentProviderOperationBuilder::WithSelection(
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs)
{
    // if (mType != IContentProviderOperation::TYPE_UPDATE && mType != IContentProviderOperation::TYPE_DELETE
    //         && mType != IContentProviderOperation::TYPE_ASSERT) {
    //     return E_ILLEGAL_ARGUMENT_EXCEPTION;
    // }

    // mSelection = selection;
    // if (NULL == selectionArgs) {
    //     mSelectionArgs = NULL;
    // }
    // else {
    //     mSelectionArgs = ArrayOf<String>::Alloc(selectionArgs->GetLength());
    //     //System.arraycopy(selectionArgs, 0, mSelectionArgs, 0, selectionArgs.length);
    //     mSelectionArgs->Copy(selectionArgs);
    // }

    return NOERROR;
}

ECode CContentProviderOperationBuilder::WithExpectedCount(
    /* [in] */ Int32 count)
{
    // if (mType != IContentProviderOperation::TYPE_UPDATE && mType != IContentProviderOperation::TYPE_DELETE
    //         && mType != IContentProviderOperation::TYPE_ASSERT) {
    //     return E_ILLEGAL_ARGUMENT_EXCEPTION;
    // }

    // mExpectedCount = count;
    return NOERROR;
}

ECode CContentProviderOperationBuilder::WithYieldAllowed(
    /* [in] */ Boolean yieldAllowed)
{
    mYieldAllowed = yieldAllowed;
    return NOERROR;
}

ECode CContentProviderOperationBuilder::GetType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mType;
    return NOERROR;
}

ECode CContentProviderOperationBuilder::GetUri(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri)
    *uri = mUri;
    REFCOUNT_ADD(*uri)
    return NOERROR;
}

ECode CContentProviderOperationBuilder::GetSelection(
    /* [out] */ String* selection)
{
    VALIDATE_NOT_NULL(selection)
    *selection = mSelection;
    return NOERROR;
}

ECode CContentProviderOperationBuilder::GetSelectionArgs(
    /* [out, callee] */ ArrayOf<String>** selectionArgs)
{
    VALIDATE_NOT_NULL(selectionArgs)
    *selectionArgs = mSelectionArgs;
    REFCOUNT_ADD(*selectionArgs)
    return NOERROR;
}

ECode CContentProviderOperationBuilder::GetValues(
    /* [out] */ IContentValues** values)
{
    VALIDATE_NOT_NULL(values)
    *values = mValues;
    REFCOUNT_ADD(*values)
    return NOERROR;
}

ECode CContentProviderOperationBuilder::GetExpectedCount(
    /* [out] */ Int32* expectedCount)
{
    VALIDATE_NOT_NULL(expectedCount)
    *expectedCount = mExpectedCount;
    return NOERROR;
}

ECode CContentProviderOperationBuilder::GetValuesBackReferences(
    /* [out] */ IContentValues** valuesBackReferences)
{
    VALIDATE_NOT_NULL(valuesBackReferences)
    *valuesBackReferences = mValuesBackReferences;
    REFCOUNT_ADD(*valuesBackReferences)
    return NOERROR;
}

ECode CContentProviderOperationBuilder::GetSelectionArgsBackReferences(
    /* [out] */ IHashMap** selectionArgsBackRef)
{
    VALIDATE_NOT_NULL(selectionArgsBackRef)
    *selectionArgsBackRef = NULL;

    // if (NULL != mSelectionArgsBackReferences) {
    //     CObjectContainer::New(selectionArgsBackRef);
    //     HashMap<Int32, Int32>::Iterator iter = mSelectionArgsBackReferences->Begin();
    //     for (; iter != mSelectionArgsBackReferences->End(); ++iter) {
    //         AutoPtr<IIntegerMapWrapper> wrapper;
    //         CIntegerMapWrapper::New(iter->mFirst, iter->mSecond, (IIntegerMapWrapper**)&wrapper);
    //         (*selectionArgsBackRef)->Add(wrapper);
    //     }
    // }

    return NOERROR;
}

ECode CContentProviderOperationBuilder::GetYieldAllowed(
    /* [out] */ Boolean* yieldAllowed)
{
    VALIDATE_NOT_NULL(yieldAllowed)
    *yieldAllowed = mYieldAllowed;
    return NOERROR;
}

}
}
}

