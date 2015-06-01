
#include "AttributedString.h"
#include <elastos/StringBuffer.h>
#include <elastos/Map.h>
#include <CObjectContainer.h>

using Elastos::Core::CObjectContainer;
using Elastos::Core::StringBuffer;
using Elastos::Utility::Pair;

namespace Elastos {
namespace Text {

// const RBTreeColorType S_RBTreeRed = FALSE;
// const RBTreeColorType S_RBTreeBlack = TRUE;

AttributedString::Range::Range(
    /* [in] */ Int32 s,
    /* [in] */ Int32 e,
    /* [in] */ IInterface* v)
    : mStart(s)
    , mEnd(e)
    , mValue(v)
{}

AttributedString::Range& AttributedString::Range::operator = (
    /* [in] */ const AttributedString::Range& other)
{
    mStart = other.mStart;
    mEnd = other.mEnd;
    mValue = other.mValue;
    return *this;
}

AttributedString::AttributedIterator::AttributedIterator(
    /* [in] */ AttributedString* attrString)
    : mBegin(0)
    , mOffset(0)
    , mAttrString(attrString)
{
    mEnd = attrString->mText.GetLength();
}

AttributedString::AttributedIterator::AttributedIterator(
    /* [in] */ AttributedString* attrString,
    /* [in] */ ArrayOf<IAttributedCharacterIteratorAttribute*>* attributes,
    /* [in] */ Int32 begin,
    /* [in] */ Int32 end)
{
    if (begin < 0 || end > (Int32)mAttrString->mText.GetLength() || begin > end) {
        //throw new IllegalArgumentException();
        assert(0);
    }
    mBegin = begin;
    mEnd = end;
    mOffset = begin;
    mAttrString = attrString;

    if (attributes != NULL) {
        Int32 len = (Int32)(attributes->GetLength() * 4 / 3) + 1;
        mAttributesAllowed.Resize(len);
        for (Int32 i = attributes->GetLength(); --i >= 0;) {
            mAttributesAllowed.Insert((*attributes)[i]);
        }
    }
}

AttributedString::AttributedIterator::~AttributedIterator()
{
    mAttributesAllowed.Clear();
}

PInterface AttributedString::AttributedIterator::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IAttributedCharacterIterator) {
        return (IAttributedCharacterIterator*)this;
    }

    return NULL;
}

UInt32 AttributedString::AttributedIterator::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AttributedString::AttributedIterator::Release()
{
    return ElRefBase::Release();
}

AttributedString::AttributedIterator::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode AttributedString::AttributedIterator::Clone(
    /* [out] */ ICharacterIterator** copy)
{
    // try {
    //     AttributedIterator clone = (AttributedIterator) super.clone();
    //     if (attributesAllowed != null) {
    //         clone.attributesAllowed = (HashSet<Attribute>) attributesAllowed
    //                 .clone();
    //     }
    //     return clone;
    // } catch (CloneNotSupportedException e) {
    //     throw new AssertionError(e);
    // }
    return E_NOT_IMPLEMENTED;
}

ECode AttributedString::AttributedIterator::Current(
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);
    if (mOffset == mEnd) {
        *value = (Char32)ICharacterIterator::DONE;
        return NOERROR;
    }
    *value = mAttrString->mText.GetChar(mOffset);
    return NOERROR;
}

ECode AttributedString::AttributedIterator::First(
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);
    if (mBegin == mEnd) {
        *value = (Char32)ICharacterIterator::DONE;
        return NOERROR;
    }
    mOffset = mBegin;
    *value = mAttrString->mText.GetChar(mOffset);
    return NOERROR;
}

ECode AttributedString::AttributedIterator::GetBeginIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = mBegin;
    return NOERROR;
}

ECode AttributedString::AttributedIterator::GetEndIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = mEnd;
    return NOERROR;
}

ECode AttributedString::AttributedIterator::GetIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = mOffset;
    return NOERROR;
}

Boolean AttributedString::AttributedIterator::InRange(
    /* [in] */ Range* range)
{
    if (range->mValue->Probe(EIID_IAnnotation) == NULL) {
        return TRUE;
    }
    return range->mStart >= mBegin && range->mStart < mEnd
            && range->mEnd > mBegin && range->mEnd <= mEnd;
}

Boolean AttributedString::AttributedIterator::InRange(
    /* [in] */ List<AutoPtr<Range> >* ranges)
{
    List<AutoPtr<Range> >::Iterator it;
    for (it = ranges->Begin(); it != ranges->End(); ++it) {
        Range* range = *it;
        if (range->mStart >= mBegin && range->mStart < mEnd) {
            return (range->mValue->Probe(EIID_IAnnotation) == NULL)
                    || (range->mEnd > mBegin && range->mEnd <= mEnd);
        }
        else if (range->mEnd > mBegin && range->mEnd <= mEnd) {
            return (range->mValue->Probe(EIID_IAnnotation) == NULL)
                    || (range->mStart >= mBegin && range->mStart < mEnd);
        }
    }
    return FALSE;
}

ECode AttributedString::AttributedIterator::GetAllAttributeKeys(
    /* [out] */ IObjectContainer** allAttributedKeys)
{
    VALIDATE_NOT_NULL(allAttributedKeys);

    AutoPtr<IObjectContainer> container;
    CObjectContainer::New((IObjectContainer**)&container);
    *allAttributedKeys = container;
    REFCOUNT_ADD(*allAttributedKeys);

    AttributeRangeMapIterator it = mAttrString->mAttributeMap.Begin();
    if (mBegin == 0 && mEnd == (Int32)mAttrString->mText.GetLength()
            && mAttributesAllowed.IsEmpty()) {
        for(; it != mAttrString->mAttributeMap.End(); ++it) {
            container->Add(it->mFirst);
        }
        return NOERROR;
    }

    for(; it != mAttrString->mAttributeMap.End(); ++it) {
        if (mAttributesAllowed.Find(it->mFirst) != mAttributesAllowed.End()) {
            AutoPtr<List<AutoPtr<Range> > > ranges = it->mSecond;
            if (InRange(ranges)) {
                container->Add(it->mFirst);
            }
        }
    }

    return NOERROR;
}

AutoPtr<IInterface> AttributedString::AttributedIterator::CurrentValue(
    /* [in] */ List<AutoPtr<Range> >* ranges)
{
    List<AutoPtr<Range> >::Iterator it = ranges->Begin();
    for (it = ranges->Begin(); it != ranges->End(); ++it) {
        AutoPtr<Range> range = *it;
        if (mOffset >= range->mStart && mOffset < range->mEnd) {
            return InRange(range) ? range->mValue : NULL;
        }
    }

    return NULL;
}

ECode AttributedString::AttributedIterator::GetAttribute(
    /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
    /* [out] */ IInterface** instance)
{
    VALIDATE_NOT_NULL(instance);

    if (mAttributesAllowed.Find(attribute) == mAttributesAllowed.End()) {
        *instance = NULL;
        return NOERROR;
    }
    AutoPtr<List<AutoPtr<Range> > > ranges;
    AttributeRangeMapIterator it = mAttrString->mAttributeMap.Find(attribute);
    if (it == mAttrString->mAttributeMap.End()) {
        *instance = NULL;
        return NOERROR;
    }
    ranges = it->mSecond;
    AutoPtr<IInterface> temp = CurrentValue(ranges);
    *instance = temp;
    REFCOUNT_ADD(*instance);
    return NOERROR;
}

ECode AttributedString::AttributedIterator::GetAttributes(
    /* [out, callee] */ AttributeObjectMap** attributes)
{
    VALIDATE_NOT_NULL(attributes);

    AutoPtr<AttributeObjectMap> result =
        new AttributeObjectMap((mAttrString->mAttributeMap.GetSize() * 4 / 3) + 1);

    AttributeRangeMapIterator it = mAttrString->mAttributeMap.Begin();
    AttributeRangeMapIterator itnext = ++mAttrString->mAttributeMap.Begin();
    for(; it != mAttrString->mAttributeMap.End(); ++it,++itnext) {
        if (mAttributesAllowed.IsEmpty()
            || mAttributesAllowed.Find(itnext->mFirst) != mAttributesAllowed.End()) {
            AutoPtr<IInterface> value = CurrentValue(itnext->mSecond);
            if (value != NULL) {
                result->Insert(Pair<AutoPtr<IAttributedCharacterIteratorAttribute>, AutoPtr<IInterface> >(itnext->mFirst, value));
            }
        }
    }

    *attributes = result;
    REFCOUNT_ADD(*attributes);
    return NOERROR;
}

ECode AttributedString::AttributedIterator::GetRunLimit(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    AutoPtr<IObjectContainer> allAttributedKeys;
    GetAllAttributeKeys((IObjectContainer**)&allAttributedKeys);
    return GetRunLimit(allAttributedKeys, index);
}

Int32 AttributedString::AttributedIterator::RunLimit(
    /* [in] */ List<AutoPtr<Range> >* ranges)
{
    Int32 result = mEnd;

    List<AutoPtr<Range> >::ReverseIterator rit = ranges->RBegin();
    for (rit = ranges->RBegin(); rit != ranges->REnd(); ++rit) {
        AutoPtr<Range> range = *rit;
        if (range->mEnd <= mBegin) {
            break;
        }
        if (mOffset >= range->mStart && mOffset < range->mEnd) {
            return InRange(range) ? range->mEnd : result;
        }
        else if (mOffset >= range->mEnd) {
            break;
        }
        result = range->mStart;
    }

    return result;
}

ECode AttributedString::AttributedIterator::GetRunLimit(
    /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    if ((!mAttributesAllowed.IsEmpty()) && (mAttributesAllowed.Find(attribute) == mAttributesAllowed.End())) {
        *index = mEnd;
        return NOERROR;
    }

    AttributeRangeMapIterator it = mAttrString->mAttributeMap.Find(attribute);
    if (it == mAttrString->mAttributeMap.End()) {
        *index = mEnd;
        return NOERROR;
    }

    AutoPtr<List<AutoPtr<Range> > > ranges = it->mSecond;
    *index = RunLimit(ranges);

    return NOERROR;

}

ECode AttributedString::AttributedIterator::GetRunLimit(
    /* [in] */ IObjectContainer* attributes,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    Int32 limit = mEnd;
    AutoPtr<IObjectEnumerator> enumerator;
    attributes->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Boolean hasNext;
    while(enumerator->MoveNext(&hasNext), hasNext) {
        AutoPtr<IAttributedCharacterIteratorAttribute> attr;
        enumerator->Current((IInterface**)&attr);
        Int32 newLimit;
        GetRunLimit(attr, &newLimit);
        if (newLimit < limit) {
            limit = newLimit;
        }
    }

    *index = limit;
    return NOERROR;
}

ECode AttributedString::AttributedIterator::GetRunStart(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    AutoPtr<IObjectContainer> allAttributedKeys;
    GetAllAttributeKeys((IObjectContainer**)&allAttributedKeys);
    return GetRunStart(allAttributedKeys, index);
}

Int32 AttributedString::AttributedIterator::RunStart(
    /* [in] */ List<AutoPtr<Range> >* ranges)
{
    Int32 result = mBegin;

    List<AutoPtr<Range> >::Iterator it;
    for (it = ranges->Begin(); it != ranges->End(); ++it) {
        AutoPtr<Range> range = *it;
        if (range->mStart >= mEnd) {
            break;
        }
        if (mOffset >= range->mStart && mOffset < range->mEnd) {
            return InRange(range) ? range->mStart : result;
        }
        else if (mOffset < range->mStart) {
            break;
        }
        result = range->mEnd;
    }

    return result;
}

ECode AttributedString::AttributedIterator::GetRunStart(
    /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    if (mAttributesAllowed.Find(attribute) == mAttributesAllowed.End()) {
        *index = mBegin;
        return NOERROR;
    }
    AttributeRangeMapIterator it = mAttrString->mAttributeMap.Find(attribute);
    if (it == mAttrString->mAttributeMap.End()) {
        *index = mBegin;
        return NOERROR;
    }

    AutoPtr<List<AutoPtr<Range> > > ranges = it->mSecond;
    *index = RunStart(ranges);
    return NOERROR;
}

ECode AttributedString::AttributedIterator::GetRunStart(
    /* [in] */ IObjectContainer* attributes,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    Int32 start = mBegin;
    AutoPtr<IObjectEnumerator> enumerator;
    attributes->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Boolean hasNext;
    while(enumerator->MoveNext(&hasNext), hasNext) {
        AutoPtr<IAttributedCharacterIteratorAttribute> attr;
        enumerator->Current((IInterface**)&attr);
        Int32 newStart;
        GetRunStart(attr, &newStart);
        if (newStart > start) {
            start = newStart;
        }
    }

    *index = start;
    return NOERROR;
}

ECode AttributedString::AttributedIterator::Last(
    /* [out] */ Char32* lastValue)
{
    VALIDATE_NOT_NULL(lastValue);
    if (mBegin == mEnd) {
        *lastValue = (Char32)ICharacterIterator::DONE;
        return NOERROR;
    }
    mOffset = mEnd - 1;
    *lastValue = mAttrString->mText.GetChar(mOffset);
    return NOERROR;
}

ECode AttributedString::AttributedIterator::Next(
    /* [out] */ Char32* nextValue)
{
    VALIDATE_NOT_NULL(nextValue);
    if (mOffset >= (mEnd - 1)) {
        mOffset = mEnd;
        *nextValue = (Char32)ICharacterIterator::DONE;
        return NOERROR;
    }
    *nextValue = mAttrString->mText.GetChar(++mOffset);
    return NOERROR;
}

ECode AttributedString::AttributedIterator::Previous(
    /* [out] */ Char32* previousValue)
{
    VALIDATE_NOT_NULL(previousValue);
    if (mOffset == mBegin) {
        *previousValue = (Char32)ICharacterIterator::DONE;
        return NOERROR;
    }

    *previousValue = mAttrString->mText.GetChar(--mOffset);
    return NOERROR;
}

ECode AttributedString::AttributedIterator::SetIndex(
    /* [in] */ Int32 location,
    /* [out] */ Char32* newChar)
{
    VALIDATE_NOT_NULL(newChar);

    if (location < mBegin || location > mEnd) {
        //throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mOffset = location;
    if (mOffset == mEnd) {
        *newChar = (Char32)ICharacterIterator::DONE;
        return NOERROR;
    }

    *newChar = mAttrString->mText.GetChar(mOffset);
    return NOERROR;
}

AttributedString::~AttributedString()
{
    mAttributeMap.Clear();
}

ECode AttributedString::Init(
    /* [in] */ IAttributedCharacterIterator* iterator)
{
    Int32 bi, ei;
    iterator->GetBeginIndex(&bi);
    iterator->GetEndIndex(&ei);
    if (bi > ei) {
        //throw new IllegalArgumentException("Invalid substring range");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    StringBuffer buffer;
    for (Int32 i = bi; i < ei; i++) {
        Char32 cv, nv;
        iterator->Current(&cv);
        buffer += cv;
        iterator->Next(&nv);
    }
    buffer.ToString(&mText);

    AutoPtr<IObjectContainer> attributes;
    iterator->GetAllAttributeKeys((IObjectContainer**)&attributes);
    if (attributes == NULL) {
        return NOERROR;
    }

    AutoPtr<IObjectEnumerator> enumerator;
    attributes->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Boolean hasNext;
    while(enumerator->MoveNext(&hasNext), hasNext) {
        AutoPtr<IAttributedCharacterIteratorAttribute> attr;
        enumerator->Current((IInterface**)&attr);
        Char32 ch;
        iterator->SetIndex(0, &ch);
        Char32 cv;
        while (iterator->Current(&cv), cv != (Char32)ICharacterIterator::DONE) {
            Int32 start;
            iterator->GetRunStart(attr, &start);
            Int32 limit;
            iterator->GetRunLimit(attr, &limit);
            AutoPtr<IInterface> value;
            iterator->GetAttribute(attr, (IInterface**)&value);
            if (value != NULL) {
                AddAttribute(attr, value, start, limit);
            }
            iterator->SetIndex(limit, &ch);
        }
    }

    return NOERROR;
}

ECode AttributedString::Init(
    /* [in] */ IAttributedCharacterIterator* iterator,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IObjectContainer* attributes)
{
    Int32 beginIndex, endIndex;
    iterator->GetBeginIndex(&beginIndex);
    iterator->GetEndIndex(&endIndex);
    if (start < beginIndex || end > endIndex || start > end) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException();
    }

    if (attributes == NULL) {
        return NOERROR;
    }

    StringBuffer buffer;
    Char32 newIndex;
    iterator->SetIndex(start, &newIndex);
    Int32 index;
    while (iterator->GetIndex(&index) ,index < end) {
        Char32 c;
        iterator->Current(&c);
        buffer += c;
        Char32 nextC;
        iterator->Next(&nextC);
    }
    buffer.ToString(&mText);

    AutoPtr<IObjectEnumerator> enumerator;
    attributes->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Boolean hasNext;
    while(enumerator->MoveNext(&hasNext), hasNext) {
        AutoPtr<IAttributedCharacterIteratorAttribute> attr;
        enumerator->Current((IInterface**)&attr);
        Char32 c;
        iterator->SetIndex(start, &c);
        Int32 id;
        while (iterator->GetIndex(&id) ,index < end) {
            AutoPtr<IInterface> value;
            iterator->GetAttribute(attr, (IInterface**)&value);
            Int32 runStart;
            iterator->GetRunStart(attr, &runStart);
            Int32 limit;
            iterator->GetRunLimit(attr, &limit);
            if ((value->Probe(EIID_IAnnotation) != NULL
                    && runStart >= start && limit <= end)
                        || (value != NULL && (value->Probe(EIID_IAnnotation) == NULL))) {
                AddAttribute(attr, value, (runStart < start ? start
                        : runStart)
                        - start, (limit > end ? end : limit) - start);
            }
            Char32 newChar;
            iterator->SetIndex(limit, &newChar);
        }
    }

    return NOERROR;
}

ECode AttributedString::Init(
    /* [in] */ IAttributedCharacterIterator* iterator,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    AutoPtr<IObjectContainer> container;
    iterator->GetAllAttributeKeys((IObjectContainer**)&container);
    return Init(iterator, start, end, container);
}

ECode AttributedString::Init(
    /* [in] */ IAttributedCharacterIterator* iterator,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ArrayOf<IAttributedCharacterIteratorAttribute*>* attributes)
{
    AutoPtr<IObjectContainer> container;
    // CObjectContainer::New((IObjectContainer**)&container);
    if (attributes != NULL) {
        for (Int32 i = 0; i < attributes->GetLength(); ++i) {
            container->Add((*attributes)[i]);
        }
    }
    return Init(iterator, start, end, container);
}

ECode AttributedString::Init(
    /* [in] */ const String& value)
{
    if (value.IsNull()) {
        //throw new NullPointerException("value == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    mText = value;
    return NOERROR;
}

// ECode AttributedString::Init(
//     /* [in] */ const String& value,
//     /* [in] */ IObjectMap * attributes)
// {
//     if (value.IsNull()) {
//         // throw new NullPointerException("value == null");
//         return E_NULL_POINTER_EXCEPTION;
//     }
//     // if (value.GetLength() == 0 && !attributes->IsEmpty()) {
//     //     throw new IllegalArgumentException("Cannot add attributes to empty string");
//     // }
//     Int32 len(0);
//     attributes->GetSize(&len);
//     mAttributeMap = new AttributeRangeMap(
//             (len * 4 / 3) + 1);
//     // Iterator<?> it = attributes.entrySet().iterator();
//     // while (it.hasNext()) {
//     //     Map.Entry<?, ?> entry = (Map.Entry<?, ?>) it.next();
//     //     ArrayList<Range> ranges = new ArrayList<Range>(1);
//     //     ranges.add(new Range(0, value.GetLength(), entry.getValue()));
//     //     attributeMap.put((AttributedCharacterIterator.Attribute) entry
//     //             .getKey(), ranges);
//     // }
//     return E_NOT_IMPLEMENTED;
// }

ECode AttributedString::AddAttribute(
    /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
    /* [in] */ IInterface* value)
{
    if (attribute == NULL) {
        //throw new NullPointerException("attribute == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    if (mText.GetByteLength() == 0) {
        //throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<List<AutoPtr<Range> > > ranges;
    AttributeRangeMapIterator it = mAttributeMap.Find(attribute);
    if (it == mAttributeMap.End()) {
        ranges = new List<AutoPtr<Range> >(0);
        mAttributeMap[attribute] = ranges;
    }
    else {
        ranges = it->mSecond;
        ranges->Clear();
    }
    AutoPtr<Range> range = new Range(0, mText.GetLength(), value);
    ranges->PushBack(range);
    return NOERROR;
}

ECode AttributedString::AddAttribute(
    /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
    /* [in] */ IInterface* value,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (attribute == NULL) {
        //throw new NullPointerException("attribute == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    if (start < 0 || end > (Int32)mText.GetLength() || start >= end) {
        //throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (value == NULL) {
        return NOERROR;
    }

    AutoPtr<List<AutoPtr<Range> > > ranges;
    AttributeRangeMapIterator mapIt = mAttributeMap.Find(attribute);
    if (mapIt == mAttributeMap.End()) {
        ranges = new List<AutoPtr<Range> >(0);
        AutoPtr<Range> range = new Range(start, end, value);
        ranges->PushBack(range);
        mAttributeMap[attribute] = ranges;
        return NOERROR;
    }
    else {
        ranges = mapIt->mSecond;
    }

    List<AutoPtr<Range> >::Iterator it;
    for (it = ranges->Begin(); it != ranges->End(); ++it) {
        Range& range = **it;
        if (end <= range.mStart) {
            break;
        }
        else if (start < range.mEnd
                || (start == range.mEnd && value == range.mValue)) {
            it = ranges->Erase(it);
            AutoPtr<Range> r1 = new Range(range.mStart, start, range.mValue);
            AutoPtr<Range> r3 = new Range(end, range.mEnd, range.mValue);

            while (end > range.mEnd && (++it) != ranges->End()) {
                range = **it;
                if (end <= range.mEnd) {
                    if (end > range.mStart
                            || (end == range.mStart && value == range.mValue)) {
                        it = ranges->Erase(it);
                        r3 = new Range(end, range.mEnd, range.mValue);
                        break;
                    }
                }
                else {
                    it = ranges->Erase(it);
                }
            }

            if (value == r1->mValue) {
                if (value == r3->mValue) {
                    AutoPtr<Range> r = new Range(r1->mStart < start ? r1->mStart : start,
                            r3->mEnd > end ? r3->mEnd : end, r1->mValue);
                    ranges->Insert(it, r);
                }
                else {
                    AutoPtr<Range> r = new Range(r1->mStart < start ? r1->mStart : start,
                            end, r1->mValue);
                    it = ranges->Insert(it, r);
                    if (r3->mStart < r3->mEnd) {
                        ranges->Insert(it, r3);
                    }
                }
            }
            else {
                if (value == r3->mValue) {
                    if (r1->mStart < r1->mEnd) {
                        it = ranges->Insert(it, r1);
                    }
                    AutoPtr<Range> r = new Range(start, r3->mEnd > end ? r3->mEnd : end,
                            r3->mValue);
                    ranges->Insert(it, r);
                }
                else {
                    if (r1->mStart < r1->mEnd) {
                        ranges->Insert(it, r1);
                    }
                    AutoPtr<Range> r = new Range(start, end, value);
                    it = ranges->Insert(it, r);
                    if (r3->mStart < r3->mEnd) {
                        ranges->Insert(it, r3);
                    }
                }
            }
            return NOERROR;
        }
    }

    AutoPtr<Range> r = new Range(start, end, value);
    ranges->Insert(it, r);
    return NOERROR;
}

ECode AttributedString::AddAttributes(
    /* [in] */ AttributeObjectMap* attributes,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    AttributeObjectMapIterator itnext = attributes->Begin();
    while (itnext != attributes->End()) {
        AddAttribute( itnext->mFirst, itnext->mSecond, start, end);
        ++attributes;
    }
    return NOERROR;
}

ECode AttributedString::GetIterator(
    /* [out] */ IAttributedCharacterIterator** iterator)
{
    VALIDATE_NOT_NULL(iterator);
    *iterator = (IAttributedCharacterIterator*) new AttributedIterator(this);
    REFCOUNT_ADD(*iterator);
    return NOERROR;
}

ECode AttributedString::GetIterator(
    /* [in] */ ArrayOf<IAttributedCharacterIteratorAttribute*>* attributes,
    /* [out] */ IAttributedCharacterIterator** iterator)
{
    VALIDATE_NOT_NULL(iterator);
    *iterator = (IAttributedCharacterIterator*)new AttributedIterator(
            this, attributes, 0, mText.GetLength());
    REFCOUNT_ADD(*iterator);
    return NOERROR;
}

ECode AttributedString::GetIterator(
    /* [in] */ ArrayOf<IAttributedCharacterIteratorAttribute*>* attributes,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IAttributedCharacterIterator** iterator)
{
    VALIDATE_NOT_NULL(iterator);
    *iterator = (IAttributedCharacterIterator*)new AttributedIterator(
            this, attributes, start, end);
    REFCOUNT_ADD(*iterator);
    return NOERROR;
}

} // namespace Text
} // namespace Elastos
