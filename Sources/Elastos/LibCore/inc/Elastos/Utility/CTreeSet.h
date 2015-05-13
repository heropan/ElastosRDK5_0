#ifndef __UTILITY_CTREESET_H__
#define __UTILITY_CTREESET_H__

#include "_CTreeSet.h"
#include "AbstractSet.h"

using Elastos::IO::IObjectInputStream;
using Elastos::IO::IObjectOutputStream;
using Elastos::IO::IObjectStreamField;
using Elastos::Core::IComparator;

namespace Elastos {
namespace Utility {

CarClass(CTreeSet) , public AbstractSet
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ INavigableMap* navmap);

    CARAPI constructor(
        /* [in] */ ICollection* map);

    CARAPI constructor(
        /* [in] */ IComparator* comparator);

    CARAPI constructor(
        /* [in] */ ISortedSet* set);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI Add(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* modified);

    CARAPI AddAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* modified);

    CARAPI Clear();

    CARAPI Contains(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI ContainsAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* result);

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI IsEmpty(
        /* [out] */ Boolean* result);

    CARAPI Remove(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* modified);

    CARAPI RemoveAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* modified);

    CARAPI RetainAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* modified);

    CARAPI GetSize(
        /* [out] */ Int32* size);

    CARAPI ToArray(
        /* [out, callee] */ ArrayOf<IInterface*>** array);

    CARAPI ToArrayEx(
        /* [in] */ ArrayOf<IInterface*>* inArray,
        /* [out, callee] */ ArrayOf<IInterface*>** outArray);

    CARAPI GetIterator(
        /* [out] */ IIterator** it);

    CARAPI Lower(
        /* [in] */ IInterface* e,
        /* [out] */ IInterface** outface);

    CARAPI Floor(
        /* [in] */ IInterface* e,
        /* [out] */ IInterface** outface);

    CARAPI Ceiling(
        /* [in] */ IInterface* e,
        /* [out] */ IInterface** outface);

    CARAPI Higher(
        /* [in] */ IInterface* e,
        /* [out] */ IInterface** outface);

    CARAPI PollFirst(
        /* [out] */ IInterface** outface);

    CARAPI PollLast(
        /* [out] */ IInterface** outface);

    CARAPI DescendingSet(
        /* [out] */ INavigableSet** outnav);

    CARAPI DescendingIterator(
        /* [out] */ IIterator** outiter);

    CARAPI SubSetEx(
        /* [in] */ IInterface* fromElement,
        /* [in] */ Boolean fromInclusive,
        /* [in] */ IInterface* toElement,
        /* [in] */ Boolean toInclusive,
        /* [out] */ INavigableSet** outnav);

    CARAPI HeadSetEx(
        /* [in] */ IInterface* toElement,
        /* [in] */ Boolean inclusive,
        /* [out] */ INavigableSet** outnav);

    CARAPI TailSetEx(
        /* [in] */ IInterface* fromElement,
        /* [in] */ Boolean inclusive,
        /* [out] */ INavigableSet** outnav);

    CARAPI Comparator(
        /* [out] */ IComparator** outcom);

    CARAPI First(
        /* [out] */ IInterface** outface);

    CARAPI HeadSet(
        /* [in] */ IInterface* end,
        /* [out] */ ISortedSet** outsort);

    CARAPI Last(
        /* [out] */ IInterface** outface);

    CARAPI SubSet(
        /* [in] */ IInterface* start,
        /* [in] */ IInterface* end,
        /* [out] */ ISortedSet** outsort);

    CARAPI TailSet(
        /* [in] */ IInterface* start,
        /* [out] */ ISortedSet** outsort);

    CARAPI Clone(
        /* [out] */ IInterface** object);

private:
    CARAPI_(void) WriteObject(
        /* [in] */ IObjectOutputStream* stream);

    CARAPI_(void) ReadObject(
        /* [in] */ IObjectInputStream* stream);

private:
    /** Keys are this set's elements. Values are always Boolean.TRUE */
    /* transient */ AutoPtr<INavigableMap> mBackingMap;

    /* transient */ AutoPtr<INavigableSet> mDescendingSet;
};

} // namespace Utility
} // namespace Elastos

#endif // __UTILITY_CTREESET_H__
