
#include "CRawHeaders.h"
#include "elastos/StringUtils.h"
#include "elastos/StringBuilder.h"
#include "CStringWrapper.h"

using Elastos::Core::StringUtils;
using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Core::StringBuilder;
using Elastos::Core::EIID_IComparator;
using Elastos::Utility::ICollection;
using Elastos::Utility::EIID_ICollection;

namespace Elastos {
namespace Net {
namespace Http {

const AutoPtr<IComparator> CRawHeaders::FIELD_NAME_COMPARATOR = (IComparator*) new CRawHeaders::Comparator();

//==========================================================
//       CRawHeaders::Comparators
//==========================================================
CAR_INTERFACE_IMPL(CRawHeaders::Comparator, IComparator);

ECode CRawHeaders::Comparator::Compare(
    /* [in] */ IInterface* lhs,
    /* [in] */ IInterface* rhs,
    /* [out] */  Int32* result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<ICharSequence> cs = ICharSequence::Probe(lhs);
    String a, b;
    cs->ToString(&a);
    cs = ICharSequence::Probe(rhs);
    cs->ToString(&b);
    if (a.Equals(b)) {
        *result = 0;
    } else if (a.IsNull()) {
        *result = -1;
    } else if (b.IsNull()) {
        *result = 1;
    } else {
        *result = a.CompareIgnoreCase(b);
    }
    return NOERROR;
}

//==========================================================
//       CRawHeaders
//==========================================================
CRawHeaders::CRawHeaders()
    : mHttpMinorVersion(1)
    , mResponseCode(-1)
{
    CArrayList::New(20, (IList**)&mNamesAndValues);
}

ECode CRawHeaders::constructor()
{
   return NOERROR;
}

ECode CRawHeaders::constructor(
    /* [in] */ IRawHeaders* copyFrom)
{
    Boolean flag = FALSE;
    CRawHeaders* copyFromObj = (CRawHeaders*)copyFrom;
    mNamesAndValues->AddAll((ICollection*)(copyFromObj->mNamesAndValues), &flag);
    mStatusLine = copyFromObj->mStatusLine;
    mHttpMinorVersion = copyFromObj->mHttpMinorVersion;
    mResponseCode = copyFromObj->mResponseCode;
    mResponseMessage = copyFromObj->mResponseMessage;
    return NOERROR;
}

ECode CRawHeaders::SetStatusLine(
    /* [in] */ const String& statusLine)
{
    mStatusLine = statusLine.Trim();

    if (mStatusLine.IsNull() || !mStatusLine.StartWith("HTTP/")) {
        return NOERROR;
    }
    mStatusLine = mStatusLine.Trim();
    Int32 mark = mStatusLine.IndexOf(" ") + 1;
    if (mark == 0) {
        return NOERROR;
    }
    if (mStatusLine.GetChar(mark - 2) != '1') {
        mHttpMinorVersion = 0;
    }
    Int32 last = mark + 3;
    if (last > mStatusLine.GetLength()) {
        last = mStatusLine.GetLength();
    }
    mResponseCode = StringUtils::ParseInt32(mStatusLine.Substring(mark, last));
    if (last + 1 <= mStatusLine.GetLength()) {
        mResponseMessage = mStatusLine.Substring(last + 1);
    }
    return NOERROR;
}

ECode CRawHeaders::GetStatusLine(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    *str = mStatusLine;
    return NOERROR;
}

ECode CRawHeaders::GetHttpMinorVersion(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    *value = mHttpMinorVersion != -1 ? mHttpMinorVersion : 1;
    return NOERROR;
}

ECode CRawHeaders::GetResponseCode(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    *value = mResponseCode;
    return NOERROR;
}

ECode CRawHeaders::GetResponseMessage(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    *str = mResponseMessage;
    return NOERROR;
}

ECode CRawHeaders::AddLine(
    /* [in] */ const String& line)
{
    Int32 index = line.IndexOf(":");
    if (index == -1) {
        return Add(String(""), line);
    }
    else {
        return Add(line.Substring(0, index), line.Substring(index + 1));
    }
}

ECode CRawHeaders::Add(
    /* [in] */ const String& fieldName,
    /* [in] */ const String& value)
{
    if (fieldName.IsNull()) {
        // throw new IllegalArgumentException("fieldName == null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (value.IsNull()) {
        /*
         * Given null values, the RI sends a malformed field line like
         * "Accept\r\n". For platform compatibility and HTTP compliance, we
         * print a warning and ignore null values.
         */
        // System.logW("Ignoring HTTP header field '" + fieldName + "' because its value is null");
        return NOERROR;
    }
    AutoPtr<ICharSequence> csfiledName;
    CStringWrapper::New(fieldName, (ICharSequence**)&csfiledName);
    Boolean flag = FALSE;
    mNamesAndValues->Add(csfiledName, &flag);
    AutoPtr<ICharSequence> csValue;
    CStringWrapper::New(value.Trim(), (ICharSequence**)&csValue);
    return mNamesAndValues->Add(csValue, &flag);
}

ECode CRawHeaders::RemoveAll(
    /* [in] */ const String& fieldName)
{
    Int32 size;
    mNamesAndValues->GetSize(&size);
    for (Int32 i = 0; i < size; i += 2) {
        if (fieldName.EqualsIgnoreCase(GetStringByIndex(i))) {
            AutoPtr<IInterface> objFieldName;
            mNamesAndValues->Remove(i, (IInterface**)&objFieldName); // field name
            AutoPtr<IInterface> objValue;
            mNamesAndValues->Remove(i, (IInterface**)&objValue); // value
        }
    }
    return NOERROR;
}

ECode CRawHeaders::AddAll(
    /* [in] */ const String& fieldName,
    /* [in] */ IList* headerFields)
{
    AutoPtr< ArrayOf<IInterface*> > array;
    headerFields->ToArray((ArrayOf<IInterface*>**)&array);
    for (Int32 i = 0; i < array->GetLength(); i++) {
        String value;
        ICharSequence::Probe((*array)[i])->ToString(&value);
        Add(fieldName, value);
    }
    return NOERROR;
}

ECode CRawHeaders::Set(
    /* [in] */ const String& fieldName,
    /* [in] */ const String& value)
{
    RemoveAll(fieldName);
    return Add(fieldName, value);
}

ECode CRawHeaders::Length(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    Int32 size;
    mNamesAndValues->GetSize(&size);
    *value = size / 2;
    return NOERROR;
}

ECode CRawHeaders::GetFieldName(
    /* [in] */ Int32 index,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    Int32 fieldNameIndex = index * 2;
    Int32 size;
    mNamesAndValues->GetSize(&size);
    if (fieldNameIndex < 0 || fieldNameIndex >= size) {
        *str = NULL;
        return NOERROR;
    }
    *str = GetStringByIndex(fieldNameIndex);
    return NOERROR;
}

ECode CRawHeaders::GetValue(
    /* [in] */ Int32 index,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    Int32 valueIndex = index * 2 + 1;
    Int32 size;
    mNamesAndValues->GetSize(&size);
    if (valueIndex < 0 || valueIndex >= size) {
        *str = NULL;
        return NOERROR;
    }
    *str = GetStringByIndex(valueIndex);
    return NOERROR;
}

ECode CRawHeaders::Get(
    /* [in] */ const String& fieldName,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    Int32 size;
    mNamesAndValues->GetSize(&size);
    for (Int32 i = size - 2; i >= 0; i -= 2) {
        if (fieldName.EqualsIgnoreCase(GetStringByIndex(i))) {
            *str = GetStringByIndex(i + 1);
            return NOERROR;
        }
    }
    *str = NULL;
    return NOERROR;
}

ECode CRawHeaders::GetAll(
    /* [in] */ ISet* fieldNames,
    /* [out] */ IRawHeaders** rh)
{
    VALIDATE_NOT_NULL(rh)

    AutoPtr<CRawHeaders> result;
    FAIL_RETURN(CRawHeaders::NewByFriend((CRawHeaders**)&result));
    Int32 size;
    mNamesAndValues->GetSize(&size);
    Boolean flag = FALSE;
    for (Int32 i = 0; i < size; i += 2) {
        String fieldName = GetStringByIndex(i);
        AutoPtr<ICharSequence> charSeq;
        CStringWrapper::New(fieldName, (ICharSequence**)&charSeq);
        if ((fieldNames->Contains(charSeq, &flag), flag)) {
            result->Add(fieldName, GetStringByIndex(i + 1));
        }
    }
    *rh = (IRawHeaders*) result.Get();
    REFCOUNT_ADD(*rh)
    return NOERROR;
}

ECode CRawHeaders::ToHeaderString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    StringBuilder result(256);
    result.AppendString(mStatusLine);
    result.AppendString(String("\r\n"));
    Int32 size;
    mNamesAndValues->GetSize(&size);
    for (Int32 i = 0; i < size; i += 2) {
        result.AppendString(GetStringByIndex(i));
        result.AppendString(String(": "));
        result.AppendString(GetStringByIndex(i + 1));
        result.AppendString(String("\r\n"));
    }
    result.AppendString(String("\r\n"));
    return result.ToString(str);
}

ECode CRawHeaders::ToMultimap(
    /* [out] */ IMap** strmap)
{
    VALIDATE_NOT_NULL(strmap)
    AutoPtr<IMap> result;
    CTreeMap::New(FIELD_NAME_COMPARATOR, (IMap**)&result);
    Int32 size;
    mNamesAndValues->GetSize(&size);
    AutoPtr<ICollections> collectionsHelper;
    CCollections::AcquireSingleton((ICollections**)&collectionsHelper);
    Boolean flag = FALSE;
    for (Int32 i = 0; i < size; i += 2) {
        AutoPtr<IInterface> fieldName;
        mNamesAndValues->Get(i, (IInterface**)&fieldName);
        AutoPtr<IInterface> value;
        mNamesAndValues->Get(i + 1, (IInterface**)&value);

        AutoPtr<IList> allValues;
        CArrayList::New((IList**)&allValues);
        AutoPtr<IInterface> otherValues;
        result->Get(fieldName, (IInterface**)&otherValues);
        if (IList::Probe(otherValues) != NULL) {
            allValues->AddAll(ICollection::Probe(otherValues), &flag);
        }
        allValues->Add(value, &flag);
        AutoPtr<IList> list;
        collectionsHelper->NewUnmodifiableList(allValues, (IList**)&list);
        AutoPtr<IInterface> outValue;
        result->Put(fieldName, list, (IInterface**)&outValue);
    }

    if (!mStatusLine.IsNull()) {
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(mStatusLine, (ICharSequence**)&cs);
        AutoPtr<IList> llist;
        collectionsHelper->NewSingletonList(cs, (IList**)&llist);
        AutoPtr<IList> rlist;
        collectionsHelper->NewUnmodifiableList(llist, (IList**)&rlist);
        AutoPtr<IInterface> outinterface;
        result->Put(NULL, rlist, (IInterface**)&outinterface);
    }
    return collectionsHelper->NewUnmodifiableMap(result, strmap);
}

ECode CRawHeaders::FromMultimap(
    /* [in] */ IMap* map,
    /* [out] */ IRawHeaders** rh)
{
    VALIDATE_NOT_NULL(rh)
    AutoPtr<IRawHeaders> result;
    CRawHeaders::New((IRawHeaders**)&result);
    AutoPtr<ISet> entries;
    map->EntrySet((ISet**)&entries);
    AutoPtr<ArrayOf<IInterface*> > array;
    ICollection::Probe(entries)->ToArray((ArrayOf<IInterface*>**)&array);
    Boolean flag = FALSE;
    for (Int32 i = 0; i < array->GetLength(); ++i) {
        AutoPtr<IMapEntry> entry = IMapEntry::Probe((*array)[i]);
        AutoPtr<IInterface> _fieldName;
        entry->GetKey((IInterface**)&_fieldName);
        String fieldName;
        ICharSequence::Probe(_fieldName)->ToString(&fieldName);

        AutoPtr<IInterface> _values;
        entry->GetValue((IInterface**)&_values);
        AutoPtr<IList> values = IList::Probe(_values);

        if (!fieldName.IsNull()) {
            result->AddAll(fieldName, values);
        } else if ((values->IsEmpty(&flag), !flag)) {
            Int32 size;
            values->GetSize(&size);
            AutoPtr<IInterface> value;
            values->Get(size - 1, (IInterface**)&value);
            String str;
            ICharSequence::Probe(value)->ToString(&str);
            result->SetStatusLine(str);
        }
    }
    *rh = result;
    REFCOUNT_ADD(*rh);
    return NOERROR;
}

String CRawHeaders::GetStringByIndex(
    /* [in] */ Int32 index)
{
    AutoPtr<IInterface> element;
    mNamesAndValues->Get(index, (IInterface**)&element);
    String retStr;
    ICharSequence::Probe(element)->ToString(&retStr);
    return retStr;
}

} // namespace Http
} // namespace Net
} // namespace Elastos
